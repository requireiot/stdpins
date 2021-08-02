/**
 * @file		  stdpins.h
 * Created		: 08.02.2017 13:25:00
 * Author		: Bernd Waldmann
 * Tabsize		: 4
 *
 * This Revision: $Id: stdpins.h 1215 2021-08-02 15:57:00Z  $
 */

/*
   Copyright (C) 2017,2021 Bernd Waldmann

   This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. 
   If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/

   SPDX-License-Identifier: MPL-2.0
*/

#ifndef __STDPINS_H
#define __STDPINS_H
/** 
	@defgroup stdpins  <stdpins.h>: AVR port pins handling.
	@{
		
	@brief Macros to define AVR port pins as input or output, and set/clear (polarity aware).

    inspired by http://www.starlino.com/port_macro.html
	
	Define a pin and its usage like so (LED connected to PB5, light if PB5=Hi)
		@code	#define LED B,5,ACTIVE_HIGH		@endcode
	Now you can define the pin as output
		@code	AS_OUTPUT(LED);					@endcode
	and turn the LED on for a while
		@code	ASSERT(LED); _delay(100); NEGATE(LED); @endcode
	If the schematics change and LED now turns on if PB5=Lo, just change the definition to
		@code	#define LED B,5,ACTIVE_LOW		@endcode
	Everything else can remain the same.
	
	To set the pin to Low, independent of polarity, use
		@code SET_LOW(LED); @endcode
		
	@}
 */

#if defined(__AVR_ATmega48__) || defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48P__) || \
defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega48PB__) || \
defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88P__) || \
defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega88PB__) || \
defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__)|| defined(__AVR_ATmega168P__) || \
defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega168PB__) || \
defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__)

    #define IS_Mxx8

#elif defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164P__) || defined(__AVR_ATmega164PA__) || \
defined(__AVR_ATmega324A__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324PA__) || \
defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__) || \
defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)

    #define IS_Mxx4

#endif

// ===========================================================================
// public constants

//! @name Polarity
//!@{
#define ACTIVE_HIGH	1
#define ACTIVE_LOW  0
#define ACTIVE_LOW_OC -1
//!@}

// ===========================================================================
// use pin toggle function available in ATmega

#if defined(IS_Mxx8) || defined(IS_Mxx4)
	#define _ppp_TOGGLE(type,name,bit,pol)	PIN ## name  = _BV(bit)		// invert bit
#else
	#define _ppp_TOGGLE(type,name,bit,pol)		type ## name  ^= _BV(bit)		// invert bit
#endif // defined__AVR_xxx

// ===========================================================================
// internal macros - these macros are used indirectly by other macros,
// mainly for string concatenation
// pin specified as name,bit,polarity

// ----- set, reset etc -- not polarity-aware

// convert bit number 0..7 to bit mask 0x01..0x80
#define _ppp_BV(name,bit,pol)               _BV(bit)
// return port & _BV(bit)
#define _ppp_READ(type,name,bit,pol)		((type ## name) & (1 << bit))
// set bit to H
#define _ppp_SET(type,name,bit,pol)			type ## name |= _BV(bit)		
// set bit to L
#define _ppp_CLR(type,name,bit,pol)			type ## name  &= ~ _BV(bit)		
// set bit to 1 if <value>!=0 else set to 0
#define _ppp_PUT(type,name,bit,pol,value)	type ## name = ( type ## name & ~_BV(bit) ) | ( (value) ? _BV(bit) : 0 )
// set multiple bits to <value>
#define _ppp_PUT_MULT(type,name,bit1,pol,nbits,value)   type ## name = (((type ## name) & ~(((1 << (nbits))-1) << bit1)) | ((value) << bit1))

// return 1 if bit is H else 0
#define _ppp_ISHIGH(type,name,bit,pol)		((type ## name >> bit) &  1)

// ----- set, reset etc -- polarity-aware

#define _ppp_ISACTIVEHIGH(name,bit,pol)	    (pol==ACTIVE_HIGH)
#define _ppp_IS_OC(name,bit,pol)	        (pol==ACTIVE_LOW_OC)

// set bit to true (LOW, open collector)
#define _ppp_ASSERT_OC(name,bit,pol)        \
    { _ppp_CLR(PORT,name,bit,pol); _ppp_SET(DDR,name,bit,pol); }

// set bit to false (OFF, open collector)
#define _ppp_NEGATE_OC(name,bit,pol)        \
    { _ppp_CLR(PORT,name,bit,pol); _ppp_CLR(DDR,name,bit,pol); }

// set bit to TRUE (polarity-aware, push-pull output)
#define _ppp_ASSERT_PP(name,bit,pol)		\
    if (pol==ACTIVE_HIGH)                   \
        { _ppp_SET(PORT,name,bit,pol); }    \
    else                                    \
        { _ppp_CLR(PORT,name,bit,pol); }

// set bit to FALSE (polarity-aware, push-pull output)
#define _ppp_NEGATE_PP(name,bit,pol)		\
    if (pol==ACTIVE_HIGH)                   \
        { _ppp_CLR(PORT,name,bit,pol); }    \
    else                                    \
        { _ppp_SET(PORT,name,bit,pol); }

// set bit to <value> (polarity-aware)
#define _ppp_PUT_PA(type,name,bit,pol,val)   _ppp_PUT(type,name,bit,pol,((pol==ACTIVE_HIGH) ? (val) : !(val)))

// return 1 if bit is H else 0
#define _ppp_ISTRUE(type,name,bit,pol)		( (pol==ACTIVE_HIGH) ? _ppp_ISHIGH(type,name,bit,pol) : !_ppp_ISHIGH(type,name,bit,pol) )

#define _ppp_ASSERT(name,bit,pol)           \
    if (pol==ACTIVE_LOW_OC)                 \
        { _ppp_ASSERT_OC(name,bit,pol); }   \
    else                                    \
        { _ppp_ASSERT_PP(name,bit,pol); }

#define _ppp_NEGATE(name,bit,pol)           \
    if (pol==ACTIVE_LOW_OC)                 \
        { _ppp_NEGATE_OC(name,bit,pol); }   \
    else                                    \
        { _ppp_NEGATE_PP(name,bit,pol); }

// ----- get port, DDR, bit
#define _ppp_NAME(type,name,bit,pol)		(type ## name)
#define _ppp_BIT(name,bit,pol)				(bit)
#define _ppp_LETTER(name,bit,pol)			name

// ----- aliases for pin change interrupt registers
#if defined(IS_Mxx4)    // ATmega 164, 324, 644, 1284
    #define _PCMSKA	PCMSK0
    #define _PCMSKB	PCMSK1
    #define _PCMSKC	PCMSK2
    #define _PCMSKD	PCMSK3

    #define _PCIEA	PCIE0
    #define _PCIEB	PCIE1
    #define _PCIEC	PCIE2
    #define _PCIED	PCIE3

    #define _PCINTA_vect PCINT0_vect
    #define _PCINTB_vect PCINT1_vect
    #define _PCINTC_vect PCINT2_vect
    #define _PCINTD_vect PCINT3_vect

#elif defined(IS_Mxx8)  // ATmega 48,88,168,328
    #define _PCMSKB	PCMSK0
    #define _PCMSKC	PCMSK1
    #define _PCMSKD	PCMSK2

    #define _PCIEB	PCIE0
    #define _PCIEC	PCIE1
    #define _PCIED	PCIE2

    #define _PCINTB_vect PCINT0_vect
    #define _PCINTC_vect PCINT1_vect
    #define _PCINTD_vect PCINT2_vect

#else   // ATtiny 25,45,85, 2313
    #define _PCMSKB	PCMSK0

    #define _PCIEB	PCIE0

    #define _PCINTB_vect PCINT0_vect
#endif


#define _ppp_ISR(name,bit,pol)			    ISR( _PCINT ## name ## _vect )
#define _ppp_PCIEx(name,bit,pol)	        (_PCIE ## name)
#define _ppp_PCIEx_ENABLE(name,bit,pol)	    PCICR |= _BV( _PCIE ## name )
#define _ppp_PCIEx_DISABLE(name,bit,pol)	PCICR &= ~_BV( _PCIE ## name )
#define _ppp_PCIFx_CLEAR(name,bit,pol)	    PCIFR &= ~_BV( _PCIE ## name )
#define _ppp_PCIFx_TEST(name,bit,pol)	    (PCIFR & _BV( _PCIE ## name ))

#define _PINDEF3(a,b,c)	a,b,c

// ----- aliases for Arduino pin groups

#if defined(IS_Mxx4)    // ATmega 164, 324, 644, 1284
    // MightyCore standard layout
    #define _ARDUINO_PIN_BASE_A     24
    #define _ARDUINO_PIN_BASE_B     0
    #define _ARDUINO_PIN_BASE_C     16
    #define _ARDUINO_PIN_BASE_D     8
#elif defined(IS_Mxx8)  // ATmega 48,88,168,328
    #define _ARDUINO_PIN_BASE_B     8
    #define _ARDUINO_PIN_BASE_C     14
    #define _ARDUINO_PIN_BASE_D     0
#endif

#define _ppp_ARDUINO_PIN_BASE(name)         (_ARDUINO_PIN_BASE_ ## name)
#define _ppp_ARDUINO_PIN(name,bit,pol)      ( bit + _ppp_ARDUINO_PIN_BASE(name) )

/** @{ */

// ===========================================================================
// public macros - these macros are used by end user
// <pin> is a combined port,bit,polarity value

//! @name Data direction
//!@{
/** set as input, pull-up undefined */
#define AS_INPUT(pin)       _ppp_CLR(DDR,pin)
/** set as input, with pull-up */
#define AS_INPUT_PU(pin)    _ppp_CLR(DDR,pin); _ppp_SET(PORT,pin)
/** set as output, without pull-up */
#define AS_INPUT_FLOAT(pin) _ppp_CLR(DDR,pin); _ppp_CLR(PORT,pin)
/** set as output */
#define AS_OUTPUT(pin)      _ppp_SET(DDR,pin)
/** Enable pull-up resistor */
#define PULLUP_ENABLE(pin)  _ppp_SET(PORT,pin)
/** Disable pull-up resistor */
#define PULLUP_DISABLE(pin) _ppp_CLR(PORT,pin)
//!@}

//! @name Get/set, ignore polarity 
//!@{

/** return TRUE if polarity is ACTIVE_HIGH */
#define IS_ACTIVE_HIGH(pin)	_ppp_ISACTIVEHIGH(pin)
/** return TRUE if polarity is ACTIVE_LOW_OC */
#define IS_OC(pin)	_ppp_IS_OC(pin)

/** read port, mask all but relevant bit */
#define READ(pin)           _ppp_READ(PIN,pin)
/** flip output */
#define TOGGLE(pin)         _ppp_TOGGLE(PORT,pin)
/** set output Hi (independent of polarity) */
#define SET_HIGH(pin)       _ppp_SET(PORT,pin)
/** set output Lo (independent of polarity) */
#define SET_LOW(pin)        _ppp_CLR(PORT,pin)
/** set output to (value), not polarity aware */
#define SET(pin,value)		_ppp_PUT(PORT,pin,value)
/** set outputs to (value), not polarity aware */
#define SET_MULT(pin,nbits,value)   _ppp_PUT_MULT(PORT,pin,nbits,value)
/** return 1 if pin is Hi else 0 */
#define IS_HIGH(pin)        _ppp_ISHIGH(PIN,pin)
/** return 0 if pin is Hi else 1 */
#define IS_LOW(pin)         !_ppp_ISHIGH(PIN,pin)
//!@}

//! @name Get/set, polarity aware 
//!@{

/** set "open collector" output  to active(low) */
#define ASSERT_OC(pin)      _ppp_CLR(PORT,pin); _ppp_SET(DDR,pin)
/** set "open collector" output to inactive (high) */
#define NEGATE_OC(pin)      _ppp_CLR(PORT,pin); _ppp_CLR(DDR,pin)
/** set output to TRUE (H or L depending on polarity) */

#define ASSERT(pin)         _ppp_ASSERT(pin)	
/** set output to TRUE (H or L depending on polarity) -- synonym for ASSERT */
#define SET_TRUE	ASSERT
/** set output to FALSE (H or L depending on polarity) */
#define NEGATE(pin)         _ppp_NEGATE(pin)	
/** set output to FALSE (H or L depending on polarity) -- synonym for NEGATE */
#define SET_FALSE	NEGATE
/** set output based on `value` (polarity aware) */
#define SET_PA(pin,value)	_ppp_PUT_PA(PORT,pin,value)
/** return 1 if pin is TRUE else 0 (polarity aware) */
#define IS_TRUE(pin)		_ppp_ISTRUE(PIN,pin)
/** return 1 if pin is FALSE else 0 (polarity aware) */
#define IS_FALSE(pin)		!_ppp_ISTRUE(PIN,pin)

//!@}

/** @name Classic names
 *  @brief Generate classic names like `DDRA`, `PORTA`, `PINA`, `A`, `5` 
 *  from `#define LED A,5,ACTIVE_HIGH`
 */
//!@{
/** return bit mask for pin */
#define BV(pin)             _ppp_BV(pin)
/** return output port name like PORTA */
#define PORT(pin)			_ppp_NAME(PORT,pin)
/** return DDR name like DDRA */
#define DDR(pin)			_ppp_NAME(DDR,pin)
/** return input port name like PINA */
#define PIN(pin)			_ppp_NAME(PIN,pin)
/** return bit number 0..7 */
#define portBIT(pin)		_ppp_BIT(pin)
/** return output port name like A */
#define portNAME(pin)		_ppp_LETTER(pin)

#define ARDUINO_PIN(pin)    _ppp_ARDUINO_PIN(pin)

//!@}

//! @name Pin change interrupt
//!@{

/** enable pin change interrupt for this pin (PCINTx bit [0..7] in PCMSK{0|1|2}) */
#define PCI_ENABLE(pin)		_ppp_SET(_PCMSK,pin)		
/** disable pin change interrupt for this pin (PCINTx bit [0..7] in PCMSK{0|1|2}) */
#define PCI_DISABLE(pin)	_ppp_CLR(_PCMSK,pin)
/** enable pin change interrupts group for this port (enable PCI group 0|1|2) */
#define PCIEx_ENABLE(pin)	_ppp_PCIEx_ENABLE(pin)
/** disable pin change interrupts for this port (enable PCI group 0|1|2) */
#define PCIEx_DISABLE(pin)	_ppp_PCIEx_DISABLE(pin)
/** reset interrupt flag for this pin group */
#define PCIFx_CLEAR(pin)	_ppp_PCIFx_CLEAR(pin)
/** return !=0 if interrupt flag for this pin group is set */
#define PCIFx_TEST(pin)	_ppp_PCIFx_CLEAR(pin)
/** return name of PCIE register for this pin (PCIE0, PCIE1 or PCIE2) */
#define PCIEx(pin)			_ppp_PCIEx(pin)
/** expands to `ISR(PCINTx_vect)` */
#define PCI_ISR(pin)		_ppp_ISR(pin)
//!@}

// ===========================================================================
// convenience definitions for alternative pin use

#if defined(IS_Mxx8) || defined(__AVR_ATmega8__) || defined(DOXYGEN)

/** @name Predefined pins with alternative functions.
    Conditional defines appropriate for different MCUs (ATtiny85, ATmega329, ...). 
    These can be used instead of explicit pin definitions, e.g. `SET_OUTPUT(_OC0A)`
 */
//!@{

	#define _OC0A(pol)	_PINDEF3(D,6,pol)
	#define _OC0B(pol)	_PINDEF3(D,5,pol)
	#define _OC1A(pol)	_PINDEF3(B,1,pol)
	#define _OC1B(pol)	_PINDEF3(B,2,pol)
	#define _OC2A(pol)	_PINDEF3(B,3,pol)
	#define _OC2B(pol)	_PINDEF3(D,3,pol)

	#define _UART_RX	D,0,ACTIVE_HIGH
	#define _UART_TX	D,1,ACTIVE_HIGH

	#define _I2C_SDA	C,4,ACTIVE_HIGH
	#define _I2C_SCL	C,5,ACTIVE_HIGH

	#define _INT0(pol)	_PINDEF3(D,2,pol)
	#define _INT1(pol)	_PINDEF3(D,3,pol)

    #define _SPI_SCK    B,5,ACTIVE_HIGH
    #define _SPI_MISO   B,4,ACTIVE_HIGH
    #define _SPI_MOSI   B,3,ACTIVE_HIGH
    #define _SPI_SS     B,2,ACTIVE_LOW
//!@}

#elif defined(__AVR_ATtiny2313__) || defined(__AVR_ATtiny2313A__) || defined(__AVR_ATtiny4313__)

	#define _OC0A(pol)	_PINDEF3(B,2,pol)
	#define _OC0B(pol)	_PINDEF3(D,5,pol)
	#define _OC1A(pol)	_PINDEF3(B,3,pol)
	#define _OC1B(pol)	_PINDEF3(B,4,pol)

	#define _UART_RX	D,0,ACTIVE_HIGH
	#define _UART_TX	D,1,ACTIVE_HIGH

	#define _I2C_SDA	B,5,ACTIVE_HIGH
	#define _I2C_SCL	B,7,ACTIVE_HIGH

	#define _INT0(pol)	_PINDEF3(D,2,pol)
	#define _INT1(pol)	_PINDEF3(D,3,pol)

#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)

	#define _OC0A(pol)	_PINDEF3(B,0,pol)
	#define _OC0B(pol)	_PINDEF3(B,1,pol)
	#define _OC1A(pol)	_PINDEF3(B,1,pol)
	#define _nOC1A(pol)	_PINDEF3(B,0,pol)
	#define _OC1B(pol)	_PINDEF3(B,4,pol)
	#define _nOC1B(pol)	_PINDEF3(B,3,pol)

	#define _I2C_SDA	B,0,ACTIVE_HIGH
	#define _I2C_SCL	B,2,ACTIVE_HIGH

#elif defined(IS_Mxx4)

	#define _OC0A(pol)	_PINDEF3(B,3,pol)
	#define _OC0B(pol)	_PINDEF3(B,4,pol)
	#define _OC1A(pol)	_PINDEF3(D,5,pol)
	#define _OC1B(pol)	_PINDEF3(D,4,pol)
	#define _OC2A(pol)	_PINDEF3(D,7,pol)
	#define _OC2B(pol)	_PINDEF3(D,6,pol)
	#define _OC3A(pol)	_PINDEF3(B,6,pol)
	#define _OC3B(pol)	_PINDEF3(B,7,pol)

	#define _UART_RX	D,0,ACTIVE_HIGH
	#define _UART_TX	D,1,ACTIVE_HIGH

   	#define _UART1_RX	D,2,ACTIVE_HIGH
	#define _UART1_TX	D,3,ACTIVE_HIGH

	#define _I2C_SDA	C,1,ACTIVE_HIGH
	#define _I2C_SCL	C,0,ACTIVE_HIGH

    #define _SPI_SCK    B,7,ACTIVE_HIGH
    #define _SPI_MISO   B,6,ACTIVE_HIGH
    #define _SPI_MOSI   B,5,ACTIVE_HIGH
    #define _SPI_SS     B,4,ACTIVE_LOW

	#define _INT0(pol)	_PINDEF3(D,2,pol)
	#define _INT1(pol)	_PINDEF3(D,3,pol)
	#define _INT2(pol)	_PINDEF3(B,2,pol)

#elif defined(__AVR_ATmega32__)

	#define _OC0A(pol)	_PINDEF3(B,3,pol)
	#define _OC0B(pol)	_PINDEF3(B,4,pol)
	#define _OC1A(pol)	_PINDEF3(D,5,pol)
	#define _OC1B(pol)	_PINDEF3(D,4,pol)
	#define _OC2(pol)	_PINDEF3(D,7,pol)

	#define _UART_RX	D,0,ACTIVE_HIGH
	#define _UART_TX	D,1,ACTIVE_HIGH

	#define _UART1_RX	D,2,ACTIVE_HIGH
	#define _UART1_TX	D,3,ACTIVE_HIGH

	#define _I2C_SDA	C,1,ACTIVE_HIGH
	#define _I2C_SCL	C,0,ACTIVE_HIGH

    #define _SPI_SCK    B,7,ACTIVE_HIGH
    #define _SPI_MISO   B,6,ACTIVE_HIGH
    #define _SPI_MOSI   B,5,ACTIVE_HIGH
    #define _SPI_SS     B,4,ACTIVE_LOW

	#define _INT0(pol)	_PINDEF3(D,2,pol)
	#define _INT1(pol)	_PINDEF3(D,3,pol)
	#define _INT2(pol)	_PINDEF3(B,2,pol)

#endif // defined(__AVR_xxx

/** @} */

#endif /* __STDPINS_H */
