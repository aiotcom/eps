#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "usart.h"
#include "Rs485.h"
#include "timer.h"
#include "IIC.h"
#include "OLED.h"
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
*	说明： 		OLED屏显示卡号实验-HAL库版本
*
*	修改记录：	
************************************************************

************************************************************
************************************************************
**/
void RS485_HandlerCb(void);
uint8_t ucArray_ID[10];
uint8_t DispBuf[40];
uint8_t GetId = 0;
int main(void)
{
    HAL_Init();     //初始化HAL库
    Rs485_Init();		//初始化485
    UART1_Init(115200);		//初始化串口，485总线使用到这个串口
    TIM3_Init(10000-1,3200-1,RS485_HandlerCb);	//初始化定时器中断周期64M/32/10000/ = 2HZ
  
    OLED_Init();   //初始化OLED屏
   
    OLED_P8x16Str(0,0,"LF-RFID"); //显示HF-RFID标题 
    /*显示一个默认值*/
    sprintf((void*)&DispBuf[0],"ID:%02X%02X%02X%02X",0,0,0,0);    
    OLED_P8x16Str(0,3,DispBuf);
    while(1)
    {
        if(GetId)//GetId,在RS485_HandlerCb()函数中。
        {//接收到要显示的ID
            GetId = 0;//清空标志
            if(!(!ucArray_ID[0] && !ucArray_ID[1] && !ucArray_ID[2] && !ucArray_ID[3]))
            {//如果不是全零，则是正确的数据。
              sprintf((void*)&DispBuf[0],"ID:%02X%02X%02X%02X",  
                        ucArray_ID[0],ucArray_ID[1],ucArray_ID[2],ucArray_ID[3]);
                OLED_P8x16Str(0,3,DispBuf);//显示ID
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
      /*发送请求LF_RFID节点，回传卡号数据*/
        Rs485_Send(Addr_LCD,Addr_LF_RFID,LF_RFID_Get_ID,0,(void*)0);
        state = 1;//下个状态为状态1   
    }   
    else if(state == 1)
    {
        if(!DataHandling_485(Addr_LCD))		//接收数据
        {
            if(Rx_Stack.Src_Adr == Addr_LF_RFID)	//来自LF-RFID的数据
            {
                for(i=0;i<4;i++)//复制数据
                {
                    ucArray_ID[i] = Rx_Stack.Data[i];
                }
                GetId = 1;    
            }
        }
        state = 0;//回到状态0
    }
}




