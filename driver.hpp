/**
 * @file driver.h
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_DRIVER_H
#define DRIVER_DRIVER_DRIVER_H

#include "def.h"

#if BUILD_TARGET == TARGET_ESP32
#include "driver_esp32/PrefManager.hpp"
#include "driver_esp32/TcpConnector.hpp"
#include "driver_esp32/isc.hpp"
#include "driver_esp32/isc_slave.hpp"
#include "driver_esp32/log.hpp"
#include "driver_esp32/misc.hpp"
#include "driver_esp32/semaphore.hpp"
#include "driver_esp32/spi.hpp"
#include "driver_esp32/task.hpp"
#include "driver_esp32/time.hpp"
#include "driver_esp32/uart.hpp"
#elif BUILD_TARGET == TARGET_PI3
#include "driver_pi3/PrefManager.hpp"
#include "driver_pi3/TcpConnector.hpp"
#include "driver_pi3/isc.hpp"
#include "driver_pi3/isc_slave.hpp"
#include "driver_pi3/log.hpp"
#include "driver_pi3/misc.hpp"
#include "driver_pi3/semaphore.hpp"
#include "driver_pi3/spi.hpp"
#include "driver_pi3/task.hpp"
#include "driver_pi3/time.hpp"
#include "driver_pi3/uart.hpp"
#endif

#endif /* DRIVER_DRIVER_DRIVER_H */