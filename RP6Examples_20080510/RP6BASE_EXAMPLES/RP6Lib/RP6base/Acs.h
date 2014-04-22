#ifndef ACS_H
#define ACS_H

/*****************************************************************************/
// ACS
#include "internal/ACS_internal.h"

void task_ACS(void);

void disableACS(void);
void enableACS(void);

void setACSPwrOff(void);
void setACSPwrLow(void);
void setACSPwrMed(void);
void setACSPwrHigh(void);

void ACS_setStateChangedHandler(void (*acsHandler)(void));

void powerON(void);
void powerOFF(void);


#endif
