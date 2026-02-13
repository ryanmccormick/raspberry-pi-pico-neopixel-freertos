/**
 * LED utilities - Pure functions for color and brightness calculations.
 * Kept hardware-independent for unit testing.
 */
#pragma once

#include <cstdint>
#include <cmath>

namespace led_utils {

/** Encode R,G,B into GRB format uint32_t for WS2812. */
inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return (static_cast<uint32_t>(r) << 8) |
           (static_cast<uint32_t>(g) << 16) |
           static_cast<uint32_t>(b);
}

/**
 * Compute brightness multiplier for pink wave animation at a given pixel and time.
 * Matches logic in NeoPixelController::update_pink_pattern.
 * @param pixel_index 0-based pixel index
 * @param time_ms time in milliseconds
 * @return brightness in [0.0, 1.0]
 */
inline float compute_pink_brightness(unsigned pixel_index, uint32_t time_ms) {
    const float phase = (pixel_index * 0.8f) + (time_ms * 0.008f);
    float wave = 0.65f + 0.3f * std::sin(phase);
    float breath = 0.8f + 0.2f * std::sin(time_ms * 0.003f);
    float brightness = wave * breath;
    if (brightness > 1.0f) brightness = 1.0f;
    if (brightness < 0.0f) brightness = 0.0f;
    return brightness;
}

}  // namespace led_utils
