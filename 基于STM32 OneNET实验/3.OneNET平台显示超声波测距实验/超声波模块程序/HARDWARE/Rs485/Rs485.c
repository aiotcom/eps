#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "Rs485.h"
#include "usart.h"

Send_Stack Tx_Stack;										//485发送数据结构体
Receive_Stack Rx_Stack;									//485接收数据结构体

uint8_t  Send_Data[Send_REC_LEN]; 			//发送缓冲,最大Send_REC_LEN个字节
uint8_t  Receive_Data[Receive_REC_LEN]; //接收缓冲,最大Send_REC_LEN个字节

//==========================================================
//	函数名称：	Rs485_Init()
//
//	函数功能：	初始化485
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：			 
//==========================================================
void Rs485_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
	
    __HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOA时钟

		GPIO_Initure.Pin = GPIO_PIN_8;						//PA8
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;	//推挽输出
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_LOW; //低速
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);

		RS485_RX_EN();   													//初始化为接收模式

		Rx_Stack_Init();													//初始化接收结构体
		Tx_Stack_Init();													//初始化发送结构体
}

//==========================================================
//	函数名称：	Tx_Stack_Init
//
//	函数功能：	RFID模块上发数据结构体初始化
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：			 
//==========================================================
void Tx_Stack_Init(void)
{
	Tx_Stack.Head = 0xE1E2E3E4;					//帧头
	Tx_Stack.Pack_Num = 0x00;						//包序号
	Tx_Stack.Src_Adr = 0x0000;					//源地址
	Tx_Stack.Dst_Adr = 0x0000;					//目的地址
	Tx_Stack.State = 0x0000;						//状态码
	Tx_Stack.Data_Len = 0x00;						//数据长度
	memset(Tx_Stack.Data, 0, sizeof(Tx_Stack.Data));//数据数组全部清空
	Tx_Stack.CrcCheck = 0x0000;					//CRC校验码
	Tx_Stack.Tail = 0xEAEBECED;					//帧尾
}

//==========================================================
//	函数名称：	Rx_Stack_Init
//
//	函数功能：	上位机下发数据结构体初始化
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void Rx_Stack_Init(void)
{
	Rx_Stack.Head = 0xE1E2E3E4;					//帧头
	Rx_Stack.Pack_Num = 0x00;						//包序号
	Rx_Stack.Src_Adr = 0x0000;					//源地址
	Rx_Stack.Dst_Adr = 0x0000;					//目的地址
	Rx_Stack.Command = 0x0000;					//命令码
	Rx_Stack.Data_Len = 0x00;						//数据长度						
	memset(Rx_Stack.Data, 0, sizeof(Rx_Stack.Data));//数据数组全部清空
	Rx_Stack.CrcCheck = 0x0000;					//CRC校验码
	Rx_Stack.Tail = 0xEAEBECED;					//帧尾
}
//==========================================================
//	函数名称：	Rs485_delay()
//
//	函数功能：	毫秒延时函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void Rs485_delay(uint32_t n)
{		
    while(n--);
}
//==========================================================
//	函数名称：	Rs485_Send()
//
//	函数功能：	发送数据到485总线上
//
//	入口参数：	SrcAdr:源地址
//							DstAdr:目的地址
//							Cmd		:命令码
//							Len		:数据长度
//							*pData:数据
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void Rs485_Send(uint16_t SrcAdr,uint16_t DstAdr,uint16_t Cmd,uint8_t Len,uint8_t *pData)
{
	uint16_t i = 0;																			//For循环使用	
	
	for(i=0;i<4;i++)
	{
		Send_Data[i] = (0xE1+i);													//帧头
	}

	if(Tx_Stack.Pack_Num>=255)
	{
		Tx_Stack.Pack_Num = 0;	
	}
	Send_Data[4] = Tx_Stack.Pack_Num;										//包序号	
	Tx_Stack.Pack_Num++;
	Send_Data[5] = SrcAdr>>8;
	Send_Data[6] = SrcAdr;															//源地址
	Send_Data[7] = DstAdr>>8;
	Send_Data[8] = DstAdr;															//目的地址
	Send_Data[9] = Cmd>>8;
	Send_Data[10] = Cmd;																//命令码
	Send_Data[11] = Len;																//数据长度
	for(i=12;i<(Len+12);i++)
	{
		Send_Data[i] = pData[i-12];												//数据
	}
	Tx_Stack.CrcCheck = CRC_16(Send_Data,(12+Len));			//CRC校验码
	Send_Data[12+Len] = Tx_Stack.CrcCheck>>8;
	Send_Data[13+Len] = Tx_Stack.CrcCheck;							//CRC校验码
	for(i=(14+Len);i<(18+Len);i++)
	{
		Send_Data[i] = (0xEA+(i-14-Len));									//帧尾
	}

	RS485_TX_EN();		//开启485发送
	Rs485_delay(200);
	HAL_UART_Transmit(&UART1_Handler,Send_Data,(18+Len),1000);//485发送数据
	Rs485_delay(100);
	RS485_RX_EN();		//开启485接收
}

//==========================================================
//	函数名称：	Rs485_Receive()
//
//	函数功能：	485接收
//
//	入口参数：	data		:485数据
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void Rs485_Receive(uint8_t data)
{	
	static uint8_t End_Flag 		= 0;	//帧尾标志位
	static uint8_t Header_Flag 	= 0;	//帧头标志位
	static uint8_t Receive_Flag = 0;	//接收标志位

/*
*
*				帧头接收
*
*/
	/*帧头1 数据帧头中第一个字节*/
	if(data == 0xE1 && Header_Flag < 4)	Header_Flag = 1;

	/*帧头2 数据帧头中第二个字节*/
	else if(Header_Flag == 1 && data == 0xE2)	Header_Flag = 2;

	/*帧头3 数据帧头中第三个字节*/
	else if(Header_Flag == 2 && data == 0xE3)	Header_Flag = 3;

	/*帧头4 数据帧头中第四个字节*/
	else if(Header_Flag == 3 && data == 0xE4)
	{
		Receive_Data[0] = 0xE1;
		Receive_Data[1] = 0xE2;
		Receive_Data[2] = 0xE3; //帧头接收完成，存入帧头数据
		USART1_RX_STA = 3;			//存放数据数组下标清零
		Receive_Flag = 1;				//开始接收数据标志位
	}

	else Header_Flag = 0;

/*
*
*				数据+帧尾接收
*
*/
	if(Receive_Flag)	//帧头接收完成
	{
		if(USART1_RX_STA>(Receive_REC_LEN-1))
		{
			End_Flag = 0;
			Receive_Flag = 0;
			USART1_RX_STA = 0;
		}

		Receive_Data[USART1_RX_STA++] = data;	//数据接收

		/*帧尾1 数据帧尾中第一个字节*/
		if(data == 0xEA)	End_Flag = 1;

		/*帧尾2  数据帧尾中第二个字节*/
		else if(End_Flag == 1 && data == 0xEB)	End_Flag = 2;

		/*帧尾3  数据帧尾中第三个字节*/
		else if(End_Flag == 2 && data == 0xEC)	End_Flag = 3;

		/*帧尾4  数据帧尾中第四个字节*/
		else if(End_Flag == 3 && data == 0xED)
		{
			USART1_RX_STA = Receiving_Completion;		//接收完成
			Receive_Flag = 0;
			End_Flag = 0;
		}

		/*接收错误 重新接收*/
		else End_Flag = 0;
	}
}

//==========================================================
//	函数名称：	DataHandling_485()
//
//	函数功能：	485数据处理函数
//
//	入口参数：	DesAddr	:目的地址
//              
//	返回参数：	0:数据接收完成
//							1:数据接收错误
//
//	说明：		
//==========================================================
uint8_t DataHandling_485(uint16_t DesAddr)
{
	static uint16_t i = 0;
	static uint8_t Return_Value = 0;
	static uint16_t Crc16Check = 0;			//存放CRC校验数据

  if(USART1_RX_STA == Receiving_Completion)
	{
			Return_Value = 0;
			if((Receive_Data[7]*256+Receive_Data[8]) == DesAddr)	//对比源地址是想要的数据，全部接收下来
			{
				/*包序号*/
				Rx_Stack.Pack_Num = Receive_Data[4];

				/*源地址*/
				Rx_Stack.Src_Adr = Receive_Data[5]*256+Receive_Data[6];

				/*目的地址*/
				Rx_Stack.Dst_Adr = Receive_Data[7]*256+Receive_Data[8];

				/*命令码*/
				Rx_Stack.Command = Receive_Data[9]*256+Receive_Data[10];

				/*数据长度*/
				Rx_Stack.Data_Len = Receive_Data[11];

				/*数据*/
				for(i=12;i<(Rx_Stack.Data_Len+12);i++)
				{
					Rx_Stack.Data[(i-12)] = Receive_Data[i];
				}

				/*CRC校验码高八位*/
				Rx_Stack.CrcCheck = Receive_Data[(12+Rx_Stack.Data_Len)]*256;

				/*CRC校验码低八位*/
				Rx_Stack.CrcCheck += Receive_Data[(13+Rx_Stack.Data_Len)];
				
				Crc16Check = CRC_16(Receive_Data,(12+Rx_Stack.Data_Len));
				if(Crc16Check != Rx_Stack.CrcCheck)
				{
					Return_Value = 1;
				}
			}
			else Return_Value = 1;
			
			memset(Receive_Data,0,Receive_REC_LEN);//清空接收数组
			
			USART1_RX_STA = 0;
	}
	else Return_Value = 1;

	return Return_Value;	//返回接收情况
}

//==========================================================
//	函数名称：	calcByte()
//
//	函数功能：	CRC_16字节计算函数
//
//	入口参数：	crc:CRC校验码
//							b:
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
uint16_t calcByte(uint16_t crc, uint8_t b)
{
		uint16_t i = 0;			//清空数组时For循环使用	

    crc = crc ^ (int)b << 8;
    for (i = 0; i < 8; i++)
    {
        if ((crc & 0x8000) == 0x8000)
        {
            crc = crc << 1 ^ 0x1021;
        }
        else
        {
            crc = crc << 1;
        }
    }
    return crc & 0xffff;
}

//==========================================================
//	函数名称：	CRC_16()
//
//	函数功能：	CRC_16计算函数
//
//	入口参数：	*p:	需要校验的数据
//							length: 校验数据长度
//              
//	返回参数：	wCRC16:16位CRC校验码
//
//	说明：			
//==========================================================
uint16_t CRC_16(void *p, int length)
{
		short wCRC16=0;
    uint8_t *pBuffer;
		uint16_t i = 0;			//清空数组时For循环使用	

    if (( p==0 )||( length==0 ))
    {
        return 0xFFFF;
    }

    pBuffer=(uint8_t *)p;
    for ( i = 0; i < length; i++)
    {
        wCRC16 = calcByte(wCRC16, pBuffer[i]);
    }

    return wCRC16;
}


