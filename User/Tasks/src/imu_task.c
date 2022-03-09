#include "imu_task.h"
#include "softi2c.h"
#include "mpu_soft_iic.h"
#include "pstwo.h"
Softi2c_device_t soft_iic_device;
int PS2_LX, PS2_LY, PS2_RX, PS2_RY, PS2_KEY, lastPS3Key, Accel_Key; // PS2????
void StartImuTask(void const *argument)
{
    soft_iic_device.I2c_Scl = Mpu9250_I2c_Scl;
    soft_iic_device.I2c_Sda = Mpu9250_I2c_Sda;
    soft_iic_device.Read_Sda = Mpu9250_I2c_Read_Sda;
    soft_iic_device.Set_Sda_In_Mode = Mpu9250_I2c_Set_Sda_In_Mode;
    soft_iic_device.Set_Sda_Out_Mode = Mpu9250_I2c_Set_Sda_Out_Mode;
    soft_iic_device.Softi2c_Delay_Us = Mpu9250_Softi2c_Delay_Us;

    Softi2c_Device_Init_Check(&soft_iic_device);
    int operationMode = 1;
    PS2_Init();
    PS2_SetInit();

    //	IIC_Init();
    MPU9250_Init();
    ___printf("hello imu\r\n");
    for (;;)
    {
        PS2_KEY = PS2_DataKey();
        if (PS2_KEY == 0 && lastPS3Key == 4)
        {
            operationMode = !operationMode;
        }
        if (PS2_KEY == 0 && lastPS3Key == 13)
        {
            if (Accel_Key > 1)
                Accel_Key--;
            else if (Accel_Key == 1)
                Accel_Key = 1;
        }
        if (PS2_KEY == 0 && lastPS3Key == 15)
        {
            if (Accel_Key < 5)
                Accel_Key++;
            else if (Accel_Key == 4)
                Accel_Key = 4;
        }
        if (operationMode == 1)
        {                                    // PS2??
            PS2_LX = PS2_AnologData(PSS_LX); // PS2????
            PS2_LY = PS2_AnologData(PSS_LY);
            PS2_RX = PS2_AnologData(PSS_RX);
            PS2_RY = PS2_AnologData(PSS_RY);
            ___printf("%d %d %d %d %d\r\n", PS2_LX, PS2_LY, PS2_RX, PS2_RY,PS2_KEY);
        }
        else if (operationMode == 0)
        { //??????
          // CAN1_SEND();             //CAN??
        }
        lastPS3Key = PS2_KEY;
        osDelay(50);
    }
}
