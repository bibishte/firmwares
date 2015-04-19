/************************************************************************
 Title:    Utils function for converting ASCI to uint64_t
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

#ifndef UTILS_H
#define	UTILS_H
/** 
 *  @defgroup biba_utils utils Library
 *  @code #include <utils.h> @endcode
 * 
 *  @brief This module contains functions that are converting ASCI to uint64_t 
 *
 * This module contains functions that are converting ASCI to uint64_t 
 * 
 *  @note Typical functions that are saving me memory.
 *  @author Bilyana Borisova bibishte@gmail.com
 */

/**@{*/


#include <stdint.h>
#include <stdio.h>
#include "usart.h"


uint64_t ConvertASCItouint64(char *in);
char *int_to_string(uint64_t i);
void putstring(const char *putc);

void putstring_P(const char *putc);

#endif	/* UTILS_H */

