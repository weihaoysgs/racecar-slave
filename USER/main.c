#include "stm32f10x.h"
#include "init_task.h"
#include "chassis_task.h"
#include "pstwo_task.h"

int main(void)
{

	// 外设初始化
	Peripheral_Init();

	// BEGIN 初始化并启动任务 BEGIN
	Pstwo_Thread_Init();
	Chassis_Thread_Init();

	rt_thread_startup(Get_Chassis_Thread_Object());
	rt_thread_startup(Get_Pstwo_Thread_Object());
	// END 初始化并启动任务 END

	for (;;)
	{
		rt_thread_delay(500);
	}
}
