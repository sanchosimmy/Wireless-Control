/**
 * \file plug.c
 * \ingroup WiControl
 * \brief Driver for controlling two plugs.
 * \author Akshay Pampatwar
 * \version 0.0
 * \date 15th April 2020
 */

#include "plug.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "pinmap.h"


//void plug1_init(void);
//void plug2_init(void);

void Plug_Init()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	//* * * * * * Setting Ouput for plug1 * * * * * * * *

	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, Plug1_Op);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, Plug1_State);

	// * * * * * * Setting Ouput for plug2* * * * * * *

	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, Plug2_Op);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, Plug2_State);
}

void Plug1on()
{
	GPIOPinWrite(GPIO_PORTC_BASE,(Plug1_Op),Plug1_Op);
	GPIOPinWrite(GPIO_PORTF_BASE,(Plug1_State),Plug1_State);
}
void Plug1off()
{
	GPIOPinWrite(GPIO_PORTC_BASE,(Plug1_Op),~Plug1_Op);
	GPIOPinWrite(GPIO_PORTF_BASE,(Plug1_State),~Plug1_State);
}
void Plug2on()
{
	GPIOPinWrite(GPIO_PORTC_BASE,(Plug2_Op),Plug2_Op);
	GPIOPinWrite(GPIO_PORTF_BASE,(Plug2_State),Plug2_State);
}
void Plug2off()
{
	GPIOPinWrite(GPIO_PORTC_BASE,(Plug2_Op),~Plug2_Op);
	GPIOPinWrite(GPIO_PORTF_BASE,(Plug2_State),~Plug2_State);
}
