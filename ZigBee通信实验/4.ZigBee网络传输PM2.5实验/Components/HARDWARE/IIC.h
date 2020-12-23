#ifndef __IIC_H 
#define __IIC_H 			   


#include <hal_types.h>

#define Bit_SET  	  1
#define Bit_RESET    	  0

#define ACK_OK  	  0x00
#define ACK_ERROR    	  0x01

#define SCL_PIN           P0_6
#define SDA_PIN           P0_7

#define SCL_HIGH()        P0_6 = 1;P0_6 = 1;
#define SCL_LOW()         P0_6 = 0;P0_6 = 0;

#define SDA_HIGH()        P0_7 = 1;P0_7 = 1;;
#define SDA_LOW()         P0_7 = 0;P0_7 = 0;;

#define SCL_STATE()       P0_6
#define SDA_STATE()       P0_7

//IIC所有操作函数
void IIC_Delay(uint32 n);		//IIC延时函数
void IIC_Init(void);           	//初始化IIC的IO口		
void IIC_Start(void);		//发送IIC开始信号
void IIC_Stop(void);	  	//发送IIC停止信号
void IIC_Ack(void);		//IIC发送ACK信号
void IIC_NAck(void);		//IIC不发送ACK信号
uint8 IIC_Wait_Ack(void);		//等待应答信号到来
void IIC_SCL_OUTPUT(void);	//IIC时钟线输出模式
void IIC_SCL_INPUT(void);	//IIC时钟线输入模式
void IIC_SDA_OUTPUT(void);	//IIC数据线输出模式 
void IIC_SDA_INPUT(void);	//IIC数据线输入模式
uint8 IIC_Read_Byte(uint8 ack);	//IIC读取数据
void IIC_Send_Byte(uint8 txd);	//IIC写数据
uint8 SHT2x_I2cReadByte(void);	//SHT20读取数据
uint8 Write_IIC_Byte(uint8 byte);	//IIC写数据

#endif

