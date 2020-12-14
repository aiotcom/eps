#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "timer.h"
#include "delay.h"
#include "Usart.h"
#include "Rs485.h"
#include "Ble.h"
#include "stdio.h"

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
*	说明： 		蓝牙库函数操作实验-HAL库版本
*
*	修改记录：
97 7B F3 62 C4 84
************************************************************
************************************************************
************************************************************
**/


int main(void)
{ 
    uint8_t ledstete = 0;
    HAL_Init();             					//初始化HAL库  
    USART3_Init(115200);
    printf("this is usart3 print\r\n");    
	UART2_Init(9600);							//初始化串口2
	Rs485_Init();   							//初始化485
	TIM3_Init(1000-1 ,64-1);					//初始化定时器3
	Ble_Init();									//初始化蓝牙
    
    USART2_RX_STA = 0;
    while(!(strstr((const char*)USART2_RX_BUF,(const char*)"Connected")));//蓝牙模块如果被连接，返回字符 Connected
    printf("connected\r\n");
    
    /*底座灯亮起绿色*/
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);    
	while(1)
	{
        if(USART2_RX_STA){//接收完成           
            HAL_Delay(50);//延时一会等待接收完成
            
            if(strstr((const char*)USART2_RX_BUF,(const char*)"blue")){
                 /*底座灯亮起蓝色*/
                 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4 ,GPIO_PIN_RESET);
                 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3 ,GPIO_PIN_SET);
                 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);
            }

            if(strstr((const char*)USART2_RX_BUF,(const char*)"red")){
                 /*底座灯亮起红色*/                
                 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4 ,GPIO_PIN_SET);
                 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3 ,GPIO_PIN_RESET);
                 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);                
            }            
            USART2_RX_STA=0;
            memset((void*)USART2_RX_BUF,0,USART1_REC_LEN);
         
        }//if(USART2_RX_STA&UART_RX_DONE_MASK)        
	}
}

