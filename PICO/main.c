#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/uart.h"
#include "./drivers/apa102/apa102.h"

#define WS2812_PIN 2
#define NUM_PIXELS 48

int main()
{
    stdio_init_all();

    apa102_init();

    while (1)
    {
        apa102_test();
    }
}