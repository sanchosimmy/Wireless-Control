/**
 * \file fan.h
 * \ingroup WiControl
 * \brief Functionn prototypes for fan controlling.
 *
 * HA + Middle layer. In case of reset of board, Saved last fan speed will be set.
 * Requires gloabalvariable.h access for MAXFANSPEED value.
 * (i)Initializes relevant port pin as an o/p, ADC pin and two timers.
 * (ii)Calculates AC voltage from ADC reading.
 * (iii)Depending on AC rms vtg and user provided speed, Calculates trigger delay after zero crossing of AC vtg for scr driving.
 * (iv)In case of voltage fluctuation speed of fan will remain same.
 *
 * \author Akshay Pampatwar
 * \version 0.0
 * \date 15th April 2020
 */

#ifndef FAN_H_
#define FAN_H_

#include <stdint.h>

///Initializes fan SCR driving port pin and ADC pin.
void Fan_Init(void);
///Turns on the fan, Fan speed rxed from Fan_Speed_Update(int8_t) function.
void Fan_On(void);
///Turns off fan speed.
void Fan_Off(void);
/**
 * Changes fan speed.
 * @param FanSpeed: Fan speed from 0 to 5. lower limit should not be less than 0.
 */
extern void Fan_Speed_Update(int8_t);

#endif /* FAN_H_ */
