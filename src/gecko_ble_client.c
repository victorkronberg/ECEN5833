/*
 * gecko_ble_client.c
 *
 * BLE stack interface.  Contains all functions which interact directly with
 * Silicon Labs BLE Stack for Server.  Utilized code from Silicon Labs example code,
 * specifically Health Thermometer Client example. Also utilized code provided by CU Boulder.
 *
 *  Created on: Oct 12, 2019
 *      Author: Victor Kronberg
 */

#include "gecko_ble_client.h"

// Health Thermometer service UUID defined by Bluetooth SIG
const uint8_t thermoService[2] = { 0x09, 0x18 };
// Temperature Measurement characteristic UUID defined by Bluetooth SIG
const uint8_t thermoChar[2] = { 0x1c, 0x2a };

struct gecko_msg_system_get_bt_address_rsp_t *bt_address;;

ConnProperties conn_properties;
ConnState conn_state;

struct gecko_msg_le_connection_get_rssi_rsp_t *rssi_rsp;

void gecko_ble_init_LCD_status_client(void)
{
	bt_address = gecko_cmd_system_get_bt_address();

	displayPrintf(DISPLAY_ROW_NAME,"Client");

	displayPrintf(DISPLAY_ROW_BTADDR,"%02x:%02x:%02x:%02x:%02x:%02x",bt_address->address.addr[5],
			bt_address->address.addr[4],bt_address->address.addr[3],bt_address->address.addr[2],
			bt_address->address.addr[1],bt_address->address.addr[0]);

	bt_address->address = (bd_addr)SERVER_BT_ADDRESS;

	displayPrintf(DISPLAY_ROW_BTADDR2,"%02x:%02x:%02x:%02x:%02x:%02x",bt_address->address.addr[5],
			bt_address->address.addr[4],bt_address->address.addr[3],bt_address->address.addr[2],
			bt_address->address.addr[1],bt_address->address.addr[0]);

}


bool gecko_ble_client_update(struct gecko_cmd_packet* evt)
{

	//LOG_INFO("BLE event %#08x occurred",evt->header);
	uint8_t* charValue;
	bool handled;

	// If not already handled, there is an open connection
		handled = 1;
		switch BGLIB_MSG_ID(evt->header)
		{
	      /* This boot event is generated when the system boots up after reset.
	       * Do not call any stack commands before receiving the boot event.
	       * Here the system is set to start discovery immediately after boot procedure. */
			case gecko_evt_system_boot_id:

	#ifdef GPIO_DISPLAY_SUPPORT_IMPLEMENTED
				gecko_ble_init_LCD_status_client();
	#endif

				// Set passive scanning on 1Mb PHY
				BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_set_discovery_type(le_gap_phy_1m, 0));
				// Set scan interval and scan window
				BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_set_discovery_timing(le_gap_phy_1m, 16, 16));
				// Start discover using 1M PHY and generic discovery mode
				BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_start_discovery(le_gap_phy_1m,le_gap_discover_generic));

				conn_state = scanning;

	#ifdef GPIO_DISPLAY_SUPPORT_IMPLEMENTED
				// Update LCD with advertising status
				displayPrintf(DISPLAY_ROW_CONNECTION,"Discovering");
	#endif

				break;

	      case gecko_evt_le_gap_scan_response_id:

	    	  	//LOG_INFO("Scan response received");
	    	  	// Check for a connectable packet
	    	  	if (evt->data.evt_le_gap_scan_response.packet_type == 0) {

	    	  		bt_address->address = (bd_addr)SERVER_BT_ADDRESS;
	    	  		bool i = findStaticBluetoothAddress(evt->data.evt_le_gap_scan_response.address, bt_address->address);
	    	  		if(i)
	    	  		{
	    	  			// Stop scanning while connected
	    	  			//gecko_cmd_le_gap_end_procedure();

						LOG_INFO("Connectable device found at %02x:%02x:%02x:%02x:%02x:%02x",evt->data.evt_le_gap_scan_response.address.addr[5],
								evt->data.evt_le_gap_scan_response.address.addr[4],evt->data.evt_le_gap_scan_response.address.addr[3],evt->data.evt_le_gap_scan_response.address.addr[2],
								evt->data.evt_le_gap_scan_response.address.addr[1],evt->data.evt_le_gap_scan_response.address.addr[0]);

						LOG_INFO("Attempting to connect at %02x:%02x:%02x:%02x:%02x:%02x",bt_address->address.addr[5],
								bt_address->address.addr[4],bt_address->address.addr[3],bt_address->address.addr[2],
								bt_address->address.addr[1],bt_address->address.addr[0]);

						// Scan response received, attempt to connect to static address
						BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_connect(bt_address->address,
								evt->data.evt_le_gap_scan_response.address_type,le_gap_phy_1m));
	    	  		}
	    	  	}


	    	  	break;

			// New connection has been opened
			case gecko_evt_le_connection_opened_id:
				// Store the connection handle
				conn_handle = evt->data.evt_le_connection_opened.connection;

#ifdef GPIO_DISPLAY_SUPPORT_IMPLEMENTED
				// Update the LCD with connection state
				displayPrintf(DISPLAY_ROW_CONNECTION,"Connected");
#endif

				BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_discover_primary_services_by_uuid(conn_handle,2,(const uint8*)thermoService));

				conn_state = discoverServices;

				break;

			// External event trigger
			case gecko_evt_system_external_signal_id:
				__disable_irq();
				// Set external events for processing
				my_state_struct.event_bitmask |= evt->data.evt_system_external_signal.extsignals;
				__enable_irq();

				break;


			case gecko_evt_gatt_service_id:

				conn_properties.thermometerServiceHandle = evt->data.evt_gatt_service.service;

				break;

			case gecko_evt_gatt_characteristic_id:

				conn_properties.thermometerCharacteristicHandle = evt->data.evt_gatt_characteristic.characteristic;

				break;

			case gecko_evt_gatt_procedure_completed_id:

				if(conn_state == discoverServices)
				{
					BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_discover_characteristics_by_uuid(evt->data.evt_gatt_procedure_completed.connection,
							conn_properties.thermometerServiceHandle,2,(const uint8_t*)thermoChar));
					conn_state = discoverCharacteristics;
					break;
				}
				if(conn_state == discoverCharacteristics)
				{
					BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_set_characteristic_notification(evt->data.evt_gatt_procedure_completed.connection,
							conn_properties.thermometerCharacteristicHandle,gatt_indication));
					conn_state = enableIndication;

#ifdef GPIO_DISPLAY_SUPPORT_IMPLEMENTED
					// Update the LCD with connection state
					displayPrintf(DISPLAY_ROW_CONNECTION,"Handling Indications");
#endif

					break;
				}

				break;

			case gecko_evt_gatt_characteristic_value_id:

				charValue = &(evt->data.evt_gatt_characteristic_value.value.data[0]); /* Stores the temperature data in the Health Thermometer (HTM) format. */

				conn_properties.temperature = (charValue[1] << 0) + (charValue[2] << 8) + (charValue[3] << 16);

				// Retrieve temperature and print to LCD if display is enabled
				gecko_ble_receive_temperature(conn_properties.temperature);

				// Send confirmation for the indication
				BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_send_characteristic_confirmation(evt->data.evt_gatt_characteristic_value.connection));

				break;

			// RSSI ready to be read
			case gecko_evt_le_connection_rssi_id:
				// Ensure results are valid
				if(evt->data.evt_le_connection_rssi.status == 0)
				{
					// Retrieve RSSI
					conn_properties.rssi = evt->data.evt_le_connection_rssi.rssi;
				}
				break;

			// Connection closed event - restart discovery
			case gecko_evt_le_connection_closed_id:
		    	 // Start discover using 1M PHY and generic discovery mode
		    	 BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_start_discovery(le_gap_phy_1m,	le_gap_discover_generic));

		    	 conn_state = scanning;

#ifdef GPIO_DISPLAY_SUPPORT_IMPLEMENTED
		    	 // Update LCD with advertising status
		    	 displayPrintf(DISPLAY_ROW_CONNECTION,"Discovering");
#endif
				break;

			default:
				LOG_INFO("No BLE events have been handled, event %#08x occurred",evt->header);
				handled = 0;
				break;
		}


	return handled;
}


void gecko_ble_receive_temperature(uint32_t temp_value)
{

	uint32_t tempF_value;


#ifdef GPIO_DISPLAY_SUPPORT_IMPLEMENTED
	// Convert milli-degrees C to degrees F (scaled by 10 to avoid float)
	tempF_value = ((temp_value * 18) + 320000)/10;

	displayPrintf(DISPLAY_ROW_TEMPVALUE,"%d.%d C / %d.%d F",temp_value/1000,(temp_value/100)%10,tempF_value/1000,(tempF_value/100)%10);
#endif

}

/**
 * @return a float value based on a UINT32 value written by FLT_TO_UINT32 and
 * UINT32_TO_BITSTREAM
 * @param value_start_little_endian is a pointer to the first byte of the float
 * which is represented in UINT32 format from FLT_TO_UINT32/UINT32_TO_BITSTREAM
 */
float gattUint32ToFloat(const uint8_t *value_start_little_endian)
{
	int8_t exponent = (int8_t)value_start_little_endian[3];
	uint32_t mantissa = value_start_little_endian[0] +
						(((uint32_t)value_start_little_endian[1]) << 8) +
						(((uint32_t)value_start_little_endian[2]) << 16);
	return (float)mantissa*pow(10,exponent);
}

// Parse advertisements looking for advertised Health Thermometer service
bool findStaticBluetoothAddress(bd_addr server_address,bd_addr target_address)
{
	if(memcmp(server_address.addr,target_address.addr,sizeof(bd_addr)))
	{
		return 0;
	}
	else
	{
		return 1;
	}

}


