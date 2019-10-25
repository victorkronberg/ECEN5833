/*
 * gpio.c
 * GPIO control for LED0 and LED1 as well as input from PB0
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */
#include "gpio.h"
#include "em_gpio.h"
#include <string.h>


/**
 * TODO: define these.  See the radio board user guide at https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf
 * and GPIO documentation at https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__GPIO.html
 */
#define	LED0_port	gpioPortF
#define LED0_pin	pin4
#define LED1_port	gpioPortF
#define LED1_pin	pin5

void gpioInit()
{
	//GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);
	//GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, false);

	// Set PD0 button as input with no filter
	GPIO_PinModeSet(PD0_BUTTON_PORT, PD0_BUTTON_PIN, gpioModeInputPullFilter, true);

	// Disable GPIO interrupts prior to configuring pin interrupts
	//GPIO_IntDisable(PD0_BUTTON_PIN);
	// Configure input interrupt for PD0 button on rising/falling edge - enable interrupts
	GPIO_ExtIntConfig(PD0_BUTTON_PORT,PD0_BUTTON_PIN,PD0_BUTTON_PIN,true,true,true);
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);

	button_state = 0x00;
}

void gpioLed0SetOn()
{
	GPIO_PinOutSet(LED0_port,LED0_pin);
}
void gpioLed0SetOff()
{
	GPIO_PinOutClear(LED0_port,LED0_pin);
}
void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED1_port,LED1_pin);
}
void gpioLed1SetOff()
{
	GPIO_PinOutClear(LED1_port,LED1_pin);
}


void gpioEnableDisplay(void)
{
	GPIO_PinOutSet(DISPLAY_ENABLE_PORT, DISPLAY_ENABLE_PIN);
}

void gpioSetDisplayExtcomin(bool high)
{
	if(high)
	{
		GPIO_PinOutClear(DISPLAY_EXTCOMIN_PORT,DISPLAY_EXTCOMIN_PIN);
	}
	else
	{
		GPIO_PinOutSet(DISPLAY_EXTCOMIN_PORT,DISPLAY_EXTCOMIN_PIN);
	}
}

uint8_t gpio_get_button_state(void)
{
	return button_state;
}

void GPIO_EVEN_IRQHandler(void)
{
	uint32_t gpio_pin_state;
	// Disable interrupt nesting
	__disable_irq();

	// Acknowledge the interrupt and clear even interrupt flags
	GPIO_IntClear(0x5555);

	gpio_pin_state = GPIO_PortInGet(PD0_BUTTON_PORT);

	// Set shift pin state so it is 0 or 1
	gpio_pin_state = ((gpio_pin_state & PD0_BUTTON_PIN_MASK) >> PD0_BUTTON_PIN);

	LOG_INFO("GPIO Pin state is %d",gpio_pin_state);

	// If 0, button is pressed and GPIO pin is grounded
	if(gpio_pin_state == 0)
	{
		gpioLed0SetOn();

		if(server_security_state == pairingConfirmKey)
		{
			gecko_external_signal(PASSKEY_CONFIRM_MASK);
		}
		button_state = 0x01;
	}
	else
	{
		gpioLed0SetOff();
		button_state = 0x00;
	}

	// Update characteristic if bonded
	if(server_security_state == bonded)
	{
		gecko_external_signal(BUTTON_EVENT_MASK);
	}

	// Toggle LED0
	//GPIO_PinOutToggle(LED0_port, LED0_pin);

	__enable_irq();

}
