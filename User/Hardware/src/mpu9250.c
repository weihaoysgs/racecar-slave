#include "mpu9250.h"
#include "softi2c.h"

uint8_t MPU9250_Init(Softi2c_device_t *soft_iic_device)
{
	// uint8_t res = 0;
	// IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_PWR_MGMT1_REG, 0x80); //��λMPU9250
	// osDelay(100);
	// IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_PWR_MGMT2_REG, 0x00); //����MPU9250
	// MPU9250_SetGyroFsr(MPU9250_GYRO_FSR);
	// MPU9250_SetAccelFsr(MPU9250_ACCEL_FSR);
	// MPU9250_SetRate(MPU9250_RATE);
	// IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_INT_EN_REG, 0x00); //�ر������ж�

	// IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_USER_CTRL_REG, 0x00); // I2C��ģʽ�ر�
	// IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_FIFO_EN_REG, 0x00);	 //�ر�FIFO
	// IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_INTBP_CFG_REG, 0x82); // INT���ŵ͵�ƽ��Ч������bypassģʽ������ֱ//��ȡMPU6500��ID�Ӷ�ȡ������

	// res = IIC_ReadAddrRegByte(MPU6500_ADDR, MPU9250_DEVICE_ID_REG);

	// if (res == MPU6500_ID)
	// {
	// 	IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_PWR_MGMT1_REG, 0x01); //����CLKSEL,PLL X��Ϊ�ο�
	// 	IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_PWR_MGMT2_REG, 0x00); //���ٶ��������Ƕ�����
	// 	MPU9250_SetRate(MPU9250_RATE);
	// }
	// else
	// {
	// 	___printf("MPU9250 Init failed 1, res = 0x%X\r\n", res);
	// 	return 1;
	// }

	// res = IIC_ReadAddrRegByte(AK8963_ADDR, MPU9250_MAG_WIA); //��ȡAK8963 ID
	// if (res == AK8963_ID)
	// {
	// 	IIC_WriteAddrRegByte(AK8963_ADDR, MPU9250_MAG_CNTL1, 0x11); //����AK8963Ϊ��������ģʽ
	// 	___printf("MPU9250 Init success.\r\n");
	// }
	// else
	// {
	// 	___printf("MPU9250 Init failed 2, res = 0x%X\r\n", res);
	// 	return 1;
	// }
	// return 0;
}

///* ����MPU9250�����Ǵ����������̷�Χ					
// * fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps	
// * ����ֵ:0,���óɹ�									
// *    ����,����ʧ�� 									
// */
//uint8_t MPU9250_SetGyroFsr(uint8_t fsr)
//{
//	return IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_GYRO_CFG_REG, fsr<<3);	//���������������̷�Χ  
//}



///* ����MPU92506050���ٶȴ����������̷�Χ	
// * fsr:0,��2g;1,��4g;2,��8g;3,��16g		
// * ����ֵ:0,���óɹ�						
// *    ����,����ʧ�� 						
// */

//uint8_t MPU9250_SetAccelFsr(uint8_t fsr)
//{
//	return IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_ACCEL_CFG_REG, fsr<<3);	//���ü��ٶȴ����������̷�Χ  
//}

///* ����MPU92506050�����ֵ�ͨ�˲���		
// * lpf:���ֵ�ͨ�˲�Ƶ��(Hz)				
// * ����ֵ:0,���óɹ�						
// *    ����,����ʧ�� 						
// */

//uint8_t MPU9250_SetLPF(uint16_t lpf)
//{
//	uint8_t data=0;
//	if(lpf>=188)
//		data=1;
//	else if(lpf>=98)
//		data=2;
//	else if(lpf>=42)
//		data=3;
//	else if(lpf>=20)
//		data=4;
//	else if(lpf>=10)
//		data=5;
//	else 
//		data=6; 
//	return IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_CFG_REG, data);			//�������ֵ�ͨ�˲���  
//}

///* ����MPU9250�Ĳ�����(�ٶ�Fs=1KHz)		
// * rate:4~1000(Hz)						
// * ����ֵ:0,���óɹ�						
// *    ����,����ʧ�� 						
// */

//uint8_t MPU9250_SetRate(uint16_t rate)
//{
//	uint8_t data;
//	if(rate>1000)
//		rate=1000;
//	if(rate<4)
//		rate=4;
//	data = 1000 / rate - 1;
//	data = IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_SAMPLE_RATE_REG, data);	//�������ֵ�ͨ�˲���
// 	return MPU9250_SetLPF(rate/2);											//�Զ�����LPFΪ�����ʵ�һ��
//}
