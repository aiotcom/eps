#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "delay.h"
#include "Relay.h"
#include "Rs485.h"
#include "delay.h"
#include "USART.h"
#include "TIMER.h"
#include "LED.h"
/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2019-12-11
*
*	版本： 		V2.0
*
*	说明： 		继电器程序
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/
uint8_t LED_State = 0;
int main(void)
{
    HAL_Init();      //初始化HAL库  
    LED_Init();   	//初始化模块
    Rs485_Init();		//初始化485
    UART1_Init(115200);	 //初始化串口1,用于485通信
    USART3_Init(115200); //用于调试
    printf("this usart3 print\r\n");
    while(1)
    {
        HAL_Delay(100);//延时100ms,每一100ms检测一次。
        if(!DataHandling_485(Addr_LED)){//是发给本机的命令
            printf("get cmd\r\n=%d\r\n",Rx_Stack.Data[0]);//调试打印
            LED_State = Rx_Stack.Data[0];//LED1灯命令
            if(LED_State){
                LED1_ON(); //LED1 亮
            }
            else{
                LED1_OFF();//LED2 灭                 
          }
        }        
    }
}




