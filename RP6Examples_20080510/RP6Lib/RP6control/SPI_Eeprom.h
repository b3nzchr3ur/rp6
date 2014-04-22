#ifndef SPI_EEPROM_H
#define SPI_EEPROM_H

#include "internal/SPI_Eeprom_internal.h"

/*****************************************************************************/
// SPI EEPROM:
uint8_t SPI_EEPROM_readByte(uint16_t memAddr);
void SPI_EEPROM_writeByte(uint16_t memAddr, uint8_t data);
void SPI_EEPROM_enableWrite(void);
void SPI_EEPROM_disableWrite(void);
uint8_t SPI_EEPROM_getStatus(void);

void SPI_EEPROM_writeBytes(uint16_t startAddr, uint8_t *buffer, uint8_t length);
void SPI_EEPROM_readBytes(uint16_t startAddr, uint8_t *buffer, uint8_t length);

#endif
