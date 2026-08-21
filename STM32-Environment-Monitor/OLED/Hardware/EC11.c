#include "stm32f10x.h"
#include "EC11.h"
#include "OLED.h"

/*????*/
#define EC11_A()    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)
#define EC11_B()    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)

uint8_t page = 1;
int8_t encoder;

/*?????*/
void EC11_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

int8_t EC11_Encoder()
{
	static uint8_t Last_A, Last_B, Init = 0;
	uint8_t Now_A, Now_B;
	
	if(Init == 0)
	{
		Last_A = EC11_A();
	  Last_B = EC11_B();
		Init = 1;
	}
	

	Now_A = EC11_A();
	Now_B = EC11_B();

	
	if(Last_A == 1 && Now_A == 0)
	{
		if(Now_B == 0)
		{
			encoder = 1;
		}
		else
		{
			encoder = -1;
		}
	}
	else
	{
		encoder = 0;
	}
	
	Last_A = Now_A;
	
	return encoder;
}

void Page_Update()
{
			if (encoder == 1)
		{
				page++;
				if (page > 4) 
				{
				  page = 1;
				}
				OLED_Clear();
		}
		else if (encoder == -1)
		{
				if (page == 1) 
				{
					page = 4;
				}
				else page--;
				OLED_Clear();
		}
}



float EC11_Modify(float modify_value)
{
    if(encoder == 1)
    {
        modify_value = modify_value + 0.05f;
    }
    else if(encoder == -1)
    {
        modify_value = modify_value - 0.05f;
    }

    return modify_value;
}





