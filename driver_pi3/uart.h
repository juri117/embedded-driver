/**
 * @file uart.h
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_PI3_UART_H
#define DRIVER_DRIVER_PI3_UART_H

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#include "../error.h"
#include "def.h"
#include "log.h"

#define UART_NUM_0 0
#define UART_NUM_1 1
#define UART_NUM_2 2
#define UART_NUM_3 3
#define UART_NUM_4 4
#define UART_NUM_5 5
#define UART_COUNT 6

typedef int uart_port_t;
typedef const char* uart_name_t;
typedef int gpio_num_t;

void uart_init(uart_port_t uart_num, uart_name_t uart_name, gpio_num_t dummy,
               int baudRate);

void uart_init_driver(uart_port_t uart_num, int rx_buffer_size,
                      int tx_buffer_size);
// void uart_enable_pattern_check(uart_port_t uartPort, char pattern);

int uart_read(uart_port_t uart_num, uint8_t* buf, uint32_t length,
              uint32_t timeout_ms);
int uart_write(uart_port_t uart_num, const uint8_t* buff, uint32_t len);

int uart_flush_rx(uart_port_t uart_num);
int uart_flush_tx(uart_port_t uart_num);

void uart_wait_for_tx_done(uart_port_t uart_num, uint32_t timeout_ms);

void uart_change_baudrate(uart_port_t uart_num, int baudRate);

#endif /* DRIVER_DRIVER_PI3_UART_H */
