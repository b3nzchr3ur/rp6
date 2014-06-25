/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M256 Examples
 * ****************************************************************************
 * Example: M256 WIFI SELFTEST
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * Test program for RP6 Control M256 WIFI. You can use it to verify that
 * everything works fine. 
 * For a full test you also need to run the Test Program in RobotLoader
 * which will check the Network Connection etc. 
 *
 * Just as the RP6Base_SELFTEST, also this program is not meant to be nice,
 * but a (relatively) large space filler...
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


#include "RP6M256Lib.h"   		// The RP6 Control Library. 
							   	// Always needs to be included!

#include "RP6I2CmasterTWI.h"	// I2C Master Library


//#define DEBUG 1


#include "fat.h"
#include "fat_config.h"
#include "partition.h"

#include "sd-reader_config.h"
#include "sd_raw.h"


/*****************************************************************************/

#define I2C_RP6_BASE_ADR 10		// The default address of the Slave Controller 
								// on the RP6 Mainboard


uint8_t errors = 0;

char recBuffer[UART_RECEIVE_BUFFER_SIZE];

void done(void)
{
	writeString_P("Done!\n"); 
	writeString_P_WIFI("Done!\n"); 
}

void bars(uint8_t number)
{
	uint8_t cnt;
	writeChar('\n');
	writeChar_WIFI('\n');
	for(;number > 0; writeChar('\n'), writeChar_WIFI('\n'), number--)
		for(cnt = 69; cnt ; cnt--) {
			writeChar('#');
			writeChar_WIFI('#');
		}
}

void test(uint8_t number)
{
	bars(2);
	writeString_P("#### TEST #");
	writeInteger(number, DEC);
	writeString_P(" ####\n");
	
	writeString_P_WIFI("#### TEST #");
	writeInteger_WIFI(number, DEC);
	writeString_P_WIFI(" ####\n");
}


void test_error(uint8_t error)
{
	bars(4);
	writeString_P("\nERROR ERROR ERROR!!!\n");
	writeString_P("CODE:");
	writeInteger(error, DEC);
	writeString_P("\n");
	
	writeString_P_WIFI("\nERROR ERROR ERROR!!!\n");
	writeString_P_WIFI("CODE:");
	writeInteger_WIFI(error, DEC);
	writeString_P_WIFI(" \n");
	
	bars(4);
}

/*****************************************************************************/
//


uint8_t getInputLine_WIFI(void)
{ 
	if(getBufferLength_WIFI()) 
	{							
		uint8_t data = readChar_WIFI();

		return parseLine_WIFI(data);
	}
	return 0;
	
}


void enterString_WIFI(void)
{
	while(!getInputLine_WIFI());
}




/*****************************************************************************/

/**
 * This function gets called automatically if there was an I2C Error like
 * the slave sent a "not acknowledge" (NACK, error codes e.g. 0x20 or 0x30).
 */
void I2C_transmissionError(uint8_t errorState)
{
	writeString_P("\n############ I2C ERROR!!!!! - TWI STATE: 0x");
	writeInteger(errorState, HEX);
	writeChar('\n');
	
	writeString_P_WIFI("\n############ I2C ERROR!!!!! - TWI STATE: 0x");
	writeInteger_WIFI(errorState, HEX);
	writeChar_WIFI('\n');
	
	errors++;
}


/*****************************************************************************/
// SDCARD: 


uint8_t print_disk_info(const struct fat_fs_struct* fs)
{
    if(!fs)
        return 0;

    struct sd_raw_info disk_info;
    if(!sd_raw_get_info(&disk_info))
        return 0;


    writeString_P_WIFI("manuf:  0x"); writeInteger_WIFI(disk_info.manufacturer,HEX); writeChar_WIFI('\n');
    writeString_P_WIFI("oem:    "); writeString_WIFI((char*) disk_info.oem); writeChar_WIFI('\n');
    writeString_P_WIFI("prod:   "); writeString_WIFI((char*) disk_info.product); writeChar_WIFI('\n');
    writeString_P_WIFI("rev:    "); writeInteger_WIFI(disk_info.revision,DEC); writeChar_WIFI('\n');
    writeString_P_WIFI("serial: 0x"); writeInteger_WIFI(disk_info.serial, HEX); writeChar_WIFI('\n');
    writeString_P_WIFI("date:   "); writeInteger_WIFI(disk_info.manufacturing_month, DEC); writeChar_WIFI('/');
                                   writeInteger_WIFI(disk_info.manufacturing_year, DEC); writeChar_WIFI('\n');
    writeString_P_WIFI("size:   "); writeInteger_WIFI(disk_info.capacity / 1024 / 1024, DEC);  writeString_P_WIFI("MB\n");
    writeString_P_WIFI("copy:   "); writeInteger_WIFI(disk_info.flag_copy,DEC); writeChar_WIFI('\n');
    writeString_P_WIFI("wr.pr.: "); writeInteger_WIFI(disk_info.flag_write_protect_temp,DEC); writeChar_WIFI('/');
                                   writeInteger_WIFI(disk_info.flag_write_protect,DEC); writeChar_WIFI('\n');
    writeString_P_WIFI("format: "); writeInteger_WIFI(disk_info.format,DEC); writeChar_WIFI('\n');
  
  // uncommented, can take quite long with SDHC Cards, but this test needs to be fast.
  //  writeString_P_WIFI("free:   "); writeInteger_WIFI(fat_get_fs_free(fs),DEC); writeChar_WIFI('/');
  //                                 writeInteger_WIFI(fat_get_fs_size(fs),DEC); writeChar_WIFI('\n');
	

    return 1;
}



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

struct fat_file_struct* open_file_in_dir(struct fat_fs_struct* fs, struct fat_dir_struct* dd, const char* name)
{
    struct fat_dir_entry_struct file_entry;
    if(!find_file_in_dir(fs, dd, name, &file_entry))
        return 0;

    return fat_open_file(fs, &file_entry);
}


void test_sdcard(void)
{

	if(sd_raw_init())
	{
		writeString_WIFI("SD INIT OK!\n"); 
	}
	else
	{
		writeString_WIFI("SD INIT FAILED!\n");
		writeString_P_WIFI("Please verify that the SD Card is OK!\n");
		errors++;
		return;
	}
	
	
	mSleep(100);
	setLEDs(0b0000);

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

        if(!partition)
        {
            /* If the partition did not open, assume the storage device
             * is a "superfloppy", i.e. has no MBR.
             */
            partition = partition_open(sd_raw_read,
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
            if(partition)
            {
                writeString_P_WIFI("Opening partition OK!\n");
            }
			else {
				writeString_P_WIFI("Opening partition FAILED!\n");
				writeString_P_WIFI("Please verify that the SD Card is OK!\n");
				errors++;
				return;
			}
        }

        // open file system 
        struct fat_fs_struct* fs = fat_open(partition);
        if(fs)
        {
            writeString_P_WIFI("opening filesystem OK!\n");
        }
		else {
			writeString_P_WIFI("opening filesystem failed!\n");
			writeString_P_WIFI("Please verify that the SD Card is OK!\n");
			errors++;
			return;
		}
		
        // open root directory
        struct fat_dir_entry_struct directory;
        fat_get_dir_entry_of_path(fs, "/", &directory);

        struct fat_dir_struct* dd = fat_open_dir(fs, &directory);
        if(dd)
        {
            writeString_P_WIFI("opening root directory OK!\n");

        }
		else {
			writeString_P_WIFI("opening root directory failed!\n");
			writeString_P_WIFI("Please verify that the SD Card is OK!\n");
			errors++;
			return;
		}

        // print some card information as a boot message
        print_disk_info(fs);
		
		// print directory listing
		struct fat_dir_entry_struct dir_entry;
		while(fat_read_dir(dd, &dir_entry))
		{
			uint8_t spaces = sizeof(dir_entry.long_name) - strlen(dir_entry.long_name) + 4;

			writeString_WIFI(dir_entry.long_name);
			writeChar_WIFI(dir_entry.attributes & FAT_ATTRIB_DIR ? '/' : ' ');
			while(spaces--)
				writeChar_WIFI(' ');
			writeInteger_WIFI(dir_entry.file_size,DEC);
			writeChar_WIFI('\n');
		}
		
		writeString_P_WIFI("\n\n");
		
		char* command = "M256_SELFTEST_TESTFILE.txt";
		
		struct fat_file_struct* fd = open_file_in_dir(fs, dd, command);
		if(!fd)
		{
			writeString_P_WIFI("error opening ");
			writeString_WIFI(command);
			writeChar_WIFI('\n');
			
			writeString_P_WIFI("Please verify that the Testfile is\n");
			writeString_P_WIFI("correctly copied on the SD Card!\n\n");
			errors++;
			return;
			
		}
		else
		{
			uint8_t buffer[160];
			uint32_t offset = 0;
			intptr_t count;
		
			while((count = fat_read_file(fd, buffer, sizeof(buffer))) > 0)
			{
				if(offset == 0) {
					char* ptr = strstr(&buffer[0], "RP6 M256");
					if(strncmp(ptr, "RP6 M256 SD CARD Testfile 01234567890", 37) != 0) {
						errors++;
						
						writeString_P("\n\n *** ERROR WHILE CHECKING THE TESTFILE! ***\n");
						writeString_P("Test Strings do not match!\n");
						writeString_P("Please verify that the Testfile is\n");
						writeString_P("correctly copied on the SD Card!\n\n");
						
						writeString_P_WIFI("\n\n *** ERROR WHILE CHECKING THE TESTFILE! ***\n");
						writeString_P_WIFI("Test Strings do not match!\n");
						writeString_P_WIFI("Please verify that the Testfile is\n");
						writeString_P_WIFI("correctly copied on the SD Card!\n\n");
						return;
					}
				}
				
				for(intptr_t i = 0; i < count; ++i)
				{
					writeChar_WIFI(buffer[i]);
					writeChar(buffer[i]);
				}
				offset += sizeof(buffer);
				if(offset % 128 == 0)
					mSleep(1);
			}
			
			fat_close_file(fd);
		}

	
        fat_close_dir(dd);
        fat_close(fs);
        partition_close(partition);
		
		mSleep(500);
}


void errorcheck(void)
{

	if(errors)
	{
		bars(4);
		writeString_P("\nERROR ERROR ERROR ERROR ERROR ERROR ERROR\n");
		writeString_P("\nATTENTION: TESTS FINISHED WITH ERRORS!!!\n");
		writeString_P("PLEASE CHECK RP6-M256 ASSEMBLY!!!\n\n");
		
		writeString_P_WIFI("\nERROR ERROR ERROR ERROR ERROR ERROR ERROR\n");
		writeString_P_WIFI("\nATTENTION: TESTS FINISHED WITH ERRORS!!!\n");
		writeString_P_WIFI("PLEASE CHECK RP6-M256 ASSEMBLY!!!\n <<<TEST FAIL, CODE ---AB5OP793FE4R30IQ931M--->>> \n");
		
		bars(4);
		writeString_P("\n\n");
		writeString_P_WIFI("\n\n");
		
		showScreenLCD("## TEST ERROR ##", "CHECK MESSAGES!");
		
		while(true) 
		{
			setLEDs(0b1111); 
			mSleep(250);
			setLEDs(0b0000); 
			mSleep(250);
		}
	}
}





char response_buffer[UART_RECEIVE_BUFFER_SIZE_WIFI+1];


void wlan_scan(void)
{
	writeString_P("\n### WIFI SCAN\n"); 
	
	setLEDs(0b0000);
	enter_cmd_mode_WIFI();
	writeString(receiveBuffer_WIFI);
	writeChar('\n');
	
	setLEDs(0b1000);
	issueCMD_WIFI("ver","WiFly");
	writeString(receiveBuffer_WIFI);
	writeChar('\n');

	setLEDs(0b1100);
	issueCMD_WIFI("get mac","Mac");
	writeString(receiveBuffer_WIFI);
	writeChar('\n');
	
	setLEDs(0b0110);
	issueCMD_WIFI("show rssi","RSSI");
	writeString(receiveBuffer_WIFI);
	writeChar('\n');
	
	setLEDs(0b0011);
	writeString_P_WIFI("show status\r");
	while(getBufferLength_WIFI())      
		writeChar(readChar_WIFI());
	writeChar('\n');
	
	
	setLEDs(0b0001);
	
	writeString_P_WIFI("scan \r");
	if(!waitResponse_WIFI("SCAN", 7500000)) { goto wifi_cmd_error; }
	writeChar('\n');
	uint16_t r = 0;
	while(getBufferLength_WIFI()) {
		uint8_t res = readChar_WIFI();
		writeChar(res);
		response_buffer[r++] = res;
	}
	
	setLEDs(0b0000);
	
 
	leave_cmd_mode_WIFI();

	uint16_t s;
	for(s = 0; r > 0; r--)
		writeChar_WIFI(response_buffer[s++]);
		
	writeChar('\n');
	writeChar_WIFI('\n');	

	return; 
	
	wifi_cmd_error:
		cmd_error_exit_WIFI();
}



void wlan_info(void)
{
	writeString_P("\n### WIFI INFO\n"); 
	
	setLEDs(0b0000);
	enter_cmd_mode_WIFI();
	writeString(receiveBuffer_WIFI);
	writeChar('\n');
	
	setLEDs(0b1000);
	issueCMD_WIFI("ver","WiFly");
	writeString(receiveBuffer_WIFI);
	writeChar('\n');

	setLEDs(0b1100);
	issueCMD_WIFI("get mac","Mac");
	writeString(receiveBuffer_WIFI);
	writeChar('\n');
	
	setLEDs(0b0110);
	issueCMD_WIFI("show rssi","RSSI");
	writeString(receiveBuffer_WIFI);
	writeChar('\n');
	
	setLEDs(0b0110);
	writeString_P_WIFI("show connection\r");
	while(getBufferLength_WIFI())      
		writeChar(readChar_WIFI());
	writeChar('\n');
	
	setLEDs(0b0011);
	writeString_P_WIFI("show stats\r");
	while(getBufferLength_WIFI())      
		writeChar(readChar_WIFI());
	writeChar('\n');
	
	setLEDs(0b0011);
	writeString_P_WIFI("show net\r");
	while(getBufferLength_WIFI())      
		writeChar(readChar_WIFI());
	writeChar('\n');
	
	setLEDs(0b0011);
	writeString_P_WIFI("show io\r");
	while(getBufferLength_WIFI())      
		writeChar(readChar_WIFI());
	writeChar('\n');


	setLEDs(0b1100);
	writeString_P_WIFI("get everything\r");
	while(getBufferLength_WIFI())      
		writeChar(readChar_WIFI());
	writeChar('\n');
	
	setLEDs(0b1000);
	
 
	leave_cmd_mode_WIFI();
		
	writeChar('\n');
	writeChar_WIFI('\n');	
	
	setLEDs(0b0000);
	return; 
	
}



void run_test_buttons(void)
{
	showScreenLCD("Testing", "Button SW1");
	setLEDs(0b0001);
	writeString_P("\n\nNow testing Buttons.\n"); 
	writeString_P("\nPlease press Button SW1!\n"); 
	
	writeString_P_WIFI("\n\nNow testing Buttons.\n"); 
	writeString_P_WIFI("\nPlease press Button SW1!\n"); 

	
	while(true)
	{
		if (! (PING & BUTTON_SW1)) {
			mSleep(1);
			if((PING & BUTTON_SW1)) {continue; };
			mSleep(1);
			if((PING & BUTTON_SW1)) {continue; };
			if(! (PING & BUTTON_SW1)) {
				setLEDs(0b1111);
				writeString_P("\n\nButton 1 pressed... relase!\n"); 
				writeString_P_WIFI("\n\nButton 1 pressed... relase!\n"); 
				while(!(PING & BUTTON_SW1)){}
				writeString_P("Button 1 relased!\n"); 
				writeString_P_WIFI("Button 1 relased!\n"); 
				break;
			}
		}
	}
	
	setLEDs(0b1000);

	showScreenLCD("Testing", "Button SW2");

	writeString_P("\nPlease press Button SW2!\n"); 
	writeString_P_WIFI("\nPlease press Button SW2!\n"); 
	
	
	while(true)
	{
		if (! (PING & BUTTON_SW2)) {
			mSleep(1);
			if((PING & BUTTON_SW2)) {continue; };
			mSleep(1);
			if((PING & BUTTON_SW2)) {continue; };
			if(! (PING & BUTTON_SW2)) {
				setLEDs(0b1111);
				writeString_P("\n\nButton 2 pressed... relase!\n"); 
				writeString_P_WIFI("\n\nButton 2 pressed... relase!\n"); 
				while(!(PING & BUTTON_SW1)){}
				writeString_P("Button 2 relased!\n"); 
				writeString_P_WIFI("Button 2 relased!\n"); 
				break;
			}
		}
	}
	
	
	writeString_P("\nButtons are OK!\n"); 
	writeString_P_WIFI("\nButtons are OK!\n"); 
}

void run_test_leds(void)
{

	showScreenLCD("Testing", "LEDS");
	setLEDs(0b0000);mSleep(200);
	setLEDs(0b0001);mSleep(200);
	setLEDs(0b0010);mSleep(200);
	setLEDs(0b0100);mSleep(200);
	setLEDs(0b1000);mSleep(200);
	setLEDs(0b1001);mSleep(200);
	setLEDs(0b1011);mSleep(200);
	setLEDs(0b1111);mSleep(200);
	mSleep(500);
	setLEDs(0b0000);
}

void run_test_i2c(void)
{
	writeString_P_WIFI("\n\nI2C Test...\n\n"); 
	writeString_P("\nI2C TWI TEST...\n");
	showScreenLCD("Testing", "I2C TWI");
	
	
	mSleep(100);

	uint8_t runningLight = 1;
	uint8_t cnt = 0;
	for(cnt = 0; cnt < 24; cnt++)
	{
		writeIntegerLength(cnt,DEC,3);
		writeChar(':');
		writeIntegerLength(runningLight,DEC,3);
		writeChar(',');
		writeChar(' ');
		
		writeIntegerLength_WIFI(cnt,DEC,3);
		writeChar_WIFI(':');
		writeIntegerLength_WIFI(runningLight,DEC,3);
		writeChar_WIFI(',');
		writeChar_WIFI(' ');
		

		I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, 3, runningLight);
		I2CTWI_transmitByte(I2C_RP6_BASE_ADR, 29);
		uint8_t result = I2CTWI_readByte(I2C_RP6_BASE_ADR);
		if(result != runningLight) 
		{
			writeString_P("\nTWI TEST ERROR!\n");
			writeString_P_WIFI("\nTWI TEST ERROR!\n");
			errors++;
		}
		runningLight <<= 1; 
		if(runningLight > 32) 
			runningLight = 1;
	
		if((cnt+1) % 6 == 0) {
			writeChar('\n');
			writeChar_WIFI('\n');
		}
		mSleep(100);
	}
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, 3, 0);
	
	writeString_P_WIFI("\nI2C TEST DONE!\n");
	writeString_P("\nI2C TEST DONE!\n");
}


void run_test_sd_card(void)
{
	writeString_P_WIFI("\n\nSDCARD Test...\n\n"); 
	writeString_P("\nSDCARD TEST...\n");
	showScreenLCD("Testing", "SDCARD");
	
	test_sdcard();
	
	mSleep(400);
	setLEDs(0b0000);
	writeString_P("SDCARD TEST DONE!\n");
	writeString_P_WIFI("SDCARD TEST DONE!\n");
}


uint8_t convert_asciihex_to_int(uint8_t character)
{
	if(character == 'a')
		return 10;
	else if(character == 'b')
		return 11;
	else if(character == 'c')
		return 12;
	else if(character == 'd')
		return 13;
	else if(character == 'e')
		return 14;
	else if(character == 'f')
		return 15;
	return character - 48;
}


//#define FLOW_CONTROL_IS_DISABLED


void test_wlan_io_ports(void)
{
	writeString_P_WIFI("\n\n+++ Now Testing WLAN Module I/O Ports +++\n\n"); 
	writeString_P_WIFI("\n\nTesting M256 CTS (PINJ7) <<-- WLAN RTS...\n\n"); 
	writeString_P("\n\nTesting M256 CTS (PINJ7) <<-- WLAN RTS...\n\n"); 
	
#ifdef FLOW_CONTROL_IS_DISABLED
		writeString_P("\n\nM256 CTS test no flow control\n\n"); 
	
#else
	
	uint8_t cts_fail = 0;
	uint16_t cnt = 0;
	if(PINJ & WLAN_CTS) 
	{
		writeString_P("\n\nM256 CTS high\n\n"); 
		while(PINJ & WLAN_CTS && cnt < 100)
		{
			mSleep(20);
			cnt++;
		}
		if(cnt >= 100) {
			writeString_P("\n\n*** M256 CTS did not go low within 2 seconds!\n\n"); 
			errors++;
			cts_fail = 1;
		}
	}
	else
	{
		writeString_P("\n\nM256 CTS low\n\n"); 
		cts_fail = 0;
	}

	cts_active = 0;
	cnt = 0;
	
	mSleep(250);
	
	// send chars:
	while(!cts_active && cnt < 200) {
		writeString_P_WIFI("##########");
		writeString_P_WIFI("**********");
		writeString_P_WIFI("++++++++++");
		writeString_P_WIFI("----------");
		writeString_P_WIFI("++++++++++");
		writeString_P_WIFI("**********");
		writeString_P_WIFI("##########\n");
		
		cnt++;
		if(cnt % 20 == 0) mSleep(50);
	}
	mSleep(250);
	if(cts_active && cts_fail == 0) {
		writeString_P_WIFI("\n\nOK, M256 CTS seems to be working fine.\n");
		writeString_P("\n\nOK, M256 CTS seems to be working fine.\n");  
	}
	else {
		writeString_P_WIFI("\n\n*** ERROR M256 CTS DOES NOT WORK PROPERLY! TRY AGAIN!\n\n"); 
		writeString_P("\n\n*** ERROR M256 CTS DOES NOT WORK PROPERLY! TRY AGAIN!\n"); 
		writeString_P_WIFI("*** Test M256 CTS --> FAIL! ############\n\n");
		writeString_P_WIFI("*** Make sure flow control is enabled!\n\n");
		errors++;
	}
	
#endif
	
	writeString_P_WIFI("Now testing the other GPIOs please wait...\n");
	

	
	
	uint8_t failure_code1 = 0;
	uint8_t failure_code2 = 0;
	uint8_t failure_code3 = 0;
	
	
	
	setLEDs(0b0000);
	enter_cmd_mode_WIFI();
	writeString(receiveBuffer_WIFI);
	writeChar('\n');
	
	
	setLEDs(0b1000);
	issueCMD_WIFI("ver","WiFly");
	writeString(receiveBuffer_WIFI);
	writeChar('\n');


#ifdef FLOW_CONTROL_IS_DISABLED
	writeString_P("\n\nM256 RTS test no flow control\n\n"); 
	
	//PORTC |= WLAN_GPIO14_OUT;
	
	PORTJ &= ~WLAN_RTS;
	mSleep(50);

	issueCMD_no_resp_WIFI("show io");
	while(getBufferLength_WIFI()) {
		if(parseLine_WIFI(readChar_WIFI())) {
			if(buffer_length_WIFI>=3) {
				uint8_t num1 = convert_asciihex_to_int(receiveBuffer_WIFI[0]);
				uint8_t num2 = convert_asciihex_to_int(receiveBuffer_WIFI[1]);
				uint8_t num3 = convert_asciihex_to_int(receiveBuffer_WIFI[2]);
				uint8_t num4 = convert_asciihex_to_int(receiveBuffer_WIFI[3]);
				
				writeString_P("NUM: 0b");
				writeIntegerLength(num1,BIN,4);
				writeIntegerLength(num2,BIN,4);
				writeIntegerLength(num3,BIN,4);
				writeIntegerLength(num4,BIN,4);
				writeString_P("\n");
		
				if(num1 & 0b0010) {
					writeString_P("*** Test WLAN RTS LOW --> FAIL! ############\n");
					errors++;
					failure_code2 = 16;
					
				}
				else {
					writeString_P("Test WLAN RTS LOW --> OK!\n");
				}
			}
			writeString_P(" [RESP:");
			writeString(receiveBuffer_WIFI);
			writeString_P("]\n");
		}
	}
	
	PORTJ |= WLAN_RTS;
	mSleep(50);

	issueCMD_no_resp_WIFI("show io");
	while(getBufferLength_WIFI()) {
		if(parseLine_WIFI(readChar_WIFI())) {
			if(buffer_length_WIFI>=3) {
				uint8_t num1 = convert_asciihex_to_int(receiveBuffer_WIFI[0]);
				uint8_t num2 = convert_asciihex_to_int(receiveBuffer_WIFI[1]);
				uint8_t num3 = convert_asciihex_to_int(receiveBuffer_WIFI[2]);
				uint8_t num4 = convert_asciihex_to_int(receiveBuffer_WIFI[3]);
				
				writeString_P("NUM: 0b");
				writeIntegerLength(num1,BIN,4);
				writeIntegerLength(num2,BIN,4);
				writeIntegerLength(num3,BIN,4);
				writeIntegerLength(num4,BIN,4);
				writeString_P("\n");
		
				if(num1 & 0b0010) {
					writeString_P("Test WLAN RTS HIGH --> OK!\n");
				}
				else {
					writeString_P("*** Test WLAN RTS HIGH --> FAIL! ############\n");
					errors++;
					failure_code2 = 16;
				}
			}
			writeString_P(" [RESP:");
			writeString(receiveBuffer_WIFI);
			writeString_P("]\n");
		}
	}
	
	
#else


	clearReceptionBuffer_WIFI();
	
	writeString_P("\n\nTesting M256 RTS (PINJ2) -->> WLAN CTS...\n\n"); 
	PORTJ &= ~WLAN_RTS;
	mSleep(20);
	
	writeString_P("\nCheck 1: "); 
	writeString_P_WIFI("ver\r");
	if(!waitResponse_WIFI("WiFly", 700000)) 
	{ 
		writeString_P("*** NO RESPONSE WHILE RTS LOW! ERROR!"); 
		failure_code1 = 4;
		errors++;
	}
	else
	{
		writeString_P("Got response while RTS low --> OK!"); 
	}

	clearReceptionBuffer_WIFI();
	
	PORTJ |= WLAN_RTS;
	mSleep(20);
	
	writeString_P("\nCheck 2: "); 
	writeString_P_WIFI("ver\r");
	if(!waitResponse_WIFI("WiFly", 700000)) 
	{ 
		writeString_P("NO response while RTS high --> OK!"); 
	}
	else
	{
		writeString_P("*** GOT RESPONSE WHILE RTS HIGH! ERROR!"); 
		failure_code1 = 5;
		errors++;
	}
	
	clearReceptionBuffer_WIFI();
	
	PORTJ &= ~WLAN_RTS;
	mSleep(20);
	
	writeString_P("\nCheck 3: "); 
	writeString_P_WIFI("ver\r"); 
	if(!waitResponse_WIFI("WiFly", 700000)) 
	{ 
		writeString_P("*** NO RESPONSE WHILE RTS LOW! ERROR!"); 
		failure_code1 = 6;
		errors++;
	}
	else
	{
		writeString_P("Got response while RTS low --> OK!"); 
	}
	
	clearReceptionBuffer_WIFI();
	
	PORTJ |= WLAN_RTS;
	mSleep(50);
	
	writeString_P("\nCheck 4: "); 
	writeString_P_WIFI("ver\r");
	if(!waitResponse_WIFI("WiFly", 700000)) 
	{ 
		writeString_P("NO response while RTS high --> OK!"); 
	}
	else
	{
		writeString_P("*** GOT RESPONSE WHILE RTS HIGH! ERROR!"); 
		failure_code1 = 7;
		errors++;
	}
	
	clearReceptionBuffer_WIFI();
	
	
	
#endif

	
	
	mSleep(50);


	writeString_P("\n\nTesting M256 GPIO14 (PINC6) -->> WLAN GPIO14...\n\n"); 
	
	PORTC |= WLAN_GPIO14_OUT;
	mSleep(50);

	writeCommand_WIFI("show io");
	while(getBufferLength_WIFI()) {
		if(parseLine_WIFI(readChar_WIFI())) {
			if(buffer_length_WIFI>=3) {
				uint8_t num1 = convert_asciihex_to_int(receiveBuffer_WIFI[0]);

				if(num1 & 0b0100) {
					writeString_P("Test GPIO14  HIGH --> OK!\n");
				}
				else {
					writeString_P("*** Test GPIO14  HIGH --> FAIL! ############\n");
					errors++;
					failure_code2 = 14;
				}
			}
		}
	}
	
	PORTC &= ~WLAN_GPIO14_OUT;
	mSleep(50);

	writeCommand_WIFI("show io");
	while(getBufferLength_WIFI()) {
		if(parseLine_WIFI(readChar_WIFI())) {
			if(buffer_length_WIFI>=3) {
				uint8_t num1 = convert_asciihex_to_int(receiveBuffer_WIFI[0]);
				
				if(!(num1 & 0b0100)) {
					writeString_P("Test GPIO14  LOW --> OK!\n");
				}
				else {
					writeString_P("*** Test GPIO14 LOW --> FAIL! ############\n");
					errors++;
					failure_code2 = 15;
				}
			}
		}
	}

	PORTC |= WLAN_GPIO14_OUT;
	mSleep(50);

	writeCommand_WIFI("show io");
	while(getBufferLength_WIFI()) {
		if(parseLine_WIFI(readChar_WIFI())) {
			if(buffer_length_WIFI>=3) {
				uint8_t num1 = convert_asciihex_to_int(receiveBuffer_WIFI[0]);
		
				if(num1 & 0b0100) {
					writeString_P("Test GPIO14  HIGH --> OK!\n");
				}
				else {
					writeString_P("*** Test GPIO16  HIGH --> FAIL! ############\n");
					errors++;
					failure_code2 = 16;
				}
			}
		}
	}
	
	PORTC &= ~WLAN_GPIO14_OUT;
	mSleep(50);

	writeCommand_WIFI("show io");
	while(getBufferLength_WIFI()) {
		if(parseLine_WIFI(readChar_WIFI())) {
			if(buffer_length_WIFI>=3) {
				uint8_t num1 = convert_asciihex_to_int(receiveBuffer_WIFI[0]);
				
				if(!(num1 & 0b0100)) {
					writeString_P("Test GPIO14  LOW --> OK!\n");
				}
				else {
					writeString_P("*** Test GPIO14 LOW --> FAIL! ############\n");
					errors++;
					failure_code2 = 17;
				}
			}
		}
	}
	
	writeString_P("\n\nTesting M256 GPIO8 (PINC7) <<-- WLAN GPIO8...\n"); 


	wifi_cmd("set sys mask 0x21f0");
	wifi_cmd("set sys value 0x0000");
	
	
	wifi_cmd("set sys output 0x0000 0x0100");
	mSleep(70);
	
	if(PINC & WLAN_GPIO8_IN)
	{
		writeString_P("\n*** GPIO 8 HIGH! SHOULD BE LOW! ERROR! ###########\n"); 
		errors++;
		failure_code3 = 18;
	}
	else
	{
		writeString_P("\nGPIO 8 LOW, OK!\n"); 
	}
	
	setLEDs(0b1000);
	wifi_cmd("set sys output 0x0100 0x0100");
	mSleep(70);
	
	
	if(PINC & WLAN_GPIO8_IN)
	{
		writeString_P("\nGPIO 8 HIGH, OK!\n"); 
	}
	else
	{
		writeString_P("\n*** GPIO 8 LOW! SHOULD BE HIGH! ERROR! ###########\n"); 
		errors++;
		failure_code3 = 19;
	}
	
	wifi_cmd("set sys output 0x0000 0x0100");
	mSleep(70);
	
	if(PINC & WLAN_GPIO8_IN)
	{
		writeString_P("\n*** GPIO 8 HIGH! SHOULD BE LOW! ERROR! ###########\n"); 
		failure_code3 = 20;
		errors++;
	}
	else
	{
		writeString_P("\nGPIO 8 LOW, OK!\n"); 
	}
	
	
	leave_cmd_mode_WIFI();
		
	writeChar('\n');
	writeChar_WIFI('\n');	
	
	
	if(failure_code1 == 4 || failure_code1 == 5 || failure_code1 == 6 || failure_code1 == 7)
	{
		writeString_P_WIFI("*** Test M256 RTS --> FAIL! ############\n\n");
		writeString_P_WIFI("*** Make sure flow control is enabled!\n\n");
	}
	else
	{
		writeString_P_WIFI("M256 RTS is working fine!\n\n");
	}
	
	
	if(failure_code2 == 14 || failure_code2 == 16)
	{
		writeString_P_WIFI("*** Test WLAN GPIO14 HIGH --> FAIL! ############\n\n");
	}
	else if(failure_code2 == 15 || failure_code2 == 17)
	{
		writeString_P_WIFI("*** Test WLAN GPIO14 LOW --> FAIL! ############\n\n");
	}
	else
	{
		writeString_P_WIFI("WLAN GPIO14 is working fine!\n\n");
	}
	
	if(failure_code3 == 18 || failure_code3 == 19 || failure_code3 == 20)
	{
		writeString_P_WIFI("*** Test WLAN GPIO 8 --> FAIL! ############\n\n");
		writeString_P_WIFI("*** Make sure SYS MASK is set correctly!\n\n");
	}
	else
	{
		writeString_P_WIFI("WLAN GPIO8 is working fine!\n\n");
	}
	
	return; 
	
	wifi_cmd_error:
		errors++;
		cmd_error_exit_WIFI();
}


void full_automatic_test(void)
{

	writeString_P_WIFI("\n\nOK! WIFI Interface seems to be working!\n");                                
	writeString_P_WIFI("\n<<<TEST SEND OK, CODE ***QH74239076KZRBV57UI0***>>>\n");
	writeString_P("\nInterface test is done!\n\n"); 
	
	run_test_buttons();
	writeString_P_WIFI("\n<<<TEST BUTTONS OK, CODE ***K5232FR1M345Z5U230XN***>>>\n");
	writeString_P("\n\nButtons test is done, now testing LEDs... \n\n"); 
	writeString_P_WIFI("\n\nButtons test is done, now testing LEDs... \n\n"); 
	
	run_test_leds();
	
	run_test_i2c();
	errorcheck();
	writeString_P_WIFI("<<<TEST I2C OK, CODE ***LD3548903WE3510FGC78***>>>\n");
		
	test_wlan_io_ports();
	errorcheck();
	writeString_P_WIFI("\n<<<TEST WLAN IO PORTS OK, CODE ***674ZUW301AJ0R47BMSY8***>>>\n");

	run_test_sd_card();
	errorcheck();
	writeString_P_WIFI("\n<<<TEST SD CARD OK, CODE ***533REQ960LB534U874FJ***>>>\n");

	showScreenLCD("ALL TESTS DONE!", "SELFTEST OK!");
	bars(1);
	writeString_P("ALL TESTS DONE!");
	writeString_P_WIFI("ALL TESTS DONE! <<<ACK TEST IS OK, CODE ***5X3AK1C0H56U72PY0N4D***>>>\n");
	bars(1);
	
}


void space_filler(void);


/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6M256(); // Always call this first! The Processor will not work
				   // correctly otherwise. 
	
	initLCD(); // Initialize the LC-Display (LCD)
			   // Always call this before using the LCD!
			   
	bars(2);
	writeString_P("\n\nRP6 Control M256 WIFI Selftest!\n\n"); 
	bars(2);
	setLEDs(0b1111);
	mSleep(50);

	showScreenLCD("################", "################");

	mSleep(50);
	showScreenLCD("################", "################");
	showScreenLCD("RP6 Control M256", "--- SELFTEST ---");

	PORTL |= WLAN_WAKE;

	mSleep(250);

	PORTL &= ~WLAN_WAKE;



	writeString_P("\n\nRP6 Control M256 WIFI Selftest!\n\n"); 
	writeString_P("\n\n*** S E R I A L    I N T E R F A C E ***\n\n"); 
	
	writeString_P_WIFI("\n\nRP6 Control M256 WIFI Selftest!\n\n"); 
	writeString_P_WIFI("\n\n*** W I F I    N E T W O R K    C O N N E C T I O N ***\n\n"); 
	
	I2CTWI_initMaster(100);  	
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);
 	mSleep(100);
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, 3, 0b001001); 
	
	
	showScreenLCD("Testing", "Communication");
	
	writeString_P("\nPlease see text output on the WIFI Interface!\n\n"); 
	
	writeChar('\n');
	writeChar_WIFI('\n');
	
	uint8_t runs = 0;
	
	setLEDs(0b0001);
	
	
	while(true)
	{
		setLEDs(0b1000);
		writeString_P_WIFI("####### <<<TEST START OK, CODE ***G56KFG29O5T3560RTJ12***>>> ##############\n");
		writeString_P_WIFI("###### ###################################################### #############\n"); 
		writeString_P_WIFI("##### #              RP6 M256 WIFI Selftest                  # ############\n"); 
		writeString_P_WIFI("#### # v. 1.4a       ----------------------        16.07.2012 # ###########\n"); 
		writeString_P_WIFI("### ############################################################ ##########\n"); 
		writeString_P_WIFI("##  This is sent through the *** WIFI *** Interface          ");
		writeIntegerLength_WIFI(runs++, DEC, 3);
		writeString_P_WIFI("  #########\n"); 
		writeString_P_WIFI("#                                                                  ########\n"); 
		writeString_P_WIFI("# Selection Menu:                                                   #######\n");
		writeString_P_WIFI("#  0 |             - Run full Selftest (button+led+i2c+gpio+sdc)     ######\n"); 
		writeString_P_WIFI("#  1 | info        - Get all WIFI Info and Settings                   #####\n"); 
		writeString_P_WIFI("#  2 | scan        - Get WIFI Information and scan for Accesspoints    ####\n"); 
		writeString_P_WIFI("#  3 | sdc         - Test SD Card                                       ###\n"); 
		writeString_P_WIFI("#  4 | leds        - LED Test                                            ##\n"); 
		writeString_P_WIFI("#  5 | i2c         - I2C Bus Test (RP6Base_I2CSlave.hex must be loaded)   #\n"); 
		writeString_P_WIFI("#  6 | buttons     - Buttons Test                                         #\n"); 
		writeString_P_WIFI("#  7 | wlan_io     - Test WLAN I/O Ports                                  #\n"); 
		writeString_P_WIFI("#                                                                         #\n"); 
		writeString_P_WIFI("# For the full automatic test, please just press ENTER or 0+Enter         #\n"); 
		writeString_P_WIFI("# to acknowledge that you can read this message.                          #\n");
		writeString_P_WIFI("# Make sure send CR+LF on Enter is enabled, otherwise this will not work! #\n"); 
		writeString_P_WIFI("###########################################################################\n"); 

		enterString_WIFI();
		setLEDs(0b0010);


		// Wifi Default config not used, RobotLoader does configuration
		if(strcmp(receiveBuffer_WIFI,"1")==0 || strcmp(receiveBuffer_WIFI,"info")==0)
		{
			wlan_info();
			continue;
		}
		else if(strcmp(receiveBuffer_WIFI,"2")==0 || strcmp(receiveBuffer_WIFI,"scan")==0)
		{
			writeString_P_WIFI("\nSCAN!\n"); 
			wlan_scan();
			continue;
		}
		else if(strcmp(receiveBuffer_WIFI,"3")==0 || strcmp(receiveBuffer_WIFI,"sdc")==0)
		{
			writeString_P_WIFI("\nSD Card Test!\n"); 
			run_test_sd_card();
			mSleep(1000);
			continue;
		}
		else if(strcmp(receiveBuffer_WIFI,"4")==0 || strcmp(receiveBuffer_WIFI,"leds")==0)
		{
			run_test_leds();
			continue;
		}
		else if(strcmp(receiveBuffer_WIFI,"5")==0 || strcmp(receiveBuffer_WIFI,"i2c")==0)
		{
			run_test_i2c();
			continue;
		}
		else if(strcmp(receiveBuffer_WIFI,"6")==0 || strcmp(receiveBuffer_WIFI,"buttons")==0)
		{
			run_test_buttons();
			continue;
		}
		else if(strcmp(receiveBuffer_WIFI,"7")==0 || strcmp(receiveBuffer_WIFI,"wlan_io")==0)
		{
			test_wlan_io_ports();
			continue;
		}
		else if(strcmp(receiveBuffer_WIFI,"0")==0)
		{
			full_automatic_test();
		}
		else
		{
		//	continue;
			full_automatic_test();
		}
		
		startStopwatch1();
		setStopwatch1(0);
		uint8_t runLEDs = 1; 
		uint8_t dir = 0;
		
		while(true) 
		{
			if(getStopwatch1() > 100) {
				setLEDs(runLEDs); 
				if(dir == 0)
					runLEDs <<= 1; 
				else
					runLEDs >>= 1;
				if(runLEDs > 7 ) 
					dir = 1;			
				else if (runLEDs < 2 ) 
					dir = 0;
				setStopwatch1(0);
				break;
			}
		}
	}
	return 0;
}

