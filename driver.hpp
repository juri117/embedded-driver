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
#include "driver_esp32/NeoPixel.hpp"
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
#include "driver_pi/NeoPixel.hpp"
#include "driver_pi/PrefManager.hpp"
#include "driver_pi/Sftp.hpp"
#include "driver_pi/TcpConnector.hpp"
#include "driver_pi/isc.hpp"
#include "driver_pi/isc_slave.hpp"
#include "driver_pi/log.hpp"
#include "driver_pi/misc.hpp"
#include "driver_pi/semaphore.hpp"
#include "driver_pi/spi.hpp"
#include "driver_pi/task.hpp"
#include "driver_pi/time.hpp"
#include "driver_pi/types.hpp"
#include "driver_pi/uart.hpp"
#endif

#endif /* DRIVER_DRIVER_DRIVER_H */