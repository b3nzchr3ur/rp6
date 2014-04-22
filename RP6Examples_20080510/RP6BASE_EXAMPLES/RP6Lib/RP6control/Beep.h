#ifndef BEEP_H
#define BEEP_H

#include "internal/Beep_internal.h"

/*****************************************************************************/
// Beeper:

void beep(unsigned char pitch, unsigned int time);
void setBeeperPitch(uint8_t pitch);

#define sound(_pitch_,_time_,_delay_) {beep(_pitch_,_time_);mSleep(_delay_ + _time_);}

#endif
