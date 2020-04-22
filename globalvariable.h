/**
 * \file globalvariable.h
 * \ingroup WiControl
 * \brief Global variables for saving status of all inputs.
 *
 * Application layer and middle layer.
 * Includes declaration of inputs of all appliances.
 * Used for transferring data from input.h and bluetooth.h to controller.
 * Also required for fan.h and led.h since it uses MAX_LED_BRIGHTNESS & MAX_FAN_SPEED.
 * \author Akshay Pampatwar
 * \version  0.0
 * \date 12th April 2020
 */

#ifndef GLOBALVARIABLE_H_
#define GLOBALVARIABLE_H_

#include <stdbool.h>
#include <stdint.h>

extern const int MAX_FAN_SPEED;
extern const int MIN_FAN_SPEED;
extern const int MAX_LED_BRIGHTNESS;
extern const int MIN_LED_BRIGHTNESS;

extern bool Fan_Cmd;
extern int8_t Fan_Speed; //Here MIN_FAN_SPEED to MAX_FAN_SPEED i.e. 0 to 5 I
extern bool Led_Cmd;
extern int8_t Led_Brightness; //MIN_LED_BRIGHTNESS to MAX_LED_BRIGHTNESS i.e. 0 to 5
extern bool Plug1_Cmd;
extern bool Plug2_Cmd;

//New command mask i.e. cmd is updated
extern bool Fan_NCmd;
extern bool Fan_Nspeed;
extern bool Led_NCmd;
extern bool Led_NBrightness;
extern bool Plug1_NCmd;
extern bool Plug2_NCmd;

#endif /* GLOBALVARIABLE_H_ */
