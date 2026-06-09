# FreeRTOS on STM32F103

基于 STM32F103、STM32 HAL 和 FreeRTOS 的递进式实验仓库。每个目录保留一个可独立打开的 Keil 工程，用于验证调度、任务通信、同步机制、软件定时器、低功耗和内存管理。

> A progressive set of FreeRTOS experiments for STM32F103, covering scheduling, inter-task communication, synchronization, timers, low-power operation, and memory management.

![C](https://img.shields.io/badge/C-00599C?style=flat-square&logo=c&logoColor=white)
![STM32F1](https://img.shields.io/badge/STM32F1-03234B?style=flat-square&logo=stmicroelectronics&logoColor=white)
![FreeRTOS](https://img.shields.io/badge/FreeRTOS-173B6C?style=flat-square)
![Keil](https://img.shields.io/badge/Keil_MDK-1F6FEB?style=flat-square)

## 学习路线

| 阶段 | 实验 |
| --- | --- |
| 任务基础 | 动态/静态创建与删除、挂起与恢复、中断管理 |
| 调度观察 | 时间片调度、任务状态、运行时间统计、延时函数差异 |
| 任务通信 | 消息队列、队列集、任务通知模拟队列 |
| 同步互斥 | 二值/计数信号量、优先级翻转、互斥信号量 |
| 事件机制 | 事件标志组、任务通知模拟信号量与事件组 |
| 系统能力 | 软件定时器、低功耗模式、内存管理 |

## 目录

```text
0.FreeRTOS工程/             基础移植工程
1.动态创建任务和删除/       动态任务管理
2.静态创建任务和删除/       静态任务管理
3.任务的挂起与恢复/         任务状态控制
4.中断管理/                 中断与 RTOS API
5.时间片调度/               同优先级任务调度
6.任务状态查询/             状态信息读取
7.任务时间统计/             运行时间统计
8.时间延时函数区分/         vTaskDelay 与阻塞延时
9.消息队列/                 队列通信
10.二值信号量/              二值同步
11.计数型信号量/            资源计数
12.优先级翻转/              问题复现
13.互斥信号量/              优先级继承
14.队列集/                  多队列等待
15.事件标志组/              多事件同步
16-18.任务通知实验/         通知模拟信号量、队列和事件组
19.软件定时器/              Timer Service
20.低功耗模式/              Tickless Idle
21.内存管理/                FreeRTOS heap 方案
```

## 典型工程结构

| 路径 | 说明 |
| --- | --- |
| `Core/Src/main.c` | 初始化和程序入口 |
| `Core/Src/freertos_demo.c` | 当前实验逻辑 |
| `Core/Inc/FreeRTOSConfig.h` | 内核配置 |
| `FreeRTOS/Source/` | FreeRTOS 内核源码 |
| `Int/Key.c`、`Int/LED.c` | 按键和 LED 驱动 |
| `MDK-ARM/*.uvprojx` | Keil 工程文件 |

## 使用方法

1. 安装 Keil MDK、STM32F1 Device Family Pack 和对应下载器驱动。
2. 进入目标实验目录。
3. 打开 `MDK-ARM/` 下的 `.uvprojx`。
4. 阅读 `freertos_demo.c` 和 `FreeRTOSConfig.h`，确认该实验启用的 API。
5. 编译、烧录，通过串口、LED 或调试器观察现象。

## 建议观察项

- 任务优先级、阻塞时间和栈深度是否符合预期
- ISR 中是否只调用 `FromISR` 版本 API
- 互斥信号量是否解决优先级翻转
- 队列满、超时和资源不足时系统如何处理
- 软件定时器回调是否包含阻塞操作
- Tickless Idle 和不同 heap 方案对系统行为的影响

## 说明

该仓库用于 RTOS 学习和实验对比，不代表所有示例都适合作为生产代码直接复用。实际项目还需要补充错误处理、边界检查、栈水位监控和长期稳定性测试。
