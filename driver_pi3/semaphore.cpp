#include "semaphore.hpp"

#if BUILD_TARGET == TARGET_ESP32

Semaphore::Semaphore() {}
bool Semaphore::take(uint32_t timeout_ms) {
  this->sema.lock();
  return true;
}
void Semaphore::give() { this->sema.unlock(); }

#endif