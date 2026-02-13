/**
 * NeoPixelController - WS2812 LED strip control implementation
 */
#include "include/neopixel_controller.hpp"
#include "include/config.hpp"
#include "pico/time.h"
#include "ws2812.pio.h"

#include <cmath>
#include <cstdint>

namespace {

constexpr uint8_t PINK_R = 255;
constexpr uint8_t PINK_G = 105;
constexpr uint8_t PINK_B = 180;

}  // namespace

bool NeoPixelController::init(uint pin, uint num_pixels, uint freq_hz) {
    num_pixels_ = num_pixels;
    const bool ok = pio_claim_free_sm_and_add_program_for_gpio_range(
        &ws2812_program, &pio_, &sm_, &offset_,
        pin, 1, true
    );
    if (!ok) return false;
    ws2812_program_init(pio_, sm_, offset_, pin, static_cast<float>(freq_hz), false);
    return true;
}

uint32_t NeoPixelController::urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return (static_cast<uint32_t>(r) << 8) |
           (static_cast<uint32_t>(g) << 16) |
           static_cast<uint32_t>(b);
}

void NeoPixelController::put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio_, sm_, pixel_grb << 8u);
}

void NeoPixelController::set_all(uint32_t color) {
    for (uint i = 0; i < num_pixels_; i++) {
        put_pixel(color);
    }
}

void NeoPixelController::off() {
    set_all(0);
}

void NeoPixelController::update_pink_pattern() {
    const uint32_t t = to_ms_since_boot(get_absolute_time());

    for (uint i = 0; i < num_pixels_; i++) {
        float phase = (i * 0.8f) + (t * 0.008f);
        float wave  = 0.65f + 0.3f * std::sin(phase);
        float breath = 0.8f + 0.2f * std::sin(t * 0.003f);
        float brightness = wave * breath;
        if (brightness > 1.0f) brightness = 1.0f;
        if (brightness < 0.0f) brightness = 0.0f;

        const uint8_t r = static_cast<uint8_t>(PINK_R * brightness);
        const uint8_t g = static_cast<uint8_t>(PINK_G * brightness);
        const uint8_t b = static_cast<uint8_t>(PINK_B * brightness);

        put_pixel(urgb_u32(r, g, b));
    }
}
