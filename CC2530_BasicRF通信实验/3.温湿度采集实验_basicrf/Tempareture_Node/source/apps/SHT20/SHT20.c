#include "SHT20.h"
#include "IIC.h"

SHT2x_PARAM g_sht2x_param;

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
    u8 tmp1, tmp2;
    u16 ST;
    
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
    u8 tmp1, tmp2;    
    u16 SRH;

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
    u8 ack, tmp1, tmp2;
    u16 ST;
    
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
    u8 ack, tmp1, tmp2;    
    u16 SRH;
    
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
void SHT2x_GetSerialNumber(u8 *buf)
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

//    g_sht2x_param.TEMP_POLL = SHT2x_MeasureTempPoll();              //非主机模式下的温度数据
//    g_sht2x_param.HUMI_POLL = SHT2x_MeasureHumiPoll();	      //非主机模式下的湿度数据
//    
//    SHT2x_GetSerialNumber(g_sht2x_param.SerialNumber);	      //序列号
}
