#include "freertos_demo.h"
#include "stdio.h"
#include "string.h"
//freertos相关的文件（必需的）
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"//事件标志组的头文件
//需要用到的其他文件
#include "LED.h"
#include "usart.h"
#include "key.h"

//启动任务相关参数设置
#define START_TASK_STACK_SIZE  128         //启动任务栈大小
#define START_TASK_PRIORITY    1           //启动任务优先级
TaskHandle_t start_task_handle;
void start_task(void *pvParameters);

//任务1相关参数设置
#define TASK1_STACK_SIZE  128         //任务1栈大小
#define TASK1_PRIORITY    2           //任务1优先级
TaskHandle_t task1_handle;
void task1(void *pvParameters);

//任务2相关参数设置
#define TASK2_STACK_SIZE  128         //任务2栈大小
#define TASK2_PRIORITY    3           //任务2优先级
TaskHandle_t task2_handle;
void task2(void *pvParameters);


EventGroupHandle_t event_group_handle;

//启动FreeRTOS
void freertos_start(void)
{
    //创建事件标志组句柄
    event_group_handle = xEventGroupCreate();
    if (event_group_handle != NULL)
    {
        HAL_UART_Transmit(&huart1,"创建事件标志组成功\r\n",sizeof("创建事件标志组成功\r\n"),1000);
    }
    
    //1.创建启动任务
    xTaskCreate((TaskFunction_t) start_task,//任务函数地址
                             (char *)  "start_task", //任务名字符串（别名）
                             (configSTACK_DEPTH_TYPE) START_TASK_STACK_SIZE,//任务栈大小
                            (void *) NULL,//传递给任务的参数
                            (UBaseType_t) START_TASK_PRIORITY,//任务优先级
                            (TaskHandle_t *) &start_task_handle);//任务句柄地址
    //2.启动调度器(会自动创建空闲任务和定时器任务)
    vTaskStartScheduler();
}

void start_task(void *pvParameters)
{
    //进入临界区：保护临界区的代码不会被打断
    taskENTER_CRITICAL();                       //临界区很重要

    //1.创建任务1
    xTaskCreate((TaskFunction_t) task1,//任务函数地址
                             (char *)  "task1", //任务名字符串（别名）
                             (configSTACK_DEPTH_TYPE) TASK1_STACK_SIZE,//任务栈大小
                            (void *) NULL,//传递给任务的参数
                            (UBaseType_t) TASK1_PRIORITY,//任务优先级
                            (TaskHandle_t *) &task1_handle);//任务句柄地址

    //2.创建任务2
    xTaskCreate((TaskFunction_t) task2,//任务函数地址
                             (char *)  "task2", //任务名字符串（别名）
                             (configSTACK_DEPTH_TYPE) TASK2_STACK_SIZE,//任务栈大小
                            (void *) NULL,//传递给任务的参数
                            (UBaseType_t) TASK2_PRIORITY,//任务优先级
                            (TaskHandle_t *) &task2_handle);//任务句柄地址
   
	
    
    //删除启动任务,删除自身
    vTaskDelete(NULL);
							
	//退出临界区
	taskEXIT_CRITICAL();
    
}
#define EVENTBIT_0 (1<<0)
#define EVENTBIT_1 (1<<1)
//任务1函数:读取按键按下的键值，根据不同值将事件标志组对应位置1，模拟事件发生
void task1(void *pvParameters)
{
    uint8_t key = 0;
    while(1)
    {
        key = Key_Detect();
       if(key == KEY1_PRESS)
       {
        //对位0置1
        xEventGroupSetBits(event_group_handle,EVENTBIT_0);
        HAL_UART_Transmit(&huart1,"按下key1,对位0置1\r\n",sizeof("按下key1,对位0置1\r\n"),1000);
       }
        if(key == KEY2_PRESS)
       {
        //对位1置1
        xEventGroupSetBits(event_group_handle,EVENTBIT_1);
        HAL_UART_Transmit(&huart1,"按下key2,对位1置1\r\n",sizeof("按下key2,对位1置1\r\n"),1000);
       }

        vTaskDelay(500);
    }
}
char msg[50];
//任务2函数：同时等待事件标志组中多个事件位，当这些事件位都置1的话就进行对应处理
void task2(void *pvParameters)
{
    EventBits_t event_bits = 0;
    while(1)
    {
        event_bits = xEventGroupWaitBits(event_group_handle,         //事件标志组句柄
                            EVENTBIT_0 | EVENTBIT_1,    //要等待的标志位
                            pdTRUE,                     //满足等待条件后，是否将对应bit位清零
                            pdTRUE,                     //所有bit位都是1，还是部分是1
                            portMAX_DELAY);             //等待时间
        sprintf(msg,"task2接收到的事件标志组=%#x....\r\n",event_bits);
        HAL_UART_Transmit(&huart1,(uint8_t *)msg,strlen(msg),1000);    
    }
}

