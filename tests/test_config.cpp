/**
 * Unit tests for config constants - ensure values are sane
 */
#include <doctest/doctest.h>

#include "include/config.hpp"

TEST_CASE("config constants have valid ranges") {
    // GPIO pins on RP2040 are 0-29
    CHECK(config::SWITCH_PIN <= 29u);
    CHECK(config::NEOPIXEL_PIN <= 29u);

    // Reasonable pixel count
    CHECK(config::NUM_PIXELS > 0u);
    CHECK(config::NUM_PIXELS <= 1024u);  // Typical strip limit

    // Debounce: human-scale, not too long
    CHECK(config::DEBOUNCE_MS >= 10u);
    CHECK(config::DEBOUNCE_MS <= 200u);

    // WS2812 typically 400kHz-800kHz
    CHECK(config::WS2812_FREQ >= 400000u);
    CHECK(config::WS2812_FREQ <= 1200000u);

    // Animation period: smooth but not CPU-burning
    CHECK(config::ANIM_PERIOD_MS >= 10u);
    CHECK(config::ANIM_PERIOD_MS <= 100u);
}

TEST_CASE("config pin assignments are distinct") {
    CHECK(config::SWITCH_PIN != config::NEOPIXEL_PIN);
}
