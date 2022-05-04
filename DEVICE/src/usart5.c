#include "usart5.h"
#include "shell_task.h"

static const uint32_t uart5_rx_max_len = 32;
static volatile uint32_t uart5_rx_length = 0;
static volatile uint8_t uart5_rx_buffer[uart5_rx_max_len] = {0};

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
	while ((UART5->SR & 0X40) == 0)
		;
	UART5->DR = (u8)ch;
	return ch;
}

#endif
// PC12 -> Tx
// PD2  -> Rx
void Uart5_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //! 注意使能 AFIO 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //! 使能 UART4 发送和接收端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); //! 使能 UART4 发送和接收端口时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);
	USART_ITConfig(UART5, USART_IT_IDLE, DISABLE); //! 串口5空闲中断（一帧数据接收完毕）
	USART_Cmd(UART5, ENABLE);
}

void UART5_IRQHandler(void)
{
	if(USART_GetITStatus(UART5, USART_IT_RXNE))
	{
		uart5_rx_buffer[uart5_rx_length] = UART5->DR;
		uart5_rx_length ++;
		USART_GetITStatus(UART5, USART_IT_RXNE);
		if(uart5_rx_length>31)
		{
			rt_sem_release(&shell_rx_semaphore); // 唤醒任务
			// uart5_rx_length = 0;
		}
	}
	if(USART_GetITStatus(UART5, USART_IT_IDLE))
	{
		(void)UART5->SR;
        (void)UART5->DR;
		rt_sem_release(&shell_rx_semaphore); // 唤醒任务
		// uart5_rx_length = 0;
	}
}

/**
 * @brief 获取当前接收长度
 * 
 * @return uint32_t 
 */
uint32_t* Get_Uart5_Rx_Length(void)
{
	return (uint32_t *)&uart5_rx_length;
}

/**
 * @brief 获取串口5DMA接受Buffer
 * 
 * @return uint8_t* 数组头指针
 */
uint8_t *Get_Uart5_Rx_Buffer(void)
{
	return (uint8_t *)uart5_rx_buffer;
}

/**
 * @brief 串口5接收DMA最大长度
 * 
 * @return const uint16_t 
 */
uint32_t Get_Uart5_Rx_Max_Len(void)
{
	return uart5_rx_max_len;
}
