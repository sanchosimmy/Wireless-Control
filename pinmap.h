/**
 * \file pinmap.h
 * \ingroup WiControl
 * \brief Pin assignment of i/p & o/p of appliances with uC.
 *
 * HAL: Hardware abstraction layer.
 * Maps pin of tm4c123gh6pm uC with corresponding functionality.
 * @note U can only change PIN maping in the same port.
 * 		i.e. If u want to change pin no. assignment in portA then updated pin should be assigned in a port A only.
 * \author $Akshay Pampatwar $
 * \version $0.0 $
 * \date $10th April 2020 $
 */

#ifndef PINMAP_H_
#define PINMAP_H_

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

//Switch Input:

	//Port D:
	#define Plug1_SW GPIO_PIN_0
	#define Plug2_SW GPIO_PIN_1
	#define Led_SW GPIO_PIN_2
	#define Fan_SW GPIO_PIN_3

	//Port A:
	#define Led_Inc_SW GPIO_PIN_4
	#define Led_Dec_SW GPIO_PIN_5
	#define Fan_Inc_SW GPIO_PIN_6
	#define Fan_Dec_SW GPIO_PIN_7

//LED Indicator O/P:

	//Port A:
	#define Fan_State GPIO_PIN_3
	#define Led_State GPIO_PIN_2
	//Port B:
	#define Fan_1 GPIO_PIN_2
	#define Fan_2 GPIO_PIN_3
	//Port E:
	#define Fan_3 GPIO_PIN_0
	//Port C:
	#define Fan_4 GPIO_PIN_4
	//Port F:
	#define Fan_5 GPIO_PIN_0
	#define Plug1_State GPIO_PIN_2
	#define Plug2_State GPIO_PIN_3

//Control O/P:

	//Port C:
	#define Plug1_Op GPIO_PIN_5
	#define Plug2_Op GPIO_PIN_6
	#define Fan_Scr_Op GPIO_PIN_7
	//Port F:
	#define Led_Op GPIO_PF4_T2CCP0
	#define Led_Op_Pin GPIO_PIN_4

//Sensor I/P:

	//Port F:
	#define ZCD GPIO_PIN_5
	//Port E:
		//If changing this pin then need to make changes in fan.c
	#define AC_IN GPIO_PIN_2
//	#define AC_IN_Pin GPIO_PIN_2

//Bluetooth:

	//Port B:
	#define Bt_En GPIO_PIN_5
	//Port D:
	#define Bt_Tx GPIO_PD7_U2TX
	#define Bt_Rx GPIO_PD6_U2RX

	#define Bt_Tx_Pin GPIO_PIN_7
	#define Bt_Rx_Pin GPIO_PIN_6

//Extra Header:

	//Port B:
	#define GPIO_1 GPIO_PIN_0
	#define GPIO_2 GPIO_PIN_1
	#define ADC1 GPIO_PIN_4
	//Port E:
	#define Rx GPIO_PIN_4
	#define Tx GPIO_PIN_5
	#define PWM GPIO_PIN_3

//Port A:
	//Input Mask
	#define PortA_IpMask ( Led_Inc_SW | Led_Dec_SW | Fan_Inc_SW | Fan_Dec_SW )
	//Output Mask
	#define PortA_OpMask ( Fan_State | Led_State )

//Port B:
	//Input Mask
	#define PortB_IpMask ( 0 )
	//Output Mask
	#define PortB_OpMask ( Fan_1 | Fan_2 | Bt_En )

//Port C:
	//Input Mask
	#define PortC_IpMask ( 0 )
	//Output Mask
	#define PortC_OpMask ( Fan_4 | Plug1_Op | Plug2_Op | Fan_Scr_Op)

//Port D:
	//Input Mask
	#define PortD_IpMask ( Plug1_SW | Plug2_SW | Led_SW | Fan_SW )

//Port E:
	//Input Mask
	#define PortE_IpMask ( 0 ) 	//AC_IN )
	//Output Mask
	#define PortE_OpMask ( Fan_3 )

//Port E:

#endif /* PINMAP_H_ */

