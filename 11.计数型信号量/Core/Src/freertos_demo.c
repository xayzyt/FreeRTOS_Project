#include "freertos_demo.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//freertos相关的文件（必需的）
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"     //信号量需要包含这个头文件
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


QueueHandle_t sem_count_handle; //信号量句柄
//启动FreeRTOS
void freertos_start(void)
{
    UBaseType_t count = 0;
    //创建计数型信号量
    sem_count_handle = xSemaphoreCreateCounting(100, 0); //最大计数值100，初始值0
    if(sem_count_handle == NULL)
    {
        //创建信号量失败
        HAL_UART_Transmit(&huart1, (uint8_t *)"sem create failed\r\n", 20, 1000);
    }
    else
    {
        //创建成功，获取一次计数值，打印
        count = uxSemaphoreGetCount(sem_count_handle);
        char msg[50] = {0};
        sprintf(msg, "sem create success, count=%lu\r\n", count);
        HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 1000);
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

//任务1函数:按键扫描，当检测到key1按下，释放信号量
void task1(void *pvParameters)
{
    uint8_t key = 0;
    BaseType_t res = 0;
    while(1)
    {
        key = Key_Detect();
        if(key == KEY1_PRESS)
        {
            //释放信号量
            res = xSemaphoreGive(sem_count_handle);
            if(res == pdPASS)
            {
                HAL_UART_Transmit(&huart1,"task1 counting semaphore give success\r\n",100,1000);
            }
            else
            {
                HAL_UART_Transmit(&huart1,"task1 counting semaphore give failed\r\n",100,1000);
            }

        }
       
        vTaskDelay(500);
    }
}

//任务2函数：获取任务2信号量，成功获取打印信息
void task2(void *pvParameters)
{
    BaseType_t res = 0;
    UBaseType_t current_count = 0;
    
    while(1)
    {
        //获取信号量
        res = xSemaphoreTake(sem_count_handle,portMAX_DELAY);
        if(res == pdPASS)
        {
            HAL_UART_Transmit(&huart1,"task2 take success\r\n",50,1000);
        }
        else
        {
            HAL_UART_Transmit(&huart1,"task2 take failed\r\n",50,1000);
        }
        //获取当前计数值并打印
        current_count = uxSemaphoreGetCount(sem_count_handle);
        HAL_UART_Transmit(&huart1, (uint8_t *)"current count=", 14, 1000);
        HAL_UART_Transmit(&huart1,(uint8_t *)&current_count,50,1000);
        HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", 2, 1000);

        vTaskDelay(500);
    }
}

