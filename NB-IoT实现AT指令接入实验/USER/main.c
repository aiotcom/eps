#include "stm32f1xx_hal.h"
#include "MsgHandler.h"
#include "stm32f1xx.h"
#include "delay.h"
#include "Rs485.h"
#include "usart.h"
#include "M5310.h"
#include "nbiot.h"
#include "utils.h"
#include "coap.h"

int main(void)
{
  HAL_Init();             					//初始化HAL库
	Rs485_Init();											//初始化485
	M5310_Power_Init(); 							//M5310的复位控制IO初始化
	UART1_Init(115200);								//初始化串口1
	UART2_Init(9600);									//初始化串口2
	netdev_init();      							//初始化M5310

	while(1)
	{

	}
}






