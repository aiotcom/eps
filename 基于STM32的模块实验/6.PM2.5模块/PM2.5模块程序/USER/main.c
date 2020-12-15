#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "delay.h"
#include "Rs485.h"
#include "usart.h"
#include "ADC.h"
#include "pwm.h"
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
uint32_t Data = 0;
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
uint8_t PM_Data[10];
#define SAMPLE_SIZE 20
int PM2P5_ADC[SAMPLE_SIZE],PM2P5_ADC_Count = 0;
uint16_t PM25_ugm3;
int main(void)
{
  float voltage;
  int i = 0;
  
  HAL_Init();             			//初始化HAL库
  ADC_Init();										//初始化ADC，采集传感器反馈的信号
  Rs485_Init();									//初始化485
  UART1_Init(115200);						//初始化串口1 485总线使用
  UART3_Init(115200);						//初始化串口3，用于调试
  PA_CTRL_Init();
  TIM3_Init(65530,640-1,(void*)0);//计数时钟10us

	while(1)
	{
        for(PM2P5_ADC_Count = 0;PM2P5_ADC_Count < SAMPLE_SIZE;PM2P5_ADC_Count++){
            /*ADC取样*/
            GPIOA->BSRR = GPIO_PIN_1;
            TIM3_DelayUS(28);//280us
            PM2P5_ADC[PM2P5_ADC_Count] = Get_Adc(ADC_CHANNEL_3);
            TIM3_DelayUS(1);//10us
            GPIOA->BRR = GPIO_PIN_1;
            TIM3_DelayUS(980);//9800us 
        }     
        bubbleSort(&PM2P5_ADC[0],SAMPLE_SIZE);//排序，数值小的在前面
        Data = 0;        
        for(i=1;i<(SAMPLE_SIZE-1);i++){//去除采样数据中的最高值及高低值
            Data += PM2P5_ADC[i];
        }
        Data = Data / (SAMPLE_SIZE-2);//减去最大和最小的两个
        voltage = (Data / 4096.0)*3.3;//转换成电压值
        
        PM25_ugm3 = (voltage*0.13)*1000;//单位 ug/m3  
        
        /*转换数据，通过485总线发送数据到显示器显示*/ 
        PM_Data[0] = PM25_ugm3>>8;PM_Data[1] = PM25_ugm3;
        Rs485_Send(Addr_PM2_5,Addr_LCD,PM2_5_Conc,2,PM_Data);	
        
        memset(PM_Data,0,10);//清空数组
        HAL_Delay(500);//每500ms采集一次
	}
}



