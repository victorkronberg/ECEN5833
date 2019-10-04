/*
 * gecko_ble.c
 *
 *  Created on: Oct 1, 2019
 *      Author: vkronber
 */

#include "gecko_ble.h"

uint8_t boot_to_dfu = 0;

struct gecko_msg_le_connection_get_rssi_rsp_t *rssi_rsp;



bool gecko_ble_update(struct gecko_cmd_packet* evt)
{

	//LOG_INFO("BLE event %#08x occurred",evt->header);

	bool handled;
	int16_t power;

	// Initially handle events related to advertising and resetting
	handled = gecko_update(evt);

	// If not already handled, there is an open connection
	if(!handled)
	{
		handled = 1;
		switch BGLIB_MSG_ID(evt->header)
		{
			// New connection has been opened
			case gecko_evt_le_connection_opened_id:
				// Store the connection handle
				conn_handle = evt->data.evt_le_connection_opened.connection;

				// Set the connection interval for current connection
				// Set connection parameters
				// First parameter is connection handle
				// Min and Max interval = value * 1.5ms
				// Latency = number of intervals slave can skip
				// Timeout = value * 10ms
				// MIN/MAX conection length = value * 0.625ms
				gecko_cmd_le_connection_set_timing_parameters(conn_handle,CONNECTION_INTERVAL_75MS,CONNECTION_INTERVAL_75MS,
						LATENCY_300MS,CONNECTION_TIMEOUT,0,MAX_CE_LENGTH);

				break;

			// External event trigger
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
					  // Enable temperature polling
					  my_state_struct.event_bitmask |= BLE_EVENT_MASK;
					  LOG_INFO("BLE Mask has been set");

				  } else if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == 0x00)
				  {

					// Disable temperature polling
					  my_state_struct.event_bitmask |= EXIT_EVENT_MASK;
				  }
				}
				break;

			// RSSI ready to be read
			case gecko_evt_le_connection_rssi_id:
				// Ensure results are valid
				if(evt->data.evt_le_connection_rssi.status == 0)
				{
					// Retrieve RSSI an set TX Power based on RSSI
					gecko_ble_dynamic_tx_power_update(evt->data.evt_le_connection_rssi.rssi);
				}
				break;

			default:
				LOG_INFO("No BLE events have been handled, event %#08x occurred",evt->header);
				handled = 0;
				break;
		}
	}

	return handled;
}

bool gecko_update(struct gecko_cmd_packet* evt)
{
	bool handled = true;

    /* Handle events */
    switch (BGLIB_MSG_ID(evt->header)) {
	 // moved to application main.c
      /* This boot event is generated when the system boots up after reset.
       * Do not call any stack commands before receiving the boot event.
       * Here the system is set to start advertising immediately after boot procedure. */
      case gecko_evt_system_boot_id:

        /* Set advertising parameters. 100ms advertisement interval.
         * The first parameter is advertising set handle
         * The next two parameters are minimum and maximum advertising interval, both in
         * units of (milliseconds * 1.6).
         * The last two parameters are duration and maxevents left as default. */
    	BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_set_advertise_timing(0, ADVERTISE_INTERVAL_250MS, ADVERTISE_INTERVAL_250MS, 0, 0));

    	// Set tx power to 0dB
		gecko_ble_update_tx_power(0);

        /* Start general advertising and enable connections. */
    	BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable));
        break;

      case gecko_evt_le_connection_closed_id:

        /* Check if need to boot to dfu mode */
        if (boot_to_dfu) {
          /* Enter to DFU OTA mode */
          gecko_cmd_system_reset(2);
        } else {

        	// Reset tx power to 0dB
        	gecko_ble_update_tx_power(0);

          /* Restart advertising after client has disconnected */
        	BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable));

        	// Set scheduler to exit polling loop
        	my_state_struct.event_bitmask |= EXIT_EVENT_MASK;
        }
        break;

      /* Events related to OTA upgrading
         ----------------------------------------------------------------------------- */

      /* Check if the user-type OTA Control Characteristic was written.
       * If ota_control was written, boot the device into Device Firmware Upgrade (DFU) mode. */
      case gecko_evt_gatt_server_user_write_request_id:

        if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control) {
          /* Set flag to enter to OTA mode */
          boot_to_dfu = 1;
          /* Send response to Write Request */
          BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_server_send_user_write_response(
            evt->data.evt_gatt_server_user_write_request.connection,
            gattdb_ota_control,
            bg_err_success));

          /* Close connection to enter to DFU OTA mode */
          BTSTACK_CHECK_RESPONSE(gecko_cmd_le_connection_close(evt->data.evt_gatt_server_user_write_request.connection));
        }
        break;

      default:
		handled = false;
		break;
    }
    return handled;
}


void gecko_ble_send_temperature(float tempData)
{

	uint8_t htmTempBuffer[5]; /* Stores the temperature data in the Health Thermometer (HTM) format. */
	uint8_t flags = 0x00;   /* HTM flags set as 0 for Celsius, no time stamp and no temperature type. */
	uint32_t temperature;   /* Stores the temperature data read from the sensor in the correct format */
	uint8_t *p = htmTempBuffer; /* Pointer to HTM temperature buffer needed for converting values to bitstream. */

	/* Convert flags to bitstream and append them in the HTM temperature data buffer (htmTempBuffer) */
	UINT8_TO_BITSTREAM(p, flags);

	/* Convert sensor data to correct temperature format */
	temperature = FLT_TO_UINT32(tempData * 1000, -3);
	/* Convert temperature to bitstream and place it in the HTM temperature data buffer (htmTempBuffer) */
	UINT32_TO_BITSTREAM(p, temperature);

	/* Send indication of the temperature in htmTempBuffer to all "listening" clients.
	* This enables the Health Thermometer in the Blue Gecko app to display the temperature.
	*  0xFF as connection ID will send indications to all connections. */
	gecko_cmd_gatt_server_send_characteristic_notification(0xFF, gattdb_temperature_measurement, 5, htmTempBuffer);

}


void gecko_ble_get_rssi(void)
{
	// Initiate RSSI retrieval before returning in order to tune TX Power
	gecko_cmd_le_connection_get_rssi(conn_handle);
}

void gecko_ble_dynamic_tx_power_update(int8_t rssi)
{
	// Check range of RSSI and assign appropriate TX power
	if(rssi > -35)
	{
		global_tx_power = TXPOWER_MIN;
	}
	else if(rssi > -45)
	{
		global_tx_power = TXPOWER_NEG_20DB;
	}
	else if(rssi > -55)
	{
		global_tx_power = TXPOWER_NEG_15DB;
	}
	else if(rssi > -65)
	{
		global_tx_power = TXPOWER_NEG_5DB;
	}
	else if(rssi > -75)
	{
		global_tx_power = TXPOWER_0DB;
	}
	else if(rssi > -85)
	{
		global_tx_power = TXPOWER_POS_5DB;
	}
	else
	{
		global_tx_power = TXPOWER_MAX;
	}

	//LOG_INFO("TX power updated with a RSSI of %d",rssi);

	// Update TX power based on calculated TX power value
	gecko_ble_update_tx_power(global_tx_power);


}

void gecko_ble_update_tx_power(int16_t power)
{
	struct gecko_msg_system_set_tx_power_rsp_t* rsp;

	// Halt the system in order to change TX power
	gecko_cmd_system_halt(1);

	// Update power
	rsp = gecko_cmd_system_set_tx_power(power);

	//LOG_INFO("TX power updated with a Power setting of %d",rsp->set_power);

	// Resume system
	gecko_cmd_system_halt(0);
}
