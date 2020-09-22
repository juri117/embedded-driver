/**
 * @file semaphore.h
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2018-02-16
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_PI3_SEMAPHORE_H
#define DRIVER_DRIVER_PI3_SEMAPHORE_H

#include <mutex>

#include "../error.hpp"

class Semaphore {
 private:
  std::timed_mutex sema;

 public:
  Semaphore();
  bool take(uint32_t timeout_ms);
  void give();
};
#endif /* DRIVER_DRIVER_PI3_SEMAPHORE_H */
