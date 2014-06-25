/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M256 Examples
 * ****************************************************************************
 * Example: WLAN Command Example
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * For some Applications it can be required to send commands to the 
 * WIFI Module e.g. to get some informations like signal strength or 
 * Accesspoints in reach.
 * It is also possible to make connections and use certain protocol clients
 * (HTTP Client, FTP Client... ), but this is not shown in detail here.
 *
 *
 * There are some small helper functions in the WIFILib that make sending
 * commands easier.
 * This very basic example just shows you how to use these functions. 
 *
 * You can watch the output in BOTH Terminals, Serial and Network/WiFi. 
 * On the serial Terminal several additional infos are shown. 
 * The WIFI Interface outputs a list of Accesspoints that are within range.
 * Due to the scanning the RSSI value may drop down from time to time.
 * So you may see the signal strength bargraph going red occasionally. 
 *
 * Please note that the commands can fail! When the module is buisy or
 * does not receive a command properly for whatever reason, 
 * you can get timeout errors.
 * The issue_CMD function then tries to auto repeat the command a few times.
 *
 * ############################################################################
 * The Robot does NOT move in this example! You can simply put it on a table
 * next to your PC and you should connect it to the PC via the USB Interface!
 * You should also connect to it via WIFI.
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6M256Lib.h" 

/*****************************************************************************/


char response_buffer[UART_RECEIVE_BUFFER_SIZE_WIFI+1];

/**
 * Scan for Accesspoints, output via serial Interface
 */
void wlan_scan(void)
{
	writeString_P("\n### WIFI SCAN\n"); 
	
	setLEDs(0b0000);
	enter_cmd_mode_WIFI();     	// Enable Command Mode for the WIFI Module.
								// In this mode, no data is transmitted but everything
								// that is send to the wifi module is interpreted 
								// as a command.
	
	writeString(receiveBuffer_WIFI);  // Should show "CMD" in the terminal. 
	writeChar('\n');
	
	setLEDs(0b1000);
	issueCMD_WIFI("ver","WiFly");       // Get Version Infos
	writeString(receiveBuffer_WIFI);    // This only works well if a 
	writeChar('\n');				    // SINGLE text line response is received!
									    // For more text output you need to check the real Buffer (s. below)
	setLEDs(0b1100);
	issueCMD_WIFI("get mac","Mac");     // Module MAC Address
	writeString(receiveBuffer_WIFI);
	writeChar('\n');
	
	setLEDs(0b0110);
	issueCMD_WIFI("show rssi","RSSI");  // RSSI = Received Signal Strength Indicator
	writeString(receiveBuffer_WIFI);
	writeChar('\n');
	
	setLEDs(0b0011);
	writeCommand_WIFI("show status\r"); // Various statistics, multiple lines output
	mSleep(25);
	while(getBufferLength_WIFI())      
		writeChar(readChar_WIFI());
	writeChar('\n');
	// Note that here we use writeCommandRaw_WIFI here and not issueCMD, this does not wait for a specific
	// response but only for the echo of the command itself. Then all Text that is received afterwards is written
	// to the serial interface.
	
	
	setLEDs(0b0001);
	
	// Scan for Accesspoints and output results:
	writeCommand_WIFI("scan \r");
										  // This command takes quite some time... 
										  // Can cause large communication delays.
	if(!waitResponse_WIFI("SCAN", 7500000)) { goto wifi_cmd_error; } // Wait for correct response...
	writeChar('\n');
	uint16_t r = 0;
	while(getBufferLength_WIFI()) // Output received Text. 
	{
		uint8_t res = readChar_WIFI();
		writeChar(res);
		response_buffer[r++] = res; // Store to buffer so it can be send back via WIFI as well later. 
	}
	
	setLEDs(0b0000);
	
	// After leaving command mode, all in the Buffer before that is "deleted", 
	// because the routine waits for a specific string and the 
	// whole buffer is read. This is why the response is copied to "response_buffer" above. 
	// This is usually not required, but here we want to output the data
	// back over the WLAN Interface... 
 
	leave_cmd_mode_WIFI(); // Leave command mode, afterwards you can send normal Data again. 
	
	// Output found Accesspoints on WLAN Interface.
	// This MUST be done after command mode has been exited. 
	// Otherwise everything is interpreted by the WLAN Module as commands...
	uint16_t s;
	for(s = 0; r > 0; r--)
		writeChar_WIFI(response_buffer[s++]);
		

	return; // Everything OK, no error. 
	
	// In case of an error, we get here and leave CMD mode. 
	wifi_cmd_error:
		cmd_error_exit_WIFI();
}



/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6M256();    // Always call this first! The Processor will not work
					  // correctly otherwise. 

	initLCD(); // Initialize the LC-Display (LCD)
			   // Always call this before using the LCD!

	writeString_P("RP6 CONTROL M256 WIFI  WLAN COMMAND DEMO!\n"); 
	writeString_P("This message was sent via ***Serial Port***!\n"); 

	writeString_P_WIFI("RP6 CONTROL M256 WIFI  WLAN COMMAND DEMO!\n"); 
	writeString_P_WIFI("This message was sent via ***WIFI INTERFACE***!\n"); 

	setLEDs(0b1111);
	
	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("RP6v2-M256-WIFI ", "<<WLAN CMD>>");
	mSleep(1000); 
	showScreenLCD("WLAN COMMAND", "Example Program");
	setLEDs(0b0000);
	
	
	while(true) 
	{
		writeString_P("***************************************************\n"); 
		writeString_P_WIFI("***************************************************\n"); 
		wlan_scan();
		setLEDs(0b0000);
		mSleep(1000); 
	}
	return 0;
}
