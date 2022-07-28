#include "upload_task.h"

/* relate to thread BEGIN */
ALIGN(RT_ALIGN_SIZE)
static char upload_thread_stack[512];
static struct rt_thread upload_thread_object;
static void Upload_Thread(void *param);

void Upload_Thread_Init(void)
{
    rt_thread_init(&upload_thread_object,
                   "upload",
                   Upload_Thread,
                   RT_NULL,
                   &upload_thread_stack[0],
                   sizeof(upload_thread_stack),
                   3,
                   20);
}

rt_thread_t Get_Upload_Thread_Object(void)
{
    return &upload_thread_object;
}
/* relate to thread END */

static void Upload_Thread(void *param)
{
    rt_thread_delay(1000);
    for (;;)
    {
        static rt_tick_t now_time;
        now_time = rt_tick_get();
        Set_Ros_Send_Data_MSG();
		Usart1_Dma_Send((uint32_t)Get_Ros_Send_Buffer(), Get_Ros_Send_Data_Len());
        rt_thread_delay_until(&now_time, 20); //max:5
    }
}

