#include "stm32f1xx.h"
#include "EM4095.h"
#include "timer.h"
#include "delay.h"
#include "string.h"
#include "rs485.h"

#define SET_EM4095_MODE_OPERATION()     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET)	
#define SET_EM4095_MODE_SLEEP()         HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET)	

#define EM4095_MOD_DISABLE()            HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET)
#define EM4095_MOD_ENABLE()             HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET)

#define READ_RDY_CLK()                  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)
//==========================================================
//	函数名称：	__us
//
//	函数功能：	一个单位的微秒延时函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================    
void __us(void)
{
    uint8_t usd = 8;
    while(usd)
    {
        usd--;
    }
}
//==========================================================
//	函数名称：	EM4095_IO_Init
//
//	函数功能：	EM4095-IO口初始化
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void EM4095_IO_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOA时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOB时钟
	
    GPIO_Initure.Pin   = GPIO_PIN_1; 	        //GPIO_PIN_1 MOD
    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP; //推挽输出
    GPIO_Initure.Pull  = GPIO_PULLUP;         //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    	//高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    EM4095_MOD_DISABLE();
    
    GPIO_Initure.Pin   = GPIO_PIN_6; 	        //GPIO_PIN_6 SHD
    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP; //推挽输出
    GPIO_Initure.Pull  = GPIO_PULLUP;         //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    	//高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    SET_EM4095_MODE_SLEEP();
    
    GPIO_Initure.Pin   = GPIO_PIN_0; 	        //GPIO_PIN_0  DEMOD
    GPIO_Initure.Mode  = GPIO_MODE_INPUT;  	  //输入
    GPIO_Initure.Pull  = GPIO_PULLUP;         //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    	//高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    
    GPIO_Initure.Pin   = GPIO_PIN_7; 	        //GPIO_PIN_7 RDY_CLK
    GPIO_Initure.Mode  = GPIO_MODE_INPUT;    	//输入
    GPIO_Initure.Pull  = GPIO_PULLUP;         //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    	//高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);    
}

//==========================================================
//	函数名称：	DEMOD_InputFilter
//
//	函数功能：	对DEMODE 的IO输入进行简单的滤波
//
//	入口参数：	void
//
//	返回参数：	返回 0,输入为低电平。返回 1，输入为高电平。
//
//	说明：		
//==========================================================
uint8_t DEMOD_Input(void)
{
    static uint8_t ret = 1;
    if(READ_DEMOD())
    {
        __us();           //延时一个微秒
        __us();           //延时一个微秒        
        if(READ_DEMOD())
        {
            ret =  1;
        }
    }

    if(!READ_DEMOD())
    {
        __us();           //延时一个微秒
        __us();           //延时一个微秒        
        if(!READ_DEMOD())
        {
            ret = 0;
        }
    }
    return ret;
}

//==========================================================
//	函数名称：	EM4095_UID_Parse
//
//	函数功能：	接收数据
//
//	入口参数：	uint8_t *pdata
//
//	返回参数：	
//
//	说明：		
//==========================================================
uint8_t  RawCardData[11][5],RawCardDataColIdx=0,RawCardDataRowIdx=0;
uint8_t uid[8],uidindex=0;
uint8_t EM4095_UID_Parse(uint8_t* pdata)
{
    uint8_t ret = 0,XOR,colxor0,colxor1,colxor2,colxor3;
    uint8_t parsestate = 0,errflag=0;
    uidindex = 0;
    RawCardDataColIdx = 0;
    RawCardDataRowIdx = 0;
    memset((void*)uid,0,8);

    if(parsestate == 0)
    {
        /*
         对数据进行行检验
        */
        while(RawCardDataColIdx < 10)
        {
            XOR               = 0;
            RawCardDataRowIdx = 0;
            while(RawCardDataRowIdx < 4)
            {
                XOR ^= RawCardData[RawCardDataColIdx][RawCardDataRowIdx++];
            }

            if(XOR != RawCardData[RawCardDataColIdx][4])
            {
                errflag = 1;
                break;
            }
            RawCardDataColIdx++;
        }
    }

    parsestate = errflag?0xff:(++parsestate);

    if(parsestate == 1)
    {
       /*
       对数据进行列检验
       */
        colxor0           = 0;
        colxor1           = 0;
        colxor2           = 0;
        colxor3           = 0;
        RawCardDataColIdx = 0;

        while(RawCardDataColIdx < 10)
        {
            colxor0 ^= RawCardData[RawCardDataColIdx][0];
            colxor1 ^= RawCardData[RawCardDataColIdx][1];
            colxor2 ^= RawCardData[RawCardDataColIdx][2];
            colxor3 ^= RawCardData[RawCardDataColIdx][3];
            RawCardDataColIdx++;
        }

        if(  (colxor0 != RawCardData[10][0])||(colxor1 != RawCardData[10][1])
           ||(colxor2 != RawCardData[10][2])||(colxor3 != RawCardData[10][3]))
        {
            errflag = 1;
        }
    }

    parsestate = errflag?0xff:(++parsestate);

    if(parsestate == 2)
    {
        /*
        组装数据
        */
        uidindex          = 0;
        RawCardDataColIdx = 0;

        while(RawCardDataColIdx < 10)
        {
            uid[uidindex] |= (RawCardData[RawCardDataColIdx][0] << 3) + (RawCardData[RawCardDataColIdx][1] << 2)
                            +(RawCardData[RawCardDataColIdx][2] << 1) + (RawCardData[RawCardDataColIdx][3] << 0);
            RawCardDataColIdx++;
            
            uid[uidindex] = uid[uidindex] << 4;
            
            uid[uidindex] |= (RawCardData[RawCardDataColIdx][0] << 3) + (RawCardData[RawCardDataColIdx][1] << 2)
                            +(RawCardData[RawCardDataColIdx][2] << 1) + (RawCardData[RawCardDataColIdx][3] << 0);
            RawCardDataColIdx++; 
            
            uidindex++;            
        }

        memcpy((void*)pdata,(const void*)uid,5);
        ret = 1;
    }

    return ret;
}

//==========================================================
//	函数名称：	EM4095_SearchHdr
//
//	函数功能：	接收数据
//
//	入口参数：	uint8_t *pcardid,pcardid如果有数据将保存读到的数据
//
//	返回参数：	返回 GET_ID,读取数据，返回 NOT_ID
//
//	说明：			
//==========================================================
uint16_t TimerCnt=0;
uint8_t  HdrBitSetCounter=0;
uint8_t  CardID_Buffer[100],CardID_Index=0;
uint16_t Hdr_LevelCounter = 0;
uint16_t timeout;
uint8_t EM4095_SearchHdr(uint8_t *pcardid)
{
    static uint8_t Is_CardExist;
    uint8_t state    = 0;
    Hdr_LevelCounter = 0;
    RawCardDataColIdx = 0;
    RawCardDataRowIdx = 0;    

    if(!DEMOD_Input())
    {
        TIM2_Delay(100);
        if(DEMOD_Input())
        {
            return NOT_ID;
        }
    }
    else
    {
        Is_CardExist = 0;
        return NOT_ID;
    }

    while(1)
    {
        if(READ_DEMOD())
        {
            TIM2_IntFlag = 0;
            __HAL_TIM_SET_COUNTER(&TIM2_Handler,0); 
            __HAL_TIM_SET_AUTORELOAD(&TIM2_Handler,550);
            __HAL_TIM_ENABLE(&TIM2_Handler);
            while(READ_DEMOD()&&(TIM2_IntFlag == 0));
            
            __HAL_TIM_DISABLE(&TIM2_Handler);
            TimerCnt = __HAL_TIM_GET_COUNTER(&TIM2_Handler);
            
            if(TimerCnt >= 484)//512*(3/4)
            {
                state = 1;                                    //找到一个数据头
                __HAL_TIM_SET_COUNTER(&TIM2_Handler,0);       //清理计算器的值
                __HAL_TIM_SET_AUTORELOAD(&TIM2_Handler,500);  //设置中断周期
                __HAL_TIM_ENABLE(&TIM2_Handler);              //使用定时器
                timeout = 380;
                TIM2_IntFlag     = 0;
                HdrBitSetCounter = 0;
                memset((void*)CardID_Buffer,0,50);
                CardID_Index = 0;
                Is_CardExist = 1;
                break;
            }
            else
            {
                Is_CardExist = 0;
                return NOT_ID;
            }
        }
    }

    if(Is_CardExist)
    {
        if(state == 1) //等待数据头
        {
            while(1)
            {
                if((__HAL_TIM_GET_COUNTER(&TIM2_Handler) >= timeout))
                {
                    TIM2_IntFlag = 0;

                    if(READ_DEMOD())
                    {
                        HdrBitSetCounter++;
                        while(READ_DEMOD());
                    }
                    else
                    {
                        break;
                    }
                    __HAL_TIM_DISABLE(&TIM2_Handler); 
                    __HAL_TIM_SET_COUNTER(&TIM2_Handler,0);       //清理计算器的值
                    __HAL_TIM_SET_AUTORELOAD(&TIM2_Handler,1000); //设置预计的超时的时间
                    __HAL_TIM_ENABLE(&TIM2_Handler);              //使用定时器  
                    timeout = 384;
                    if(HdrBitSetCounter == 8)
                    {
                        state = 2;        //开始接收数据
                        break;
                    }
                }
            }
        }

        if(state == 2)
        {
            while(1)
            {
                if((__HAL_TIM_GET_COUNTER(&TIM2_Handler) >= timeout))
                {
                    TIM2_IntFlag = 0;
                    if(READ_DEMOD())
                    {
                        CardID_Buffer[CardID_Index++] = 1;
                        RawCardData[RawCardDataColIdx][RawCardDataRowIdx++] = 1;
                        while(READ_DEMOD());
                    }
                    else
                    {
                        CardID_Buffer[CardID_Index++] = 0;
                        RawCardData[RawCardDataColIdx][RawCardDataRowIdx++] = 0;
                        while(!READ_DEMOD());
                    }

                    if(RawCardDataRowIdx == 5)
                    {
                        RawCardDataRowIdx = 0;
                        RawCardDataColIdx++;
                    }
                    __HAL_TIM_DISABLE(&TIM2_Handler); 
                    __HAL_TIM_SET_COUNTER(&TIM2_Handler,0);       //清理计算器的值
                    __HAL_TIM_SET_AUTORELOAD(&TIM2_Handler,1000); //设置中断周期
                    __HAL_TIM_ENABLE(&TIM2_Handler);              //使用定时器  
                    timeout = 384;
                }

                if(CardID_Index >= 55)
                {
                    __HAL_TIM_DISABLE(&TIM2_Handler);
                    state = 3;
                    break;
                }
            }
        }

        if(state == 3)
        {
            if(EM4095_UID_Parse(pcardid))
            {
                return GET_ID;
            }
        }
        return NOT_ID;
    }
    return NOT_ID;
}

//==========================================================
//	函数名称：	EM4095_Init
//
//	函数功能：	EM4095初始化函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void EM4095_Init(void)
{
    EM4095_IO_Init();
    SET_EM4095_MODE_SLEEP();
    HAL_Delay(5);
    SET_EM4095_MODE_OPERATION();
    EM4095_MOD_DISABLE();
    HAL_Delay(5);
}





