#include "pstwo_task.h"

/* relate to thread BEGIN */
ALIGN(RT_ALIGN_SIZE)
static char pstwo_thread_stack[512];
static struct rt_thread pstwo_thread_object;
static void Pstwo_Thread(void *param);

void Pstwo_Thread_Init(void)
{
    rt_thread_init(&pstwo_thread_object,
                   "pstwo",
                   Pstwo_Thread,
                   RT_NULL,
                   &pstwo_thread_stack[0],
                   sizeof(pstwo_thread_stack),
                   3,
                   20);
}

rt_thread_t Get_Pstwo_Thread_Object(void)
{
    return &pstwo_thread_object;
}
/* relate to thread END */

static PS2 remoter;

static uint8_t *uart4_rx_buffer;
static uint8_t *uart4_rxd_len;

static void Pstwo_Thread(void *param)
{
    uart4_rx_buffer = Get_Uart4_Rxd_Buffer();
    uart4_rxd_len = Get_Uart4_Rxd_Length();
    rt_thread_delay(1000);

    USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);
    
    for (;;)
    {
        // for (uint8_t i = 0; i < *uart4_rxd_len; i++)
        // {
        //     printf("%x hello", uart4_rx_buffer[i]);
        // }
        printf("%d\r\n",*uart4_rxd_len);
        rt_thread_delay(30);
    }
}

PS2 *Get_Remoter_Data(void)
{
    return &remoter;
}
