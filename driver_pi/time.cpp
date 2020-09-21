#include "time.hpp"

#if BUILD_TARGET == TARGET_PI3

using namespace std::chrono;

uint32_t get_time_system_ms() {
  // get time in us, convert to ms, wrap one week
  // struct timeval time;
  // gettimeofday(&time, NULL);
  // return time.tv_msec;
  // return (uint32_t)(get_time_system_us() / 1000);
  return (uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

uint64_t get_time_system_us() {
  // get time in us
  // struct timeval time;
  // gettimeofday(&time, NULL);
  // return time.tv_usec;
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}
#endif