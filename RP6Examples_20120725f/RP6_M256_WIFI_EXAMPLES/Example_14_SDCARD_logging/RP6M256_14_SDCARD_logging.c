/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M256 Examples
 * ****************************************************************************
 * Example: SD Card Example
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * This example logs data to the SD Card and can send the contents of the 
 * logfile to a webbrowser. 
 * 
 * WARNING: You must STOP logging to SD Card before you reset the 
 * Microcontroller or turn power off! For this, press Button SW1. 
 * To start logging again, press Button SW2. 
 *
 * This ensures that no write operation occurs while the power is lost
 * or a reset is performed.   
 *
 *
 * Note: 
 * Please see RP6Lib/RP6control_M256_WIFI/sdc/doc  
 * for detail documentation of the SD Card Library!	
 * 
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
#include "RP6I2CmasterTWI.h"

#include "fat.h"
#include "fat_config.h"
#include "partition.h"

#include "sd-reader_config.h"
#include "sd_raw.h"

#include <string.h>

/*****************************************************************************/
/*****************************************************************************/
// Include the "RP6 Control M256 I2C Master library".

#include "RP6M256_I2CMasterLib.h"  

/*****************************************************************************/

/**
 * Show a running light...
 */
void runningLight(void)
{
	static uint8_t runLight = 1; 
	static uint8_t dir;  
	
	if(getStopwatch1() > 100)
	{
		setLEDs(runLight); 
	
		if(dir == 0)
			runLight <<= 1; 
		else
			runLight >>= 1;
			
		if(runLight > 7 ) 
			dir = 1;			
		else if (runLight < 2 ) 
			dir = 0;
		
		setStopwatch1(0);
	}
}



/**
 * Finds a specific file in a directory.
 */
uint8_t find_file_in_dir(struct fat_fs_struct* fs, struct fat_dir_struct* dd, const char* name, struct fat_dir_entry_struct* dir_entry)
{
    while(fat_read_dir(dd, dir_entry))
    {
        if(strcmp(dir_entry->long_name, name) == 0)
        {
            fat_reset_dir(dd);
            return 1;
        }
    }

    return 0;
}

/**
 * Opens a file in a directory
 */
struct fat_file_struct* open_file_in_dir(struct fat_fs_struct* fs, struct fat_dir_struct* dd, const char* name)
{
    struct fat_dir_entry_struct file_entry;
    if(!find_file_in_dir(fs, dd, name, &file_entry))
        return 0;

    return fat_open_file(fs, &file_entry);
}



/**
 * Append Data to a file on the SD Card. 
 */
void print_file_content(char* filename, uint32_t max_len)
{

	if(!sd_raw_init()) {
		writeString("SD INIT FAILED!\n");
		writeString_P("Please verify that the SD Card is OK and properly inserted!\n");
		showScreenLCD("SD INIT", "FAIL!");
		while(true);
	}

    // open first partition
    struct partition_struct* partition = 
		partition_open(
		sd_raw_read,
        sd_raw_read_interval,
#if SD_RAW_WRITE_SUPPORT
        sd_raw_write,
        sd_raw_write_interval,
#else
        0,
        0,
#endif
        0
        );


        if(!partition) {
			// If the partition did not open, assume the storage device
			// is a "superfloppy", i.e. has no MBR.
    /*        partition = partition_open(sd_raw_read,
                                       sd_raw_read_interval,
#if SD_RAW_WRITE_SUPPORT
                                       sd_raw_write,
                                       sd_raw_write_interval,
#else
                                       0,
                                       0,
#endif
                                       -1
                                      );
            if(!partition) {*/
				writeString_P("Opening partition FAILED!\n");
				writeString_P("Please verify that the SD Card is OK!\n");
				return;
		//	}
        }


        // open file system 
        struct fat_fs_struct* fs = fat_open(partition);
        if(fs) {
            writeString_P("Opening filesystem OK!\n");
        }
		else {
			writeString_P("opening filesystem failed!\n");
			writeString_P("Please verify that the SD Card is OK!\n");
			return;
		}

	
        // open root directory
        struct fat_dir_entry_struct directory;
        fat_get_dir_entry_of_path(fs, "/", &directory);

        struct fat_dir_struct* dd = fat_open_dir(fs, &directory);
        if(!dd) {
			writeString_P("opening root directory failed!\n");
			writeString_P("Please verify that the SD Card is OK!\n");
			return;
		}
		
		writeString_P("\n");
		
		writeString_P("READ:\n");
		
		struct fat_file_struct* fd = open_file_in_dir(fs, dd, filename);
		
		if(!fd) {
			writeString_P("error opening ");
			writeString(filename);
			writeChar('\n');
			return;
		}
		else {
			uint8_t buffer[1024];
			uint32_t offset = 0;
			intptr_t count;
		
			while((count = fat_read_file(fd, buffer, sizeof(buffer))) > 0) {
				// Output the content of the Textfile to both, serial and WIFI:
				for(intptr_t i = 0; i < count; ++i) {
					writeChar_WIFI(buffer[i]);  // write to WIFI Interface only!
					//writeChar(buffer[i]);
				}
				offset += sizeof(buffer);
				mSleep(25); // break to allow network packet transmission.
				if(offset > max_len)
					break;
			}
			fat_close_file(fd);
		}

        fat_close_dir(dd);
        fat_close(fs);
        if(!partition_close(partition))
		{
			writeString_P("PARTITION CLOSE FAILURE!\n");
		}
}


// Variable to show logfile size on the website: 
int32_t logfile_size = 0;

/**
 * Append Data to a file on the SD Card. 
 */
void append_to_file(char* filename, char* data, uint16_t data_len)
{

	if(!sd_raw_init()) {
		writeString("SD INIT FAILED!\n");
		writeString_P("Please verify that the SD Card is OK and properly inserted!\n");
		showScreenLCD("SD INIT", "FAIL!");
		while(true);
	}
	
    // open first partition
    struct partition_struct* partition = 
		partition_open(
		sd_raw_read,
        sd_raw_read_interval,
#if SD_RAW_WRITE_SUPPORT
        sd_raw_write,
        sd_raw_write_interval,
#else
        0,
        0,
#endif
        0
        );


        if(!partition) {
			writeString_P("Opening partition FAILED!\n");
			writeString_P("Please verify that the SD Card is OK!\n");
			return;
        }


        // open file system 
        struct fat_fs_struct* fs = fat_open(partition);
        if(fs) {
            writeString_P("Opening filesystem OK!\n");
        }
		else {
			writeString_P("opening filesystem failed!\n");
			writeString_P("Please verify that the SD Card is OK!\n");
			return;
		}

	
        // open root directory
        struct fat_dir_entry_struct directory;
        fat_get_dir_entry_of_path(fs, "/", &directory);

        struct fat_dir_struct* dd = fat_open_dir(fs, &directory);
        if(!dd) {
			writeString_P("opening root directory failed!\n");
			writeString_P("Please verify that the SD Card is OK!\n");
			return;
		}
		
		writeString_P("\n");
		
		
		// ###############################################################################
		// Create file if it is not already there: 
		
		struct fat_dir_entry_struct file_entry;
		if(!fat_create_file(dd, filename, &file_entry)) {
			writeString_P("error seeking on ");
			writeString(filename);
			writeChar('\n');
		}
		
		#if SD_RAW_WRITE_BUFFERING
		if(!sd_raw_sync())
                writeString_P("error syncing disk\n");
		#endif
		

		// ###############################################################################
		writeString_P("WRITE:\n");
		
		struct fat_file_struct* fd = open_file_in_dir(fs, dd, filename);
		if(!fd)
		{
			writeString_P("error opening ");
			writeString(filename);
			writeChar('\n');
			return;
		}
		else
		{
			int32_t offset = 0;
			if(!fat_seek_file(fd, &offset, FAT_SEEK_END))
			{
				writeString_P("error seeking on ");
				writeString(filename);
				writeChar('\n');

				fat_close_file(fd);
				return;
			}
			
			writeString_P("WRITING to offset: ");
			write32BitInteger(offset,DEC);
			writeChar('\n');
			logfile_size = offset;
			
	
			// write text to file
			if(fat_write_file(fd, (uint8_t*) data, data_len) != data_len)
			{
				writeString_P("error writing to file\n");
			}
			else {
				writeString_P("\nDONE!\n");
			}
			fat_close_file(fd);
		}

		
		#if SD_RAW_WRITE_BUFFERING
		if(!sd_raw_sync())
                writeString_P("error syncing disk\n");
		#endif

	
        fat_close_dir(dd);
        fat_close(fs);
        if(!partition_close(partition))
		{
			writeString_P("PARTITION CLOSE FAILURE!\n");
		}
}


// Small helper functions to copy numbers to strings:  
void writeIntegerToString(char * string, int16_t number, uint8_t base)
{
	char buffer[17];
	itoa(number, &buffer[0], base);
	strcat(string, buffer);
}

void writeIntegerToString_Length(char * string, int16_t number, uint8_t base, uint8_t length)
{
	char buffer[17];
	itoa(number, &buffer[0], base);
	int8_t cnt1 = length - strlen(buffer);
	if(cnt1 > 0) {
		for(; cnt1 > 0; cnt1--, strcat(string, "0"));
		strcat(string, buffer);
	}
	else 
		strncat(string, buffer, length);
}

// Counter for # of logfile entries. Is reset after every reboot!
uint16_t cnt = 0;

/*
 * Gets sensor values and writes a textline with their values to a file on the SD Card. 
 */
void task_log_everything(void)
{	
	if(getStopwatch2() > 1000)
	{
		getAllSensors();		
		
		char logbuf[250];	// Message below is around 170 Chars. 
		logbuf[0] = 0; // Start with a 0 terminated String.

		if(cnt == 0) // Reboot!
		{
			strcat(logbuf, "# System was rebooted here #\n");
		}

		// Copy all info to the logbuf String. 
		strcat(logbuf, "LOG #");  // strcat can concatenate two Strings, in this case
								  // an empty string with the String "TEST: "
		writeIntegerToString_Length(logbuf,cnt++,DEC,5);
		
		strcat(logbuf, " | PL:");writeIntegerToString_Length(logbuf,mleft_power,DEC,3);
		strcat(logbuf, " | PR:");writeIntegerToString_Length(logbuf,mright_power,DEC,3);
		strcat(logbuf, " | VL:");writeIntegerToString_Length(logbuf,mleft_speed,DEC,3);
		strcat(logbuf, " | VR:");writeIntegerToString_Length(logbuf,mright_speed,DEC,3);
		strcat(logbuf, " | DL:");writeIntegerToString_Length(logbuf,mleft_des_speed,DEC,3);
		strcat(logbuf, " | DR:");writeIntegerToString_Length(logbuf,mright_des_speed,DEC,3);
		strcat(logbuf, " | DSTL:");writeIntegerToString_Length(logbuf,mleft_dist,DEC,5);
		strcat(logbuf, " | DSTR:");writeIntegerToString_Length(logbuf,mright_dist,DEC,5);
		strcat(logbuf, " | LSL:");writeIntegerToString_Length(logbuf,adcLSL,DEC,4);
		strcat(logbuf, " | LSR:");writeIntegerToString_Length(logbuf,adcLSR,DEC,4);
		strcat(logbuf, " | MCL:");writeIntegerToString_Length(logbuf,adcMotorCurrentLeft,DEC,4);
		strcat(logbuf, " | MCR:");writeIntegerToString_Length(logbuf,adcMotorCurrentRight,DEC,4);
		strcat(logbuf, " | BAT:");writeIntegerToString_Length(logbuf,adcBat,DEC,4);
		strcat(logbuf, " | AD0:");writeIntegerToString_Length(logbuf,adc0,DEC,4);
		strcat(logbuf, " | AD1:");writeIntegerToString_Length(logbuf,adc1,DEC,4);
		strcat(logbuf, " |\n");
			
		// Show string in serial Terminal:
		writeString(logbuf);
		
		// Save to logfile: 
		append_to_file("M256_log.txt", logbuf, strlen(logbuf));
		setStopwatch2(0);
	}
}


uint8_t counter = 0;

/*
 * The most simple "Webserver" you can think of. 
 * Here it displays a portion of the logfile contents. 
 */
void task_simple_webserver(void)
{
	if(getBufferLength_WIFI()) { 	// Do we have data?					
		if(parseLine_WIFI(readChar_WIFI()))  // Did we receive a full text line?
		{									 // This also stores the text line in receiveBuffer_WIFI
			setLEDs(0b1000);
			writeString_P("Got request: ");
			writeString(receiveBuffer_WIFI);
			
			// Check if the User has clicked on the /stop_log or /start_log links
			if (strstr(receiveBuffer_WIFI,"GET /stop_log")!=NULL) // Check for GET and stop_log
			{
			
				showScreenLCD("Data logging", "STOPPED!");
				stopStopwatch1();
				stopStopwatch2();
				stopStopwatch3();
				writeString_P("\nData logging STOPPED!\n");
			}
			else if (strstr(receiveBuffer_WIFI,"GET /start_log")!=NULL) // Check for GET and start_log
			{
				showScreenLCD("Data logging", "STARTED!");
				startStopwatch1();
				startStopwatch2();
				startStopwatch3();
				writeString_P("\nData logging STARTED!\n");
			}
			
			
			// Note that here is no "else if". It just continues as if the other two 
			// checks above don't exist. 
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
				writeString_P_WIFI("<font size=\"+4\" face=\"sans-serif\">RP6v2-M256-WIFI Data logging to SD Card.</font><br>");  
	

				writeString_P_WIFI("<br><br><font size=\"+3\" face=\"sans-serif\">");
				writeString_P_WIFI("Stopwatch 3 Value: ");
				write32BitInteger_WIFI(getStopwatch3(), DEC); // Function write32BitInteger used to avoid negative display
				writeString_P_WIFI("<br>");
				
		
				writeString_P_WIFI("Logging to SD Card is: ");
				if(isStopwatch2Running())
				{
					writeString_P_WIFI("STARTED!");
					
				}
				else
				{
					writeString_P_WIFI("STOPPED!");
				}
				writeString_P_WIFI("<br>");
				writeString_P_WIFI("<a href=\"/\"\">Refresh view!</a> | ");
				writeString_P_WIFI("<a href=\"start_log\">START Logging to SD Card!</a> | ");
				writeString_P_WIFI("<a href=\"stop_log\">STOP Logging to SD Card!</a>");
				writeString_P_WIFI("<br>");
				writeString_P_WIFI("<br>");
				
				writeString_P_WIFI("Logfile size in Bytes: ");
				write32BitInteger_WIFI(logfile_size, DEC);
				writeString_P_WIFI("<br>");
				
				// Show a counter that displays how often the page has been reloaded
				// (hit RELOAD in your Webbrowser a few times):
				counter++;
				writeString_P_WIFI("Page hit counter: ");
				writeInteger_WIFI(counter, DEC);
				
		
				writeString_P_WIFI("<br><br> Log File Contents:");
				writeString_P_WIFI("</font>");
				writeString_P_WIFI("<br>");
				
				
				// Output the logfile contents:
				writeString_P_WIFI("<pre>");
				mSleep(25); // Delay to allow transmission of the first part of the website... 
				print_file_content("M256_log.txt", 32768); // Output max. the first 32KByte
				mSleep(25);
				writeString_P_WIFI("</pre>");
				
				
				
				writeString_P_WIFI("<br>");
				
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
// I2C Requests: 

/**
 * The I2C_requestedDataReady Event Handler
 */
void I2C_requestedDataReady(uint8_t dataRequestID)
{
	checkRP6Status(dataRequestID);
}

/*****************************************************************************/
// I2C Error handler

/**
 * This function gets called automatically if there was an I2C Error like
 * the slave sent a "not acknowledge" (NACK, error codes e.g. 0x20 or 0x30).
 */
void I2C_transmissionError(uint8_t errorState)
{
	writeString_P("\nI2C ERROR - TWI STATE: 0x");
	writeInteger(errorState, HEX);
	writeChar('\n');
}


/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6M256();
	initLCD(); 
	
	setLEDs(0b1111);
	
	writeString_P("\n\nRP6 CONTROL M256 WIFI SD Card Data Logging Example!\n"); 
	writeString_P("\nCheck WIFI Terminal output!\n"); 
	writeString_P_WIFI("\n\nRP6 CONTROL M256 WIFI SD Card Data Logging Example!\n"); 


	// ---------------------------------------
	I2CTWI_initMaster(100);  
	I2CTWI_setRequestedDataReadyHandler(I2C_requestedDataReady);
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);



	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("<<RP6  Control>>", "<<microSD Card>>");
	mSleep(1000); 
	showScreenLCD("microSD Card", "Logging");
	setLEDs(0b0000);
	
	// Make sure WLAN Module Packet size / buffer size and flush timer is OK
	enter_cmd_mode_WIFI();   
	issueCMD_WIFI("set comm size 1024","AOK"); 
	issueCMD_WIFI("set comm time 10","AOK"); 
	leave_cmd_mode_WIFI();  
	
	
	startStopwatch1();
	startStopwatch2();
	startStopwatch3();
	
	while(true) 
	{
		task_log_everything();
		task_simple_webserver();
		runningLight();
		
		// This can be used to stop logging data to the disk. 
		// If you turn off power or perform a reset while data is written to the disk, 
		// the file system may get corrupted so it is always a good idea to 
		// stop logging before this. 
		uint8_t key = checkPressedKeyEvent(); 
		if(key == 1)
		{
			writeString_P_WIFI("SW1 was pressed and released!\n"); 
			showScreenLCD("Data logging", "STOPPED!");
			stopStopwatch1();
			stopStopwatch2();
			stopStopwatch3();
		}
		if(key == 2)
		{
			writeString_P_WIFI("SW1 was pressed and released!\n"); 
			showScreenLCD("Data logging", "STARTED!");
			startStopwatch1();
			startStopwatch2();
			startStopwatch3();
		}
	}
	return 0;
}
