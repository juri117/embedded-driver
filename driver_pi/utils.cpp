

#include "misc.hpp"

#if BUILD_TARGET == TARGET_PI3

static const char *TAG = "pi_utils";

std::string exec(const char *cmd) {
  std::array<char, 128> buffer;
  std::string result;
  // std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
  FILE *pipe = popen(cmd, "r");
  if (!pipe) {
    log_e(TAG, "exec failed for: %s, err: %s", cmd, strerror(errno));
    pclose(pipe);
    return "";
    // throw std::runtime_error("popen() failed!");
  }
  while (!feof(pipe)) {
    if (fgets(buffer.data(), 128, pipe) != nullptr) result += buffer.data();
  }
  // int return_code = WEXITSTATUS(pclose(pipe));
  pclose(pipe);
  return result;
}

int exec_ret_code(const char *cmd) {
  std::array<char, 128> buffer;
  std::string result;
  // std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
  FILE *pipe = popen(cmd, "r");
  if (!pipe) {
    log_e(TAG, "exec_ret_code failed for: %s, err: %s", cmd, strerror(errno));
    pclose(pipe);
    return -1;
    // throw std::runtime_error("popen() failed!");
  }
  while (!feof(pipe)) {
    if (fgets(buffer.data(), 128, pipe) != nullptr) result += buffer.data();
  }
  return WEXITSTATUS(pclose(pipe));
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