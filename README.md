# Box Lights

Raspberry Pi Pico project with FreeRTOS: interrupt-driven switch on **PIN 14** controls NeoPixel (WS2812) lights on **PIN 28**.

## Hardware

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

```bash
mkdir build && cd build
cmake -DPICO_SDK_PATH=/path/to/pico-sdk ..
make
```

Or set `PICO_SDK_PATH` in your environment:

```bash
export PICO_SDK_PATH=/path/to/pico-sdk
mkdir build && cd build
cmake ..
make
```

## Flash

Copy `box_lights.uf2` to the Pico while holding BOOTSEL.

## Configuration

| Constant       | Default | Description               |
|----------------|---------|---------------------------|
| `SWITCH_PIN`   | 14      | GPIO for the switch       |
| `NEOPIXEL_PIN` | 28      | GPIO for NeoPixel data    |
| `NUM_PIXELS`   | 8       | Number of LEDs in strip   |

Edit `main.cpp` to change colors or behavior (e.g. `neopixel_on()` uses warm white).
