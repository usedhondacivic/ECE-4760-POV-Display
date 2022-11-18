cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(test_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico_w)
pico_sdk_init()

add_executable(main)

# For wifi:
target_include_directories(main PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
        )

pico_generate_pio_header(main ${CMAKE_CURRENT_LIST_DIR}/drivers/ws2812/ws2812.pio)

target_sources(main PRIVATE main.c drivers/ws2812/ws2812.c utils/picow_tcp_client.c drivers/apa102/apa102.c)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 1)
pico_add_extra_outputs(main)
target_link_libraries(main pico_stdlib hardware_pwm hardware_adc hardware_pio hardware_spi pico_cyw43_arch_lwip_threadsafe_background pico_bootsel_via_double_reset)