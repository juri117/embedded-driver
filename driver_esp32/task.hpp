/**
 * @file task.h
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_ESP32_TASK_H
#define DRIVER_DRIVER_ESP32_TASK_H

#include <driver/gpio.h>
#include <driver/timer.h>
#include <driver/uart.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>

#include "../error.hpp"

typedef EventBits_t event_bit_t;

void start_task(TaskFunction_t pvTaskCode, const char* const pcName,
                const uint32_t usStackDepth, UBaseType_t uxPriority);

void task_delay_ms(uint32_t delay_ms);

void end_task();

class DelayUntil {
 private:
  TickType_t last_wake_tick;

 public:
  DelayUntil();
  void wait_for(uint32_t wait_ms);
};

class TaskEventVar {
 private:
  EventGroupHandle_t event_group;

 public:
  TaskEventVar() { this->event_group = xEventGroupCreate(); }
  void set(uint32_t bits) { xEventGroupSetBits(this->event_group, bits); }
  bool wait_for(uint32_t bits) {
    return (xEventGroupWaitBits(this->event_group, bits, true, true,
                                portMAX_DELAY) &
            bits) == bits;
  }
  bool wait_for(uint32_t bits, uint32_t timeout_ms) {
    return (xEventGroupWaitBits(this->event_group, bits, true, true,
                                pdMS_TO_TICKS(timeout_ms)) &
            bits) == bits;
  }
  uint32_t wait_for_one(uint32_t bits) {
    return xEventGroupWaitBits(this->event_group, bits, true, false,
                               portMAX_DELAY);
  }
  uint32_t wait_for_one(uint32_t bits, uint32_t timeout_ms) {
    return xEventGroupWaitBits(this->event_group, bits, true, false,
                               pdMS_TO_TICKS(timeout_ms));
  }
  bool is_set(uint32_t bits) {
    return (xEventGroupGetBits(this->event_group) & bits) == bits;
  }
  void clear(uint32_t bits) { xEventGroupClearBits(this->event_group, bits); }
};

template <typename T>
class TaskQue {
 public:
  // T pop() {
  //   std::unique_lock<std::mutex> mlock(mutex_);
  //   while (queue_.empty()) {
  //     cond_.wait(mlock);
  //   }
  //   auto val = queue_.front();
  //   queue_.pop();
  //   return val;
  // }
  TaskQue() {}
  void init(uint32_t element_count, uint32_t element_size) {
    this->queue_ = xQueueCreate(element_count, element_size);
  }
  bool pop(T* item) {
    return xQueueReceive(this->queue_, item, portMAX_DELAY);
    // xQueueReceive(this->queue_, &item, pdMS_TO_TICKS(portMAX_DELAY));
  }

  bool pop(T* item, uint32_t timeout_ms) {
    return xQueueReceive(this->queue_, item, pdMS_TO_TICKS(timeout_ms));
  }

  void push_from_intr(const T* item) {
    xQueueSendFromISR(this->queue_, item, NULL);
  }
  void push(const T* item) { xQueueSend(this->queue_, item, NULL); }
  bool empty() { return uxQueueMessagesWaiting(this->queue_) == 0; }

  // TaskQue(const Queue&) = delete;               // disable copying
  // TaskQue& operator=(const TaskQue&) = delete;  // disable assignment

 private:
  xQueueHandle queue_;
};

#endif /* DRIVER_DRIVER_ESP32_TASK_H */
