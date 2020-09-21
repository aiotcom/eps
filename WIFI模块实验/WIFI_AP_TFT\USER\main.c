#include "stm32f1xx.h"
#include "string.h"
#include "delay.h"
#include "Usart.h"
#include "Rs485.h"
#include "WiFi.h"
#include "TIMER.h"
#include "WS2812b.h"
/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2019-01-01
*
*	版本： 		V1.0
*
*	说明： 		LORA模块
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/
uint8_t WiFiReceiveBuffer[32],lenght=0;
void RS485_HandlerCb(void);
int main(void)
{
	uint8_t LedState = 1;

	HAL_Init();    //初始化HAL库       
    USART3_Init(115200);            //printf 打印信息，PB10->TX,PB11->RX,通常只使用，TX功能 
    printf("WIFI:usart3 print\r\n");
    
    WS2812B_Init();
    UART2_Init(115200);
    WiFi_Init();                    //初始化WIFI模块
	Rs485_Init();				    //初始化485
    TIM3_Init(10000-1,640-1,RS485_HandlerCb);//中断频率64M/640/10000=10HZ，中断频率10HZ
	UART1_Init(115200);				//初始化串口1

	
	while(1)
	{
        if((lenght = WiFi_Receive(WiFiReceiveBuffer))!=0)//
        {
           Rs485_Send(Addr_WiFi,Addr_LCD,SHT20_All,4,WiFiReceiveBuffer);
        }           
	}
}
//==========================================================
//	函数名称：	void RS485_HandlerCb(void)
//
//	函数功能：	通过485总线获取数据处理函数
//
//	入口参数：	void
//
//	返回参数：	无
//
//	说明：		定时器3中断，回调函数，每100ms调用一次
//==========================================================
void RS485_HandlerCb(void)
{

}
