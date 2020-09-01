/**
 * @file isc_slave.h
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_ESP32_ISC_SLAVE_H
#define DRIVER_DRIVER_ESP32_ISC_SLAVE_H

#include <driver/i2c.h>
#include <esp_err.h>
#include <esp_system.h>
#include <stdio.h>
#include <stdlib.h>

#include "../error.h"
#include "def.h"
#include "driver/driver.h"

#define ISC_ACK_VAL 0x0
#define ISC_NACK_VAL 0x1

#define ISC_ACK_CHECK_ENABLE 0x01

error_t isc_slave_init(i2c_port_t port, gpio_num_t sda, gpio_num_t scl,
                       uint16_t address);
int isc_slave_read_bytes(i2c_port_t i2c_num, uint8_t *data_rd, uint16_t size,
                         uint32_t timeout_ms);
error_t isc_reset_tx(i2c_port_t port);
error_t isc_slave_write_bytes(i2c_port_t port, uint8_t *data, uint16_t size,
                              uint32_t timeout_ms);

#endif /* DRIVER_DRIVER_ESP32_ISC_SLAVE_H */