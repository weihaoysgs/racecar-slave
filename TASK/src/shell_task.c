#include "shell_task.h"

/* relate to thread BEGIN */
ALIGN(RT_ALIGN_SIZE)
static char shell_thread_stack[512];
static struct rt_thread shell_thread_object;
static void Shell_Thread(void *param);

void Shell_Thread_Init(void)
{
    rt_thread_init(&shell_thread_object,
                   "shell",
                   Shell_Thread,
                   RT_NULL,
                   &shell_thread_stack[0],
                   sizeof(shell_thread_stack),
                   3,
                   20);
}

rt_thread_t Get_Shell_Thread_Object(void)
{
    return &shell_thread_object;
}
/* relate to thread END */

struct rt_semaphore shell_rx_semaphore;

static void Shell_Thread(void *param)
{
    rt_sem_init(&shell_rx_semaphore,
                "shell_rx",
                0,
                RT_IPC_FLAG_PRIO);
    rt_thread_delay(1000);
    printf("test111\r\n");
    USART_ClearITPendingBit(UART5, USART_IT_RXNE);
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
    (void)UART5->SR;
    (void)UART5->DR;
	// USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);
    for (;;)
    {
        rt_sem_take(&shell_rx_semaphore, RT_WAITING_FOREVER);
        // 将数据拷贝进处理缓冲区，并清除接收缓冲区
        uint32_t usart485_rx_length = *Get_Uart5_Rx_Length();
        uint8_t usart485_rx_buffer[32] = {0}; // 32和usart1中定义的缓冲区同大小
        memcpy(usart485_rx_buffer, Get_Uart5_Rx_Buffer(), usart485_rx_length);
        *Get_Uart5_Rx_Length() = 0; // 清除串口1(485串口)接收计数

        printf("len:%d  -%s-\r\n", usart485_rx_length, usart485_rx_buffer);
    }
}

