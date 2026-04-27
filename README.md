# FreeRTOS_Project

STM32F103 + STM32 HAL + FreeRTOS 实验工程。

![C](https://img.shields.io/badge/C-00599C?style=flat-square&logo=c&logoColor=white)
![STM32F1](https://img.shields.io/badge/STM32F1-03234B?style=flat-square&logo=stmicroelectronics&logoColor=white)
![FreeRTOS](https://img.shields.io/badge/FreeRTOS-173B6C?style=flat-square)
![Keil](https://img.shields.io/badge/Keil_MDK-1F6FEB?style=flat-square)

## 内容

- FreeRTOS 基础工程
- 动态创建和删除任务
- 静态创建和删除任务
- 中断管理实验
- 按键、LED、串口等基础外设

## 目录

| 目录 | 说明 |
| --- | --- |
| `0.FreeRTOS工程/` | 基础工程 |
| `1.动态创建任务和删除/` | 动态任务实验 |
| `2.静态创建任务和删除/` | 静态任务实验 |
| `4.中断管理/` | 中断相关实验 |

## 主要文件

| 路径 | 说明 |
| --- | --- |
| `Core/Src/main.c` | 工程入口 |
| `Core/Src/freertos_demo.c` | FreeRTOS 实验代码 |
| `Core/Inc/FreeRTOSConfig.h` | FreeRTOS 配置 |
| `FreeRTOS/Source/` | FreeRTOS 源码 |
| `Int/Key.c` | 按键驱动 |
| `Int/LED.c` | LED 驱动 |
| `MDK-ARM/*.uvprojx` | Keil 工程文件 |

## 环境

- STM32F103
- STM32 HAL
- FreeRTOS
- Keil MDK
- STM32CubeMX

## 使用

1. 进入对应实验目录。
2. 打开 `MDK-ARM/` 下的 `.uvprojx` 文件。
3. 使用 Keil 编译、下载和调试。
