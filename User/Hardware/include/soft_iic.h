#ifndef _SOFT_IIC_
#define _SOFT_IIC_

#include "stm32f1xx.h"
#include "port_operate.h"
#include "delay_us.h"
#include "uart4.h"

/*
 * IIC������������
 */

void IIC_Init(void); // ��ʼ��IIC��IO��

void IIC_Start(void);           // ����IIC��ʼ�ź�
void IIC_Stop(void);            // ����IICֹͣ�ź�
void IIC_Ack(void);             // IIC����ACK�ź�
void IIC_NAck(void);            // IIC����NACK�ź�
void IIC_SendByte(uint8_t txd); // IIC����һ���ֽڣ������ȴ�Ӧ��
uint8_t IIC_RecvByte(void);     // IIC����һ���ֽڣ�����Ӧ��
uint8_t IIC_WaitAck(void);      // IIC�ȴ�ACK�ź�

/*
 * IIC��ϲ�������
 */

uint8_t IIC_WriteByte(uint8_t txd);                                                  // IIC����һ���ֽڣ����ȴ�Ӧ��
uint8_t IIC_WriteAddrRegByte(uint8_t addr, uint8_t reg, uint8_t dat);                // �ڸ������������Ĵ���д��һ����������
uint8_t IIC_WriteAddrRegBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf); // �ڸ������������Ĵ���д������������

uint8_t IIC_ReadByte(uint8_t ack);                                                  // IIC��ȡһ���ֽ�
uint8_t IIC_ReadAddrRegByte(uint8_t addr, uint8_t reg);                             // �ڸ������������Ĵ�������һ������
uint8_t IIC_ReadAddrRegBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf); // �ڸ������������Ĵ��������������

#endif // _SOFT_IIC_
