/**
 * \file system.h
 * \ingroup WiControl
 * \brief Fn prototypes for peripherals of tm4c123gh6pm uC.
 *
 * HAL: Hardware abstraction layer.
 * Functions to initialize required peripherals of tm4c123gh6pm uC.
 * \author Akshay Pampatwar
 * \version 0.0
 * \date 9th April 2020
 */
#ifndef SYSTEM_H_
#define SYSTEM_H_

//#include "commonheader.h"
#include <stdint.h>

///Initializes system with 20MHz CPU Clock and enable clk to all required ports
 void Sys_Init(void);

/**
 * Initializes GPIOs as an input for switches and Timer 0 to take care of debounce.
 * @param debounce : Debounce Time setting for switches
 */
 void Sys_GpipInit(uint32_t debounce);

///Init eeprom
 void Sys_Eeprom_Init(void);

 /**
  * Returns content from eeprom corresponding to address passed to it.
  * @param address: 0 to n | n belongs to Natural number
  * @return Byte (int8_t): From addressed passed
  */
 int8_t Sys_Restore_Byte(int8_t address);

/**
 *Store passed contents to an eeprom.
 * @param value: 8 bit data which needs to be stored
 * @param address: 0 to n | n belongs to Natural number
 */
 void Sys_Store_Byte(int8_t value, int8_t address);

#endif /* SYSTEM_H_ */
