#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "delay.h"
#include "Rs485.h"
#include "usart.h"
#include "timer.h"
#include "WiFi.h"
#include "WS2812B.h"
/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2019-04-22
*
*	版本： 		V1.0
*
*	说明： 		WiFi控制
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/
uint8_t Send_OneNET_Buffer[32];
uint8_t Temp,Humi;
uint32_t Ticks_SendOnenET=0;
int main(void)
{
    HAL_Init();             					//初始化HAL库  
	Rs485_Init();								//初始化485
	UART1_Init(115200);						    //初始化串口1
	UART2_Init(115200);						    //初始化串口2
    USART3_Init(115200);                        //用做调试串口
    printf("this is usart3 print\r\n");
    WS2812B_Init();                             //初始化全彩灯WS2812B
	
    WiFi_Init();							    //初始化WiFi
	TIM2_Init(1000-1,64-1);						//初始化定时器2(1ms)
    Ticks_SendOnenET = HAL_GetTick() + 2000;
    Temp = 16;
    Humi = 60;
	while(1)
	{
        if(HAL_GetTick() > Ticks_SendOnenET){
            sprintf((char *)Send_OneNET_Buffer,"%d%d",Temp,Humi);
            HAL_UART_Transmit(&UART2_Handler,Send_OneNET_Buffer,strlen((const char*)Send_OneNET_Buffer),1000);	//发送数据到OneNET
            
            Temp++;
            if(Temp>60){
                Temp = 16;
            }
            Humi++;
            if(Humi>90){
                Humi = 60;
            }
            printf("send onenet again\r\n");
            Ticks_SendOnenET = HAL_GetTick() + 2000;
        }
	}
}






