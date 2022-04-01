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
	for (;;)
	{
		rt_thread_delay(30);
		LED_TOGGLE();
		// SetMotorLeftPower(3000);
		// SetMotorRightPower(5000);
		printf("tim2 %d,tim3 %d ;speed_left:%d; speed_right:%d \r\n", TIM2->CNT, TIM3->CNT,GetMotorLeftSpeed(),GetMotorRightSpeed());
		// Set_Chassis_Motor_Speed(remoter->ch0 * 30, remoter->ch0 * 30);
	}
}

static Pid_Position_t motor_left_speed_pid = NEW_POSITION_PID(11, 0, 4.5, 2000, 7000, 0, 1000, 500);
static Pid_Position_t motor_right_speed_pid = NEW_POSITION_PID(11, 0, 4.5, 2000, 7000, 0, 1000, 500);

void Set_Chassis_Motor_Speed(float left_motor_speed, float right_motor_speed)
{
	int16_t pid_left = Pid_Position_Calc(&motor_left_speed_pid, left_motor_speed, (float)GetMotorLeftSpeed());
	int16_t pid_right = Pid_Position_Calc(&motor_right_speed_pid, right_motor_speed, (float)GetMotorRightSpeed());
	printf("%d %d\r\n", pid_left, pid_right);
	SetMotorLeftPower(pid_left);
	SetMotorRightPower(pid_right);
}
