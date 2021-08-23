/**
 * @file utils.h
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_PI3_UTILS_H
#define DRIVER_DRIVER_PI3_UTILS_H

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../error.hpp"
#include "log.hpp"
#include "task.hpp"
#include "time.hpp"
#include "types.hpp"

typedef struct {
  int code;            // mm
  std::string result;  // receive time in ms
} exec_return_t;

bool file_exists(const uint8_t *name);
exec_return_t exec(const char *cmd);

bool find_script(const char *script_name, uint8_t *script_path,
                 uint16_t max_len);

#endif /* DRIVER_DRIVER_PI3_UTILS_H */
