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

#ifndef DRIVER_DRIVER_PI3_TIME_H
#define DRIVER_DRIVER_PI3_TIME_H

#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include "../error.h"
#include "def.h"

uint32_t get_time_system_ms();
uint64_t get_time_system_us();

#endif /* DRIVER_DRIVER_PI3_TIME_H */
