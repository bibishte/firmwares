/************************************************************************
 Title:    Macros and configuration of ports for DRV8825
 Author:   Bilyana Borisova <bibishte@gmail.com>
 File:     $Id: config.h 2015/04/19
 Software: AVR-GCC 4.9.2, AVR Libc 1.8.0
 Hardware: DRV8825 & ATmega8
 License:  GNU General Public License v2
 Usage:    see Doxygen manual

 LICENSE:
 Copyright (C) 2015 Bilyana Borisova

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License v2 as published by
 the Free Software Foundation; either version 2 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License v2 for more details.

 ************************************************************************/

#ifndef CONFIG_H_
#define CONFIG_H_
/** 
 *  @defgroup biba_config config Library
 *  @code #include <config.h> @endcode
 * 
 *  @brief General macros for port manipolation and  port naming
 *
 *  Defines simple macros for setting bits and bytes for manipolating DRV8825
 * 
 *  @author Bilyana Borisova bibishte@gmail.com
 */

/**@{*/


#include <avr/io.h>

#define set_bit(byte, bit) byte |= (1 << bit)
#define clr_bit(byte, bit) byte &= ~(1 << bit)

/** \def set_as_output
 * Write logical one bit \c DDxn in IO register \c DDRx.
 * This will configure Pxn as an output pin. */
#define set_as_output(DDRx, DDxn) DDRx |= (1 << DDxn)

/** \def set_as_input
 * Write logical zero bit \c DDxn in IO register \c DDRx.
 * This will configure Pxn as an input pin. */
#define set_as_input(DDRx, DDxn) DDRx &= ~(1 << DDxn)

/** \def set_port
 * Write logic one bit \c PORTxn in IO register \c PORTx.
 * When the pin is configured as an output pin,
 * the port pin is driven high (one).*/
#define set_port(PORTx, PORTxn) PORTx |= (1 << PORTxn)

/** \def clr_port
 * Write logic zero bit \c PORTxn in IO register \c PORTx.
 * When the pin is configured as an output pin,
 * the port pin is driven low (zero).*/
#define clr_port(PORTx, PORTxn) PORTx &= ~(1 << PORTxn)

/** \def check_pin
 * Test whether bit \c PINxn in IO register \c PINx is set.
 * This will return a 0 if the port pin is driven low, and non-zero
 * if the pin is driven high. */
#define check_pin(PINx, PINxn) (PINx & (1 << PINxn))

#define CHECKPIN(x,y)	((x&(1<<y))!=0?1:0)


//Inputs
#define RXD                     PD0
#define HOME                    PC2
#define SCL                     PB5
#define MISO                    PB4
#define FAULT                   PB1


//Outputs

#define ENABLE_STEPPER          PD5
#define TXD                     PD1
#define MODE0                   PC3
#define MODE1                   PC4
#define MODE2                   PC5
#define DECAY                   PC1
#define STEP                    PC0
#define MOSI                    PB3
#define DIRECTION               PB2
#define DIR_485                 PB0

//Input ports
#define PRXD                     PORTD
#define PHOME                    PORTC
#define PSCL                     PORTB
#define PMISO                    PORTB
#define PFAULT                   PORTB


//Output ports

#define PENABLE_STEPPER          PORTD
#define PTXD                     PORTD
#define PMODE0                   PORTC
#define PMODE1                   PORTC
#define PMODE2                   PORTC
#define PDECAY                   PORTC
#define PSTEP                    PORTC
#define PMOSI                    PORTB
#define PDIRECTION               PORTB
#define PDIR_485                 PORTB

//Input direction
#define DRXD                     DDRD
#define DHOME                    DDRC
#define DSCL                     DDRB
#define DMISO                    DDRB
#define DFAULT                   DDRB


//Output direction

#define DENABLE_STEPPER          DDRD
#define DTXD                     DDRD
#define DMODE0                   DDRC
#define DMODE1                   DDRC
#define DMODE2                   DDRC
#define DDECAY                   DDRC
#define DSTEP                    DDRC
#define DMOSI                    DDRB
#define DDIRECTION               DDRB
#define DDIR_485                 DDRB


//Input pins
#define IRXD                     PIND
#define IHOME                    PINC
#define ISCL                     PINB
#define IMISO                    PINB
#define IFAULT                   PINB


//Output pins
#define IENABLE_STEPPER          PIND
#define ITXD                     PIND
#define IMODE0                   PINC
#define IMODE1                   PINC
#define IMODE2                   PINC
#define IDECAY                   PINC
#define ISTEP                    PINC
#define IMOSI                    PINB
#define IDIRECTION               PINB
#define IDIR_485                 PINB

#define true 1==1
#define false 1!=1

#endif /* CONFIG_H_ */
