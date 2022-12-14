cmake_minimum_required(VERSION 3.13) # CMAKE (heading)
include(pico_sdk_import.cmake) # CMAKE -> PICO SDK (include)

project(logicanalyser C CXX ASM) # CMAKE ("project")
pico_sdk_init() # PICO SDK (initialization)

add_executable(logicanalyser) # CMAKE (target)

pico_generate_pio_header(logicanalyser ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated) # PICO SDK (PIO)

target_sources(logicanalyser PRIVATE logic_analyser.c) # CMAKE -> C (compiler)
target_link_libraries(logicanalyser PRIVATE pico_stdlib hardware_pio hardware_dma) # CMAKE -> C (linker)

pico_enable_stdio_usb(logicanalyser 1) # PICO SDK (USB)
pico_enable_stdio_uart(logicanalyser 0) # PICO SDK (USB)

pico_add_extra_outputs(logicanalyser) # PICO SDK (UF2)
