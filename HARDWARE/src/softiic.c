#include "softiic.h"

#include "delay.h"

//MPU IIC 延时函数
static void IIC_Delay(void)
{
	delay_us(2);
}

//初始化IIC
void IIC_Init(void)
{					     
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_I2C_PORT,ENABLE);//先使能外设IO PORTB时钟 
		
  GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN|I2C_SDA_PIN;	 // 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);					 //根据设定参数初始化GPIO 
	
  GPIO_SetBits(GPIO_PORT_I2C,I2C_SCL_PIN|I2C_SDA_PIN);						 //PB10,PB11 输出高	
 
}
//产生IIC起始信号
void IIC_Start(void)
{
	MPU_SDA_OUT();     //sda线输出
	MPU_IIC_SDA=1;	  	  
	MPU_IIC_SCL=1;
	IIC_Delay();
 	MPU_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	IIC_Delay();
	MPU_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  

//产生IIC停止信号
void IIC_Stop(void)
{
	MPU_SDA_OUT();//sda线输出
	MPU_IIC_SCL=0;
	MPU_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	IIC_Delay();
	MPU_IIC_SCL=1; 
	MPU_IIC_SDA=1;//发送I2C总线结束信号
	IIC_Delay();							   	
}
/*
  等待应答信号到来
  返回值：1，接收应答失败
        0，接收应答成功
*/
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	MPU_SDA_IN();      //SDA设置为输入  
	MPU_IIC_SDA=1;IIC_Delay();	   
	MPU_IIC_SCL=1;IIC_Delay();	 
	while(MPU_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	MPU_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	MPU_IIC_SCL=0;
	MPU_SDA_OUT();
	MPU_IIC_SDA=0;
	IIC_Delay();
	MPU_IIC_SCL=1;
	IIC_Delay();
	MPU_IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	MPU_IIC_SCL=0;
	MPU_SDA_OUT();
	MPU_IIC_SDA=1;
	IIC_Delay();
	MPU_IIC_SCL=1;
	IIC_Delay();
	MPU_IIC_SCL=0;
}	

/*
	IIC发送一个字节
	返回从机有无应答
	1，有应答
	0，无应答	
*/
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	MPU_SDA_OUT(); 	    
    MPU_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
			MPU_IIC_SDA=(txd&0x80)>>7;
			txd<<=1; 	  
			MPU_IIC_SCL=1;
			IIC_Delay(); 
			MPU_IIC_SCL=0;	
			IIC_Delay();
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MPU_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
		MPU_IIC_SCL=0; 
		IIC_Delay();
		MPU_IIC_SCL=1;
		receive<<=1;
		if(MPU_READ_SDA)receive++;   
		IIC_Delay(); 
  }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

