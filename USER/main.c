#include "stm32f10x.h"
#include "init_task.h"
#include "chassis_task.h"
#include "pstwo_task.h"
#include "motor_speed_timer.h"
#include "ahrs_task.h"
#include "shell_task.h"

int main(void)
{

	// �����ʼ��
	Peripheral_Init();

	// BEGIN ��ʼ������������ BEGIN
	Motor_Speed_Clc_Timer_Init();
	Pstwo_Thread_Init();
	Chassis_Thread_Init();
	Ahrs_Thread_Init();
	Shell_Thread_Init();

	rt_timer_start(Get_Motor_Speed_Clc_Timer_Object());
	rt_thread_startup(Get_Chassis_Thread_Object());
	rt_thread_startup(Get_Pstwo_Thread_Object());
	rt_thread_startup(Get_Ahrs_Thread_Object());
	rt_thread_startup(Get_Shell_Thread_Object());
	// END ��ʼ������������ END

	for (;;)
	{
		rt_thread_delay(500);
	}
}
