/* ****************************************************************************
 *                           _______________________
 *                           \| RP6  ROBOT SYSTEM |/
 *                            \_-_-_-_-_-_-_-_-_-_/           >>> RP6 M256 WIFI
 * ----------------------------------------------------------------------------
 * ------------------- [c]2011 / 2012 - AREXX ENGINEERING ---------------------
 * -------------------------- http://www.arexx.com/ ---------------------------
 * --------------------------- All rights reserved ----------------------------
 * ****************************************************************************
 * File: RP6M256_WIFIlib.c
 * Version: 1.1 - 16.07.2012
 * Target: RP6 CONTROL M256 WIFI - ATMEGA2560 @16.00MHz
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * The RP6 WIFI UART Library. 
 *
 * It contains data transfer functions for the UART used for WIFI Module. 
 * This is very similar to the normal UART Library, so in general usage is
 * the same. But this lib supports flow control and has some additional 
 * functions for the WIFI Module command mode.
 *
 * --------
 *
 * Hint: You should better leave all this as it is if you just started with
 * C programming, but it is a good idea to read the comments and review the
 * code, it will help you to understand C programming for AVR better.
 *
 * Of course you are free to add new functions and improvements to this
 * library and make them available to the public on the Internet.
 * Please use the changelog at the end of this file to document your
 * changes. And add your name (or nickname) to any new function or 
 * modification you added! E.g. a "modified by <nickname> at <date>" is 
 * always a good idea to show other users where and what you changed the 
 * source code!
 *
 * ****************************************************************************
 * CHANGELOG AND LICENSING INFORMATION CAN BE FOUND AT THE END OF THIS FILE!
 * ****************************************************************************
 */
 
/*****************************************************************************/
// Includes:

#include "RP6M256_WIFIlib.h"



/*****************************************************************************/
// UART transmit functions:

uint8_t cts_active = 0; 
uint8_t enable_flow = 1;

/**
 * Write a single character to the UART used for the WIFI Module.
 * It supports flow control which is required for the WIFI Module. 
 *
 * Example:
 *
 *			writeChar_WIFI('R');
 *			writeChar_WIFI('P');
 *			writeChar_WIFI('6');
 *			writeChar_WIFI('\n');
 *			// '\n' is a special code for the "new line" character!
 *			writeChar_WIFI('0');
 *			writeChar_WIFI(48); // 48 is ASCII code for '0'
 *			writeChar_WIFI(49); // 1
 *			writeChar_WIFI(50); // 2
 *			writeChar_WIFI(51); // 3
 *			//...
 *
 *			would output:
 *			RP6
 *			00123
 *
 */
void writeChar_WIFI(char ch)
{
    while (!(UCSR3A & (1<<UDRE3))){nop();}
    while((PINJ & WLAN_CTS) && enable_flow) {cts_active = 1;}
    UDR3 = (uint8_t)ch;
}

/**
 * Writes a null terminated string or buffer from SRAM to UART.
 * Make sure that it really IS null terminated!
 * ("null terminated" means that the string has a null (=0) at the end.
 * this is automatically added to it by the compiler when you put the
 * string in double quotes like this: writeString("test");  )
 *
 * ATTENTION: This fills up SRAM Memory with the
 * strings, even if they are constant and never changed.
 * If you want to write constant text strings to the UART, better use
 * writeNStringP(const uint8_t *pstring) (s. below), which reads the
 * text from flash program memory and does not fill up the SRAM with
 * the string data!
 *
 * Example:
 *
 *			writeString_WIFI("RP6 Robot System\n");
 *
 */
void writeString_WIFI(char *string)
{
	while(*string)
		writeChar_WIFI(*string++);
}
		
/**
 * Writes a null terminated string from flash program memory to UART.
 * You can use the macro writeString_P(STRING); , this macro
 * ensures that the String is stored in program memory only!
 * Otherwise you need to use PSTR("your string") from AVRLibC for this. 
 *
 * Example:
 *
 *			writeNStringP_WIFI(PSTR("RP6 Robot System\n"));
 *
 *			// There is also a Macro that makes life easier and
 *			// you can simply write:
 *			writeString_P_WIFI("RP6 Robot System\n");
 *
 */
void writeNStringP_WIFI(const char *pstring)
{
    uint8_t c;
    for (;(c = pgm_read_byte_near(pstring++));writeChar_WIFI(c));
}


/**
 * Writes a string with specified length and offset from SRAM to UART.
 * If it is a null terminated string, output will be stopped at the
 * end! It does not need to be null terminated, but it is recommended
 * to use only null terminated strings/buffers, otherwise the function could
 * output any SRAM memory data stored after the string until it reaches a 0
 * or the specified length.
 *
 * Example:
 *
 *			writeStringLength_WIFI("RP6 Robot Sytem\n",16,0);
 *			// would output: "RP6 Robot Sytem\n"
 *			writeStringLength_WIFI("RP6 Robot Sytem\n",11,4);
 *			// would output: "Robot System"
 * 			writeStringLength_WIFI("RP6 Robot Sytem\n",40,4);
 *			// would output: "Robot System\n"
 *			// No matter if the specified length is 40 characters!
 *
 */
void writeStringLength_WIFI(char *string, uint8_t length, uint8_t offset)
{
	for(string = &string[offset]; *string && length; length--)
		writeChar_WIFI(*string++);
}

/**
 * Write a number (with specified base) to the UART.
 *
 * Example:
 *
 *			// Write a hexadecimal number to the UART:
 *			writeInteger_WIFI(0xAACC,16);
 *			// Instead of 16 you can also write "HEX" as this is defined in the
 *			// RP6RobotBaseLib.h :
 *			writeInteger_WIFI(0xAACC, HEX);
 *			// Other Formats:
 *			writeInteger_WIFI(1024,DEC);  	// Decimal
 *			writeInteger_WIFI(044,OCT);		// Ocal
 *			writeInteger_WIFI(0b11010111,BIN); // Binary
 */
void writeInteger_WIFI(int16_t number, uint8_t base)
{
	char buffer[17];
	itoa(number, &buffer[0], base);
	writeString_WIFI(&buffer[0]);
}

/**
 * Write a 32 Bit number (with specified base) to the WIFI UART.
 * Usage is the same as writeInteger.
 */
void write32BitInteger_WIFI(int32_t number, uint8_t base)
{
	char buffer[33];
	ltoa(number, &buffer[0], base);
	writeString_WIFI(&buffer[0]);
}

/**
 * Same as writeInteger, but with defined length.
 * This means this routine will add leading zeros to the number if length is
 * larger than the actual value or cut the upper digits if length is smaller
 * than the actual value.
 *
 * Example:
 *
 *			// Write a hexadecimal number to the UART:
 *			writeIntegerLength_WIFI(0xAACC, 16, 8);
 *			// Instead of 16 you can also write "HEX" as this is defined in the
 *			// RP6RobotBaseLib.h :
 *			writeIntegerLength_WIFI(0xAACC, HEX, 8);
 *			// Other Formats:
 *			writeIntegerLength_WIFI(1024,DEC,6);  	// Decimal
 *			writeIntegerLength_WIFI(044,OCT,4);		// Ocal
 *			writeIntegerLength_WIFI(0b11010111,BIN,8); // Binary
 */
void writeIntegerLength_WIFI(int16_t number, uint8_t base, uint8_t length)
{
	char buffer[17];
	itoa(number, &buffer[0], base);
	int8_t cnt = length - strlen(buffer);
	if(cnt > 0) {
		for(; cnt > 0; cnt--, writeChar_WIFI('0'));
		writeString_WIFI(&buffer[0]);
	}
	else 
		writeStringLength_WIFI(&buffer[0],length,-cnt);
}

/*****************************************************************************/
// UART receive functions:

volatile char uart_receive_buffer_WIFI[UART_RECEIVE_BUFFER_SIZE_WIFI+1];

volatile uint8_t uart_status_WIFI;

uint16_t read_pos_WIFI = 0;
uint16_t write_pos_WIFI = 0; 
uint16_t read_size_WIFI = 0;
uint16_t write_size_WIFI = 0;

/**
 * UART receive ISR.
 * Handles reception to circular buffer.
 */
ISR(USART3_RX_vect)
{	
	static volatile uint8_t dummy;
	if(((uint16_t)(write_size_WIFI - read_size_WIFI)) < UART_RECEIVE_BUFFER_SIZE_WIFI) {
		uart_receive_buffer_WIFI[write_pos_WIFI++] = UDR3;
		write_size_WIFI++;
		if(write_pos_WIFI > UART_RECEIVE_BUFFER_SIZE_WIFI) 
			write_pos_WIFI = 0;
		uart_status_WIFI = UART_BUFFER_OK;
	}
	else {	
		dummy = UDR3;
		PORTJ |= WLAN_RTS;
		uart_status_WIFI = UART_BUFFER_OVERFLOW;
	}
}

/**
 * Read a char from the circular buffer. 
 * The char is removed from the buffer (or more precise: not accessible directly anymore
 * and will be overwritten as soon as new data becomes available)!
 *
 * Example:
 *
 * // [...]
 * if(getBufferLength_WIFI()) 		
 *	   receivedData[data_position++] = readChar_WIFI();
 * // [...]
 *
 */
char readChar_WIFI(void)
{
	if(((uint16_t)(write_size_WIFI - read_size_WIFI)) > 0) {
		read_size_WIFI++;
		if(read_pos_WIFI > UART_RECEIVE_BUFFER_SIZE_WIFI) 
			read_pos_WIFI = 0;
		uart_status_WIFI = UART_BUFFER_OK;
	  PORTJ &= ~WLAN_RTS;
		return uart_receive_buffer_WIFI[read_pos_WIFI++];
	}
	return 0;
}

/**
 * Same as readChar, but this function copies numberOfChars chars from the
 * circular buffer to buf. 
 * It also returns the number of characters really copied to the buffer! 
 * Just in case that there were fewer chars in the buffer...
 */
uint16_t readChars_WIFI(char *buf, uint16_t numberOfChars)
{
	uint16_t i = 0;
	while(((uint16_t)(write_size_WIFI - read_size_WIFI)) >= 0 && (i < numberOfChars)) {
		read_size_WIFI++;
		buf[i++] = uart_receive_buffer_WIFI[read_pos_WIFI++];
		if(read_pos_WIFI > UART_RECEIVE_BUFFER_SIZE_WIFI) 
			read_pos_WIFI = 0;
	}
	uart_status_WIFI = UART_BUFFER_OK;
	PORTJ &= ~WLAN_RTS;
	return i;
}

/**
 * Returns the current number of elements in the buffer.
 *
 * Example:
 * s. readChar function above!
 */
uint16_t getBufferLength_WIFI(void)
{
	return (((uint16_t)(write_size_WIFI - read_size_WIFI)));
}

/**
 * Clears the reception buffer - it disables UART Receive 
 * interrupt for a short period of time. 
 */
void clearReceptionBuffer_WIFI(void)
{
	static uint8_t dummy;
	UCSR3B &= ~(1 << RXCIE3); // disable UART RX Interrupt
	dummy = UDR3;
	read_pos_WIFI = 0;
	write_pos_WIFI = 0; 
	read_size_WIFI = 0;
	write_size_WIFI = 0;
	uart_status_WIFI = UART_BUFFER_OK;
	UCSR3B |= (1 << RXCIE3); // enable Interrupt again
	PORTJ &= ~WLAN_RTS;
}




/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// WIFI Command Mode related functions


// Uncomment this to enable some debug output in the command functions: 
#define WIFI_DEBUG


uint8_t packet_state = 0; 
uint8_t packet_type = 0;
uint8_t connection_num = 0;
uint16_t packet_length = 0;

char receiveBuffer_WIFI[256];

uint16_t buffer_length_WIFI = 0;
uint8_t buffer_pos_WIFI = 0;

/*
 * Writes received byte data into buffer receiveBuffer_WIFI and returns if a full
 * line of text has been received. Then the buffer is terminated with 0.  
 * Returns 0 if new line has been detected and buffer is ready.
 * Returns 1 if no newline has been received yet
 * Returns 2 if buffer overflow occured 
 */
uint8_t parseLine_WIFI(uint8_t data)
{
	static uint8_t last_char_WIFI = 0;

		if(data=='\n' || data=='\r') {
			if(last_char_WIFI)  // Check for \r\n or \n\r sequences --> ignore. 
			{
				if(data=='\n' && last_char_WIFI=='\r') {
					last_char_WIFI = 0;
					buffer_pos_WIFI = 0;
					return 0;
				}
				if(data=='\r' && last_char_WIFI=='\n') {
					last_char_WIFI = 0;
					buffer_pos_WIFI = 0;
					return 0;
				}
			}
			last_char_WIFI = data;
			receiveBuffer_WIFI[buffer_pos_WIFI]='\0'; 
			buffer_length_WIFI = buffer_pos_WIFI;
			buffer_pos_WIFI = 0;
			return 1;
		}
		else if(buffer_pos_WIFI > 254) {									
			receiveBuffer_WIFI[255]='\0';	
			buffer_length_WIFI = buffer_pos_WIFI;
			last_char_WIFI = 0;
			buffer_pos_WIFI = 0;
			return 2;
		}
		last_char_WIFI = 0;
		receiveBuffer_WIFI[buffer_pos_WIFI] = data;
		buffer_pos_WIFI++;
	
	return 0;
}


/*
 * Write Command and wait for each char to be echoed back to
 * make sure the command is received properly. 
 * This function does not add the \r automatically, you must do
 * this otherwise the command will not be executed. 
 *
 * Returns 1 on success, 0 on failure.
 *
 * Example:
 * writeCommand_WIFI("scan\r");
 *
 *
 */
int8_t writeCommand_WIFI(char * cmd)
{
	uint32_t dly = 0;
	uint8_t uart_data = 0;
	
	#ifdef WIFI_DEBUG
		writeString_P("RAW_CMD{");
		writeString(cmd);
		writeString_P("}\n");
	#endif
	clearReceptionBuffer_WIFI();

	char c = 0;
	uint8_t error = 0;
	while(*cmd) {
		c = *cmd++;
		#ifdef WIFI_DEBUG
			writeChar(c);
		#endif
		writeChar_WIFI(c);
		dly = 150;
		while(true) {
			sleep(2);
			if(getBufferLength_WIFI()) {
				uart_data = readChar_WIFI(); 
				if(uart_data == c) {
					error = 0;
					dly = 0; 
					break;
				}
				else {
					error = 1;
					break;
				}
			}
			else if(dly-- < 1) {
				if(error < 2) {
					writeString_P("\n#### TIMEOUT! TRY AGAIN! ####\n");
					writeChar_WIFI(c);
					dly = 250;
					error++;
				}
				else
					break;
			}
		}
		if(error > 0) {
			writeString_P("ERROR!\n");
			return 0;
		}
	}

	if(error > 0) {
		#ifdef WIFI_DEBUG
			writeString_P("\n#### Write Commad FAIL! ####\n");
		#endif
		return 0;
	}
	
	#ifdef WIFI_DEBUG
		writeString_P("\nWrite Commad done!\n");
	#endif
	return 1;
}


/*
 * Wait (blocking!) until a specific character has been received. 
 *
 * Returns 1 on success, 0 on failure.
 *
 */
int8_t waitCharResponse_WIFI(char response, uint32_t timeout)
{
	uint32_t dly = 0;
	
	#ifdef WIFI_DEBUG
		writeString_P("\nW{");
		writeChar(response);
		writeString_P("}\n");
	#endif

	uint8_t uart_data = 0;

	while(true) {
		if(getBufferLength_WIFI()) {
			uart_data = readChar_WIFI(); 

			dly = 0; 
			#ifdef WIFI_DEBUG
				writeChar(uart_data);
			#endif
			if(uart_data == response) {
				#ifdef WIFI_DEBUG
					writeString_P("\n[OK:");
					writeChar(uart_data);
					writeString_P("]\n");
				#endif
				mSleep(10);
				return 1;
			}
		}

		if(dly >= timeout)	{ // 750000
			writeString_P("\nTIMEOUT\n");
			dly = 0;
			mSleep(100);
			return 0;
		}
		dly++;
	}
	#ifdef WIFI_DEBUG
		writeString_P("\nEND\n");
	#endif
	return 0;
}


/*
 * Wait (blocking!) until a specific string has been received. 
 *
 * Returns 1 on success, 0 on failure.
 * 
 */
int8_t waitResponse_WIFI(char * response, uint32_t timeout)
{
	uint32_t dly = 0;
	#ifdef WIFI_DEBUG
		writeString_P("\nW{");
		writeString(response);
		writeString_P("}\n");
	#endif
	
	uint8_t uart_data = 0;
	while(true) {
		
		if(getBufferLength_WIFI()) {
			uart_data = readChar_WIFI(); 
			dly = 0; 
			#ifdef WIFI_DEBUG
				writeChar(uart_data);
			#endif
			
			if(parseLine_WIFI(uart_data)) {
				if(buffer_length_WIFI > 1) {
					if(strstr(receiveBuffer_WIFI,response)!=NULL) {
						#ifdef WIFI_DEBUG
							writeString_P("\n[OK:");
							writeString(receiveBuffer_WIFI);
							writeString_P("]\n");
						#endif
						mSleep(10);
						return 1;
					}
				}
			}
		}

		if(dly >= timeout)	{ // 750000
			writeString_P("\nTIMEOUT\n");
			dly = 0;
			mSleep(100);
			return 0;
		}
		dly++;
	}
	#ifdef WIFI_DEBUG
		writeString_P("\nEND\n");
	#endif
	return 0;
}


/*
 * Issue a command and wait for a specific response string. 
 * Also checks the echo of the command that is send from the WIFI Module. 
 * Retries up to 3 times. 
 * 
 * Warning: If the WLAN Module is buisy or something goes wrong, 
 * this may also fail completely!
 *      
 * Returns 1 on success, 0 on failure.
 *
 *
 * Example usage:
 * enter_cmd_mode_WIFI();
 * issueCMD_WIFI("set ip address 192.168.10.180","AOK");
 * issueCMD_WIFI("set ip netmask 255.255.255.0","AOK");
 * issueCMD_WIFI("set ip gateway 192.168.10.1","AOK");
 * leave_cmd_mode_WIFI();
 *
 */
int8_t issueCMD_WIFI(char * cmd, char * response)
{
	uint32_t dly = 0;
	static uint8_t retry = 0; 
	uint8_t uart_data = 0;
	
	#ifdef WIFI_DEBUG
		writeString_P(" W_CMD{");
		writeString(cmd);
		writeString_P(" ; ");
		writeString(response);
		writeString_P("}\n");
	#endif

	if(writeCommand_WIFI(cmd) <= 0)
	{
		writeString_P("\n#### WRITE COMMAND FAILURE! ####\n");
		return -1;
	}
	writeString_WIFI("\r");
	
	char * ret = NULL;
	
	while(true) {
		if(getBufferLength_WIFI()) {
			uart_data = readChar_WIFI(); 
			dly = 0; 
			if(parseLine_WIFI(uart_data)) {
				if(buffer_length_WIFI > 1) {
					ret = strstr(receiveBuffer_WIFI,response);
					if(ret!=NULL) {
						#ifdef WIFI_DEBUG
							writeString_P(" [RESP_OK:");
							writeString(receiveBuffer_WIFI);
							writeString_P("]\n");
						#endif
						mSleep(10);
						retry = 0;
						return 1;
					}
				}
			}
		}
		if(dly >= 250000)	{	
			writeString_P("\n####\n");
			writeString(cmd);
			writeString_P("\n#### TIMEOUT! ####n");
			dly = 0;
			mSleep(100);
			if(retry < 3) {
				retry++;
				writeString_P("\n#### RETRY! ####\n");
				return issueCMD_WIFI(cmd, response);
			}
			else {
				retry = 0;
				return 0;
			}
		}
		else
			dly++;
	}
	#ifdef WIFI_DEBUG
		writeString_P("\nEND\n");
	#endif
	retry = 0;
	return 0;
}


/*
 * Simple error handler, make sure command mode is left. 
 */
void cmd_error_exit_WIFI(void)
{
	writeString("\nWLAN CMD ERROR! Leaving WLAN CMD Mode\n");
	writeString_WIFI("\r");
	writeString_WIFI("exit\r");
	mSleep(150);
	writeString_WIFI("\r");
	mSleep(50);
	writeString_WIFI("exit\r");
	mSleep(150);
	writeString_WIFI("\r");
	writeString_WIFI("exit\r");
	writeString_WIFI("\r");
	mSleep(150);
	writeString("\n******* Last input from WIFI:\n");
	while(getBufferLength_WIFI()) {
		writeChar(readChar_WIFI());
	}
	writeString("\n");
}



/*
 * Activate command mode of the WIFI Module.
 * Make sure to leave it again after you performed all commands!
 *
 * To speed up entering command mode, make sure GPIO14 usage is enabled 
 * (can be done via RobotLoader)
 */
void enter_cmd_mode_WIFI(void)
{
	#ifdef WIFI_DEBUG
		writeString_P("\n### WIFI Enter CMD mode\n"); 
	#endif
	
	clearReceptionBuffer_WIFI();
	
	if(use_gpio14) {
		#ifdef WIFI_DEBUG
			writeString_P("ENTER CMD GPIO14!\n"); 
		#endif
		PORTC |= WLAN_GPIO14_OUT;
		mSleep(5);
		writeString_WIFI("\r");
		if(!waitCharResponse_WIFI('?', 75000)) { 
		// Try once again: 
			PORTC &= ~WLAN_GPIO14_OUT;
			#ifdef WIFI_DEBUG
				writeString_P("\n### CMD mode ***FAIL*** TRY AGAIN...\n"); 
			#endif
			mSleep(5);
			PORTC |= WLAN_GPIO14_OUT;
			mSleep(5);
			writeString_WIFI("\r");
			if(!waitCharResponse_WIFI('?', 750000)) { goto wifi_cmd_error; }
			PORTC &= ~WLAN_GPIO14_OUT;
		}
		PORTC &= ~WLAN_GPIO14_OUT;
	}
	else {
		#ifdef WIFI_DEBUG
			writeString_P("ENTER CMD NORMAL!\n"); 
		#endif
		mSleep(260);
		writeString_WIFI("$$$");
		if(!waitResponse_WIFI("CMD", 7500000)) { goto wifi_cmd_error; }
	}

	mSleep(5);
	writeString_WIFI("\r");
	mSleep(1);
	
	return;
	wifi_cmd_error:
		cmd_error_exit_WIFI();
}


/*
 * Leave command mode of the WIFI Module. 
 */
void leave_cmd_mode_WIFI(void)
{
	#ifdef WIFI_DEBUG
		writeString_P("\n### WIFI Leave CMD mode\n"); 
	#endif
	
	writeString_WIFI("\r");
	mSleep(150);
	issueCMD_WIFI("exit", "EXIT");
}




/******************************************************************************
 * Additional info
 * ****************************************************************************
 * Changelog:
 * - v. 1.1 16.07.2012 by Dominik S. Herwald
 *    - FIX: various bugfixes and enhancements for the command mode functions
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

/*****************************************************************************/
// EOF
