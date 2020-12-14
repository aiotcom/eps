#include "timer.h"
#include "delay.h"
#include "stm32f1xx.h"

uint32_t Task_Count = 0;				//分时任务计数值
TIM_HandleTypeDef TIM2_Handler; //定时器句柄 

//==========================================================
//	函数名称：	TIM2_Init
//
//	函数功能：	通用定时器3中断初始化
//
//	入口参数：	arr：自动重装值。
//              psc：时钟预分频数
//
//	返回参数：	无
//
//	说明：		定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.   Ft=定时器工作频率,单位:Mhz
//==========================================================
void TIM2_Init(unsigned int arr,unsigned int psc)
{  
    TIM2_Handler.Instance=TIM2;                          //通用定时器2
    TIM2_Handler.Init.Prescaler=psc;                     //分频系数
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Handler.Init.Period=arr;                        //自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM2_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM2_Handler); //使能定时器2和定时器2更新中断：TIM_IT_UPDATE   
}

//==========================================================
//	函数名称：	HAL_TIM_Base_MspInit
//
//	函数功能：	使能定时器
//
//	入口参数：	
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断   
	}
}

//==========================================================
//	函数名称：	TIM2_IRQHandler
//
//	函数功能：	定时器2中断服务函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void TIM2_IRQHandler(void)
{
	Task_Count++;
	HAL_TIM_IRQHandler(&TIM2_Handler);
}





