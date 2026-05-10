#include "freertos_demo.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//freertos相关的文件（必需的）
#include "FreeRTOS.h"
#include "task.h"

//队列相关的头文件
#include "queue.h"

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

//消息队列相关句柄
QueueHandle_t queue1;
QueueHandle_t big_queue;

//启动FreeRTOS
void freertos_start(void)
{
    //创建消息队列
    /*创建一个存放key值编号的小队列queue*/
    queue1 = xQueueCreate(2, sizeof(uint8_t));
    if(queue1 == NULL)
    {
        //创建队列失败
       HAL_UART_Transmit(&huart1, (uint8_t *)"queue1 create failed\r\n", 23, 1000);
    }
    else
    {
        //创建队列成功
         HAL_UART_Transmit(&huart1, (uint8_t *)"queue1 create success\r\n", 24, 1000);
    }

    /*创建一个存放大数据的队列big_queue*/
    big_queue = xQueueCreate(20, sizeof(char *));
    if(big_queue == NULL)
    {
        //创建队列失败
      HAL_UART_Transmit(&huart1, (uint8_t *)"big_queue create failed\r\n", 26, 1000);

    }
    else
    {
        //创建队列成功
     HAL_UART_Transmit(&huart1, (uint8_t *)"big_queue create success\r\n", 27, 1000);
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

    //1.创建任务1:按下key1或key2，将键值拷贝到队列queue1中，按下key3将传输大数据到big_queue中
    xTaskCreate((TaskFunction_t) task1,//任务函数地址
                             (char *)  "task1", //任务名字符串（别名）
                             (configSTACK_DEPTH_TYPE) TASK1_STACK_SIZE,//任务栈大小
                            (void *) NULL,//传递给任务的参数
                            (UBaseType_t) TASK1_PRIORITY,//任务优先级
                            (TaskHandle_t *) &task1_handle);//任务句柄地址

    //2.创建任务2：读取小队列queue1中的键值，并通过串口发送
    xTaskCreate((TaskFunction_t) task2,//任务函数地址
                             (char *)  "task2", //任务名字符串（别名）
                             (configSTACK_DEPTH_TYPE) TASK2_STACK_SIZE,//任务栈大小
                            (void *) NULL,//传递给任务的参数
                            (UBaseType_t) TASK2_PRIORITY,//任务优先级
                            (TaskHandle_t *) &task2_handle);//任务句柄地址
    //3.创建任务3：读取大队列big_queue中的数据，并通过串口发送
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

//任务1函数:按下key1或key2，将键值拷贝到队列queue1中，按下key3将传输大数据到big_queue中
void task1(void *pvParameters)
{
    uint8_t key = 0;
    BaseType_t res = 0;
    char *big_data = "This is a big data string from key3 press.";
    while(1)
    {
       key = Key_Detect();
       if(key == KEY1_PRESS || key == KEY2_PRESS)
       {
           //将key值发送到小队列queue1中
           res = xQueueSend(queue1, &key, portMAX_DELAY);
           if(res == pdPASS)
           {
               HAL_UART_Transmit(&huart1, (uint8_t *)"Key value send to queue1 failed\r\n", 34, 1000);
           }
              else
              {
                
                HAL_UART_Transmit(&huart1, (uint8_t *)"Key value sent to queue1\r\n", 28, 1000);
              }
       }
       else if(key == KEY3_PRESS)
       {
           //将大数据发送到大队列big_queue中
           res = xQueueSend(big_queue, &big_data, portMAX_DELAY);
              if(res == pdPASS)
              {
               
                 HAL_UART_Transmit(&huart1, (uint8_t *)"Big data send to big_queue failed\r\n", 35, 1000);

              }
              else
                {
                   
                     HAL_UART_Transmit(&huart1, (uint8_t *)"Big data sent to big_queue\r\n", 29, 1000);
                }
       }
       vTaskDelay(500);
    }
}

//任务2函数：读取小队列queue1中的键值，并通过串口发送
void task2(void *pvParameters)
{
    uint8_t receive = 0;
    uint8_t res = 0;
    char msg[50];
    while(1)
    {
        res = xQueueReceive(queue1, &receive, portMAX_DELAY);
        if(res == pdPASS)
        {

            sprintf(msg, "Received key value from queue1: %d\r\n", receive);
            HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 1000);
        }

    }
}

//任务3函数：读取大队列big_queue中的数据，并通过串口发送
void task3(void *pvParameters)
{
    uint8_t res = 0;
    char msg[50];
    char *receive_buff;
    while(1)
    {
        res = xQueueReceive(big_queue, &receive_buff, portMAX_DELAY);
        if(res == pdPASS)
        {

            sprintf(msg, "Received big data from big_queue: %s\r\n", receive_buff);
            HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 1000);
        }

    }
    
}

