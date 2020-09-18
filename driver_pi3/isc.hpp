/**
 * @file log.h
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_PI3_ISC_H
#define DRIVER_DRIVER_PI3_ISC_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPiI2C.h>

#include "../error.hpp"
#include "log.hpp"
#include "types.hpp"

#define I2C_NUM_0 0
#define I2C_NUM_1 1

#define ISC_ACK_VAL 0x0
#define ISC_NACK_VAL 0x1

#define ISC_ACK_CHECK_ENABLE 0x01

#define ISC_ADDRESS_COUNT 0xFF

void isc_scan(i2c_port_t port);
error_t isc_probe(i2c_port_t port, uint8_t address);
error_t isc_master_read_bytes(i2c_port_t i2c_num, uint16_t slave_address,
                              uint8_t *data_rd, uint16_t size);
error_t isc_master_read_register(i2c_port_t i2c_num, uint16_t address,
                                 uint8_t regAdd, uint8_t *readBuff,
                                 uint16_t readBuffLen);

int isc_master_write(i2c_port_t i2c_num, uint16_t address, uint8_t *buff,
                     uint16_t len);
int isc_master_write_register(i2c_port_t i2c_num, uint16_t address,
                              uint8_t regAdd, uint8_t *writeBuff,
                              uint16_t writeBuffLen);
error_t isc_master_init(i2c_port_t port, gpio_num_t sda, gpio_num_t scl);

error_t isc_reset_tx(i2c_port_t port);

error_t isc_master_connect_device(uint16_t address);

#endif /* DRIVER_DRIVER_PI3_ISC_H */