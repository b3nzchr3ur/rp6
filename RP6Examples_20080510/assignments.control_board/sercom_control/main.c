/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT CONTROL EXAMPLE
 * ****************************************************************************
 * Example: Serial Interface Example - reading and writing numbers and strings
 * Author(s): P. Lambooij
 * ****************************************************************************
 *
 * Description:
 * This sample program shows how to send and receive data over the serial port (UART).
 * It uses some of the interface functions of the RP6Library (see also "RP6uart.c"). 
 * The writeInteger function is very important because you can
 * output sensor values over the serial interface with it.
 * This is very useful to solve problems in your programs or optimize some
 * parameters.
 *
 */

/*****************************************************************************/
// Includes:

#include "RP6ControlLib.h" // The RP6 Robot Control Library.
								// Always needs to be included!

#define RECEIVE_BUFFER_SIZE 101

void SendDataOverUART(int value)
{
	// These functions send data over the UART
	writeString_P("Counter: "); 

	writeInteger(value, DEC);
	writeString_P("(DEC) | ");

	writeInteger(value, HEX);
	writeString_P("(HEX) | ");
	
	writeInteger(value, BIN);
	writeString_P("(BIN) ");

	writeChar('\n'); // New Line
}

void ShowDataReceivedOverUART( void )
{
	char receiveBuffer[RECEIVE_BUFFER_SIZE];
	uint8_t nrOfCharsReceived = getBufferLength();
	
	// check if no data was received
	if (nrOfCharsReceived == 0) return;

	if (nrOfCharsReceived > (RECEIVE_BUFFER_SIZE -1)) 
		nrOfCharsReceived = RECEIVE_BUFFER_SIZE-1;

	for(int c = 0; c < nrOfCharsReceived; c++) {
		receiveBuffer[c] = readChar();
	}
	receiveBuffer[nrOfCharsReceived + 1 ] = '\0';

	clearLCD();
	writeStringLCD(receiveBuffer);
}

int main(void)
{
	initRP6Control(); // Always call this first! 
	                  // The Processor will not work correctly otherwise.
					 
	initLCD(); // Initialize the LC-Display (LCD)
			   // Always call this before using the LCD!

	// Play two sounds with the Piezo Beeper on the RP6Control:
	sound(180,80,25);
	sound(220,80,0);

	// Write a text message to the UART:
	writeString_P("\nCounting and Receiving\n");
	
	// Define a counting variable:
	uint16_t counter = 0;
	
	// clear the UART buffer once at the start of the program
	clearReceptionBuffer();
	
	while(true)
	{
		// example of sending some data over UART
		counter++;    
		SendDataOverUART(counter);

		//example of receiving some data over UART
		ShowDataReceivedOverUART();
		
		mSleep(100); // delay 100ms = 0.1s
	}
	return 0;
}
