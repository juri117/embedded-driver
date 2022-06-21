
#include "PrefManager.hpp"

#if BUILD_TARGET == TARGET_PI3

static const char *TAG = "PrefManager";

using namespace rapidjson;

PrefManager::PrefManager() {
  // nothing
  // this->nvs = 0;
}

PrefManager::~PrefManager() {}

void PrefManager::init() {
  if (this->storage_exists()) {
    FILE *fpSchema = fopen("json_schema/config.json", "r");
    char schemaReadBuffer[65536];
    FileReadStream schemaJson(fpSchema, schemaReadBuffer,
                              sizeof(schemaReadBuffer));
    // Document schemaDoc;
    // schemaDoc.ParseStream(schemaIs);

    Document sd;
    if (!sd.ParseStream(schemaJson).HasParseError()) {
      SchemaDocument schema(sd);  // Compile a Document to SchemaDocument
                                  // parse json config
      FILE *fp = fopen("config.json", "r");  // non-Windows use "r"
      char readBuffer[65536];
      FileReadStream is(fp, readBuffer, sizeof(readBuffer));
      if (!this->doc.ParseStream(is).HasParseError()) {
        SchemaValidator validator(schema);
        if (!this->doc.Accept(validator)) {
          // Input JSON is invalid according to the schema
          // Output diagnostic information
          StringBuffer sb;
          validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
          log_w(TAG, "Invalid schema: %s", sb.GetString());
          log_w(TAG, "Invalid keyword: %s",
                validator.GetInvalidSchemaKeyword());
          sb.Clear();
          validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
          log_w(TAG, "Invalid document: %s", sb.GetString());
        }
        // return;
      } else {
        log_e(TAG, "could not parse config.json");
      }
      fclose(fp);
    } else {
      log_e(TAG, "could not parse config_schema.json");
    }
    fclose(fpSchema);

    return;
  }
  log_e(TAG, "could not find config.json");
  // const char json[] = "{}";
  this->doc.Parse("{}");
}

bool PrefManager::storage_exists() {
  std::ifstream f("config.json");
  return f.good();
}

void PrefManager::commit() {
  FILE *fp = fopen("config.json", "w");  // non-Windows use "w"

  char writeBuffer[65536];
  FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

  PrettyWriter<FileWriteStream> writer(os);
  this->doc.Accept(writer);

  fclose(fp);
  log_i(TAG, "wrote settings to config.json");
}

uint8_t PrefManager::read_u8(const char *key, uint8_t def) {
  if (!this->doc.HasMember(key)) {
    log_d(TAG, "could not read: %s use default %d", key, def);
    return def;
  }
  if (this->doc[key].IsUint()) {
    uint8_t val = this->doc[key].GetUint();
    // log_d(TAG, "read: %s = %d", key, val);
    return val;
  }
  log_d(TAG, "could not read: %s use default %d", key, def);
  return def;
}

void PrefManager::write_u8(const char *key, uint8_t val) {
  if (this->doc.HasMember(key)) {
    this->doc[key] = val;
    return;
  }
  this->doc.AddMember(StringRef(key), val, this->doc.GetAllocator());
}

uint16_t PrefManager::read_u16(const char *key, uint16_t def) {
  if (!this->doc.HasMember(key)) {
    log_d(TAG, "could not read: %s use default %d", key, def);
    return def;
  }
  if (this->doc[key].IsUint()) {
    uint16_t val = this->doc[key].GetUint();
    // log_d(TAG, "read: %s = %d", key, val);
    return val;
  }
  log_d(TAG, "could not read: %s use default %d", key, def);
  return def;
}

void PrefManager::write_u16(const char *key, uint16_t val) {
  if (this->doc.HasMember(key)) {
    this->doc[key] = val;
    return;
  }
  this->doc.AddMember(StringRef(key), val, this->doc.GetAllocator());
}

int16_t PrefManager::read_i16(const char *key, int16_t def) {
  if (!this->doc.HasMember(key)) {
    log_d(TAG, "could not read: %s use default %d", key, def);
    return def;
  }
  if (this->doc[key].IsUint()) {
    int16_t val = this->doc[key].GetUint();
    // log_d(TAG, "read: %s = %d", key, val);
    return val;
  }
  log_d(TAG, "could not read: %s use default %d", key, def);
  return def;
}

void PrefManager::write_i16(const char *key, int16_t val) {
  if (this->doc.HasMember(key)) {
    this->doc[key] = val;
    return;
  }
  this->doc.AddMember(StringRef(key), val, this->doc.GetAllocator());
}

uint32_t PrefManager::read_u32(const char *key, uint32_t def) {
  if (!this->doc.HasMember(key)) {
    log_d(TAG, "could not read: %s use default %d", key, def);
    return def;
  }
  if (this->doc[key].IsUint()) {
    uint32_t val = this->doc[key].GetUint();
    // log_d(TAG, "read: %s = %d", key, val);
    return val;
  }
  log_d(TAG, "could not read: %s use default %d", key, def);
  return def;
}

void PrefManager::write_u32(const char *key, uint32_t val) {
  if (this->doc.HasMember(key)) {
    this->doc[key] = val;
    return;
  }
  this->doc.AddMember(StringRef(key), val, this->doc.GetAllocator());
}

int32_t PrefManager::read_i32(const char *key, int32_t def) {
  if (!this->doc.HasMember(key)) {
    log_d(TAG, "could not read: %s use default %d", key, def);
    return def;
  }
  if (this->doc[key].IsInt()) {
    int32_t val = this->doc[key].GetInt();
    // log_d(TAG, "read: %s = %d", key, val);
    return val;
  }
  log_d(TAG, "could not read: %s use default %d", key, def);
  return def;
}

void PrefManager::write_i32(const char *key, int32_t val) {
  if (this->doc.HasMember(key)) {
    this->doc[key] = val;
    return;
  }
  this->doc.AddMember(StringRef(key), val, this->doc.GetAllocator());
}

uint64_t PrefManager::read_u64(const char *key, uint64_t def) {
  if (!this->doc.HasMember(key)) {
    log_d(TAG, "could not read: %s use default %ull", key, def);
    return def;
  }
  if (this->doc[key].IsUint64()) {
    uint64_t val = this->doc[key].GetUint64();
    // log_d(TAG, "read: %s = %d", key, val);
    return val;
  }
  log_d(TAG, "could not read: %s use default %d", key, def);
  return def;
}

void PrefManager::write_u64(const char *key, uint64_t val) {
  if (this->doc.HasMember(key)) {
    this->doc[key] = val;
    return;
  }
  this->doc.AddMember(StringRef(key), val, this->doc.GetAllocator());
}

double PrefManager::read_double(const char *key, double def) {
  if (!this->doc.HasMember(key)) {
    log_d(TAG, "could not read: %s use default %f", key, def);
    return def;
  }
  if (this->doc[key].IsDouble()) {
    double val = this->doc[key].GetDouble();
    // log_d(TAG, "read: %s = %d", key, val);
    return val;
  }
  log_d(TAG, "could not read: %s use default %d", key, def);
  return def;
}

void PrefManager::write_double(const char *key, double val) {
  if (this->doc.HasMember(key)) {
    this->doc[key] = val;
    return;
  }
  this->doc.AddMember(StringRef(key), val, this->doc.GetAllocator());
}

float PrefManager::read_float(const char *key, float def) {
  if (!this->doc.HasMember(key)) {
    log_d(TAG, "could not read: %s use default %f", key, def);
    return def;
  }
  if (this->doc[key].IsFloat()) {
    float val = this->doc[key].GetFloat();
    // log_d(TAG, "read: %s = %d", key, val);
    return val;
  }
  log_d(TAG, "could not read: %s use default %d", key, def);
  return def;
}

void PrefManager::write_float(const char *key, float val) {
  if (this->doc.HasMember(key)) {
    this->doc[key] = val;
    return;
  }
  this->doc.AddMember(StringRef(key), val, this->doc.GetAllocator());
}

uint16_t PrefManager::read_ary(const char *key, uint8_t *ary,
                               uint16_t exp_len) {
  if (!this->doc.HasMember(key)) {
    log_d(TAG, "could not read: %s", key);
    return 0;
  }
  if (this->doc[key].IsArray()) {
    if (this->doc[key].Size() > exp_len) {
      log_w(TAG, "array %s is bigger than expected: %d but is &d", key, exp_len,
            this->doc[key].Size());
    }
    for (uint16_t i = 0; i < this->doc[key].Size(); i++) {
      ary[i] = (uint8_t)this->doc[key][i].GetInt();
    }
  }
  return this->doc[key].Size();
}

void PrefManager::write_ary(const char *key, uint8_t *ary, uint16_t len) {
  if (this->doc.HasMember(key)) {
    this->doc[key].Clear();
    for (uint16_t i = 0; i < len; i++) {
      this->doc[key].PushBack(ary[i], this->doc.GetAllocator());
    }
    return;
  }
  rapidjson::Value myArray(rapidjson::kArrayType);
  for (uint16_t i = 0; i < len; i++) {
    rapidjson::Value objValue;
    objValue.SetInt(ary[i]);
    myArray.PushBack(objValue, this->doc.GetAllocator());
  }
  this->doc.AddMember(StringRef(key), myArray, this->doc.GetAllocator());
}

uint16_t PrefManager::read_str(const char *key, uint8_t *ary,
                               uint16_t max_len) {
  if (!this->doc.HasMember(key)) {
    log_d(TAG, "could not read: %s", key);
    return 0;
  }
  if (this->doc[key].IsString()) {
    memcpy(ary, this->doc[key].GetString(), this->doc[key].GetStringLength());
    // log_d(TAG, "read: %s = %s", key, this->doc[key].GetString());
    return this->doc[key].GetStringLength();
  }
  return 0;
}

void PrefManager::write_str(const char *key, uint8_t *ary, uint16_t len,
                            uint16_t max_len) {
  if (this->doc.HasMember(key)) {
    this->doc[key].SetString((const char *)ary, len);
    return;
  }
  Value val;
  val.SetString((const char *)ary, len, this->doc.GetAllocator());
  this->doc.AddMember(StringRef(key), val, this->doc.GetAllocator());
}

#endif