#include "mpu9250.h"

#include "softiic.h"
#include "delay.h"
#include "usart4.h"

unsigned char mup_buff[10]; //接收数据缓存区
Mpu_Data_t raw_data;

uint8_t temp;

void Init_MPU9250(void)
{

	IIC_Init();
	delay_ms(10);
	if (MPU_Read_Byte(GYRO_ADDRESS, WHO_AM_I) == 0x71)
	{
		printf("mpu9250 init success\r\n");
		MPU_Write_Byte(GYRO_ADDRESS, PWR_MGMT_1, 0x80); //复位MPU9250
		delay_ms(100);
		MPU_Write_Byte(GYRO_ADDRESS, PWR_MGMT_1, 0x00);	   //唤醒MPU9250
		MPU_Write_Byte(GYRO_ADDRESS, SMPLRT_DIV, 0x07);	   //陀螺仪采样率,125Hz
		MPU_Write_Byte(GYRO_ADDRESS, CONFIG, 0x06);		   //低通滤波频率,5Hz
		MPU_Write_Byte(GYRO_ADDRESS, GYRO_CONFIG, 0x18);   //陀螺仪自检及测量范围,不自检，±2000deg/s
		MPU_Write_Byte(GYRO_ADDRESS, ACCEL_CONFIG, 0x018); //±16g
	}
}

//******读取MPU9250数据****************************************
void READ_MPU9250_ACCEL(void)
{

	mup_buff[0] = MPU_Read_Byte(ACCEL_ADDRESS, ACCEL_XOUT_L);
	mup_buff[1] = MPU_Read_Byte(ACCEL_ADDRESS, ACCEL_XOUT_H);
	raw_data.ax = (mup_buff[1] << 8) | mup_buff[0];
	raw_data.ax /= 164; //读取计算X轴数据

	mup_buff[2] = MPU_Read_Byte(ACCEL_ADDRESS, ACCEL_YOUT_L);
	mup_buff[3] = MPU_Read_Byte(ACCEL_ADDRESS, ACCEL_YOUT_H);
	raw_data.ay = (mup_buff[3] << 8) | mup_buff[2];
	raw_data.ay /= 164; //读取计算Y轴数据
	mup_buff[4] = MPU_Read_Byte(ACCEL_ADDRESS, ACCEL_ZOUT_L);
	mup_buff[5] = MPU_Read_Byte(ACCEL_ADDRESS, ACCEL_ZOUT_H);
	raw_data.az = (mup_buff[5] << 8) | mup_buff[4];
	raw_data.az /= 164; //读取计算Z轴数据
}

void READ_MPU9250_GYRO(void)
{

	mup_buff[0] = MPU_Read_Byte(GYRO_ADDRESS, GYRO_XOUT_L);
	mup_buff[1] = MPU_Read_Byte(GYRO_ADDRESS, GYRO_XOUT_H);
	raw_data.gx = (mup_buff[1] << 8) | mup_buff[0];
	raw_data.gx /= 16.4; //读取计算X轴数据

	mup_buff[2] = MPU_Read_Byte(GYRO_ADDRESS, GYRO_YOUT_L);
	mup_buff[3] = MPU_Read_Byte(GYRO_ADDRESS, GYRO_YOUT_H);
	raw_data.gy = (mup_buff[3] << 8) | mup_buff[2];
	raw_data.gy /= 16.4; //读取计算Y轴数据
	mup_buff[4] = MPU_Read_Byte(GYRO_ADDRESS, GYRO_ZOUT_L);
	mup_buff[5] = MPU_Read_Byte(GYRO_ADDRESS, GYRO_ZOUT_H);
	raw_data.gz = (mup_buff[5] << 8) | mup_buff[4];
	raw_data.gz /= 16.4; //读取计算Z轴数据

	// BUF[6]=Single_Read(GYRO_ADDRESS,TEMP_OUT_L);
	// BUF[7]=Single_Read(GYRO_ADDRESS,TEMP_OUT_H);
	// T_T=(BUF[7]<<8)|BUF[6];
	// T_T = 35+ ((double) (T_T + 13200)) / 280;// 读取计算出温度
}

void READ_MPU9250_MAG(void)
{
	MPU_Write_Byte(GYRO_ADDRESS, 0x37, 0x02); // turn on Bypass Mode
	delay_ms(10);
	MPU_Write_Byte(MAG_ADDRESS, 0x0A, 0x01);
	delay_ms(10);
	mup_buff[0] = MPU_Read_Byte(MAG_ADDRESS, MAG_XOUT_L);
	mup_buff[1] = MPU_Read_Byte(MAG_ADDRESS, MAG_XOUT_H);
	raw_data.mx = (mup_buff[1] << 8) | mup_buff[0];

	mup_buff[2] = MPU_Read_Byte(MAG_ADDRESS, MAG_YOUT_L);
	mup_buff[3] = MPU_Read_Byte(MAG_ADDRESS, MAG_YOUT_H);
	raw_data.my = (mup_buff[3] << 8) | mup_buff[2];
	//读取计算Y轴数据

	mup_buff[4] = MPU_Read_Byte(MAG_ADDRESS, MAG_ZOUT_L);
	mup_buff[5] = MPU_Read_Byte(MAG_ADDRESS, MAG_ZOUT_H);
	raw_data.mz = (mup_buff[5] << 8) | mup_buff[4];
	//读取计算Z轴数据
}

// IIC写一个字节
// reg:寄存器地址
// data:数据
//返回值:0,正常
//     其他,错误代码
void MPU_Write_Byte(uint8_t slave_add, uint8_t reg, uint8_t data)
{
	IIC_Start();
	IIC_Send_Byte(slave_add); //发送器件地址+写命令
	IIC_Wait_Ack();			  //等待应答
	IIC_Send_Byte(reg);		  //写寄存器地址
	IIC_Wait_Ack();			  //等待应答
	IIC_Send_Byte(data);	  //发送数据
	IIC_Wait_Ack();			  //等待ACK
	IIC_Stop();
}

// IIC读一个字节
// reg:寄存器地址
//返回值:读到的数据
uint8_t MPU_Read_Byte(uint8_t slave_add, uint8_t reg)
{
	uint8_t res;
	IIC_Start();
	IIC_Send_Byte(slave_add); //发送器件地址+写命令
	res = IIC_Wait_Ack();	  //等待应答
	IIC_Send_Byte(reg);		  //写寄存器地址
	res = IIC_Wait_Ack();	  //等待应答
	IIC_Start();
	IIC_Send_Byte(slave_add + 1); //发送器件地址+读命令
	res = IIC_Wait_Ack();		  //等待应答
	res = IIC_Read_Byte(0);		  //读取数据,发送nACK
	IIC_Stop();					  //产生一个停止条件
	return res;
}
