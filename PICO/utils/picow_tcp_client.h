/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#define LED_NUM 4
#define ROTATIONS 10

// static void dump_bytes(const uint8_t *bptr, uint32_t len) ;

// typedef struct TCP_CLIENT_T_ {
//     struct tcp_pcb *tcp_pcb;
//     ip_addr_t remote_addr;
//     uint8_t buffer[BUF_SIZE];
//     int buffer_len;
//     int sent_len;
//     bool complete;
//     int run_count;
//     bool connected;
// } TCP_CLIENT_T;

static err_t tcp_client_close(void *arg) ;

// Called with results of operation
static err_t tcp_result(void *arg, int status) ;

static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) ;

static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err) ;

static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb);

static void tcp_client_err(void *arg, err_t err) ;

err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) ;

static bool tcp_client_open(void *arg) ;

// Perform initialisation
// static TCP_CLIENT_T* tcp_client_init(void) ;

void run_tcp_client_test(uint8_t (*led_array)[ROTATIONS][LED_NUM][3]) ;