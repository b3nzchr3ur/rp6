#ifndef MMA7455L_H
#define MMA7455L_H

#include <stdint.h>					

typedef struct {
	int8_t Xacc;
	int8_t Yacc;
	int8_t Zacc;
} Meetdata;


/* DATA ready status is NOT OUTPUT TO INT1 PIN
 * 3-wire SPI mod
 * Self Test NOT enabled
 * +/-2g sensitivity mode
 * Measurement mode
 */
void initMMA7455(void);

/* idem, standby mode */
void MMA7455_set_to_standby_mode(void);

/* idem, Measurement mode */
void MMA7455_set_to_measurement_mode(void);

/* vraag de sensordata op */
void MMA7455_measure(Meetdata *data);

#endif
