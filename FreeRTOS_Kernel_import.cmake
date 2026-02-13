# Locate FreeRTOS Kernel for RP2040
# Set FREERTOS_KERNEL_PATH or add FreeRTOS-Kernel as submodule in ./FreeRTOS-Kernel

if (DEFINED ENV{FREERTOS_KERNEL_PATH} AND (NOT FREERTOS_KERNEL_PATH))
    set(FREERTOS_KERNEL_PATH $ENV{FREERTOS_KERNEL_PATH})
    message(STATUS "Using FREERTOS_KERNEL_PATH from environment: ${FREERTOS_KERNEL_PATH}")
endif()

set(FREERTOS_KERNEL_RP2040_RELATIVE_PATH "portable/ThirdParty/GCC/RP2040")

if (NOT FREERTOS_KERNEL_PATH)
    # Check if FreeRTOS-Kernel is a submodule
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/CMakeLists.txt")
        set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel)
        message(STATUS "Using FreeRTOS-Kernel submodule: ${FREERTOS_KERNEL_PATH}")
    endif()
endif()

if (NOT FREERTOS_KERNEL_PATH)
    # Check as sibling of pico-sdk
    if (PICO_SDK_PATH AND EXISTS "${PICO_SDK_PATH}/../FreeRTOS-Kernel")
        set(FREERTOS_KERNEL_PATH ${PICO_SDK_PATH}/../FreeRTOS-Kernel)
        message(STATUS "Using FreeRTOS-Kernel as sibling of SDK: ${FREERTOS_KERNEL_PATH}")
    endif()
endif()

if (NOT FREERTOS_KERNEL_PATH)
    message(FATAL_ERROR "FreeRTOS Kernel not found. Add as submodule: git submodule add https://github.com/FreeRTOS/FreeRTOS-Kernel.git FreeRTOS-Kernel")
endif()

set(FREERTOS_KERNEL_PATH "${FREERTOS_KERNEL_PATH}" CACHE PATH "Path to FreeRTOS Kernel" FORCE)
get_filename_component(FREERTOS_KERNEL_PATH "${FREERTOS_KERNEL_PATH}" REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
if (NOT EXISTS ${FREERTOS_KERNEL_PATH}/${FREERTOS_KERNEL_RP2040_RELATIVE_PATH}/CMakeLists.txt)
    message(FATAL_ERROR "FreeRTOS-Kernel at '${FREERTOS_KERNEL_PATH}' does not contain RP2040 port")
endif()

add_subdirectory(${FREERTOS_KERNEL_PATH}/${FREERTOS_KERNEL_RP2040_RELATIVE_PATH} FREERTOS_KERNEL)
