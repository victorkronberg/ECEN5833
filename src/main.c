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

myStateTypeDef my_state_struct;

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

	float temperature;

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



  my_state_struct.current_state = STATE0_WAIT_FOR_TIMER;

  // Set deepest sleep mode
  SLEEP_SleepBlockBegin(blocked_sleep_mode);

  //init_timer_interrupt();

  CORE_DECLARE_IRQ_STATE;
  // Initialize BLE stack.
  // This is disabled for assignments #2, 3 and 4 as it will prevent sleep modes below EM2
  // gecko_init(&config);
  /* Infinite loop */
  while (1) {

	  delayApproxOneSecond();
	  LOG_INFO("The current time is:");


	  // Check for event on wake
	  if(my_state_struct.event_bitmask == 0)
	  {
		  // Wait for event
		  SLEEP_Sleep();
	  }
	  else
	  {
		  CORE_ENTER_CRITICAL();

		  // Call scheduler
		  my_scheduler(&my_state_struct);

		  /*if(((TIMER_EVENT_MASK & event_bitmask) >> TIMER_EVENT_MASK_POS) == 1)
		  {
			  disable_letimer();
			  temperature = read_temp();
			  // Clear event bitmask
			  event_bitmask &= ~TIMER_EVENT_MASK;
			  reset_timer_interrupt();
		  }
		  */
		  CORE_EXIT_CRITICAL();
	  }




  }
}

void my_scheduler(myStateTypeDef *state_struct)
{
	switch (state_struct->current_state)
	{
		case STATE0_WAIT_FOR_TIMER:
			if( ((state_struct->event_bitmask & TIMER_EVENT_MASK) >> TIMER_EVENT_MASK_POS) == 1 )
			{
				// Clear event bitmask
				state_struct->event_bitmask &= ~TIMER_EVENT_MASK;

				// Reset period interrupt
				reset_periodic_timer();

				// Call I2C power-up
				enable_si7021_power();

				// Set timer for power-up delay
				delay_ms(POWER_UP_DELAY);

				// Set next state
				state_struct->next_state = STATE1_I2C_POWER_UP;

			}
			break;
		case STATE1_I2C_POWER_UP:
			if( ((state_struct->event_bitmask & DELAY_EVENT_MASK) >> DELAY_EVENT_MASK_POS) == 1 )
			{
				// Clear event bitmask
				state_struct->event_bitmask &= ~DELAY_EVENT_MASK;

				// Add sleep blocking

				// Start I2C write

				// Set next state
				state_struct->next_state = STATE2_I2C_WRITE;
			}
			break;
		case STATE2_I2C_WRITE:
			//if( ((state_struct->event_bitmask & I2C_EVENT_MASK) >> I2C_EVENT_MASK_POS) == 1 )
			//{
				// Clear event bitmask
				state_struct->event_bitmask &= ~I2C_EVENT_MASK;

				// Handle I2C event - check for complete

				// What to do while we wait for write to complete? Do we need an if here?

				delay_ms(CONVERSION_DELAY);
				// Set next state if complete
				state_struct->next_state = STATE3_I2C_WAIT;
			//}
			break;
		case STATE3_I2C_WAIT:
			// Deep sleep here?
			state_struct->next_state = STATE4_I2C_READ;
			break;
		case STATE4_I2C_READ:
			state_struct->next_state = STATE5_I2C_POWER_DOWN;
			break;
		case STATE5_I2C_POWER_DOWN:
			disable_si7021_power();
			state_struct->next_state = STATE0_WAIT_FOR_TIMER;
			// Remove sleep blocking
			break;
		default:
			break;
	}

	if(state_struct->current_state != state_struct->next_state)
	{
		LOG_DEBUG("State transitioned from state %d to %d",
				state_struct->current_state,state_struct->next_state);
		state_struct->current_state = state_struct->next_state;
	}

}
