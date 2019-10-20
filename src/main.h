/*
 * main.h
 *
 * Main header file
 *
 *  Created on: Sep 9, 2019
 *      Author: Victor Kronberg
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

/* Standard C Headers */
#include <time.h>

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
#include "infrastructure.h"

/* User files */
#include "timer.h"
#include "errors.h"
#include "log.h"
#include "i2c.h"
#include "Si7021.h"
#include "scheduler.h"
#include "gecko_ble_server.h"
#include "gecko_ble_client.h"
#include "ble_device_type.h"
#include "display.h"
#include "gecko_ble_security.h"

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif

#define EnergyMode3	(1)

#define SCHEDULER_SUPPORTS_DISPLAY_UPDATE_EVENT 	(1)
#define TIMER_SUPPORTS_1HZ_TIMER_EVENT				(1)

// Event bitmasks
#define TIMER_EVENT_MASK			(0x0001)
#define TIMER_EVENT_MASK_POS		(0)
#define DELAY_EVENT_MASK			(0x0002)
#define DELAY_EVENT_MASK_POS		(1)
#define I2C_EVENT_MASK				(0x0004)
#define I2C_EVENT_MASK_POS			(2)
#define BLE_EVENT_MASK				(0x0008)
#define BLE_EVENT_MASK_POS			(3)
#define EXIT_EVENT_MASK				(0x0010)
#define EXIT_EVENT_MASK_POS			(4)
#define ONE_HZ_EVENT_MASK			(0x0020)
#define ONE_HZ_EVENT_MASK_POS		(5)
#define PASSKEY_CONFIRM_MASK		(0x0040)
#define PASSKEY_CONFIRM_POS			(6)

uint32_t event_bitmask;
uint32_t interrupt_event_bitmask;



#endif /* SRC_MAIN_H_ */
