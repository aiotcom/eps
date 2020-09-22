#include "stm32f1xx.h"
#include "LORA.h"
#include "delay.h"
void SX1278Reset(void);
void SX1278IO_Init(void);
/* SPI1 init function */
SPI_HandleTypeDef hspi1;
static void MX_SPI1_Init(void)
{
  /* SPI1 parameter configuration*/
  hspi1.Instance               = SPI1;
  hspi1.Init.Mode              = SPI_MODE_MASTER;
  hspi1.Init.Direction         = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize          = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity       = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase          = SPI_PHASE_1EDGE;
  hspi1.Init.NSS               = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode            = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial     = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    while(1);
  }

}
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hspi->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }

}
HAL_StatusTypeDef error;
u8 SPI1_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
   error  =  HAL_SPI_TransmitReceive(&hspi1,&TxData,&Rxdata,1, 100);       
 	return Rxdata;          		    //返回收到的数据		
}
u8 SX1278_RLEN;
void LORA_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOB时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOA时钟  
    MX_SPI1_Init();
	SX1278IO_Init();

	SX1278Reset();
	SX1276LORA_INT();
	//enableInterrupts();
	SX1278_RLEN = 0;     
}

u8 Frequency[3] = {0x6c,0x80,0x00};//频率计算值 434M * 16384 = Fre

//u8 Frequency[3] = {0x69,0x00,0x00};//频率计算值 420M

u8 SpreadingFactor=11;             //7-12 扩频因子参数
u8 CodingRate=2;
u8 Bw_Frequency=7;      //6-9扩频带宽参数
u8 powerValue=7;//0-7 发射功率参数
u8 power_data[8]={0X80,0X80,0X80,0X83,0X86,0x89,0x8c,0x8f}; //
u8 RF_EX0_STATUS;
u8 CRC_Value;



//初始化SPI FLASH的IO口
void SX1278IO_Init(void)
{	    
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOB时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOA时钟    

    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull  = GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    	 	//高速
	
    GPIO_Initure.Pin   = GPIO_PIN_0|GPIO_PIN_1; 	//PA0-> CE->reset,PA1->CS
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0|GPIO_PIN_1,GPIO_PIN_SET);	//PA0,PA1
    
    GPIO_Initure.Mode  = GPIO_MODE_INPUT;  	//输入模式
    GPIO_Initure.Pull  = GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    	 	//高速
	
    GPIO_Initure.Pin   = GPIO_PIN_0; 	     //SX1278IO IRQ
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);    
} 

void SX1278_Init(void)
{  	
	
}

void SX1278Reset(void)
{
	SX1278_RST_L();
	delay_ms(200);
	SX1278_RST_H();
	delay_ms(500);
}

///
void SX1276WriteBuffer( unsigned char addr, unsigned char buffer)
{ 
	SX1278_CS_L(); //NSS = 0;选中芯片
	SPI1_ReadWriteByte( addr | 0x80 );
	SPI1_ReadWriteByte( buffer);
	SX1278_CS_H(); //NSS = 1;

}

///
u8 SX1276ReadBuffer(unsigned char addr)
{
	unsigned char Value;
	SX1278_CS_L(); //NSS = 0;
	SPI1_ReadWriteByte( addr & 0x7f  );
	Value = SPI1_ReadWriteByte(0xff);
	SX1278_CS_H();//NSS = 1;
	return Value; 
}

void SX1276LoRaSetOpMode( RFMode_SET opMode )
{
	unsigned char opModePrev;
	opModePrev=SX1276ReadBuffer(REG_LR_OPMODE);  //读0x01模式寄存器
	opModePrev &=0xf8;  //清零低三位
	opModePrev |= (unsigned char)opMode; //或上形参
	SX1276WriteBuffer( REG_LR_OPMODE, opModePrev); //重新写回去	
}

void SX1276LoRaFsk( Debugging_fsk_ook opMode )
{
	unsigned char opModePrev;
	opModePrev=SX1276ReadBuffer(REG_LR_OPMODE); //读0x01模式寄存器
	opModePrev &=0x7F; //清零最高位
	opModePrev |= (unsigned char)opMode;  //或上形参
	SX1276WriteBuffer( REG_LR_OPMODE, opModePrev); //重新写回去		
}

void SX1276LoRaSetRFFrequency(void)
{
	SX1276WriteBuffer( REG_LR_FRFMSB, Frequency[0]);  //写0x06寄存器
	SX1276WriteBuffer( REG_LR_FRFMID, Frequency[1]);  //写0x07寄存器
	SX1276WriteBuffer( REG_LR_FRFLSB, Frequency[2]);  //写0x08寄存器
}

void SX1276LoRaSetRFPower(unsigned char power )
{
	//Set Pmax to +20dBm for PA_HP, Must turn off PA_LF or PA_HF, and set RegOcp
	//SX1276WriteBuffer( REG_LR_PACONFIG,  power_data[power] ); //若没有这一句，频谱仪几乎测不到信号,但是可以进行正常收发。
	//SX1276WriteBuffer( REG_LR_OCP, 0x3f);  //add by skay,20160810, 写电流保护寄存器，
	SX1276WriteBuffer( REG_LR_PADAC, 0x84);  //high power
	SX1276WriteBuffer( REG_LR_PACONFIG,  /*power_data[power]*/0x4f ); //若没有这一句，频谱仪几乎测不到信号,但是可以进行正常收发。
}

void SX1276LoRaSetNbTrigPeaks(unsigned char value )
{
	unsigned char RECVER_DAT;
	RECVER_DAT = SX1276ReadBuffer( 0x31);  //Read RegDetectOptimize,
	RECVER_DAT = ( RECVER_DAT & 0xF8 ) | value; //process;
	SX1276WriteBuffer( 0x31, RECVER_DAT );  //write back;
}

void SX1276LoRaSetSpreadingFactor(unsigned char factor )
{
	unsigned char RECVER_DAT;
	SX1276LoRaSetNbTrigPeaks( 3 ); //0x03-->SF7 to SF12
	RECVER_DAT=SX1276ReadBuffer( REG_LR_MODEMCONFIG2); //读0x1E寄存器  
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG2_SF_MASK ) | ( factor << 4 );
	SX1276WriteBuffer( REG_LR_MODEMCONFIG2, RECVER_DAT );	 
}
	
void SX1276LoRaSetErrorCoding(unsigned char value )
{	
	unsigned char RECVER_DAT;
	RECVER_DAT=SX1276ReadBuffer( REG_LR_MODEMCONFIG1); //读0x1D寄存器
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG1_CODINGRATE_MASK ) | ( value << 1 );
	SX1276WriteBuffer( REG_LR_MODEMCONFIG1, RECVER_DAT);
}

void SX1276LoRaSetPacketCrcOn(BOOL_t enable )
{	
	unsigned char RECVER_DAT;
	RECVER_DAT= SX1276ReadBuffer( REG_LR_MODEMCONFIG2);  //读0x1E寄存器 
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK ) | ( enable << 2 );
	SX1276WriteBuffer( REG_LR_MODEMCONFIG2, RECVER_DAT );
}

void SX1276LoRaSetSignalBandwidth(unsigned char bw )
{
	unsigned char RECVER_DAT;
	RECVER_DAT=SX1276ReadBuffer( REG_LR_MODEMCONFIG1);  //读0x1D寄存器
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG1_BW_MASK ) | ( bw << 4 );
	SX1276WriteBuffer( REG_LR_MODEMCONFIG1, RECVER_DAT );
}

void SX1276LoRaSetImplicitHeaderOn(BOOL_t enable )
{
	unsigned char RECVER_DAT;
	RECVER_DAT=SX1276ReadBuffer( REG_LR_MODEMCONFIG1 );  //读0x1D寄存器
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK ) | ( enable );
	SX1276WriteBuffer( REG_LR_MODEMCONFIG1, RECVER_DAT );
}
	
void SX1276LoRaSetSymbTimeout(unsigned int value )
{
	unsigned char RECVER_DAT[2];
	RECVER_DAT[0]=SX1276ReadBuffer( REG_LR_MODEMCONFIG2 );    //读0x1E寄存器
	RECVER_DAT[1]=SX1276ReadBuffer( REG_LR_SYMBTIMEOUTLSB );  //读0x1F寄存器
	RECVER_DAT[0] = ( RECVER_DAT[0] & RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK ) | ( ( value >> 8 ) & ~RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK );
	RECVER_DAT[1] = value & 0xFF;
	SX1276WriteBuffer( REG_LR_MODEMCONFIG2, RECVER_DAT[0]);
	SX1276WriteBuffer( REG_LR_SYMBTIMEOUTLSB, RECVER_DAT[1]);
}
	
void SX1276LoRaSetPayloadLength(unsigned char value )
{
	SX1276WriteBuffer( REG_LR_PAYLOADLENGTH, value );  //写0x22寄存器
} 

	
#if 0
void SX1276LoRaSetPreamLength(unsigned int value )
{
	unsigned char a[2];
	a[0]=(value&0xff00)>>8;
	a[1]=value&0xff;
	SX1276WriteBuffer( REG_LR_PREAMBLEMSB, a[0] );
	SX1276WriteBuffer( REG_LR_PREAMBLELSB, a[1] );
}
#endif

void SX1276LoRaSetMobileNode(BOOL_t enable )
{	 
	unsigned char RECVER_DAT;
	RECVER_DAT=SX1276ReadBuffer( REG_LR_MODEMCONFIG3 );  //读0x26寄存器
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG3_MOBILE_NODE_MASK ) | ( enable << 3 );
	SX1276WriteBuffer( REG_LR_MODEMCONFIG3, RECVER_DAT );
}

void SX1276LORA_INT(void)
{
	SX1276LoRaSetOpMode(Sleep_mode);  //设置睡眠模式
	SX1276LoRaFsk(LORA_mode);	     		// 设置扩频模式
	SX1276LoRaSetOpMode(Stdby_mode);  // 设置为普通模式
	SX1276WriteBuffer( REG_LR_DIOMAPPING1,GPIO_VARE_1); //写0x40寄存器,DIO引脚映射设置
	SX1276WriteBuffer( REG_LR_DIOMAPPING2,GPIO_VARE_2); //写0x41寄存器
	SX1276LoRaSetRFFrequency();  			//频率设置
	SX1276LoRaSetRFPower(powerValue); //功率设置
	SX1276LoRaSetSpreadingFactor(SpreadingFactor);	 		// 扩频因子设置
	SX1276LoRaSetErrorCoding(CodingRate);		 	//有效数据比
	SX1276LoRaSetPacketCrcOn(true);			 			//CRC 校验打开
	SX1276LoRaSetSignalBandwidth( Bw_Frequency );	 			//设置扩频带宽, 125khz
	SX1276LoRaSetImplicitHeaderOn(false);		 	//同步头是显性模式
	SX1276LoRaSetPayloadLength( 0xff);
	SX1276LoRaSetSymbTimeout( 0x3FF );
	SX1276LoRaSetMobileNode(true); 			 			// 低数据的优化

	RF_RECEIVE();
}

void FUN_RF_SENDPACKET(unsigned char *RF_TRAN_P,unsigned char LEN)
{	
	unsigned char ASM_i;
	
	SX1276LoRaSetOpMode( Stdby_mode );
	SX1276WriteBuffer( REG_LR_HOPPERIOD, 0 );			//不做频率跳变
	SX1276WriteBuffer(REG_LR_IRQFLAGSMASK,IRQN_TXD_Value);	//打开发送中断
	SX1276WriteBuffer( REG_LR_PAYLOADLENGTH, LEN);//最大数据包
	SX1276WriteBuffer( REG_LR_FIFOTXBASEADDR, 0); //写Tx FIFO基址
	SX1276WriteBuffer( REG_LR_FIFOADDRPTR, 0 ); 	//SPI interface address pointer in FIFO data buffer
	SX1278_CS_L();   //开片选
	SPI1_ReadWriteByte( 0x80 );
	for( ASM_i = 0; ASM_i < LEN; ASM_i++ )
	{
		SPI1_ReadWriteByte( *RF_TRAN_P );RF_TRAN_P++;
	}
	SX1278_CS_H();  //关片选
	SX1276WriteBuffer(REG_LR_DIOMAPPING1,0x40);  //发射中断映射到DIO0引脚
	SX1276WriteBuffer(REG_LR_DIOMAPPING2,0x00);
	SX1276LoRaSetOpMode( Transmitter_mode );     //设置为传输模式
	
}
void RF_RECEIVE (void)
{
	SX1276LoRaSetOpMode(Stdby_mode );
	SX1276WriteBuffer(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);  //0x11,打开接收中断
	SX1276WriteBuffer(REG_LR_HOPPERIOD,	PACKET_MIAX_Value );//0x24寄存器
	SX1276WriteBuffer( REG_LR_DIOMAPPING1, 0X00 ); 					//DIO引脚映射设置，按默认
	SX1276WriteBuffer( REG_LR_DIOMAPPING2, 0X00 );	
	SX1276LoRaSetOpMode( Receiver_mode );  //设置为连续接收模式
}

#if 0	
void RF_CAD_RECEIVE (void)
{
	SX1276LoRaSetOpMode( Stdby_mode );
	SX1276WriteBuffer(REG_LR_IRQFLAGSMASK,  IRQN_CAD_Value);	//打开发送中断
	SX1276WriteBuffer( REG_LR_DIOMAPPING1, 0X80 );
	SX1276WriteBuffer( REG_LR_DIOMAPPING2, 0X00 );	
	SX1276LoRaSetOpMode( CAD_mode );
}
#endif

#if 0
void RF_SEELP(void)
{
	SX1276LoRaSetOpMode( Stdby_mode );
	SX1276WriteBuffer(REG_LR_IRQFLAGSMASK,  IRQN_SEELP_Value);  //打开发送中断
	SX1276WriteBuffer( REG_LR_DIOMAPPING1, 0X00 );
	SX1276WriteBuffer( REG_LR_DIOMAPPING2, 0X00 );	
	SX1276LoRaSetOpMode( Sleep_mode );
}
#endif

//**************下面是中断里面处理的代码*******************************
unsigned char RF_REC_RLEN_i;
//==========================================================
//	函数名称：	uint8_t SX1278_InteruptHandler(uint8_t *pRecv)
//
//	函数功能：	处理LORA芯片产生的中断
//
//	入口参数：	uint8_t *pRecv，用于接收数据，当用于接收数据时pRecv，传入数据指针，其他情况传入(void*)0
//
//	返回参数：	返回中断类型
//
//	说明：	     @1:当用于接收数据时	
//   uint8_t recv[30]
//    if(SX1278_InteruptHandler(recv) == INT_FLG_RX_DONE)
//    {
//
//    }
//               @2:其他情况
//               SX1278_InteruptHandler((void*)0)
//==========================================================
uint8_t SX1278_InteruptHandler(uint8_t *pRecv)
{
    uint8_t ret;
	RF_EX0_STATUS=SX1276ReadBuffer( REG_LR_IRQFLAGS ); 
	if((RF_EX0_STATUS&0x40)==0x40)  //接收完成
	{
		CRC_Value=SX1276ReadBuffer( REG_LR_MODEMCONFIG2 );
		if((CRC_Value&0x04)==0x04) //是否打开CRC校验
		{
			SX1276WriteBuffer (REG_LR_FIFOADDRPTR,0x00);
			SX1278_RLEN = SX1276ReadBuffer(REG_LR_NBRXBYTES); //读取最后一个包的字节数
			SX1278_CS_L();
			SPI1_ReadWriteByte( 0x00 );
			if(SX1278_RLEN > 249)  //接收不超过10个字节
				SX1278_RLEN = 249;			
			for(RF_REC_RLEN_i=0;RF_REC_RLEN_i<SX1278_RLEN;RF_REC_RLEN_i++)
			{
				pRecv[RF_REC_RLEN_i]=SPI1_ReadWriteByte(0xff);		
			}
			SX1278_CS_H();
		}
		SX1276LoRaSetOpMode( Stdby_mode );
		SX1276WriteBuffer(REG_LR_IRQFLAGSMASK, IRQN_RXD_Value);  //打开发送中断
		SX1276WriteBuffer(REG_LR_HOPPERIOD,    PACKET_MIAX_Value);
		SX1276WriteBuffer( REG_LR_DIOMAPPING1, 0X00 );
		SX1276WriteBuffer( REG_LR_DIOMAPPING2, 0x00 );	
		SX1276LoRaSetOpMode( Receiver_mode );
        
    ret = INT_FLG_RX_DONE;
	}
	else if((RF_EX0_STATUS&0x08)==0x08)  //发射完成
	{
		SX1276LoRaSetOpMode( Stdby_mode );
		SX1276WriteBuffer(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);	//打开发送中断
		SX1276WriteBuffer(REG_LR_HOPPERIOD,   PACKET_MIAX_Value );
		SX1276WriteBuffer( REG_LR_DIOMAPPING1, 0X00 );
		SX1276WriteBuffer( REG_LR_DIOMAPPING2, 0x00 );	
		SX1276LoRaSetOpMode( Receiver_mode );
        
		ret = INT_FLG_TX_DONE;
	}
	else if((RF_EX0_STATUS&0x04)==0x04)  //cad完成
	{  
		if((RF_EX0_STATUS&0x01)==0x01)
		{	 
		//表示CAD 检测到扩频信号 模块进入了接收状态来接收数据
			SX1276LoRaSetOpMode( Stdby_mode );
			SX1276WriteBuffer(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);	//打开发送中断
			SX1276WriteBuffer(REG_LR_HOPPERIOD,   PACKET_MIAX_Value );
			SX1276WriteBuffer( REG_LR_DIOMAPPING1, 0X02 );
			SX1276WriteBuffer( REG_LR_DIOMAPPING2, 0x00 );	
			SX1276LoRaSetOpMode( Receiver_mode );
            
			ret = INT_FLG_CAD_DONE;
		}
		else
		{						   
		// 没检测到
			SX1276LoRaSetOpMode( Stdby_mode );
			SX1276WriteBuffer(REG_LR_IRQFLAGSMASK,	IRQN_SEELP_Value);	//打开发送中断
			SX1276WriteBuffer( REG_LR_DIOMAPPING1, 0X00 );
			SX1276WriteBuffer( REG_LR_DIOMAPPING2, 0X00 );	
			SX1276LoRaSetOpMode( Sleep_mode );
            
			ret = INT_FLG_CAD_FAIL;
		}
	}
	else
	{
		SX1276LoRaSetOpMode( Stdby_mode );
		SX1276WriteBuffer(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);	//打开发送中断
		SX1276WriteBuffer(REG_LR_HOPPERIOD,   PACKET_MIAX_Value );
		SX1276WriteBuffer( REG_LR_DIOMAPPING1, 0X02 );
		SX1276WriteBuffer( REG_LR_DIOMAPPING2, 0x00 );	
		SX1276LoRaSetOpMode( Receiver_mode );
		
	}
	SX1276WriteBuffer( REG_LR_IRQFLAGS, 0xff  );
    
    return ret;

}
//
u8 Sx1278SendLong(unsigned char *RF_TRAN_P,unsigned char LEN)
{
	uint8_t  Irq_flag=0;
	uint16_t Wait_FLAG = 500;
	
	FUN_RF_SENDPACKET(RF_TRAN_P,LEN); //发射数据
	while(((Irq_flag&0x08) != 0x08) && Wait_FLAG)
	{
		HAL_Delay(1);
		Wait_FLAG--;
		Irq_flag=SX1276ReadBuffer( REG_LR_IRQFLAGS ); 
	}
    HAL_Delay(100);
    RF_RECEIVE(); //转换到接收模式
    HAL_Delay(10);
	return Wait_FLAG?1:0;
}
uint8_t LR_Version;
void ProcessRecv(void)
{
#if  0  
    LR_Version = SX1276ReadBuffer( REG_LR_IRQFLAGS );
	if(SX1278_IRQ_INPUT())  //读0x12状态寄存器
    {
				SX1278_InteruptHandler((void*)0);
    }
#endif    
}
