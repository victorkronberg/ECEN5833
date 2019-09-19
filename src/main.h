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
#include "em_core.h"

/* User files */
#include "timer.h"
#include "errors.h"
#include "log.h"
#include "i2c.h"
#include "Si7021.h"

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif

#define EnergyMode2	(1)

// Event bitmasks
#define TIMER_EVENT_MASK		(0x0001)
#define TIMER_EVENT_MASK_POS	(0)

uint32_t event_bitmask;
uint32_t interrupt_event_bitmask;

#endif /* SRC_MAIN_H_ */
