#ifndef _USART1_H_
#define _USART1_H_

#include "stm32f10x.h"
#include "usart4.h"

void Usart1_Init(void);
void Usart1_Dma_Send(uint32_t data_address, uint16_t size);
void Usart1_Init_Self(void);
uint8_t *Get_Usart1_Rxd_Len(void);
uint8_t *Get_Usart1_Rxd_Buffer(void);

#endif //_USART1_H_
