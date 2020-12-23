#include "UART2RS485.h"
#include "MT_UART.h"
#include "HAL_DEFS.h"
#include "string.h"
#include "stdio.h"
/*
初始化UART转485芯片收发控制引脚
*/
void UART2RS485_Init(void)
{  
    halUARTCfg_t uartConfig;
    
    P1SEL &= ~BV(3);     //设置P1.3为普通IO口  
    P1DIR |= BV(3);      //P1.3定义为输出 
    UartInit(HAL_UART_PORT_1,HAL_UART_BR_115200);
    printf("this is uart1 print\r\n");
}
/*
通过RS485总线发送数据
*/
void RS485Bus_Send(void)
{
  
}