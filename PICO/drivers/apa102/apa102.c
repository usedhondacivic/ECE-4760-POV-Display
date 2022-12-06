#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#include "apa102.h"

#define PIN_MISO 4
#define PIN_SCK 2
#define PIN_MOSI 3

#define SPI_PORT spi0

uint32_t start_marker = 0;
uint32_t end_marker = 0xFFFFFFFF;

void apa102_init()
{
    // This example will use SPI0 at 15MHz.
    spi_init(SPI_PORT, 20000 * 1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    bi_decl(bi_3pins_with_func(PIN_MISO, PIN_MOSI, PIN_SCK, GPIO_FUNC_SPI));
}

void apa102_test()
{
    uint32_t led = 0xFFFF0000;
    uint32_t buf[] = {
        start_marker,
        led,
        end_marker};

    const uint8_t test_buf[] = {
        0x00,
        0x00,
        0x00,
        0x00,       // Start cond
        0b11111111, // Brightness
        0xFF,       // RED
        0x00,       // BLUE
        0x00,       // GREEN
        0b11110001, // Brightness
        0x00,       // RED
        0xFF,       // BLUE
        0x00,       // GREEN
        0b11110001, // Brightness
        0x00,       // RED
        0x00,       // BLUE
        0xFF,       // GREEN
        0xFF,
        0xFF,
        0xFF,
        0xFF // End cond
    };
    spi_write_blocking(SPI_PORT, test_buf, 16);
}

void apa102_write_strip(uint8_t (*color_data)[3], uint16_t num_leds)
{
    uint8_t spi_buffer[num_leds * 4 + 8];
    spi_buffer[0] = 0x00;
    spi_buffer[1] = 0x00;
    spi_buffer[2] = 0x00;
    spi_buffer[3] = 0x00;
    for (int i = 0; i < num_leds; i++)
    {
        if (i < 5)
        {
            spi_buffer[i * 4 + 4] = 0b11100001; // Not full brightness so I don't kill my retinas
        }
        else
        {
            spi_buffer[i * 4 + 4] = 0b11100011; // Not full brightness so I don't kill my retinas
        }
        // APA102's we have are RBG instead of RGB,so flip the BG inputs here.
        spi_buffer[i * 4 + 5] = color_data[i][0];
        spi_buffer[i * 4 + 6] = color_data[i][2];
        spi_buffer[i * 4 + 7] = color_data[i][1];
    }
    spi_buffer[num_leds * 4 + 4] = 0xFF;
    spi_buffer[num_leds * 4 + 5] = 0xFF;
    spi_buffer[num_leds * 4 + 6] = 0xFF;
    spi_buffer[num_leds * 4 + 7] = 0xFF;

    spi_write_blocking(SPI_PORT, spi_buffer, num_leds * 4 + 8);
}