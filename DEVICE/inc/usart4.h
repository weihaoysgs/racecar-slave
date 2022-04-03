#ifndef _USART4_H_
#define _USART4_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "pstwo_task.h"

void Uart4_Init(void);
uint8_t *Get_Uart4_Rxd_Buffer(void);
void Uart4_DMA_Reset(void);

#endif //_USART4_H_
