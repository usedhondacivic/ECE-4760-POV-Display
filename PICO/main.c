#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/uart.h"
#include "./drivers/apa102/apa102.h"

#define WS2812_PIN 2
#define NUM_PIXELS 48
#include "./utils/picow_tcp_client.h"

#define WIFI_SSID "I14"
#define WIFI_PASSWORD "horwitz3"
#define TEST_TCP_SERVER_IP "172.20.10.2"


int main()
{
    stdio_init_all();

    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
        return 1;
    }
    cyw43_arch_enable_sta_mode();

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
    run_tcp_client_test();
    cyw43_arch_deinit();

    apa102_init();

    uint8_t strip[2][3] = {{255, 0, 0},
                           {0, 255, 0}};

    while (1)
    {
        apa102_write_strip(strip, 2);
    }
}