/**
 * @file PrefManager.h
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2018-02-16
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_ESP32_PREFMANAGER_H_
#define DRIVER_DRIVER_ESP32_PREFMANAGER_H_

#include <esp_err.h>
#include <esp_system.h>
#include <nvs.h>
#include <nvs_flash.h>

#include <cstdint>
#include <cstring>
#include <iostream>

#include "driver/driver.h"
#include "log.h"
#include "utils/converterUtils.h"

class PrefManager {
 private:
  nvs_handle nvs;

 public:
  PrefManager();
  ~PrefManager();
  void init();
  bool storage_exists();
  void commit();
  uint8_t read_u8(const char* key, uint8_t def);
  void write_u8(const char* key, uint8_t val);
  uint16_t read_u16(const char* key, uint16_t def);
  void write_u16(const char* key, uint16_t val);
  uint32_t read_u32(const char* key, uint32_t def);
  void write_u32(const char* key, uint32_t val);
  int32_t read_i32(const char* key, int32_t def);
  void write_i32(const char* key, int32_t val);
  uint64_t read_u64(const char* key, uint64_t def);
  void write_u64(const char* key, uint64_t val);
  double read_double(const char* key, double def);
  void write_double(const char* key, double val);
  float read_float(const char* key, float def);
  void write_float(const char* key, float val);
  uint16_t read_ary(const char* key, uint8_t* ary, uint16_t exp_len);
  void write_ary(const char* key, uint8_t* ary, uint16_t len);
  uint16_t read_str(const char* key, uint8_t* ary, uint16_t max_len);
  void write_str(const char* key, uint8_t* ary, uint16_t len, uint16_t max_len);
};

#endif /* DRIVER_DRIVER_ESP32_PREFMANAGER_H_ */
