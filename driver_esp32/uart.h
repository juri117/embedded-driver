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

#ifndef DRIVER_DRIVER_ESP32_UART_H
#define DRIVER_DRIVER_ESP32_UART_H

#include <driver/uart.h>
#include <esp_system.h>
#include <esp_timer.h>

#include "../error.h"
#include "def.h"
#include "time.h"

typedef gpio_num_t uart_name_t;

void uart_init(uart_port_t uart_num, gpio_num_t rtxPin, gpio_num_t rxPin,
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

#endif /* DRIVER_DRIVER_ESP32_UART_H */
