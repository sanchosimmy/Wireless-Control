/**
 * \file bluetooth.h
 * \ingroup WiControl
 * \brief Function prototypes for bluetooth module.
 *
 *  Middle + Hardware abstraction layer.
 * (i)Initializing serial port for communicating with bluetooth module.
 * (ii)Updating data from user to corresponding global variable to control respective appliances.
 * \author Akshay Pampatwar
 * \version  0.0
 * \date 10th April 2020
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

///Wireless input initialization for UART to serially communicate with bluetooth module.
void Wip_Init(void);

///Gate fn for HAL to ML. ISR for rxed data for UART
extern void GetUartData(int32_t);

#endif /* BLUETOOTH_H_ */
