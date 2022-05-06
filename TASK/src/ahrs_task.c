#include "ahrs_task.h"

/* relate to thread BEGIN */
ALIGN(RT_ALIGN_SIZE)
static char ahrs_thread_stack[512];
static struct rt_thread ahrs_thread_object;
static void Ahrs_Thread(void *param);

void Ahrs_Thread_Init(void)
{
	rt_thread_init(&ahrs_thread_object,
				   "ahrs",
				   Ahrs_Thread,
				   RT_NULL,
				   &ahrs_thread_stack[0],
				   sizeof(ahrs_thread_stack),
				   2,
				   20);
}

rt_thread_t Get_Ahrs_Thread_Object(void)
{
	return &ahrs_thread_object;
}
/* relate to thread END */

Mpu_Data_t *raw_data_pt;

static void Ahrs_Thread(void *param)
{
	raw_data_pt = Get_Mpu_Data_pt();
	for (;;)
	{
		READ_MPU9250_ACCEL();
		READ_MPU9250_GYRO();
		READ_MPU9250_MAG();

		// printf("ax: %d ay:%d  az:%d\t",raw_data_pt->ax  ,raw_data_pt->ay,raw_data_pt->az);
		// printf("gx: = %d  gy:%d  gz:%d\t  ",raw_data_pt->gx,raw_data_pt->gy,raw_data_pt->gz);
		// printf("mx:%d  my:%d  mz:%d\r\n",raw_data_pt->mx,raw_data_pt->my,raw_data_pt->mz);
		
		rt_thread_delay(20);
	}
}
