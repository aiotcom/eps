#ifndef __OLED_IIC_H 
#define __OLED_IIC_H 			   

#include <ioCC2530.h>
#include "delay.h"

#define OLED_Bit_SET  	  1
#define OLED_Bit_RESET    0

#define OLED_ACK_OK  	  0x00
#define OLED_ACK_ERROR    0x01

#define OLED_SCL_PIN      P0_6
#define OLED_SDA_PIN      P0_7

#define OLED_SCL_HIGH()   P0_6 = 1;P0_6 = 1;
#define OLED_SCL_LOW()    P0_6 = 0;P0_6 = 0;

#define OLED_SDA_HIGH()   P0_7 = 1;P0_7 = 1;
#define OLED_SDA_LOW()    P0_7 = 0;P0_7 = 0;

#define OLED_SCL_STATE()  P0_6
#define OLED_SDA_STATE()  P0_7

//IIC所有操作函数
void OLED_IIC_Delay(u32 n);	//OLED_IIC延时函数
void IIC_Init(void);       //初始化OLED_IIC的IO口		
void OLED_IIC_Start(void);	//发送OLED_IIC开始信号
void OLED_IIC_Stop(void);	//发送OLED_IIC停止信号
void OLED_IIC_Ack(void);	//OLED_IIC发送ACK信号
void OLED_IIC_NAck(void);	//OLED_IIC不发送ACK信号
u8 OLED_IIC_Wait_Ack(void);	//等待应答信号到来
void OLED_IIC_SCL_OUTPUT(void);	//OLED_IIC时钟线输出模式
void OLED_IIC_SCL_INPUT(void);	//OLED_IIC时钟线输入模式
void OLED_IIC_SDA_OUTPUT(void);	//OLED_IIC数据线输出模式 
void OLED_IIC_SDA_INPUT(void);	//OLED_IIC数据线输入模式
u8 OLED_IIC_Read_Byte(u8 ack);	//OLED_IIC读取数据
void OLED_IIC_Send_Byte(u8 txd);	//OLED_IIC写数据
u8 Write_OLED_IIC_Byte(u8 byte);//OLED_IIC写数据

#endif

