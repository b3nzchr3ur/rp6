#ifndef LCD_INTERNAL_H
#define LCD_INTERNAL_H

/*****************************************************************************/
// LCD

void _showScreenLCD_P(const char *line1, const char *line2);
#define showScreenLCD(__line1,__line2); ({_showScreenLCD_P((PSTR(__line1)),(PSTR(__line2)));})

void setLCDD(uint8_t lcdd);
void write4BitLCDData(uint8_t data);
void writeLCDCommand(uint8_t cmd);


#endif
