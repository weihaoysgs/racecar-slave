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
PS2 *remoter;
static void Chassis_Thread(void *param)
{
	remoter = Get_Remoter_Data();
	rt_thread_delay(1000);
	TIM2->ARR = 3300 ;
	for (;;)
	{

		printf("tim2_cnt:%d ;tim3_cnt:%d;%d %d\r\n",TIM2->CNT,TIM3->CNT ,GetMotorLeftSpeed(), GetMotorRightSpeed());
		LED_TOGGLE();
		rt_thread_delay(51);
		// TIM_SetCompare1(TIM8, 5000);
		// TIM_SetCompare2(TIM8, 0);

		// SetMotorLeftPower((int32_t)(remoter->ch1) * 40);
		// SetMotorRightPower((int32_t)(remoter->ch1) * 40);
		// printf("%d %d \r\n", (remoter->ch1) * 30, (remoter->ch1) * 30);
	}
}
