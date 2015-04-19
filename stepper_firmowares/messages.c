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
	putstring_P( PSTR( "8 microsteps\r" ) );
}

void M16Step( void )
{
	putstring_P( PSTR( "16 microsteps\r" ) );
}

void M32Step( void )
{
	putstring_P( PSTR( "32 microsteps\r" ) );
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


