#include "SHT20.h"
#include "TM1640.h"
#include <ioCC2530.h>
#include "OLED.h"
SHT2x_PARAM g_sht2x_param;
//==========================================================
//	函数名称：	IIC_Delay
//
//	函数功能：	IIC延时函数
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================

static void IIC_Delay(uint32 n)
{
  while(0);
}

//==========================================================
//	函数名称：	SHT20_IIC_Init
//
//	函数功能：	初始化IIC
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static void SHT20_IIC_Init(void)
{
    P0SEL &= ~0x03;     //设置P0.0、P0.1为普通IO口  
    P0DIR |= 0x03;      //P0.0、P0.1定义为输出
}

//==========================================================
//	函数名称：	IIC_SCL_OUTPUT
//
//	函数功能：	IIC时钟线输出模式
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static void IIC_SCL_OUTPUT(void)
{
    P0SEL &= ~0x02;     //设置P0.1为普通IO口  
    P0DIR |= 0x02;      //P0.1定义为输出
}

//==========================================================
//	函数名称：	IIC_SCL_INPUT
//
//	函数功能：	IIC时钟线输入模式
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static void IIC_SCL_INPUT(void)
{
    P0SEL &= ~0x02;     //设置P0.1为普通IO口  
    P0DIR &= ~0x02;     //按键接在P0.6口上，设P0.6为输入模式 
    P0INP &= ~0x02;     //打开P0.1上拉电阻
}

//==========================================================
//	函数名称：	IIC_SDA_OUTPUT
//
//	函数功能：	IIC数据线输出模式 
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static void IIC_SDA_OUTPUT(void)
{
    P0SEL &= ~0x01;     //设置P0.0为普通IO口  
    P0DIR |= 0x01;      //P0.0定义为输出
}
//==========================================================
//	函数名称：	IIC_SDA_INPUT
//
//	函数功能：	IIC数据线输入模式
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static void IIC_SDA_INPUT(void)
{
    P0SEL &= ~0x01;     //设置P0.0为普通IO口  
    P0DIR &= ~0x01;     //按键接在P0.0口上，设P0.7为输入模式 
    P0INP &= ~0x01;     //打开P0.0上拉电阻
}
//==========================================================
//	函数名称：	IIC_Start
//
//	函数功能：	IIC启动时序
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static void IIC_Start(void)
{
    IIC_SDA_OUTPUT();	//时钟线输出模式
    IIC_SCL_OUTPUT();	//数据线输出模式
    SDA_HIGH();		//拉高数据线
    SCL_HIGH();		//拉高时钟线
    SDA_LOW();		//拉低数据线			
    SCL_LOW();		//拉低时钟线
}
//==========================================================
//	函数名称：	IIC_Stop
//
//	函数功能：	IIC停止时序 
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static void IIC_Stop(void)
{
    IIC_SDA_OUTPUT();
    IIC_SCL_OUTPUT();
    SCL_LOW();
    SDA_LOW();
    SCL_HIGH();
    SDA_HIGH();
}

//==========================================================
//	函数名称：	IIC_Wait_Ack
//
//	函数功能：	等待应答信号到来
//
//	入口参数：	无
//
//	返回参数：	返回值：1，接收应答失败
//        							0，接收应答成功
//
//	说明：		
//==========================================================
//static uint8 IIC_Wait_Ack(void)
//{
//    uint8 Time=0;
//    IIC_SCL_OUTPUT();
//    IIC_SDA_OUTPUT();
//    SDA_HIGH();
//    SCL_HIGH();
//    IIC_SDA_INPUT();
//    while(SDA_STATE())
//    {
//      Time++;
//      if(Time>250)
//      {
//        IIC_Stop();
//        return 1;
//      }
//    }
//    SCL_LOW();
//    return 0;
//}
//==========================================================
//	函数名称：	IIC_Ack
//
//	函数功能：	IIC ACK应答
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static void IIC_Ack(void)
{
    IIC_SCL_OUTPUT();
    SCL_LOW();
    IIC_SDA_OUTPUT();
    SDA_LOW();
    SCL_HIGH();
    SCL_LOW();
}

//==========================================================
//	函数名称：	IIC_NAck
//
//	函数功能：	IIC 无ACK应答
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static void IIC_NAck(void)
{
    IIC_SCL_OUTPUT();
    SCL_LOW();
    IIC_SDA_OUTPUT();
    SDA_HIGH();
    SCL_HIGH();
    SCL_LOW();
}
//==========================================================
//	函数名称：	IIC_Read_Byte
//
//	函数功能：	IIC 读取数据
//
//	入口参数：	ack:是否发送ACK
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static uint8 IIC_Read_Byte(uint8 ack)
{
    uint8 i,receive=0;
    IIC_SDA_INPUT();
    IIC_SCL_OUTPUT();
    for(i=0;i<8;i++)
    {
        SCL_LOW();
        SCL_HIGH();
        receive<<=1;
        if(SDA_STATE())receive++;
    }
    if(!ack) IIC_NAck();
    else IIC_Ack();
    return receive;
}


//==========================================================
//	函数名称：	Write_IIC_Byte
//
//	函数功能：	IIC 写入数据
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static uint8 Write_IIC_Byte(uint8 byte)
{
    uint8 i, ack;

    IIC_SCL_OUTPUT();
    IIC_SDA_OUTPUT();		//设置为输出模式
    
    for(i = 0; i < 8; i++)
    {
        if(byte & 0x80)         //一次读取最高位 发送数据
        {
            SDA_HIGH();
        }
        else 
        {
            SDA_LOW();
        }
        
        SCL_HIGH();  		//时钟线拉高
        SCL_LOW();   		//时钟线拉低
        
        byte <<= 1;
    }

    IIC_SDA_INPUT();  	        //数据线输入模式
    
    SCL_HIGH();			//时钟线拉高
    
    if(Bit_SET == SDA_STATE())  //如果接受到数据线高信号  ACK错误
    {
        ack = ACK_ERROR;
    }
    else
    {
        ack = ACK_OK;
    }
    
    SCL_LOW();          //拉低时钟线

    IIC_SDA_OUTPUT();	

    SDA_HIGH();		//拉高数据线
    
    return (ack);
}
//==========================================================
//	函数名称：	SHT2x_I2cReadByte
//
//	函数功能：	IIC 读取数据
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
uint8 SHT2x_I2cReadByte(void)
{
    uint8 i, val = 0;

    IIC_SCL_OUTPUT();  		    //设置为输出模式
    IIC_SDA_OUTPUT();

    SDA_HIGH();			    //数据线拉高
    
    IIC_SDA_INPUT();		    //数据线设置为输入模式
        
    for(i = 0; i < 8; i++)
    {
        val <<= 1; 
        
        SCL_HIGH();  	 	    //拉高时钟线
        
        if(Bit_SET == SDA_STATE())  //如果数据线被拉高
        {
            val |= 0x01;	    //读取数据线中的数据
        }
        
        SCL_LOW();      	    //拉低时钟线
    }

    IIC_SDA_OUTPUT();  		    //数据线输出模式

    SDA_HIGH();			    //数据线拉高
    
    return (val);		    //返回读取到的数据
}
////==========================================================
////	函数名称：	IIC_Send_Byte
////
////	函数功能：	IIC 写入数据
////
////	入口参数：	txd:写入数据
////
////	返回参数：	无
////
////	说明：		
////==========================================================
//static void IIC_Send_Byte(uint8 txd)
//{
//    uint8 t;
//    IIC_SCL_OUTPUT();
//    IIC_SDA_OUTPUT();
//    SCL_LOW();
//    for(t=0;t<8;t++)
//    {
//        if(txd & 0x80)
//        {
//            SDA_HIGH();
//        }
//        else 
//        {
//            SDA_LOW();
//        }
//        
//        SCL_HIGH();
//        SCL_LOW();
//        txd<<=1;
//    }
//}

//==========================================================
//	函数名称：	SHT2x_MeasureTempHM
//
//	函数功能：	主机模式温度读取
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
float SHT2x_MeasureTempHM(void)
{
    float TEMP;
    uint8 tmp1, tmp2;
    uint16 ST;
    
    IIC_SCL_OUTPUT();  					//时钟线设置为输出模式
     
    IIC_Start();    					//开启IIC传输状态                       
    Write_IIC_Byte(I2C_ADR_W);				//写入0x80 表示开始写数据
    Write_IIC_Byte(TRIG_TEMP_MEASUREMENT_HM); 		//主机模式相对温度
  
    IIC_Start();					//开启IIC传输状态      
    Write_IIC_Byte(I2C_ADR_R);				//写入0x81 表示开始读数据

    SCL_HIGH();   					//拉高时钟线

    IIC_SCL_INPUT();					//设置为输入模式

    while(Bit_RESET == SCL_STATE())  			//如果该时钟线被置低
    {
        IIC_Delay(20);
    }
    
    tmp1 = SHT2x_I2cReadByte();                         //读取数据
    IIC_Ack();						//ACK应答
    tmp2 = SHT2x_I2cReadByte();     	                //读取数据
    IIC_NAck();						//不应答
    IIC_Stop();						//关闭IIC
    
    ST = (tmp1 << 8) | (tmp2 << 0);
    ST &= ~0x0003;
    TEMP = ((float)ST * 0.00268127) - 46.85;  //转换所得到的数据

    IIC_SCL_OUTPUT();  			      //时钟线输出

    return (TEMP);	    		      //返回温度值
}

//==========================================================
//	函数名称：	SHT2x_MeasureHumiHM
//
//	函数功能：	主机模式湿度读取
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
float SHT2x_MeasureHumiHM(void)
{
    float HUMI;
    uint8 tmp1, tmp2;    
    uint16 SRH;

    IIC_SCL_OUTPUT();
    
    IIC_Start();                               
    Write_IIC_Byte(I2C_ADR_W);
    Write_IIC_Byte(TRIG_HUMI_MEASUREMENT_HM);

    IIC_Start();
    Write_IIC_Byte(I2C_ADR_R);

    SCL_HIGH();

    IIC_SCL_INPUT();

    while(Bit_RESET == SCL_STATE())
    {
        IIC_Delay(20);
    }
    
    tmp1 = SHT2x_I2cReadByte();
    IIC_Ack();
    tmp2 = SHT2x_I2cReadByte();
    IIC_NAck();
    IIC_Stop();
    
    SRH = (tmp1 << 8) | (tmp2 << 0);
    SRH &= ~0x0003;
    HUMI = ((float)SRH * 0.00190735) - 6;

    IIC_SCL_OUTPUT();

    return (HUMI);
}

//==========================================================
//	函数名称：	SHT2x_MeasureTempPoll
//
//	函数功能：	非主机模式温度读取 
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
float SHT2x_MeasureTempPoll(void)
{
    float TEMP;
    uint8 ack, tmp1, tmp2;
    uint16 ST;
    
    IIC_Start();                            
    Write_IIC_Byte(I2C_ADR_W);
    Write_IIC_Byte(TRIG_TEMP_MEASUREMENT_POLL);

    do {
        IIC_Delay(20);               
        IIC_Start();
        ack = Write_IIC_Byte(I2C_ADR_R);
    } while(ACK_ERROR == ack);
    
    tmp1 = SHT2x_I2cReadByte();
    IIC_Ack();
    tmp2 = SHT2x_I2cReadByte();
    IIC_NAck();
    IIC_Stop();
    
    ST = (tmp1 << 8) | (tmp2 << 0);
    ST &= ~0x0003;
    TEMP = ((float)ST * 0.00268127) - 46.85;

    return (TEMP);	  
}

//==========================================================
//	函数名称：	SHT2x_MeasureHumiPoll
//
//	函数功能：	非主机模式湿度读取 
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
float SHT2x_MeasureHumiPoll(void)
{
    float HUMI;
    uint8 ack, tmp1, tmp2;    
    uint16 SRH;
    
    IIC_Start();                               
    Write_IIC_Byte(I2C_ADR_W);
    Write_IIC_Byte(TRIG_HUMI_MEASUREMENT_POLL);
    
    do {
        IIC_Delay(20);
        IIC_Start();
        ack = Write_IIC_Byte(I2C_ADR_R);
    } while(ACK_ERROR == ack);
    
    tmp1 = SHT2x_I2cReadByte();
    IIC_Ack();
    tmp2 = SHT2x_I2cReadByte();
    IIC_NAck();
    IIC_Stop();
    
    SRH = (tmp1 << 8) | (tmp2 << 0);
    SRH &= ~0x0003;
    HUMI = ((float)SRH * 0.00190735) - 6;

    return (HUMI);
}

//==========================================================
//	函数名称：	SHT2x_GetSerialNumber
//
//	函数功能：	序列号
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void SHT2x_GetSerialNumber(uint8 *buf)
{
    /* Read from memory location 1 */
    IIC_Start();
    Write_IIC_Byte(I2C_ADR_W);	                //I2C address
    Write_IIC_Byte(0xFA); 			//Command for readout on-chip memory
    Write_IIC_Byte(0x0F); 			//on-chip memory address
    IIC_Start();
    Write_IIC_Byte(I2C_ADR_R);	                //I2C address
    buf[5] = SHT2x_I2cReadByte();               //Read SNB_3
    IIC_Ack();
    SHT2x_I2cReadByte(); 			//Read CRC SNB_3 (CRC is not analyzed)
    IIC_Ack();
    buf[4] = SHT2x_I2cReadByte(); 	        //Read SNB_2
    IIC_Ack();
    IIC_Read_Byte(0); 				//Read CRC SNB_2 (CRC is not analyzed)
    IIC_Ack();
    buf[3] = SHT2x_I2cReadByte(); 	        //Read SNB_1
    IIC_Ack();
    SHT2x_I2cReadByte(); 			//Read CRC SNB_1 (CRC is not analyzed)
    IIC_Ack();
    buf[2] = SHT2x_I2cReadByte(); 	        //Read SNB_0
    IIC_Ack();
    SHT2x_I2cReadByte(); 			//Read CRC SNB_0 (CRC is not analyzed)
    IIC_Ack();
    IIC_Stop();

    /* Read from memory location 2 */
    IIC_Start();
    Write_IIC_Byte(I2C_ADR_W); 	                //I2C address
    Write_IIC_Byte(0xFC); 			//Command for readout on-chip memory
    Write_IIC_Byte(0xC9); 			//on-chip memory address
    IIC_Start();
    Write_IIC_Byte(I2C_ADR_R); 	                //I2C address
    buf[1] = SHT2x_I2cReadByte(); 	        //Read SNC_1
    IIC_Ack();
    buf[0] = SHT2x_I2cReadByte(); 	        //Read SNC_0
    IIC_Ack();
    IIC_Read_Byte(0); 			        //Read CRC SNC0/1 (CRC is not analyzed)
    IIC_Ack();
    buf[7] = SHT2x_I2cReadByte(); 	        //Read SNA_1
    IIC_Ack();
    buf[6] = SHT2x_I2cReadByte(); 	        //Read SNA_0
    IIC_Ack();
    SHT2x_I2cReadByte(); 			//Read CRC SNA0/1 (CRC is not analyzed)
    IIC_NAck();
    IIC_Stop();
}

//==========================================================
//	函数名称：	SHT2x_GetTempHumi
//
//	函数功能：	得到的数据 
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void SHT2x_GetTempHumi(void)
{   
    g_sht2x_param.TEMP_HM = SHT2x_MeasureTempHM();			//主机模式下的温度数据
    g_sht2x_param.HUMI_HM = SHT2x_MeasureHumiHM(); 			//主机模式下的湿度数据
}
//==========================================================
//	函数名称：	SHT2x_GetTemp
//
//	函数功能：	得到的数据 
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
uint8 SHT2x_GetTemp(void)
{
    return (uint8)g_sht2x_param.TEMP_HM;
}
uint8 SHT2x_GetHumi(void)
{
    return (uint8)g_sht2x_param.HUMI_HM;
}
void SHT2x_DisplayHumi(void)
{
    g_sht2x_param.HUMI_HM = SHT2x_MeasureHumiHM();	
    send_LED_Display(0xc0,(uint8)g_sht2x_param.HUMI_HM,2);
}
void SHT2x_DisplayTemp(void)
{
    g_sht2x_param.TEMP_HM = SHT2x_MeasureTempHM();
    send_LED_Display(0xc0,(uint8)g_sht2x_param.TEMP_HM,1); 
}
void SHT2x_Init(void)
{
  SHT20_IIC_Init();
  TM1640_Init();
  SHT2x_GetTempHumi(); 
}