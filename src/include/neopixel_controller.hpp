/**
 * NeoPixelController - Manages WS2812 LED strip via PIO
 */
#pragma once

#include <cstdint>

#include "hardware/pio.h"
#include "pico/types.h"

class NeoPixelController {
public:
    NeoPixelController() = default;

    /**
     * Initialize PIO and claim state machine for WS2812.
     * @return true on success, false if no free SM available
     */
    bool init(uint pin, uint num_pixels, uint freq_hz);

    /** Set all pixels to a single color (GRB format). */
    void set_all(uint32_t color);

    /** Turn off all pixels. */
    void off();

    /** Update animated pink wave pattern. */
    void update_pink_pattern();

    uint num_pixels() const { return num_pixels_; }

private:
    static uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
    void put_pixel(uint32_t pixel_grb);

    PIO pio_{};
    uint sm_{};
    uint offset_{};
    uint num_pixels_{};
};
