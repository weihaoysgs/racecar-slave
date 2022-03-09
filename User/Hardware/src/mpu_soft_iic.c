#include "mpu_soft_iic.h"

#define MPU_9250_GPIO_PORT      GPIOB
#define MPU_9250_SDA_PIN        LL_GPIO_PIN_11
#define MPU_9250_SCL_PIN        LL_GPIO_PIN_10



void Mpu9250_I2c_Set_Sda_In_Mode(void)
{
    LL_GPIO_SetPinMode(MPU_9250_GPIO_PORT, MPU_9250_SDA_PIN, LL_GPIO_MODE_INPUT);
}

void Mpu9250_I2c_Set_Sda_Out_Mode(void)
{
    LL_GPIO_SetPinMode(MPU_9250_GPIO_PORT, MPU_9250_SDA_PIN, LL_GPIO_MODE_OUTPUT);
}

void Mpu9250_I2c_Scl(unsigned char state)
{
    if (state == 0)
    {
        LL_GPIO_ResetOutputPin(MPU_9250_GPIO_PORT, MPU_9250_SCL_PIN);
    }
    else if (state == 1)
    {
        LL_GPIO_SetOutputPin(MPU_9250_GPIO_PORT, MPU_9250_SCL_PIN);
    }
}

void Mpu9250_I2c_Sda(unsigned char state)
{
    if (state == 0)
    {
       LL_GPIO_ResetOutputPin(MPU_9250_GPIO_PORT, MPU_9250_SDA_PIN);
    }
    else if (state == 1)
    {
        LL_GPIO_SetOutputPin(MPU_9250_GPIO_PORT, MPU_9250_SDA_PIN);
    }
}

unsigned char Mpu9250_I2c_Read_Sda(void )
{
    uint8_t state;

    if (LL_GPIO_GetPinMode(MPU_9250_GPIO_PORT, MPU_9250_SDA_PIN) == LL_GPIO_MODE_OUTPUT)
    {
        state = LL_GPIO_IsOutputPinSet(MPU_9250_GPIO_PORT, MPU_9250_SDA_PIN);
    }
    else if (LL_GPIO_GetPinMode(MPU_9250_GPIO_PORT, MPU_9250_SDA_PIN) == LL_GPIO_MODE_INPUT)
    {
        state = LL_GPIO_IsInputPinSet(MPU_9250_GPIO_PORT, MPU_9250_SDA_PIN);
    }
    return state;
}

void Mpu9250_Softi2c_Delay_Us(unsigned int us)
{
    delay_us(us);
}