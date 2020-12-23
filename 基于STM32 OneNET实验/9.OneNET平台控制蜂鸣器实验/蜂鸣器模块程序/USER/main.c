#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "delay.h"
#include "beep.h"
#include "Rs485.h"
#include "delay.h"
#include "USART.h"
#include "TIMER.h"
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
uint8_t BeepState = 0;
uint8_t BeepCmd = 0;
int main(void)
{
    HAL_Init();     //初始化HAL库  
    Beep_Init();		//初始化蜂鸣
    Rs485_Init();		//初始化485
    UART1_Init(115200);	//初始化串口1,用于485通信
    USART3_Init(115200);
    printf("this usart3 print\r\n");
    while(1)
    {
        if(!DataHandling_485(Addr_BEEP)){//是发给本机的指令
            printf("get data\r\n"); 
            BeepCmd = Rx_Stack.Data[0];//控制指令
            if(BeepCmd){
                BeepState = 1;//打开蜂鸣器
            }
            else{
                BeepState = 0;//关闭蜂鸣器                
            }
        }

        if(BeepState){
            HAL_Delay(1);//延时1ms，蜂鸣器的声音频率为500HZ
            BEEP_IO_TOGGLE();//蜂鸣器IO电平反转
        }
        else{
            BEEP_IO_LOW();//停止蜂鸣器
        }
    }
}




