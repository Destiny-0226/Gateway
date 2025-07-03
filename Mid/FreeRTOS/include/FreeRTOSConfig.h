// /*
//  * FreeRTOS V202411.00
//  * Copyright (C) 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
//  *
//  * Permission is hereby granted, free of charge, to any person obtaining a copy of
//  * this software and associated documentation files (the "Software"), to deal in
//  * the Software without restriction, including without limitation the rights to
//  * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
//  * the Software, and to permit persons to whom the Software is furnished to do so,
//  * subject to the following conditions:
//  *
//  * The above copyright notice and this permission notice shall be included in all
//  * copies or substantial portions of the Software.
//  *
//  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
//  * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
//  * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
//  * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//  *
//  * https://www.FreeRTOS.org
//  * https://github.com/FreeRTOS
//  *
//  */

// #ifndef FREERTOS_CONFIG_H
// #define FREERTOS_CONFIG_H

// /*-----------------------------------------------------------
//  * Application specific definitions.
//  *
//  * These definitions should be adjusted for your particular hardware and
//  * application requirements.
//  *
//  * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
//  * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
//  *
//  * See http://www.freertos.org/a00110.html
//  *----------------------------------------------------------*/

// #define configUSE_PREEMPTION     1
// #define configUSE_IDLE_HOOK      0
// #define configUSE_TICK_HOOK      0
// #define configCPU_CLOCK_HZ       ((unsigned long)72000000)
// #define configTICK_RATE_HZ       ((TickType_t)1000)
// #define configMAX_PRIORITIES     (5)
// #define configMINIMAL_STACK_SIZE ((unsigned short)128)
// #define configTOTAL_HEAP_SIZE    ((size_t)(12 * 1024))
// #define configMAX_TASK_NAME_LEN  (16)

// #define configUSE_16_BIT_TICKS   0
// #define configIDLE_SHOULD_YIELD  1

// /* Co-routine definitions. */
// #define configUSE_CO_ROUTINES           0
// #define configMAX_CO_ROUTINE_PRIORITIES (2)

// /* Set the following definitions to 1 to include the API function, or zero
// to exclude the API function. */

// #define INCLUDE_vTaskPrioritySet      1
// #define INCLUDE_uxTaskPriorityGet     1
// #define INCLUDE_vTaskDelete           1
// #define INCLUDE_vTaskCleanUpResources 0
// #define INCLUDE_vTaskSuspend          1
// #define INCLUDE_vTaskDelayUntil       1
// #define INCLUDE_vTaskDelay            1

// /* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
// (lowest) to 0 (1?) (highest). */
// #define configKERNEL_INTERRUPT_PRIORITY 255
// /* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
// See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
// #define configMAX_SYSCALL_INTERRUPT_PRIORITY 191 /* equivalent to 0xb0, or priority 11. */

// /* This is the value being used as per the ST library which permits 16
// priority values, 0 to 15.  This must correspond to the
// configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
// NVIC value of 255. */
// #define configLIBRARY_KERNEL_INTERRUPT_PRIORITY 15

// #define vPortSVCHandler                         SVC_Handler
// #define xPortPendSVHandler                      PendSV_Handler
// #define xPortSysTickHandler                     SysTick_Handler

// // 任务通知
// #define configUSE_TASK_NOTIFICATIONS 1

// // // 获取调度器运行状态
// // #define INCLUDE_xTaskGetSchedulerState 1
// // // 静态创建任务
// // #define configSUPPORT_STATIC_ALLOCATION 0

// // // 任务栈历史最小剩余空间
// // #define INCLUDE_uxTaskGetStackHighWaterMark 1
// // // 获取任务状态
// // #define INCLUDE_eTaskGetState 1

// // // 运行统计时间
// // // extern uint32_t runtime_tick;
// // // #define configGENERATE_RUN_TIME_STATS            1
// // // #define configUSE_STATS_FORMATTING_FUNCTIONS     1
// // // #define configUSE_TRACE_FACILITY                 1
// // // #define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() HAL_TIM_Base_Start_IT(&htim6)
// // // #define portGET_RUN_TIME_COUNTER_VALUE()         runtime_tick

// // // 计数型信号量
// // #define configUSE_COUNTING_SEMAPHORES 1
// // // 互斥信号量
// // #define configUSE_MUTEXES 1

// // // 队列集
// // #define configUSE_QUEUE_SETS 1

// // // 软件定时器
// // #define configUSE_TIMERS                 1
// // #define configSUPPORT_DYNAMIC_ALLOCATION 1
// // #define configTIMER_TASK_PRIORITY        4
// // #define configTIMER_QUEUE_LENGTH         10
// // #define configTIMER_TASK_STACK_DEPTH     128

// #define configUSE_COUNTING_SEMAPHORES    1
// #define configSUPPORT_DYNAMIC_ALLOCATION 1

// #endif /* FREERTOS_CONFIG_H */

/// ======================================================================================================================

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

// #define configUSE_PREEMPTION 1
#define configUSE_IDLE_HOOK      0
#define configUSE_TICK_HOOK      0
#define configCPU_CLOCK_HZ       ((unsigned long)72000000)
#define configTICK_RATE_HZ       ((TickType_t)1000) /**/
#define configMAX_PRIORITIES     (5)
#define configMINIMAL_STACK_SIZE ((unsigned short)128)
#define configTOTAL_HEAP_SIZE    ((size_t)(10 * 1024))
#define configMAX_TASK_NAME_LEN  (16)
// #define configUSE_TRACE_FACILITY 0
#define configUSE_16_BIT_TICKS  0 /* 时间组最多有24个事件 */
#define configIDLE_SHOULD_YIELD 1

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet      1
#define INCLUDE_uxTaskPriorityGet     1
#define INCLUDE_vTaskDelete           1 /* 删除任务 */
#define INCLUDE_vTaskCleanUpResources 0
#define INCLUDE_vTaskSuspend          1 /* 挂起任务(暂停) */
#define INCLUDE_vTaskDelayUntil       1
#define INCLUDE_vTaskDelay            1 /* 延时 */

/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
(lowest) to 0 (1?) (highest). */
// #define configKERNEL_INTERRUPT_PRIORITY 255
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
// #define configMAX_SYSCALL_INTERRUPT_PRIORITY 191 /* equivalent to 0xb0, or priority 11. */

/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY 15

/* 1. 基础的必须的配置 */
#define xPortPendSVHandler             PendSV_Handler /* 用来处理PendSV中断:系统的任务的切换 */
#define vPortSVCHandler                SVC_Handler    /* svc: 实现操作系统的系统调用 */
#define xPortSysTickHandler            SysTick_Handler

#define INCLUDE_xTaskGetSchedulerState 1 /* 用来获取调度状态  */

/* 2. 动态创建任务 */
/* 动态内存分配 */
#define configSUPPORT_DYNAMIC_ALLOCATION 1
#define configUSE_COUNTING_SEMAPHORES    1
#define configFRTOS_MEMORY_SCHEME        4 // 或者选择其他方案，根据你的需求

/* 3. 显示任务的状态 */
#define configUSE_TRACE_FACILITY             1 /* 使用追功能 */
#define configUSE_STATS_FORMATTING_FUNCTIONS 1 /* 字符格式化 */

/* 4. 中断相关 */
/* rtos内核的中断优先级: 配置为最低优先级, 主要是为了不影响其他中断 */
#define configKERNEL_INTERRUPT_PRIORITY (15 << 4)
/* rtos管理的中断的阈值 */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY (5 << 4)
/* 是新版本,与上面的等价 */
#define configMAX_API_CALL_INTERRUPT_PRIORITY configMAX_SYSCALL_INTERRUPT_PRIORITY

/* 5. 时间片轮转 */
/* 抢占式调度 */
#define configUSE_PREEMPTION 1
/* 时间片轮转 */
#define configUSE_TIME_SLICING 1

// /* 6. 任务相关api */
// #define INCLUDE_xTaskGetHandle 1
// #define INCLUDE_uxTaskGetStackHighWaterMark 1

// /* 7. 运行时间统计 */
// /* 开启时间统计 */

// #define configGENERATE_RUN_TIME_STATS 1
// /* 更改精度的计数器青0 */
// #define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() (highCount = 0)
// /* 返回当前计数器的值 */
// #define portGET_RUN_TIME_COUNTER_VALUE() (highCount)

// /* 8. 互斥信号量 */
// #define configUSE_MUTEXES 1

// /* 9. 队列集 */
// #define configUSE_QUEUE_SETS 1

// /* 10 任务通知 */
// #define configUSE_TASK_NOTIFICATIONS 1
// /* 一个任务最多可以同时缓冲2个通知 */
// #define configTASK_NOTIFICATION_ARRAY_ENTRIES 2

// 这个是中断要添加的
//  #include "FreeRTOS.h"
//  #include "task.h"
//  extern void xPortSysTickHandler(void);

// if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
// {
//     xPortSysTickHandler();

#endif /* FREERTOS_CONFIG_H */
// }
