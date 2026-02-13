#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* RP2040 runs at 125 MHz by default */
#define configCPU_CLOCK_HZ    (125000000UL)

#define configTICK_RATE_HZ                         (1000U)
#define configUSE_PREEMPTION                        1
#define configUSE_TIME_SLICING                     1
#define configMAX_PRIORITIES                       8
#define configMINIMAL_STACK_SIZE                   256
#define configMAX_TASK_NAME_LEN                    16
#define configTICK_TYPE_WIDTH_IN_BITS              TICK_TYPE_WIDTH_32_BITS
#define configIDLE_SHOULD_YIELD                    1
#define configUSE_TICKLESS_IDLE                    0
#define configUSE_IDLE_HOOK                        0
#define configUSE_TICK_HOOK                        0

/* Memory */
#define configSUPPORT_STATIC_ALLOCATION            1
#define configSUPPORT_DYNAMIC_ALLOCATION           1
#define configTOTAL_HEAP_SIZE                      (16 * 1024)
#define configUSE_MUTEXES                          1
#define configUSE_RECURSIVE_MUTEXES                1
#define configUSE_COUNTING_SEMAPHORES              1
#define configUSE_TASK_NOTIFICATIONS              1
#define configUSE_EVENT_GROUPS                     1

/* Required by RP2040 SMP port - xEventGroupSetBitsFromISR needs xTimerPendFunctionCall */
#define INCLUDE_xTimerPendFunctionCall              1

/* Idle/timer task memory - we provide vApplicationGetIdleTaskMemory in main.cpp */
#define configKERNEL_PROVIDED_STATIC_MEMORY        0
#define configUSE_TIMERS                           1
#define configTIMER_TASK_PRIORITY                  ( configMAX_PRIORITIES - 1 )
#define configTIMER_TASK_STACK_DEPTH               configMINIMAL_STACK_SIZE
#define configTIMER_QUEUE_LENGTH                   10

/* SMP: use single core for simplicity */
#define configNUMBER_OF_CORES                      1

/* Interrupt priority */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY       0
#define configMAX_API_CALL_INTERRUPT_PRIORITY       configMAX_SYSCALL_INTERRUPT_PRIORITY
#define INCLUDE_xTaskGetSchedulerState              1
#define INCLUDE_xTaskResumeFromISR                  1
#define INCLUDE_vTaskDelay                          1

/* configASSERT: use __builtin_trap to avoid dependency on taskDISABLE_INTERRUPTS
   which is not yet defined when portmacro.h uses configASSERT */
#define configASSERT(x) do { if (!(x)) __builtin_trap(); } while (0)

#endif /* FREERTOS_CONFIG_H */
