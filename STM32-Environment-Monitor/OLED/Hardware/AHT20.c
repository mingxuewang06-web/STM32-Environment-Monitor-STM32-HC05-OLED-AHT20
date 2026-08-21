#include "stm32f10x.h"
#include "AHT20.h"
#include "Delay.h"

#define AHT20_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)(x))
#define AHT20_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)(x))
#define AHT20_R_SDA()    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)

uint8_t data[7];
uint32_t hum_raw;
uint32_t temp_raw;

void AHT20_I2C_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	AHT20_W_SCL(1);
	AHT20_W_SDA(1);
	Delay_ms(5);
}

void AHT20_I2C_Delay(void)
{
	Delay_us(2);
}

/**
  * @brief  I2C??
  * @param  ?
  * @retval ?
  */
void AHT20_I2C_Start(void)
{
	AHT20_W_SDA(1);
	AHT20_W_SCL(1);
	AHT20_I2C_Delay();

	AHT20_W_SDA(0);
	AHT20_I2C_Delay();

	AHT20_W_SCL(0);
	AHT20_I2C_Delay();

}

/**
  * @brief  I2C??
  * @param  ?
  * @retval ?
  */
void AHT20_I2C_Stop(void)
{
	AHT20_W_SDA(0);
	AHT20_I2C_Delay();
	AHT20_W_SCL(1);
	AHT20_I2C_Delay();
	AHT20_W_SDA(1);
	AHT20_I2C_Delay();
}



/**
  * @brief  I2C??????
  * @param  Byte ????????
  * @retval ?
  */
void AHT20_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		AHT20_W_SCL(0);
	  AHT20_I2C_Delay();
		AHT20_W_SDA(!!(Byte & (0x80 >> i)));
	  AHT20_I2C_Delay();

		AHT20_W_SCL(1);
		AHT20_I2C_Delay();

	}

	AHT20_W_SCL(0);
	AHT20_I2C_Delay();

}

uint8_t AHT20_ReadByte(void)
{
    uint8_t i;
    uint8_t Byte = 0x00;

    AHT20_W_SDA(1);

    for (i = 0; i < 8; i++)
    {
        AHT20_W_SCL(0);
        AHT20_I2C_Delay();

        AHT20_W_SCL(1);
        AHT20_I2C_Delay();

        if (AHT20_R_SDA() == 1)
        {
            Byte |= (0x80 >> i);
        }
    }

    AHT20_W_SCL(0);
    AHT20_I2C_Delay();


    return Byte;
}



/**
  * @brief  AHT20
  * @param  Command ??????
  * @retval ?
  */
void AHT20_WriteCommand(uint8_t Command)
{
	AHT20_I2C_Start();
	AHT20_I2C_SendByte(0x70);		//????
	AHT20_I2C_SendByte(Command);
	AHT20_I2C_Stop();
}

uint8_t AHT20_WaitAck()
{
	uint8_t ack;

	AHT20_W_SDA(1);
	AHT20_I2C_Delay();

	AHT20_W_SCL(1);
	AHT20_I2C_Delay();

	ack = AHT20_R_SDA();
	AHT20_W_SCL(0);
	AHT20_I2C_Delay();


	return ack;
}


void AHT20_Measure(void)
{
	AHT20_I2C_Start();

	AHT20_I2C_SendByte(0x70);
	AHT20_WaitAck();

	AHT20_I2C_SendByte(0xAC);
	AHT20_WaitAck();

	AHT20_I2C_SendByte(0x33);
	AHT20_WaitAck();

	AHT20_I2C_SendByte(0x00);
	AHT20_WaitAck();

  AHT20_I2C_Stop();

}

void AHT20_SendAck(uint8_t AckBit)
{
	AHT20_W_SCL(0);
	AHT20_I2C_Delay();

	AHT20_W_SDA(AckBit);//0 ack ,1 nack
	AHT20_I2C_Delay();

	AHT20_W_SCL(1);
	AHT20_I2C_Delay();

	AHT20_W_SCL(0);
	AHT20_I2C_Delay();

  AHT20_W_SDA(1);

}

//CRC
//CRC8
//X8+X5+X4+1
//Poly:0011 0001 0x31
unsigned char Calc_CRC8(unsigned char *message,unsigned char Num)
{
	unsigned char i;
	unsigned char byte;
	unsigned char crc =0xFF;
	for (byte = 0;byte<Num;byte++)
	{
		crc^=(message[byte]);
		for(i=8;i>0;--i)
		{
			if(crc&0x80)
				crc=(crc<<1)^0x31;
			else
				crc=(crc<<1);
		}
	}
	return crc;
}
//******


void AHT20_Receive()
{
	AHT20_I2C_Start();

	AHT20_I2C_SendByte(0x71);
	AHT20_WaitAck();

	data[0] = AHT20_ReadByte();
	AHT20_SendAck(0);

	data[1] = AHT20_ReadByte();
	AHT20_SendAck(0);

	data[2] = AHT20_ReadByte();
	AHT20_SendAck(0);

	data[3] = AHT20_ReadByte();
	AHT20_SendAck(0);

	data[4] = AHT20_ReadByte();
	AHT20_SendAck(0);

	data[5] = AHT20_ReadByte();
	AHT20_SendAck(0);

	data[6] = AHT20_ReadByte();
	AHT20_SendAck(1);

	AHT20_I2C_Stop();

}

void AHT20_Calculate(void)
{


    hum_raw = ((uint32_t)data[1] << 12) |
              ((uint32_t)data[2] << 4)  |
              (data[3] >> 4);

    temp_raw = (((uint32_t)data[3] & 0x0F) << 16) |
               ((uint32_t)data[4] << 8) |
               data[5];

}
