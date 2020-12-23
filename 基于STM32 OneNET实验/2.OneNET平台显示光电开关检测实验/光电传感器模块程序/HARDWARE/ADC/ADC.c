#include "ADC.h"
#include "delay.h"
#include "stm32f1xx.h"

ADC_HandleTypeDef hadc1;		//ADC句柄

//==========================================================
//	函数名称：	ADC_Init()
//
//	函数功能：	初始化ADC
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ADC_Init(void)
{ 
		RCC_PeriphCLKInitTypeDef ADC_CLKInit;
		
		ADC_CLKInit.PeriphClockSelection=RCC_PERIPHCLK_ADC;			//ADC外设时钟
		ADC_CLKInit.AdcClockSelection=RCC_ADCPCLK2_DIV6;				//分频因子6时钟
		HAL_RCCEx_PeriphCLKConfig(&ADC_CLKInit);								//设置ADC时钟
	
    hadc1.Instance=ADC1;
    hadc1.Init.DataAlign=ADC_DATAALIGN_RIGHT;             	//右对齐
    hadc1.Init.ScanConvMode=DISABLE;                      	//非扫描模式
    hadc1.Init.ContinuousConvMode=DISABLE;                	//关闭连续转换
    hadc1.Init.NbrOfConversion=1;                         	//1个转换在规则序列中 也就是只转换规则序列1 
    hadc1.Init.DiscontinuousConvMode=DISABLE;             	//禁止不连续采样模式
    hadc1.Init.NbrOfDiscConversion=0;                     	//不连续采样通道数为0
    hadc1.Init.ExternalTrigConv=ADC_SOFTWARE_START;       	//软件触发
    HAL_ADC_Init(&hadc1);                                 	//初始化 
	
		HAL_ADCEx_Calibration_Start(&hadc1);					 					//校准ADC
}


//==========================================================
//	函数名称：	HAL_ADC_MspInit()
//
//	函数功能：	ADC底层驱动，引脚配置，时钟使能
//							此函数会被HAL_ADC_Init()调用
//
//	入口参数：	hadc:ADC句柄
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();						//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}


//==========================================================
//	函数名称：	Get_Adc
//
//	函数功能：	获得ADC值
//
//	入口参数：	ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//
//	返回参数：	转换结果
//
//	说明：			
//==========================================================
uint16_t Get_Adc(uint32_t ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //通道
    ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_239CYCLES_5;      //采样时间               
    HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        				//通道配置
	
    HAL_ADC_Start(&hadc1);                               				//开启ADC
	
    HAL_ADC_PollForConversion(&hadc1,10);                				//轮询转换
 
		return (uint16_t)HAL_ADC_GetValue(&hadc1);	         				//返回最近一次ADC1规则组的转换结果
}

//==========================================================
//	函数名称：	Get_Adc_Average
//
//	函数功能：	获取指定通道的转换值，取times次,然后平均 
//
//	入口参数：	times:获取次数
//
//	返回参数：	通道ch的times次转换结果平均值
//
//	说明：			
//==========================================================
double temp_val=0;   		//存放获取到的AD值
uint32_t lux=0;   		 	//光照强度值
uint8_t t;
uint16_t Get_Adc_Average(uint32_t ch,uint8_t times)
{
	temp_val=0;
	
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		HAL_Delay(5);
	}
	temp_val = (temp_val/times); 	//实际AD值
	
	return (temp_val);
} 



