#ifndef _MPU_SOFT_IIC_H_
#define _MPU_SOFT_IIC_H_

#include "softi2c.h"
#include "stm32f1xx_ll_gpio.h"
#include "delay_us.h"

void Mpu9250_I2c_Set_Sda_In_Mode(void);
void Mpu9250_I2c_Set_Sda_Out_Mode(void);
void Mpu9250_I2c_Scl(unsigned char state);
void Mpu9250_I2c_Sda(unsigned char state);
unsigned char Mpu9250_I2c_Read_Sda(void );
void Mpu9250_Softi2c_Delay_Us(unsigned int us);

#endif //_MPU_SOFT_IIC_H_
