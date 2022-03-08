#include "imu_task.h"

void StartImuTask(void const *argument)
{
    MPU9250_Init();
    ___printf("hello imu\r\n");
    for (;;)
    {
        osDelay(500);
    }
}
