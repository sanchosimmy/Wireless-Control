/**
 * \file globalvariable.c
 * \ingroup WiControl
 * \brief Gloabal variables defination and initialization.
 * \author Akshay Pampatwar
 * \version  0.0
 * \date 14th April 2020
 */

#include "globalvariable.h"

const int MAX_FAN_SPEED=5;
const int MIN_FAN_SPEED = 0;
const int MAX_LED_BRIGHTNESS= 5;
const int MIN_LED_BRIGHTNESS=0;

bool Fan_Cmd = 0;
int8_t Fan_Speed = 0; //Here MIN_FAN_SPEED to MAX_FAN_SPEED i.e. 0 to 5 I
bool Led_Cmd = 0;
int8_t Led_Brightness = 0; //MIN_LED_BRIGHTNESS to MAX_LED_BRIGHTNESS i.e. 0 to 5
bool Plug1_Cmd = 0;
bool Plug2_Cmd = 0;

//New command mask i.e. ccmd is updated
bool Fan_NCmd = 0;
bool Fan_Nspeed = 0;
bool Led_NCmd = 0;
bool Led_NBrightness = 0;
bool Plug1_NCmd = 0;
bool Plug2_NCmd = 0;


