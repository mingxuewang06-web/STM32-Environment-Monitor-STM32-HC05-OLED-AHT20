#ifndef __EC11_H
#define __EC11_H

#include "stm32f10x.h"

extern uint8_t page;
extern int8_t encoder;


void EC11_Init(void);
int8_t EC11_Encoder();
void Page_Update();
float EC11_Modify(float modify_value);
	
#endif