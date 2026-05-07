#include "freertos_demo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
char string_buffer[50];
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

//任务2函数:用于展示任务状态查询相关API函数的使用
char task_info_buffer[200];
void task2(void *pvParameters)
{
    UBaseType_t task1_priority = 0;
    UBaseType_t task_num = 0;
    TaskStatus_t task_status[10] = {0};
    TaskHandle_t current_task_handle = NULL;
    UBaseType_t task_stack_reamin_min = 0;
    eTaskState e_task_state = eInvalid;
    //通过句柄，获取任务优先级
    task1_priority = uxTaskPriorityGet(task1_handle);
    sprintf(string_buffer, "task1 priority: %d\r\n", task1_priority);
    HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 1000);
    //设置任务优先级
    vTaskPrioritySet(task1_handle, 4);
    //重新获取任务优先级
    task1_priority = uxTaskPriorityGet(task1_handle);
    sprintf(string_buffer, "task1 new priority: %d\r\n", task1_priority);
    HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 1000);
    //获取当前系统中的任务数
    task_num = uxTaskGetNumberOfTasks();
    sprintf(string_buffer, "task num: %d\r\n", task_num);
    HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 1000);
    //获取所有任务状态信息
    uxTaskGetSystemState(task_status,task_num,NULL);
    for(int i=0;i<task_num;i++)
    {
        sprintf(string_buffer, "task %d name: %s\r\n state: %d\r\n", i, task_status[i].pcTaskName, task_status[i].eCurrentState);
        HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 1000);
    }
    //获取单个任务信息
    vTaskGetInfo(task1_handle, &task_status[0], pdTRUE, eInvalid);
    sprintf(string_buffer, "task1 stack high water mark: %d\r\n", task_status[0].usStackHighWaterMark);
    HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 1000);
    //获取当前任务的句柄
    current_task_handle = xTaskGetCurrentTaskHandle();
    sprintf(string_buffer, "current task handle: %p\r\n,task2_handle: %p\r\n", current_task_handle, task2_handle);
    HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 1000);
    //通过任务名获取指定任务的句柄
    current_task_handle = xTaskGetHandle("task1");
    sprintf(string_buffer, "task1 handle by name: %p\r\n", current_task_handle);
    HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 1000);
    //!!!!!!!!!!!!获取任务的任务栈历史剩余最小值!!!!!!!!!!!!帮助我们判断任务栈分配空间是否合适（任务栈大小-历史使用最大值）
    task_stack_reamin_min = uxTaskGetStackHighWaterMark(task2_handle);
    sprintf(string_buffer, "task2 stack reamin min: %d\r\n", task_stack_reamin_min);
    HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 1000);
    //获取任务状态
    e_task_state = eTaskGetState(task1_handle);
    sprintf(string_buffer, "task1 state: %d\r\n", e_task_state);
    HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 1000);
    //以表格形式获取系统中所有任务的信息
    vTaskList(task_info_buffer);
    HAL_UART_Transmit(&huart1, (uint8_t *)"Task Name          State   Prio    Stack   Num\r\n", 48, 1000);
    HAL_UART_Transmit(&huart1, (uint8_t *)"**********************************************\r\n", 48, 1000);
    HAL_UART_Transmit(&huart1, (uint8_t *)task_info_buffer, strlen(task_info_buffer), 1000);    

    while(1)
    {
        vTaskDelay(500);
    }

}



