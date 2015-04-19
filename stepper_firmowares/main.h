/************************************************************************
 Title:    This is the main fail with continuos loop 
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

#ifndef MAIN_H
#define	MAIN_H
/** 
 *  @defgroup biba_main main Library
 *  @code #include <main.h> @endcode
 * 
 *  @brief This turns on the motor and directly goes in the continuos loop for controlling it. 
 *
 *  This turns on the motor and directly goes in the continuos loop for controlling it. 
 *
 *  @note this is whear all the magic happens
 *  @author Bilyana Borisova bibishte@gmail.com
 */

/**@{*/

void Init_Input_Output( void );
void test( void );



#endif	/* MAIN_H */

