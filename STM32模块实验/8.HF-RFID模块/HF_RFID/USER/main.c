#include "stm32f1xx.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"	
#include "RC522.h"
#include "Rs485.h"	
#include "string.h"
#include "timer.h"
/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2018-01-01
*
*	版本： 		V1.0
*
*	说明： 		按键函数操作实验-HAL库版本
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/
void RS485_HandlerCb(void);
uint8_t DataToIC_Card[16]={1,2,3,5,6,7,8,9,0,1,2,3,4,5,6};//保存写入IC卡的信息
uint8_t kv;
uint8_t count = 0;
uint8_t SendBuf[16];
int main(void)
{
    uint8_t index;  
    uint8_t i;
    HAL_Init();             //初始化HAL库

    USART3_Init(115200);		//初始化串口    
    printf("usart3 print\r\n");
    RC522_Init ();     //RC522模块初始化    
    delay_ms(200);	

    Rs485_Init();
    UART1_Init(115200);		//初始化串口
    
    TIM3_Init(10000-1,640-1,RS485_HandlerCb);	//初始化定时器中断周期64M/64/10000/ = 10HZ
	while(1)
	{
        if(IC_Card_Search(SendBuf))//IC卡检测  
        {
            //printf("%d,%d,%d,%d\r\n",SendBuf[0],SendBuf[1],SendBuf[2],SendBuf[3]);
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
    if(!DataHandling_485(Addr_HF_RFID))		//接收数据
    {
        if(Rx_Stack.Src_Adr == Addr_LCD)	//来自LF-RFID的数据
        {
            Rs485_Send(Addr_HF_RFID,Addr_LCD,HF_RFID_ID,4,SendBuf);
        }
    }
}
