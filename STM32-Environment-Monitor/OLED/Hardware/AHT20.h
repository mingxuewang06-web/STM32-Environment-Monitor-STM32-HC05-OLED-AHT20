#include "stm32f10x.h"

#ifndef __AHT20_H
#define __AHT20_H

extern uint32_t hum_raw;
extern uint32_t temp_raw;

void AHT20_I2C_Init(void);
void AHT20_I2C_Start(void);
void AHT20_I2C_Stop(void);
void AHT20_I2C_SendByte(uint8_t Byte);
uint8_t AHT20_ReadByte(void);
void AHT20_WriteCommand(uint8_t Command);
uint8_t AHT20_WaitAck();
void AHT20_Measure(void);
void AHT20_SendAck(uint8_t AckBit);
unsigned char Calc_CRC8(unsigned char *message,unsigned char Num);
void AHT20_Receive();
void AHT20_Calculate(void);

#endif
