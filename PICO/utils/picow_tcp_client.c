/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// #include <string.h>
// #include <time.h>

// #include "pico/stdlib.h"
// #include "pico/cyw43_arch.h"

// #include "lwip/pbuf.h"
// #include "lwip/tcp.h"

// #if !defined(TEST_TCP_SERVER_IP)
// #error TEST_TCP_SERVER_IP not defined
// #endif

#include "picow_tcp_client.h"

// #define TEST_TCP_SERVER_IP "172.20.10.2"
#define TEST_TCP_SERVER_IP "192.168.0.132"

#define TCP_PORT 4242
#define DEBUG_printf printf

// #define LED_NUM 40
#define RPB 2
#define PACKET_NUM (ROTATIONS / RPB)
#define BUF_SIZE (LED_NUM * RPB * 3)

#define TEST_ITERATIONS 10
#define POLL_TIME_S 5

uint8_t led_array[ROTATIONS][LED_NUM][3];

typedef struct TCP_CLIENT_T_
{
    struct tcp_pcb *tcp_pcb;
    ip_addr_t remote_addr;
    uint8_t buffer[BUF_SIZE];
    int buffer_len;
    int sent_len;
    bool complete;
    int run_count;
    bool connected;
    int curr_rot;
} TCP_CLIENT_T;

static int dump_bytes(const uint8_t *bptr, uint32_t len, int curr_rot)
{
    // unsigned int start_i = curr_rot * ROTATIONS * LED_NUM * 3;
    static unsigned int arr_i = 0;
    unsigned int led_i;
    unsigned int rot_i;
    unsigned char rgb_i;
    uint8_t x;
    // printf("dump_bytes %d\n", len);
    for (unsigned int i = 0; i < len; i++)
    {
        x = bptr[i];
        // arr_i = start_i + i;
        rgb_i = arr_i % 3;
        led_i = (arr_i / 3) % LED_NUM;
        rot_i = (arr_i / (LED_NUM * 3)) % ROTATIONS;
        led_array[rot_i][led_i][rgb_i] = x;
        arr_i++;
        // printf("Should be: %d, Got: %d | ", x, led_array[rot_i][led_i][rgb_i]);
        // printf("rot: %d, led: %d, rgb: %d\n", rot_i, led_i, rgb_i);
    }
    // printf("\n");
    return rot_i + 1;
}
#define DUMP_BYTES dump_bytes

static err_t tcp_client_close(void *arg)
{
    TCP_CLIENT_T *state = (TCP_CLIENT_T *)arg;
    err_t err = ERR_OK;
    if (state->tcp_pcb != NULL)
    {
        tcp_arg(state->tcp_pcb, NULL);
        tcp_poll(state->tcp_pcb, NULL, 0);
        tcp_sent(state->tcp_pcb, NULL);
        tcp_recv(state->tcp_pcb, NULL);
        tcp_err(state->tcp_pcb, NULL);
        err = tcp_close(state->tcp_pcb);
        if (err != ERR_OK)
        {
            DEBUG_printf("close failed %d, calling abort\n", err);
            tcp_abort(state->tcp_pcb);
            err = ERR_ABRT;
        }
        state->tcp_pcb = NULL;
    }
    return err;
}

// Called with results of operation
static err_t tcp_result(void *arg, int status)
{
    TCP_CLIENT_T *state = (TCP_CLIENT_T *)arg;
    if (status == 0)
    {
        DEBUG_printf("test success\n");
    }
    else
    {
        DEBUG_printf("test failed %d\n", status);
    }
    state->complete = true;
    return tcp_client_close(arg);
}

static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    TCP_CLIENT_T *state = (TCP_CLIENT_T *)arg;
    DEBUG_printf("tcp_client_sent %u\n", len);
    state->sent_len += len;

    if (state->sent_len >= BUF_SIZE)
    {

        state->run_count++;
        if (state->run_count >= TEST_ITERATIONS)
        {
            tcp_result(arg, 0);
            return ERR_OK;
        }

        // We should receive a new buffer from the server
        state->buffer_len = 0;
        state->sent_len = 0;
        DEBUG_printf("Waiting for buffer from server\n");
    }

    return ERR_OK;
}

static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    TCP_CLIENT_T *state = (TCP_CLIENT_T *)arg;
    if (err != ERR_OK)
    {
        printf("connect failed %d\n", err);
        return tcp_result(arg, err);
    }
    state->connected = true;
    DEBUG_printf("Waiting for buffer from server\n");
    return ERR_OK;
}

static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb)
{
    DEBUG_printf("tcp_client_poll\n");
    return tcp_result(arg, -1); // no response is an error?
}

static void tcp_client_err(void *arg, err_t err)
{
    if (err != ERR_ABRT)
    {
        DEBUG_printf("tcp_client_err %d\n", err);
        tcp_result(arg, err);
    }
}

err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    TCP_CLIENT_T *state = (TCP_CLIENT_T *)arg;
    if (!p)
    {
        return tcp_result(arg, -1);
    }
    // this method is callback from lwIP, so cyw43_arch_lwip_begin is not required, however you
    // can use this method to cause an assertion in debug mode, if this method is called when
    // cyw43_arch_lwip_begin IS needed
    cyw43_arch_lwip_check();
    int rot;
    state->curr_rot = 0;
    if (p->tot_len > 0)
    {
        // DEBUG_printf("recv %d err %d\n", p->tot_len, err);
        for (struct pbuf *q = p; q != NULL; q = q->next)
        {
            rot = DUMP_BYTES(q->payload, q->len, state->curr_rot);
            if (rot != state->curr_rot + RPB)
                printf("Got wrong rotation increase: Should be: %d, got: %d\n", state->curr_rot + RPB, rot);
            state->curr_rot += RPB;
        }
        /*printf("PRINT FROM CLIENT");
        for (int r = 0; r < ROTATIONS; r++)
        {
            for (int l = 0; l < LED_NUM; l++)
            {
                printf("ROT: %d, LED: %d, R: %d, G:%d, B: %d\n", r, l, led_array[r][l][0], led_array[r][l][1], led_array[r][l][2]);
            }
        }*/
        // Receive the buffer
        const uint16_t buffer_left = BUF_SIZE - state->buffer_len;
        state->buffer_len += pbuf_copy_partial(p, state->buffer + state->buffer_len,
                                               p->tot_len > buffer_left ? buffer_left : p->tot_len, 0);
        tcp_recved(tpcb, p->tot_len);
    }
    pbuf_free(p);

    state->run_count++;
    printf("count: %d\n", state->run_count);
    if (state->run_count >= PACKET_NUM)
    {
        tcp_result(arg, 0);
        return ERR_OK;
    }

    // If we have received the whole buffer, send it back to the server
    // if (state->buffer_len == BUF_SIZE) {
    //     DEBUG_printf("Writing %d bytes to server\n", state->buffer_len);
    //     err_t err = tcp_write(tpcb, state->buffer, state->buffer_len, TCP_WRITE_FLAG_COPY);
    //     if (err != ERR_OK) {
    //         DEBUG_printf("Failed to write data %d\n", err);
    //         return tcp_result(arg, -1);
    //     }
    // }
    return ERR_OK;
}

static bool tcp_client_open(void *arg)
{
    TCP_CLIENT_T *state = (TCP_CLIENT_T *)arg;
    DEBUG_printf("Connecting to %s port %u\n", ip4addr_ntoa(&state->remote_addr), TCP_PORT);
    state->tcp_pcb = tcp_new_ip_type(IP_GET_TYPE(&state->remote_addr));
    if (!state->tcp_pcb)
    {
        DEBUG_printf("failed to create pcb\n");
        return false;
    }

    tcp_arg(state->tcp_pcb, state);
    tcp_poll(state->tcp_pcb, tcp_client_poll, POLL_TIME_S * 2);
    tcp_sent(state->tcp_pcb, tcp_client_sent);
    tcp_recv(state->tcp_pcb, tcp_client_recv);
    tcp_err(state->tcp_pcb, tcp_client_err);

    state->buffer_len = 0;

    // cyw43_arch_lwip_begin/end should be used around calls into lwIP to ensure correct locking.
    // You can omit them if you are in a callback from lwIP. Note that when using pico_cyw_arch_poll
    // these calls are a no-op and can be omitted, but it is a good practice to use them in
    // case you switch the cyw43_arch type later.
    cyw43_arch_lwip_begin();
    err_t err = tcp_connect(state->tcp_pcb, &state->remote_addr, TCP_PORT, tcp_client_connected);
    cyw43_arch_lwip_end();

    return err == ERR_OK;
}

// Perform initialisation
static TCP_CLIENT_T *tcp_client_init()
{
    TCP_CLIENT_T *state = calloc(1, sizeof(TCP_CLIENT_T));
    if (!state)
    {
        DEBUG_printf("failed to allocate state\n");
        return NULL;
    }
    ip4addr_aton(TEST_TCP_SERVER_IP, &state->remote_addr);
    state->curr_rot = 0;
    return state;
}

int run_tcp_client_test()
{
    TCP_CLIENT_T *state = tcp_client_init();
    if (!state)
    {
        return -1;
    }
    if (!tcp_client_open(state))
    {
        return tcp_result(state, -1);
    }
    while (!state->complete)
    {
        // the following #ifdef is only here so this same example can be used in multiple modes;
        // you do not need it in your code
#if PICO_CYW43_ARCH_POLL
        // if you are using pico_cyw43_arch_poll, then you must poll periodically from your
        // main loop (not from a timer) to check for WiFi driver or lwIP work that needs to be done.
        cyw43_arch_poll();
        sleep_ms(1);
#else
        // if you are not using pico_cyw43_arch_poll, then WiFI driver and lwIP work
        // is done via interrupt in the background. This sleep is just an example of some (blocking)
        // work you might be doing.
        sleep_ms(1000);
#endif
    }
    // free(state);
    return 0;
}

// int main() {
//     stdio_init_all();

//     if (cyw43_arch_init()) {
//         DEBUG_printf("failed to initialise\n");
//         return 1;
//     }
//     cyw43_arch_enable_sta_mode();

//     printf("Connecting to WiFi...\n");
//     if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
//         printf("failed to connect.\n");
//         return 1;
//     } else {
//         printf("Connected.\n");
//     }
//     run_tcp_client_test();
//     cyw43_arch_deinit();
//     return 0;
// }