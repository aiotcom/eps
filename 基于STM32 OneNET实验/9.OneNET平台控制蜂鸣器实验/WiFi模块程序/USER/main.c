#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "delay.h"
#include "Rs485.h"
#include "usart.h"
#include "timer.h"
#include "WiFi.h"

/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2020-03-19
*
*	版本： 		V2.0
*
*	说明： 		WiFi控制
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/
void RS485_HandlerCb(void);
uint32_t SensorData[3]={0};
uint8_t SendBuffer[16];
uint16_t Beep_Ctrl = 0;
int main(void)
{
    HAL_Init();             				//初始化HAL库  
    Rs485_Init();										//初始化485
    UART1_Init(115200);							//初始化串口1 485总线使用
    UART2_Init(115200);							//初始化串口2
    USART3_Init(115200);						//调试串口   
    printf("this usart3 print\r\n");
    WiFi_Init();											//初始化WiFi，并连接OneNET
    while(1)
    {
        if(USART2_RX_STA){
            HAL_Delay(50);//延时50ms等待接收完成
            printf("get cmd=%s\r\n",USART2_RX_BUF);//调试打印
            if(strstr((void*)&USART2_RX_BUF[0],(const char*)"$BEEP,1")){//继电器1 打开
                Beep_Ctrl = 1;//打开继电器控制命令
                Rs485_Send(Addr_WiFi,Addr_BEEP,BEEP_CTRL,1,(void*)&Beep_Ctrl);//发送命令控制继电器1             
            }
            else if(strstr((void*)&USART2_RX_BUF[0],(const char*)"$BEEP,0")){//继电器1 关闭
                Beep_Ctrl = 0;//关闭继电器控制命令
                Rs485_Send(Addr_WiFi,Addr_BEEP,BEEP_CTRL,1,(void*)&Beep_Ctrl);//发送命令控制继电器1             
            }            
            USART2_RX_STA = 0;//清空串口接收计数器
            memset((void*)USART2_RX_BUF,0,USART2_REC_LEN);//清空接收缓冲
        }
    }
}





