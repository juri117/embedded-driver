#include "log.hpp"
#if BUILD_TARGET == TARGET_ESP32

void log_buffer(const char* tag, uint8_t* buff, uint16_t len) {
  printf("%s: hex(%d): ", tag, len);
  for (int i = 0; i < len; i++) {
    printf("%x ", buff[i]);
  }
  printf("\n");
}

void log_set_level(const char* tag, log_level_t level) {
  esp_log_level_set(tag, (esp_log_level_t)level);
}

vprintf_like_t log_set_vprintf(vprintf_like_t func) {
  return esp_log_set_vprintf(func);
}

#endif