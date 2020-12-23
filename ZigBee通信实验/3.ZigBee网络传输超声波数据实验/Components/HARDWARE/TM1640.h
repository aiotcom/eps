#ifndef __TM1640_H
#define __TM1640_H

#include "ioCC2530.h"
#include <hal_types.h>
#include "delay.h"

/*			TM1604			*/	 
#define SET_DATA_ORDER	0X40    //数据命令设置 普通模式

#define SET_DISPLAY     0X8c    //显示命令控制，脉冲宽度 11/16

#define	sda_H P0_2 = 1;
#define	scl_H P0_3 = 1;

#define	sda_L P0_2 = 0;
#define	scl_L P0_3 = 0;
	 
/*			TM1604			*/		
extern void TM1640_Init(void);
extern void start(void);
extern void stop(void);
extern void write(unsigned char date);
extern void init_tm1640(void);
extern void send_LED_Display(unsigned char address,uint16_t data,unsigned char type);
extern void send_z(uint8_t address,uint8_t date);
extern void send(uint8_t address, uint8_t date);
extern void init_tm1640_NEW(void);

#endif

