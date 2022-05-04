#include "shell_task.h"

/* relate to thread BEGIN */
ALIGN(RT_ALIGN_SIZE)
static char shell_thread_stack[1024];
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
                   6,
                   20);
}

rt_thread_t Get_Shell_Thread_Object(void)
{
    return &shell_thread_object;
}
/* relate to thread END */

struct rt_semaphore shell_rx_semaphore;
static void Shell_Str_Process(char* rx_buffer, uint32_t rx_length);

static void Shell_Thread(void *param)
{
    rt_sem_init(&shell_rx_semaphore,
                "shell_rx",
                0,
                RT_IPC_FLAG_PRIO);
    Shell_Init();
    rt_thread_delay(1000);
    printf("RaceCar> Running.\r\n");
    USART_ClearITPendingBit(UART5, USART_IT_RXNE);
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
    (void)UART5->SR;
    (void)UART5->DR;
	USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);
    for (;;)
    {
        rt_sem_take(&shell_rx_semaphore, RT_WAITING_FOREVER);

        // 将数据拷贝进处理缓冲区，并清除接收缓冲区
        uint32_t shell_rx_length = *Get_Uart5_Rx_Length();
        char shell_rx_buffer[32] = {0}; // 32和usart1中定义的缓冲区同大小
        memcpy(shell_rx_buffer, Get_Uart5_Rx_Buffer(), shell_rx_length);
        *Get_Uart5_Rx_Length() = 0; // 清除串口1(485串口)接收计数

        Shell_Str_Process(shell_rx_buffer, shell_rx_length);
        Shell_Command_Parse(shell_rx_buffer);

        // 测试数据正常
        // printf("len:%d  -%s-\r\n", shell_rx_length, shell_rx_buffer);
    }
}

static void Shell_Str_Process(char* rx_buffer, uint32_t rx_length)
{
	if(rx_buffer[rx_length-2]=='\r' && rx_buffer[rx_length-1]=='\n')
	{
		rx_buffer[rx_length-2] = '\0';
	}
	else
	{
		rx_buffer[rx_length] = '\0';
	}
}
