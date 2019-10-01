/*
 * gecko_ble.c
 *
 *  Created on: Oct 1, 2019
 *      Author: vkronber
 */

#include "gecko_ble.h"


bool gecko_ble_update(struct gecko_cmd_packet* evt, myStateTypeDef *state_struct)
{

	bool handled;

	handled = gecko_update(evt);

	if(!handled)
	{
		handled = 1;
		switch BGLIB_MSG_ID(evt->header)
		{
		case gecko_evt_system_external_signal_id:

			// Set external events for processing
			my_state_struct.event_bitmask |= evt->data.evt_system_external_signal.extsignals;

			break;

		/* This event is generated when a connected client has either
		* 1) changed a Characteristic Client Configuration, meaning that they have enabled
		* or disabled Notifications or Indications, or
		* 2) sent a confirmation upon a successful reception of the indication. */
		case gecko_evt_gatt_server_characteristic_status_id:
		/* Check that the characteristic in question is temperature - its ID is defined
		* in gatt.xml as "temperature_measurement". Also check that status_flags = 1, meaning that
		* the characteristic client configuration was changed (notifications or indications
		* enabled or disabled). */
			if ((evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_temperature_measurement)
				&& (evt->data.evt_gatt_server_characteristic_status.status_flags == 0x01))
			{
			  if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == 0x02)
			  {
				/* Indications have been turned ON */
				// Enable temperature polling

			  } else if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == 0x00)
			  {

				// Disable temperature polling

			  }
			}
			break;

		default:
			handled = 0;
			break;
		}
	}

}
