/**
  ******************************************************************************
  * @file    Templates/Src/stm32f1xx_hal_msp.c
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    29-April-2016
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspInit(void)
{
}

/**
  * @brief  DeInitializes the Global MSP.
  * @param  None  
  * @retval None
  */
void HAL_MspDeInit(void)
{
}

//==========================================================
//	函数名称：	Platform_RGB_LED_Blink
//
//	函数功能：	初始化底座的RGB LED灯的控制IO,PB3、PB4、PB15。控制输出电平占空比为50%，周期为1s。
//              该函数在systick中断处理函数中调用。
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void Platform_RGB_LED_Blink(void)
{
    static uint8_t  InitFlg = 0;
    static uint16_t count   = 500; 
    static uint8_t  seq     = 0;
    
    GPIO_InitTypeDef GPIO_Initure;
    
    if(InitFlg == 0)
    {
        InitFlg = 1;

        __HAL_RCC_AFIO_CLK_ENABLE();	
        __HAL_AFIO_REMAP_SWJ_NOJTAG(); 
        
        __HAL_RCC_GPIOB_CLK_ENABLE();           	                //开启GPIOB时钟
        __HAL_RCC_GPIOA_CLK_ENABLE();           	                //开启GPIOB时钟

        GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP;  	                //推挽输出
        GPIO_Initure.Pull  = GPIO_PULLUP;          	                //上拉
        GPIO_Initure.Speed = GPIO_SPEED_HIGH;    	 	            //高速
        
        GPIO_Initure.Pin   = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_15; 	//PB3,PB4
        HAL_GPIO_Init(GPIOB,&GPIO_Initure);

        GPIO_Initure.Pin   = GPIO_PIN_15; 	                        //PA15 	 	            
        HAL_GPIO_Init(GPIOA,&GPIO_Initure);
        
        ////PB3,PB4,PA15 置1，默认初始化后灯灭 
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_15,GPIO_PIN_SET);	
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15                      ,GPIO_PIN_SET);        
    }
    else
    {
        if((++count) >= 500)//由于systick中断1ms中断一次，500次计数刚好一秒。
        {
            count = 0;
            switch(seq)
            {
                case 0:
                     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4 ,GPIO_PIN_RESET);
                     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3 ,GPIO_PIN_SET);
                     HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);

                     seq++;
                break;
                case 1:
                     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3            ,GPIO_PIN_RESET);
                     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4|GPIO_PIN_15,GPIO_PIN_SET);
                     HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15           ,GPIO_PIN_SET);
                
                     seq++;
                break;
                case 2:
                     HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15          ,GPIO_PIN_RESET);
                     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3|GPIO_PIN_4,GPIO_PIN_SET);
                
                     seq = 0;
                break;
            }
            
        }            
    }
}
/**
  * @brief  Initializes the PPP MSP.
  * @param  None
  * @retval None
  */
/*void HAL_PPP_MspInit(void)
{*/
/*}*/

/**
  * @brief  DeInitializes the PPP MSP.
  * @param  None  
  * @retval None
  */
/*void HAL_PPP_MspDeInit(void)
{*/
/*}*/

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
