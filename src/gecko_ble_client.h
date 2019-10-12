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

#endif /* SRC_GECKO_BLE_CLIENT_H_ */
