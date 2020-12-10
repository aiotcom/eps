#include "stm32f1xx.h"
#include "UART.h"	

UART_HandleTypeDef UART2_Handler; 								//UART句柄


//==========================================================
//	函数名称：	UART2_Init()
//
//	函数功能：	串口2初始化
//
//	入口参数：	bound:波特率
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void UART2_Init(int bound)
{	
	//UART 初始化设置
	UART2_Handler.Instance=USART2;					    						//USART2
	UART2_Handler.Init.BaudRate=bound;				    					//波特率
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   		//字长为8位数据格式
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    			//一个停止位
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    			//无奇偶校验位
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   		//无硬件流控
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    				//收发模式
  UART2_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&UART2_Handler);					    						//HAL_UART_Init()会使能UART1
    
  __HAL_UART_ENABLE_IT(&UART2_Handler, UART_IT_RXNE);    
}

//==========================================================
//	函数名称：	HAL_UART_MspInit()
//
//	函数功能：	UART底层初始化，时钟使能，引脚配置，中断配置
//
//	入口参数：	huart:串口句柄
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  /*	GPIO端口设置	*/
	GPIO_InitTypeDef GPIO_Initure;
 
  if(huart->Instance==USART2)							//如果是串口2，进行串口2 MSP初始化
	{
			__HAL_RCC_GPIOA_CLK_ENABLE();							//使能GPIOA时钟
			__HAL_RCC_USART2_CLK_ENABLE();						//使能USART2时钟
			
		 // __HAL_RCC_AFIO_CLK_ENABLE();

			GPIO_Initure.Pin=GPIO_PIN_2;							//PA2
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;				//复用推挽输出
			GPIO_Initure.Pull=GPIO_PULLUP;						//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;	//高速
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   		//初始化PA2

			GPIO_Initure.Pin=GPIO_PIN_3;							//PA3
			GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;			//模式要设置为复用输入模式！	
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   		//初始化PA3
			
			HAL_NVIC_EnableIRQ(USART2_IRQn);					//使能USART2中断通道
			HAL_NVIC_SetPriority(USART2_IRQn,1,1);		//抢占优先级1，子优先级1
  }
}




