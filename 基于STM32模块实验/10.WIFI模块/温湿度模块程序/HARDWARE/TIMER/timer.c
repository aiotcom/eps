#include "ADC.h"
#include "timer.h"
#include "Usart.h"
#include "SHT20.h"	
#include "Rs485.h"

uint32_t Display_Count = 0;					//数码管显示计数值
uint8_t Temp[10],Humi[10],Light[10],Sum_Data[10];//温度、湿度、光强、数据集合
TIM_HandleTypeDef TIM3_Handler;   	//定时器句柄 

//==========================================================
//	函数名称：	TIM3_Init
//
//	函数功能：	通用定时器3中断初始化
//
//	入口参数：	
//							arr：自动重装值。
//							psc：时钟预分频数
//							定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//							Ft=定时器工作频率,单位:Mhz
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void TIM3_Init(uint16_t arr,uint16_t psc)
{  
    TIM3_Handler.Instance=TIM3;                          //通用定时器3
    TIM3_Handler.Init.Prescaler=psc;                     //分频系数
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Handler.Init.Period=arr;                        //自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
}

//==========================================================
//	函数名称：	HAL_TIM_Base_MspInit
//
//	函数功能：	定时器底册驱动，开启时钟，设置中断优先级
//
//	入口参数：	
//
//	返回参数：	无
//
//	说明：			此函数会被HAL_TIM_Base_Init()函数调用
//==========================================================
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断   
	}
}

//==========================================================
//	函数名称：	TIM3_IRQHandler
//
//	函数功能：	定时器3中断服务函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：			
//==========================================================
void TIM3_IRQHandler(void)
{
	Display_Count++;
	if(!DataHandling_485(Addr_SHT20))		//处理485数据
	{
		switch(Rx_Stack.Command)
		{
			case SHT20_Get_Temp:	//获取温度数据
				Rs485_Send(Addr_SHT20,Rx_Stack.Src_Adr,SHT20_Temp,1,Temp);		//发送温度数据
				break;
			case SHT20_Get_Humi:	//获取湿度数据
				Rs485_Send(Addr_SHT20,Rx_Stack.Src_Adr,SHT20_Humi,1,Humi);		//发送湿度数据
				break;
			case SHT20_Get_Light:	//获取光强数据
				Rs485_Send(Addr_SHT20,Rx_Stack.Src_Adr,SHT20_Light,2,Light);	//发送光强数据
				break;
			case SHT20_Get_All:		//获取温湿度+光强数据
				Rs485_Send(Addr_SHT20,Rx_Stack.Src_Adr,SHT20_All,4,Sum_Data);	//发送温湿度+光强数据
				break;
			default:break;
		}
	}
	HAL_TIM_IRQHandler(&TIM3_Handler);
}









