/**
 * SwitchController - Interrupt-driven GPIO switch with debounce
 */
#pragma once

#include <cstdint>

#include "FreeRTOS.h"
#include "semphr.h"

class SwitchController {
public:
    SwitchController() = default;
    ~SwitchController() = default;

    SwitchController(const SwitchController&) = delete;
    SwitchController& operator=(const SwitchController&) = delete;

    /**
     * Initialize GPIO and IRQ, create semaphore.
     * @return true on success
     */
    bool init(uint pin, uint debounce_ms);

    SemaphoreHandle_t semaphore() const { return sem_; }
    uint pin() const { return pin_; }
    uint debounce_ms() const { return debounce_ms_; }

    /** Read current switch state (0 = pressed, 1 = released). */
    bool is_pressed() const;

private:
    static void irq_callback(uint gpio, uint32_t events);

    static inline SwitchController* s_instance{nullptr};

    uint pin_{};
    uint debounce_ms_{};
    SemaphoreHandle_t sem_{nullptr};
};
