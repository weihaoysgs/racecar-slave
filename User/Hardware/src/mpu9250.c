#include "mpu9250.h"
#include "softi2c.h"

uint8_t MPU9250_Init(Softi2c_device_t *soft_iic_device)
{
	// uint8_t res = 0;
	// IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_PWR_MGMT1_REG, 0x80); //复位MPU9250
	// osDelay(100);
	// IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_PWR_MGMT2_REG, 0x00); //唤醒MPU9250
	// MPU9250_SetGyroFsr(MPU9250_GYRO_FSR);
	// MPU9250_SetAccelFsr(MPU9250_ACCEL_FSR);
	// MPU9250_SetRate(MPU9250_RATE);
	// IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_INT_EN_REG, 0x00); //关闭所有中断

	// IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_USER_CTRL_REG, 0x00); // I2C主模式关闭
	// IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_FIFO_EN_REG, 0x00);	 //关闭FIFO
	// IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_INTBP_CFG_REG, 0x82); // INT引脚低电平有效，开启bypass模式，可以直//读取MPU6500的ID接读取磁力计

	// res = IIC_ReadAddrRegByte(MPU6500_ADDR, MPU9250_DEVICE_ID_REG);

	// if (res == MPU6500_ID)
	// {
	// 	IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_PWR_MGMT1_REG, 0x01); //设置CLKSEL,PLL X轴为参考
	// 	IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_PWR_MGMT2_REG, 0x00); //加速度与陀螺仪都工作
	// 	MPU9250_SetRate(MPU9250_RATE);
	// }
	// else
	// {
	// 	___printf("MPU9250 Init failed 1, res = 0x%X\r\n", res);
	// 	return 1;
	// }

	// res = IIC_ReadAddrRegByte(AK8963_ADDR, MPU9250_MAG_WIA); //读取AK8963 ID
	// if (res == AK8963_ID)
	// {
	// 	IIC_WriteAddrRegByte(AK8963_ADDR, MPU9250_MAG_CNTL1, 0x11); //设置AK8963为连续测量模式
	// 	___printf("MPU9250 Init success.\r\n");
	// }
	// else
	// {
	// 	___printf("MPU9250 Init failed 2, res = 0x%X\r\n", res);
	// 	return 1;
	// }
	// return 0;
}

///* 设置MPU9250陀螺仪传感器满量程范围					
// * fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps	
// * 返回值:0,设置成功									
// *    其他,设置失败 									
// */
//uint8_t MPU9250_SetGyroFsr(uint8_t fsr)
//{
//	return IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_GYRO_CFG_REG, fsr<<3);	//设置陀螺仪满量程范围  
//}



///* 设置MPU92506050加速度传感器满量程范围	
// * fsr:0,±2g;1,±4g;2,±8g;3,±16g		
// * 返回值:0,设置成功						
// *    其他,设置失败 						
// */

//uint8_t MPU9250_SetAccelFsr(uint8_t fsr)
//{
//	return IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_ACCEL_CFG_REG, fsr<<3);	//设置加速度传感器满量程范围  
//}

///* 设置MPU92506050的数字低通滤波器		
// * lpf:数字低通滤波频率(Hz)				
// * 返回值:0,设置成功						
// *    其他,设置失败 						
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
//	return IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_CFG_REG, data);			//设置数字低通滤波器  
//}

///* 设置MPU9250的采样率(假定Fs=1KHz)		
// * rate:4~1000(Hz)						
// * 返回值:0,设置成功						
// *    其他,设置失败 						
// */

//uint8_t MPU9250_SetRate(uint16_t rate)
//{
//	uint8_t data;
//	if(rate>1000)
//		rate=1000;
//	if(rate<4)
//		rate=4;
//	data = 1000 / rate - 1;
//	data = IIC_WriteAddrRegByte(MPU6500_ADDR, MPU9250_SAMPLE_RATE_REG, data);	//设置数字低通滤波器
// 	return MPU9250_SetLPF(rate/2);											//自动设置LPF为采样率的一半
//}
