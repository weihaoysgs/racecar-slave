#include "oled_task.h"
#include "oled.h"

/* relate to thread BEGIN */
ALIGN(RT_ALIGN_SIZE)
static char oled_thread_stack[1024];
static struct rt_thread oled_thread_object;
static void Oled_Thread(void *param);

void Oled_Thread_Init(void)
{
    rt_thread_init(&oled_thread_object,
                   "oled",
                   Oled_Thread,
                   RT_NULL,
                   &oled_thread_stack[0],
                   sizeof(oled_thread_stack),
                   6,
                   20);
}

rt_thread_t Get_Oled_Thread_Object(void)
{
    return &oled_thread_object;
}
/* relate to thread END */

static void Oled_Thread(void *param)
{
    OLED_Init();
    rt_thread_delay(1000);
    for (;;)
    {
        OLED_ShowString(00, 0, "A");
		OLED_DrawPoint(30,30,0);
		OLED_DrawPoint(32,32,1);
        OLED_DrawPoint(0,0,1);
		OLED_Refresh_Gram();
        rt_thread_delay(100); //max:5
    }
}

