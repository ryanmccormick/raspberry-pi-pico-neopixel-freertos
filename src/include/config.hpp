/**
 * Box Lights - Configuration constants
 */
#pragma once

#include <cstdint>

namespace config {

inline constexpr uint32_t SWITCH_PIN   = 14;
inline constexpr uint32_t NEOPIXEL_PIN = 28;
inline constexpr uint32_t NUM_PIXELS   = 14;
inline constexpr uint32_t DEBOUNCE_MS  = 25;
inline constexpr uint32_t WS2812_FREQ  = 800000;
inline constexpr uint32_t ANIM_PERIOD_MS = 30;

}  // namespace config
