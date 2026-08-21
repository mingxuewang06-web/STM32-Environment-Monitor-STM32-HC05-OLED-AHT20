#include "stm32f10x.h"
#include "Buzzer.h"

#define Buzzer_W(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)(x))


void Buzzer_OFF();

void Buzzer_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	Buzzer_OFF();

}
void Buzzer_ON()
{
	Buzzer_W(0);
}
void Buzzer_OFF()
{
	Buzzer_W(1);
}

