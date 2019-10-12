/*
 * gecko_ble_client.h
 *
 *  Created on: Oct 12, 2019
 *      Author: vkronber
 */

#ifndef SRC_GECKO_BLE_CLIENT_H_
#define SRC_GECKO_BLE_CLIENT_H_

#include "main.h"

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

#endif /* SRC_GECKO_BLE_CLIENT_H_ */
