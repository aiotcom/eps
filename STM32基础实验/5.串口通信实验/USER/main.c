#include "stm32f1xx.h"
#include "UART.h"
#include "LED.h"
/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2020-01-19
*
*	版本： 		V2.0
*
*	说明： 		串口2通信实验-HAL库版本
*
*	修改记录：
************************************************************
************************************************************
************************************************************
**/
#define UART2_RX_SIZE  32 //串口2接收缓冲的大小
uint8_t UART2_RxBuf[UART2_RX_SIZE];  //串口2接收弹缓冲
uint8_t UART2_RxCnt = 0;  //串口2接收计数
char *usage="LED3 on->点亮LED3\r\nLED3 off->熄灭LED3\r\n";
int main(void)
{
    HAL_Init();				    //初始化HAL库，为随后用到的HAL_Delay()函数提供时钟。    
    UART2_Init(115200);   //串口1波特率115200
    LED_Init();           //只初始化了LED3  
    HAL_UART_Transmit(&UART2_Handler,(uint8_t*)usage,strlen(usage),10);//打印使用方法
    while(1)
    {
        if(UART2_RxCnt){
            HAL_Delay(100);
            HAL_UART_Transmit(&UART2_Handler,UART2_RxBuf,UART2_RxCnt,10);
            if(strstr((void*)&UART2_RxBuf[0],"LED3 on")){//收到LED3 on指示，点亮LED3
                LED3_ON();
            }
            if(strstr((void*)&UART2_RxBuf[0],"LED3 off")){//收到LED3 off指示，熄灭LED3
                LED3_OFF();
            }            
            UART2_RxCnt = 0;
        }
    }
}
//==========================================================
//	函数名称：	USART2_IRQHandler()
//
//	函数功能：	串口2中断服务程序
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：串口2的接收中断，函数名字不可随意修改。
//==========================================================
void USART2_IRQHandler(void)
{
    uint8_t res;
    if((__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_RXNE)!=RESET)){  
        /*是串口的接收中断*/
        res = USART2->DR;
        UART2_RxBuf[UART2_RxCnt++] = res;
      
        if(UART2_RxCnt > (UART2_RX_SIZE-1)){//如果接收数据大于缓冲区，UART2_RxCnt清零。
            UART2_RxCnt = 0;
        }
    }
    HAL_UART_IRQHandler(&UART2_Handler);//清除中断标志位
}



