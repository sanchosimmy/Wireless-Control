/**
 * \file input.h
 * \ingroup WiControl
 * \brief Function prototype for scanning switches and updating final status of input.
 *
 * Middle layer.
 * Read switches for input and updates status in global variables
 * \author Akshay Pampatwar
 * \version 0.0
 * \date 11th April 2020
 */
#ifndef INPUT_H_
#define INPUT_H_

#include <stdint.h>

///Middle layer fn. Initializes GPIO as an input for all the switches
void Input_Init(void);

/**
 * ISR: Called after every debounce period (20mS).
 * Gate called from HAL to Middle layer
 *
 * (i)Take care of bounce back effect of switches.
 * (ii)Updates Changed in a output to a global variable related to home appliances.
 * @param currentIP : 32 bit which is an encoded status of all switches interface with Port A and Port D.
 * 			2nd last byte = Port A & LSByte = Port D
 */
 extern void Gpip(uint32_t currentIP);			//ISR for timer

#endif /* INPUT_H_ */
