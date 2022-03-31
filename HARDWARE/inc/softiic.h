#ifndef __SOFT_IIC_H_
#define __SOFT_IIC_H_

#include "stm32f10x.h"
#include "sys.h"

// IO��������
#define MPU_SDA_IN()              \
    {                             \
        GPIOB->CRH &= 0XFFFF0FFF; \
        GPIOB->CRH |= 8 << 12;    \
    }
#define MPU_SDA_OUT()             \
    {                             \
        GPIOB->CRH &= 0XFFFF0FFF; \
        GPIOB->CRH |= 3 << 12;    \
    }

// IO��������
#define MPU_IIC_SCL PBout(10) // SCL
#define MPU_IIC_SDA PBout(11) // SDA
#define MPU_READ_SDA PBin(11) //����SDA

#define RCC_I2C_PORT RCC_APB2Periph_GPIOB /* GPIO�˿�ʱ�� */
#define GPIO_PORT_I2C GPIOB               /* GPIO�˿� */
#define I2C_SCL_PIN GPIO_Pin_10           /* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN GPIO_Pin_11           /* ���ӵ�SDA�����ߵ�GPIO */

// IIC���в�������
void IIC_Init(void);                 //��ʼ��IIC��IO��
void IIC_Start(void);                //����IIC��ʼ�ź�
void IIC_Stop(void);                 //����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);          // IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack); // IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void);               // IIC�ȴ�ACK�ź�
void IIC_Ack(void);                  // IIC����ACK�ź�
void IIC_NAck(void);                 // IIC������ACK�ź�

#endif // __SOFT_IIC_H_
