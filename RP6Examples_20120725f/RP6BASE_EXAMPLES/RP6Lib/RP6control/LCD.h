#ifndef LCD_H
#define LCD_H

#include "internal/LCD_internal.h"

/*****************************************************************************/
// LCD:

void initLCD(void);

void clearLCD(void);
void clearPosLCD(uint8_t line, uint8_t pos, uint8_t length);

void writeCharLCD(uint8_t ch);

#define writeStringLCD_P(__pstr) writeNStringLCD_P((PSTR(__pstr)))
void writeStringLengthLCD(char *string, uint8_t length, uint8_t offset);
void writeStringLCD(char *string);
void writeNStringLCD_P(const char *pstring);

void writeIntegerLCD(int16_t number, uint8_t base);
void writeIntegerLengthLCD(int16_t number, uint8_t base, uint8_t length);

void setCursorPosLCD(uint8_t line, uint8_t pos);

#ifndef HEX
	#define HEX 16
#endif
#ifndef DEC 
	#define DEC 10
#endif
#ifndef OCT
	#define OCT 8
#endif
#ifndef BIN
	#define BIN 2
#endif


#endif
