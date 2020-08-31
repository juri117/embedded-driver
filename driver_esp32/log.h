/**
 * @file log.h
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2018-02-16
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_ESP32_LOG_H
#define DRIVER_DRIVER_ESP32_LOG_H

#include <esp_log.h>
#include <stdio.h>
#include <stdlib.h>

#include "../error.h"
#include "sdkconfig.h"

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

#define log_v(tag, format, ...) ESP_LOGV(tag, format, ##__VA_ARGS__)
#define log_d(tag, format, ...) ESP_LOGD(tag, format, ##__VA_ARGS__)
#define log_i(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)
#define log_w(tag, format, ...) ESP_LOGW(tag, format, ##__VA_ARGS__)
#define log_e(tag, format, ...) ESP_LOGE(tag, format, ##__VA_ARGS__)

void log_buffer(const char* tag, uint8_t* buff, uint16_t len);

void log_set_level(const char* tag, log_level_t level);

vprintf_like_t log_set_vprintf(vprintf_like_t func);

#endif /* DRIVER_DRIVER_ESP32_LOG_H */