#include"iocc2530.h"
#include"OnBoard.h"

#define uint unsigned int 
#define uchar unsigned char

#define Ds18b20Data P0_6 //温度传感器引脚

#define ON 0x01  //读取成功返回0x00，失败返回0x01
#define OFF 0x00

void Ds18b20Delay(uint k);
void Ds18b20InputInitial(void);//设置端口为输入
void Ds18b20OutputInitial(void);//设置端口为输出
uchar Ds18b20Initial(void);
void Ds18b20Write(uchar infor);
uchar Ds18b20Read(void);
void Temp_test(void); //温度读取函数



unsigned char temp,test1,test2; //储存温度信息

//时钟频率为32M
void Ds18b20Delay(uint k) //调用函数，延时us
{
  MicroWait(k);
}

void Ds18b20InputInitial(void)//设置端口为输入
{
  P0DIR &= 0xbf;
}

void Ds18b20OutputInitial(void)//设置端口为输出
{
   P0DIR |= 0x40;
}

//ds18b20初始化
//初始化成功返回0x00，失败返回0x01
uchar Ds18b20Initial(void)
{
  uchar Status = 0x00;
  uint CONT_1 = 0;
  uchar Flag_1 = ON;
  Ds18b20OutputInitial();
  Ds18b20Data = 1;
  Ds18b20Delay(260);
  Ds18b20Data = 0;
  Ds18b20Delay(750);
  Ds18b20Data = 1;
  Ds18b20InputInitial();
  while((Ds18b20Data != 0)&&(Flag_1 == ON))//等待ds18b20响应，具有防止超时功能
  {                                        //等待约60ms左右
    CONT_1++;
    Ds18b20Delay(10);
    if(CONT_1 > 8000)Flag_1 = OFF;
    Status = Ds18b20Data;
  }
  Ds18b20OutputInitial();
  Ds18b20Data = 1;
  Ds18b20Delay(100);
  return Status;
}


void Ds18b20Write(uchar infor)
{
  uint i;
  Ds18b20OutputInitial();
  for(i=0;i<8;i++)
  {
  if((infor & 0x01))
  {
  Ds18b20Data = 0;
  Ds18b20Delay(6);
  Ds18b20Data = 1;
  Ds18b20Delay(50);
  }
  else
  {
  Ds18b20Data = 0;
  Ds18b20Delay(50);
  Ds18b20Data = 1;
  Ds18b20Delay(6);
  }
  infor >>= 1;
  }
}

uchar Ds18b20Read(void)
{
  uchar Value = 0x00;
  uint i;
  Ds18b20OutputInitial();
  Ds18b20Data = 1;
  Ds18b20Delay(10);
  for(i=0;i<8;i++)
  {
  Value >>= 1; 
  Ds18b20OutputInitial();
  Ds18b20Data = 0;
  Ds18b20Delay(3);
  Ds18b20Data = 1;
  Ds18b20Delay(3);
  Ds18b20InputInitial();
  if(Ds18b20Data == 1) Value |= 0x80;
  Ds18b20Delay(15);
  } 
  return Value;
}


void Temp_test(void) //温度读取函数
{
  uchar V1,V2;
  
  test1=Ds18b20Initial();
  Ds18b20Write(0xcc);
  Ds18b20Write(0x44);
  
  test2=Ds18b20Initial();
  Ds18b20Write(0xcc);
  Ds18b20Write(0xbe);
  
  V1 = Ds18b20Read();
  V2 = Ds18b20Read();
  temp = ((V1 >> 4)+((V2 & 0x07)*16));  

}