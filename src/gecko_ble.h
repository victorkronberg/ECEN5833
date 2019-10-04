/*
 * gecko_ble.h
 *
 *  Created on: Oct 1, 2019
 *      Author: vkronber
 */

#ifndef SRC_GECKO_BLE_H_
#define SRC_GECKO_BLE_H_

#include "main.h"
#include "gecko_ble_errors.h"

#define ADVERTISE_INTERVAL_250MS	(400)
#define CONNECTION_INTERVAL_75MS	(50)
#define LATENCY_300MS				(3)
#define CONNECTION_TIMEOUT			(350)	// Set to 3.5 second
#define MAX_CE_LENGTH				(0xffff)

// TXPOWER defines
// Ranges based on UG103.14: Bluetooth LE Fundamentals by Silicon Labs
// and Bluetoooth Low Energy: A Developer's Hanbook by Robin Heydon (p.56-57)
#define TXPOWER_MIN					(-200)
#define TXPOWER_NEG_20DB			(-200)
#define TXPOWER_NEG_15DB			(-150)
#define TXPOWER_NEG_5DB				(-50)
#define TXPOWER_0DB					(0)
#define TXPOWER_POS_5DB				(50)
#define TXPOWER_MAX					(100)

// Global variable containing connection handle
uint8_t conn_handle;
volatile int16_t global_tx_power;

bool gecko_ble_update(struct gecko_cmd_packet* evt);

bool gecko_update(struct gecko_cmd_packet* evt);

void gecko_ble_send_temperature(float temperature);

void gecko_ble_get_rssi(void);

void gecko_ble_dynamic_tx_power_update(int8_t rssi);

void gecko_ble_update_tx_power(int16_t power);


#endif /* SRC_GECKO_BLE_H_ */
