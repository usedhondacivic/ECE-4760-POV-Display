/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "hardware/gpio.h"
#include "hardware/sync.h"
#include "pico/stdlib.h"
#include "hardware/structs/iobank0.h"
#include "hardware/irq.h"

#include "hall_effect.h"

#define	MY_SM	0
#define GPIO_PIN 21



void gpio_callback(uint gpio, uint32_t events) {
     printf("Detected");
}