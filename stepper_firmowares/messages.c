/************************************************************************
 Title:    All the printed masseges
 Author:   Bilyana Borisova <bibishte@gmail.com>
 File:     $Id: messages.c.c 2015/04/19
 Software: AVR-GCC 4.9.2, AVR Libc 1.8.0
 Hardware: DRV8825 & ATmega8
 License:  GNU General Public License v2
 Usage:    see Doxygen manual

 DESCRIPTION:
 This modul contains functions that are printing messages

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License v2 as published by
 the Free Software Foundation; either version 2 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License v2 for more details.

 ************************************************************************/



#include "messages.h"
#include "utils.h"
#include <avr/pgmspace.h>
#include "drv_8825.h"





const char string_1[] PROGMEM = "SDec\r";
const char string_2[] PROGMEM = "FDec\r";
const char string_3[] PROGMEM = "ADec\r";
const char string_4[] PROGMEM = "full step\r";
const char string_5[] PROGMEM = "half step\r";
const char string_6[] PROGMEM = "quater step\r";
const char string_7[] PROGMEM = "8 microsteps\r";
const char string_8[] PROGMEM = "16 microsteps\r";
const char string_9[] PROGMEM = "32 microsteps\r";
const char string_10[] PROGMEM = "Acc=";
const char string_11[] PROGMEM = "MinSPD=";
const char string_12[] PROGMEM = "ERROR\r";
const char string_13[] PROGMEM = "MaxSPD=";
const char string_14[] PROGMEM = "SPR=";
const char string_15[] PROGMEM = "RMinSpd=";
const char string_16[] PROGMEM = "RMaxSPD=";
const char string_17[] PROGMEM = "RAcc=";
const char string_18[] PROGMEM = "Stop\r";
const char string_19[] PROGMEM = "Enable\r";
const char string_20[] PROGMEM = "Disable\r";
const char string_21[] PROGMEM = "NOT SET MODE\r";
const char string_22[] PROGMEM = "NOT SET DECAY\r";
const char string_23[] PROGMEM = "Done ";
const char string_24[] PROGMEM = " steps\r";

PGM_P const messages_table[] PROGMEM =
{
string_1,
string_2,
string_3,
string_4,
string_5,
string_6,
string_7,
string_8,
string_9,
string_10,
string_11,
string_12,
string_13,
string_14,
string_15,
string_16,
string_17,
string_18,
string_19,
string_20,
string_21,
string_22,
string_23,
string_24
};


/*



void SDec( void )
{
	putstring_P( PSTR( "SDec\r" ) );
}

void FDec( void )
{
	putstring_P( PSTR( "FDec\r" ) );
}

void ADec( void )
{
	putstring_P( PSTR( "ADec\r" ) );
}

void Fstep( void )
{
	putstring_P( PSTR( "full step\r" ) );
}

void HStep( void )
{
	putstring_P( PSTR( "half step\r" ) );
}

void Qstep( void )
{
	putstring_P( PSTR( "quater step\r" ) );
}

void M8Step( void )
{
	putstring_P( PSTR( "8 " ) );
	Microstep();
}

void M16Step( void )
{
	putstring_P( PSTR( "16 " ) );
	Microstep();
}

void M32Step( void )
{
	putstring_P( PSTR( "32 " ) );
	Microstep();
}

void Acc( void )
{
	putstring_P( PSTR( "Acc=" ) );
	putstring( int_to_string( ( uint64_t ) ( GetAcceleration( ) ) ) );
	putstring_P( PSTR( "\r" ) );
}

void Minspeed( void )
{
	putstring_P( PSTR( "MinSPD=" ) );
	putstring( int_to_string( ( uint64_t ) ( GetMinSpeed( ) ) ) );
	putstring_P( PSTR( "\r" ) );
}

void Errormssg( void )
{
	putstring_P( PSTR( "ERROR\r" ) );
}

void Maxspeed( void )
{
	putstring_P( PSTR( "MaxSPD=" ) );
	putstring( int_to_string( ( uint64_t ) ( GetMaxSpeed( ) ) ) );
	putstring_P( PSTR( "\r" ) );
}

void SptRevol( void )
{
	putstring_P( PSTR( "SPR=" ) );
	putstring( int_to_string( ( uint16_t ) ( Get_Steps_revol( ) ) ) );
	putstring_P( PSTR( "\r" ) );
}

void RMinSpd( void )
{
	putstring_P( PSTR( "RMinSpd=" ) );
	putstring( int_to_string( ( uint16_t ) ( GetRealMinSpeed( ) ) ) );
	putstring_P( PSTR( "\r" ) );
}

void RMaxSpd( void )
{
	putstring_P( PSTR( "RMaxSPD=" ) );
	putstring( int_to_string( ( uint64_t ) ( GetRealMaxSpeed( ) ) ) );
	putstring_P( PSTR( "\r" ) );
}

void RAcc( void )
{
	putstring_P( PSTR( "RAcc=" ) );
	putstring( int_to_string( ( uint16_t ) ( GetRealAcc( ) ) ) );
	putstring_P( PSTR( "\r" ) );
}

void Stop(void)
{
	putstring_P( PSTR( "Stop" ) );
}

void Microstep(void)
{
	putstring_P( PSTR( "microsteps\r" ) );
}
*/



void Message(uint8_t a)
{
	putstring_P((PGM_P)pgm_read_word(&(messages_table[a])));
}

void Message_xcall(uint8_t a, uint16_t (*func)(void))
{
	putstring_P((PGM_P)pgm_read_word(&(messages_table[a])));
	putstring( int_to_string( ( uint64_t ) (func()) )) ;
	putstring_P( PSTR( "\r" ) );
}