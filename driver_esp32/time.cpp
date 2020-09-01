#include "time.hpp"

#if BUILD_TARGET == TARGET_ESP32

uint32_t get_time_system_ms() {
  // get time in us, convert to ms, wrap one week
  return ((uint32_t)(
      (esp_timer_get_time() / 1000)));  // % (1000 * 60 * 60 * 24 * 7)));
}

uint64_t get_time_system_us() {
  // get time in us
  return (esp_timer_get_time());
}

#endif