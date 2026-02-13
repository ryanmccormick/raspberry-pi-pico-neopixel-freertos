/**
 * BoxLightsApp - Application coordinator implementation
 */
#include "include/box_lights_app.hpp"
#include "include/config.hpp"

void BoxLightsApp::task_entry(void* param) {
    auto* app = static_cast<BoxLightsApp*>(param);
    if (app != nullptr) {
        app->task_loop();
    }
}

void BoxLightsApp::task_loop() {
    auto sem = switch_.semaphore();
    const auto debounce_ticks = pdMS_TO_TICKS(config::DEBOUNCE_MS);
    const auto anim_ticks = pdMS_TO_TICKS(config::ANIM_PERIOD_MS);

    for (;;) {
        const TickType_t timeout = lights_on_ ? anim_ticks : portMAX_DELAY;

        if (xSemaphoreTake(sem, timeout) == pdTRUE) {
            // Drain extra triggers from switch bounce
            while (xSemaphoreTake(sem, 0) == pdTRUE) {}
            vTaskDelay(debounce_ticks);

            lights_on_ = switch_.is_pressed();
            if (!lights_on_) {
                neopixel_.off();
            }
        } else if (lights_on_) {
            // Timeout while on: update animation
            neopixel_.update_pink_pattern();
        }
    }
}

bool BoxLightsApp::init() {
    if (!switch_.init(config::SWITCH_PIN, config::DEBOUNCE_MS)) {
        return false;
    }

    if (!neopixel_.init(
            config::NEOPIXEL_PIN,
            config::NUM_PIXELS,
            config::WS2812_FREQ)) {
        return false;
    }

    neopixel_.off();
    return true;
}

void BoxLightsApp::run() {
    configASSERT(switch_.semaphore() != nullptr);

    xTaskCreate(
        task_entry,
        "BoxLights",
        256,
        this,
        tskIDLE_PRIORITY + 1,
        nullptr
    );

    vTaskStartScheduler();

    // Should not reach here
    for (;;) {
        configASSERT(0);
    }
}
