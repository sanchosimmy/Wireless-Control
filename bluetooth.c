/**
 * \file bluetooth.c
 * \ingroup WiControl
 * \brief Driver for bluetooth module.
 * \author Akshay Pampatwar
 * \version 0.0
 * \date 10th April 2020
 */

#include "bluetooth.h"
#include "commonheader.h"
#include "driverlib/uart.h"

//#include "system.h"
#include "globalvariable.h"
#include "pinmap.h"



const uint32_t BAUDRATE = 9600;

/**
 * Initializez UART module 2 and setup ISR for rxed data
 * @param Baud_Rate
 */
 void Sys_UartInit(uint32_t);

 ///Triggers when required depth of data rxed on Rx FIFO.
 void UART2IntHandler(void);		//ISR for UART 2

 void GetUartData(int32_t RxTemp);

void Wip_Init(void)
{
	Sys_UartInit(BAUDRATE);
}


void GetUartData(int32_t RxTemp)
{
//	Fan_Cmd =1 ;
//	Fan_NCmd = 0;
}

//System level fns:

void Sys_UartInit(uint32_t BaudRate)
{
	    // Enable GPIO port A which is used for UART0 pins.
	    // TODO: change this to whichever GPIO port you are using.
	    //
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	    //
	    // Configure the pin muxing for UART0 functions on port A0 and A1.
	    // This step is not necessary if your part does not support pin muxing.
	    // TODO: change this to select the port/pin you are using.
	    //
	    GPIOPinConfigure(Bt_Tx);
	    GPIOPinConfigure(Bt_Rx);

	    //
	    // Enable UART0 so that we can configure the clock.
	    //
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);

	    //
	    // Use the internal 16MHz oscillator as the UART clock source.
	    //
	    UARTClockSourceSet(UART2_BASE, UART_CLOCK_PIOSC);

	    //
	    // Select the alternate (UART) function for these pins.
	    // TODO: change this to select the port/pin you are using.
	    //
	    GPIOPinTypeUART(GPIO_PORTD_BASE, Bt_Tx_Pin| Bt_Rx_Pin);

	    // Initialize the UART. Set the baud rate, number of data bits, turn off
	    // parity, number of stop bits, and stick mode.
	    //
	    UARTConfigSetExpClk(UART2_BASE,16000000 , BaudRate,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));	//SysCtlClockGet()
	    //Enable FIFO
	    UARTFIFOEnable(UART2_BASE);
	    //Uart FIFO level after which interrupt is generated
	    UARTFIFOLevelSet(UART2_BASE,UART_FIFO_TX1_8,UART_FIFO_RX1_8);
	    //Enable UART interrupt
	    UARTIntEnable(UART2_BASE,UART_INT_RX);
	    //Link ISR fn call to UART 2 interrupt
	    UARTIntRegister(UART2_BASE, UART2IntHandler);
	    // Enable the UART 2 interrupt on the processor (NVIC).
	    IntEnable(INT_UART2);
	    // Enable the UART.
	    UARTEnable(UART2_BASE);
}

void UART2IntHandler( void )
{
	UARTIntClear(UART2_BASE,UART_INT_RX);
	UARTIntDisable(UART2_BASE,UART_INT_RX);

	while(UARTCharsAvail(UART2_BASE))
	{
		GetUartData(UARTCharGetNonBlocking(UART2_BASE));
	}

	UARTIntEnable(UART2_BASE,UART_INT_RX);
}
