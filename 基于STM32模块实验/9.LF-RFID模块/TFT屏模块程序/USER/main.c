#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "delay.h"
#include "usart.h"
#include "Rs485.h"
#include "TFT.h"
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
*	说明： 		TFT操作实验-HAL库版本
*
*	修改记录：	
************************************************************

************************************************************
************************************************************
**/
void RS485_HandlerCb(void);
uint8_t ucArray_ID[10];
uint8_t SendData[40];
uint8_t GetId = 0;
int main(void)
{
    uint8_t i = 0;
    HAL_Init();             //初始化HAL库
    TFT_Init();             //初始化TFT显示器端口
    Rs485_Init();			//初始化485
    UART1_Init(115200);		//初始化串口
    Lcd_Init();				//初始化LCD
    LCD_UI();				//背景UI
	TIM3_Init(10000-1,6400-1,RS485_HandlerCb);	//初始化定时器中断周期64M/64/10000/ = 1HZ
    while(1)
    {
        if(GetId)
        {
            GetId = 0;
            for(i = 0;i < 8;i ++)
            {//转换成ASCII
                if(i%2==0)
                {
                     DataRep((ucArray_ID[(i/2)]>>4),i,1,1);
                }
                else
                {
                    DataRep((ucArray_ID[(i/2)]&0x0f),i,0,1);
                }
            }

            if(!(!ucArray_ID[0] && !ucArray_ID[1] && !ucArray_ID[2] && !ucArray_ID[3]))
            {
                LCD_ShowString(60,80,SendData,0);			
            } 
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
	static uint16_t i = 0,state = 0;

    if(state == 0)
    {
        Rs485_Send(Addr_LCD,Addr_LF_RFID,LF_RFID_Get_ID,0,(void*)0);
         state = 1;   
    }   
    else if(state == 1)
    {
        if(!DataHandling_485(Addr_LCD))		//接收数据
        {
            if(Rx_Stack.Src_Adr == Addr_LF_RFID)	//来自LF-RFID的数据
            {
                for(i=0;i<4;i++)
                {
                    ucArray_ID[i] = Rx_Stack.Data[i];
                }
                GetId = 1;    
            }
        }
        state = 0;
    }
}




