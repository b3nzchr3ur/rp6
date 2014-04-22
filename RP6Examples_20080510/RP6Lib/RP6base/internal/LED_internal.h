#ifndef LED_INTERNAL_H
#define LED_INTERNAL_H

/*****************************************************************************/
// Status LEDs

// A shadow register that simplifies usage of status LEDs:
union {
 	uint8_t byte;
	struct {
		unsigned LEDsR:3;
		unsigned LEDsL:3;
		unsigned reserved:2;
	};
	struct {
		unsigned LED1:1;
		unsigned LED2:1;
		unsigned LED3:1;
		unsigned LED4:1;
		unsigned LED5:1;
		unsigned LED6:1;
		unsigned reserved1:1;
		unsigned reserved2:1;
	};
} statusLEDs;

#ifdef POWER_ON_WARNING
	void enablePowerOnWarning(void);
	void disablePowerOnWarning(void);
#endif

#endif

