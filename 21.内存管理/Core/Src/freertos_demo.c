#include "freertos_demo.h"
#include <stdio.h>
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

   
							
	
    
    //删除启动任务,删除自身
    vTaskDelete(NULL);
							
	//退出临界区
	taskEXIT_CRITICAL();
    
}



//任务1函数：当KEY1按下则申请内存，当KEY2按下则释放内存，并打印剩余内存信息。
void task1(void *pvParameters)
{
    uint8_t key = 0;
    void * buff = 0;
    size_t res = 0;
    while(1)
    {
        key = Key_Detect();
        if(key == KEY1_PRESS)
        {
            //申请内存:实际申请会比传入参数大，多出两部分：1.队结构体大小 2.字节对齐会浪费一部分
           buff = pvPortMalloc(20);
           res = xPortGetFreeHeapSize();
           HAL_UART_Transmit(&huart1,(uint8_t *)"申请20空间,剩下空间=",sizeof("申请20空间,剩下空间="),(uint32_t)1000);
           HAL_UART_Transmit(&huart1,(uint8_t *)res,sizeof(res),(uint32_t)1000);
        }
        else if(key == KEY2_PRESS)
        {
            //释放内存
            vPortFree( buff);
            res = xPortGetFreeHeapSize();
           HAL_UART_Transmit(&huart1,(uint8_t *)"释放20空间,剩下空间=",sizeof("释放20空间,剩下空间="),(uint32_t)1000);
           HAL_UART_Transmit(&huart1,(uint8_t *)res,sizeof(res),(uint32_t)1000);
        }
        vTaskDelay(500);
    }
    
}

