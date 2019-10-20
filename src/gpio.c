/*
 * gpio.c
 * GPIO control for LED0 and LED1
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

uint32_t ledstatus;

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
	// Configure input interrupt for PD0 button on falling edge - keep interrupt disabled
	GPIO_ExtIntConfig(PD0_BUTTON_PORT,PD0_BUTTON_PIN,PD0_BUTTON_PIN,false,true,true);
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);

	//GPIO_IntEnable(PD0_BUTTON_PIN);
	ledstatus = 0;
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

void GPIO_EVEN_IRQHandler(void)
{
	// Disable interrupt nesting
	__disable_irq();

	// Acknowledge the interrupt and clear flags
	GPIO_IntClear(0x5555);

	// Toggle LED0
	GPIO_PinOutToggle(LED0_port, LED0_pin);

	__enable_irq();

}
