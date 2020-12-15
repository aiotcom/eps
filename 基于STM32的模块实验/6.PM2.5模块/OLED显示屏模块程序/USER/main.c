#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "usart.h"
#include "Rs485.h"
#include "timer.h"
#include "OLED.h"
/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2019-01-17
*
*	版本： 		V2.0
*
*	说明： 		OLED显示PM2.5实验-HAL库版本
*
*	修改记录：	
************************************************************

************************************************************
************************************************************
**/
static uint8_t PMData[16];
static uint16_t pmdata = 0;
int main(void)
{
    HAL_Init();             	//初始化HAL库
		Rs485_Init();							//初始化485
    UART1_Init(115200);				//初始化串口,485总线使用这个串口
    OLED_Init();              //OLED屏初始化，初始化IIC总线，设置显示驱动芯片

    /*显示一个默认的数据0*/
    sprintf((char *)PMData,"PM2.5:%03d ug/m3",0);
    OLED_P8x16Str(0,3,PMData);	
  
		while(1)
		{
        if(!DataHandling_485(Addr_LCD))//接收是发给LCD显示的数据
        {
            pmdata = Rx_Stack.Data[0]*256 + Rx_Stack.Data[1];//获取485总线上的数据 
            sprintf((char *)PMData,"PM2.5:%03d ug/m3",pmdata);//将数据转换成字符串
            OLED_P8x16Str(0,3,PMData);//OLED屏显示数据	
        }	
		}
}




