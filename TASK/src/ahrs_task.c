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

static void Ahrs_Thread(void *param)
{
	uint8_t ahrs_data[20];
	static const uint8_t HEAD = 0xFF;
	static const uint8_t TAIL = 0xDD;
	for (;;)
	{
		READ_MPU9250_ACCEL();
		READ_MPU9250_GYRO();
		READ_MPU9250_MAG();

		ahrs_data[0] = HEAD;
		ahrs_data[1] = raw_data.ax >> 8;
		ahrs_data[2] = raw_data.ax;
		ahrs_data[3] = raw_data.ay >> 8;
		ahrs_data[4] = raw_data.ay;
		ahrs_data[5] = raw_data.az >> 8;
		ahrs_data[6] = raw_data.az;
		ahrs_data[7] = raw_data.gx >> 8;
		ahrs_data[8] = raw_data.gx;
		ahrs_data[9] = raw_data.gy >> 8;
		ahrs_data[10] = raw_data.gy;
		ahrs_data[11] = raw_data.gz >> 8;
		ahrs_data[12] = raw_data.gz;
		ahrs_data[13] = raw_data.mx >> 8;
		ahrs_data[14] = raw_data.mx;
		ahrs_data[15] = raw_data.my >> 8;
		ahrs_data[16] = raw_data.my;
		ahrs_data[17] = raw_data.mz >> 8;
		ahrs_data[18] = raw_data.mz;
		ahrs_data[19] = TAIL;

		// printf("ax: = %d  ay:%d az:%d\t",raw_data.ax,raw_data.ay,raw_data.az);
		// printf("gx: = %d  gy:%d  gz:%d\t",raw_data.gx,raw_data.gy,raw_data.gz);
		// printf("mx:%d  my:%d  mz:%d\r\n",raw_data.mx,raw_data.my,raw_data.mz);
		// Usart4_Dma_Send((uint32_t)ahrs_data, 20);
		rt_thread_delay(50);
	}
}
