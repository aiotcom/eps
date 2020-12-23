#include "SHT20.h"	

SHT2x_PARAM g_sht2x_param;

//==========================================================
//	函数名称：	SHT2x_Delay
//
//	函数功能：	延时函数
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void SHT2x_Delay(u32 n)
{
    u8 i;
    
    while(n--)
    {
        for(i = 0; i < 100; i++)
        {
            __NOP();
        }
    }
}


//==========================================================
//	函数名称：	SHT2x_Init
//
//	函数功能：	初始化IIC
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void SHT2x_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	__HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOB时钟

    GPIO_InitStructure.Pin =  SHT2x_SCL_PIN | SHT2x_SDA_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SHT2x_I2C_PORT, &GPIO_InitStructure);

    SHT2x_SCL_HIGH();
    SHT2x_SDA_HIGH();

    SHT2x_Delay(80);

    SHT2x_SoftReset();
}

//==========================================================
//	函数名称：	SHT2x_SCL_OUTPUT
//
//	函数功能：	IIC时钟线输出模式
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void SHT2x_SCL_OUTPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		//__HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOB时钟

    GPIO_InitStructure.Pin =  SHT2x_SCL_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SHT2x_I2C_PORT, &GPIO_InitStructure);
}

//==========================================================
//	函数名称：	SHT2x_SCL_INPUT
//
//	函数功能：	IIC时钟线输入模式
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void SHT2x_SCL_INPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		//__HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOB时钟

    GPIO_InitStructure.Pin =  SHT2x_SCL_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(SHT2x_I2C_PORT, &GPIO_InitStructure);
}

//==========================================================
//	函数名称：	SHT2x_SDA_OUTPUT
//
//	函数功能：	IIC数据线输出模式 
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void SHT2x_SDA_OUTPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		//__HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOB时钟

    GPIO_InitStructure.Pin =  SHT2x_SDA_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;				//输出
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;				//低速
    HAL_GPIO_Init(SHT2x_I2C_PORT, &GPIO_InitStructure);
}

//==========================================================
//	函数名称：	SHT2x_SDA_INPUT
//
//	函数功能：	IIC数据线输入模式
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void SHT2x_SDA_INPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		//__HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOB时钟

    GPIO_InitStructure.Pin =  SHT2x_SDA_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;			//输入模式
    HAL_GPIO_Init(SHT2x_I2C_PORT, &GPIO_InitStructure);
}

//==========================================================
//	函数名称：	SHT2x_I2cStartCondition
//
//	函数功能：	IIC启动时序
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void SHT2x_I2cStartCondition(void)
{
    SHT2x_SCL_OUTPUT();			//时钟线输出模式
    SHT2x_SDA_OUTPUT();			//数据线输出模式
    
    SHT2x_SDA_HIGH();				//拉高数据线
    SHT2x_SCL_HIGH();				//拉高时钟线
    SHT2x_SDA_LOW();				//拉低数据线
    SHT2x_Delay(30);
    SHT2x_SCL_LOW();				//拉低时钟线
    SHT2x_Delay(30);
}

//==========================================================
//	函数名称：	SHT2x_I2cStopCondition
//
//	函数功能：	IIC停止时序 
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void SHT2x_I2cStopCondition(void)
{
    SHT2x_SCL_OUTPUT();
    SHT2x_SDA_OUTPUT();
    
    SHT2x_SDA_LOW();
    SHT2x_SCL_LOW();
    SHT2x_SCL_HIGH();
    SHT2x_Delay(30);
    SHT2x_SDA_HIGH();
    SHT2x_Delay(30);
}

//==========================================================
//	函数名称：	SHT2x_I2cAcknowledge
//
//	函数功能：	IIC ACK应答
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void SHT2x_I2cAcknowledge(void)
{
    SHT2x_SCL_OUTPUT();
    SHT2x_SDA_OUTPUT();

    SHT2x_SDA_LOW();
    
    SHT2x_SCL_HIGH();
    SHT2x_Delay(80);
    SHT2x_SCL_LOW();   
    SHT2x_Delay(80);
}

//==========================================================
//	函数名称：	SHT2x_I2cNoAcknowledge
//
//	函数功能：	IIC 无ACK应答
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void SHT2x_I2cNoAcknowledge(void)
{
    SHT2x_SCL_OUTPUT();
    SHT2x_SDA_OUTPUT();

    SHT2x_SDA_HIGH();
    
    SHT2x_SCL_HIGH();
    SHT2x_Delay(80);
    SHT2x_SCL_LOW();   
    SHT2x_Delay(80);
}

//==========================================================
//	函数名称：	SHT2x_I2cReadByte
//
//	函数功能：	IIC 读取数据
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
u8 SHT2x_I2cReadByte(void)
{
    u8 i, val = 0;


    SHT2x_SCL_OUTPUT();  //设置为输出模式
    SHT2x_SDA_OUTPUT();

    SHT2x_SDA_HIGH();					//数据线拉高
    
    SHT2x_SDA_INPUT();				//数据线设置为输入模式
        
    for(i = 0; i < 8; i++)
    {
        val <<= 1; 
        
        SHT2x_SCL_HIGH();  	 	//拉高时钟线
        
        if(Bit_SET == SHT2x_SDA_STATE())   //如果数据线被拉高
        {
            val |= 0x01;			//读取数据线中的数据
        }
        
        SHT2x_SCL_LOW();      //拉低时钟线
    }

    SHT2x_SDA_OUTPUT();  			//数据线输出模式

    SHT2x_SDA_HIGH();					//数据线拉高
    
    return (val);							//返回读取到的数据
}

//==========================================================
//	函数名称：	SHT2x_I2cWriteByte
//
//	函数功能：	IIC 写入数据
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
u8 SHT2x_I2cWriteByte(u8 byte)
{
    u8 i, ack;


    SHT2x_SCL_OUTPUT();
    SHT2x_SDA_OUTPUT();		//设置为输出模式
    
    for(i = 0; i < 8; i++)
    {
        if(byte & 0x80)   //一次读取最高位 发送数据
        {
            SHT2x_SDA_HIGH();
        }
        else 
        {
            SHT2x_SDA_LOW();
        }
        
        SHT2x_SCL_HIGH();  //时钟线拉高
        SHT2x_Delay(80);
        SHT2x_SCL_LOW();   //时钟线拉低
        SHT2x_Delay(80);
        
        byte <<= 1;
    }

    SHT2x_SDA_INPUT();  //数据线输入模式
    
    SHT2x_SCL_HIGH();		//时钟线拉高
    
    if(Bit_SET == SHT2x_SDA_STATE())   //如果接受到数据线高信号  ACK错误
    {
        ack = ACK_ERROR;
    }
    else
    {
        ack = ACK_OK;
    }
    
    SHT2x_SCL_LOW();    //拉低时钟线

    SHT2x_SDA_OUTPUT();	

    SHT2x_SDA_HIGH();		//拉高数据线
    
    return (ack);
}

//==========================================================
//	函数名称：	SHT2x_MeasureTempHM
//
//	函数功能：	主机模式温度读取
//
//	入口参数：	时间
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
    

    SHT2x_SCL_OUTPUT();  													//时钟线设置为输出模式
     
    SHT2x_I2cStartCondition();    								//开启IIC传输状态                       
    SHT2x_I2cWriteByte(I2C_ADR_W);								//写入0x80 表示开始写数据
    SHT2x_I2cWriteByte(TRIG_TEMP_MEASUREMENT_HM); //主机模式相对温度

    SHT2x_I2cStartCondition();										//开启IIC传输状态      
    SHT2x_I2cWriteByte(I2C_ADR_R);								//写入0x81 表示开始读数据

    SHT2x_SCL_HIGH();   													//拉高时钟线

    SHT2x_SCL_INPUT();														//设置为输入模式

    while(Bit_RESET == SHT2x_SCL_STATE())  				//如果该时钟线被置低
    {
        SHT2x_Delay(20);
    }
    
    tmp1 = SHT2x_I2cReadByte(); //读取数据
    SHT2x_I2cAcknowledge();			//ACK应答
    tmp2 = SHT2x_I2cReadByte();	//读取数据
    SHT2x_I2cNoAcknowledge();		//不应答
    SHT2x_I2cStopCondition();		//关闭IIC
    
    ST = (tmp1 << 8) | (tmp2 << 0);
    ST &= ~0x0003;
    TEMP = ((float)ST * 0.00268127) - 46.85;  //转换所得到的数据

    SHT2x_SCL_OUTPUT();  	//时钟线输出

    return (TEMP);	    	//返回温度值
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


    SHT2x_SCL_OUTPUT();
    
    SHT2x_I2cStartCondition();                               
    SHT2x_I2cWriteByte(I2C_ADR_W);
    SHT2x_I2cWriteByte(TRIG_HUMI_MEASUREMENT_HM);

    SHT2x_I2cStartCondition();
    SHT2x_I2cWriteByte(I2C_ADR_R);

    SHT2x_SCL_HIGH();

    SHT2x_SCL_INPUT();

    while(Bit_RESET == SHT2x_SCL_STATE())
    {
        SHT2x_Delay(20);
    }
    
    tmp1 = SHT2x_I2cReadByte();
    SHT2x_I2cAcknowledge();
    tmp2 = SHT2x_I2cReadByte();
    SHT2x_I2cNoAcknowledge();
    SHT2x_I2cStopCondition();
    
    SRH = (tmp1 << 8) | (tmp2 << 0);
    SRH &= ~0x0003;
    HUMI = ((float)SRH * 0.00190735) - 6;

    SHT2x_SCL_OUTPUT();

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
    
    SHT2x_I2cStartCondition();                            
    SHT2x_I2cWriteByte(I2C_ADR_W);
    SHT2x_I2cWriteByte(TRIG_TEMP_MEASUREMENT_POLL);

    do {
        SHT2x_Delay(20);               
        SHT2x_I2cStartCondition();
        ack = SHT2x_I2cWriteByte(I2C_ADR_R);
    } while(ACK_ERROR == ack);
    
    tmp1 = SHT2x_I2cReadByte();
    SHT2x_I2cAcknowledge();
    tmp2 = SHT2x_I2cReadByte();
    SHT2x_I2cNoAcknowledge();
    SHT2x_I2cStopCondition();
    
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
    
    SHT2x_I2cStartCondition();                               
    SHT2x_I2cWriteByte(I2C_ADR_W);
    SHT2x_I2cWriteByte(TRIG_HUMI_MEASUREMENT_POLL);
    
    do {
        SHT2x_Delay(20);
        SHT2x_I2cStartCondition();
        ack = SHT2x_I2cWriteByte(I2C_ADR_R);
    } while(ACK_ERROR == ack);
    
    tmp1 = SHT2x_I2cReadByte();
    SHT2x_I2cAcknowledge();
    tmp2 = SHT2x_I2cReadByte();
    SHT2x_I2cNoAcknowledge();
    SHT2x_I2cStopCondition();
    
    SRH = (tmp1 << 8) | (tmp2 << 0);
    SRH &= ~0x0003;
    HUMI = ((float)SRH * 0.00190735) - 6;

    return (HUMI);
}

//==========================================================
//	函数名称：	SHT2x_ReadUserReg
//
//	函数功能：	读取数据
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
u8 SHT2x_ReadUserReg(void)    
{
    u8 reg;
  
    SHT2x_I2cStartCondition();                 
    SHT2x_I2cWriteByte(I2C_ADR_W);
    SHT2x_I2cWriteByte(USER_REG_R);
    SHT2x_I2cStartCondition(); 
    SHT2x_I2cWriteByte(I2C_ADR_R);
    reg = SHT2x_I2cReadByte();
    SHT2x_I2cNoAcknowledge();
    SHT2x_I2cStopCondition();
    
    return (reg); 
}

//==========================================================
//	函数名称：	SHT2x_WriteUserReg
//
//	函数功能：	发送ACK数据
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
u8 SHT2x_WriteUserReg(u8 reg)
{
    u8 ack;
    
    SHT2x_I2cStartCondition();
    SHT2x_I2cWriteByte(I2C_ADR_W);
    SHT2x_I2cWriteByte(USER_REG_W);
    ack = SHT2x_I2cWriteByte(reg);
    SHT2x_I2cStopCondition();  
    
    return (ack);
}

//==========================================================
//	函数名称：	SHT2x_SoftReset
//
//	函数功能：	软复位
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void SHT2x_SoftReset(void)
{
    SHT2x_I2cStartCondition();
    SHT2x_I2cWriteByte(I2C_ADR_W);
    SHT2x_I2cWriteByte(SOFT_RESET);
    SHT2x_I2cStopCondition();

    SHT2x_Delay(80);
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
    SHT2x_I2cStartCondition();
    SHT2x_I2cWriteByte(I2C_ADR_W);//I2C address
    SHT2x_I2cWriteByte(0xFA); 		//Command for readout on-chip memory
    SHT2x_I2cWriteByte(0x0F); 		//on-chip memory address
    SHT2x_I2cStartCondition();
    SHT2x_I2cWriteByte(I2C_ADR_R);//I2C address
    buf[5] = SHT2x_I2cReadByte(); //Read SNB_3
    SHT2x_I2cAcknowledge();
    SHT2x_I2cReadByte(); 					//Read CRC SNB_3 (CRC is not analyzed)
    SHT2x_I2cAcknowledge();
    buf[4] = SHT2x_I2cReadByte(); //Read SNB_2
    SHT2x_I2cAcknowledge();
    SHT2x_I2cReadByte(); 					//Read CRC SNB_2 (CRC is not analyzed)
    SHT2x_I2cAcknowledge();
    buf[3] = SHT2x_I2cReadByte(); //Read SNB_1
    SHT2x_I2cAcknowledge();
    SHT2x_I2cReadByte(); 					//Read CRC SNB_1 (CRC is not analyzed)
    SHT2x_I2cAcknowledge();
    buf[2] = SHT2x_I2cReadByte(); //Read SNB_0
    SHT2x_I2cAcknowledge();
    SHT2x_I2cReadByte(); 					//Read CRC SNB_0 (CRC is not analyzed)
    SHT2x_I2cNoAcknowledge();
    SHT2x_I2cStopCondition();

    /* Read from memory location 2 */
    SHT2x_I2cStartCondition();
    SHT2x_I2cWriteByte(I2C_ADR_W); 	//I2C address
    SHT2x_I2cWriteByte(0xFC); 			//Command for readout on-chip memory
    SHT2x_I2cWriteByte(0xC9); 			//on-chip memory address
    SHT2x_I2cStartCondition();
    SHT2x_I2cWriteByte(I2C_ADR_R); 	//I2C address
    buf[1] = SHT2x_I2cReadByte(); 	//Read SNC_1
    SHT2x_I2cAcknowledge();
    buf[0] = SHT2x_I2cReadByte(); 	//Read SNC_0
    SHT2x_I2cAcknowledge();
    SHT2x_I2cReadByte(); 						//Read CRC SNC0/1 (CRC is not analyzed)
    SHT2x_I2cAcknowledge();
    buf[7] = SHT2x_I2cReadByte(); 	//Read SNA_1
    SHT2x_I2cAcknowledge();
    buf[6] = SHT2x_I2cReadByte(); 	//Read SNA_0
    SHT2x_I2cAcknowledge();
    SHT2x_I2cReadByte(); 						//Read CRC SNA0/1 (CRC is not analyzed)
    SHT2x_I2cNoAcknowledge();
    SHT2x_I2cStopCondition();
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

    g_sht2x_param.TEMP_POLL = SHT2x_MeasureTempPoll();  //非主机模式下的温度数据
    g_sht2x_param.HUMI_POLL = SHT2x_MeasureHumiPoll();	//非主机模式下的湿度数据
    
    SHT2x_GetSerialNumber(g_sht2x_param.SerialNumber);	//序列号
}




