/**
 * @file spi.h
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2018-02-16
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_PI3_SPI_H
#define DRIVER_DRIVER_PI3_SPI_H

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include <cstring>
#include <iostream>
#include <memory>
#include <string>

#include "../error.hpp"
#include "log.hpp"
#include "semaphore.hpp"
#include "types.hpp"

#define SPI_SEMAPHORE_MAX_WAIT_MS 200
#define SPI_SEMAPHORE_MAX_WAIT_SETUP_MS 1000

void spi_init(gpio_num_t miso_gpio, gpio_num_t mosi_gpio, gpio_num_t clk_gpio);
spi_device_handle_t spi_add_device(int ss_pin);
error_t spi_send(spi_device_handle_t dev, uint8_t* txBuff, uint8_t* rxBuff,
                 uint16_t len);
void setup_spi_ready_interrupt(int intr_pin);

#endif /* DRIVER_DRIVER_PI3_SPI_H */
