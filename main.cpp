/**
 * Box Lights - Raspberry Pi Pico with FreeRTOS
 *
 * Interrupt-driven switch on PIN 14 controls NeoPixel lights on PIN 28.
 * Switch typically: one side to PIN 14, other to GND. Use internal pull-up.
 */

#include <cstdint>
#include <cstdlib>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

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

// Pin configuration
#define SWITCH_PIN   14
#define NEOPIXEL_PIN 28
#define NUM_PIXELS   14   // Adjust for your strip length
#define DEBOUNCE_MS  25
#define WS2812_FREQ  800000

// FreeRTOS handles
static SemaphoreHandle_t switch_sem = nullptr;

// NeoPixel state
static PIO pio;
static uint sm;
static uint offset;
static bool lights_on = false;

// GRB format for WS2812
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

static void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

static void set_all_pixels(uint32_t color) {
    for (uint i = 0; i < NUM_PIXELS; i++) {
        put_pixel(color);
    }
}

static void neopixel_off() {
    set_all_pixels(0);
}

static void neopixel_on() {
    // Warm white, or customize: e.g. urgb_u32(255, 180, 100)
    set_all_pixels(urgb_u32(255, 255, 255));
}

// GPIO interrupt callback - runs in IRQ context
// Must use FreeRTOS ISR-safe primitives (xSemaphoreGiveFromISR)
void gpio_irq_callback(uint gpio, uint32_t events) {
    if (gpio == SWITCH_PIN) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(switch_sem, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

// FreeRTOS task: watches semaphore, sets lights from switch state (with debounce)
void neopixel_task(void* /*pvParameters*/) {
    for (;;) {
        if (xSemaphoreTake(switch_sem, portMAX_DELAY) == pdTRUE) {
            // Drain any extra triggers from switch bounce
            while (xSemaphoreTake(switch_sem, 0) == pdTRUE) {}
            vTaskDelay(pdMS_TO_TICKS(DEBOUNCE_MS));
            // Switch pressed (low) = lights on, released (high) = lights off
            lights_on = (gpio_get(SWITCH_PIN) == 0);
            if (lights_on) {
                neopixel_on();
            } else {
                neopixel_off();
            }
        }
    }
}

int main() {
    stdio_init_all();

    // Create semaphore for switch -> task communication
    switch_sem = xSemaphoreCreateBinary();
    configASSERT(switch_sem != nullptr);

    // Configure switch pin: input with pull-up (switch to GND = active low)
    gpio_init(SWITCH_PIN);
    gpio_set_dir(SWITCH_PIN, GPIO_IN);
    gpio_pull_up(SWITCH_PIN);
    gpio_set_irq_enabled_with_callback(
        SWITCH_PIN,
        GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,  // Trigger on press and release
        true,
        gpio_irq_callback
    );

    // Initialize WS2812 PIO
    bool ok = pio_claim_free_sm_and_add_program_for_gpio_range(
        &ws2812_program, &pio, &sm, &offset,
        NEOPIXEL_PIN, 1, true
    );
    configASSERT(ok);

    ws2812_program_init(pio, sm, offset, NEOPIXEL_PIN, WS2812_FREQ, false);

    // Start with lights off
    neopixel_off();

    // Create NeoPixel task
    xTaskCreate(
        neopixel_task,
        "NeoPixel",
        256,
        nullptr,
        tskIDLE_PRIORITY + 1,
        nullptr
    );

    // Start scheduler (never returns)
    vTaskStartScheduler();

    for (;;) {
        configASSERT(0);  // Should not reach here
    }
    return 0;
}
