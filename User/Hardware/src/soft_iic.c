#include "soft_iic.h"

/****************************** IICӲ����ʼ�� ***********************************
 * ʹ��IIC1
 * PB10 -- SCL
 * PB11 -- SDA
 *******************************************************************************/

#define SDA_IN() LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_11, LL_GPIO_MODE_INPUT)

#define SDA_OUT() LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_11, LL_GPIO_MODE_OUTPUT)

#define IIC_SCL PBout(10) // SCL
#define IIC_SDA PBout(11) // SDA
#define READ_SDA PBin(11) //����SDA

#define IIC_GPIO GPIOB
#define IIC_PIN GPIO_PIN_10 | GPIO_PIN_11

#define IIC_DELAY_1 1 // Ĭ��Ϊ4
#define IIC_DELAY_2 1 // Ĭ��Ϊ2
#define IIC_DELAY_3 0 // Ĭ��Ϊ1

/*
 * IICӲ����ʼ��
 */

void IIC_Init(void)
{
    // LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
    // LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_INPUT);
    // GPIO_InitTypeDef GPIO_InitStructure; // GPIO��ʼ���ṹ��
    // IIC_ENCLK();						 // ʹ�� IIC ʱ��

    // GPIO_InitStructure.Pin = IIC_PIN;
    // GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; // �������
    // GPIO_InitStructure.Speed = GPIO_SPEED_FAST;	   // 100MHz
    // GPIO_InitStructure.Pull = GPIO_PULLUP;		   // ����
    // HAL_GPIO_Init(IIC_GPIO, &GPIO_InitStructure);  // ��ʼ��

    IIC_SCL = 1;
    IIC_SDA = 1;
}

/****************************** IIC������������ ***********************************/
/*
 * IIC ��ʼ�ź�
 * SCL=1ʱ��SDA��1��0
 */

void IIC_Start(void)
{
    SDA_OUT(); // SDA���ģʽ
    IIC_SDA = 1;
    IIC_SCL = 1; // SCLΪ1
    delay_us(IIC_DELAY_1);
    IIC_SDA = 0; // ��ʼ��SDA��1��0
    delay_us(IIC_DELAY_1);
    IIC_SCL = 0; //ǯסI2C���ߣ�׼�����ͻ��������
}

/*
 * ����IICֹͣ�ź�
 * SCL=1ʱ��SDA��0��1
 */

void IIC_Stop(void)
{
    SDA_OUT();
    IIC_SCL = 0;
    IIC_SDA = 0;
    delay_us(IIC_DELAY_1);
    IIC_SCL = 1;
    IIC_SDA = 1;
    delay_us(IIC_DELAY_1);
}

/*
 * �ȴ�Ӧ���źŵ���
 * �ӻ�����Ӧ��SDA = 0
 * ����ֵ��0�����յ�Ӧ��
 *        1��δ���յ�Ӧ�𣬶�Ӧ��ʼע�͵�2��
 */

uint8_t IIC_WaitAck(void)
{
    uint8_t cnt = 0;
    SDA_IN(); // SDA����Ϊ����
    IIC_SDA = 1;
    delay_us(IIC_DELAY_3);
    IIC_SCL = 1;
    delay_us(1);
    while (READ_SDA) // ��ȡSDA���ӻ�Ӧ��0 �˳�
    {
        cnt++;
        if (cnt > 250)
        {
            IIC_Stop(); // ��ʱ�˳�
            return 1;
        }
    }
    IIC_SCL = 0;
    return 0;
}

/*
 * ����ACKӦ��
 * ��SDA���ͱ�ʾӦ��
 */

void IIC_Ack(void)
{
    IIC_SCL = 0;
    SDA_OUT();
    IIC_SDA = 0; // SDAΪ0 -- Ӧ��
    delay_us(IIC_DELAY_2);
    IIC_SCL = 1;
    delay_us(IIC_DELAY_2);
    IIC_SCL = 0;
}

/*
 * ����ACKӦ��
 * ��SDA���߱�ʾ��Ӧ��
 */

void IIC_NAck(void)
{
    IIC_SCL = 0;
    SDA_OUT();
    IIC_SDA = 1; // SDAΪ1 -- ��Ӧ��
    delay_us(IIC_DELAY_2);
    IIC_SCL = 1;
    delay_us(IIC_DELAY_2);
    IIC_SCL = 0;
}

/*
 * IIC����һ���ֽڣ�û�еȴ�Ӧ��
 */

void IIC_SendByte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    IIC_SCL = 0; // ������ʱ���ٿ�ʼ���ݴ���
    for (t = 0; t < 8; t++)
    {
        IIC_SDA = (txd & 0x80) >> 7; // �������λ
        txd <<= 1;                   // ����һλ
        delay_us(IIC_DELAY_2);       // ��TEA5767��������ʱ���Ǳ����
        IIC_SCL = 1;                 // ����SCL�Ա��ȡ����
        delay_us(IIC_DELAY_2);
        IIC_SCL = 0; // ����ʱ��׼����һλ����
        delay_us(IIC_DELAY_2);
    }
}

/*
 * ����1���ֽ�
 * ���ز��������յ����ֽ�
 */

uint8_t IIC_RecvByte(void)
{
    uint8_t i, recv = 0;
    SDA_IN(); // SDA����Ϊ����
    for (i = 0; i < 8; i++)
    {
        IIC_SCL = 0;
        delay_us(IIC_DELAY_2);
        IIC_SCL = 1;
        if (READ_SDA) // ��ȡ��1��recv����1λ����1
        {
            recv <<= 1;
            recv += 0x01;
        }
        else // ��ȡ��0��recv����1λ����0
        {
            recv <<= 1;
            recv += 0x00;
        }
        delay_us(IIC_DELAY_3);
    }
    return recv;
}

/****************************** IIC��ϲ������� ***********************************/

/*
 * IIC����һ���ֽ�
 * ���شӻ�����Ӧ���൱�� IIC_SendByte() + IIC_WaitAck()
 * ack=0��ʾӦ��ack=1 ��ʾ��Ӧ�𣡣���
 */

uint8_t IIC_WriteByte(uint8_t txd)
{
    uint8_t ack;
    IIC_SendByte(txd);
    ack = IIC_WaitAck(); // �ȴ��ӻ�Ӧ��t=0��ʾӦ��t=1 ��ʾ��Ӧ�𣡣���
    return ack;
}

/*
 * IIC��ָ������ָ���Ĵ�������һ���ֽ�
 * ���شӻ�����Ӧ��
 * 0 ��ʾӦ�𣨷��ͳɹ�����1 ��ʾ��Ӧ�𣨷���ʧ�ܣ�������
 */

uint8_t IIC_WriteAddrRegByte(uint8_t addr, uint8_t reg, uint8_t dat)
{
    IIC_Start();
    if (IIC_WriteByte(addr))
    {
        IIC_Stop();
        ___printf("IIC write addr fail\r\n");
        return 1;
    }
    if (IIC_WriteByte(reg))
    {
        IIC_Stop();
        ___printf("IIC write reg fail\r\n");
        return 1;
    }
    if (IIC_WriteByte(dat))
    {
        IIC_Stop();
        ___printf("IIC write data fail\r\n");
        return 1;
    }
    IIC_Stop();
    return 0;
}

/*
 * IIC��ָ������ָ���Ĵ�������һ���ֽ�
 * ���شӻ�����Ӧ��
 * 0 ��ʾӦ�𣨷��ͳɹ�����1 ��ʾ��Ӧ�𣨷���ʧ�ܣ�������
 */

uint8_t IIC_WriteAddrRegBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    uint8_t i;
    IIC_Start();
    if (IIC_WriteByte(addr))
    {
        IIC_Stop();
        return 1;
    }
    if (IIC_WriteByte(reg))
    {
        IIC_Stop();
        return 1;
    }
    for (i = 0; i < len; i++)
    {
        if (IIC_WriteByte(buf[i]))
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_Stop();
    return 0;
}

/*
 * ��1���ֽڣ��൱�� IIC_RecvByte() + IIC_(N)Ack()
 * ack=0ʱ������ACK��ack=1������NACK�����ǹ��ⷴ�߼���ԭ�����ͷע��2��
 * ���ض�ȡ���ֽ�													'
 */

uint8_t IIC_ReadByte(uint8_t ack)
{
    uint8_t recv = 0;
    recv = IIC_RecvByte();
    if (ack)
        IIC_NAck(); // ack=1 ����nACK
    else
        IIC_Ack(); // ack=0 ����ACK
    return recv;
}

/*
 * �ڸ������������Ĵ�������һ������
 */
uint8_t IIC_ReadAddrRegByte(uint8_t addr, uint8_t reg)
{
    uint8_t recv;

    IIC_Start();
    IIC_WriteByte(addr);
    IIC_WriteByte(reg);

    IIC_Start();
    IIC_WriteByte(addr | 0x01);
    recv = IIC_ReadByte(1); // NAck
    IIC_Stop();
    return recv;
}

/*
 * �ڸ������������Ĵ��������������
 */
uint8_t IIC_ReadAddrRegBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    uint8_t i;
    IIC_Start();
    if (IIC_WriteByte(addr))
    {
        IIC_Stop();
        return 1;
    }
    IIC_WriteByte(reg);

    IIC_Start();
    IIC_WriteByte(addr | 0x01);
    for (i = 0; i < len; i++)
    {
        if (i == len - 1)
            buf[i] = IIC_ReadByte(1); // ���һ�ֽ� NAck
        else
            buf[i] = IIC_ReadByte(0); // �������һ�ֽ�ʱ Ack
    }
    IIC_Stop();
    return 0;
}
