#ifndef __GL5516_H
#define __GL5516_H

#include "stm32f10x.h"

void GL5516_Init(void);
uint16_t GL5516_GetADValue(void);
float GL5516_GetVoltage(void);

#endif