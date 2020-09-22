#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "TM1640.h"	
#include "SHT20.h"	
#include "delay.h"
#include "timer.h"
#include "Usart.h"
#include "RS485.h"
#include "ADC.h"
#include "USART.h"

void RS485_HandlerCb(void);
uint8_t SensorData[5];
int main(void)
{
    HAL_Init();             													//初始化HAL库  
		ADC_Init();																				//初始化ADC
    USART3_Init(115200);
		SHT2x_Init();																			//初始化SHT20
		TM1640_Init();																		//初始化TM1640
    Rs485_Init();
		TIM3_Init(10000-1,64-1,RS485_HandlerCb);					//初始化定时器中断周期64M/64/10000/ = 100HZ
    
    printf("sht20:usart3 print\r\n");
		while(1)
		{
				Display_Data();																//获取温湿度并显示到数码管上

        /*将传感器的值转换出来*/
        SensorData[0] = g_sht2x_param.TEMP_POLL;        
        SensorData[1] = g_sht2x_param.HUMI_HM;
        SensorData[2] = (uint8_t)LDR_Data;
        SensorData[3] = (uint8_t)(LDR_Data>>8);          
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
//	说明：		定时器3中断，回调函数，每100ms调用一次
//==========================================================
void RS485_HandlerCb(void)
{  
    if(!DataHandling_485(Addr_SHT20))
    {
        Rs485_Send(Addr_SHT20,Addr_WiFi,SHT20_All,4,SensorData);
    }        
}




