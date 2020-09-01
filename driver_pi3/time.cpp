#include "time.hpp"

#if BUILD_TARGET == TARGET_PI3
uint32_t get_time_system_ms() {
  // get time in us, convert to ms, wrap one week
  // struct timeval time;
  // gettimeofday(&time, NULL);
  // return time.tv_msec;
  return get_time_system_us() / 1000;
}

uint64_t get_time_system_us() {
  // get time in us
  struct timeval time;
  gettimeofday(&time, NULL);
  return time.tv_usec;
}
#endif