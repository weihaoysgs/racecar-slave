#include "imu_task.h"

void StartImuTask(void const *argument)
{
    Init_MPU9250();
    for (;;)
    {
        READ_MPU9250_ACCEL();
        READ_MPU9250_GYRO();
        READ_MPU9250_MAG();

        // ___printf("\r\nax:%d  ay:%d  az:%d\r\n", raw_data.ax, raw_data.ay, raw_data.az);
        // ___printf("\r\ngx:%d  gy:%d  gz:%d\r\n", raw_data.gx, raw_data.gy, raw_data.gz);
        // ___printf("\r\nmx:%d  my:%d  mz:%d\r\n", raw_data.mx, raw_data.my, raw_data.mz);

        osDelay(50);
    }
}
