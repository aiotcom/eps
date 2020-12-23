#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "delay.h"
#include "Rs485.h"
#include "usart.h"
#include "ADC.h"
#include "timer.h"
/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2019-12-18
*
*	版本： 		V2.0
*
*	说明： 		PM2.5
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/
/*
在电压小于3.5v大于0.5的情况下  ，电压和浓度成一个线型比例的。

浓度N= K* V;

K=N/V.约等于0.13；

当我们用stm32的12位的ADC采集数值后先变成电压，让value*3300/4096便是电压值，然后乘以0.13便得到mg/m3了
*/
uint8_t PM_Data[10];
#define SAMPLE_SIZE 20
int PM2P5_ADC[SAMPLE_SIZE],PM2P5_ADC_Count = 0;
uint16_t PM25_ugm3;
void RS485_HandlerCb(void);
uint32_t Data = 0;
/*
PM2.5传感器控制引脚初始化
*/
void PA_CTRL_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();						//开启GPIOA时钟

    GPIO_Initure.Pin=GPIO_PIN_1;          	//PA1
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		//复用推挽输出
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_LOW; //高速

    HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
    GPIOA->BRR = GPIO_PIN_1;
}
/*
排序算法
*/
void bubbleSort(int arr[], int n)
{
    int i, j, tmp;

    for(i = 0; i < n - 1; i++){
        for(j = 1; j < n; j++){
            if(arr[j] < arr[j - 1]){
                tmp        = arr[j];
                arr[j]     = arr[j - 1];
                arr[j - 1] = tmp;
            }
        }
    }
}

int main(void)
{
    float voltage;
    int i = 0;
    HAL_Init();             			//初始化HAL库
    ADC_Init();										//初始化ADC
    Rs485_Init();									//初始化485
    UART1_Init(115200);						//初始化串口1
    UART3_Init(115200);						//初始化串口3 用于调试功能
    PA_CTRL_Init();
    printf("this is uart 3 print\r\n");
    TIM2_Init(65530,640-1);//中断周期10us
    TIM3_Init(10000-1,640-1,RS485_HandlerCb);//计数时钟10HZ

	while(1)
	{
        for(PM2P5_ADC_Count = 0;PM2P5_ADC_Count < SAMPLE_SIZE;PM2P5_ADC_Count++){
            /*ADC取样*/
            GPIOA->BSRR = GPIO_PIN_1;//使能PM2.5模块
            TIM2_Delay10US(28);//280us
            PM2P5_ADC[PM2P5_ADC_Count] = Get_Adc(ADC_CHANNEL_3);//读取PM2.5传感器ADC数值
            TIM2_Delay10US(1);//10us
            GPIOA->BRR = GPIO_PIN_1;//禁止PM2.5模块
            TIM2_Delay10US(980);//9800us 
        }     
        bubbleSort(&PM2P5_ADC[0],SAMPLE_SIZE);//排序，数值小的在前面
        Data = 0;        
        for(i=1;i<(SAMPLE_SIZE-1);i++){//去除采样数据中的最高值及最低值
            Data += PM2P5_ADC[i];
        }
        Data = Data / (SAMPLE_SIZE-2);//减去最大和最小的两个
        voltage = (Data / 4096.0)*3.3;//转换成电压值
        
        PM25_ugm3 = (voltage*0.13)*1000;//转换成PM2.5值ug/m3        
        printf("PM25=%d\r\n",PM25_ugm3);//调试打印
        /*保持到数组中，将被发送到WIFI节点*/
        PM_Data[0] = PM25_ugm3>>8;
        PM_Data[1] = PM25_ugm3;
        HAL_Delay(500);//延时500ms,每500ms采集一次
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
    if(!DataHandling_485(Addr_PM2_5)){//收到给本机的请求
        printf("get requery\r\n");
        /*发送数据到WIFI*/  
        Rs485_Send(Addr_PM2_5,Addr_WiFi,PM2_5_Conc,2,PM_Data);	       
    }
}


