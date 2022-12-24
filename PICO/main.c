#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/divider.h"
#include "pico/multicore.h"

#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/uart.h"
#include "hardware/sync.h"

#include "hardware/clocks.h"
#include "hardware/pll.h"
#include "hardware/vreg.h"

#include <string.h>
#include <math.h>

#include "./drivers/apa102/apa102.h"
#include "./utils/pt_cornell_rp2040_v1.h"

#include "./drivers/hall_effect/hall_effect.h"

#define GPIO_IRQ_EDGE_RISE 0x8u
#define WS2812_PIN 2
#include "./utils/picow_tcp_client.h"

// WiFi Consts:
#define WIFI_SSID "CenturyLink9790"
#define WIFI_PASSWORD "e6baa5ee3c7f99"

volatile static unsigned int detect_time;
volatile static unsigned int old_time;
volatile static unsigned int time_period = 10000;
volatile static unsigned int curr_rot;

#define GPIO_PIN 21

int flag = 0;

volatile static unsigned int last_rise;

void gpio_fall(unsigned int gpio, uint32_t events)
{
    detect_time = time_us_32();

    if (detect_time - old_time > 10000)
    {
        time_period = detect_time - old_time;
        // printf("Detected \n");
        // printf("time_period: %u", time_period);

        old_time = detect_time;
        curr_rot = 0;
    }
    // reset_flag = 1;
    // last_zero_time = detect_time;
    // printf("Time: %d\n", time_period);
}

// void gpio_rise(unsigned int gpio, uint32_t events)
// {
//     last_rise = time_us_32();
// }

static PT_THREAD(protothread_timing(struct pt *pt))
{
    // Mark beginning of thread
    PT_BEGIN(pt);
    volatile static int begin_time;
    volatile static int spare_time;
    while (1)
    {
        // Measure time at start of thread
        begin_time = time_us_32();
        apa102_write_strip(led_array[curr_rot % ROTATIONS], LED_NUM);
        curr_rot++;
        unsigned int theta_time = time_period / ROTATIONS;
        spare_time = theta_time - (time_us_32() - begin_time);

        PT_YIELD_usec(spare_time);
    }
    PT_END(pt);
}

void core1_main()
{
    // Add led timing thread

    pt_add_thread(protothread_timing);
    // //   Start the core 1 scheduler
    pt_schedule_start;
}

int main()
{
    vreg_set_voltage(VREG_VOLTAGE_1_30);
    set_sys_clock_khz(250000, true);

    stdio_init_all();

    apa102_init();

    uint8_t strip[LED_NUM][3] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 0},
        {0, 255, 0},
        {255, 0, 0}};
    apa102_write_strip(strip, LED_NUM);

    gpio_init(GPIO_PIN);
    gpio_set_dir(GPIO_PIN, 0);
    gpio_set_pulls(GPIO_PIN, true, false);

    gpio_set_irq_enabled_with_callback(GPIO_PIN, GPIO_IRQ_EDGE_RISE, 1, gpio_fall);
    gpio_set_irq_enabled_with_callback(GPIO_PIN, GPIO_IRQ_EDGE_FALL, 1, gpio_fall);

    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
        return 1;
    }
    cyw43_arch_enable_sta_mode();

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    printf("Connecting to WiFi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        printf("failed to connect.\n");
        return 1;
    }
    else
    {
        printf("Connected.\n");
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    }

    // cyw43_arch_deinit();

    // start core 1
    multicore_reset_core1();
    multicore_launch_core1(&core1_main);

    while (true)
    {
        if (run_tcp_client_test() == -1)
        {
            printf("FAILED\n");
        }
    }

    return 0;
}