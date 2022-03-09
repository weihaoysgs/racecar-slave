#include "imu_task.h"
#include "softi2c.h"
#include "mpu_soft_iic.h"

Softi2c_device_t soft_iic_device;

void StartImuTask(void const *argument)
{
    soft_iic_device.I2c_Scl = Mpu9250_I2c_Scl;
    soft_iic_device.I2c_Sda = Mpu9250_I2c_Sda;
    soft_iic_device.Read_Sda = Mpu9250_I2c_Read_Sda;
    soft_iic_device.Set_Sda_In_Mode = Mpu9250_I2c_Set_Sda_In_Mode;
    soft_iic_device.Set_Sda_Out_Mode = Mpu9250_I2c_Set_Sda_Out_Mode;
    soft_iic_device.Softi2c_Delay_Us = Mpu9250_Softi2c_Delay_Us;

    Softi2c_Device_Init_Check(&soft_iic_device);
	
//	IIC_Init();
   MPU9250_Init();
    ___printf("hello imu\r\n");
    for (;;)
    {
        osDelay(500);
    }
}
