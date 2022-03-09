#ifndef __SOFTI2C_H_
#define __SOFTI2C_H_

#include "stm32f1xx.h"
#include "stdio.h"

typedef struct
{
	void (* Set_Sda_In_Mode)(void); ///< ����Sda����ģʽ
	void (* Set_Sda_Out_Mode)(void); ///< ����Sda���ģʽ
	void (* I2c_Scl)(unsigned char); ///< ����Scl�����ƽ
	void (* I2c_Sda)(unsigned char); ///< ����Sda�����ƽ
	unsigned char (* Read_Sda)(void); ///< ��Sda�����ƽ
	void (* Softi2c_Delay_Us)(unsigned int); ///< ���뼶��ʱ
	uint32_t delay_time_us;
	uint8_t init_flag;
}Softi2c_device_t;

uint8_t Softi2c_Device_Init_Check(Softi2c_device_t *softi2c_device);
void Softi2c_Device_Deinit(Softi2c_device_t* softi2c_device);

void Softi2c_Start(const Softi2c_device_t* softi2c_device);
void Softi2c_Stop(const Softi2c_device_t* softi2c_device);
unsigned char Softi2c_Wait_Ack(const Softi2c_device_t* softi2c_device);
void Softi2c_Ack(const Softi2c_device_t* softi2c_device);
void Softi2c_NAck(const Softi2c_device_t* softi2c_device);
void Softi2c_Send_Byte(const Softi2c_device_t *softi2c_device, unsigned char tx_data);
unsigned char Softi2c_Read_Byte(const Softi2c_device_t *softi2c_device);

unsigned char Softi2c_Write_Bytes(const Softi2c_device_t *softi2c_device, unsigned char dev_addr, unsigned char *w_data, unsigned char w_len);
unsigned char Softi2c_Write_Then_Read_Bytes(const Softi2c_device_t *softi2c_device, unsigned char dev_addr, unsigned char *w_data, unsigned char w_len, unsigned char *r_data, unsigned char r_len);

#endif // __SOFTI2C_H_
