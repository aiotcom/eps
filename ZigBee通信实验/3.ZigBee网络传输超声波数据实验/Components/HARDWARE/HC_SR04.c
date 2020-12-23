#include "HC_SR04.h"
#include "TM1640.h"	
#include "delay.h"
#include "Time.h"

//==========================================================
//	函数名称：	HCSR04_Init()
//
//	函数功能：	初始化超声波模块
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void HCSR04_Init(void)
{
    P0SEL &= ~0x03;     //设置P0.0、P0.1普通IO口  
    P1SEL &= ~0xF3;     //设置P1.0、P1.1、P1.4、P1.5、P1.6、P1.7为普通IO口 
    
    P0DIR |= 0x01;      //P0.0定义为输出
    P1DIR |= 0x62;      //P1.1、P1.5、P1.6定义为输出
    
    P0DIR &= ~0x02;     //设P0.1为输入模式 
    P0INP &= ~0x02;     //打开P0.1上拉电阻
    P1DIR &= ~0x91;     //设P1.0、P1.4、P1.7为输入模式 
    P1INP &= ~0x91;     //打开P1.0、P1.4、P1.7上拉电阻
	
    Trigl1_LOW();	//Trigl1初始化置0
    Trigl2_LOW()	//Trigl2初始化置0
    Trigl3_LOW()	//Trigl3初始化置0
    Trigl4_LOW()	//Trigl4初始化置0	
}

//==========================================================
//	函数名称：	HCSR04_StartMeasure()
//
//	函数功能：	超声波模块测距
//
//	入口参数：	number:多次平均值 次数
//
//	返回参数：	无
//
//	说明：		
//==========================================================
uint16 HCSR04_StartMeasure(uint8 number)
{
					uint16_t rspcount;
  static  uint8 Flag = 1;
  static  uint16 i = 0;
  static  double HCSR04_Data = 0;
  static  double Apart = 0;
  
  for(i=0;i<number;i++)
  {
      Flag = 1;
      Trigl3_HIGH();  //拉高Trigl3
      delay_us(20);   //必须大于10us
      Trigl3_LOW();		//拉低Trigl3
      rspcount = 0;
			 
      while((!Echo3_STATE())&&(rspcount<9000)){	//等待返回
					delay_us(1);
					rspcount++;
			}
      HCSR04Count = 0; //清零计数器
      T1IE = 1;        //开启ITM1中断,开始计时
      
			rspcount = 0;
      while(Flag)		//等待接收回响信号
      {
					rspcount++;
					delay_us(1);
          if(!Echo3_STATE())    //接收完成
          {
              T1IE = 0;	        //关闭ITM2中断
              if((HCSR04Count*17/50)>=400){
                  i--;
              }
              else{ 
                  Apart += HCSR04Count*17/100;	//距离数据
							}
              HCSR04Count = 0;
              Flag = 0;
          }
      }
			T1IE = 0;//停步定时器1计数  
			delay_ms(10);
  }
  
  HCSR04_Data = (Apart/number)/2;//取平均值
  Apart = 0;
	return (uint16)HCSR04_Data;
}




