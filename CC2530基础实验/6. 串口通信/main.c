/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2019-12-25
*
*	版本： 		V1.0
*
*	说明： 		串口实验
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/

#include <ioCC2530.h>
#include <stdlib.h>
#include <string.h>
#include "Usart.h"
#include "LED.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"
int UART0_RxCnt = 0;
//==========================================================
//	函数名称：	main()
//
//	函数功能：	程序入口函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================

void main(void)
{
    Hal_Init_32M();   //初始化32M时钟
    Init_Uart0();     //初始化串口0，115200BPS的波特率
    LED3_Init();       //lED模块LED灯控制IO初始化

    UR0SendString("LED3 on 点亮LED3\r\nLED3 off 熄灭LED3");

    while(1)              
    {
        if(UART0_RxCnt)
        {
						delay_ms(50);
						if(strstr((void*)&USART0_RX_BUF[0],(char const*)"LED3 on")){
								LED3_ON();
						}
						if(strstr((void*)&USART0_RX_BUF[0],(char const*)"LED3 off")){
								LED3_OFF();						
						}						
            memset(USART0_RX_BUF,0,USART0_RX_SIZE);
						UART0_RxCnt = 0;
        }
    }
}
//==========================================================
//	函数名称：	UR0_RecvInt()
//
//	函数功能：	UR0接收中断服务函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
#pragma vector = URX0_VECTOR
__interrupt void UR0_RecvInt()
{
  URX0IF = 0;                           //清除RX接收中断标志
	USART0_RX_BUF[UART0_RxCnt++]=U0DBUF ;
	if(UART0_RxCnt>(USART0_RX_SIZE-1)){
		UART0_RxCnt = 0;
	}
}


