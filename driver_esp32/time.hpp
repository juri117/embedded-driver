/**
 * @file time.h
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_ESP32_TIME_H
#define DRIVER_DRIVER_ESP32_TIME_H

#include <driver/gpio.h>
#include <driver/timer.h>
#include <driver/uart.h>
#include <esp_system.h>

#include "../error.hpp"

uint32_t get_time_system_ms();
uint64_t get_time_system_us();

#endif /* DRIVER_DRIVER_ESP32_TIME_H */
