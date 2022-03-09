#include "softi2c.h"

uint8_t Softi2c_Device_Init_Check(Softi2c_device_t *softi2c_device)
{
	softi2c_device->init_flag = 0;
	if (softi2c_device->I2c_Scl != NULL && softi2c_device->I2c_Sda != NULL && softi2c_device->Read_Sda != NULL && softi2c_device->Softi2c_Delay_Us != NULL)
	{
		softi2c_device->init_flag = 1;
	}
	return softi2c_device->init_flag;
}

void Softi2c_Device_Deinit(Softi2c_device_t* softi2c_device)
{
	softi2c_device->Set_Sda_In_Mode = NULL;
	softi2c_device->Set_Sda_Out_Mode = NULL;
	softi2c_device->I2c_Scl = NULL;
	softi2c_device->I2c_Sda = NULL;
	softi2c_device->Read_Sda = NULL;
	softi2c_device->Softi2c_Delay_Us = NULL;
	softi2c_device->delay_time_us = 0;
	softi2c_device->init_flag = 0;
}

void Softi2c_Start(const Softi2c_device_t* softi2c_device)
{
	softi2c_device->Set_Sda_Out_Mode();
	softi2c_device->I2c_Sda(1);
	softi2c_device->I2c_Scl(1);
	softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us/2);
	softi2c_device->I2c_Sda(0);
	softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us);
	softi2c_device->I2c_Scl(0);
	softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us/2);
}

//����IICֹͣ�ź�
void Softi2c_Stop(const Softi2c_device_t* softi2c_device)
{
	softi2c_device->Set_Sda_Out_Mode();
	softi2c_device->I2c_Sda(0);
	softi2c_device->I2c_Scl(0);
	softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us/2);
	softi2c_device->I2c_Scl(1);
 	softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us);
	softi2c_device->I2c_Sda(1);
	softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us/2);
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
unsigned char Softi2c_Wait_Ack(const Softi2c_device_t* softi2c_device)
{
	unsigned char ucErrTime=0;
	softi2c_device->Set_Sda_In_Mode();
	softi2c_device->I2c_Sda(1);
	softi2c_device->Softi2c_Delay_Us(1);	   
	softi2c_device->I2c_Scl(1);
	softi2c_device->Softi2c_Delay_Us(1);	 
	while(softi2c_device->Read_Sda())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			Softi2c_Stop(softi2c_device);
			return 1;
		}
	}
	softi2c_device->I2c_Scl(0);//ʱ�����0 	   
	return 0;
} 
//����ACKӦ��
void Softi2c_Ack(const Softi2c_device_t* softi2c_device)
{
	softi2c_device->I2c_Scl(0);
	softi2c_device->Set_Sda_Out_Mode();;
	softi2c_device->I2c_Sda(0);
	softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us/2);
	softi2c_device->I2c_Scl(1);
	softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us);
	softi2c_device->I2c_Scl(0);
	softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us/2);
}
//������ACKӦ��
void Softi2c_NAck(const Softi2c_device_t* softi2c_device)
{
	softi2c_device->I2c_Scl(0);
	softi2c_device->Set_Sda_Out_Mode();
	softi2c_device->I2c_Sda(1);
	softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us/2);
	softi2c_device->I2c_Scl(1);
	softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us);
	softi2c_device->I2c_Scl(0);
	softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us/2);
}					 				     

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void Softi2c_Send_Byte(const Softi2c_device_t *softi2c_device, unsigned char tx_data)
{
	unsigned char _t;
	softi2c_device->Set_Sda_Out_Mode();
	softi2c_device->I2c_Scl(0);
	for (_t = 0; _t < 8; _t++)
	{
		softi2c_device->I2c_Sda(((tx_data & 0x80) >> 7));
		tx_data <<= 1;
		softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us/2);
		softi2c_device->I2c_Scl(1);
		softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us);
		softi2c_device->I2c_Scl(0);
		softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us/2);
	}
}

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
unsigned char Softi2c_Read_Byte(const Softi2c_device_t *softi2c_device)
{
	unsigned char _t = 0;
	unsigned char receive = 0;
	softi2c_device->Set_Sda_In_Mode();
	softi2c_device->I2c_Scl(0);
	for (_t = 0; _t < 8; _t++)
	{
		softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us/2);
		softi2c_device->I2c_Scl(1);
		softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us);
		receive <<= 1;
		if (softi2c_device->Read_Sda())
		{
			receive++;
		}
		softi2c_device->I2c_Scl(0);
		softi2c_device->Softi2c_Delay_Us(softi2c_device->delay_time_us/2);
	}
	return receive;
}

unsigned char Softi2c_Write_Bytes(const Softi2c_device_t *softi2c_device, unsigned char dev_addr, unsigned char *w_data, unsigned char w_len)
{
	unsigned char w_cnt = 0;
	Softi2c_Start(softi2c_device);
	Softi2c_Send_Byte(softi2c_device, dev_addr << 1); //����д����
	Softi2c_Wait_Ack(softi2c_device);

	for (w_cnt = 0; w_cnt < w_len; w_cnt++)
	{
		Softi2c_Send_Byte(softi2c_device, w_data[w_cnt]); //���͵�ַ
		Softi2c_Wait_Ack(softi2c_device);
	}
	Softi2c_Stop(softi2c_device); //����һ��ֹͣ����
	return 1;
}

unsigned char Softi2c_Write_Then_Read_Bytes(const Softi2c_device_t *softi2c_device, unsigned char dev_addr, unsigned char *w_data, unsigned char w_len, unsigned char *r_data, unsigned char r_len)
{
	unsigned char w_cnt = 0;
	unsigned char r_cnt = 0;
	Softi2c_Start(softi2c_device);
	Softi2c_Send_Byte(softi2c_device, dev_addr << 1); //����д����
	Softi2c_Wait_Ack(softi2c_device);

	for (w_cnt = 0; w_cnt < w_len; w_cnt++)
	{
		Softi2c_Send_Byte(softi2c_device, w_data[w_cnt]); //���͵�ַ
		Softi2c_Wait_Ack(softi2c_device);
	}

	Softi2c_Start(softi2c_device);
	Softi2c_Send_Byte(softi2c_device, (dev_addr << 1) + 1); //�������ģʽ
	Softi2c_Wait_Ack(softi2c_device);

	for (r_cnt = 0; r_cnt < r_len; r_cnt++)
	{
		r_data[r_cnt] = Softi2c_Read_Byte(softi2c_device);
		if (r_cnt != r_len - 1)
			Softi2c_Ack(softi2c_device); //��ACK�Ķ�����
		else
			Softi2c_NAck(softi2c_device); //���һ���ֽ�NACK
	}
	Softi2c_Stop(softi2c_device); //����һ��ֹͣ����
	return r_cnt;
}
