#include "freertos_demo.h"

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


QueueHandle_t sem_handle; //信号量句柄
//启动FreeRTOS
void freertos_start(void)
{
    //创建二值信号量
    vSemaphoreCreateBinary(sem_handle);
    if(sem_handle == NULL)
    {
        //创建信号量失败
        HAL_UART_Transmit(&huart1, (uint8_t *)"sem create failed\r\n", 20, 1000);
    }
    else
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)"sem create success\r\n", 21, 1000);
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
    uint8_t res = 0;
    while(1)
    {
        key = Key_Detect();
        if(key == KEY1_PRESS)
        {
            //释放信号量
            res = xSemaphoreGive(sem_handle);
            if(res != pdPASS)
            {
                HAL_UART_Transmit(&huart1,"task1信号量获取失败\r\n",100,HAL_MAX_DELAY);
            }
            else
            {
                HAL_UART_Transmit(&huart1,"task1信号量获取成功\r\n",100,HAL_MAX_DELAY);
            }

        }
       
        vTaskDelay(500);
    }
}

//任务2函数：获取任务2信号量，成功获取打印信息
void task2(void *pvParameters)
{
    BaseType_t res = 0;
    while(1)
    {
        //获取信号量
        xSemaphoreTake(sem_handle,portMAX_DELAY);
        if(res == pdPASS)
        {
            HAL_UART_Transmit(&huart1,"task2获取成功\r\n",100,HAL_MAX_DELAY);
        }
        vTaskDelay(500);
    }
}

