#include "stm32f1xx.h"
#include "delay.h"
#include "Rs485.h"
#include "usart.h"
#include "key.h"
#include "led.h"


#define	Rs485_TX	1
#define	Rs485_RX	0

int main(void)
{
	HAL_Init();						//初始化HAL库    
	LED_Init();						//初始化LED	
	KEY_Init();						//初始化按键
	Rs485_Init();					//初始化485
	UART1_Init(115200);		//初始化串口一

	while(1)
	{

#if Rs485_TX	//发送

		key_Operatio();	//按键发送控制命令，控制LED灯

#endif

#if Rs485_RX	//接收
		if(!DataHandling_485(Addr_LED))		//处理485数据
		{
			switch(Rx_Stack.Command)
			{
				case LED_ON1:
					LED1_ON();	//点亮LED1
					break;
				case LED_ON2:
					LED2_ON();	//点亮LED2
					break;
				case LED_ON3:
					LED3_ON();	//点亮LED3
					break;
				case LED_ON4:
					LED4_ON();	//点亮LED4
					break;
				case LED_OFF1:
					LED1_OFF();	//关闭LED1
					break;
				case LED_OFF2:
					LED2_OFF();	//关闭LED2
					break;
				case LED_OFF3:
					LED3_OFF();	//关闭LED3
					break;
				case LED_OFF4:
					LED4_OFF();	//关闭LED4
					break;
				case LED_Control://控制4个LED灯
					Rx_Stack.Data[0]==0?LED1_OFF():LED1_ON();
					Rx_Stack.Data[1]==0?LED2_OFF():LED2_ON();
					Rx_Stack.Data[2]==0?LED3_OFF():LED3_ON();
					Rx_Stack.Data[3]==0?LED4_OFF():LED4_ON();
					break;
				default: break;
			}
		}

#endif

	}
}




