#include "stm32f10x.h"
#include "Button.h"
#include "Delay.h"

#define Button_R()    GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)

void Button_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

uint8_t Button_GetFall(void)
{
    static uint8_t Last = 1;
    uint8_t Now;
    uint8_t Fall = 0;

    Now = Button_R();

    if (Last == 1 && Now == 0)
    {
        Delay_ms(20);

        if (Button_R() == 0)
        {
            Fall = 1;
        }
    }

    Last = Now;

    return Fall;
}



