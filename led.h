/**
 * \file led.h
 * \ingroup WiControl
 * \brief Function prototypes for LED strip control.
 *
 * Middle layer.
 * Initializes o/p port to command LED driver.
 * \author Akshay Pampatwar
 * \version 0.0
 * \date Apr 17, 2020
 */

#ifndef LED_H_
#define LED_H_

#include <stdint.h>

///Initializes o/p pin & PWM for driving LED. Check pinmap.h for pin no. Uses Timer 2A
void Led_Init(void);
///Turn On LED, Intensity depends on parameter passed via Led_Intensity_Update(int8_t);
void Led_On(void);
///Turns Off LED
void Led_Off(void);
/// Led Intensity: 0 to 5. Uses global variable for max value allowed
void Led_Intensity_Update(int8_t);

#endif /* LED_H_ */
