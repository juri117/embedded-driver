/**
 * @file spi.h
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_ESP32_SPI_H
#define DRIVER_DRIVER_ESP32_SPI_H

#include <driver/spi_master.h>
#include <esp_err.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <stdio.h>
#include <stdlib.h>

#include <cstring>
#include <iostream>
#include <memory>
#include <string>

#include "../error.h"
#include "def.h"
#include "driver/driver.h"
#include "semaphore.h"

#define SPI_SEMAPHORE_MAX_WAIT_MS 200
#define SPI_SEMAPHORE_MAX_WAIT_SETUP_MS 1000

void spi_init(gpio_num_t miso_gpio, gpio_num_t mosi_gpio, gpio_num_t clk_gpio);
spi_device_handle_t spi_add_device(int ss_pin);
error_t spi_send(spi_device_handle_t dev, uint8_t* txBuff, uint8_t* rxBuff,
                 uint16_t len);
void setup_spi_ready_interrupt(int intr_pin);

#endif /* DRIVER_DRIVER_ESP32_SPI_H */
