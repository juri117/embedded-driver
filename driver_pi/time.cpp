#include "time.hpp"

#if BUILD_TARGET == TARGET_PI3

using namespace std::chrono;

uint64_t start_time_offset_us = 0;
uint64_t start_time_offset_ms = 0;

void init_time() {
  start_time_offset_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count();
  start_time_offset_us =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count();
}

uint32_t get_time_system_ms() {
  if (start_time_offset_ms == 0) {
    init_time();
  }
  return (uint32_t)(std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch())
                        .count() -
                    start_time_offset_ms);
}

uint64_t get_time_system_us() {
  if (start_time_offset_us == 0) {
    init_time();
  }
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::system_clock::now().time_since_epoch())
             .count() -
         start_time_offset_us;
}
#endif