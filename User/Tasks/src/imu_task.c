#include "imu_task.h"
#include "softi2c.h"
#include "mpu_soft_iic.h"
#include "pstwo.h"

void StartImuTask(void const *argument)
{
    
    PS2_Init();
    PS2_SetInit();

    MPU9250_Init();
    ___printf("hello imu\r\n");
    for (;;)
    {

        osDelay(50);
    }
}
