#ifndef _USART5_H_
#define _USART5_H_

#include "stm32f10x.h"
#include "stdio.h"

void Uart5_Init(void);
uint32_t* Get_Uart5_Rx_Length(void);
uint8_t *Get_Uart5_Rx_Buffer(void);
uint32_t Get_Uart5_Rx_Max_Len(void);

#endif //_USART5_H_
