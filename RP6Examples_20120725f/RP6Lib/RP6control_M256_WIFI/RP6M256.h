/* ****************************************************************************
 *                           _______________________
 *                           \| RP6  ROBOT SYSTEM |/
 *                            \_-_-_-_-_-_-_-_-_-_/           >>> RP6 M256 WIFI
 * ----------------------------------------------------------------------------
 * ------------------- [c]2011 / 2012 - AREXX ENGINEERING ---------------------
 * -------------------------- http://www.arexx.com/ ---------------------------
 * --------------------------- All rights reserved ----------------------------
 * ****************************************************************************
 * File: RP6M256.h
 * Version: 1.1 - 16.07.2012
 * Target: RP6 CONTROL M256 WIFI - ATMEGA2560 @16.00MHz
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * The RP6 CONTROL M256 WIFI header file.
 *
 * ****************************************************************************
 * CHANGELOG AND LICENSING INFORMATION CAN BE FOUND AT THE END OF THIS FILE!
 * ****************************************************************************
 */

#ifndef RP6M256_H
#define RP6M256_H

/*****************************************************************************/
// CPU Clock:

#ifndef F_CPU
#define F_CPU 16000000 //Expansion: 16.00MHz  - DO NOT CHANGE!
#endif

/*****************************************************************************/
// Includes:

#include <stdint.h>			// Standard Integer definitions
#include <avr/io.h>			// I/O Port definitions
#include <avr/interrupt.h>	// Signal and other Interrupt macros


// ---------------------------------------------------
// A/D Convertor channels (on Port F (0 - 7) and Port K (8 - 15))


#define ADC_15 		16
#define ADC_14 		14
#define ADC_13		13
#define ADC_12 		12
#define ADC_11 		11
#define ADC_10 		10
#define ADC_9 		9
#define ADC_8 		8
#define ADC_7 		7
#define ADC_6		6
#define ADC_5 		5
#define ADC_4 		4
#define ADC_3 		3
#define ADC_2 		2
#define ADC_1 		1
#define ADC_0 		0

// ---------------------------------------------------
// PORTA

#define DISP_D7				(1 << PINA7) // 
#define DISP_D6				(1 << PINA6) //
#define DISP_D5				(1 << PINA5) //
#define DISP_D4				(1 << PINA4) //
#define DISP_D3				(1 << PINA3) //
#define DISP_D2				(1 << PINA2) //
#define DISP_D1				(1 << PINA1) //
#define DISP_D0				(1 << PINA0) //

// Initial value of port and direction registers.
#define INIT_DDRA 0b11111111
#define INIT_PRTA 0b00000000

// ---------------------------------------------------
// PORTB

#define OC0A_OCM_PI7		(1 << PINB7) 	//
#define OC1B_PI6			(1 << PINB6) 	//
#define OC1A_PI5			(1 << PINB5) 	//
#define OC2A_PI4			(1 << PINB4)	//
#define SPI_MISO			(1 << PINB3) 	// IN SDCARD / ISP
#define SPI_MOSI			(1 << PINB2)    // OUT SDCARD / ISP
#define SPI_SCK				(1 << PINB1)	// OUT SDCARD / ISP
#define SPI_SS				(1 << PINB0)	// OUT SDCARD / ISP

// Initial value of port and direction registers.
#define INIT_DDRB 0b00000000
#define INIT_PRTB 0b11110111

// ---------------------------------------------------
// PORTC

#define WLAN_GPIO8_IN		(1 << PINC7)	// INPUT
#define WLAN_GPIO14_OUT		(1 << PINC6)	// OUTPUT
#define SLED4				(1 << PINC5)	// OUTPUT
#define SLED3				(1 << PINC4)	// OUTPUT
#define SLED2				(1 << PINC3)	// OUTPUT
#define SLED1				(1 << PINC2)	// OUTPUT
#define IO_PC1				(1 << PINC1)	// I2C
#define IO_PC0				(1 << PINC0)	// I2C

// Initial value of port and direction registers.
#define INIT_DDRC 0b01111100
#define INIT_PRTC 0b10000011

// ---------------------------------------------------
// PORTD

#define IO_PD7_T2			(1 << PIND7)	// 
#define IO_PD6_T1			(1 << PIND6)	// 
#define IO_PD5_XCK1			(1 << PIND5)	// 
#define IO_PD4_ICP1			(1 << PIND4)	//
#define TXD1				(1 << PIND3)	// 
#define RXD1				(1 << PIND2)	// 
#define SDA					(1 << PIND1)	// 
#define SCL					(1 << PIND0)	// 

// Initial value of port and direction registers.
#define INIT_DDRD 0b00000000
#define INIT_PRTD 0b11111111

// ---------------------------------------------------
// PORTE

#define IO_PE7_ICP3_I7 		(1 << PINE7)	// 
#define IO_PE6_T3_I6 		(1 << PINE6)	// 
#define IO_PE5_OC3C_I5 		(1 << PINE5)	// 
#define IO_PE4_OC3B_I4 		(1 << PINE4)	// 
#define IO_PE3_OC3A_AIN1 	(1 << PINE3)	// 
#define IO_PE2_XCK0_AIN0	(1 << PINE2)	// 
#define TXD0				(1 << PINE1)	// USART TX (Output)
#define RXD0				(1 << PINE0)	// USART RX (Input)

// Initial value of port and direction registers.
#define INIT_DDRE 0b00000010
#define INIT_PRTE 0b11111101

// ---------------------------------------------------
// PORTF

#define IO_ADC7 			(1 << PINF7)	//
#define IO_ADC6 			(1 << PINF6)	//
#define IO_ADC5 			(1 << PINF5)	//
#define IO_ADC4 			(1 << PINF4)	//
#define IO_ADC3 			(1 << PINF3)	//
#define IO_ADC2 			(1 << PINF2)	//
#define IO_ADC1 			(1 << PINF1)	//
#define IO_ADC0 			(1 << PINF0)	//

// Initial value of port and direction registers.
#define INIT_DDRF 0b00000000
#define INIT_PRTF 0b11111111

// ---------------------------------------------------
// PORTG

#define IO_OC0B 			(1 << PING5)	//
#define BUTTON_SW1 			(1 << PING4)	//
#define BUTTON_SW2 			(1 << PING3)	//
#define DISP_RS_ALE 		(1 << PING2)	//
#define DISP_EN_RD 			(1 << PING1)	//
#define DISP_WR 			(1 << PING0)	//

// Initial value of port and direction registers.
#define INIT_DDRG 0b00000111
#define INIT_PRTG 0b00111000

// ---------------------------------------------------
// PORTH

#define IO_PH7_T4 			(1 << PINH7)	//
#define IO_PH6_OC2B 		(1 << PINH6)	//
#define IO_PH5_OC4C 		(1 << PINH5)	//
#define IO_PH4_OC4B 		(1 << PINH4)	//
#define IO_PH3_OC4A 		(1 << PINH3)	//
#define IO_PH2_XCK2 		(1 << PINH2)	//
#define TXD2 				(1 << PINH1)	//
#define RXD2 				(1 << PINH0)	//

// Initial value of port and direction registers.
#define INIT_DDRH 0b00000000
#define INIT_PRTH 0b11111111

// ---------------------------------------------------
// PORTJ

#define WLAN_CTS 			(1 << PINJ7)	// INPUT CTS (RTS OUTPUT from WiFi)
#define INT2_PI15 			(1 << PINJ6)	//
#define INT3_PI14 			(1 << PINJ5)	//
#define INTU_PI13 			(1 << PINJ4)	//
#define INT1_PI12 			(1 << PINJ3)	//
#define WLAN_RTS 			(1 << PINJ2)	// Output RTS (CTS input to Wifi, must be low!)
#define WLAN_TXD3 			(1 << PINJ1)	// WLAN USART TX (Output)
#define WLAN_RXD3 			(1 << PINJ0)	// WLAN USART RX (Input)

// Initial value of port and direction registers.
#define INIT_DDRJ 0b00000110
#define INIT_PRTJ 0b01111101

// ---------------------------------------------------
// PORTK

#define IO_ADC15_PI23  		(1 << PINK7)	//
#define IO_ADC14_PI22  		(1 << PINK6)	//
#define IO_ADC13_PI21  		(1 << PINK5)	//
#define IO_ADC12_PI20  		(1 << PINK4)	//
#define IO_ADC11_PI19  		(1 << PINK3)	//
#define IO_ADC10_PI18  		(1 << PINK2)	//
#define IO_ADC9_PI17 		(1 << PINK1)	//
#define IO_ADC8_PI16 		(1 << PINK0)	//

// Initial value of port and direction registers.
#define INIT_DDRK 0b00000000
#define INIT_PRTK 0b11111111

// ---------------------------------------------------
// PORTL

#define WLAN_RESET 			(1 << PINL7)	// Output
#define WLAN_WAKE 			(1 << PINL6)	// Output
#define IO_PL5_OC5C 		(1 << PINL5)	//
#define IO_PL4_OC5B 		(1 << PINL4)	//
#define IO_PL3_OC5A 		(1 << PINL3)	//
#define IO_PL2_T5 			(1 << PINL2)	//
#define IO_PL1_ICP5 		(1 << PINL1)	//
#define IO_PL0_ICP4 		(1 << PINL0)	//

// Initial value of port and direction registers.
#define INIT_DDRL 0b11000000
#define INIT_PRTL 0b00111111

/*****************************************************************************/
// I/O Port init macro - always call this first!

#define portInit();	\
PORTA = INIT_PRTA;	\
PORTB = INIT_PRTB;	\
PORTC = INIT_PRTC;	\
PORTD = INIT_PRTD;	\
PORTE = INIT_PRTE;	\
PORTF = INIT_PRTF;	\
PORTG = INIT_PRTG;	\
PORTH = INIT_PRTH;	\
PORTJ = INIT_PRTJ;	\
PORTK = INIT_PRTK;	\
PORTL = INIT_PRTL;	\
DDRA = INIT_DDRA;	\
DDRB = INIT_DDRB;	\
DDRC = INIT_DDRC;	\
DDRD = INIT_DDRD;	\
DDRE = INIT_DDRE;	\
DDRF = INIT_DDRF;	\
DDRG = INIT_DDRG;	\
DDRH = INIT_DDRH;	\
DDRJ = INIT_DDRJ;	\
DDRK = INIT_DDRK;	\
DDRL = INIT_DDRL;


/*****************************************************************************/
// Some additional predefined stuff, that further simplifies the source code:

// Boolean:
#define true 1
#define false 0

// Application and Bootload addresses:
#define app_start(); (((void(*)(void))0x0000)());
#define bl_start(); (((void(*)(void))0x3C00)());

// Assembly and system macros:
#define nop(); {asm volatile("nop\n\t");}
#define bl_reset(); cli();bl_start();

/*****************************************************************************/
// Baudrates:

// Default rates used for link with WIFI Module:

#define BAUD_LOW_WIFI		9600UL  //Low speed - 9600 kBaud
#define UBRR_BAUD_LOW_WIFI	((F_CPU/(16*BAUD_LOW_WIFI))-1)

#define BAUD_HIGH_WIFI		500000UL  // High speed - standard rate in this case
#define UBRR_BAUD_HIGH_WIFI	((F_CPU/(16*BAUD_HIGH_WIFI))-1)


/*
List of possible exactly matched Baudrates for WIFI Module and ATMEGA
(non standard - but this does not cause any issues as this is just a direct
on-board link between two known devices): 
*/

#define BAUD_WIFI		250000UL 
// #define BAUD_WIFI		166667UL 
// #define BAUD_WIFI		125000UL 
// #define BAUD_WIFI		100000UL 
// #define BAUD_WIFI		83333UL 
// #define BAUD_WIFI		71429UL 
// #define BAUD_WIFI		62500UL 
// #define BAUD_WIFI		45455UL 
// #define BAUD_WIFI		41667UL 
// #define BAUD_WIFI		38462UL 
// #define BAUD_WIFI		33333UL 
// #define BAUD_WIFI		31250UL 
// #define BAUD_WIFI		25000UL 
// #define BAUD_WIFI		20000UL 
// #define BAUD_WIFI		19231UL 
// #define BAUD_WIFI		15625UL 
// #define BAUD_WIFI		12500UL 
// #define BAUD_WIFI		10000UL 
// #define BAUD_WIFI		9615UL 


#define UBRR_BAUD_WIFI	((F_CPU/(16*BAUD_WIFI))-1)


// Microcontroller PROG/UART Baudrates: 
 
#define BAUD_LOW		38400UL  //Low speed - 38.4 kBaud
#define UBRR_BAUD_LOW	((F_CPU/(16*BAUD_LOW))-1)

#define BAUD_HIGH		500000UL //High speed - 500 kBaud
#define UBRR_BAUD_HIGH	((F_CPU/(16*BAUD_HIGH))-1)

/******************************************************************************
 * Additional info
 * ****************************************************************************
 * Changelog:
 * - v. 1.1 16.07.2012 by Dominik S. Herwald
 * 		- added a List of supported Baudrates
 * - v. 1.0 (initial release) 24.02.2012 by Dominik S. Herwald
 *
 * ****************************************************************************
 * Bugs, feedback, questions and modifications can be posted on the AREXX Forum
 * on http://www.arexx.com/forum/ !
 * Of course you can also write us an e-mail to: info@arexx.nl
 * AREXX Engineering may publish updates from time to time on AREXX.com!
 * ****************************************************************************
 * - LICENSE -
 * GNU GPL v2 (http://www.gnu.org/licenses/gpl.txt, a local copy can be found
 * on the RP6 CD in the RP6 sorce code folders!)
 * This program is free software. You can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 * ****************************************************************************
 */

#endif

/*****************************************************************************/
// EOF
