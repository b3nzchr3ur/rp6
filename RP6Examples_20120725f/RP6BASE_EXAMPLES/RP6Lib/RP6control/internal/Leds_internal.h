#ifndef LEDS_INTERNAL_H
#define LEDS_INTERNAL_H

/*****************************************************************************/
// Leds

// A shadow register that simplifies usage of status LEDs:
typedef union {
 	uint8_t byte;
	struct {
		uint8_t LEDS:4;
		uint8_t LCDD:4;
	};
	struct {
		uint8_t LED4:1;
		uint8_t LED3:1;
		uint8_t LED2:1;
		uint8_t LED1:1;
		uint8_t D0:1;
		uint8_t D1:1;
		uint8_t D2:1;
		uint8_t D3:1;
	};
} externalPort_t;

extern externalPort_t externalPort;

void outputExt(void);

#endif
