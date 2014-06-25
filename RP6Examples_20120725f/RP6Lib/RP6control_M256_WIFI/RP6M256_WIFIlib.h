/* ****************************************************************************
 *                           _______________________
 *                           \| RP6  ROBOT SYSTEM |/
 *                            \_-_-_-_-_-_-_-_-_-_/           >>> RP6 M256 WIFI
 * ----------------------------------------------------------------------------
 * ------------------- [c]2011 / 2012 - AREXX ENGINEERING ---------------------
 * -------------------------- http://www.arexx.com/ ---------------------------
 * --------------------------- All rights reserved ----------------------------
 * ****************************************************************************
 * File: RP6M256_WIFIlib.h
 * Version: 1.1 - 16.07.2012
 * Target: RP6 CONTROL M256 WIFI - ATMEGA2560 @16.00MHz
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * The RP6 uart function Library header file. Detailled description
 * of each function can be found in the RP6uart.c file!
 *
 * ****************************************************************************
 * CHANGELOG AND LICENSING INFORMATION CAN BE FOUND AT THE END OF THIS FILE!
 * ****************************************************************************
 */

#ifndef RP6_WIFILIB_H
#define RP6_WIFILIB_H

/*****************************************************************************/
// Includes:

#include "RP6M256.h"
#include "RP6M256Lib.h"

#include <avr/pgmspace.h> 	// Program memory (=Flash ROM) access routines.
#include <stdlib.h>			// C standard functions (e.g. itoa...)
#include <string.h>
#include <avr/io.h>			// I/O Port definitions
#include <avr/interrupt.h>	// Interrupt macros (e.g. cli(), sei())

/*****************************************************************************/
// UART


// TX:

extern uint8_t cts_active; 
extern uint8_t enable_flow;

void writeChar_WIFI(char ch);
void writeStringLength_WIFI(char *data, uint8_t length, uint8_t offset);
void writeString_WIFI(char *data);
void writeNStringP_WIFI(const char *pstring);
#define writeString_P_WIFI(__pstr) writeNStringP_WIFI((PSTR(__pstr)))

#define HEX 16
#define DEC 10
#define OCT 8
#define BIN 2
void writeInteger_WIFI(int16_t number, uint8_t base);
void write32BitInteger_WIFI(int32_t number, uint8_t base);
void writeIntegerLength_WIFI(int16_t number, uint8_t base, uint8_t length);


// RX:
extern volatile uint8_t uart_status;

#define UART_RECEIVE_BUFFER_SIZE_WIFI 1000
#define UART_BUFFER_OK 0
#define UART_BUFFER_OVERFLOW 1

char readChar_WIFI(void);
uint16_t readChars_WIFI(char *buf, uint16_t numberOfChars);
uint16_t getBufferLength_WIFI(void);
void clearReceptionBuffer_WIFI(void);


extern char receiveBuffer_WIFI[256];

extern uint16_t buffer_length_WIFI;
extern uint8_t buffer_pos_WIFI;


uint8_t parseLine_WIFI(uint8_t data);


// Command Mode: 
int8_t waitCharResponse_WIFI(char response, uint32_t timeout);
int8_t waitResponse_WIFI(char * response, uint32_t timeout);
int8_t issueCMD_WIFI(char * cmd, char * response);

int8_t writeCommand_WIFI(char * cmd);

void cmd_error_exit_WIFI(void);
void enter_cmd_mode_WIFI(void);
void leave_cmd_mode_WIFI(void);


// These Makros add very basic error handling around the issueCMD_WIFI function:
#define wifi_cmd(__command__) { if(!issueCMD_WIFI(__command__, "AOK")) { goto wifi_cmd_error; } } 
#define wifi_cmd_resp(__command__, __response__) { if(!issueCMD_WIFI(__command__, __response__)) { goto wifi_cmd_error; } }


#endif

/******************************************************************************
 * Additional info
 * ****************************************************************************
 * Changelog:
 * 
 *  ---> changes are documented in the file "RP6M256_WIFIlib.c"
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

/*****************************************************************************/
// EOF
