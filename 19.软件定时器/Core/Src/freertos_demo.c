#include "freertos_demo.h"
#include <stdio.h>
#include <string.h>
//freertos相关的文件（必需的）
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
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


//软件定时器的超时回调函数
void timer1_callback(TimerHandle_t xTimer);
void timer2_callback(TimerHandle_t xTimer);
//软件定时器句柄
TimerHandle_t timer1_handle;
TimerHandle_t timer2_handle;
//启动FreeRTOS
void freertos_start(void)
{
    //创建一次性软件定时器
    timer1_handle = xTimerCreate(
                                    "gouxiang",      //软件定时器名称
                                    (TickType_t)500, //超时时间为500个rtos时钟节拍
                                    pdFALSE,         //是否自动重载  pdfalse：一次性 pdtrue：周期性
                                    (void *)1,       //定时器唯一ID
                                    timer1_callback  //定时器的回调函数
                                );
    if(timer1_handle != NULL )
    {
        HAL_UART_Transmit(&huart1,(uint8_t *)"timer1一次性定时器创建成功\r\n",sizeof("timer1一次性定时器创建成功\r\n"),(uint32_t)1000);
    }
    //创建周期性软件定时器
    timer2_handle = xTimerCreate(
                                    "goushen",      //软件定时器名称
                                    (TickType_t)500, //超时时间为500个rtos时钟节拍
                                    pdTRUE,         //是否自动重载  pdfalse：一次性 pdtrue：周期性
                                    (void *)2,       //定时器唯一ID
                                    timer2_callback  //定时器的回调函数
                                );                            
    if(timer2_handle != NULL )
    {
        HAL_UART_Transmit(&huart1,(uint8_t *)"timer2周期性定时器创建成功\r\n",sizeof("timer2周期性定时器创建成功\r\n"),(uint32_t)1000);
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

   
							
	
    
    //删除启动任务,删除自身
    vTaskDelete(NULL);
							
	//退出临界区
	taskEXIT_CRITICAL();
    
}

//任务1函数
void task1(void *pvParameters)
{
    uint8_t key = 0;
    uint8_t res = 0;
    while(1)
    {
       key = Key_Detect();
       if(key == KEY1_PRESS)
       {
            //开启软件定时器
            res = xTimerStart(timer1_handle,portMAX_DELAY);
            if(res != pdFALSE)
            {
                HAL_UART_Transmit(&huart1,(uint8_t *)"timer1一次性定时器启动成功\r\n",sizeof("timer1一次性定时器启动成功\r\n"),(uint32_t)1000);
            }
            res = xTimerStart(timer2_handle,portMAX_DELAY);
            if(res != pdFALSE)
            {
                HAL_UART_Transmit(&huart1,(uint8_t *)"timer2周期性定时器启动成功\r\n",sizeof("timer2周期性定时器启动成功\r\n"),(uint32_t)1000);
            }
       }
       else if(key == KEY2_PRESS)
       {
            //停止软件定时器
            res = xTimerStop(timer1_handle,portMAX_DELAY);
            if(res != pdFALSE)
            {
                HAL_UART_Transmit(&huart1,(uint8_t *)"timer1一次性定时器停止成功\r\n",sizeof("timer1一次性定时器停止成功\r\n"),(uint32_t)1000);
            }
            res = xTimerStop(timer2_handle,portMAX_DELAY);
            if(res != pdFALSE)
            {
                HAL_UART_Transmit(&huart1,(uint8_t *)"timer2周期性定时器停止成功\r\n",sizeof("timer2周期性性定时器停止成功\r\n"),(uint32_t)1000);
            }
       }
        vTaskDelay(500);
    }
}
char msg[50];
char string[50];
void timer1_callback(TimerHandle_t xTimer)
{
    static uint16_t timer1_count = 0;
    sprintf(msg,"timer1超时回调=%d次\r\n",++timer1_count);
    HAL_UART_Transmit(&huart1,(uint8_t *)msg,strlen(msg),(uint32_t)1000);
}
void timer2_callback(TimerHandle_t xTimer)
{
    static uint16_t timer2_count = 0;
    sprintf(string,"timer2超时回调=%d次\r\n",++timer2_count);
    HAL_UART_Transmit(&huart1,(uint8_t *)string,strlen(string),(uint32_t)1000);
}
