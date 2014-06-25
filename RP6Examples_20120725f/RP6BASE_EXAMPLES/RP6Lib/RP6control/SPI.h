#ifndef SPI_H
#define SPI_H

#include "internal/SPI_internal.h"

/*****************************************************************************/
// SPI:

void writeSPI(uint8_t data);
uint8_t readSPI(void);
uint16_t readWordSPI(void);
void writeWordSPI(uint16_t data);
void writeBufferSPI(uint8_t *buffer, uint8_t length);
void readBufferSPI(uint8_t *buffer, uint8_t length);

#endif
