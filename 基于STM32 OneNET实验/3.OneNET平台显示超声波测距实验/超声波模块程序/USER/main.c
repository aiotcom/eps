#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "TM1640.h"	
#include "Rs485.h"
#include "SHT20.h"	
#include "delay.h"
#include "timer.h"
#include "Usart.h"
#include "HC_SR04.h"
#include "TM1640.h"

/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2019-03-18
*
*	版本： 		V1.0
*
*	说明： 		温湿度模块
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/
void RS485_HandlerCb(void);
uint8_t SendBuffer[3];
uint16_t Avg;        //测量返回的距离值
int main(void)
{
    HAL_Init();             					//初始化HAL库  
    TM1640_Init();			 //初始化TM1640，数码管驱动
    HCSR04_Init();			 //初始化超声波模块  
    Rs485_Init();											//初始化485
    USART3_Init(115200);              //用于调试
    UART1_Init(115200);								//初始化串口1,RS485通信
    TIM2_Init(10-1,64-1);		//初始化定时器2(中断周期10us)，超声波测量使用
    /*中断频率20HZ,关联RS485_HandlerCb()回调函数*/
    TIM3_Init(10000-1,320-1,RS485_HandlerCb);    
    while(1)
    {
        Avg =  HCSR04_StartMeasure(10);	//探测距离共探测10次
        send_LED_Display(0xC0,Avg,1);		//数码管显示距离
        HAL_Delay(500);                 //每500ms测量一次
        
        /*转换到数组中*/
        SendBuffer[0] = Avg>>8;
        SendBuffer[1] = (uint8_t)Avg;    
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
    if(!DataHandling_485(Addr_Ultrasonic)){	//是本机期望的485数据处理
        printf("get requery\r\n");
        /*485发送数据*/
        printf("%d,%d\r\n",SendBuffer[0],SendBuffer[1]);
        /*向WIFI节点发送数据*/
        Rs485_Send(Addr_Ultrasonic,Addr_WiFi,Ultrasonic_Dis,2,&SendBuffer[0]);
    }
}




