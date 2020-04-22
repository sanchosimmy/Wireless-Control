/**
 * \file input.c
 * \ingroup WiControl
 * \brief Driver for scanning switches.
 * \author Akshay Pampatwar
 * \version 0.0
 * \date 11th April 2020
 */
#include "input.h"
#include "commonheader.h"
#include "system.h"
//#include "bluetooth.h"
#include "globalvariable.h"
#include "pinmap.h"

//#include <stdbool.h>

#define DEBOUNCE_TIME 20	//In ms

void FanSpeedUpdate(void);
void LedBrightnessUpdate(void);


void Input_Init(void)
{
	 Sys_Eeprom_Init();

	 Sys_GpipInit(DEBOUNCE_TIME);
//	 Wip_Init();
}


void Gpip(uint32_t currentIP)
{
	static uint32_t lastIP = 0;
	static uint32_t finalIP = 0;
	uint32_t changeIpMask = 0;

	changeIpMask = (~(lastIP ^ currentIP))&(currentIP ^ finalIP);
	finalIP = (lastIP & changeIpMask) | (finalIP & ~changeIpMask);
	lastIP = currentIP;


	if(changeIpMask)
	{
		if(changeIpMask & Plug1_SW )
		{
				Plug1_Cmd = (finalIP & Plug1_SW) ;
				Plug1_NCmd = 1;
		}

		if(changeIpMask & Plug2_SW )
		{
				Plug2_Cmd = finalIP & Plug2_SW  ;
				Plug2_NCmd = 1;
		}

		if(changeIpMask & Fan_SW )
		{
			Fan_Cmd = finalIP & Fan_SW  ;
			Fan_NCmd = 1;
		}

		if(changeIpMask & Led_SW )
		{
			Led_Cmd = finalIP & Led_SW  ;
			Led_NCmd = 1;
		}

		//Port A:

		if((changeIpMask >> 8) & Led_Inc_SW )
		{
			if ((finalIP >> 8) & Led_Inc_SW)
			{
				Led_NBrightness = 1;

				if( ++Led_Brightness > MAX_LED_BRIGHTNESS)
					{
						Led_Brightness = MAX_LED_BRIGHTNESS;
						Led_NBrightness = 0;
					}
			}
		}

		if((changeIpMask >> 8) & Led_Dec_SW )
		{
			if ((finalIP >> 8) & Led_Dec_SW)
			{
				Led_NBrightness = 1;

				if(--Led_Brightness < MIN_LED_BRIGHTNESS)
				{
					Led_Brightness = MIN_LED_BRIGHTNESS;
					Led_NBrightness = 0;
				}
			}
		}

		if((changeIpMask >> 8) & Fan_Inc_SW )
		{
			if ((finalIP >> 8) & Fan_Inc_SW)
			{
				Fan_Nspeed = 1;

				if(++Fan_Speed > MAX_FAN_SPEED)
				{
					Fan_Speed = MAX_FAN_SPEED ;
					Fan_Nspeed = 0;
				}
			}
		}

		if((changeIpMask >> 8) & Fan_Dec_SW )
		{
			if ((finalIP >> 8) & Fan_Dec_SW)
			{
				Fan_Nspeed = 1;

				if(--Fan_Speed < MIN_FAN_SPEED)
				{
					Fan_Speed = MIN_FAN_SPEED ;
					Fan_Nspeed = 0;
				}
			}
		}
		changeIpMask = 0; 	//Clear Mask so that it will nt trigger again
	}
}


