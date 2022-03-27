#include "soft_iic.h"
#include "delay_us.h"
#include "stm32f1xx_ll_bus.h"
// MPU IIC ��ʱ����

static void IIC_Delay(void)
{
	delay_us(2);
}

//��ʼ��IIC
void IIC_Init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

	GPIO_InitStruct.Pin = I2C_SCL_PIN | I2C_SDA_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//-------------------------------------------------------------------------------------//
	//	GPIO_InitTypeDef GPIO_InitStructure;

	// RCC_APB2PeriphClockCmd(RCC_I2C_PORT, ENABLE); //��ʹ������IO PORTBʱ��

	// GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN; // �˿�����
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //�������
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // IO���ٶ�Ϊ50MHz
	// GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);			 //�����趨������ʼ��GPIO

	// GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN | I2C_SDA_PIN); // PB10,PB11 �����
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	MPU_SDA_OUT(); // sda�����
	MPU_IIC_SDA = 1;
	MPU_IIC_SCL = 1;
	IIC_Delay();
	MPU_IIC_SDA = 0; // START:when CLK is high,DATA change form high to low
	IIC_Delay();
	MPU_IIC_SCL = 0; //ǯסI2C���ߣ�׼�����ͻ��������
}

//����IICֹͣ�ź�
void IIC_Stop(void)
{
	MPU_SDA_OUT(); // sda�����
	MPU_IIC_SCL = 0;
	MPU_IIC_SDA = 0; // STOP:when CLK is high DATA change form low to high
	IIC_Delay();
	MPU_IIC_SCL = 1;
	MPU_IIC_SDA = 1; //����I2C���߽����ź�
	IIC_Delay();
}
/*
  �ȴ�Ӧ���źŵ���
  ����ֵ��1������Ӧ��ʧ��
		0������Ӧ��ɹ�
*/
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime = 0;
	MPU_SDA_IN(); // SDA����Ϊ����
	MPU_IIC_SDA = 1;
	IIC_Delay();
	MPU_IIC_SCL = 1;
	IIC_Delay();
	while (MPU_READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	MPU_IIC_SCL = 0; //ʱ�����0
	return 0;
}
//����ACKӦ��
void IIC_Ack(void)
{
	MPU_IIC_SCL = 0;
	MPU_SDA_OUT();
	MPU_IIC_SDA = 0;
	IIC_Delay();
	MPU_IIC_SCL = 1;
	IIC_Delay();
	MPU_IIC_SCL = 0;
}
//������ACKӦ��
void IIC_NAck(void)
{
	MPU_IIC_SCL = 0;
	MPU_SDA_OUT();
	MPU_IIC_SDA = 1;
	IIC_Delay();
	MPU_IIC_SCL = 1;
	IIC_Delay();
	MPU_IIC_SCL = 0;
}

/*
	IIC����һ���ֽ�
	���شӻ�����Ӧ��
	1����Ӧ��
	0����Ӧ��
*/
void IIC_Send_Byte(uint8_t txd)
{
	uint8_t t;
	MPU_SDA_OUT();
	MPU_IIC_SCL = 0; //����ʱ�ӿ�ʼ���ݴ���
	for (t = 0; t < 8; t++)
	{
		MPU_IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		MPU_IIC_SCL = 1;
		IIC_Delay();
		MPU_IIC_SCL = 0;
		IIC_Delay();
	}
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	MPU_SDA_IN(); // SDA����Ϊ����
	for (i = 0; i < 8; i++)
	{
		MPU_IIC_SCL = 0;
		IIC_Delay();
		MPU_IIC_SCL = 1;
		receive <<= 1;
		if (MPU_READ_SDA)
			receive++;
		IIC_Delay();
	}
	if (!ack)
		IIC_NAck(); //����nACK
	else
		IIC_Ack(); //����ACK
	return receive;
}
