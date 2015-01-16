/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT CONTROL EXAMPLE
 * ****************************************************************************
 * Example: Serial Interface Example - reading and writing numbers and strings
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

#include "RP6RobotBaseLib.h" // The RP6 Robot Control Library.

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

	int index = 0;
	while(getBufferLength() > 0) {
		receiveBuffer[index] = readChar();
		index++;

		//- reserve the last character of the buffer for '\0' character
		//- check not to write outside array boundaries.	
		if (index > (RECEIVE_BUFFER_SIZE -2)) 
		{
			break;
		}
	}

	receiveBuffer[index] = '\0';

	writeString_P("Echo: ");
	writeString(receiveBuffer);
}

int main(void)
{
	initRobotBase(); // Always call this first! 
	                  // The Processor will not work correctly otherwise.
					 
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
