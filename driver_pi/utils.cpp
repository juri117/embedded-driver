

#include "misc.hpp"

#if BUILD_TARGET == TARGET_PI3

static const char *TAG = "pi_utils";

std::string exec(const char *cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
  if (!pipe) throw std::runtime_error("popen() failed!");
  while (!feof(pipe.get())) {
    if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
      result += buffer.data();
  }
  return result;
}

bool file_exists(const uint8_t *name) {
  return (access((const char *)name, F_OK) != -1);
}

bool find_script(const char *script_name, uint8_t *script_path,
                 uint16_t max_len) {
  char current_path[PATH_MAX];
  memset(current_path, 0x00, PATH_MAX);
  getcwd(current_path, FILENAME_MAX);

  memset(script_path, 0x00, max_len);
  sprintf((char *)script_path, "%s/main/driver/driver_pi/%s", current_path,
          script_name);
  if (file_exists(script_path)) {
    return true;
  }
  memset(script_path, 0x00, max_len);
  sprintf((char *)script_path, "%s/scripts/%s", current_path, script_path);
  if (file_exists(script_path)) {
    return true;
  }
  return false;
}

#endif