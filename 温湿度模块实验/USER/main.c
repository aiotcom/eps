#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "TM1640.h"	
#include "SHT20.h"	
#include "delay.h"
#include "timer.h"
#include "Usart.h"

int main(void)
{
  HAL_Init();             					//初始化HAL库
	SHT2x_Init();											//初始化SHT20
	TM1640_Init();										//初始化TM1640
	TIM3_Init(2000-1,640-1);					//初始化定时器3(20ms)

	while(1)
	{
		Display_Data();									//获取温湿度并显示到数码管上
	}
}





