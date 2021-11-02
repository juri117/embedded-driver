#include "task.hpp"

#if BUILD_TARGET == TARGET_PI3

void start_task(void (*function)(), const char* const pcName,
                const uint32_t usStackDepth, int uxPriority) {
  // std::thread task(func);
  // std::thread(func);
  std::thread t;
  t = std::thread(function);
  pthread_setname_np(t.native_handle(), pcName);
  t.detach();
}

void task_delay_ms(uint32_t delay_ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
}

void end_task() {}

DelayUntil::DelayUntil() : last_wake_tick(std::chrono::steady_clock::now()){};

void DelayUntil::wait_for(uint32_t wait_ms) {
  if ((last_wake_tick + std::chrono::milliseconds(wait_ms) -
       std::chrono::steady_clock::now())
          .count() < 0)
    last_wake_tick = std::chrono::steady_clock::now();
  else
    last_wake_tick += std::chrono::milliseconds(wait_ms);
  std::this_thread::sleep_until(last_wake_tick);
};

#endif