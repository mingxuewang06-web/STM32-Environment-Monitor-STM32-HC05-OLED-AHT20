#ifndef __Serial_H
#define __Serial_H

#include "stm32f10x.h"

void Serial_Init();
void Serial_SendByte(uint8_t Byte);
void Serial_SendString(char *String);
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_SendFloat(float Number, uint8_t IntLength, uint8_t DecLength);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_ReceiveByte(void);


#endif