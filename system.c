
/**
 * \file system.c
 * \ingroup WiControl
 * \brief Driver for peripherals of tm4c123gh6pm uC
  * \author Akshay Pampatwar
 * \version 0.0
 * \date 10th April 2020
 */

#include "system.h"
#include "pinmap.h"
#include "commonheader.h"
#include "input.h"
#include "bluetooth.h"
#include "driverlib/uart.h"
#include "driverlib/eeprom.h"



//void Sys_UartInit(uint32_t BaudRate);

/**
 * Init Timer0 for periodic triggering of a ISR for scanning i/p.
 * @param debounce : mS
 */
void Timer0Init(uint32_t debounce);
///ISR for Timer0 : Reads port A and D, packs it and passed on to Gpip() fn in input.c
void Timer0IntHandler(void);

void Sys_Init(void){
 	//Set CPU Clock to 20MHz. 400MHz PLL/2 = 200 DIV 10 = 20MHz
 	SysCtlClockSet(SYSCTL_SYSDIV_10|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

 	//Peripherals clk enable
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

}

void Sys_GpipInit(uint32_t debounce)
{
		GPIOPinTypeGPIOInput(GPIO_PORTA_BASE,PortA_IpMask);
		GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,PortD_IpMask);

		GPIOPadConfigSet(GPIO_PORTA_BASE,PortA_IpMask,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOPadConfigSet(GPIO_PORTD_BASE,PortD_IpMask,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD_WPU);

		Timer0Init(debounce);
}

void Sys_Eeprom_Init()
{
	/* EEPROM SETTINGS */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0); // EEPROM activate
	EEPROMInit(); // EEPROM start
}

int8_t Sys_Restore_Byte(int8_t address)
{
	uint32_t temp;
	EEPROMRead(&temp, 4*address, 4);
	return (int8_t) temp;
}

void Sys_Store_Byte(int8_t value, int8_t address)
{
	uint32_t	temp;
	temp = (uint32_t) value;
	temp = EEPROMProgram(&temp,4*address,4);
}

void Timer0Init(uint32_t debounce)
{
	// The Timer0 peripheral must be enabled for use.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	// The Timer0 peripheral must be enabled for use.
	TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT);
    //Load Timer value
	TimerLoadSet(TIMER0_BASE,TIMER_A, SysCtlClockGet()*debounce/1000);
	// Enable processor interrupts.
    IntMasterEnable();
    // Configure the Timer0A interrupt for timer timeout.
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    //Link ISR fn call to timer 0 interrupt
    TimerIntRegister(TIMER0_BASE,TIMER_A, Timer0IntHandler);
    // Enable the Timer0A interrupt on the processor (NVIC).
    IntEnable(INT_TIMER0A);
    // Enable Timer0B.
    TimerEnable(TIMER0_BASE, TIMER_A);
}

void Timer0IntHandler(void)
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	Gpip((GPIOPinRead(GPIO_PORTA_BASE,PortA_IpMask)<<8) | GPIOPinRead(GPIO_PORTD_BASE,PortD_IpMask));

}
