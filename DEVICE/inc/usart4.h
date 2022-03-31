#ifndef _USART4_H_
#define _USART4_H_

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stdio.h"

void Uart4_Init(void);
uint8_t* Get_Uart4_Rxd_Length(void);
uint8_t *Get_Uart4_Rxd_Buffer(void);
void DMA2_Channel4_5_IRQHandler(void);
void Usart4_Dma_Send(uint32_t data_address, uint16_t size);

#endif //_USART4_H_
