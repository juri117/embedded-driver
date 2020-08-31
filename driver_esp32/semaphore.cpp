#include "semaphore.h"

#if BUILD_TARGET == TARGET_ESP32

Semaphore::Semaphore() {
  this->sema = xSemaphoreCreateMutex();
  //   if (this->sema == NULL) {
  //     log_e(TAG, "Error: xSemaphore was not created...");
  //   }
}
bool Semaphore::take(uint32_t timeout_ms) {
  return xSemaphoreTake(this->sema, pdMS_TO_TICKS(timeout_ms)) == pdTRUE;
}
void Semaphore::give() { xSemaphoreGive(this->sema); }

#endif