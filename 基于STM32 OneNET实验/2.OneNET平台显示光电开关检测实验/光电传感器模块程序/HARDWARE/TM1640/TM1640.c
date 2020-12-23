#include "TM1640.h"	

void TM1640_Init(void)
{
	  GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();								//开启GPIOA时钟
	
		GPIO_Initure.Pin = GPIO_PIN_4|GPIO_PIN_5;		//PA0
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;		//输出
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_LOW;		//低速
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);
	
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);
}


/******									TM1640									******/

unsigned char TM164_DATA[14] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0X39,0X76,0X80};//0~9,空白，C,H,点;
unsigned char TM164_SIG[4] = {0x01,0x02,0x04,0x08};

//==========================================================
//	函数名称：	start
//
//	函数功能：	TM1640开始
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void start(void)
{
		sda_H;
		scl_H;
		HAL_Delay(2);
		sda_L;
		HAL_Delay(2);
		scl_L;
		HAL_Delay(2);
}

//==========================================================
//	函数名称：	stop
//
//	函数功能：	TM1640停止
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void stop(void)
{
		sda_L;
		scl_H;
		HAL_Delay(2);
		sda_H;
		HAL_Delay(2);
		scl_L;
		HAL_Delay(2);
}


//==========================================================
//	函数名称：	write
//
//	函数功能：	设置数据
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void write(unsigned char date)
{
		unsigned char i,aa;
		aa=date;
		sda_L;
		scl_L;
		for(i=0;i<8;i++)
		{
				scl_L;
				if(aa&0x01)
				{
					sda_H;
					HAL_Delay(2);
				}
				else
				{
					sda_L
					HAL_Delay(2);
				}
				scl_H;
				aa=aa>>1;
		}
		scl_L;

		sda_L;

}

//==========================================================
//	函数名称：	init_tm1640
//
//	函数功能：	初始化TM1640
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void init_tm1640(void)

{
		sda_H;
		scl_H;
		start();
		write(SET_DATA_ORDER);		//设置数据，0x40,0x44分别对应地址自动加一和固定地址模式
		stop();
		start();
		write(SET_DISPLAY);				//控制显示，开显示0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f分别对应脉冲宽度
															//------------------1/16, 2/16, 4/16, 10/16, 11/16, 12/16, 13/16, 14/16

		//0x80关显示
		stop();

}

//==========================================================
//	函数名称：	init_tm1640_NEW
//
//	函数功能：	设置TM1640
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void init_tm1640_NEW(void)
{
	unsigned char i;
		sda_H;
		scl_H;
	
		start();
		write(SET_DATA_ORDER);	//设置数据，0x40,0x44分别对应地址自动加一和固定地址模式
		stop();
	
		start();
		write(0x00);					//设置地址
		for(i=0;i<4;i++)
			{
				write(TM164_DATA[i]);
			}
		stop();
	
		start();
		write(SET_DISPLAY);	//控制显示，开显示0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f分别对应脉冲宽度
												//------------------1/16, 2/16, 4/16, 10/16, 11/16, 12/16, 13/16, 14/16
		//0x80关显示
		stop();

}

//==========================================================
//	函数名称：	send_LED_Display
//
//	函数功能：	显示
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void send_LED_Display(unsigned char address,uint16_t data,unsigned char type)  
{
		unsigned char SEG_1,SEG_2,SEG_3,SEG_4;
		if(type == 1)//显示温度的数据
		{
			SEG_4 = 11;
		}
		else if(type == 2)//显示湿度的数据
		{
			SEG_4 = 12;
		}
		else if(type == 3)//显示光强的数据
		{
			SEG_4 = data%10;
		}
		else if(type == 4)//配合其他模块显示
		{
			SEG_1 =  data/1000;
			SEG_2 = (data/100)%10;
			SEG_3 = (data/10)%10;
			SEG_4 = data%10;
		}
		if(data > 1000)
		{          
			SEG_1 = data/1000;
			SEG_2 = (data/100)%10;
			SEG_3 = (data/10)%10;
		}
		else if(data > 9)
		{
			SEG_1 = data/10; 
			SEG_2 = data%10;
			SEG_3 = 10;
		}
		else
		{
			SEG_1 = 0x00;
			SEG_2 = data;
			SEG_3 = 10;
		}
		
		sda_H;
		scl_H;
	
		start();
		write(SET_DATA_ORDER);	//设置数据，0x40,0x44分别对应地址自动加一和固定地址模式
		stop();
		
		start();
		write(address);	//设置起始地址
		write(TM164_DATA[SEG_1]);
		write(TM164_DATA[SEG_2]);
		write(TM164_DATA[SEG_3]);
		write(TM164_DATA[SEG_4]);
		
		stop();
		
				start();
		write(SET_DISPLAY);	//控制显示，开显示0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f分别对应脉冲宽度
		//------------------1/16, 2/16, 4/16, 10/16, 11/16, 12/16, 13/16, 14/16
		//0x80关显示
		stop();
}

/******自动加一模式*****/

void send_z(uint8_t address,uint8_t date)
{
		unsigned char i;
		start();
		write(address);	//设置起始地址
		for(i=0;i<4;i++)
		{
			write(TM164_DATA[i]);
		}
		stop();
}

/******固定地址模式*****/

void send(uint8_t address, uint8_t date)

{
	start();
	write(address);	//传显示数据对应地址
	write(date);	//传BYTE显示数据
	stop();
}



/*   								TM1604结束   								*/

