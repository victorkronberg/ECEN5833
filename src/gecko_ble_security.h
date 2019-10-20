/*
 * gecko_ble_security.h
 *
 *  Created on: Oct 19, 2019
 *      Author: vkronber
 */

#ifndef SRC_GECKO_BLE_SECURITY_H_
#define SRC_GECKO_BLE_SECURITY_H_

#include "main.h"


#define MITM_REQD			(0x01)
#define ENCRYPT_REQ_BOND	(0x02)
#define SECURE_ONLY			(0x04)
#define CONFIRM_BOND		(0x08)

#define BONDABLE			(1)
#define CONFIRM				(1)

typedef enum {
  advertising,
  connectedNotPaired,
  pairingConfirmKey,
  bonded
} ServerSecurityState;

ServerSecurityState server_security_state;

void gecko_ble_security_init(void);

void gecko_ble_security_confirm_passkey(void);

bool gecko_security_update(struct gecko_cmd_packet* evt);

#endif /* SRC_GECKO_BLE_SECURITY_H_ */
