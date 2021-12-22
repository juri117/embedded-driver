#include "semaphore.hpp"

#if BUILD_TARGET == TARGET_ESP32

void give_semaphore(Semaphore sema) { sema.give(); }

Semaphore::Semaphore() {}
bool Semaphore::take(uint32_t timeout_ms) {
  this->sema.try_lock_for(std::chrono::milliseconds(timeout_ms));
  return true;
}
void Semaphore::give() { this->sema.unlock(); }

#endif