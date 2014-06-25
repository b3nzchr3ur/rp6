#ifndef BUMPERS_H
#define BUMPERS_H

#include "internal/Bumpers_internal.h"

/*****************************************************************************/
// Bumpers

void task_Bumpers(void);

uint8_t getBumperLeft(void);
uint8_t getBumperRight(void);

void BUMPERS_setStateChangedHandler(void (*bumperHandler)(void)); 

#endif


