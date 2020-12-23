#ifndef __Rs485_H
#define __Rs485_H
#include "stm32f1xx_hal.h"

#define 		Send_REC_LEN  				200  			//定义最大接收字节数 200
#define 		Receive_REC_LEN  			200  			//定义最大接收字节数 200

#define			Receiving_Completion	0x8000		//接收完成

/*		模式控制		*/ 
#define RS485_TX_EN()		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_SET)		//485发送模式
#define RS485_RX_EN()		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_RESET)	//485接收模式

/*		设备地址		*/
#define			Addr_Power						0x1001			//电源盒地址
#define			Addr_LED							0x1101			//LED模块地址
#define			Addr_Matrixkey				0x1201			//矩阵键盘模块地址
#define			Addr_Fan							0x1301			//风扇模块地址
#define			Addr_Relay						0x1401			//继电器地址
#define			Addr_LCD							0x1501			//LCD屏地址
#define			Addr_SHT20						0x1601			//温湿度模块地址
#define			Addr_Pre							0x1701			//压力传感器模块地址
#define			Addr_HF_RFID					0x1801			//HF_RFID模块地址
#define			Addr_Gyr							0x1901			//陀螺仪模块地址
#define			Addr_PIR							0x1A01			//人体红外模块地址
#define			Addr_NB								0x1B01			//NB-IoT模块地址
#define			Addr_BLE							0x1C01			//蓝牙模块地址
#define			Addr_ZigBee						0x1D01			//ZigBee模块地址
#define			Addr_GPS							0x1E01			//GPS模块地址
#define			Addr_WiFi							0x1F01			//WiFi模块地址
#define			Addr_Ultrasonic				0x2001			//超声波模块地址
#define			Addr_StepperMotor			0x2101			//步进电机模块地址
#define			Addr_PM2_5						0x2201			//PM2.5模块地址
#define			Addr_IPV4							0x2301			//IPV4模块地址
#define			Addr_Flame						0x2401			//火焰传感器模块地址
#define			Addr_Shock						0x2501			//震动传感器模块地址
#define			Addr_Heart						0x2601			//心率传感器地址
#define			Addr_LF_RFID					0x2701			//LF_RFID模块地址
#define			Addr_A_P							0x2801			//大气压力模块地址
#define			Addr_LORA							0x2901			//LORA模块地址
#define			Addr_MP3							0x2A01			//MP3模块地址
#define			Addr_PH								0x2B01			//光电开关模块地址
#define			Addr_Touch_Key				0x2C01			//触摸按键模块地址
#define			Addr_TF								0x2D01			//TF卡模块地址
#define			Addr_Alcohol					0x2E01			//酒精传感器模块地址
#define			Addr_Formaldehyde			0x2F01			//甲醛传感器模块地址
#define			Addr_Smoke						0x3001			//烟雾传感器模块地址
#define			Addr_Color						0x3101			//颜色传感器模块地址
#define			Addr_Noncontact_Temp	0x3201			//非接触温度测量传感器模块地址
#define			Addr_Moisture					0x3301			//土壤湿度传感器模块地址
#define			Addr_Gesture					0x3401			//手势识别传感器模块地址
#define			Addr_BEEP					0x3501			//蜂鸣器地址
/*		命令码		*/

#define			LED_Control						0x1100		//控制LED
#define			LED_ON1								0x1111		//打开LED1
#define			LED_ON2								0x1121		//打开LED2
#define			LED_ON3								0x1131		//打开LED3
#define			LED_ON4								0x1141		//打开LED4
#define			LED_OFF1							0x1110		//关闭LED1
#define			LED_OFF2							0x1120		//关闭LED2
#define			LED_OFF3							0x1130		//关闭LED3
#define			LED_OFF4							0x1140		//关闭LED4

#define			Matrixkey_Get_value		0x1200		//矩阵键盘-被获取按键值
#define			Matrixkey0						0x1201		//矩阵键盘-按键0
#define			Matrixkey1						0x1211		//矩阵键盘-按键1
#define			Matrixkey2						0x1221		//矩阵键盘-按键2
#define			Matrixkey3						0x1231		//矩阵键盘-按键3
#define			Matrixkey4						0x1241		//矩阵键盘-按键4
#define			Matrixkey5						0x1251		//矩阵键盘-按键5
#define			Matrixkey6						0x1261		//矩阵键盘-按键6
#define			Matrixkey7						0x1271		//矩阵键盘-按键7
#define			Matrixkey8						0x1281		//矩阵键盘-按键8
#define			Matrixkey9						0x1291		//矩阵键盘-按键9
#define			MatrixkeyEnter				0x12A1		//矩阵键盘-按键Enter
#define			MatrixkeyCanel				0x12B1		//矩阵键盘-按键Canel
#define			MatrixkeyUP						0x12C1		//矩阵键盘-按键UP
#define			MatrixkeyDown					0x12D1		//矩阵键盘-按键Down
#define			MatrixkeyLeft					0x12E1		//矩阵键盘-按键Left
#define			MatrixkeyRight				0x12F1		//矩阵键盘-按键Right

#define			FAN_OFF								0x1310		//关闭风扇
#define			FAN_ON1								0x1311		//打开风扇-1档
#define			FAN_ON2								0x1321		//打开风扇-2档
#define			FAN_ON3								0x1331		//打开风扇-3档

#define			Relay1_ON							0x1411		//打开继电器1
#define			Relay2_ON							0x1421		//打开继电器2
#define			Relay1_OFF						0x1410		//关闭继电器1
#define			Relay2_OFF						0x1420		//关闭继电器2

#define			LCD_Display						0x1511		//发送数据到LCD屏显示

#define			SHT20_Temp						0x1601		//温湿度-温度
#define			SHT20_Humi						0x1611		//温湿度-湿度
#define			SHT20_Light						0x1621		//温湿度-光照强度
#define			SHT20_All							0x1631		//温湿度-全部数据
#define			SHT20_Get_Temp				0x1600		//温湿度-被获取温度
#define			SHT20_Get_Humi				0x1610		//温湿度-被获取湿度
#define			SHT20_Get_Light				0x1620		//温湿度-被获取光照强度
#define			SHT20_Get_All					0x1630		//温湿度-被获取全部数据

#define			Pre_Weight						0x1701		//压力-重量
#define			Pre_Get_Weight				0x1700		//压力-被获取重量

#define			HF_RFID_ID						0x1801		//HF_RFID-卡号
#define			HF_RFID_Block					0x1811		//HF_RFID-块数据
#define			HF_RFID_All						0x1821		//HF_RFID-卡号+块数据
#define			HF_RFID_Get_ID				0x1800		//HF_RFID-被获取卡号
#define			HF_RFID_Get_Block			0x1810		//HF_RFID-被获取块数据
#define			HF_RFID_Get_All				0x1820		//HF_RFID-被获取卡号+块数据

#define			Gyr_Angle							0x1901		//陀螺仪-角度
#define			Gyr_Get_Angle					0x1900		//陀螺仪-被获取角度

#define			PIR_State							0x1A01		//人体红外-是否检测到人体
#define			PIR_Get_State					0x1A00		//人体红外-被获取是否检测到人体

#define			GPS_Location					0x1E01		//GPS-位置数据(经纬度)
#define			GPS_Get_Location			0x1E00		//GPS-被获取位置数据(经纬度)

#define			Ultrasonic_Dis				0x2001		//超声波-距离(cm)
#define			Ultrasonic_Get_Dis		0x2000		//超声波-被获取距离(cm)

#define			Motor_OFF							0x2100		//步进电机-关闭
#define			Motor_ON							0x2101		//步进电机-开启

#define			PM2_5_Conc						0x2201		//PM2.5-PM2.5浓度
#define			PM2_5_Get_Conc				0x2200		//PM2.5-被获取PM2.5浓度

#define			Flame_State						0x2401		//火焰传感器-是否起火状态
#define			Flame_Get_State				0x2400		//火焰传感器-被获取是否起火状态

#define			Shock_Count						0x2501		//震动-震动次数
#define			Shock_Get_Count				0x2500		//震动-被获取震动次数

#define			Heart_Count						0x2601		//心率-每分钟心跳次数
#define			Heart_Get_Count				0x2600		//心率-被获取每分钟心跳次数

#define			LF_RFID_ID						0x2701		//LF_RFID-卡号
#define			LF_RFID_Block					0x2711		//LF_RFID-块数据
#define			LF_RFID_All						0x2721		//LF_RFID-卡号+块数据
#define			LF_RFID_Get_ID				0x2700		//LF_RFID-被获取卡号
#define			LF_RFID_Get_Block			0x2710		//LF_RFID-被获取块数据
#define			LF_RFID_Get_All				0x2720		//LF_RFID-被获取卡号+块数据

#define			A_P_Altitude					0x2801		//大气压力-海拔高度
#define			A_P_Get_Altitude			0x2800		//大气压力-被获取海拔高度

#define			MP3_TO_MP3						0x2A00		//MP3-发送给MP3模块播放

#define			PH_State							0x2B01		//光电开关-开关状态
#define			PH_Get_State					0x2B00		//光电开关-被获取开关状态

#define			Touch_Key_State				0x2C01		//触摸按键-按键状态
#define			Touch_Key_Get_State		0x2C00		//触摸按键-被获取按键状态

#define			TF_Data								0x2D01		//TF卡-数据
#define			TF_Get_Data						0x2D00		//TF卡-被获取数据

#define			Alcohol_Data					0x2E01		//酒精传感器-酒精浓度数据
#define			Alcohol_Get_Data			0x2E00		//酒精传感器-被获取酒精浓度数据
#define			Alcohol_State					0x2E11		//酒精传感器-是否检测到酒精
#define			Alcohol_Get_State			0x2E10		//酒精传感器-被获取是否检测到酒精

#define			Formaldehyde_Data			0x2F01		//甲醛传感器-甲醛浓度数据
#define			Formaldehyde_Get_Data	0x2F00		//甲醛传感器-被获取甲醛浓度数据

#define			Smoke_Data						0x3001		//烟雾传感器-烟雾浓度数据
#define			Smoke_Get_Data				0x3000		//烟雾传感器-被获取烟雾浓度数据
#define			Smoke_State						0x3011		//烟雾传感器-检测是否有烟雾
#define			Smoke_Get_State				0x3010		//烟雾传感器-被获取是否有烟雾

#define			Color_Data						0x3101		//颜色传感器-RGB颜色数据
#define			Color_Get_Data				0x3100		//颜色传感器-被获取RGB颜色数据

#define			Noncontact_Temp_Data	0x3201		//非接触温度检测传感器-温度值
#define			Noncontact_Temp_Get_Data	0x3200//非接触温度检测传感器-被获取温度值

#define			Moisture_Data					0x3301		//土壤湿度传感器-湿度数据
#define			Moisture_Get_Data			0x3300		//土壤湿度传感器-被获取湿度数据

#define			Gesture_Get_Action		0x3400		//手势传感器-获取动作
#define			Gesture_Up_Down				0x3411		//手势传感器-从上到下
#define			Gesture_Down_Up				0x3410		//手势传感器-从下到上
#define			Gesture_Left_Right		0x3421		//手势传感器-从左到右
#define			Gesture_Right_Left		0x3420		//手势传感器-从右到左
#define			Gesture_Beyond				0x3431		//手势传感器-远离
#define			Gesture_Near					0x3430		//手势传感器-靠近
#define			BEEP_CTRL					    0x3500		//蜂鸣器控制
/*
		定义485发送数据结构体
*/
typedef struct
{
    uint32_t Head;					//协议帧头
		uint8_t Pack_Num;				//包序号
		uint16_t Src_Adr;				//源地址
		uint16_t Dst_Adr;				//目的地址
    uint16_t State;					//状态码
		uint8_t Data_Len;				//数据长度
    uint8_t Data[Send_REC_LEN];//数据
		uint16_t CrcCheck;			//CRC校验码
		uint32_t Tail;					//协议帧尾
}Send_Stack;

/*
		定义485接收数据结构体
*/
typedef struct
{
    uint32_t Head;					//协议帧头
		uint8_t Pack_Num;				//包序号
		uint16_t Src_Adr;				//源地址
		uint16_t Dst_Adr;				//目的地址
    uint16_t Command;				//命令码
		uint8_t Data_Len;				//数据长度
    uint8_t Data[Receive_REC_LEN];//数据
		uint16_t CrcCheck;			//CRC校验码
		uint32_t Tail;					//协议帧尾
}Receive_Stack;

/*
		声明485接收数据结构体
*/
extern Send_Stack Tx_Stack;														//485发送数据结构体
extern Receive_Stack Rx_Stack;												//485接收数据结构体

/*
		声明485发送/接收数组
*/
extern uint8_t Send_Data[Send_REC_LEN]; 							//发送缓冲,最大Send_REC_LEN个字节
extern uint8_t Receive_Data[Receive_REC_LEN]; 				//接收缓冲,最大Send_REC_LEN个字节

/*
		声明函数
*/
extern void Rs485_Init(void);  		//初始化485
extern void Tx_Stack_Init(void);	//485发送数据结构体初始化
extern void Rx_Stack_Init(void);	//485接收数据结构体初始化
void Rs485_delay_ms(unsigned int nms);								//485延时函数
extern void Rs485_Receive(uint8_t data);							//485接收函数
extern uint16_t CRC_16(void *p, int length);					//CRC_16计算函数
extern uint8_t DataHandling_485(uint16_t DesAddr);		//485数据处理
extern uint16_t calcByte(uint16_t crc, uint8_t b);		//CRC_16字节计算函数
extern void Rs485_Send(uint16_t SrcAdr,uint16_t DstAdr,uint16_t Cmd,uint8_t Len,uint8_t *pData);//485发送函数

#endif



