
/************************************************************************
 Title:    Interrupt UART library with receive/transmit circular buffers
 Author:   Peter Fleury <pfleury@gmx.ch>   http://jump.to/fleury
 File:     $Id: uart.h,v 1.8.2.1 2007/07/01 11:14:38 peter Exp $
 Software: AVR-GCC 4.5, AVR Libc 1.7.0
 Hardware: any AVR with built-in UART, tested on AT90S8515 & ATmega8 at 4 Mhz
 License:  GNU General Public License
 Usage:    see Doxygen manual

 LICENSE:
 Copyright (C) 2006 Peter Fleury

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 ************************************************************************/


#ifndef USART_H_
#define USART_H_
/** 
 *  @defgroup pfleury_uart UART Library
 *  @code #include <usart.h> @endcode
 * 
 *  @brief Interrupt UART library using the built-in UART with transmit and receive circular buffers. 
 *
 *  This library can be used to transmit and receive data through the built in UART. 
 *
 *  An interrupt is generated when the UART has finished transmitting or
 *  receiving a byte. The interrupt handling routines use circular buffers
 *  for buffering received and transmitted data.
 *
 *  The UART_RX_BUFFER_SIZE and UART_TX_BUFFER_SIZE constants define
 *  the size of the circular buffers in bytes. Note that these constants must be a power of 2.
 *  You may need to adapt this constants to your target and your application by adding 
 *  CDEFS += -DUART_RX_BUFFER_SIZE=nn -DUART_RX_BUFFER_SIZE=nn to your Makefile.
 *
 *  @note Based on Atmel Application Note AVR306
 *  @author Peter Fleury pfleury@gmx.ch  http://jump.to/fleury
 */

/**@{*/


#if (__GNUC__ * 100 + __GNUC_MINOR__) < 405
#error "This library requires AVR-GCC 4.5 or later, update to newer AVR-GCC compiler !"
#endif

#if  defined(__AVR_ATmega8__)  || defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) \
  || defined(__AVR_ATmega323__)
#	include "usartm8.h"
#elif defined(__AVR_ATmega162__)
#	include "usartm162.h"
#else
#error "No USART definition for MCU available"
#endif

/*
 ** constants and macros
 */

//RS485 Driver definition
#if (USART0_TYPE) == 485
#define transceiver0	PB0
#define transmitter0_disable()	PORTB &= ~(1 << transceiver0)
#define transmitter0_enable()	PORTB |= (1 << transceiver0)
#endif

#if defined( ATMEGA_USART1 ) && (USART1_TYPE) == 485
#define transceiver1	PE0
#define transmitter1_disable()	PORTE &= ~(1 << transceiver1)
#define transmitter1_enable()	PORTE |= (1 << transceiver1)
#endif

/** @brief  USART Baudrate Expression
 *  @param  xtalcpu  system clock in Mhz, e.g. 4000000L for 4Mhz          
 *  @param  baudrate baudrate in bps, e.g. 1200, 2400, 9600     
 */
#define USART_BAUD_SELECT(baudRate,xtalCpu) ((xtalCpu)/((baudRate)*16l)-1)

/** @brief  USART Baudrate Expression for ATmega double speed mode
 *  @param  xtalcpu  system clock in Mhz, e.g. 4000000L for 4Mhz           
 *  @param  baudrate baudrate in bps, e.g. 1200, 2400, 9600     
 */
#define USART_BAUD_SELECT_DOUBLE_SPEED(baudRate,xtalCpu) (((xtalCpu)/((baudRate)*8l)-1)|0x8000)

/**
 * End of line character.
 */
#define VEOL (char) 0x0d // CR -> '/r'
/*
 ** function prototypes
 */

/**
 @brief   Initialize USART and set baudrate
 @param   baudrate Specify baudrate using macro UART_BAUD_SELECT()
 @return  none
 */
void usart_init(unsigned int);

/**
 *  @brief   Get received byte from ringbuffer
 *
 * Returns in the lower byte the received character and in the 
 * higher byte the last receive error.
 * UART_NO_DATA is returned when no data is available.
 *
 *  @param   void
 *  @return  lower byte:  received byte from ringbuffer
 *  @return  higher byte: last receive status
 *           - \b 0 successfully received data from UART
 *           - \b UART_NO_DATA           
 *             <br>no receive data available
 *           - \b UART_BUFFER_OVERFLOW   
 *             <br>Receive ringbuffer overflow.
 *             We are not reading the receive buffer fast enough, 
 *             one or more received character have been dropped 
 *           - \b UART_OVERRUN_ERROR     
 *             <br>Overrun condition by UART.
 *             A character already present in the UART UDR register was 
 *             not read by the interrupt handler before the next character arrived,
 *             one or more received characters have been dropped.
 *           - \b UART_FRAME_ERROR       
 *             <br>Framing Error by UART
 */
int usart_getchar(FILE *stream);

/**
 *  @brief   Put byte to ringbuffer for transmitting via UART
 *  @param   data byte to be transmitted
 *  @return  none
 */
int usart_putchar(char c, FILE *stream);

/**
 *  @brief   Put string to ringbuffer for transmitting via UART
 *
 *  The string is buffered by the uart library in a circular buffer
 *  and one character at a time is transmitted to the UART using interrupts.
 *  Blocks if it can not write the whole string into the circular buffer.
 * 
 *  @param   s string to be transmitted
 *  @return  none
 */
//void usart_puts(char const *);

#if defined( ATMEGA_USART1 )
/** @brief  Initialize USART1 (only available on selected ATmegas) @see uart_init */
void usart1_init(unsigned int);
/** @brief  Get received byte of USART1 from ringbuffer. (only available on selected ATmega) @see uart_getc */
int usart1_getchar(FILE *stream);
/** @brief  Put byte to ringbuffer for transmitting via USART1 (only available on selected ATmega) @see uart_putc */
int usart1_putchar(char c, FILE *stream);
/** @brief  */
void usart1_clear_rx(void);
/** @brief  Put string to ringbuffer for transmitting via USART1 (only available on selected ATmega) @see uart_puts */
//void usart1_puts(const char *);
#endif

#endif // USART_H_
