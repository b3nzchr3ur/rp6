#ifndef MICROPHONE_H
#define MICROPHONE_H

#include "internal/Microphone_internal.h"

/*****************************************************************************/
// Microphone:

void dischargePeakDetector(void);
uint16_t getMicrophonePeak(void);

#endif
