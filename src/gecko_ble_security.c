/*
 * gecko_ble_security.c
 *
 *  Created on: Oct 19, 2019
 *      Author: vkronber
 */


#include "gecko_ble_security.h"

uint8_t pairing_conn_handle;

void gecko_ble_security_init(void)
{
	// Delete any prior bonding information
	gecko_cmd_sm_delete_bondings();

	// Configure security requirements and I/O capabilities of system
	uint8_t flags = (MITM_REQD|ENCRYPT_REQ_BOND|SECURE_ONLY|CONFIRM_BOND);
	gecko_cmd_sm_configure(flags,sm_io_capability_displayyesno);

	// Enable device to be bondable - by default this is disabled
	gecko_cmd_sm_set_bondable_mode(BONDABLE);

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
			gecko_cmd_sm_bonding_confirm(evt->data.evt_sm_confirm_bonding.connection,CONFIRM);

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

			break;

		case gecko_evt_sm_bonding_failed_id:
			// Log reason for bonding failure
			LOG_INFO("Bonding failed due to %d",evt->data.evt_sm_bonding_failed.reason);
			// Clear passkey and action item
			displayPrintf(DISPLAY_ROW_PASSKEY,"");
			displayPrintf(DISPLAY_ROW_ACTION,"");
			break;

		default:
			handled = false;
			break;
	}
	return handled;
}
