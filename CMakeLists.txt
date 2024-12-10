cmake_minimum_required(VERSION 3.13)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(myrp)

pico_sdk_init()

add_executable(myrp)

target_sources(myrp PRIVATE
        main.c
        ../lib/rp2350_playground.c
        ../lib/display/display.c
        ../lib/display/font_renderer.c
        ../lib/display/FreeSans5pt7b.c
        ../lib/display/FreeSans9pt7b.c
        ../lib/display/ssd1306.c)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib")

# target_compile_definitions(myrp PRIVATE GLITCH_DETECTOR_ON=0)

target_link_libraries(myrp PRIVATE
        pico_stdlib
        hardware_pio
        hardware_i2c
        hardware_powman
        )

# Signing and hashing
pico_sign_binary(myrp ${CMAKE_CURRENT_SOURCE_DIR}/ec_private_key.pem)
pico_hash_binary(myrp)
# pico_set_otp_key_output_file(myrp ${CMAKE_CURRENT_LIST_DIR}/otp.json)

pico_add_extra_outputs(myrp)

# enable usb output, disable uart output
pico_enable_stdio_usb(myrp 1)
pico_enable_stdio_uart(myrp 0)