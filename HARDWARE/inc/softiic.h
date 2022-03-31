#ifndef __SOFT_IIC_H_
#define __SOFT_IIC_H_

#include "stm32f10x.h"
#include "sys.h"

// IO方向设置
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

// IO操作函数
#define MPU_IIC_SCL PBout(10) // SCL
#define MPU_IIC_SDA PBout(11) // SDA
#define MPU_READ_SDA PBin(11) //输入SDA

#define RCC_I2C_PORT RCC_APB2Periph_GPIOB /* GPIO端口时钟 */
#define GPIO_PORT_I2C GPIOB               /* GPIO端口 */
#define I2C_SCL_PIN GPIO_Pin_10           /* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN GPIO_Pin_11           /* 连接到SDA数据线的GPIO */

// IIC所有操作函数
void IIC_Init(void);                 //初始化IIC的IO口
void IIC_Start(void);                //发送IIC开始信号
void IIC_Stop(void);                 //发送IIC停止信号
void IIC_Send_Byte(u8 txd);          // IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack); // IIC读取一个字节
u8 IIC_Wait_Ack(void);               // IIC等待ACK信号
void IIC_Ack(void);                  // IIC发送ACK信号
void IIC_NAck(void);                 // IIC不发送ACK信号

#endif // __SOFT_IIC_H_
