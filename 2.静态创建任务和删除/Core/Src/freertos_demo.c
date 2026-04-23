#include "freertos_demo.h"

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
StackType_t start_task_stack[START_TASK_STACK_SIZE]; //静态任务栈数组
StaticTask_t start_task_buf;                        //静态任务控制块结构体

//任务1相关参数设置
#define TASK1_STACK_SIZE  128         //任务1栈大小
#define TASK1_PRIORITY    2           //任务1优先级
TaskHandle_t task1_handle;
StackType_t task1_stack[TASK1_STACK_SIZE]; //静态任务栈数组
StaticTask_t task1_buf;                        //静态任务控制块结构体
void task1(void *pvParameters);

//任务2相关参数设置
#define TASK2_STACK_SIZE  128         //任务2栈大小
#define TASK2_PRIORITY    3           //任务2优先级
TaskHandle_t task2_handle;
StackType_t task2_stack[TASK2_STACK_SIZE]; //静态任务栈数组
StaticTask_t task2_buf;                        //静态任务控制块结构体
void task2(void *pvParameters);

//任务3相关参数设置
#define TASK3_STACK_SIZE  128         //任务3栈大小
#define TASK3_PRIORITY    4           //任务3优先级
TaskHandle_t task3_handle;
StackType_t task3_stack[TASK3_STACK_SIZE]; //静态任务栈数组
StaticTask_t task3_buf;                        //静态任务控制块结构体
void task3(void *pvParameters);


/* ===================================静态任务创建需要手动指定2个特殊任务的资源===============================*/

//空闲任务的配置
StaticTask_t idle_task_buf;               //静态任务控制块结构体
StackType_t idle_task_stack[configMINIMAL_STACK_SIZE]; //静态任务栈数组

//软件定时器任务的配置
StaticTask_t timer_task_buf;               //静态任务控制块结构体
StackType_t timer_task_stack[configTIMER_TASK_STACK_DEPTH]; //静态任务栈数组

//分配空闲任务的资源
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,           //静态任务控制块结构体tcb
                                        StackType_t ** ppxIdleTaskStackBuffer,      //静态任务栈数组
                                        uint32_t * pulIdleTaskStackSize )           //任务栈大小

{
    *ppxIdleTaskTCBBuffer = &idle_task_buf;                         //静态任务控制块结构体tcb
    *ppxIdleTaskStackBuffer = idle_task_stack;                  //静态任务栈数组
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;               //任务栈大小
}
//分配定时器任务的资源
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                         StackType_t ** ppxTimerTaskStackBuffer,
                                         uint32_t * pulTimerTaskStackSize )
{
    *ppxTimerTaskTCBBuffer = &timer_task_buf;                         //静态任务控制块结构体tcb
    *ppxTimerTaskStackBuffer = timer_task_stack;                  //静态任务栈数组
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;               //任务栈大小
}



//启动FreeRTOS
void freertos_start(void)
{
    //1.创建启动任务
    // xTaskCreate((TaskFunction_t) start_task,//任务函数地址
    //                          (char *)  "start_task", //任务名字符串（别名）
    //                          (configSTACK_DEPTH_TYPE) START_TASK_STACK_SIZE,//任务栈大小
    //                         (void *) NULL,//传递给任务的参数
    //                         (UBaseType_t) START_TASK_PRIORITY,//任务优先级
    //                         (TaskHandle_t *) &start_task_handle);//任务句柄地址

    //句柄是返回值
    start_task_handle = xTaskCreateStatic((TaskFunction_t) start_task,//任务函数地址
                             (char *)  "start_task", //任务名字符串（别名）
                             (uint32_t) START_TASK_STACK_SIZE,//任务栈大小
                            (void *) NULL,//传递给任务的参数
                            (UBaseType_t) START_TASK_PRIORITY,//任务优先级
                            (StackType_t *) start_task_stack,//静态任务栈数组
                            (StaticTask_t *) &start_task_buf);//静态任务控制块结构体tcb

    //2.启动调度器(会自动创建空闲任务和定时器任务)///////////静态方式要手动创建两个分配资源的接口函数
    vTaskStartScheduler();
}

//使用静态创建三个任务
void start_task(void *pvParameters)
{
    //进入临界区：保护临界区的代码不会被打断
    taskENTER_CRITICAL();                       //临界区很重要

    //1.创建任务1
    // xTaskCreate((TaskFunction_t) task1,//任务函数地址
    //                          (char *)  "task1", //任务名字符串（别名）
    //                          (configSTACK_DEPTH_TYPE) TASK1_STACK_SIZE,//任务栈大小
    //                         (void *) NULL,//传递给任务的参数
    //                         (UBaseType_t) TASK1_PRIORITY,//任务优先级
    //                         (TaskHandle_t *) &task1_handle);//任务句柄地址

    task1_handle = xTaskCreateStatic((TaskFunction_t) task1,//任务函数地址
                             (char *)  "task1", //任务名字符串（别名）
                             (uint32_t) TASK1_STACK_SIZE,//任务栈大小
                            (void *) NULL,//传递给任务的参数
                            (UBaseType_t) TASK1_PRIORITY,//任务优先级
                            (StackType_t *) task1_stack,//静态任务栈数组
                            (StaticTask_t *) &task1_buf);//静态任务控制块结构体tcb
    //2.创建任务2
    // xTaskCreate((TaskFunction_t) task2,//任务函数地址
    //                          (char *)  "task2", //任务名字符串（别名）
    //                          (configSTACK_DEPTH_TYPE) TASK2_STACK_SIZE,//任务栈大小
    //                         (void *) NULL,//传递给任务的参数
    //                         (UBaseType_t) TASK2_PRIORITY,//任务优先级
    //                         (TaskHandle_t *) &task2_handle);//任务句柄地址

    task2_handle = xTaskCreateStatic((TaskFunction_t) task2,//任务函数地址
                             (char *)  "task2", //任务名字符串（别名）
                             (uint32_t) TASK2_STACK_SIZE,//任务栈大小
                            (void *) NULL,//传递给任务的参数
                            (UBaseType_t) TASK2_PRIORITY,//任务优先级
                            (StackType_t *) task2_stack,//静态任务栈数组
                            (StaticTask_t *) &task2_buf);//静态任务控制块结构体tcb

    //3.创建任务3
    // xTaskCreate((TaskFunction_t) task3,//任务函数地址
    //                          (char *)  "task3", //任务名字符串（别名）
    //                          (configSTACK_DEPTH_TYPE) TASK3_STACK_SIZE,//任务栈大小
    //                         (void *) NULL,//传递给任务的参数
    //                         (UBaseType_t) TASK3_PRIORITY,//任务优先级
    //                         (TaskHandle_t *) &task3_handle);//任务句柄地址
		
    task3_handle = xTaskCreateStatic((TaskFunction_t) task3,//任务函数地址
                             (char *)  "task3", //任务名字符串（别名）
                             (uint32_t) TASK3_STACK_SIZE,//任务栈大小
                            (void *) NULL,//传递给任务的参数
                            (UBaseType_t) TASK3_PRIORITY,//任务优先级
                            (StackType_t *) task3_stack,//静态任务栈数组
                            (StaticTask_t *) &task3_buf);//静态任务控制块结构体tcb
	
    
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

//任务3函数
void task3(void *pvParameters)
{
    uint8_t key = 0;
    while(1)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)"task3 is running\r\n", 18, 1000);
        key = Key_Detect();
        if(key == KEY1_PRESS)
        {
            //判断是否删过，避免重复删除
            if(task1_handle != NULL)
            {
                //删除任务1
                HAL_UART_Transmit(&huart1, (uint8_t *)"Delete task1\r\n", 15, 1000);
                vTaskDelete(task1_handle);
                task1_handle = NULL;
            }
        }
        vTaskDelay(500);
    }
    
}

