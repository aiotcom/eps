#include "stm32f1xx_hal.h"
#include "MsgHandler.h"
#include "stm32f1xx.h"
#include "delay.h"
#include "Rs485.h"
#include "usart.h"
#include "timer.h"
#include "M5310.h"
#include "nbiot.h"
#include "utils.h"
#include "coap.h"

/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2019-04-18
*
*	版本： 		V1.0
*
*	说明： 		基于NB-IoT网络温湿度传感器实验
*
*	修改记录：
************************************************************
************************************************************
************************************************************
**/
const char endpoint_name[] = "865820034613336;460043579906277";
uint32_t Ticks_SendData=0;
uint8_t SendOneNET[32];
float temp=5,humi=5;
void Platform_LED_Init(void);
void Platform_LED_Blue(void);
void Platform_LED_Green(void);
void Platform_LED_Red(void);
int main(void)
{
    HAL_Init();             					//初始化HAL库  
	Rs485_Init();								//初始化485
    Platform_LED_Init();
	M5310_Power_Init(); 						//M5310的复位控制IO初始化
	UART1_Init(115200);							//初始化串口1
	UART2_Init(9600);							//初始化串口2
	netdev_init();      						//初始化M5310
    init_miplconf(86400,coap_uri,endpoint_name);//上报注册码
	Subscription_esources();					//订阅 Object 组配置
	m5310_register_request();         //发出登陆请求
	delay_ms(1000);
	TIM2_Init(1000-1,64-1);						//初始化定时器2(1ms)
    Ticks_SendData = HAL_GetTick() + 10000; 
    Platform_LED_Green();
	while(1)
	{
		msgcode = Msg_Handler();				//返回数据解析
		USART_Data_Send();							//串口数据发送
        if(HAL_GetTick() > Ticks_SendData){
			memset((void*)SendOneNET,0,10);
			sprintf((char *)SendOneNET,"%0.2f",temp);
			m5310_notify_upload(&temp_uri,NBIOT_FLOAT,(char*)&SendOneNET[0],1,0);	//上传温度数据
			
			memset((void*)SendOneNET,0,10);
			sprintf((char *)SendOneNET,"%0.2f",humi);
			m5310_notify_upload(&humi_uri,NBIOT_FLOAT,(char*)&SendOneNET[0],1,0);	//上传湿度数据
            Ticks_SendData = HAL_GetTick() + 5000; //5秒后在次发送
            humi++;
            if(humi>50){
                humi = 30;
            }
            temp++;
            if(temp>60){
                temp = 10;
            }
        }
	}
}
void Platform_LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_AFIO_CLK_ENABLE();	
    __HAL_AFIO_REMAP_SWJ_NOJTAG(); 

    __HAL_RCC_GPIOB_CLK_ENABLE();           	                
    __HAL_RCC_GPIOA_CLK_ENABLE();           	              

    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP;  	                
    GPIO_Initure.Pull  = GPIO_PULLUP;          	                
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    	 	       

    GPIO_Initure.Pin   = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_15; 	
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

    GPIO_Initure.Pin   = GPIO_PIN_15; 	                        	 	            
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);


    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_15,GPIO_PIN_SET);	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15                      ,GPIO_PIN_SET);   
}
void Platform_LED_Blue(void)
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);
}
void Platform_LED_Red(void)
{
     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3            ,GPIO_PIN_RESET);
     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4|GPIO_PIN_15,GPIO_PIN_SET);
     HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15           ,GPIO_PIN_SET);
}
void Platform_LED_Green(void)
{
     HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15          ,GPIO_PIN_RESET);
     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3|GPIO_PIN_4,GPIO_PIN_SET);
}






