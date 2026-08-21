//阻塞式

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AHT20.h"
#include "GL5516.h"
#include "EC11.h"
#include "Buzzer.h"
#include "Serial.h"

uint16_t Light_AD;
float Light_Voltage;
float LV;

float RH;
float T;

uint8_t Edit_Mode = 0;
uint8_t RxData;


void OLED_Page();
void threshold_Modify();


float threshold_LV = 0.5;

uint8_t Buzzer_Mode = 0;   // 0 自动模式，1 手动模式
uint8_t Buzzer_State = 0;  // 手动模式下蜂鸣器状态



int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	AHT20_I2C_Init();
	GL5516_Init();
	EC11_Init();
	Buzzer_Init();
	Button_Init();
	Serial_Init();

	OLED_Clear();
	
	
	while(1)
	{


		encoder = EC11_Encoder();
		AHT20_Measure();
		
    //AHT20准备的80ms
		for(uint8_t i = 0; i < 16; i++)
		{
			Delay_ms(5);
			encoder = EC11_Encoder();

			threshold_Modify();

			if (!(page == 4 && Edit_Mode == 1))
			{
					Page_Update();
			}

			OLED_Page();
		}
		
		AHT20_Receive();
		AHT20_Calculate();
		
		RH = hum_raw/1048576.0f*100.f;
		T = temp_raw/1048576.0f*200-50; 
			
		Light_AD = GL5516_GetADValue();
		Light_Voltage = GL5516_GetVoltage();
		LV = Light_Voltage;
		


		
		for(uint8_t i = 0; i < 50; i++)
		{
			Delay_ms(5);
			encoder = EC11_Encoder();

			threshold_Modify();

			if (!(page == 4 && Edit_Mode == 1))
			{
					Page_Update();
			}

			OLED_Page();
		}

		OLED_Page();
		
		
		
    //蜂鸣器响
		if(Buzzer_Mode == 0)
		{
				if(LV < threshold_LV)
				{
						Buzzer_ON();
				}
				else
				{
						Buzzer_OFF();
				}
		}
		else
		{
				if(Buzzer_State == 1)
				{
						Buzzer_ON();
				}
				else
				{
						Buzzer_OFF();
				}
		}
		
		

		//蓝牙发送至手机
		Serial_SendString("T:");
		Serial_SendFloat(T, 2, 2);
		
    Serial_SendString("RH:");
		Serial_SendFloat(RH, 2, 2);		
		
    Serial_SendString("LV:");
		Serial_SendFloat(LV, 1, 3);
		
    Serial_SendString("TH:");
		Serial_SendFloat(threshold_LV, 1, 2);
		
		Serial_SendString("\n");


		
		//手机发送指令
		if (Serial_GetRxFlag() == 1)
		{
			RxData = Serial_ReceiveByte();

			if (RxData == '+')
			{
				threshold_LV += 0.1f;
			}
			else if (RxData == '-')
			{
				threshold_LV -= 0.1f;

					if (threshold_LV < 0)
					{
						threshold_LV = 0;
					}
			}
			else if(RxData == 'A')
			{
					Buzzer_Mode = 0;
			}
			else if(RxData == 'N')
			{
					Buzzer_Mode = 1;
					Buzzer_State = 1;
			}
			else if(RxData == 'F')
			{
					Buzzer_Mode = 1;
					Buzzer_State = 0;
			}
			else if(RxData == 'P')
			{
				page++;
				if (page > 4)
				{
					page = 1;
				}
				 OLED_Clear();
			}

		}		
		
		
		
		Delay_ms(50);
//			
	}
}

void OLED_Page()
{

	if(page == 1)
	{
		OLED_ShowString(1, 1, "T:");
		OLED_ShowFloat(1, 3, T, 2, 2);
		OLED_ShowChar(1, 8, 'C');

		OLED_ShowString(2, 1, "RH:");
		OLED_ShowFloat(2, 4, RH, 2, 2);
		OLED_ShowChar(2, 9, '%');


	}
	else if(page == 2)
	{
		OLED_ShowString(3, 1, "LV:");
	  OLED_ShowFloat(3, 4, LV, 1, 3);
	  OLED_ShowChar(3, 9, 'V');
		

	}
	else if(page == 3)
	{
		OLED_ShowString(1, 1, "T:");
		OLED_ShowFloat(1, 3, T, 2, 2);
		OLED_ShowChar(1, 8, 'C');

		OLED_ShowString(2, 1, "RH:");
		OLED_ShowFloat(2, 4, RH, 2, 2);
		OLED_ShowChar(2, 9, '%');
			
		OLED_ShowString(3, 1, "LV:");
		OLED_ShowFloat(3, 4, LV, 1, 3);
		OLED_ShowChar(3, 9, 'V');
		

		}
	else if(page == 4)
	{
		OLED_ShowString(1, 1, "threshold_LV:");
		OLED_ShowFloat(2, 1, threshold_LV, 1, 2);
		

	}
 // threshold_Modify();


}

void threshold_Modify(void)
{
    if(page == 4)
    {
        if(Button_GetFall() == 1)
        {
            Edit_Mode = !Edit_Mode;
            OLED_Clear();
        }

        if(Edit_Mode == 1)
        {
            threshold_LV = EC11_Modify(threshold_LV);
        }
    }
    else
    {
        Edit_Mode = 0;
    }
}


////非阻塞式
//#include "stm32f10x.h"
//#include "Delay.h"
//#include "OLED.h"
//#include "AHT20.h"
//#include "GL5516.h"
//#include "EC11.h"

//#define TICK_MS                 5

//#define OLED_PERIOD_TICK        20      // 100ms
//#define LIGHT_PERIOD_TICK       20      // 100ms
//#define AHT20_PERIOD_TICK       100     // 500ms
//#define AHT20_WAIT_TICK         16      // 80ms

//uint16_t Light_AD;
//float Light_Voltage;
//float LV;

//float RH;
//float T;

//static uint16_t OLED_Timer = OLED_PERIOD_TICK;
//static uint16_t Light_Timer = LIGHT_PERIOD_TICK;
//static uint16_t AHT20_Timer = AHT20_PERIOD_TICK;

//static uint8_t AHT20_Busy = 0;
//static uint8_t AHT20_WaitTimer = 0;

//void Encoder_Task(void);
//void Light_Task(void);
//void AHT20_Task(void);
//void OLED_Task(void);
//void OLED_Page(void);

//int main(void)
//{
//    OLED_Init();
//    AHT20_I2C_Init();
//    GL5516_Init();
//    EC11_Init();

//    OLED_Clear();

//    while (1)
//    {
//        Delay_ms(TICK_MS);

//        Encoder_Task();     // 每 5ms 检测编码器
//        Light_Task();       // 每 100ms 采光照
//        AHT20_Task();       // AHT20非阻塞采集
//        OLED_Task();        // 每 100ms刷新OLED
//    }
//}

//void Encoder_Task(void)
//{
//    encoder = EC11_Encoder();

//    if (encoder == 1)
//    {
//        page++;

//        if (page > 3)
//        {
//            page = 1;
//        }

//        OLED_Clear();
//        OLED_Page();
//        OLED_Timer = 0;
//    }
//    else if (encoder == -1)
//    {
//        if (page == 1)
//        {
//            page = 3;
//        }
//        else
//        {
//            page--;
//        }

//        OLED_Clear();
//        OLED_Page();
//        OLED_Timer = 0;
//    }
//}

//void Light_Task(void)
//{
//    Light_Timer++;

//    if (Light_Timer >= LIGHT_PERIOD_TICK)
//    {
//        Light_Timer = 0;

//        Light_AD = GL5516_GetADValue();
//        Light_Voltage = Light_AD / 4095.0f * 3.3f;
//        LV = Light_Voltage;
//    }
//}

//void AHT20_Task(void)
//{
//    if (AHT20_Busy == 0)
//    {
//        AHT20_Timer++;

//        if (AHT20_Timer >= AHT20_PERIOD_TICK)
//        {
//            AHT20_Timer = 0;

//            AHT20_Measure();
//            AHT20_Busy = 1;
//            AHT20_WaitTimer = AHT20_WAIT_TICK;
//        }
//    }
//    else
//    {
//        if (AHT20_WaitTimer > 0)
//        {
//            AHT20_WaitTimer--;
//        }

//        if (AHT20_WaitTimer == 0)
//        {
//            AHT20_Busy = 0;

//            AHT20_Receive();
//            AHT20_Calculate();

//            RH = hum_raw / 1048576.0f * 100.0f;
//            T = temp_raw / 1048576.0f * 200.0f - 50.0f;
//        }
//    }
//}

//void OLED_Task(void)
//{
//    OLED_Timer++;

//    if (OLED_Timer >= OLED_PERIOD_TICK)
//    {
//        OLED_Timer = 0;
//        OLED_Page();
//    }
//}

//void OLED_Page(void)
//{
//    if (page == 1)
//    {
//        OLED_ShowString(1, 1, "T:");
//        OLED_ShowFloat(1, 3, T, 2, 2);
//        OLED_ShowChar(1, 8, 'C');

//        OLED_ShowString(2, 1, "RH:");
//        OLED_ShowFloat(2, 4, RH, 2, 2);
//        OLED_ShowChar(2, 9, '%');
//    }
//    else if (page == 2)
//    {
//        OLED_ShowString(1, 1, "Light");

//        OLED_ShowString(2, 1, "AD:");
//        OLED_ShowNum(2, 4, Light_AD, 4);

//        OLED_ShowString(3, 1, "LV:");
//        OLED_ShowFloat(3, 4, LV, 1, 3);
//        OLED_ShowChar(3, 9, 'V');
//    }
//    else if (page == 3)
//    {
//        OLED_ShowString(1, 1, "T:");
//        OLED_ShowFloat(1, 3, T, 2, 2);
//        OLED_ShowChar(1, 8, 'C');

//        OLED_ShowString(2, 1, "RH:");
//        OLED_ShowFloat(2, 4, RH, 2, 2);
//        OLED_ShowChar(2, 9, '%');

//        OLED_ShowString(3, 1, "LV:");
//        OLED_ShowFloat(3, 4, LV, 1, 3);
//        OLED_ShowChar(3, 9, 'V');
//    }
//}