#include "stm32f10x.h"
#include "init_task.h"
#include "chassis_task.h"
#include "pstwo_task.h"
#include "motor_speed_timer.h"
#include "ahrs_task.h"
#include "upload_task.h"
#include "shell_task.h"

int main(void)
{

	// 外设初始化
	Peripheral_Init();

	// BEGIN 初始化并启动任务 BEGIN
	Motor_Speed_Clc_Timer_Init();
	Pstwo_Thread_Init();
	Chassis_Thread_Init();
	Ahrs_Thread_Init();
	Upload_Thread_Init();
	Shell_Thread_Init();

	rt_timer_start(Get_Motor_Speed_Clc_Timer_Object());
	rt_thread_startup(Get_Chassis_Thread_Object());
	rt_thread_startup(Get_Pstwo_Thread_Object());
	rt_thread_startup(Get_Ahrs_Thread_Object());
	rt_thread_startup(Get_Upload_Thread_Object());
	rt_thread_startup(Get_Shell_Thread_Object());
	// END 初始化并启动任务 END

	for (;;)
	{
		rt_thread_delay(500);
	}
}
