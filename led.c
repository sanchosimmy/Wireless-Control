/**
 * \file led.c
 * \ingroup WiControl
 * \brief Driver for LED strip control.
 * \author Akshay Pampatwar
 * \version 0.0
 * \date Apr 17, 2020
 */
#include "led.h"
#include "commonheader.h"
#include "pinmap.h"
#include "globalvariable.h"

static unsigned long ulPeriod = 1000; // with a 16-bit timer, maximum is 65535
static unsigned long dutyCycle = 250; // 75 % duty cycle (0.25 * ulPeriod)

///Initializes Timer2A for PWM and configure port pin for the PWM o/p
void Timer2APWMInit(void);



void Led_Init()
{
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, Led_State);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, Led_Op_Pin);

    Timer2APWMInit();
}

void Timer2APWMInit()
{
	    // The PWM peripheral must be enabled for use.

//	    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

		// For this example PWM0 is used with PortB Pin6.  The actual port and pins
	    // used may be different on your part, consult the data sheet for more
	    // information.  GPIO port B needs to be enabled so these pins can be used.
	    // TODO: change this to whichever GPIO port you are using.
	    //
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	    // Configure the GPIO pin muxing to select PWM00 functions for these pins.
	    // This step selects which alternate function is available for these pins.
	    // This is necessary if your part supports GPIO pin function muxing.
	    // Consult the data sheet to see which functions are allocated per pin.
	    // TODO: change this to select the port/pin you are using.

//	    GPIOPinConfigure(GPIO_PB6_M0PWM0);

//-------------------------------------------------------
	    //
	    // Set the PWM clock to the system clock.
	    //
	    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

	    // Configure PB6 as T0CCP0
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		GPIOPinConfigure(Led_Op);
		GPIOPinTypeTimer(GPIO_PORTF_BASE, Led_Op_Pin);

		// Configure timers
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
		TimerConfigure(TIMER2_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM);
		TimerLoadSet(TIMER2_BASE, TIMER_A, ulPeriod -1);
		TimerMatchSet(TIMER2_BASE, TIMER_A, dutyCycle); // PWM
//		TimerEnable(TIMER1_BASE, TIMER_A);
}

void Led_On()
{
	TimerEnable(TIMER2_BASE, TIMER_A);
//	TimerDisable(TIMER1_BASE, TIMER_A);
//	GPIOPinWrite(GPIO_PORTF_BASE,Led_Op,Led_Op);
}
void Led_Off()
{
	TimerDisable(TIMER2_BASE, TIMER_A);
//	GPIOPinConfigure(GPIO_PB6_M0PWM0);
	GPIOPinWrite(GPIO_PORTF_BASE,Led_Op_Pin,~Led_Op_Pin);
}

void Led_Intensity_Update(int8_t Led_Intensity)
{
	dutyCycle = (MAX_LED_BRIGHTNESS - Led_Intensity) * (ulPeriod -1)/ MAX_LED_BRIGHTNESS;

	TimerMatchSet(TIMER2_BASE, TIMER_A, dutyCycle); // PWM
}

