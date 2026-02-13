/**
 * Unit tests for led_utils - pure LED/color utilities
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "include/led_utils.hpp"

#include <cstdint>
#include <cmath>

TEST_CASE("urgb_u32 encodes RGB to GRB format correctly") {
    // Black
    CHECK(led_utils::urgb_u32(0, 0, 0) == 0u);

    // Red only -> G=0, R=255, B=0 -> 0x00FF00
    CHECK(led_utils::urgb_u32(255, 0, 0) == 0x00FF00u);

    // Green only -> G=255, R=0, B=0 -> 0xFF0000
    CHECK(led_utils::urgb_u32(0, 255, 0) == 0xFF0000u);

    // Blue only -> G=0, R=0, B=255 -> 0x0000FF
    CHECK(led_utils::urgb_u32(0, 0, 255) == 0x0000FFu);

    // White
    CHECK(led_utils::urgb_u32(255, 255, 255) == 0xFFFFFFu);

    // Arbitrary values - GRB order: bits 0-7=B, 8-15=R, 16-23=G
    CHECK(led_utils::urgb_u32(1, 2, 3) == ((1u << 8) | (2u << 16) | 3u));
}

TEST_CASE("compute_pink_brightness returns values in [0, 1]") {
    for (unsigned i = 0; i < 20; ++i) {
        for (uint32_t t : {0u, 100u, 1000u, 10000u, 60000u}) {
            const float b = led_utils::compute_pink_brightness(i, t);
            CHECK(b >= 0.0f);
            CHECK(b <= 1.0f);
        }
    }
}

TEST_CASE("compute_pink_brightness varies with pixel index") {
    const uint32_t t = 0;
    float first = led_utils::compute_pink_brightness(0, t);
    bool found_different = false;
    for (unsigned i = 1; i < 14; ++i) {
        float curr = led_utils::compute_pink_brightness(i, t);
        if (curr != first) found_different = true;
    }
    CHECK(found_different);
}

TEST_CASE("compute_pink_brightness varies with time") {
    const unsigned pixel = 0;
    float prev = led_utils::compute_pink_brightness(pixel, 0);
    bool varied = false;
    for (uint32_t t = 100; t <= 5000; t += 100) {
        float curr = led_utils::compute_pink_brightness(pixel, t);
        if (curr != prev) varied = true;
        prev = curr;
    }
    CHECK(varied);
}

TEST_CASE("compute_pink_brightness wave formula at t=0") {
    // phase = i * 0.8, wave = 0.65 + 0.3*sin(phase), breath = 0.8 + 0.2*sin(0) = 0.8
    const uint32_t t = 0;
    float breath = 0.8f + 0.2f * std::sin(0.0f);
    CHECK(breath == doctest::Approx(0.8f));

    // Pixel 0: phase=0, sin(0)=0, wave=0.65, brightness=0.65*0.8=0.52
    float b0 = led_utils::compute_pink_brightness(0, t);
    CHECK(b0 == doctest::Approx(0.52f).epsilon(0.01f));

    // Pixel 1: phase=0.8, wave = 0.65 + 0.3*sin(0.8) ≈ 0.65 + 0.216 ≈ 0.866
    float b1 = led_utils::compute_pink_brightness(1, t);
    CHECK(b1 > 0.5f);
    CHECK(b1 <= 1.0f);
}
