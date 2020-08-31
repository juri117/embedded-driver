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
#include "driver/driver_esp32/PrefManager.h"
#include "driver/driver_esp32/TcpConnector.h"
#include "driver/driver_esp32/isc.h"
#include "driver/driver_esp32/log.h"
#include "driver/driver_esp32/misc.h"
#include "driver/driver_esp32/semaphore.h"
#include "driver/driver_esp32/spi.h"
#include "driver/driver_esp32/task.h"
#include "driver/driver_esp32/time.h"
#include "driver/driver_esp32/uart.h"
#elif BUILD_TARGET == TARGET_PI3
#include "driver_pi3/PrefManager.h"
#include "driver_pi3/TcpConnector.h"
#include "driver_pi3/isc.h"
#include "driver_pi3/log.h"
#include "driver_pi3/misc.h"
#include "driver_pi3/semaphore.h"
#include "driver_pi3/spi.h"
#include "driver_pi3/task.h"
#include "driver_pi3/time.h"
#include "driver_pi3/uart.h"
#endif

#endif /* DRIVER_DRIVER_DRIVER_H */