/*************************************************************************
 Title:    Interrupt USART library with receive/transmit circular buffers
 Author:   Ognian Petrov <ovp@epiq.com>. Original from Peter Fleury <pfleury@gmx.ch>   http://jump.to/fleury
 Software: AVR-GCC 4.5, AVR Libc 1.7.0 or higher
 Hardware: any AVR with built-in USART,
 License:  GNU General Public License

 DESCRIPTION:
 An interrupt is generated when the UART has finished transmitting or
 receiving a byte. The interrupt handling routines use circular buffers
 for buffering received and transmitted data.

 The UART_RX_BUFFER_SIZE and UART_TX_BUFFER_SIZE variables define
 the buffer size in bytes. Note that these variables must be a
 power of 2.

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 *************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <stdio.h>
#include "config.h"
#include "usart.h"

#define BSIZE 32
#define B1SIZE 32

typedef struct ring_buffer
{
	uint8_t *data;
	uint8_t size;
	uint8_t head;
	uint8_t tail;
	uint8_t fillcount; // Number of unread characters in buffer
	uint8_t nlines; //Number of EOL character received
} ring_buffer_t;

/*
 *  Module global variables
 */
static uint8_t brx[BSIZE];
static uint8_t btx[BSIZE];
volatile ring_buffer_t usart_tx;
volatile ring_buffer_t usart_rx;

#if defined( ATMEGA_USART1 )
static uint8_t brx1[B1SIZE];
static uint8_t btx1[B1SIZE];
volatile ring_buffer_t usart1_tx;
volatile ring_buffer_t usart1_rx;
#endif

/**
 * Initialize USART and set baudrate
 * Baudrate using macro USART_BAUD_SELECT()
 * @param baudrate
 */
void usart_init(unsigned int baudrate)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		usart_rx.data = brx;
		usart_rx.size = BSIZE;
		usart_rx.head = 0;
		usart_rx.tail = 0;
		usart_rx.fillcount = 0;
		usart_rx.nlines = 0;

		usart_tx.data = btx;
		usart_tx.size = BSIZE;
		usart_tx.head = 0;
		usart_tx.tail = 0;
		usart_tx.fillcount = 0;
		usart_tx.nlines = 0;

		/* Set baud rate */
		if (baudrate & 0x8000)
		{
			UCSR0A = (1 << U2X0); //Enable 2x speed
			baudrate &= ~0x8000;
		}
		UBRR0H = (unsigned char) (baudrate >> 8);
		UBRR0L = (unsigned char) baudrate;

#if (USART0_TYPE) == 485

#if  defined(__AVR_ATmega8__)  || defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) \
  || defined(__AVR_ATmega323__)
		PORTD |= (1 << PD0); //pull-up RX0
#elif defined(__AVR_ATmega162__)
		PORTB |= (1 << PB2); //pull-up RX1
#else
#error "RX port is not internally pulled up"
#endif

		//Receiver output enable
		transmitter0_disable();
		set_as_output(DDRB, transceiver0);

		//Enable USART0 receiver and transmitter, receive complete
		//and transmit complete interrupt.
		UCSR0B = (1 << RXCIE0) | (1 << TXCIE0) | (1 << RXEN0) | (1 << TXEN0);
#else
		/* Enable USART receiver and transmitter and receive complete interrupt */
		UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
#endif
		/* Set frame format: asynchronous, 8data, no parity, 1stop bit */
		UCSR0C = (1 << URSEL0) | (1 << UCSZ01) | (1 << UCSZ00);
	}
}/* usart_init */

/**
 * Read byte from ring buffer
 * @param FILE stream
 * @return byte from ring buffer
 */
int usart_getchar(FILE *stream)
{
	uint8_t c;

	if (usart_rx.fillcount == 0)
		return EOF;

	if (usart_rx.nlines == 0)
		return EOF;

	c = usart_rx.data[usart_rx.tail];
	usart_rx.tail = (usart_rx.tail + 1) & (usart_rx.size - 1);

	//Disable RXC interrupt to run next lines atomic
	UCSR0B &= ~(1 << RXCIE0);
	usart_rx.fillcount--;

	//Behavior similar to Unix stty ICRNL
	if (c == VEOL)
	{
		c = '\n';
		usart_rx.nlines--;
	}

	//Enable RXC interrupt
	UCSR0B |= (1 << RXCIE0);

	return c;
}/* usart_getchar */

/**
 * Write byte to ring buffer for transmitting via USART
 * @param c byte to be transmitted
 * @param stream
 * @return
 */
int usart_putchar(char c, FILE *stream)
{
#if (USART0_TYPE) == 485
	//Driver output enable
	transmitter0_enable();
#endif

	if (c == '\n')
		c = VEOL;

	while (usart_tx.fillcount == usart_tx.size)
		; //wait for free space in buffer

	usart_tx.data[usart_tx.head] = c;
	usart_tx.head = (usart_tx.head + 1) & (usart_tx.size - 1);

	//Disable UDRE interrupt to run next lines atomic
	UCSR0B &= ~(1 << UDRIE0);

	usart_tx.fillcount++;

	//Enable UDRE interrupt
	UCSR0B |= (1 << UDRIE0);

	return 0;
}/* usart_putchar */

/**
 * USART0 Receive Complete interrupt
 */

ISR(USART0_RXC_vect)
{
	uint8_t data;

	data = UDR0;

	if (usart_rx.fillcount < usart_rx.size)
	{
		if (data == VEOL)
			usart_rx.nlines++;

		usart_rx.data[usart_rx.head] = data;

		usart_rx.head = (usart_rx.head + 1) & (usart_rx.size - 1);
		usart_rx.fillcount++;
	}
	else
	{
		//The Buffer is full! clear it
		usart_rx.head = 0;
		usart_rx.tail = 0;
		usart_rx.fillcount = 0;
		usart_rx.nlines = 0;

		/* Wait for empty transmit buffer */
		while (!(UCSR0A & (1 << UDRE0)))
			;
		UDR0 = 'B';
		while (!(UCSR0A & (1 << UDRE0)))
			;
		UDR0 = 'O';
		while (!(UCSR0A & (1 << UDRE0)))
			;
		UDR0 = 'F';
		while (!(UCSR0A & (1 << UDRE0)))
			;
		UDR0 = VEOL;
		//TODO Software flow control XON/XOFF. http://www-user.tu-chemnitz.de/~heha/hs_freeware/terminal/terminal.htm
		//SBUF = 0x13; //Pause transmission. Send XOFF character
	}
}

/**
 * USART0 Data Register Empty interrupt
 * Called when the USART is ready to transmit the next byte
 */

ISR(USART0_UDRE_vect)
{
	if (usart_tx.fillcount > 0)
	{
		UDR0 = usart_tx.data[usart_tx.tail];

		usart_tx.tail = (usart_tx.tail + 1) & (usart_tx.size - 1);
		usart_tx.fillcount--;
	}
	else
	{
		/* tx buffer empty, disable UDRE interrupt */
		UCSR0B &= ~(1 << UDRIE0);
	}
}

/**
 * USART0 Transmit complete interrupt
 * The Transmit Complete (TXC) Flag bit is set one when the entire frame
 * in the Transmit Shift Register has been shifted out and there are no new data
 * currently present in the transmit buffer. The TXC Flag bit is automatically cleared
 * when a transmit complete interrupt is executed.
 */
#if (USART0_TYPE) == 485
ISR(USART0_TXC_vect, ISR_NAKED)
{
	//Disable transmitter and Enable receiver
	transmitter0_disable();
	reti();
}
#endif

/* ========================================================================
 * These functions are only for ATmegas with two USART
 * =======================================================================*/
#if defined( ATMEGA_USART1 )

/**
 * Initialize USART1 and set baudrate
 * Baudrate using macro USART_BAUD_SELECT()
 * @param baudrate
 */
void usart1_init(unsigned int baudrate)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		usart1_rx.data = brx1;
		usart1_rx.size = B1SIZE;
		usart1_rx.head = 0;
		usart1_rx.tail = 0;
		usart1_rx.fillcount = 0;
		usart1_rx.nlines = 0;

		usart1_tx.data = btx1;
		usart1_tx.size = B1SIZE;
		usart1_tx.head = 0;
		usart1_tx.tail = 0;
		usart1_tx.fillcount = 0;
		usart1_tx.nlines = 0;

		/* Set baud rate */
		if (baudrate & 0x8000)
		{
			UCSR1A = (1 << U2X1); //Enable 2x speed
			baudrate &= ~0x8000;
		}
		UBRR1H = (unsigned char) (baudrate >> 8);
		UBRR1L = (unsigned char) baudrate;

#if (USART1_TYPE) == 485

#if defined(__AVR_ATmega162__)
		PORTB |= (1 << PB2); //pull-up RX1
#else
#error "RX port is not internally pulled up"
#endif

		//Receiver output enable
		transmitter1_disable();
		set_as_output(DDRE, transceiver1);

		//Enable USART1 receiver and transmitter, receive complete
		//and transmit complete interrupt.
		UCSR1B = (1 << RXCIE1) | (1 << TXCIE1) | (1 << RXEN1) | (1 << TXEN1);
#else
		//Enable USART1 receiver and transmitter and receive complete interrupt
		UCSR1B = (1 << RXCIE1) | (1 << RXEN1) | (1 << TXEN1);
#endif
		/* Set frame format: asynchronous, 8data, no parity, 1stop bit */
		UCSR1C = (1 << URSEL1) | (1 << UCSZ11) | (1 << UCSZ10);
	}
}/* usart1_init */

/**
 * Read byte from ring buffer
 * @param FILE stream
 * @return byte from ring buffer
 */
int usart1_getchar(FILE *stream)
{
	uint8_t c;

	if (usart1_rx.fillcount == 0)
	return EOF;

	if (usart1_rx.nlines == 0)
	return EOF;

	c = usart1_rx.data[usart1_rx.tail];
	usart1_rx.tail = (usart1_rx.tail + 1) & (usart1_rx.size - 1);

	//Disable RXC interrupt to run next lines atomic
	UCSR1B &= ~(1 << RXCIE1);
	usart1_rx.fillcount--;

	//Behavior similar to Unix stty ICRNL
	if (c == VEOL)
	{
		c = '\n';
		usart1_rx.nlines--;
	}

	/* Enable RXCIE interrupt */
	UCSR1B |= (1 << RXCIE1);

	return c;
}/* usart_getchar */

/**
 * Write byte to ring buffer for transmitting via USART
 * @param c byte to be transmitted
 * @param stream
 * @return
 */
int usart1_putchar(char c, FILE *stream)
{
#if (USART1_TYPE) == 485
	//Driver output enable
	transmitter1_enable();
#endif

	if (c == '\n')
	c = VEOL;

	while (usart1_tx.fillcount == usart1_tx.size)
	; //wait for free space in buffer

	usart1_tx.data[usart1_tx.head] = c;
	usart1_tx.head = (usart1_tx.head + 1) & (usart1_tx.size - 1);

	//Disable UDRE interrupt to run next lines atomic
	UCSR1B &= ~(1 << UDRIE1);

	usart1_tx.fillcount++;

	//Enable UDRE interrupt
	UCSR1B |= (1 << UDRIE1);

	return 0;
}/* usart_putchar */

/**
 * USART1 Receive Complete interrupt
 */

ISR(USART1_RXC_vect)
{
	uint8_t data;

	data = UDR1;

	if (usart1_rx.fillcount < usart1_rx.size)
	{
		if (data == VEOL)
		usart1_rx.nlines++;

		usart1_rx.data[usart1_rx.head] = data;

		usart1_rx.head = (usart1_rx.head + 1) & (usart1_rx.size - 1);
		usart1_rx.fillcount++;
	}
	else
	{
		//The Buffer is full! clear it
		usart1_rx.head = 0;
		usart1_rx.tail = 0;
		usart1_rx.fillcount = 0;
		usart1_rx.nlines = 0;

		/* Wait for empty transmit buffer */
		while (!(UCSR1A & (1 << UDRE1)))
		;
		UDR1 = 'B';
		while (!(UCSR1A & (1 << UDRE1)))
		;
		UDR1 = 'O';
		while (!(UCSR1A & (1 << UDRE1)))
		;
		UDR1 = 'F';
		while (!(UCSR1A & (1 << UDRE1)))
		;
		UDR1 = VEOL;
	}
}

/**
 *
 */
void usart1_clear_rx(void)
{
	usart1_rx.head = 0;
	usart1_rx.tail = 0;
	usart1_rx.fillcount = 0;
	usart1_rx.nlines = 0;
}

/**
 * USART1 Data Register Empty interrupt
 * Called when the USART is ready to transmit the next byte
 */

ISR(USART1_UDRE_vect)
{
	if (usart1_tx.fillcount > 0)
	{
		UDR1 = usart1_tx.data[usart1_tx.tail];

		usart1_tx.tail = (usart1_tx.tail + 1) & (usart1_tx.size - 1);
		usart1_tx.fillcount--;
	}
	else
	{
		/* tx buffer empty, disable UDRE interrupt */
		UCSR1B &= ~(1 << UDRIE1);
	}
}

/**
 * USART1 Transmit complete interrupt
 * The Transmit Complete (TXC) Flag bit is set one when the entire frame
 * in the Transmit Shift Register has been shifted out and there are no new data
 * currently present in the transmit buffer. The TXC Flag bit is automatically cleared
 * when a transmit complete interrupt is executed.
 */
#if (USART1_TYPE) == 485
ISR(USART1_TXC_vect, ISR_NAKED)
{
	//Disable transmitter and Enable receiver
	transmitter1_disable();
	reti();
}
#endif

#endif
