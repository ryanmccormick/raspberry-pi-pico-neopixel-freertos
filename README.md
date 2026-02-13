# Box Lights

Raspberry Pi Pico project with FreeRTOS: interrupt-driven switch on **PIN 14** controls NeoPixel (WS2812) lights on **PIN 28**.

## Hardware

![Pinout](docs/pinout.png)

- **Switch**: Connect one side to **GPIO 14**, other to **GND**. Internal pull-up is used (active-low).
- **NeoPixels**: Data line to **GPIO 28**, power as per your strip.

## Prerequisites

1. **pico-sdk** – [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk)
   ```bash
   export PICO_SDK_PATH=/path/to/pico-sdk
   ```

2. **FreeRTOS-Kernel** – Add as submodule (or set `FREERTOS_KERNEL_PATH`):
   ```bash
   git submodule add https://github.com/FreeRTOS/FreeRTOS-Kernel.git FreeRTOS-Kernel
   git submodule update --init --recursive
   ```

## Build

Use the build script:

```bash
./run-build.sh
```

Or manually:

```bash
mkdir build && cd build
cmake -DPICO_SDK_PATH=/path/to/pico-sdk ..
make
```

Set `PICO_SDK_PATH` in your environment if needed, or the script may use it automatically.

## Test

Run the test suite with:

```bash
./run-tests.sh
```

## Flash

Copy `box_lights.uf2` to the Pico while holding BOOTSEL.

## Configuration

Configuration constants are defined in `src/include/config.hpp`:

| Constant         | Default | Description                    |
|------------------|---------|--------------------------------|
| `SWITCH_PIN`     | 14      | GPIO for the switch            |
| `NEOPIXEL_PIN`   | 28      | GPIO for NeoPixel data         |
| `NUM_PIXELS`     | 14      | Number of LEDs in strip        |
| `DEBOUNCE_MS`    | 25      | Switch debounce delay (ms)     |
| `WS2812_FREQ`    | 800000  | WS2812 clock frequency (Hz)    |
| `ANIM_PERIOD_MS` | 30      | Animation update period (ms)   |

For colors and animation, edit `neopixel_controller.cpp` (PINK_R/G/B) and `led_utils.hpp` (wave formula).
