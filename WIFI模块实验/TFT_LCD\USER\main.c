#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "delay.h"
#include "Rs485.h"
#include "usart.h"
#include "timer.h"
#include "LCD.h"
#include "lcd.h"
#include "text.h"
#include "gui.h"
#include "protocol.h"
#include "TIMER.h"
#include "usart.h"

extern void TIM3_Init(uint16_t arr,uint16_t psc,void (*cb)(void));
void Master_do_RS485Bus_Poll(void);
void RS485_HandlerCb(void);

struct _SHT20{
    uint8_t   temp;  																								//温度
    uint8_t   humi;  																								//湿度
    uint16_t  LightIntensity; 																		  //光强
};
struct _SHT20 SensorData;

uint32_t SensorDataUpdateIntervalCounter=0;
uint32_t TouchIntervalCounter;
uint8_t  update = 1;
int main(void)
{
    
    HAL_Init();             																				//初始化HAL库
		LCD_Init();             																				//初始化LCD显示器端口
		Lcd_Init();																											//初始化LCD
		Rs485_Init();																										//初始化485
		UART1_Init(115200);																							//初始化串口一
		UART2_Init(115200);																							//初始化串口一		
    USART3_Init(115200);																						//初始化串口一	
		LCD_Clear(WHITE);																								//清屏
    TIM3_Init(10000-1,64-1,RS485_HandlerCb);												//初始化定时器中断周期64M/64/10000/ = 100HZ
//	Touch_Init();
//	Touch_Adjust();  																								//屏幕校准 
		POINT_COLOR = RED;
		GUI_DrawTempTitle(1,10);
    GUI_DrawHumiTitle(1,68);
    GUI_DrawLinghtIntensityTitle(1,126);
    
    printf("TFT_LCD:usart3 printf\r\n");
		while(1)
		{	
        if(update)
        {
            update = 0;
            GUI_DrawTempData(106-32,10,SensorData.temp);
            GUI_DrawHumiData(106-32,68,SensorData.humi);
            GUI_DrawLinghtIntensityData(106-32,126,SensorData.LightIntensity);            
        }
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
    if(!DataHandling_485(Addr_LCD))
    {
        //Rs485_Send(Addr_SHT20,Addr_2_4G,SHT20_All,4,(uint8_t*)&SensorData);
        SensorData.temp = Rx_Stack.Data[0];
        SensorData.humi = Rx_Stack.Data[1];
        SensorData.LightIntensity = Rx_Stack.Data[2] + (Rx_Stack.Data[3]<<8);
        update = 1;
    }     
}



