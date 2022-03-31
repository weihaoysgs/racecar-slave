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
static void Pstwo_Thread(void *param)
{
    rt_thread_delay(1000);
    PS2_Init();

    for (;;)
    {
        Update_Ps2_Data();
        Get_Rc_Data(&remoter);
        rt_thread_delay(50);
        // printf("ch0:%d ch1:%d ch2:%d ch3:%d key:%d \r\n",
        //        remoter.ch0, remoter.ch1, remoter.ch2, remoter.ch3, remoter.key);
    }
}

PS2 *Get_Remoter_Data(void)
{
    return &remoter;
}
