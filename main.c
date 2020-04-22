/**
 * \file main.c
 * \ingroup WiControl
 * \brief This is a main file.
 *
 * Consists of controller. Regularly scan for change in input using global variables.
 * \author Akshay Pampatwar
 * \version 0.0
 * \date 9th April 2020
 */

/** \defgroup WiControl Wireless Appliances control
 *
 * \brief This project gives wireless control of home appliances to user.
 * 	Appliances can be turned on / off using app connected via bluetooth or electric switch board.
 */
#include "system.h"
#include "input.h"
#include "bluetooth.h"
#include "plug.h"
#include "led.h"
#include "fan.h"
#include "commonheader.h"
#include "globalvariable.h"



///Initializes controller - Application layer function
void control_init(void);

/**
 * Controller: Controls whole flow of operation
 *
 * Sends updated command to appliances. Requires data from global variable.h
 */
void controller(void);


void main(void)
{
	
		control_init();

		while(1)
		{
			controller();
		}
}

void control_init()
{
	Sys_Init();
	Plug_Init();
	Fan_Init();
	Led_Init();
	Input_Init();
	Wip_Init();

	Fan_Speed = Sys_Restore_Byte(0);
	Fan_Speed_Update(Fan_Speed);
	Led_Brightness = Sys_Restore_Byte(1);
	Led_Intensity_Update(Led_Brightness);
}


void controller()
{

		if(Fan_NCmd)
		{
			if(Fan_Cmd)
				Fan_On();
			else
				 Fan_Off();
			Fan_NCmd = 0;
		}
		 if(Fan_Nspeed)
		{
			Sys_Store_Byte(Fan_Speed, 0);
			Fan_Speed_Update(Fan_Speed);
			Fan_Nspeed = 0;
		}
		if(Led_NCmd)
		{
			if(Led_Cmd)
				Led_On();
			else
				Led_Off();

			Led_NCmd =0;
		}
		if(Led_NBrightness)
		{
			Sys_Store_Byte(Led_Brightness, 1);
			Led_Intensity_Update(Led_Brightness);
			Led_NBrightness=0;
		}
		if(Plug1_NCmd)
		{
			if(Plug1_Cmd)
				Plug1on();
			else
				Plug1off();
		}
		if(Plug2_NCmd)
		{
			if(Plug2_Cmd)
				Plug2on();
			else
				Plug2off();
		}
}
