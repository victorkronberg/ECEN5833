/*
 * gecko_ble_client.h
 *
 *  Created on: Oct 12, 2019
 *      Author: vkronber
 */

#ifndef SRC_GECKO_BLE_CLIENT_H_
#define SRC_GECKO_BLE_CLIENT_H_

#include "main.h"
#include "math.h"

typedef enum {
  scanning,
  opening,
  discoverServices,
  discoverCharacteristics,
  enableIndication,
  running
} ConnState;

typedef struct {
  uint8_t  connectionHandle;
  int8_t   rssi;
  uint16_t serverAddress;
  uint32_t thermometerServiceHandle;
  uint16_t thermometerCharacteristicHandle;
  uint32_t temperature;
} ConnProperties;

/**
 * [gecko_ble_init_LCD_status_client]
 * @description: Initialize LCD display with name of device ("Client"), address
 *                of client, and target address of server.
 */
void gecko_ble_init_LCD_status_client(void);

/**
 * [gecko_ble_client_update description]
 * @description:  Handles BLE-triggered events
 * @param        evt [Command packet containing event ID and associated data]
 * @return           [True: Triggering event was handled
 *                    False: Triggering event was not handled]
 */
bool gecko_ble_client_update(struct gecko_cmd_packet* evt);

/**
 * [gecko_ble_receive_temperature]
 * @description:  Converts temperature from milli-degrees C to milli-degrees F
 *                and prints out both degrees C and degrees F to LCD if the
 *                display is enabled.
 * @param        temp_value [uint32_t temperature in milli-degrees C]
 */
void gecko_ble_receive_temperature(uint32_t temp_value);

/**
 * @return a float value based on a UINT32 value written by FLT_TO_UINT32 and
 * UINT32_TO_BITSTREAM
 * @param value_start_little_endian is a pointer to the first byte of the float
 * which is represented in UINT32 format from FLT_TO_UINT32/UINT32_TO_BITSTREAM
 */
float gattUint32ToFloat(const uint8_t *value_start_little_endian);

/**
 * [findStaticBluetoothAddress]
 * @description: Checks if Bluetooth server address matches the address of the
 *                target device.  Returns true if they match and false if not.
 * @param        server_address [bd_addr format server address. Sent from the
 *                                scan packet address]
 * @param        target_address [bd_addr format target address]
 * @return                      [True if match, false if not]
 */
bool findStaticBluetoothAddress(bd_addr server_address,bd_addr target_address);

#endif /* SRC_GECKO_BLE_CLIENT_H_ */
