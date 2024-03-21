add_library(
    mcp794xx_driver
    src/compile_time_date.h
    src/mcp794xx_driver.c
    src/mcp794xx_driver_basic.c
    src/mcp794xx_driver_interface.c
    src/mcp794xx_driver.h
    src/mcp794xx_driver_basic.h
    src/mcp794xx_driver_interface.h
)

target_include_directories(mcp794xx_driver PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")