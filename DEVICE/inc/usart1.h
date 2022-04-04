#ifndef _USART1_H_
#define _USART1_H_

#include "stm32f10x.h"

void Usart1_Init(void);
void Usart1_Dma_Send(uint32_t data_address, uint16_t size);
void Usart1_Init_Qingzhou(void);
void usart1_send(u8 data);

#endif //_USART1_H_
