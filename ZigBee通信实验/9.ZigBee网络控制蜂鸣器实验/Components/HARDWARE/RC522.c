#include "OLED.h"
#include "RC522.h"
#include "delay.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int icd  = 0;
uint8_t i = 0;
unsigned char tests[300];
uint8_t IC_Card_Exist = 0;
unsigned char pk[6] ={0xff,0xff,0xff,0xff,0xff,0xff};     
unsigned char ucArray_ID [ 10 ] = {0,0,0,0};//先后存放IC卡的类型和UID(IC卡序列号)
uint8_t SendBuf[33];
uint8_t SendData[9];
unsigned char ucStatusReturn;  //返回状态
unsigned char date1[16] = {0x00};
static void RC522_SPI_Config( void );

//==========================================================
//	函数名称：	RC522_Init
//
//	函数功能：	RC522初始化函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void RC522_Init ( void )
{
    RC522_SPI_Config ();
    
    macRC522_Reset_Disable();
    
    macRC522_CS_Disable();
    
    PcdReset ();
    
    M500PcdConfigISOType ( 'A' );//设置工作模式
	
}

//==========================================================
//	函数名称：	DataRep()
//
//	函数功能：	显示数据装换
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void DataRep(uint8_t data,uint8_t num,uint8_t flag,uint8_t flg)
{
  if(flg)
  {
    switch(data)
    {
      case 0x0A:
        SendData[num] = 65;
        break;
      case 0x0B:
        SendData[num] = 66;
        break;
      case 0x0C:
        SendData[num] = 67;
        break;
      case 0x0D:
        SendData[num] = 68;
        break;
      case 0x0E:
        SendData[num] = 69;
        break; 
      case 0x0F:
        SendData[num] = 70;
        break; 
      default:
        if(flag)
        {
          SendData[num] = (ucArray_ID[(num/2)]>>4)+0x30;
        }
        else
        {
          SendData[num] = (ucArray_ID[(num/2)]&0x0f)+0x30;
        }     
      break;
    } 
  }
  else
  {
    switch(data)
    {
      case 0x0A:
        SendBuf[num] = 65;
        break;
      case 0x0B:
        SendBuf[num] = 66;
        break;
      case 0x0C:
        SendBuf[num] = 67;
        break;
      case 0x0D:
        SendBuf[num] = 68;
        break;
      case 0x0E:
        SendBuf[num] = 69;
        break; 
      case 0x0F:
        SendBuf[num] = 70;
        break; 
      default:
        if(flag)
        {
          SendBuf[num] = (date1[(num/2)]>>4)+0x30;
        }
        else
        {
          SendBuf[num] = (date1[(num/2)]&0x0f)+0x30;
        }     
      break;
    }
  }
}

//==========================================================
//	函数名称：	IC_test
//
//	函数功能：	读IC卡
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void IC_Card_Search ( void )
{
    SendData[8] = 0;
    SendBuf[32] = 0;
    ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );//若失败再次寻卡
    
    if ((ucStatusReturn == MI_OK )&&(IC_Card_Exist == 0))
    {
        if (PcdAnticoll(ucArray_ID) == MI_OK)  //防冲撞（当有多张卡进入读写器操作范围时，防冲突机制会从其中选择一张进行操作）
        {               
            if (PcdSelect(ucArray_ID) == MI_OK)//选择ucArray_ID的卡
            {
                IC_Card_Exist = 1;
                if (PcdAuthState(0x61,0x5,pk,ucArray_ID) == MI_OK)//申请对卡的授权  
                {
                    if (PcdRead(0x5,date1) == MI_OK )
                    {   
                        for(i = 0;i < 8;i ++)
                        {
                          if(i%2==0)
                          {
                             DataRep((ucArray_ID[(i/2)]>>4),i,1,1);
                          }
                          else
                          {
                            DataRep((ucArray_ID[(i/2)]&0x0f),i,0,1);
                          }                         
                        }
                        
                        
                        for(i = 0;i < 32;i ++)
                        {
                          if(i%2==0)
                          {
                             DataRep((date1[(i/2)]>>4),i,1,0);
                          }
                          else
                          {
                            DataRep((date1[(i/2)]&0x0f),i,0,0);
                          }                         
                        }
                        OLED_P8x16Str(41,2,"0x");
                        OLED_P8x16Str(58,2,SendData);
                        OLED_P6x8Str(1,6,"0x");
                        OLED_P6x8Str(15,6,SendBuf);
                    }
                }
            }
        }
    }
    
    if(ucStatusReturn != MI_OK)//无卡
    {
        IC_Card_Exist = 0;
    }
}
//==========================================================
//	函数名称：	IC_Card_Write
//
//	函数功能：	向IC卡里写数据
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
uint8_t cStatus;
uint8_t IC_Card_Write(uint8_t *p)
{
    if (PcdAnticoll(ucArray_ID) == MI_OK)  //防冲撞（当有多张卡进入读写器操作范围时，防冲突机制会从其中选择一张进行操作）
    {               
        if (PcdSelect(ucArray_ID) == MI_OK)//选择ucArray_ID的卡
        {
            if (PcdAuthState(0x61,0x5,pk,ucArray_ID) == MI_OK)//申请对卡的授权  
            {
                cStatus = PcdWrite(0x5,p);
                if(cStatus == MI_ERR)
                {//如果写失败在写一次
                    cStatus = PcdWrite(0x5,p);
                }
            }
        }
    }
    return cStatus;
}
//==========================================================
//	函数名称：	IC_Card_Read
//
//	函数功能：	从IC卡里计出数据
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void IC_Card_Read(uint8_t *p)
{
     PcdRead(0X5,p);
}
//==========================================================
//	函数名称：	RC522_SPI_Config
//
//	函数功能：	RC522 SPI初始化
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static void RC522_SPI_Config ( void )
{     
  P1SEL &= ~0xF2;     //设置P1.1、P1.4、P1.5、P1.6、P1.7为普通IO口  
  P1DIR |= 0x72;      //P1.1、P1.4、P1.5、P1.6定义为输出   
  
  P1DIR &= ~0x80;     //按键接在P1.7口上，设P1.7为输入模式 
  P1INP &= ~0x80;     //打开P1.7上拉电阻
}

//==========================================================
//	函数名称：	SPI_RC522_SendByte
//
//	函数功能：	向RC522发送1 Byte 数据
//
//	入口参数：	byte，要发送的数据
//
//	返回参数：	RC522返回的数据
//
//	说明：		
//==========================================================
void SPI_RC522_SendByte ( unsigned char byte )
{
    unsigned char counter;
		
    for(counter=0;counter<8;counter++)
    {     
        if ( byte & 0x80 )
        {
          macRC522_MOSI_1 ();
        }		    
        else
        {
          macRC522_MOSI_0 ();
        }		    

//			Delay_us ( 3 );
        macRC522_DELAY();

        macRC522_SCK_0 ();

//			Delay_us ( 1 );
//			Delay_us ( 3 );
        macRC522_DELAY();
         
        macRC522_SCK_1();

//			Delay_us ( 3 );
        macRC522_DELAY();
         
        byte <<= 1; 
			
    } 
}

//==========================================================
//	函数名称：	SPI_RC522_ReadByte
//
//	函数功能：	从RC522发送1 Byte 数据
//
//	入口参数：	无
//
//	返回参数：	RC522返回的数据
//
//	说明：		
//==========================================================
unsigned char SPI_RC522_ReadByte ( void )
{
	unsigned char counter;
	unsigned char SPI_Data;


	for(counter=0;counter<8;counter++)
	{
            SPI_Data <<= 1;

            macRC522_SCK_0 ();

//			Delay_us ( 3 );
            macRC522_DELAY();
    
            if ( macRC522_MISO_GET() == 1)
               SPI_Data |= 0x01;

//			Delay_us ( 2 );
//			Delay_us ( 3 );
            macRC522_DELAY();

            macRC522_SCK_1 ();

//			Delay_us ( 3 );
            macRC522_DELAY();
			
	}
	
	return SPI_Data;
	
}

//==========================================================
//	函数名称：	ReadRawRC
//
//	函数功能：	读RC522寄存器
//
//	入口参数：	ucAddress，寄存器地址
//
//	返回参数：	寄存器的当前值
//
//	说明：		
//==========================================================
unsigned char ReadRawRC ( unsigned char ucAddress )
{
      unsigned char ucAddr, ucReturn;
      
      ucAddr = ( ( ucAddress << 1 ) & 0x7E ) | 0x80;
      
      macRC522_CS_Enable();
      
      SPI_RC522_SendByte ( ucAddr );
      
      ucReturn = SPI_RC522_ReadByte ();
      
      macRC522_CS_Disable();
      
      return ucReturn;
}
//==========================================================
//	函数名称：	WriteRawRC
//
//	函数功能：	写RC522寄存器
//
//	入口参数：	ucAddress，寄存器地址
//              ucValue，写入寄存器的值
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void WriteRawRC ( unsigned char ucAddress, unsigned char ucValue )
{  
	unsigned char ucAddr;
	
	ucAddr = ( ucAddress << 1 ) & 0x7E;
	
	macRC522_CS_Enable();
	
	SPI_RC522_SendByte ( ucAddr );
	
	SPI_RC522_SendByte ( ucValue );
	
	macRC522_CS_Disable();	

}
//==========================================================
//	函数名称：	SetBitMask
//
//	函数功能：	对RC522寄存器置位
//
//	入口参数：	ucReg，寄存器地址
//              ucMask，置位值
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void SetBitMask ( unsigned char ucReg, unsigned char ucMask )  
{
    unsigned char ucTemp;
	
    ucTemp = ReadRawRC ( ucReg );
	
    WriteRawRC ( ucReg, ucTemp | ucMask );         // set bit mask
}

//==========================================================
//	函数名称：	ClearBitMask
//
//	函数功能：	对RC522寄存器清位
//
//	入口参数：	ucReg，寄存器地址
//              ucMask，清位值
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ClearBitMask ( unsigned char ucReg, unsigned char ucMask )  
{
    unsigned char ucTemp;
		
    ucTemp = ReadRawRC ( ucReg );
	
    WriteRawRC ( ucReg, ucTemp & ( ~ ucMask) );  // clear bit mask
}

//==========================================================
//	函数名称：	PcdAntennaOn
//
//	函数功能：	开启天线 
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void PcdAntennaOn ( void )
{
    unsigned char uc;
	
    uc = ReadRawRC ( TxControlReg );
	
    if ( ! ( uc & 0x03 ) )
	SetBitMask(TxControlReg, 0x03);	
}

//==========================================================
//	函数名称：	PcdAntennaOff
//
//	函数功能：	关闭天线
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void PcdAntennaOff ( void )
{
  ClearBitMask ( TxControlReg, 0x03 );
}

//==========================================================
//	函数名称：	PcdReset
//
//	函数功能：	复位RC522 
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void PcdReset ( void )
{
	macRC522_Reset_Disable();
	
	delay_us ( 1 );
	
	macRC522_Reset_Enable();
	
	delay_us ( 1 );
	
	macRC522_Reset_Disable();
	
	delay_us ( 1 );
	
	WriteRawRC ( CommandReg, 0x0f );
	
	while ( ReadRawRC ( CommandReg ) & 0x10 );
	
	delay_us ( 1 );
	
        WriteRawRC ( ModeReg, 0x3D );           //定义发送和接收常用模式 和Mifare卡通讯，CRC初始值0x6363
	
        WriteRawRC ( TReloadRegL, 30 );         //16位定时器低位    
	WriteRawRC ( TReloadRegH, 0 );	        //16位定时器高位
	
        WriteRawRC ( TModeReg, 0x8D );	        //定义内部定时器的设置
	
        WriteRawRC ( TPrescalerReg, 0x3E );	//设置定时器分频系数
	
	WriteRawRC ( TxAutoReg, 0x40 );	        //调制发送信号为100%ASK	
	

}

//==========================================================
//	函数名称：	M500PcdConfigISOType
//
//	函数功能：	设置RC522的工作方式
//
//	入口参数：	ucType，工作方式
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void M500PcdConfigISOType ( unsigned char ucType )
{
  if ( ucType == 'A')                     //ISO14443_A
  {
        ClearBitMask ( Status2Reg, 0x08 );

        WriteRawRC ( ModeReg, 0x3D );//3F

        WriteRawRC ( RxSelReg, 0x86 );//84

        WriteRawRC( RFCfgReg, 0x7F );   //4F

        WriteRawRC( TReloadRegL, 30 );//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 

        WriteRawRC ( TReloadRegH, 0 );

        WriteRawRC ( TModeReg, 0x8D );

        WriteRawRC ( TPrescalerReg, 0x3E );

        delay_us ( 2 );

        PcdAntennaOn ();//开天线	
   } 
}

//==========================================================
//	函数名称：	PcdComMF522
//
//	函数功能：	通过RC522和ISO14443卡通讯
//
//	入口参数：	ucCommand，RC522命令字
//              pInData，通过RC522发送到卡片的数据
//              ucInLenByte，发送数据的字节长度
//              pOutData，接收到的卡片返回数据
//              pOutLenBit，返回数据的位长度
//
//	返回参数：	状态值 = MI_OK，成功
//
//	说明：		
//==========================================================
char PcdComMF522 ( unsigned char ucCommand, unsigned char * pInData, unsigned char ucInLenByte, unsigned char * pOutData, int * pOutLenBit )		
{
    char cStatus = MI_ERR;
    unsigned char ucIrqEn   = 0x00;
    unsigned char ucWaitFor = 0x00;
    unsigned char ucLastBits;
    unsigned char ucN;
    int ul;
	
	
    switch ( ucCommand )
    {
       case PCD_AUTHENT:		//Mifare认证
          ucIrqEn   = 0x12;		//允许错误中断请求ErrIEn  允许空闲中断IdleIEn
          ucWaitFor = 0x10;		//认证寻卡等待时候 查询空闲中断标志位
          break;
			 
       case PCD_TRANSCEIVE:		//接收发送 发送接收
          ucIrqEn   = 0x77;		//允许TxIEn RxIEn IdleIEn LoAlertIEn ErrIEn TimerIEn
          ucWaitFor = 0x30;		//寻卡等待时候 查询接收中断标志位与 空闲中断标志位
          break;
			 
       default:
         break;
			 
    }
   
    WriteRawRC ( ComIEnReg, ucIrqEn | 0x80 );		//IRqInv置位管脚IRQ与Status1Reg的IRq位的值相反 
    ClearBitMask ( ComIrqReg, 0x80 );			//Set1该位清零时，CommIRqReg的屏蔽位清零
    WriteRawRC ( CommandReg, PCD_IDLE );		//写空闲命令
    SetBitMask ( FIFOLevelReg, 0x80 );			//置位FlushBuffer清除内部FIFO的读和写指针以及ErrReg的BufferOvfl标志位被清除
    
    for ( ul = 0; ul < ucInLenByte; ul ++ )
		  WriteRawRC ( FIFODataReg, pInData [ ul ] );    		//写数据进FIFOdata
			
    WriteRawRC ( CommandReg, ucCommand );					//写命令
   
    
    if ( ucCommand == PCD_TRANSCEIVE )
			SetBitMask(BitFramingReg,0x80);  				//StartSend置位启动数据发送 该位与收发命令使用时才有效
    
    ul = 1000;//根据时钟频率调整，操作M1卡最大等待时间25ms
		
    do 														//认证 与寻卡等待时间	
    {
         ucN = ReadRawRC ( ComIrqReg );							//查询事件中断
         ul --;
    } while ( ( ul != 0 ) && ( ! ( ucN & 0x01 ) ) && ( ! ( ucN & ucWaitFor ) ) );		//退出条件i=0,定时器中断，与写空闲命令
		
    ClearBitMask ( BitFramingReg, 0x80 );					//清理允许StartSend位
		
    if ( ul != 0 )
    {
			if ( ! ( ReadRawRC ( ErrorReg ) & 0x1B ) )			//读错误标志寄存器BufferOfI CollErr ParityErr ProtocolErr
			{
				cStatus = MI_OK;
				
				if ( ucN & ucIrqEn & 0x01 )					//是否发生定时器中断
				  cStatus = MI_NOTAGERR;   
					
				if ( ucCommand == PCD_TRANSCEIVE )
				{
					ucN = ReadRawRC ( FIFOLevelReg );			//读FIFO中保存的字节数
					
					ucLastBits = ReadRawRC ( ControlReg ) & 0x07;	//最后接收到得字节的有效位数
					
					if ( ucLastBits )
						* pOutLenBit = ( ucN - 1 ) * 8 + ucLastBits;   	//N个字节数减去1（最后一个字节）+最后一位的位数 读取到的数据总位数
					else
						* pOutLenBit = ucN * 8;   					//最后接收到的字节整个字节有效
					
					if ( ucN == 0 )		
            ucN = 1;    
					
					if ( ucN > MAXRLEN )
						ucN = MAXRLEN;   
					
					for ( ul = 0; ul < ucN; ul ++ )
					  pOutData [ ul ] = ReadRawRC ( FIFODataReg );   
					
					}		
      }
			
			else
				cStatus = MI_ERR;   
    }
   
   SetBitMask ( ControlReg, 0x80 );           // stop timer now
   WriteRawRC ( CommandReg, PCD_IDLE ); 
	
   return cStatus;
}

//==========================================================
//	函数名称：	PcdRequest
//
//	函数功能：	寻卡
//
//	入口参数：	ucReq_code，寻卡方式
//                     = 0x52，寻感应区内所有符合14443A标准的卡
//                     = 0x26，寻未进入休眠状态的卡
//              pTagType，卡片类型代码
//                   = 0x4400，Mifare_UltraLight
//                   = 0x0400，Mifare_One(S50)
//                   = 0x0200，Mifare_One(S70)
//                   = 0x0800，Mifare_Pro(X))
//                   = 0x4403，Mifare_DESFire
//
//	返回参数：	状态值 = MI_OK，成功
//
//	说明：		
//==========================================================
char PcdRequest ( unsigned char ucReq_code, unsigned char * pTagType )
{
   char cStatus;  
   unsigned char ucComMF522Buf [ MAXRLEN ]; 
   int ulLen;

   ClearBitMask ( Status2Reg, 0x08 );	//清理指示MIFARECyptol单元接通以及所有卡的数据通信被加密的情况
   WriteRawRC ( BitFramingReg, 0x07 );	//	发送的最后一个字节的 七位
   SetBitMask ( TxControlReg, 0x03 );	//TX1,TX2管脚的输出信号传递经发送调制的13.56的能量载波信号

   ucComMF522Buf [ 0 ] = ucReq_code;		//存入 卡片命令字

   cStatus = PcdComMF522 ( PCD_TRANSCEIVE,	ucComMF522Buf, 1, ucComMF522Buf, & ulLen );	//寻卡  
    //	入口参数：	ucCommand，RC522命令字
//              pInData，通过RC522发送到卡片的数据
//              ucInLenByte，发送数据的字节长度
//              pOutData，接收到的卡片返回数据
//              pOutLenBit，返回数据的位长度
  
   if ( ( cStatus == MI_OK ) && ( ulLen == 0x10 ) )	//寻卡成功返回卡类型 
   {    
       * pTagType = ucComMF522Buf [ 0 ];
       * ( pTagType + 1 ) = ucComMF522Buf [ 1 ];
   }
   else
     cStatus = MI_ERR;
	 
   return cStatus; 
}

//==========================================================
//	函数名称：	PcdAnticoll
//
//	函数功能：	防冲撞
//
//	入口参数：	pSnr，卡片序列号，4字节
//
//	返回参数：	状态值 = MI_OK，成功
//
//	说明：		
//==========================================================
char PcdAnticoll ( unsigned char * pSnr )
{
    char cStatus;
    unsigned char uc, ucSnr_check = 0;
    unsigned char ucComMF522Buf [ MAXRLEN ]; 
	int ulLen;
    

    ClearBitMask ( Status2Reg, 0x08 );		//清MFCryptol On位 只有成功执行MFAuthent命令后，该位才能置位
    WriteRawRC ( BitFramingReg, 0x00);		//清理寄存器 停止收发
    ClearBitMask ( CollReg, 0x80 );			//清ValuesAfterColl所有接收的位在冲突后被清除
   
    ucComMF522Buf [ 0 ] = 0x93;	//卡片防冲突命令
    ucComMF522Buf [ 1 ] = 0x20;
   
    cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, & ulLen);//与卡片通信
	
    if ( cStatus == MI_OK)		//通信成功
    {
			for ( uc = 0; uc < 4; uc ++ )
			{
                 * ( pSnr + uc )  = ucComMF522Buf [ uc ];			//读出UID
                 ucSnr_check ^= ucComMF522Buf [ uc ];
            }
			
      if ( ucSnr_check != ucComMF522Buf [ uc ] )
				cStatus = MI_ERR;    
				 
    }
    
    SetBitMask ( CollReg, 0x80 );
		
		
    return cStatus;
		
		
}

//==========================================================
//	函数名称：	CalulateCRC
//
//	函数功能：	用RC522计算CRC16
//
//	入口参数：	pIndata，计算CRC16的数组
//              ucLen，计算CRC16的数组字节长度
//              pOutData，存放计算结果存放的首地址
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void CalulateCRC ( unsigned char * pIndata, unsigned char ucLen, unsigned char * pOutData )
{
    unsigned char uc, ucN;
	
	
    ClearBitMask(DivIrqReg,0x04);
	
    WriteRawRC(CommandReg,PCD_IDLE);
	
    SetBitMask(FIFOLevelReg,0x80);
	
    for ( uc = 0; uc < ucLen; uc ++)
	    WriteRawRC ( FIFODataReg, * ( pIndata + uc ) );   

    WriteRawRC ( CommandReg, PCD_CALCCRC );
	
    uc = 0xFF;
	
    do 
    {
        ucN = ReadRawRC ( DivIrqReg );
        uc --;
    } while ( ( uc != 0 ) && ! ( ucN & 0x04 ) );
		
    pOutData [ 0 ] = ReadRawRC ( CRCResultRegL );
    pOutData [ 1 ] = ReadRawRC ( CRCResultRegM );
		
		
}

//==========================================================
//	函数名称：	PcdSelect
//
//	函数功能：	选定卡片
//
//	入口参数：	pSnr，卡片序列号，4字节
//
//	返回参数：	状态值 = MI_OK，成功
//
//	说明：		
//==========================================================
char PcdSelect ( unsigned char * pSnr )
{
    char ucN;
    unsigned char uc;
	  unsigned char ucComMF522Buf [ MAXRLEN ]; 
    int  ulLen;
    
    
    ucComMF522Buf [ 0 ] = PICC_ANTICOLL1;
    ucComMF522Buf [ 1 ] = 0x70;
    ucComMF522Buf [ 6 ] = 0;
	
    for ( uc = 0; uc < 4; uc ++ )
    {
    	ucComMF522Buf [ uc + 2 ] = * ( pSnr + uc );
    	ucComMF522Buf [ 6 ] ^= * ( pSnr + uc );
    }
		
    CalulateCRC ( ucComMF522Buf, 7, & ucComMF522Buf [ 7 ] );
  
    ClearBitMask ( Status2Reg, 0x08 );

    ucN = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, & ulLen );
    
    if ( ( ucN == MI_OK ) && ( ulLen == 0x18 ) )
      ucN = MI_OK;  
    else
      ucN = MI_ERR;    

		
    return ucN;
		
		
}

//==========================================================
//	函数名称：	PcdAuthState
//
//	函数功能：	验证卡片密码
//
//	入口参数：	ucAuth_mode，密码验证模式
//                     = 0x60，验证A密钥
//                     = 0x61，验证B密钥
//         unsigned char ucAddr，块地址
//         pKey，密码
//         pSnr，卡片序列号，4字节
//
//	返回参数：	 状态值 = MI_OK，成功
//
//	说明：		
//==========================================================
char PcdAuthState ( unsigned char ucAuth_mode, unsigned char ucAddr, unsigned char * pKey, unsigned char * pSnr )
{
    char cStatus;
	  unsigned char uc, ucComMF522Buf [ MAXRLEN ];
    int ulLen;
    
	
    ucComMF522Buf [ 0 ] = ucAuth_mode;
    ucComMF522Buf [ 1 ] = ucAddr;
	
    for ( uc = 0; uc < 6; uc ++ )
	    ucComMF522Buf [ uc + 2 ] = * ( pKey + uc );   
	
    for ( uc = 0; uc < 6; uc ++ )
	    ucComMF522Buf [ uc + 8 ] = * ( pSnr + uc );   

    cStatus = PcdComMF522 ( PCD_AUTHENT, ucComMF522Buf, 12, ucComMF522Buf, & ulLen );
	
    if ( ( cStatus != MI_OK ) || ( ! ( ReadRawRC ( Status2Reg ) & 0x08 ) ) )
      cStatus = MI_ERR;   
    
		
    return cStatus;
		
		
}

//==========================================================
//	函数名称：	PcdWrite
//
//	函数功能：	写数据到M1卡一块
//
//	入口参数：	unsigned char ucAddr，块地址
//              pData，写入的数据，16字节
//
//	返回参数：	状态值 = MI_OK，成功
//
//	说明：		
//==========================================================
char PcdWrite ( unsigned char ucAddr, unsigned char * pData )
{
    char cStatus;
	  unsigned char uc, ucComMF522Buf [ MAXRLEN ];
    int ulLen;
     
    
    ucComMF522Buf [ 0 ] = PICC_WRITE;
    ucComMF522Buf [ 1 ] = ucAddr;
	
    CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
 
    cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );

    if ( ( cStatus != MI_OK ) || ( ulLen != 4 ) || ( ( ucComMF522Buf [ 0 ] & 0x0F ) != 0x0A ) )
      cStatus = MI_ERR;   
        
    if ( cStatus == MI_OK )
    {
			//memcpy(ucComMF522Buf, pData, 16);
      for ( uc = 0; uc < 16; uc ++ )
			  ucComMF522Buf [ uc ] = * ( pData + uc );  
			
      CalulateCRC ( ucComMF522Buf, 16, & ucComMF522Buf [ 16 ] );

      cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 18, ucComMF522Buf, & ulLen );
			
			if ( ( cStatus != MI_OK ) || ( ulLen != 4 ) || ( ( ucComMF522Buf [ 0 ] & 0x0F ) != 0x0A ) )
        cStatus = MI_ERR;   
			
    } 
		
		
    return cStatus;
			
}

//==========================================================
//	函数名称：	PcdRead
//
//	函数功能：	读取M1卡一块数据
//
//	入口参数：	unsigned char ucAddr，块地址
//               pData，读出的数据，16字节
//
//	返回参数：	状态值 = MI_OK，成功
//
//	说明：		
//==========================================================
char PcdRead ( unsigned char ucAddr, unsigned char * pData )
{
    char cStatus;
	  unsigned char uc, ucComMF522Buf [ MAXRLEN ]; 
    int ulLen;
    

    ucComMF522Buf [ 0 ] = PICC_READ;
    ucComMF522Buf [ 1 ] = ucAddr;
	
    CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
   
    cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );
	
    if ( ( cStatus == MI_OK ) && ( ulLen == 0x90 ) )
    {
			for ( uc = 0; uc < 16; uc ++ )
        * ( pData + uc ) = ucComMF522Buf [ uc ];   
    }
		
    else
      cStatus = MI_ERR;   
    
		
    return cStatus;
		
		
}

//==========================================================
//	函数名称：	PcdHalt
//
//	函数功能：	命令卡片进入休眠状态
//
//	入口参数：	无
//
//	返回参数：	状态值 = MI_OK，成功
//
//	说明：		
//==========================================================
char PcdHalt( void )
{
	unsigned char ucComMF522Buf [ MAXRLEN ]; 
	int  ulLen;
  

  ucComMF522Buf [ 0 ] = PICC_HALT;
  ucComMF522Buf [ 1 ] = 0;
	
  CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
 	PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );

  return MI_OK;
	
}


void IC_CMT ( unsigned char * UID, unsigned char * KEY, unsigned char RW, unsigned char * Dat )
{
  unsigned char ucArray_ID [ 4 ] = { 0 };//先后存放IC卡的类型和UID(IC卡序列号)
  
	
  PcdRequest ( 0x52, ucArray_ID );//寻卡

  PcdAnticoll ( ucArray_ID );//防冲撞
  
  PcdSelect ( UID );//选定卡
  
  PcdAuthState ( 0x60, 0x10, KEY, UID );//校验
	

	if ( RW )//读写选择，1是读，0是写
    PcdRead ( 0x10, Dat );
   
   else 
     PcdWrite ( 0x10, Dat );
   
	 
   PcdHalt ();
	 
	 
}


