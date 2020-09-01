#include "task.hpp"

#if BUILD_TARGET == TARGET_ESP32

void start_task(TaskFunction_t pvTaskCode, const char* const pcName,
                const uint32_t usStackDepth, UBaseType_t uxPriority) {
  xTaskCreate(pvTaskCode, pcName, usStackDepth, NULL, uxPriority, NULL);
}

void task_delay_ms(uint32_t delay_ms) { vTaskDelay(pdMS_TO_TICKS(delay_ms)); }

void end_task() { vTaskDelete(NULL); }

DelayUntil::DelayUntil() { this->last_wake_tick = xTaskGetTickCount(); };
void DelayUntil::wait_for(uint32_t wait_ms) {
  if (this->last_wake_tick + pdMS_TO_TICKS(wait_ms) < xTaskGetTickCount()) {
    this->last_wake_tick = xTaskGetTickCount();
  }
  vTaskDelayUntil(&(this->last_wake_tick), pdMS_TO_TICKS(wait_ms));
};

#endif