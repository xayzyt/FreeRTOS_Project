#include "freertos_demo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//freertos相关的文件（必需的）
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
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


QueueHandle_t sem_handle;
//启动FreeRTOS
void freertos_start(void)
{
    //创建互斥信号量
    sem_handle = xSemaphoreCreateMutex();
    if(sem_handle == NULL)
    {
        //创建失败
        HAL_UART_Transmit(&huart1, (uint8_t *)"Create semaphore failed\r\n", 26, 1000);
        while(1);
    }
    else
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)"Create semaphore success\r\n", 27, 1000);
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

//任务1函数，低优先级获取信号量
void task1(void *pvParameters)
{
    
    BaseType_t res;
    while(1)
    {
        //获取信号量
        HAL_UART_Transmit(&huart1, (uint8_t *)"低优先级的task1开始获取信号量\r\n", 100, 1000);
        res = xSemaphoreTake(sem_handle, portMAX_DELAY);
        if(res!= pdPASS)
        {
            HAL_UART_Transmit(&huart1, (uint8_t *)"task1获取信号量失败\r\n", 15, 1000);
        }
        else
        {
            HAL_UART_Transmit(&huart1, (uint8_t *)"task1获取信号量成功\r\n", 100, 1000);   
              
        }
        HAL_UART_Transmit(&huart1, (uint8_t *)"task1正在执行..\r\n", 100, 1000);
        HAL_Delay(3000);
        //释放信号量
        HAL_UART_Transmit(&huart1, (uint8_t *)"低优先级的task1开始释放信号量\r\n", 100, 1000);
        res = xSemaphoreGive(sem_handle);
        if(res != pdPASS)
        {
            HAL_UART_Transmit(&huart1, (uint8_t *)"task1释放信号量失败\r\n", 100, 1000);
        }
        vTaskDelay(1000);
    }
}

//任务2函数
void task2(void *pvParameters)
{
    while(1)
    {
       HAL_UART_Transmit(&huart1, (uint8_t *)"中优先级task2 is running\r\n", 100, 1000);
       HAL_Delay(1500);
       HAL_UART_Transmit(&huart1, (uint8_t *)"task2执行完一次\r\n", 100, 1000);
       vTaskDelay(1000);
    }
}

//任务3函数，高优先级也获取信号量
void task3(void *pvParameters)
{
   BaseType_t res;
    while(1)
    {
        //获取信号量
        HAL_UART_Transmit(&huart1, (uint8_t *)"高优先级的task3开始获取信号量\r\n", 100, 1000);
        res = xSemaphoreTake(sem_handle, portMAX_DELAY);
        if(res!= pdPASS)
        {
            HAL_UART_Transmit(&huart1, (uint8_t *)"task3获取信号量失败\r\n", 100, 1000);
        }
        else
        {
            HAL_UART_Transmit(&huart1, (uint8_t *)"task3获取信号量成功\r\n", 100, 1000);

        }
        HAL_UART_Transmit(&huart1, (uint8_t *)"task3正在执行.\r\n", 100, 1000);
        HAL_Delay(1000);
        //释放信号量
        HAL_UART_Transmit(&huart1, (uint8_t *)"task3准备释放信号量\r\n", 100, 1000);
        res = xSemaphoreGive(sem_handle);
        if(res != pdPASS)
        {
            HAL_UART_Transmit(&huart1, (uint8_t *)"task3释放信号量失败\r\n", 100, 1000);
        }
        vTaskDelay(1000);
    }
    
}

