#include "delay.h"
#include "timer.h"
#include "Usart.h"
uint32_t HCSR04Count = 0;
TIM_HandleTypeDef TIM3_Handler;   	//定时器句柄 
static void (*TIMCb)(void);            //回调函数的指针
TIM_HandleTypeDef TIM2_Handler;      //定时器句柄 

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

    HAL_TIM_Base_Start_IT(&TIM2_Handler); 								//使能定时器2和定时器2更新中断：TIM_IT_UPDATE   
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
//===========================	===============================
void TIM2_IRQHandler(void)
{
		HCSR04Count++;
    HAL_TIM_IRQHandler(&TIM2_Handler);
}
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
void TIM3_Init(uint16_t arr,uint16_t psc,void (*cb)(void))
{  
    TIM3_Handler.Instance        = TIM3;                          //通用定时器3
    TIM3_Handler.Init.Prescaler  = psc;                     //分频系数
    TIM3_Handler.Init.CounterMode= TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Handler.Init.Period     = arr;                        //自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM3_Handler);
    if(cb != (void*)0){//如果回调函数不是空指针
        TIMCb = cb;  
    }
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
    if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM3中断   
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
    HAL_TIM_IRQHandler(&TIM3_Handler);
    TIMCb();
}









