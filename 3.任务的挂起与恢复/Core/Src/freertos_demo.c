#include "freertos_demo.h"
#include <string.h>

//freertos相关的文件（必需的）
#include "FreeRTOS.h"
#include "task.h"

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

//任务3相关参数设置
#define TASK3_STACK_SIZE  128         //任务3栈大小
#define TASK3_PRIORITY    4           //任务3优先级
TaskHandle_t task3_handle;
void task3(void *pvParameters);



//启动FreeRTOS
void freertos_start(void)
{
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
    //3.创建任务3
    xTaskCreate((TaskFunction_t) task3,//任务函数地址
                             (char *)  "task3", //任务名字符串（别名）
                             (configSTACK_DEPTH_TYPE) TASK3_STACK_SIZE,//任务栈大小
                            (void *) NULL,//传递给任务的参数
                            (UBaseType_t) TASK3_PRIORITY,//任务优先级
                            (TaskHandle_t *) &task3_handle);//任务句柄地址
							
	
    
    //删除启动任务,删除自身
    vTaskDelete(NULL);
							
	//退出临界区
	taskEXIT_CRITICAL();
    
}

//任务1函数
void task1(void *pvParameters)
{
    while(1)
    {
       HAL_UART_Transmit(&huart1, (uint8_t *)"task1 is running\r\n", 18, 1000);
        LED_Toggle(LED1_Pin);
        vTaskDelay(500);
    }
}

//任务2函数
void task2(void *pvParameters)
{
    while(1)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)"task2 is running\r\n", 18, 1000);
        LED_Toggle(LED2_Pin);
        vTaskDelay(500);
    }
}

char task_info[500];   //定义一个字符数组，用来接收任务信息字符串
//任务3函数
//Key1按下挂起任务1，key2按下恢复任务1，key3按下挂起调度器，key4按下恢复调度器
void task3(void *pvParameters)
{
    uint8_t key = 0;
    while(1)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)"task3 is running\r\n", 18, 1000);
        key = Key_Detect();
        if(key == KEY1_PRESS)
        {
            //挂起任务1
            HAL_UART_Transmit(&huart1, (uint8_t *)"suspend task1\r\n", 16, 1000);
            vTaskSuspend(task1_handle);
        }
        else if(key == KEY2_PRESS)
        {
           //恢复任务1
           HAL_UART_Transmit(&huart1, (uint8_t *)"resume task1\r\n", 15, 1000);
           vTaskResume(task1_handle);
        }
        else if(key == KEY3_PRESS)
        {
           //挂起调度器
           HAL_UART_Transmit(&huart1, (uint8_t *)"suspend scheduler\r\n", 20, 1000);
           vTaskSuspendAll();
        }
        else if(key == KEY4_PRESS)
        {
           //恢复调度器
           HAL_UART_Transmit(&huart1, (uint8_t *)"resume scheduler\r\n", 18, 1000);
           xTaskResumeAll();
        }

        
        //打印任务的状态
        vTaskList(task_info); //获取任务信息字符串
        HAL_UART_Transmit(&huart1,(uint8_t *)task_info, strlen(task_info), 1000);

        vTaskDelay(500);
    }
    
}

