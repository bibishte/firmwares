/*
 * usatm8.h
 *
 *  Created on: 08.02.2011
 *      Author: ovpetrov
 */

#ifndef USATM8_H_
#define USATM8_H_

/* This file should only be included from <avr/io.h>, never directly. */

#ifndef USART_H_
#  error "Include <com/usart.h> instead of this file."
#endif

#ifndef USARTXXX_H_
#  define USARTXXX_H_ "usartm8.h"
#else
#  error "Attempt to include more than one <com/usartXXX.h> file."
#endif

//Atmega8 is only with one USART engine
#undef ATMEGA_USART1

/* USART I/O registers*/
#define UBRR0L	UBRRL
#define UCSR0C	UCSRC
#define UCSR0B	UCSRB
#define UCSR0A	UCSRA
#define UDR0	UDR
#define UBRR0H	UBRRH
#define UBRR0L	UBRRL

#define USART0_RXC_vect		USART_RXC_vect
#define USART0_TXC_vect		USART_TXC_vect
#define USART0_UDRE_vect	USART_UDRE_vect

/* UBRRH bit definitions */
#define URSEL0	URSEL

/* UCSRA bit definitions */
#define RXC0	RXC
#define TXC0	TXC
#define UDRE0	UDRE
#define FE0		FE
#define DOR0	DOR
#define UPE0	PE
#define U2X0	U2X
#define MPCM0	MPCM

/* UCSRB bit definitions */
#define RXCIE0	RXCIE
#define TXCIE0	TXCIE
#define UDRIE0	UDRIE
#define RXEN0	RXEN
#define	TXEN0	TXEN
#define UCSZ02 	UCSZ2
#define RXB80	RXB8
#define TXB80	TXB8

/* UCSRC bit definitions */
#define URSEL0	URSEL
#define UMSEL0	UMSEL
#define UPM01	UPM1
#define UPM00	UPM0
#define USBS0	USBS
#define UCSZ01	UCSZ1
#define UCSZ00	UCSZ0
#define UCPOL0	UCPOL

#endif /* USATM8_H_ */
