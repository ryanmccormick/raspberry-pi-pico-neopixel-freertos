/**
 * Box Lights - Raspberry Pi Pico with FreeRTOS
 *
 * Interrupt-driven switch on PIN 14 controls NeoPixel lights on PIN 28.
 * Switch: one side to PIN 14, other to GND. Uses internal pull-up.
 */

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

#include "include/box_lights_app.hpp"

/* Required when configKERNEL_PROVIDED_STATIC_MEMORY=0 */
#if ( configSUPPORT_STATIC_ALLOCATION == 1 ) && ( configKERNEL_PROVIDED_STATIC_MEMORY == 0 )
extern "C" {
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    configSTACK_DEPTH_TYPE *puxIdleTaskStackSize )
{
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *puxIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

#if ( configUSE_TIMERS == 1 )
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     configSTACK_DEPTH_TYPE *puxTimerTaskStackSize )
{
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *puxTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
#endif
}
#endif

int main() {
    stdio_init_all();

    BoxLightsApp app;
    configASSERT(app.init());

    app.run();

    for (;;) {
        configASSERT(0);
    }
    return 0;
}
