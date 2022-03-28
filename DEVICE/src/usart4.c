#include "usart4.h"

#if 1
#pragma import(__use_no_semihosting)

struct __FILE
{
	int handle;
};

FILE __stdout;
void _sys_exit(int x)
{
	x = x;
}
int fputc(int ch, FILE *f)
{
	while ((UART4->SR & 0X40) == 0)
		;
	UART4->DR = (u8)ch;
	return ch;
}

#endif

static char uart4_dma_rx_buf[128];
static uint8_t uart4_dma_rxd_length = 0;
static const uint16_t uart4_dma_rx_max_size = 128;
// PC10 -> Tx
// PC11 -> Rx
void Uart4_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //! 使能 UART4 发送和接收端口时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//! 串口4空闲中断（一帧数据接收完毕）
	USART_ITConfig(UART4, USART_IT_IDLE, DISABLE);
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
	USART_Cmd(UART4, ENABLE);

	//! DMA2流3 串口4接收
	DMA_DeInit(DMA2_Channel3);

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (UART4->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uart4_dma_rx_buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = uart4_dma_rx_max_size;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //! 接收开启循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA2_Channel3, &DMA_InitStructure);
	DMA_Cmd(DMA2_Channel3, ENABLE);
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);
}

void UART4_IRQHandler(void)
{
	if (USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(DMA2_Channel3, DISABLE);

		(void)UART4->SR;
		(void)UART4->DR;

		uart4_dma_rxd_length = uart4_dma_rx_max_size - DMA_GetCurrDataCounter(DMA2_Channel3);

		DMA_SetCurrDataCounter(DMA2_Channel3, uart4_dma_rx_max_size);

		// Shell_Get_Data();

		DMA_Cmd(DMA2_Channel3, ENABLE);
	}
}