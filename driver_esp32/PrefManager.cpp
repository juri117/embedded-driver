

#include "PrefManager.hpp"

#if BUILD_TARGET == TARGET_ESP32

static const char* TAG = "PrefManager";

PrefManager::PrefManager() {
  // nothing
  this->nvs = 0;
}

PrefManager::~PrefManager() { nvs_close(this->nvs); }

void PrefManager::init() {
  // Open NVS
  error_t err;
  err = nvs_open("settings", NVS_READWRITE, &this->nvs);
  if (err != ERROR_OK) {
    log_e(TAG, "Error (%x) opening NVS handle!", err);
  }
  // else {
  //   log_i(TAG, "Pref opened!");
  // }
}

bool PrefManager::storage_exists() { return true; }

void PrefManager::commit() {
  error_t err = nvs_commit(this->nvs);
  if (err != ERROR_OK) {
    log_e(TAG, "could not commit to nvs: %x", err);
  }
}

uint8_t PrefManager::read_u8(const char* key, uint8_t def) {
  uint8_t val = def;
  error_t err = nvs_get_u8(nvs, key, &val);
  switch (err) {
    case ERROR_OK:
      log_d(TAG, "Pref read: %s = %d", key, val);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set default: %s = %d", key, def);
      break;
    default:
      log_e(TAG, "Error reading pref %s: (%x)", key, err);
  }
  return val;
}

void PrefManager::write_u8(const char* key, uint8_t val) {
  error_t err = nvs_set_u8(nvs, key, val);
  switch (err) {
    case ERROR_OK:
      log_d(TAG, "Pref write: %s = %d", key, val);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set: %s = %d", key, val);
      break;
    default:
      log_e(TAG, "Error writing pref %s: (%x)", key, err);
  }
}

uint16_t PrefManager::read_u16(const char* key, uint16_t def) {
  uint16_t val = def;
  error_t err = nvs_get_u16(nvs, key, &val);
  switch (err) {
    case ERROR_OK:
      log_d(TAG, "Pref read: %s = %d", key, val);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set default: %s = %d", key, def);
      break;
    default:
      log_e(TAG, "Error reading pref %s: (%x)", key, err);
  }
  return val;
}

void PrefManager::write_u16(const char* key, uint16_t val) {
  error_t err = nvs_set_u16(nvs, key, val);
  switch (err) {
    case ERROR_OK:
      log_d(TAG, "Pref write: %s = %d", key, val);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set: %s = %d", key, val);
      break;
    default:
      log_e(TAG, "Error writing pref %s: (%x)", key, err);
  }
}

int16_t PrefManager::read_i16(const char* key, int16_t def) {
  int16_t val = def;
  error_t err = nvs_get_i16(nvs, key, &val);
  switch (err) {
    case ERROR_OK:
      log_d(TAG, "Pref read: %s = %d", key, val);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set default: %s = %d", key, def);
      break;
    default:
      log_e(TAG, "Error reading pref %s: (%x)", key, err);
  }
  return val;
}

void PrefManager::write_i16(const char* key, int16_t val) {
  error_t err = nvs_set_i16(nvs, key, val);
  switch (err) {
    case ERROR_OK:
      log_d(TAG, "Pref write: %s = %d", key, val);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set: %s = %d", key, val);
      break;
    default:
      log_e(TAG, "Error writing pref %s: (%x)", key, err);
  }
}

uint32_t PrefManager::read_u32(const char* key, uint32_t def) {
  uint32_t val = def;
  error_t err = nvs_get_u32(nvs, key, &val);
  switch (err) {
    case ERROR_OK:
      log_d(TAG, "Pref read: %s = %d", key, val);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set default: %s = %d", key, def);
      break;
    default:
      log_e(TAG, "Error reading pref %s: (%x)", key, err);
  }
  return val;
}

void PrefManager::write_u32(const char* key, uint32_t val) {
  error_t err = nvs_set_u32(nvs, key, val);
  switch (err) {
    case ERROR_OK:
      log_d(TAG, "Pref write: %s = %d", key, val);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set: %s = %d", key, val);
      break;
    default:
      log_e(TAG, "Error writing pref %s: (%x)", key, err);
  }
}

int32_t PrefManager::read_i32(const char* key, int32_t def) {
  int32_t val = def;
  error_t err = nvs_get_i32(nvs, key, &val);
  switch (err) {
    case ERROR_OK:
      log_d(TAG, "Pref read: %s = %d", key, val);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set default: %s = %d", key, def);
      break;
    default:
      log_e(TAG, "Error reading pref %s: (%x)", key, err);
  }
  return val;
}

void PrefManager::write_i32(const char* key, int32_t val) {
  error_t err = nvs_set_i32(nvs, key, val);
  switch (err) {
    case ERROR_OK:
      log_d(TAG, "Pref write: %s = %d", key, val);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set: %s = %d", key, val);
      break;
    default:
      log_e(TAG, "Error writing pref %s: (%x)", key, err);
  }
}

uint64_t PrefManager::read_u64(const char* key, uint64_t def) {
  uint64_t val = def;
  error_t err = nvs_get_u64(nvs, key, &val);
  switch (err) {
    case ERROR_OK:
      log_d(TAG, "Pref read: %s = %llu", key, val);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set default: %s = %llu", key, def);
      break;
    default:
      log_e(TAG, "Error reading pref %s: (%x)", key, err);
  }
  return val;
}

void PrefManager::write_u64(const char* key, uint64_t val) {
  error_t err = nvs_set_u64(nvs, key, val);
  switch (err) {
    case ERROR_OK:
      log_d(TAG, "Pref write: %s = %llu", key, val);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set: %s = %llu", key, val);
      break;
    default:
      log_e(TAG, "Error writing pref %s: (%x)", key, err);
  }
}

double PrefManager::read_double(const char* key, double def) {
  double val = def;
  // Workaround: somehow if the buff length is 8, an ESP_ERR_NVS_INVALID_LENGTH
  // is returned 'sometimes'!
  uint8_t buff[9];
  size_t len;
  error_t err = nvs_get_blob(nvs, key, buff, &len);
  switch (err) {
    case ERROR_OK:
      doubleConv b2d;
      memcpy(b2d.b, buff, 8);
      val = b2d.d;
      // val = bytes_to_double(buff);
      log_d(TAG, "Pref read: %s = %f", key, val);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set default: %s = %f", key, def);
      break;
    case ESP_ERR_NVS_INVALID_LENGTH:
      log_i(TAG, "read blob but buff is to short %s, len = %d", key, len);
      break;
    default:
      log_e(TAG, "Error reading pref %s: (%x)", key, err);
  }
  return val;
}

void PrefManager::write_double(const char* key, double val) {
  uint8_t buff[8];
  doubleConv d2b;
  d2b.d = val;
  memcpy(buff, d2b.b, 8);
  // double_to_bytes(val, buff);
  error_t err = nvs_set_blob(nvs, key, buff, 8);
  switch (err) {
    case ERROR_OK:
      log_d(TAG, "Pref write: %s = %f", key, val);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set: %s = %f", key, val);
      break;
    default:
      log_e(TAG, "Error writing pref %s: (%x)", key, err);
  }
}

float PrefManager::read_float(const char* key, float def) {
  double val = def;
  // Workaround: somehow if the buff length is 4, an ESP_ERR_NVS_INVALID_LENGTH
  // is returned 'sometimes'!
  uint8_t buff[5];
  size_t len;
  error_t err = nvs_get_blob(nvs, key, buff, &len);
  switch (err) {
    case ERROR_OK:
      floatConv b2f;
      memcpy(b2f.b, buff, 4);
      val = b2f.f;
      // val = bytes_to_float(buff);
      log_d(TAG, "Pref read: %s = %f", key, val);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set default: %s = %f", key, def);
      break;
    case ESP_ERR_NVS_INVALID_LENGTH:
      log_i(TAG, "read blob but buff is to short %s, len = %d", key, len);
      break;
    default:
      log_e(TAG, "Error reading pref %s: (%x)", key, err);
  }
  return val;
}

void PrefManager::write_float(const char* key, float val) {
  uint8_t buff[4];
  floatConv f2b;
  f2b.f = val;
  memcpy(buff, f2b.b, 4);
  // float_to_bytes(val, buff);
  error_t err = nvs_set_blob(nvs, key, buff, 4);
  switch (err) {
    case ERROR_OK:
      log_d(TAG, "Pref write: %s = %f", key, val);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set: %s = %f", key, val);
      break;
    default:
      log_e(TAG, "Error writing pref %s: (%x)", key, err);
  }
}

uint16_t PrefManager::read_ary(const char* key, uint8_t* ary,
                               uint16_t exp_len) {
  size_t len = 0;
  error_t err = nvs_get_blob(nvs, key, nullptr, &len);
  if (err != ERROR_OK) {
    log_e(TAG, "could not retrieve length of %s, err = %x", key, err);
    return 0;
  }
  if (len > exp_len) {
    log_e(TAG,
          "length of %s is bigger then expected, len = %d, but expect = %d",
          key, len, exp_len);
    return 0;
  }
  uint8_t buff[len];
  err = nvs_get_blob(nvs, key, buff, &len);
  switch (err) {
    case ERROR_OK:
      memcpy(ary, buff, len);
      log_d(TAG, "Pref read: %s", key);
      // log_buffer(TAG, ary, len);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set default: %s", key);
      return 0;
    case ESP_ERR_NVS_INVALID_LENGTH:
      log_i(TAG, "read blob but buff is to short %s, len = %d", key, len);
      return 0;
    default:
      log_e(TAG, "Error reading pref %s: (%x)", key, err);
      return 0;
  }
  return len;
}

void PrefManager::write_ary(const char* key, uint8_t* ary, uint16_t len) {
  error_t err = nvs_set_blob(nvs, key, ary, len);
  switch (err) {
    case ERROR_OK:
      log_d(TAG, "Pref write: %s", key);
      // log_buffer(TAG, ary, len);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      log_i(TAG, "Pref not initialized yet, set: %s", key);
      break;
    default:
      log_e(TAG, "Error writing pref %s: (%x)", key, err);
  }
}

uint16_t PrefManager::read_str(const char* key, uint8_t* ary,
                               uint16_t max_len) {
  this->read_ary(key, ary, max_len);
  uint16_t strLen = max_len;
  for (int i = 0; i < max_len; i++) {
    if (ary[i] == 0x00) {
      strLen = i;
      break;
    }
  }
  return strLen;
}

void PrefManager::write_str(const char* key, uint8_t* ary, uint16_t len,
                            uint16_t max_len) {
  // zero terminate string
  if (len < max_len) {
    ary[len] = 0x00;
    this->write_ary(key, ary, len + 1);
  } else {
    this->write_ary(key, ary, len);
  }
}

#endif