#ifndef _USART4_H_
#define _USART4_H_

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stdio.h"

void Uart4_Init(void);
uint8_t* Get_Uart4_Rxd_Length(void);
uint8_t *Get_Uart4_Rxd_Buffer(void);

#endif //_USART4_H_
