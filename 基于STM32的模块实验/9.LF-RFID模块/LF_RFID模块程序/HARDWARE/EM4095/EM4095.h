#ifndef __EM4095_H
#define __EM4095_H
#define GET_ID 1
#define NOT_ID 0
void EM4095_Init(void);
uint8_t EM4095_SearchHdr(uint8_t *pcardid);
#define READ_DEMOD()                    HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  
#endif
