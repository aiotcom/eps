#include "FAN.h"
#include "delay.h"

TIM_HandleTypeDef 	TIM4_Handler;      	//定时器句柄 
TIM_OC_InitTypeDef 	TIM4_CH1Handler;	//定时器1通道1句柄


//==========================================================
//	函数名称：	TIM4_PWM_Init
//
//	函数功能：	TIM4 PWM部分初始化 
//
//	入口参数：	arr：自动重装值。
//							psc：时钟预分频数
//							定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft(单位S).
//							Ft=定时器工作频率,单位:Mhz
//							
//	返回参数：	无
//
//	说明：		
//==========================================================
void TIM4_PWM_Init(uint16_t psc,uint16_t arr)
{  
    TIM4_Handler.Instance=TIM4;         								//定时器4
    TIM4_Handler.Init.Prescaler=psc;       							//定时器分频
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;		//向上计数模式
    TIM4_Handler.Init.Period=arr;          							//自动重装载值
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM4_Handler);       							//初始化PWM

    TIM4_CH1Handler.OCMode=TIM_OCMODE_PWM1; 						//模式选择PWM1
    TIM4_CH1Handler.Pulse=0;           	 								//设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM4_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; 			//输出比较极性为高 
    HAL_TIM_PWM_ConfigChannel(&TIM4_Handler,&TIM4_CH1Handler,TIM_CHANNEL_1);//配置TIM4通道1
	
    HAL_TIM_PWM_Start(&TIM4_Handler,TIM_CHANNEL_1);			//开启PWM通道1
		PWM_SetTIM4Compare2(0);	//关闭风扇

}

//==========================================================
//	函数名称：	HAL_TIM_PWM_MspInit
//
//	函数功能：	定时器底层驱动，时钟使能，引脚配置
//							此函数会被HAL_TIM_PWM_Init()调用
//
//	入口参数：	htim:定时器句柄
//							
//	返回参数：	无
//
//	说明：			
//==========================================================
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	
  if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();					//使能定时器4
		__HAL_RCC_GPIOB_CLK_ENABLE();					//开启GPIOB时钟
		
		GPIO_Initure.Pin=GPIO_PIN_6;          //PB6
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;        //上拉
		GPIO_Initure.Speed=GPIO_SPEED_HIGH;   //高速
		
		HAL_GPIO_Init(GPIOB,&GPIO_Initure); 	
	}
}

uint8_t dir=1;
uint16_t led0pwmval=0; 

//==========================================================
//	函数名称：	TIM_SetTIM4Compare2
//
//	函数功能：	设置TIM4通道1的占空比
//
//	入口参数：	compare:比较值
//							
//	返回参数：	无
//
//	说明：		
//==========================================================
void TIM_SetTIM4Compare2(uint32_t compare)
{
	TIM4->CCR1=compare; 
}


//==========================================================
//	函数名称：	PWM_SetTIM4Compare2
//
//	函数功能：	PWM设置占空比  控制LED3的亮灭频率以及风扇的转动频率
//
//	入口参数：	compare:比较值
//							
//	返回参数：	无
//
//	说明：		
//==========================================================
void PWM_SetTIM4Compare2(uint32_t compare)
{
	TIM_SetTIM4Compare2(compare);	//修改比较值，修改占空比
}












































