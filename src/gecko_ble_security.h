/*
 * gecko_ble_security.h
 *
 * Header for BLE security-related functions
 *
 *  Created on: Oct 19, 2019
 *      Author: Victor Kronberg
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

/**
 * [gecko_ble_security_init]
 * @description:  Initializes BLE stack for pairing.  Configures I/O capabilities,
 *                sets device to bondable, removes prior bonding information, and
 *                configures security requirements of system.
 */
void gecko_ble_security_init(void);

/**
 * [gecko_ble_security_confirm_passkey]
 * @description:  User has confirmed passkey displayed on server is that same as
 *                passkey displayed on cliet. If successful, will confirm bonding
 */
void gecko_ble_security_confirm_passkey(void);

/**
 * [gecko_security_update]
 * @description:  Handles BLE-triggered events related to pairing
 * @param        evt [Command packet containing event ID and associated data]
 * @return           [True: Triggering event was handled
 *                    False: Triggering event was not handled]
 */
bool gecko_security_update(struct gecko_cmd_packet* evt);

bool gecko_security_client_update(struct gecko_cmd_packet* evt);

#endif /* SRC_GECKO_BLE_SECURITY_H_ */
