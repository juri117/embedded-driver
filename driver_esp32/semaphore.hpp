/**
 * @file semaphore.hpp
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_ESP32_SEMAPHORE_H
#define DRIVER_DRIVER_ESP32_SEMAPHORE_H

#include <driver/gpio.h>
#include <driver/timer.h>
#include <driver/uart.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include "../error.hpp"

class Semaphore {
 private:
  SemaphoreHandle_t sema;

 public:
  Semaphore();
  bool take(uint32_t timeout_ms);
  void give();
};

void give_semaphore(Semaphore sema);

#endif /* DRIVER_DRIVER_ESP32_SEMAPHORE_H */
