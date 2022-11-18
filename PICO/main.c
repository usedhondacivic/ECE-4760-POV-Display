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
#define NUM_PIXELS 48
// #include "./utils/picow_tcp_client.h"

// WiFi Consts:
#define WIFI_SSID "I14"
#define WIFI_PASSWORD "horwitz3"
#define TEST_TCP_SERVER_IP "172.20.10.2"

#define LED_NUM 4
#define ROTATIONS 10

static unsigned int(detect_time);
static unsigned int(old_time);
static unsigned int(time_period);

uint8_t (*led_array)[ROTATIONS][LED_NUM][3];

#define GPIO_PIN 21

void gpio_callback1(unsigned int gpio, uint32_t events)
{
    detect_time = time_us_32();
    time_period = detect_time - old_time;
    printf("Detected \n");
    printf("time_period: %u", time_period);
    old_time = detect_time;
}

int main()
{
    stdio_init_all();
    gpio_init(GPIO_PIN);
    gpio_set_dir(GPIO_PIN, 0);
    printf("HELLO");
    // gpio_set_irq_enabled(GPIO_PIN, GPIO_IRQ_EDGE_RISE, 1);
    // gpio_set_irq_callback(gpio_callback1);

    gpio_set_irq_enabled_with_callback(GPIO_PIN, GPIO_IRQ_EDGE_RISE, 1, gpio_callback1);

    while (1)
    {
    }

    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
        return 1;
    }
    cyw43_arch_enable_sta_mode();

    // printf("Connecting to WiFi...\n");
    // if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
    // {
    //     printf("failed to connect.\n");
    //     return 1;
    // }
    // else
    // {
    //     printf("Connected.\n");
    // }
    // run_tcp_client_test(led_array);
    // cyw43_arch_deinit();

    apa102_init();

    uint8_t strip[2][3] = {{255, 0, 0},
                           {0, 0, 0}};

    /*double t = 0;
    while (1)
    {
        t += 0.01;
        strip[0][0] = (int)(cos(t) * 120) + 125;
        strip[0][1] = (int)(cos((double)t + 3.14 / 2) * 120) + 125;

        apa102_write_strip(strip, 2);
    }*/

    int t = 0;
    double d = 0;
    while (1)
    {
        t++;
        d += 0.001;
        if (t % 2 == 1)
        {
            strip[0][0] = 255;
            strip[0][1] = 255;
            strip[0][2] = 255;
        }
        else
        {
            strip[0][0] = 0;
            strip[0][1] = 0;
            strip[0][2] = 0;
        }
        strip[1][0] = (int)(cos(d) * 120) + 125;
        strip[1][1] = (int)(cos((double)d + 3.14 / 2) * 120) + 125;
        apa102_write_strip(strip, 2);
    }
}