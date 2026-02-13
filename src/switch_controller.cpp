/**
 * SwitchController - GPIO switch with IRQ and debounce implementation
 */
#include "include/switch_controller.hpp"
#include "hardware/gpio.h"

bool SwitchController::init(uint pin, uint debounce_ms) {
    pin_ = pin;
    debounce_ms_ = debounce_ms;

    sem_ = xSemaphoreCreateBinary();
    if (sem_ == nullptr) return false;

    s_instance = this;

    gpio_init(pin_);
    gpio_set_dir(pin_, GPIO_IN);
    gpio_pull_up(pin_);
    gpio_set_irq_enabled_with_callback(
        pin_,
        GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,
        true,
        irq_callback
    );

    return true;
}

bool SwitchController::is_pressed() const {
    return gpio_get(pin_) == 0;
}

void SwitchController::irq_callback(uint gpio, uint32_t /*events*/) {
    if (s_instance != nullptr && gpio == s_instance->pin_) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(s_instance->sem_, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
