/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M256 Examples
 * ****************************************************************************
 * Example: Simple Webserver Example
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:  
 * In this Demo we show the most simple "webserver" you can think of. 
 * It is of course not comparable with a real webserver, it only responds to
 * a GET Command and can not do anything else. It only accepts ONE connection
 * at a time, any more connections will fail. 
 *   
 * However it is sufficient for displaying some status information and to 
 * show you the general idea of using the WIFI Module to communicate
 * with standard PC Software.
 *
 * In this example it only displays some text, the current stopwatch value
 * and a page hit counter. So the page shows dynamic info every time you
 * reload it (click refresh in your browser!). 
 *
 * To view the Page, enter <ip>:<port> in your webbrowser.
 * For example: http://192.168.10.171:2000
 * (http:// can be omitted for most modern Browsers)
 *
 * YOU MUST configure the WIFI Module to NOT SEND *HELLO* Message
 * when opening a new connection! This is the typical configuration
 * when delivered, but may be set to something different after a 
 * factory reset!
 * Otherwise the Browser will not display the page correctly.
 * Enter WIFI Command mode and perform command
 * set comm remote 0
 * to disable the opening message. 
 *
 * ##################################################################
 * ##################################################################
 *
 * YOU MUST CLOSE THE NETWORK CONNECTION IN ROBOTLOADER BEFORE
 * YOU CAN CONNECT WITH THE WEB BROWSER!
 *
 * If you do not do this, you will not be able to get a connection
 * with your Webbrowser!
 * You can watch the Debug output on the serial interface. 
 *
 * ##################################################################
 * ##################################################################
 *
 * Note: 
 * Multiple connections may be possible in the future with a Firmware
 * update of the WIFI Module and a Bootloader update (both can be done
 * with RobotLoader). 
 *
 * Hint: 
 * To speed up closing the current connection you can use RobotLoader
 * WIFI config to enable usage of GPIO14 for entering the CMD Mode. 
 * The raw Command used for this is
 * set comm cmd-GPIO 14  
 *
 * This enables command mode using a GPIO instead of waiting 260ms, send $$$
 * over UART and wait another 260ms for the correct response.
 * This was introduced with Firmware 2.32, any earlier Firmware needs to
 * be updated to support this. 
 
 * ############################################################################
 * The Robot does NOT move in this example! You can simply put it on a table
 * next to your PC and you should connect it to the PC via the USB Interface!
 *
 * Connection via Webbrowser is possible if connection in RobotLoader is closed.
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6M256Lib.h" 

/*****************************************************************************/


uint8_t counter = 0;

/*
 * The most simple "Webserver" you can think of. 
 */
void task_simple_webserver(void)
{
	if(getBufferLength_WIFI()) { 	// Do we have data?					
		if(parseLine_WIFI(readChar_WIFI()))  // Did we receive a full text line?
		{									 // This also stores the text line in receiveBuffer_WIFI
			setLEDs(0b1000);
			writeString("Got request: ");
			writeString(receiveBuffer_WIFI);
			if (strstr(receiveBuffer_WIFI,"GET /")!=NULL) // Check for GET Command
			{
				writeString("\nGOT GET!\n");
				setLEDs(0b1100);
				mSleep(50);
				
				
				
				// ***************************************************************** 
				// The content: 
				
				// Send HTTP Header:
				writeString_P_WIFI("HTTP/1.0 200 OK\r\n"); 
				writeString_P_WIFI("Content-Type: text/html\r\n"); 
				writeString_P_WIFI("\r\n");
				
				// Send HTML Content:
				writeString_P_WIFI("<html><head><title>RP6v2-M256-WIFI Simple Webserver</title></head><body>");  
				writeString_P_WIFI("<font size=\"+4\" face=\"sans-serif\">HELLO FROM RP6v2-M256-WIFI!</font><br>");  
				writeString_P_WIFI("Please visit my homepage: ");
				writeString_P_WIFI("<a href=\"http://www.arexx.com/rp6\">www.arexx.com/rp6</a>");
				
				// Show 
				writeString_P_WIFI("<br><br><font size=\"+3\" face=\"sans-serif\">");
				writeString_P_WIFI("Stopwatch 1 Value: ");
				writeInteger_WIFI(getStopwatch1(), DEC);
				writeString_P_WIFI("<br>");
				
				// Show a counter that displays how often the page has been reloaded
				// (hit RELOAD in your Webbrowser a few times):
				counter++;
				writeString_P_WIFI("Page hit counter: ");
				writeInteger_WIFI(counter, DEC);
				uint8_t cnt; 
				for(cnt = 0; cnt < counter; cnt++)
				{
					if(cnt % 8 == 0)
						writeString_P_WIFI("<br>");
					writeString_P_WIFI("*");  
				}
				
				writeString_P_WIFI("</font>");
				writeString_P_WIFI("</body></html>");  
				
				// ***************************************************************** 
				
				
				
				// Wait a bit and then close connection:
				setLEDs(0b0011);
				mSleep(100);  // Depending on the content you may have to wait longer. 
				
				writeString("\nClose connection... ");
				enter_cmd_mode_WIFI();
				writeCommand_WIFI("close\r");
				leave_cmd_mode_WIFI();
				writeString("done!\n");
				setLEDs(0b0000);
			}
			else {
				writeString_P_WIFI("\nI am simple. I don't know.\n");
				writeString_P("\nI am simple. I don't know.\n");
				writeString("\nClose connection... ");
				enter_cmd_mode_WIFI();
				writeCommand_WIFI("close\r");
				leave_cmd_mode_WIFI();
				writeString("done!\n");
				setLEDs(0b0000);
			}
		}
	}
}



/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6M256(); 
	initLCD();
   
	writeString_P("\n\nSimple Webserver Example...\n"); 
	writeString_P_WIFI("\n\nSimple Webserver Example...\n"); 

	setLEDs(0b1111);
	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("WIFI DEMO", "Webserver");
	mSleep(500);
	setLEDs(0b0000);
	
	startStopwatch1();
	
	// Make sure WLAN Module Packet size / buffer size and flush timer is OK
	enter_cmd_mode_WIFI();   
	issueCMD_WIFI("set comm size 1024","AOK"); 
	issueCMD_WIFI("set comm time 10","AOK"); 
	leave_cmd_mode_WIFI();  
	
	while(true) 
	{ 
		task_simple_webserver();
		if(getStopwatch1() > 10000) // Just to have some dynamic data to display... 
			setStopwatch1(0);
	}
	return 0;
}

