/*
 * main.h
 *
 *  Created on: Sep 9, 2019
 *      Author: vkronber
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

/* Board headers */
#include "init_mcu.h"
#include "init_board.h"
#include "init_app.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"

/* Device initialization header */
#include "hal-config.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif

#include "em_device.h"
#include "em_chip.h"
#include "gpio.h"
#include "sleep.h"

/* User files */
#include "timer.h"
#include "errors.h"

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif

#endif /* SRC_MAIN_H_ */
