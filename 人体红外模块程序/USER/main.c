#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "delay.h"
#include "PIR.h"


int main(void)
{
  HAL_Init();						//初始化HAL库 
	PIR_Init();						//初始化人体红外

	while(1)
	{
		if(PIR_Read())
		{
			PIR_LED_ON();
		}
		else
		{
			PIR_LED_OFF();
		}
	}
}

