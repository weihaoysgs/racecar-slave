#ifndef _UART4_H_
#define _UART4_H_

#include "stm32f1xx.h"
#include "usart.h"
#include "cmsis_os.h"
#include "stdarg.h"
#include "stdio.h"
#include "chassis_task.h"
#include "stdlib.h"
#include "string.h"
#include "default_task.h"

void Uart4_Tx_Init(void);
void Uart4_Transmit_Dma(uint32_t data_address, uint32_t len);
void Uart4_It_Tc_Callback(void);
void Uart4_Rx_Init(void);
uint8_t *Get_Uart4_DMA_RxBuffer(void);
const uint16_t *Get_Uart4_DMA_Rx_MaxLen(void);
void Uart4_DMA_RxCp_Callback(void);
int ___printf(const char *format, ...);

#endif //_UART4_H_
