cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(mcp794xx_rtc_RP2040)

pico_sdk_init()

add_subdirectory(mcp794xx_driver)

add_executable(${PROJECT_NAME}
    main.c
)

# Add pico_stdlib library, add more if used
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    mcp794xx_driver
    hardware_i2c
)

# enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Need to generate UF2 file for upload to RP2040
pico_add_extra_outputs(${PROJECT_NAME})