/*
 * Main heading
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

static void delayApproxOneSecond(void)
{
	/**
	 * Wait loops are a bad idea in general!  Don't copy this code in future assignments!
	 * We'll discuss how to do this a better way in the next assignment.
	 */
	volatile int i;
	i=0;
	for (i = 0; i < 350000; ) {
		  i=i+1;
	}
}


int main(void)
{

	/* Event pointer for handling events */
	struct gecko_cmd_packet* evt;

	// Configure deepest sleep variable
	SLEEP_EnergyMode_t sleep_mode;

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


  my_state_struct.current_state = STATE0_WAIT_FOR_BLE;
  my_state_struct.next_state = STATE0_WAIT_FOR_BLE;

  // Set deepest sleep mode
  SLEEP_SleepBlockBegin(blocked_sleep_mode);

  //init_timer_interrupt();

  // Initialize BLE stack.
  // This is disabled for assignments #2, 3 and 4 as it will prevent sleep modes below EM2
  gecko_init(&config);

  /* Infinite loop */
  while (1)
  {

		// Check for external event
		if(my_state_struct.event_bitmask != 0)
		{
			__disable_irq();

			// Call scheduler
			my_scheduler(&my_state_struct);

			__enable_irq();
		}
		else
		{
			/* Check for stack event. */
			// BLE sleep
			evt = gecko_wait_event();

			LOG_INFO("Wake event");

			__disable_irq();

			gecko_ble_update(evt);

			__enable_irq();

		}

  }
}

