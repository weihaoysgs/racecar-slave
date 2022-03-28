#include "stm32f10x.h"
#include "init_task.h"
#include "chassis_task.h"

int main(void) {

	// �����ʼ��
	Peripheral_Init();

	// BEGIN ��ʼ������������ BEGIN

	Chassis_Thread_Init();
	rt_thread_startup(Get_Chassis_Thread_Object());

	// END ��ʼ������������ END

	for(;;) {
		rt_thread_delay(500);
	}
}
