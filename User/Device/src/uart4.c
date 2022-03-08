#include "uart4.h"

static volatile uint8_t uart4_tx_lock = 0;
static volatile char uart4_dma_transmit_buf[256];
static const uint16_t uart4_dma_rx_max_len = 50;
volatile uint8_t uart4_dma_rx_buffer[uart4_dma_rx_max_len];
static volatile uint16_t uart4_rxd_data_len;
extern osMutexId printfMutexHandle;

void Uart4_Tx_Init(void)
{
    LL_DMA_SetPeriphAddress(DMA2, LL_DMA_CHANNEL_5, LL_USART_DMA_GetRegAddr(UART4)); // Set the Peripheral address.
    LL_USART_EnableDMAReq_TX(UART4);                                                 // Enable DMA Mode for transmission
}

void Uart4_Transmit_Dma(uint32_t data_address, uint32_t len)
{
    LL_DMA_SetMemoryAddress(DMA2, LL_DMA_CHANNEL_5, data_address);
    LL_DMA_SetDataLength(DMA2, LL_DMA_CHANNEL_5, len);
    LL_USART_ClearFlag_TC(UART4);
    LL_DMA_ClearFlag_TC5(DMA2);
    LL_DMA_EnableChannel(DMA2, LL_DMA_CHANNEL_5);
}

void Uart4_It_Tc_Callback(void)
{
    // usart3_tx_lock = 0;
    LL_USART_DisableIT_TC(UART4); // Disable Transmission Complete Interrupt
    LL_DMA_DisableChannel(DMA2, LL_DMA_CHANNEL_5);
}

void Uart4_Rx_Init(void)
{
    LL_DMA_SetPeriphAddress(DMA2, LL_DMA_CHANNEL_5, LL_USART_DMA_GetRegAddr(UART4));
    LL_DMA_SetMemoryAddress(DMA2, LL_DMA_CHANNEL_5, (uint32_t)(uart4_dma_rx_buffer));

    LL_DMA_SetDataLength(DMA2, LL_DMA_CHANNEL_5, uart4_dma_rx_max_len);

    LL_USART_ClearFlag_IDLE(UART4);
    LL_USART_EnableIT_IDLE(UART4);

    LL_USART_EnableDMAReq_RX(UART4);

    LL_DMA_EnableChannel(DMA2, LL_DMA_CHANNEL_5);
}

uint8_t *Get_Uart4_DMA_RxBuffer(void)
{
    return (uint8_t *)uart4_dma_rx_buffer;
}

const uint16_t *Get_Uart4_DMA_Rx_MaxLen(void)
{
    return &uart4_dma_rx_max_len;
}

void Uart4_DMA_RxCp_Callback(void)
{
    ///< 关闭 DMA
    LL_DMA_DisableChannel(DMA2, LL_DMA_CHANNEL_5);

    ///< 清楚 IDLE 中断、DMA TC2 标志位
    LL_USART_ClearFlag_IDLE(UART4);
    LL_DMA_ClearFlag_TC2(DMA2);

    ///< 获取该帧的数据长度
    uart4_rxd_data_len = uart4_dma_rx_max_len - LL_DMA_GetDataLength(DMA2, LL_DMA_CHANNEL_5);

    ///< 重新设置数据传输长度
    LL_DMA_SetDataLength(DMA2, LL_DMA_CHANNEL_5, uart4_dma_rx_max_len);

    ///< 通知任务进行解析
    Info_Chassis_Task_Get_Control_Data();

    ///< 重新开启 DMA
    LL_DMA_EnableChannel(DMA2, LL_DMA_CHANNEL_5);
}

static uint8_t first_release_mutex_flag = 0;

int ___printf(const char *format, ...)
{
	
	if (first_release_mutex_flag == 0)
	{
		first_release_mutex_flag = 1;
		osMutexRelease(printfMutexHandle);
	}
    // 第一次时一定可以得到互斥锁的
	osMutexWait(printfMutexHandle, osWaitForever);

	uint32_t len;
	va_list args;
	va_start(args, format);
	len = vsnprintf((char *)uart4_dma_transmit_buf, sizeof(uart4_dma_transmit_buf), (const char *)format, args);
	va_end(args);
	Uart4_Transmit_Dma((uint32_t)uart4_dma_transmit_buf, len);
	LED_GREEN_OFF();
	while (!((UART4->SR) & USART_SR_TC))
	{
		LED_GREEN_ON();
	}
	///< 关闭 DMA
	LL_DMA_DisableChannel(DMA2, LL_DMA_CHANNEL_5);
	LED_GREEN_OFF();
	///< 这里释放锁
	osMutexRelease(printfMutexHandle);

	return -1;
}
