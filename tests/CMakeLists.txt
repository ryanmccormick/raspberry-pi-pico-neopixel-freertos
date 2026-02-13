# Unit tests for box-lights - host build (no Pico SDK)
cmake_minimum_required(VERSION 3.13)
project(box_lights_tests CXX)
set(CMAKE_CXX_STANDARD 17)

# Fetch doctest
include(FetchContent)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG        v2.4.12
)
FetchContent_MakeAvailable(doctest)

# Host-only test executable - no Pico SDK
add_executable(box_lights_tests
    test_led_utils.cpp
    test_config.cpp
)

target_include_directories(box_lights_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_link_libraries(box_lights_tests PRIVATE
    doctest::doctest
)

# CTest integration
enable_testing()
add_test(NAME box_lights_tests COMMAND box_lights_tests)
