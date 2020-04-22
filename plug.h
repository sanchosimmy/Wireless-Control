/**
 * \file plug.h
 * \ingroup WiControl
 * \brief Function prototypes for controlling two plugs.
 *
 * 	Middle + HA layer.
 *  (i)Functions to initialize required port pin as o/p.
 *  (ii)Fns for controlling o/p of plug and indicator LEDs
 * \author Akshay Pampatwar
 * \version 0.0
 * \date 15th April 2020
 */

#ifndef PLUG_H_
#define PLUG_H_

///Init corresponding gpio as an o/p
void Plug_Init(void);

///Turns on plug 1 and its indicator
void Plug1on(void);
///Turns off plug 1 and its indicator
void Plug1off(void);
///Turns on plug 2 and its indicator
void Plug2on(void);
///Turns off plug 2 and its indicator
void Plug2off(void);

#endif /* PLUG_H_ */
