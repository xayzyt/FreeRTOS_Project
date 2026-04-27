# FreeRTOS_Project

基于 **STM32F103 + STM32 HAL + FreeRTOS** 的实时系统实验仓库。它不是单纯的笔记目录，而是一组可打开、可编译、可对照源码学习的 FreeRTOS 小工程，适合展示我对任务生命周期、调度机制、同步通信和中断协作的理解。

![C](https://img.shields.io/badge/C-00599C?style=flat-square&logo=c&logoColor=white)
![STM32F1](https://img.shields.io/badge/STM32F1-03234B?style=flat-square&logo=stmicroelectronics&logoColor=white)
![FreeRTOS](https://img.shields.io/badge/FreeRTOS-173B6C?style=flat-square)
![Keil](https://img.shields.io/badge/Keil_MDK-1F6FEB?style=flat-square)

## 为什么这个仓库值得看

这个仓库记录的是我从裸机思维切到 RTOS 思维的过程：把一个循环里的逻辑拆成任务，把共享资源放到同步机制里，把外部事件放到中断和任务之间协作处理。

面试官可以重点看：

- 我如何创建、删除和组织 FreeRTOS 任务。
- 我如何理解动态任务和静态任务的区别。
- 我如何配置 `FreeRTOSConfig.h`、移植层和堆内存管理。
- 我如何把按键、LED、串口等简单外设接入 RTOS 实验。
- 我如何处理“中断里做什么、任务里做什么”的边界。

## 实验导览

| 目录 | 内容 | 可以体现的能力 |
| --- | --- | --- |
| `0.FreeRTOS工程/` | STM32F1 + HAL + FreeRTOS 基础工程 | 工程搭建、FreeRTOS 移植、基础外设初始化 |
| `1.动态创建任务和删除/` | 动态任务创建与删除实验 | `xTaskCreate`、任务句柄、任务生命周期管理 |
| `2.静态创建任务和删除/` | 静态任务创建与删除实验 | 静态栈/TCB 分配、资源可控的任务设计 |
| `4.中断管理/` | 中断与 RTOS 协作实验 | 中断服务函数、任务响应、实时事件处理 |

## 核心源码入口

| 文件/目录 | 说明 |
| --- | --- |
| `Core/Src/main.c` | HAL 初始化、系统入口、FreeRTOS 启动流程 |
| `Core/Inc/FreeRTOSConfig.h` | FreeRTOS 裁剪配置、时钟节拍、API 开关 |
| `Core/Src/freertos_demo.c` | 各实验的任务创建和调度逻辑 |
| `FreeRTOS/Source/` | FreeRTOS 内核源码，包括 `tasks.c`、`queue.c`、`timers.c` 等 |
| `FreeRTOS/portable/MemMang/heap_4.c` | 动态内存管理策略 |
| `Int/Key.c`, `Int/LED.c` | 按键、LED 等基础外设封装 |
| `MDK-ARM/*.uvprojx` | Keil 工程文件 |

## 我在这个仓库里练的不是 API 背诵

FreeRTOS 的 API 本身不难，真正重要的是设计边界：

```text
外部事件 -> 中断捕获 -> 最小处理 -> 通知/唤醒任务 -> 任务完成耗时逻辑
```

这个仓库里的实验都围绕这个思路展开：先把任务跑起来，再比较不同创建方式，最后把中断事件纳入 RTOS 调度模型。

## 开发环境

- MCU: STM32F103 系列
- IDE: Keil MDK / STM32CubeMX
- SDK: STM32 HAL
- RTOS: FreeRTOS
- Language: C

## 如何打开工程

1. 进入对应实验目录，例如 `2.静态创建任务和删除/`。
2. 打开 `MDK-ARM/` 下的 `.uvprojx` 工程文件。
3. 使用 Keil MDK 编译、烧录和调试。
4. 从 `Core/Src/freertos_demo.c` 或 `Core/Src/main.c` 追踪实验逻辑。

## 面试时我会重点讲

- 动态任务和静态任务的内存差异。
- FreeRTOS 任务优先级、时间片和阻塞态的理解。
- `heap_4.c` 的作用，以及为什么嵌入式项目要关注堆使用。
- 中断中不做耗时逻辑，如何把事件交给任务处理。
- 为什么 RTOS 项目比裸机循环更适合复杂状态和多模块协作。

