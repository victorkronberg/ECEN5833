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

void gecko_ble_init_LCD_status_client(void);

bool gecko_ble_client_update(struct gecko_cmd_packet* evt);

void gecko_ble_receive_temperature(uint32_t temp_value);

float gattUint32ToFloat(const uint8_t *value_start_little_endian);

bool findStaticBluetoothAddress(bd_addr server_address,bd_addr target_address);

#endif /* SRC_GECKO_BLE_CLIENT_H_ */
