/************************************************************************
 Title:    All the printed masseges
 Author:   Bilyana Borisova <bibishte@gmail.com>
 File:     $Id: drv_8825.h 2015/04/19
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


#ifndef MESSAGES_H
#define	MESSAGES_H


#include <stdint.h>

#define SDEC 0
#define FDEC 1
#define ADEC 2
#define FSTEP 3
#define HSTEP 4
#define QSTEP 5
#define M8 6
#define M16 7
#define M32 8
#define ACC 9
#define MINSPD 10
#define ERROR 11
#define MAXSPD 12
#define SPR 13
#define RMINSPD 14
#define RMAXSPD 15
#define RACC 16
#define STOP 17
#define ENBL 18
#define DISBL 19
#define NSM 20
#define NSD 21
#define DONE 22
#define STEPMSG 23

/** 
 *  @defgroup biba_messges messages Library
 *  @code #include <messages.h> @endcode
 * 
 *  @brief This module contains functions that are printing messages 
 *
 * This module contains functions that are printing stuff
 * 
 *  @note Typical functions that are saving me memory.
 *  @author Bilyana Borisova bibishte@gmail.com
 */

/**@{*/



//void SDec(void);
//void FDec(void);
//void ADec(void);
//void Fstep(void);
//void HStep(void);
//void Qstep(void);
//void M8Step(void);
//void M16Step(void);
//void M32Step(void);
//void Acc(void);
//void Minspeed(void);
//void Errormssg(void);
//void Maxspeed(void);
//void SptRevol(void);
//void RMinSpd(void);
//void RMaxSpd(void);
//void RAcc(void);
//void Stop(void);
//void Microstep(void);
void Message(uint8_t a);
void Message_xcall(uint8_t a, uint16_t (*func)(void));

#endif	/* MESSAGES_H */

