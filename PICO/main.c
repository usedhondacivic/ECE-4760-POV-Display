#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/uart.h"
#include "./drivers/apa102/apa102.h"

#include <math.h>

#include "./drivers/hall_effect/hall_effect.h"

#define GPIO_IRQ_EDGE_RISE 0x8u
#define WS2812_PIN 2
#define LED_NUM 40
#include "./utils/picow_tcp_client.h"

// WiFi Consts:
#define WIFI_SSID "I14"
#define WIFI_PASSWORD "horwitz3"
#define TEST_TCP_SERVER_IP "172.20.10.2"

#define ROTATIONS 10

static unsigned int(detect_time);
static unsigned int(old_time);
static unsigned int(time_period);

#define GPIO_PIN 21

int flag = 0;

void gpio_callback1(unsigned int gpio, uint32_t events)
{
    detect_time = time_us_32();
    time_period = detect_time - old_time;
    // printf("Detected \n");
    // printf("time_period: %u", time_period);
    flag = !flag;
    old_time = detect_time;
}

const uint LED_PIN = 25;

int main()
{
    stdio_init_all();
    gpio_init(GPIO_PIN);
    gpio_set_dir(GPIO_PIN, 0);
    // printf("HELLO");
    // gpio_set_irq_enabled(GPIO_PIN, GPIO_IRQ_EDGE_RISE, 1);
    // gpio_set_irq_callback(gpio_callback1);

    gpio_set_irq_enabled_with_callback(GPIO_PIN, GPIO_IRQ_EDGE_RISE, 1, gpio_callback1);

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
    }
    if (run_tcp_client_test(led_array) == -1)
    {
        printf("FAILED\n");
        return 1;
    }
    cyw43_arch_deinit();

    // printf("PRINT FROM MAIN \n");
    // for (int r = 0; r < ROTATIONS; r++)
    // {
    //     for (int l = 0; l < NUM_PIXELS; l++)
    //     {
    //         printf("ROT: %d, LED: %d, R: %d, G:%d, B: %d\n", r, l, (*led_array)[r][l][0], (*led_array)[r][l][1], (*led_array)[r][l][2]);
    //     }
    // }

    apa102_init();

    // uint8_t strip[NUM_PIXELS][3] = {
    //     {255, 0, 0},
    //     {0, 255, 0},
    //     {0, 0, 0},
    //     {0, 255, 0},
    //     {255, 0, 0}};
    // apa102_write_strip(led_array[0], LED_NUM);
    int i = 0;
    while (1)
    {
        apa102_write_strip(led_array[i % 10], LED_NUM);
        i++;
        sleep_ms(100 / 5);
    }

    /*double t = 0;
    while (1)
    {
        t += 0.01;
        strip[0][0] = (int)(cos(t) * 120) + 125;
        strip[0][1] = (int)(cos((double)t + 3.14 / 2) * 120) + 125;

        apa102_write_strip(strip, 2);
    }*/

    /*int t = 0;
    double d = 0;
    while (1)
    {
        t++;
        d += 0.02;
        for (int i = 0; i < NUM_PIXELS; i++)
        {
            if (flag)
            {
                double a = d + ((double)i) / NUM_PIXELS;
                strip[i][0] = (int)(cos(a) * 120) + 125;
                strip[i][1] = (int)(cos(a * 1.25 + 3.14 / 2) * 120) + 125;
                strip[i][2] = (int)(cos(a * 1.33 + 3.14 / 3) * 120) + 125;
            }
            else
            {
                strip[i][0] = 0;
                strip[i][1] = 0;
                strip[i][2] = 0;
            }
        }
        apa102_write_strip(strip, NUM_PIXELS);*/
}