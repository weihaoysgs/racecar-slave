#include "oled_task.h"
#include "oled.h"
#include "pstwo_task.h"
#include "rosmsg_task.h"
#include "adc.h"

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
        static float battery_volt = 0.0f;
        battery_volt = battery_volt*0.9 + Get_battery_volt()*0.1;
        int battery_volt_out = battery_volt;

		OLED_ShowString(00, 00, "Power:");
	    OLED_ShowNumber(60, 00, battery_volt_out/100, 2, 12); //整数位
        OLED_ShowNumber(80, 00, (battery_volt_out%100)/10, 1, 12); //小数点后第1位
        OLED_ShowNumber(86, 00, battery_volt_out%10, 1, 12); //小数点后第2位

        OLED_ShowString(00, 20, "Time:");
	    OLED_ShowNumber(50, 20, (rt_tick_get()/1000)/60, 2, 12);
        OLED_ShowNumber(80, 20, (rt_tick_get()/1000)%60, 2, 12);

        OLED_ShowString(00, 40, "Rc:");
	    OLED_ShowNumber(25, 40, Rc_Valid_Status(), 1, 12);

        OLED_ShowString(60, 40, "Ros:");
	    OLED_ShowNumber(95, 40, Get_Ros_Message_Status(), 1, 12);

		OLED_Refresh_Gram();
        rt_thread_delay(200);
    }
}

