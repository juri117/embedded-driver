

#include "misc.hpp"

#if BUILD_TARGET == TARGET_PI3

static const char *TAG = "pi_utils";

exec_return_t exec(const char *cmd) {
  std::array<char, 128> buffer;
  std::string result;
  int return_code = -1;
  auto pclose_wrapper = [&return_code](FILE *cmd) {
    return_code = pclose(cmd);
  };
  {  // scope is important, have to make sure the ptr goes out of scope first
    const std::unique_ptr<FILE, decltype(pclose_wrapper)> pipe(popen(cmd, "r"),
                                                               pclose_wrapper);
    if (pipe) {
      while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
      }
    }
  }
  // return make_pair(result, return_code);
  return {return_code, result};
}

bool file_exists(const uint8_t *name) {
  return (access((const char *)name, F_OK) != -1);
}

bool find_script(const char *script_name, uint8_t *script_path,
                 uint16_t max_len) {
  char current_path[FILENAME_MAX];
  memset(current_path, 0x00, FILENAME_MAX);
  getcwd(current_path, FILENAME_MAX);

  memset(script_path, 0x00, max_len);
  sprintf((char *)script_path, "%s/main/driver/driver_pi/scripts/%s",
          current_path, script_name);
  if (file_exists(script_path)) {
    return true;
  }
  memset(script_path, 0x00, max_len);
  sprintf((char *)script_path, "%s/build/scripts/%s", current_path,
          script_name);
  if (file_exists(script_path)) {
    return true;
  }
  return false;
}

#endif