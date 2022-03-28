#include "chassis_task.h"
#include "motor_speed_timer.h"
#include "usart4.h"

/* relate to thread BEGIN */
ALIGN(RT_ALIGN_SIZE)
static char chassis_thread_stack[512];
static struct rt_thread chassis_thread_object;
static void Chassis_Thread(void *param);

void Chassis_Thread_Init(void)
{
	rt_thread_init(&chassis_thread_object,
				   "chassis",
				   Chassis_Thread,
				   RT_NULL,
				   &chassis_thread_stack[0],
				   sizeof(chassis_thread_stack),
				   2,
				   20);
}

rt_thread_t Get_Chassis_Thread_Object(void)
{
	return &chassis_thread_object;
}
/* relate to thread END */

static void Chassis_Thread(void *param)
{
	rt_thread_delay(1000);
	for (;;)
	{
		printf("%d %d\r\n", GetMotorLeftSpeed(), GetMotorRightSpeed());
		LED_TOGGLE();
		rt_thread_delay(51);
		TIM_SetCompare1(TIM8, 5000);
		TIM_SetCompare2(TIM8, 0);
	}
}
