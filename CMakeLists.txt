cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(pico_sdk_import.cmake)

project(box_lights C CXX ASM)

pico_sdk_init()

# FreeRTOS config - must be set before including FreeRTOS
set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/config)
include(FreeRTOS_Kernel_import.cmake)

# FreeRTOS kernel (must be before pico_sdk_init on older SDK versions - include at top)
# Build with FreeRTOS
add_executable(box_lights)

# Generate PIO header for WS2812
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)
pico_generate_pio_header(box_lights ${CMAKE_CURRENT_SOURCE_DIR}/src/pio/ws2812.pio
    OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated
)

target_sources(box_lights PRIVATE
    src/main.cpp
    src/neopixel_controller.cpp
    src/switch_controller.cpp
    src/box_lights_app.cpp
)

target_include_directories(box_lights PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}/generated
)

target_link_libraries(box_lights PRIVATE
    pico_stdlib
    hardware_pio
    hardware_gpio
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)

pico_add_extra_outputs(box_lights)
