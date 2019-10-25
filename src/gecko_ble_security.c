/*
 * gecko_ble_security.c
 *
 * BLE security functions.  Contains functions related to configuring security
 * for pairing/bonding with external devices.  Also contains BLE stack event
 * handler for security-related events.
 *
 *  Created on: Oct 19, 2019
 *      Author: Victor Kronberg
 */


#include "gecko_ble_security.h"

uint8_t pairing_conn_handle;

void gecko_ble_security_init(void)
{
	// Delete any prior bonding information
	gecko_cmd_sm_delete_bondings();

	// Configure security requirements and I/O capabilities of system
	// MITM is enable -> bonding is also enabled
	uint8_t flags = (MITM_REQD|ENCRYPT_REQ_BOND|CONFIRM_BOND);
	BTSTACK_CHECK_RESPONSE(gecko_cmd_sm_configure(flags,sm_io_capability_displayyesno));

	server_security_state = advertising;

}

void gecko_ble_security_confirm_passkey(void)
{
	gecko_cmd_sm_passkey_confirm(pairing_conn_handle,CONFIRM);
}

bool gecko_security_update(struct gecko_cmd_packet* evt)
{
	bool handled = true;

    /* Handle events */
	switch (BGLIB_MSG_ID(evt->header))
	{
		case gecko_evt_sm_confirm_bonding_id:
			// Accept bonding request to initiate pairing
			BTSTACK_CHECK_RESPONSE(gecko_cmd_sm_bonding_confirm(evt->data.evt_sm_confirm_bonding.connection,CONFIRM));

			// Store connection handle for passkey confirmation
			pairing_conn_handle = evt->data.evt_sm_confirm_bonding.connection;
			break;

		case gecko_evt_sm_confirm_passkey_id:
			// Display passkey and required action
			displayPrintf(DISPLAY_ROW_PASSKEY,"Passkey %06d",evt->data.evt_sm_confirm_passkey.passkey);
			displayPrintf(DISPLAY_ROW_ACTION,"Confirm with PB0");

			// Set security state to waiting for pairing key confirmation
			server_security_state = pairingConfirmKey;

			break;

		case gecko_evt_sm_bonded_id:
			// Update status as bonded
			displayPrintf(DISPLAY_ROW_CONNECTION,"Bonded");
			// Clear passkey and action
			displayPrintf(DISPLAY_ROW_PASSKEY,"");
			displayPrintf(DISPLAY_ROW_ACTION,"");

			// Update security state
			server_security_state = bonded;

			break;

		case gecko_evt_sm_bonding_failed_id:
			// Log reason for bonding failure
			LOG_INFO("Bonding failed due to %d",evt->data.evt_sm_bonding_failed.reason);

			// Clear passkey and action item
			displayPrintf(DISPLAY_ROW_PASSKEY,"");
			displayPrintf(DISPLAY_ROW_ACTION,"");

			// Update security state
			server_security_state = connectedNotPaired;

			break;

		default:
			handled = false;
			break;
	}
	return handled;
}
