/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M256 Examples
 * ****************************************************************************
 * Example: SD Card Example
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * This Demo program shows how to use the SD Card routines.
 *
 * Please use a suitable microSD Card and COPY THE FILE: 
 * M256_SELFTEST_TESTFILE.txt
 * into the root directory (no subfolder) of the card. 
 * DO NOT rename it otherwise it will not work (you can change this program
 * of course).
 *  
 * You MUST insert a suitable microSD Card into the slot BEFORE turning on
 * the power of the Robot. It may also work when you hotplug the card, but
 * this is not guranteed. 
 *
 * Then upload and start this program and watch the output. 
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

#include "RP6M256Lib.h" 					// The RP6 M256 Library. 
											// Always needs to be included!

#include "fat.h"
#include "fat_config.h"
#include "partition.h"

#include "sd-reader_config.h"
#include "sd_raw.h"

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
 * Print out some info about the SD Card (size, manufacturer etc. )
 */
uint8_t print_disk_info(const struct fat_fs_struct* fs)
{
    if(!fs)
        return 0;

    struct sd_raw_info disk_info;
    if(!sd_raw_get_info(&disk_info))
        return 0;

    writeString_P_WIFI("manuf:  0x"); 	writeInteger_WIFI(disk_info.manufacturer,HEX); writeChar_WIFI('\n');
    writeString_P_WIFI("oem:    "); 	writeString_WIFI((char*) disk_info.oem); writeChar_WIFI('\n');
    writeString_P_WIFI("prod:   "); 	writeString_WIFI((char*) disk_info.product); writeChar_WIFI('\n');
    writeString_P_WIFI("rev:    "); 	writeInteger_WIFI(disk_info.revision,DEC); writeChar_WIFI('\n');
    writeString_P_WIFI("serial: 0x"); 	writeInteger_WIFI(disk_info.serial, HEX); writeChar_WIFI('\n');
    writeString_P_WIFI("date:   "); 	writeInteger_WIFI(disk_info.manufacturing_month, DEC); writeChar_WIFI('/');
										writeInteger_WIFI(disk_info.manufacturing_year, DEC); writeChar_WIFI('\n');
    writeString_P_WIFI("size:   "); 	writeInteger_WIFI(disk_info.capacity / 1024 / 1024, DEC);  writeString_P_WIFI("MB\n");
    writeString_P_WIFI("copy:   "); 	writeInteger_WIFI(disk_info.flag_copy,DEC); writeChar_WIFI('\n');
    writeString_P_WIFI("wr.pr.: "); 	writeInteger_WIFI(disk_info.flag_write_protect_temp,DEC); writeChar_WIFI('/');
										writeInteger_WIFI(disk_info.flag_write_protect,DEC); writeChar_WIFI('\n');
    writeString_P_WIFI("format: "); 	writeInteger_WIFI(disk_info.format,DEC); writeChar_WIFI('\n');
  
    writeString_P_WIFI("free:   "); 	writeInteger_WIFI(fat_get_fs_free(fs),DEC); writeChar_WIFI('/');
										writeInteger_WIFI(fat_get_fs_size(fs),DEC); writeChar_WIFI('\n');
	
    return 1;
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
 * Perform SD Card Test. 
 * READ a specific file on a FAT32 formatted microSD Card and output its contents. 
 */
void test_sdcard(void)
{
	writeString_P_WIFI("Testing SD Card. Please wait (SDHC Cards will take longer)...\n\n"); 
	
	if(sd_raw_init())
	{
		writeString_WIFI("SD INIT OK!\n"); 
	}
	else
	{
		writeString_WIFI("SD INIT FAILED!\n");
		writeString_P_WIFI("Please verify that the SD Card is OK and properly inserted!\n");
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
			// If the partition did not open, assume the storage device
			// is a "superfloppy", i.e. has no MBR.
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
			return;
		}

        // print some card information 
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
		
		
		// ###############################################################################
		// Make sure this file is on the SD Card - right in the root directory!
		char* filename = "M256_SELFTEST_TESTFILE.txt";
		
		struct fat_file_struct* fd = open_file_in_dir(fs, dd, filename);
		if(!fd)
		{
			writeString_P_WIFI("error opening ");
			writeString_WIFI(filename);
			writeChar_WIFI('\n');
			
			writeString_P_WIFI("Please verify that the Testfile is\n");
			writeString_P_WIFI("correctly copied on the SD Card!\n\n");
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
					// Briefly check if the contents are correct!
					// Only one single line is checked. 
					char* ptr = strstr(&buffer[0], "RP6 M256");
					if(strncmp(ptr, "RP6 M256 SD CARD Testfile 01234567890", 37) != 0) {
						
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
				
				// Output the content of the Textfile to both, serial and WIFI:
				for(intptr_t i = 0; i < count; ++i)
				{
					writeChar_WIFI(buffer[i]);
					writeChar(buffer[i]);
				}
				offset += sizeof(buffer);
				if(offset % 128 == 0)  // small break for larger files...  
					mSleep(2);
			}
			
			fat_close_file(fd);
		}

	
        fat_close_dir(dd);
        fat_close(fs);
        partition_close(partition);
}


/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6M256();
	initLCD(); 
	
	setLEDs(0b1111);
	
	writeString_P("\n\n   _______________________\n");
	writeString_P("   \\| RP6  ROBOT SYSTEM |/\n");
	writeString_P("    \\_-_-_-_-_-_-_-_-_-_/\n\n");
	
	writeString_P("Hello World from RP6 CONTROL M256 WIFI!\n"); 
	writeString_P("Let's move it!  :)\n"); 
	writeString_P("This message was sent via ***Serial Port***!\n"); 

	writeString_P_WIFI("\n\n   _______________________\n");
	writeString_P_WIFI("   \\| RP6  ROBOT SYSTEM |/\n");
	writeString_P_WIFI("    \\_-_-_-_-_-_-_-_-_-_/\n\n");
	
	writeString_P_WIFI("Hello World from RP6 CONTROL M256 WIFI!\n"); 
	writeString_P_WIFI("Let's move it!  :)\n"); 
	writeString_P_WIFI("This message was sent via ***WLAN***!\n"); 

	writeString_P("\nCheck WIFI Terminal output!\n"); 


	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("<<RP6  Control>>", "<<microSD Card>>");
	mSleep(1500); 
	showScreenLCD("microSD Card", "Example Program");

	
	
	test_sdcard();
	
	startStopwatch1();
	
	while(true) 
	{
		runningLight();
	}
	return 0;
}
