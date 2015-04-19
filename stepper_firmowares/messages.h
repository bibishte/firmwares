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


void SDec(void);
void FDec(void);
void ADec(void);
void Fstep(void);
void HStep(void);
void Qstep(void);
void M8Step(void);
void M16Step(void);
void M32Step(void);
void Acc(void);
void Minspeed(void);
void Errormssg(void);
void Maxspeed(void);
void SptRevol(void);
void RMinSpd(void);
void RMaxSpd(void);
void RAcc(void);
void Stop(void);


#endif	/* MESSAGES_H */

