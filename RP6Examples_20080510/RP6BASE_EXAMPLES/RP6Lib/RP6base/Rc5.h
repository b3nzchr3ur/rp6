#ifndef RC5_H
#define RC5_H

/*****************************************************************************/
// RC5:

#define TOGGLEBIT 32

typedef union {
	uint16_t data;
	struct {
		unsigned key_code:6;
		unsigned device:5;
		unsigned toggle_bit:1;
		unsigned reserved:3;
	};
} RC5data_t;

void IRCOMM_sendRC5(uint8_t adr, uint8_t data);
void IRCOMM_setRC5DataReadyHandler(void (*rc5Handler)(RC5data_t));

#endif
