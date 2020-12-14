#ifndef __TM1640_H
#define __TM1640_H
#include "stm32f1xx_hal.h"

/*			TM1604			*/	 
#define SET_DATA_ORDER	0X40 //数据命令设置 普通模式

#define SET_DISPLAY 0X8c	//显示命令控制，脉冲宽度 11/16

#define	sda_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
#define	scl_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);

#define	sda_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
#define	scl_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	 
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

