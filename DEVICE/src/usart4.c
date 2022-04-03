#include "usart4.h"

static const uint16_t uart4_dma_rx_max_size = 25;
static uint8_t uart4_dma_rx_buf[25];

// PC10 -> Tx NOTUSE
// PC11 -> Rx
void Uart4_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 100000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	USART_Init(UART4, &USART_InitStructure);
	USART_Cmd(UART4, ENABLE);
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);

	DMA_DeInit(DMA2_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(UART4->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&uart4_dma_rx_buf[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = uart4_dma_rx_max_size;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //! ���տ���ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel3, &DMA_InitStructure);
	DMA_Cmd(DMA2_Channel3, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, DISABLE);
}

uint8_t *Get_Uart4_Rxd_Buffer(void)
{
	return uart4_dma_rx_buf;
}

void Uart4_DMA_Reset(void)
{
    DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, DISABLE);
	USART_Cmd(UART4, DISABLE);
	DMA_Cmd(DMA2_Channel3, DISABLE);
	DMA_SetCurrDataCounter(DMA2_Channel3, uart4_dma_rx_max_size);
	memset(uart4_dma_rx_buf, 0u, uart4_dma_rx_max_size);
	DMA_Cmd(DMA2_Channel3, ENABLE);
	USART_Cmd(UART4, ENABLE);
    DMA_ClearITPendingBit(DMA2_IT_TC3);
    DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE);
	
}

void DMA2_Channel3_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_IT_TC3))
	{
		DMA_ClearITPendingBit(DMA2_IT_TC3);
		rt_sem_release(&remoter_get_semaphore);
	}
}

