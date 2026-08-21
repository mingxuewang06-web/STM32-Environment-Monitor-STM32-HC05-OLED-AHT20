#include "stm32f10x.h"
#include "Serial.h"


void Serial_Init()
{
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
 	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, Byte);
}

void Serial_SendString(char *String)
{
    uint8_t i;

    for (i = 0; String[i] != '\0'; i++)
    {
        Serial_SendByte(String[i]);
    }
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;

    while (Y--)
    {
        Result *= X;
    }

    return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;

    for (i = 0; i < Length; i++)
    {
        Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
    }
}

void Serial_SendFloat(float Number, uint8_t IntLength, uint8_t DecLength)
{
    uint32_t IntPart;
    uint32_t DecPart;
    uint32_t Pow = 1;
    uint8_t i;

    if (Number < 0)
    {
        Serial_SendByte('-');
        Number = -Number;
    }

    for (i = 0; i < DecLength; i++)
    {
        Pow *= 10;
    }

    IntPart = (uint32_t)Number;
    DecPart = (uint32_t)((Number - IntPart) * Pow + 0.5f);

    if (DecPart >= Pow)
    {
        IntPart++;
        DecPart -= Pow;
    }

    Serial_SendNumber(IntPart, IntLength);
    Serial_SendByte('.');
    Serial_SendNumber(DecPart, DecLength);
}

uint8_t Serial_GetRxFlag(void)
{
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
    {
        return 1;
    }
    return 0;
}

uint8_t Serial_ReceiveByte(void)
{
    return USART_ReceiveData(USART1);
}