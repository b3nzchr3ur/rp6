#ifndef SPI_EEPROM_INTERNAL_H
#define SPI_EEPROM_INTERNAL_H

/*****************************************************************************/
// SPI_Eeprom

#define SPI_EEPROM_READ		0x03	// Read byte(s)
#define SPI_EEPROM_WRITE	0x02	// Write byte(s)
#define SPI_EEPROM_WREN		0x06	// Write Enable
#define SPI_EEPROM_WRDI		0x04	// Write Disable
#define SPI_EEPROM_RDSR		0x05	// Read Status Register
#define SPI_EEPROM_WRSR		0x01	// Write Status Register

#define SPI_EEPROM_STAT_WIP	1	 // Write in Progress Bit
#define SPI_EEPROM_STAT_WEL	2	 // Write Enable Latch Bit
#define SPI_EEPROM_STAT_BP0	4	 // Block Protect 0 Bit
#define SPI_EEPROM_STAT_BP1	8	 // Block Protect 1 Bit
#define SPI_EEPROM_STAT_SRWD 128 // Status Register Write Protect

#define SPI_EEPROM_PAGESIZE 64



#endif
