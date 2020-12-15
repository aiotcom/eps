#include "stm32f1xx.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "EM4095.h"
#include "usart.h"
#include "Rs485.h"
/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2019-12-17
*
*	版本： 		V2.0
*
*	说明： 		LF-RFID模块
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/

void RS485_HandlerCb(void);
uint8_t GetId = 0;
uint8_t CardID[5]={0,0,0,0,0};
int main(void)
{
  HAL_Init();           //初始化HAL库  
  Rs485_Init();    		 //初始化485
  EM4095_Init();			 //初始化EM4095
  UART1_Init(115200);		 //初始化串口1，用于485通信
  TIM2_Init(2000-1,64-1);	 //初始化定时器2(2ms中断)，读EM4095芯片数据
  UART2_Init(115200);		 //初始化串口1
  USART3_Init(115200);   //调试串口
  printf("usart3 print\r\n");
  TIM3_Init(10000-1,6400-1,RS485_HandlerCb);	//初始化定时器中断周期64M/64/10000/ = 10HZ
	while(1)
	{
        GetId = EM4095_SearchHdr(CardID);//读取卡
        if(GetId){
            printf("===%d\r\n",*(uint32_t*)CardID);//调试打印
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
//	说明：		接收通过485总线发送过来的数据请求
//==========================================================
void RS485_HandlerCb(void)
{
    if(!DataHandling_485(Addr_LF_RFID))		//接收数据
    {
        if(Rx_Stack.Src_Adr == Addr_LCD)	//来自LF-RFID的数据
        {
            printf("get req\r\n");//调试打印
            Rs485_Send(Addr_LF_RFID,Addr_LCD,LF_RFID_ID,4,CardID);			//发送ID数据
            
            /*清空数据*/
            CardID[0] = 0;
            CardID[1] = 0;
            CardID[2] = 0;
            CardID[3] = 0;
        }
    }
}




