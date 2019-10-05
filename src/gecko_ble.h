/*
 * gecko_ble.h
 *
 * BLE interface header
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

/**
 * [gecko_ble_update]
 * @description:  Handles BLE-triggered events
 * @param        evt [Command packet containing event ID and associated data]
 * @return           [True: Triggering event was handled
 *                    False: Triggering event was not handled]
 */
bool gecko_ble_update(struct gecko_cmd_packet* evt);

/**
 * [gecko_update]
 * @description:  Handles BLE-triggered events related to system boot,
 *                dropped connection, and OTA events.  Called from within
 *                gecko_ble_update. Based on Silicon Labs empty Blue Gecko
 *                Bluetoot project.
 * @param        evt [Command packet containing event ID and associated data]
 * @return           [True: Triggering event was handled
 *                    False: Triggering event was not handled]
 */
bool gecko_update(struct gecko_cmd_packet* evt);

/**
 * [gecko_ble_send_temperature]
 * @description:  Converts floating point temperature to bitstream for BLE
 *                packet.  Sends temperature "indication" to all listening
 *                clients. Based on Silicon Labs Health Thermometer Blue
 *                Gecko example.
 * @param        temperature [Floating point temperature in degrees C]
 */
void gecko_ble_send_temperature(float temperature);

/**
 * [gecko_ble_get_rssi]
 * @description:  Initiates RSSI retrieval using global connection handle
 */
void gecko_ble_get_rssi(void);

/**
 * [gecko_ble_dynamic_tx_power_update]
 * @description:  Updates the global TX power based on given RSSI value.
 *                First, it calculates transmit power from RSSI in 0.1dBm units,
 *                then uses that value to call gecko_ble_update_tx_power.
 * @param        rssi [RSSI value - signal strength]
 */
void gecko_ble_dynamic_tx_power_update(int8_t rssi);

/**
 * [gecko_ble_update_tx_power]
 * @description:  Updates max system TX power.  Halts system prior to updating
 *                power based on given TX power, then resumes system.
 * @param        power [TX Power in units of 0.1dBm]
 */
void gecko_ble_update_tx_power(int16_t power);


#endif /* SRC_GECKO_BLE_H_ */
