#include "stm32f10x.h"
#include "init_task.h"
#include "chassis_task.h"
#include "pstwo_task.h"
#include "motor_speed_timer.h"
#include "ahrs_task.h"
#include "rosmsg_task.h"
#include "upload_task.h"
#include "oled_task.h"
#include "shell_task.h"

int main(void)
{
	// ����MCU��ʼ��
	{
		// �ж���
		rt_base_t level = rt_hw_interrupt_disable();

		// �����ʼ��
		Peripheral_Init();

		// BEGIN ��ʼ������������ BEGIN
		Motor_Speed_Clc_Timer_Init();
		Pstwo_Thread_Init();
		Chassis_Thread_Init();
		Ahrs_Thread_Init();
		Rosmsg_Thread_Init();
		Upload_Thread_Init();
		Oled_Thread_Init();
		Shell_Thread_Init();

		rt_timer_start(Get_Motor_Speed_Clc_Timer_Object());
		rt_thread_startup(Get_Chassis_Thread_Object());
		rt_thread_startup(Get_Pstwo_Thread_Object());
		rt_thread_startup(Get_Ahrs_Thread_Object());
		rt_thread_startup(Get_Rosmsg_Thread_Object());
		rt_thread_startup(Get_Upload_Thread_Object());
		rt_thread_startup(Get_Oled_Thread_Object());
		rt_thread_startup(Get_Shell_Thread_Object());
		// END ��ʼ������������ END

		// ����ж���
		rt_hw_interrupt_enable(level);

		// ���� MAIN_THREAD �����ȼ�
		uint8_t main_priority = 7u;
		rt_thread_control(rt_thread_self(), RT_THREAD_CTRL_CHANGE_PRIORITY, &main_priority);
	}

	// ��ʱ����
	for (;;)
	{
		LED_TOGGLE();
		rt_thread_delay(800);
	}
}
