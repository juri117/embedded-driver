#include "log.h"

#if BUILD_TARGET == TARGET_PI3

using namespace std;

log_level_t log_level = LOG_VERBOSE;

static vprintf_like_t s_log_print_func = &vprintf;

void log(char level, const char* tag, const char* format, va_list args) {
  uint16_t pointer = 0;
  char buff[256];
  memset(buff, 0x00, 256);
  buff[pointer] = level;
  pointer++;
  buff[pointer] = ':';
  pointer++;
  memcpy(buff + pointer, tag, strlen(tag));
  pointer += strlen(tag);
  buff[pointer] = ':';
  pointer++;
  buff[pointer] = ' ';
  pointer++;
  memcpy(buff + pointer, format, strlen(format));
  pointer += strlen(format);
  buff[pointer] = '\n';
  pointer++;
  (*s_log_print_func)(buff, args);
}

void log_v(const char* tag, const char* format, ...) {
  if (log_level >= LOG_VERBOSE) {
    // log('V', tag, format, args);
    printf("V:%s:", tag);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
  }
}
void log_d(const char* tag, const char* format, ...) {
  if (log_level >= LOG_DEBUG) {
    va_list args;
    va_start(args, format);
    log('D', tag, format, args);
    va_end(args);
  }
}
void log_i(const char* tag, const char* format, ...) {
  if (log_level >= LOG_INFO) {
    va_list args;
    va_start(args, format);
    log('I', tag, format, args);
    va_end(args);
  }
}
void log_w(const char* tag, const char* format, ...) {
  if (log_level >= LOG_WARN) {
    va_list args;
    va_start(args, format);
    log('W', tag, format, args);
    va_end(args);
  }
}
void log_e(const char* tag, const char* format, ...) {
  if (log_level >= LOG_ERROR) {
    va_list args;
    va_start(args, format);
    log('E', tag, format, args);
    va_end(args);
  }
}

void log_buffer(const char* tag, uint8_t* buff, uint16_t len) {
  printf("%s: hex(%d): ", tag, len);
  for (int i = 0; i < len; i++) {
    printf("%x ", buff[i]);
  }
  printf("\n");
}

void log_set_level(const char* tag, log_level_t level) {
  if (tag[0] == '*') {
    log_level = level;
  }
}

vprintf_like_t log_set_vprintf(vprintf_like_t func) {
  vprintf_like_t orig_func = s_log_print_func;
  s_log_print_func = func;
  return orig_func;
}

#endif