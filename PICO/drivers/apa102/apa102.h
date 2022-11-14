#pragma once
#include <stdio.h>
#include <stdlib.h>

void apa102_init();

void apa102_test();

void apa102_write_strip(uint8_t (*color_data)[3], uint16_t length);