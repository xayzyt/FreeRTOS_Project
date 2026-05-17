#include "freertos_demo.h"
#include "string.h"
#include "stdio.h"
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
    
							
	
    
    //删除启动任务,删除自身
    vTaskDelete(NULL);
							
	//退出临界区
	taskEXIT_CRITICAL();
    
}

//任务1函数用于按键扫描，当检测到按键KEY1被按下时，将发送任务通知。
void task1(void *pvParameters)
{
    uint8_t key = 0;
    BaseType_t res = 0;
    while(1)
    {
       key = Key_Detect();
       if(key == KEY1_PRESS)
       {
        //发送通知,只通知不带值
        res = xTaskNotifyGive(task2_handle);
        if(res == pdPASS)
        {
            HAL_UART_Transmit(&huart1,(uint8_t *)"task1向task2发送任务通知成功\r\n",sizeof("task1向task2发送任务通知成功\r\n"),1000);

        }
       }
        vTaskDelay(500);
    }
}

//任务2函数用于接收任务通知，并打印相关提示信息。
void task2(void *pvParameters)
{
    uint32_t notify_value = 0;
    char msg[50];
    while(1)
    {
       notify_value = ulTaskNotifyTake(pdFALSE,             //接收完通知后是否对通知清零或减1：pdtrue 清零 pdfalse 通知值-1
                        portMAX_DELAY       //等待任务通知阻塞时间
                        );

       snprintf(msg, sizeof(msg), "task2 received notification value: %lu\r\n", (uint32_t)notify_value);
       HAL_UART_Transmit(&huart1,(uint8_t *)msg,strlen(msg),1000);

       vTaskDelay(5000);    //每过5秒减1
    }
    
    
}   

