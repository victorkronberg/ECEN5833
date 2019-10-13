/*
 * main.c
 *
 * Main function
 *
 * Created on: September 9, 2019
 *  Author: Victor Kronberg
 *
 */

#include "main.h"

uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

#ifdef	EnergyMode3
const SLEEP_EnergyMode_t blocked_sleep_mode = sleepEM4;
#elif	EnergyMode2
const SLEEP_EnergyMode_t blocked_sleep_mode = sleepEM3;
#elif	EnergyMode1
const SLEEP_EnergyMode_t blocked_sleep_mode = sleepEM2;
#endif

// Gecko configuration parameters (see gecko_configuration.h)
static const gecko_configuration_t config = {
  .config_flags = 0,
  .sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100, // ppm
  .gattdb = &bg_gattdb_data,
  .ota.flags = 0,
  .ota.device_name_len = 3,
  .ota.device_name_ptr = "OTA",
#if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
  .pa.config_enable = 1, // Enable high power PA
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
};


int main(void)
{

	__disable_irq();

	/* Event pointer for handling events */
	struct gecko_cmd_packet* evt;

  // Initialize device
  initMcu();
  // Initialize board
  initBoard();
  // Initialize application
  initApp();

  // Initialize logging
  logInit();

  // Initialize event flag
  my_state_struct.event_bitmask = 0;

  // Initialize GPIO
  gpioInit();

  // Initialize timer
  init_letimer();

  init_si7021();

  i2c_init();

  displayInit();

#ifdef BUILD_INCLUDES_BLE_SERVER
  my_state_struct.current_state = STATE0_WAIT_FOR_BLE;
  my_state_struct.next_state = STATE0_WAIT_FOR_BLE;
#endif

  // Set deepest sleep mode
  SLEEP_SleepBlockBegin(blocked_sleep_mode);

  //init_timer_interrupt();

  // Initialize BLE stack.
  // This is disabled for assignments #2, 3 and 4 as it will prevent sleep modes below EM2
  gecko_init(&config);

  __enable_irq();

  /* Infinite loop */
  while (1)
  {

#if BUILD_INCLUDES_BLE_SERVER
		// Check for external event
		if(my_state_struct.event_bitmask != 0)
		{
			// Call scheduler
			my_scheduler(&my_state_struct);

		}
		else
		{
			/* Check for stack event. */
			// BLE sleep
			evt = gecko_wait_event();

			//LOG_INFO("Wake event");
			// Server BLE update
			gecko_ble_server_update(evt);
		}
#endif

#if BUILD_INCLUDES_BLE_CLIENT

		if(my_state_struct.event_bitmask != 0)
		{
			client_scheduler(&my_state_struct);
		}
		else
		{
			/* Check for stack event. */
			// BLE sleep
			evt = gecko_wait_event();

			//LOG_INFO("Wake event");
			// Client BLE update
			gecko_ble_client_update(evt);
		}

#endif

  }
}
