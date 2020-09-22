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

#ifndef DRIVER_DRIVER_PI3_LOG_H
#define DRIVER_DRIVER_PI3_LOG_H

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error.hpp"

typedef enum {
  LOG_NONE,  /*!< No log output */
  LOG_ERROR, /*!< Critical errors, software module can not recover on its own */
  LOG_WARN,  /*!< Error conditions from which recovery measures have been taken
              */
  LOG_INFO,  /*!< Information messages which describe normal flow of events */
  LOG_DEBUG, /*!< Extra information which is not necessary for normal use
                (values, pointers, sizes, etc). */
  LOG_VERBOSE /*!< Bigger chunks of debugging information, or frequent messages
                 which can potentially flood the output. */
} log_level_t;

typedef int (*vprintf_like_t)(const char*, va_list);

void log(char level, const char* tag, const char* format, va_list args);
void log_v(const char* tag, const char* format, ...);
void log_d(const char* tag, const char* format, ...);
void log_i(const char* tag, const char* format, ...);
void log_w(const char* tag, const char* format, ...);
void log_e(const char* tag, const char* format, ...);

void log_buffer(const char* tag, uint8_t* buff, uint16_t len);

void log_set_level(const char* tag, log_level_t level);

vprintf_like_t log_set_vprintf(vprintf_like_t func);

#endif /* DRIVER_DRIVER_PI3_LOG_H */