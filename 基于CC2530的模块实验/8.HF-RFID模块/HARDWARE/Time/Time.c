#include "Time.h"
#include "RC522.h"
#include "Rs485.h"

//==========================================================
//	函数名称：	Init_Timer1()
//
//	函数功能：	定时器1初始化函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：          
//==========================================================
void Init_Timer1(void)
{
  T1CC0L = 0xC4;        //设置最大计数值的低8位
  T1CC0H = 0x09;        //设置最大计数值的高8位
  T1CCTL0 |= 0x04;      //开启通道0的输出比较模式
  T1IE = 1;             //使能定时器1中断
  T1OVFIM = 1;          //使能定时器1溢出中断
  EA = 1;               //使能总中断
  T1CTL = 0x0e;         //分频系数是128,模模式
}

//==========================================================
//	函数名称：	Timer1_Sevice()
//
//	函数功能：	定时器1服务函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
#pragma vector = T1_VECTOR
__interrupt void Timer1_Sevice()
{
  if(!DataHandling_485(Addr_HF_RFID)) //处理485数据
  {
    switch(Rx_Stack.Command)
    {
      case HF_RFID_Get_ID:	//获取ID数据
        Rs485_Send(Addr_HF_RFID,Rx_Stack.Src_Adr,HF_RFID_ID,4,ucArray_ID);	//发送ID数据
        break;
      case HF_RFID_Get_Block:	//获取Data数据
        Rs485_Send(Addr_HF_RFID,Rx_Stack.Src_Adr,HF_RFID_Block,16,Block_Date);	//发送Data数据
        break;
      case HF_RFID_Get_All:	//获取ID+Data数据
        Rs485_Send(Addr_HF_RFID,Rx_Stack.Src_Adr,HF_RFID_All,20,Data_Send);	//发送ID+Data数据
        break;
      default:break;
    }
  }
  T1STAT &= ~0x01; //清除定时器1通道0中断标志 
}
