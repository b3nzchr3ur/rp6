#ifndef KEYS_H
#define KEYS_H

#include "internal/Keys_internal.h"

/*****************************************************************************/
// Keys:

uint8_t getPressedKeyNumber(void);
uint8_t checkPressedKeyEvent(void);
uint8_t checkReleasedKeyEvent(void);

#endif
