#ifndef ADC_H
#define ADC_H

/*****************************************************************************/
// ADC

#include "internal/ADC_internal.h"

void task_ADC(void);

uint16_t readADC(uint8_t channel);
void startADC(uint8_t channel);

#endif
