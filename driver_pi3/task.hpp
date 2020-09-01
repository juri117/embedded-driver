/**
 * @file task.h
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2018-02-16
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_PI3_TASK_H
#define DRIVER_DRIVER_PI3_TASK_H

#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

#include "../error.hpp"

#define BIT31 0x80000000
#define BIT30 0x40000000
#define BIT29 0x20000000
#define BIT28 0x10000000
#define BIT27 0x08000000
#define BIT26 0x04000000
#define BIT25 0x02000000
#define BIT24 0x01000000
#define BIT23 0x00800000
#define BIT22 0x00400000
#define BIT21 0x00200000
#define BIT20 0x00100000
#define BIT19 0x00080000
#define BIT18 0x00040000
#define BIT17 0x00020000
#define BIT16 0x00010000
#define BIT15 0x00008000
#define BIT14 0x00004000
#define BIT13 0x00002000
#define BIT12 0x00001000
#define BIT11 0x00000800
#define BIT10 0x00000400
#define BIT9 0x00000200
#define BIT8 0x00000100
#define BIT7 0x00000080
#define BIT6 0x00000040
#define BIT5 0x00000020
#define BIT4 0x00000010
#define BIT3 0x00000008
#define BIT2 0x00000004
#define BIT1 0x00000002
#define BIT0 0x00000001

// WORKAROUND
#define portPRIVILEGE_BIT 0

typedef uint32_t event_bit_t;

// typedef std::function<void(void*)> task_func_t;
typedef std::function<void(void*)> task_func_t;

void start_task(void (*function)(), const char* const pcName,
                const uint32_t usStackDepth, int uxPriority);
void start_task_(void (*function)(void), const char* const pcName,
                 const uint32_t usStackDepth, int uxPriority);

void task_delay_ms(uint32_t delay_ms);

void end_task();

class DelayUntil {
 private:
  //   TickType_t last_wake_tick;

 public:
  DelayUntil();
  void wait_for(uint32_t wait_ms);
};

// FIXME: this is unlikely the best solution
class TaskEventVar {
 private:
  std::condition_variable cv[8];
  std::mutex cv_m[8];
  uint32_t cond_bits;

 public:
  TaskEventVar() : cond_bits(0) {}
  void set(uint32_t bits) {
    for (uint8_t i = 0; i < 8; i++) {
      if (bits & (1 << i) > 0) {
        std::lock_guard<std::mutex> lk(cv_m[i]);
        this->cond_bits = this->cond_bits | bits;
        cv[i].notify_all();
      }
    }
  }
  bool wait_for(uint32_t bits) {
    for (uint8_t i = 0; i < 8; i++) {
      if (bits & (1 << i) > 0) {
        std::unique_lock<std::mutex> lk(cv_m[i]);
        cv[i].wait(lk);
        return true;
      }
    }
    return false;
    // std::unique_lock<std::mutex> lk(cv_m);
    // cv.wait(lk, [] { return this->cond_bits & bits > 0; });
  }
  bool wait_for(uint32_t bits, uint32_t timeout_ms) {
    for (uint8_t i = 0; i < 8; i++) {
      if (bits & (1 << i) > 0) {
        std::unique_lock<std::mutex> lk(cv_m[i]);
        cv[i].wait_for(lk, std::chrono::milliseconds(timeout_ms));
        return true;
      }
    }
    return false;
  }
  uint32_t wait_for_one(uint32_t bits) {
    // FIXME: just waits for the first bit now
    return this->wait_for(bits);
  }
  uint32_t wait_for_one(uint32_t bits, uint32_t timeout_ms) {
    // FIXME: just waits for the first bit now
    return this->wait_for(bits, timeout_ms);
  }
  bool is_set(uint32_t bits) { return this->cond_bits & bits > 0; }
  void clear(uint32_t bits) {
    if (this->is_set(bits)) {
      this->cond_bits = this->cond_bits & ~bits;
    }
  }
};

template <typename T>
class TaskQue {
 public:
  // T pop() {
  //   std::unique_lock<std::mutex> mlock(mutex_);
  //   while (queue_.empty()) {
  //     cond_.wait(mlock);
  //   }
  //   auto val = queue_.front();
  //   queue_.pop();
  //   return val;
  // }
  TaskQue() {}
  void init(uint32_t element_count, uint32_t element_size) {}
  bool pop(T* item) {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty()) {
      cond_.wait(mlock);
    }
    *item = queue_.front();
    queue_.pop();
    return true;
  }

  bool pop(T* item, uint32_t timeout_ms) {
    std::unique_lock<std::mutex> mlock(mutex_);
    if (queue_.empty()) {
      auto now = std::chrono::system_clock::now();
      if (cond_.wait_until(mlock,
                           now + std::chrono::milliseconds(timeout_ms)) ==
          std::cv_status::timeout) {
        return false;
      }
    }
    *item = queue_.front();
    queue_.pop();
    return true;
  }

  void push(const T* item) {
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(*item);
    mlock.unlock();
    cond_.notify_one();
  }
  void push_from_intr(const T* item) { this->push(item); }
  bool empty() { return queue_.empty(); }
  // TaskQue(const TaskQue&) = delete;             // disable copying
  // TaskQue& operator=(const TaskQue&) = delete;  // disable assignment

 private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

#endif /* DRIVER_DRIVER_PI3_TASK_H */
