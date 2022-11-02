#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/uart.h"
#include "./drivers/ws2812/ws2812.h"

#define WS2812_PIN 2
#define NUM_PIXELS 48

int main()
{
    stdio_init_all();

    ws2812_init(NUM_PIXELS, WS2812_PIN, 0);

    ws2812_pattern_snakes(48, 0);
}