#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"

void ws2812_put_pixel(uint32_t pixel_grb);

void ws2812_pattern_snakes(uint len, uint t);

void ws2812_init(int NUM_PIXELS, int WS2812_PIN, int IS_RGBW);