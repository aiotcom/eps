#include "delay.h"
#include "IIC.h"

//==========================================================
//	函数名称：	OLED_IIC_Delay
//
//	函数功能：	OLED_IIC延时函数
//
//	入口参数：	时间
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OLED_IIC_Delay(u32 n)
{
  
}

//==========================================================
//	函数名称：	OLED_IIC_Init
//
//	函数功能：	初始化OLED_IIC
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void IIC_Init(void)
{
    P0SEL &= ~0xC0;     //设置P0.6、P0.7为普通IO口  
    P0DIR |= 0xC0;      //P0.6、P0.7定义为输出
}

//==========================================================
//	函数名称：	OLED_IIC_SCL_OUTPUT
//
//	函数功能：	OLED_IIC时钟线输出模式
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OLED_IIC_SCL_OUTPUT(void)
{
    P0SEL &= ~0x40;     //设置P0.6为普通IO口  
    P0DIR |= 0x40;      //P0.6定义为输出
}

//==========================================================
//	函数名称：	OLED_IIC_SCL_INPUT
//
//	函数功能：	OLED_IIC时钟线输入模式
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OLED_IIC_SCL_INPUT(void)
{
    P0SEL &= ~0x40;     //设置P0.6为普通IO口  
    P0DIR &= ~0x40;     //按键接在P0.6口上，设P0.6为输入模式 
    P0INP &= ~0x40;     //打开P0.6上拉电阻
}

//==========================================================
//	函数名称：	OLED_IIC_SDA_OUTPUT
//
//	函数功能：	OLED_IIC数据线输出模式 
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OLED_IIC_SDA_OUTPUT(void)
{
    P0SEL &= ~0x80;     //设置P0.7为普通IO口  
    P0DIR |= 0x80;      //P0.7定义为输出
}

//==========================================================
//	函数名称：	OLED_IIC_SDA_INPUT
//
//	函数功能：	OLED_IIC数据线输入模式
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OLED_IIC_SDA_INPUT(void)
{
    P0SEL &= ~0x80;     //设置P0.7为普通IO口  
    P0DIR &= ~0x80;     //按键接在P0.7口上，设P0.7为输入模式 
    P0INP &= ~0x80;     //打开P0.7上拉电阻
}

//==========================================================
//	函数名称：	OLED_IIC_Start
//
//	函数功能：	OLED_IIC启动时序
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OLED_IIC_Start(void)
{
    OLED_IIC_SDA_OUTPUT();	//时钟线输出模式
    OLED_IIC_SCL_OUTPUT();	//数据线输出模式
    OLED_SDA_HIGH();		//拉高数据线
    OLED_SCL_HIGH();		//拉高时钟线
    OLED_SDA_LOW();		//拉低数据线			
    OLED_SCL_LOW();		//拉低时钟线
}

//==========================================================
//	函数名称：	OLED_IIC_Stop
//
//	函数功能：	OLED_IIC停止时序 
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OLED_IIC_Stop(void)
{
    OLED_IIC_SDA_OUTPUT();
    OLED_IIC_SCL_OUTPUT();
    OLED_SCL_LOW();
    OLED_SDA_LOW();
    OLED_SCL_HIGH();
    OLED_SDA_HIGH();
}

//==========================================================
//	函数名称：	OLED_IIC_Wait_Ack
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
u8 OLED_IIC_Wait_Ack(void)
{
    u8 Time=0;
    OLED_IIC_SCL_OUTPUT();
    OLED_IIC_SDA_OUTPUT();
    OLED_SDA_HIGH();
    OLED_SCL_HIGH();
    OLED_IIC_SDA_INPUT();
    while(OLED_SDA_STATE())
    {
      Time++;
      if(Time>250)
      {
        OLED_IIC_Stop();
        return 1;
      }
    }
    OLED_SCL_LOW();
    return 0;
}



//==========================================================
//	函数名称：	OLED_IIC_Ack
//
//	函数功能：	OLED_IIC ACK应答
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OLED_IIC_Ack(void)
{
    OLED_IIC_SCL_OUTPUT();
    OLED_SCL_LOW();
    OLED_IIC_SDA_OUTPUT();
    OLED_SDA_LOW();
    OLED_SCL_HIGH();
    OLED_SCL_LOW();
}

//==========================================================
//	函数名称：	OLED_IIC_NAck
//
//	函数功能：	IIC 无ACK应答
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OLED_IIC_NAck(void)
{
    OLED_IIC_SCL_OUTPUT();
    OLED_SCL_LOW();
    OLED_IIC_SDA_OUTPUT();
    OLED_SDA_HIGH();
    OLED_SCL_HIGH();
    OLED_SCL_LOW();
}

//==========================================================
//	函数名称：	OLED_IIC_Read_Byte
//
//	函数功能：	OLED_IIC 读取数据
//
//	入口参数：	ack:是否发送ACK
//
//	返回参数：	无
//
//	说明：		
//==========================================================
u8 OLED_IIC_Read_Byte(u8 ack)
{
    u8 i,receive=0;
    OLED_IIC_SDA_INPUT();
    OLED_IIC_SCL_OUTPUT();
    for(i=0;i<8;i++)
    {
        OLED_SCL_LOW();
        OLED_SCL_HIGH();
        receive<<=1;
        if(OLED_SDA_STATE())receive++;
    }
    if(!ack) OLED_IIC_NAck();
    else OLED_IIC_Ack();
    return receive;
}

//==========================================================
//	函数名称：	Write_OLED_IIC_Byte
//
//	函数功能：	OLED_IIC 写入数据
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
u8 Write_OLED_IIC_Byte(u8 byte)
{
    u8 i, ack;

    OLED_IIC_SCL_OUTPUT();
    OLED_IIC_SDA_OUTPUT();	//设置为输出模式
    
    for(i = 0; i < 8; i++)
    {
        if(byte & 0x80)         //一次读取最高位 发送数据
        {
            OLED_SDA_HIGH();
        }
        else 
        {
            OLED_SDA_LOW();
        }
        
        OLED_SCL_HIGH();  	//时钟线拉高
        OLED_SCL_LOW();   	//时钟线拉低
        
        byte <<= 1;
    }

    OLED_IIC_SDA_INPUT();  	//数据线输入模式
    OLED_SCL_HIGH();		//时钟线拉高

    if(OLED_Bit_SET == OLED_SDA_STATE())  //如果接受到数据线高信号  ACK错误
    {
        ack = OLED_ACK_ERROR;
    }
    else
    {
        ack = OLED_ACK_OK;
    }
    
    OLED_SCL_LOW();           //拉低时钟线

    OLED_IIC_SDA_OUTPUT();	

    OLED_SDA_HIGH();	      //拉高数据线
    
    return (ack);
}

//==========================================================
//	函数名称：	OLED_IIC_Send_Byte
//
//	函数功能：	OLED_IIC 写入数据
//
//	入口参数：	txd:写入数据
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OLED_IIC_Send_Byte(u8 txd)
{
    u8 t;
    OLED_IIC_SCL_OUTPUT();
    OLED_IIC_SDA_OUTPUT();
    OLED_SCL_LOW();
    for(t=0;t<8;t++)
    {
        if(txd & 0x80)
        {
            OLED_SDA_HIGH();
        }
        else 
        {
            OLED_SDA_LOW();
        }
        
        OLED_SCL_HIGH();
        OLED_SCL_LOW();
        txd<<=1;
    }
}


