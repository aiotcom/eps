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
uint8_t WiFiSendBuffer[32],UpdataFlg = 0;
uint8_t WiFiReceiveBuffer[32],lenght=0;
void RS485_HandlerCb(void);
int main(void)
{
	uint8_t LedState = 1;

	HAL_Init();                     //初始化HAL库       
    USART3_Init(115200);            //printf 打印信息，PB10->TX,PB11->RX,通常只使用，TX功能 
    printf("WIFI:usart3 print\r\n");//调试打印信息
    
    WS2812B_Init();
    UART2_Init(115200);
    WiFi_Init();                    //WIFI模块初始化
	Rs485_Init();				    //初始化485
    TIM3_Init(10000-1,640-1,RS485_HandlerCb);//中断频率64M/640/10000=10HZ，中断频率10HZ
	UART1_Init(115200);				//初始化串口1
	
	while(1)
	{
        if(UpdataFlg)
        {//有数据更新发送数据，RS485_HandlerCb()函数中更新UpdataFlg
            UpdataFlg = 0;
            WiFi_Send(WiFiSendBuffer,4);
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
    static uint32_t PollCounter=0;
    static uint8_t state = 0;
    
    if(state == 0)
    {
        if((++PollCounter) >= 20)
        {//2000ms发送一次轮询指令
            PollCounter = 0;
            Rs485_Send(Addr_WiFi,Addr_SHT20,SHT20_Get_All,  0,(void*)0); 
            state = 1;
        }
    }
    else if(state == 1)
    {
        if(!DataHandling_485(Addr_WiFi))
        {
            //printf("=%d,%d\r\n",Rx_Stack.Data[0],Rx_Stack.Data[1]);
            memcpy((void*)WiFiSendBuffer,Rx_Stack.Data,4);
            UpdataFlg = 1;
        }
        state = 0;    
    }
}
