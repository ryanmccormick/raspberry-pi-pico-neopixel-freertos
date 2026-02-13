/**
 * BoxLightsApp - Coordinates switch input and NeoPixel output
 */
#pragma once

#include "FreeRTOS.h"
#include "task.h"

#include "include/neopixel_controller.hpp"
#include "include/switch_controller.hpp"

class BoxLightsApp {
public:
    BoxLightsApp() = default;

    /**
     * Initialize NeoPixel and switch controllers.
     * @return true on success
     */
    bool init();

    /**
     * Create the main task and start FreeRTOS scheduler.
     * Does not return on success.
     */
    void run();

private:
    static void task_entry(void* param);
    void task_loop();

    NeoPixelController neopixel_;
    SwitchController switch_;
    bool lights_on_{false};
};
