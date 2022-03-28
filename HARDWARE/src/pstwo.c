#include "pstwo.h"
#include "delay.h"

#define DELAY_SHORT  delay_us(4);     //4us  250KHz
#define DELAY_LONG  delay_us(16);  //ʵ��15us

#define DI   PCin(2)           //DAT����   ,�ֱ�����Ƭ��
 
#define DO_H PCout(1)=1        //CMD����� ,��Ƭ�����ֱ�
#define DO_L PCout(1)=0        //CMD�����
 
#define CS_H PCout(3)=1        //Ƭѡ����
#define CS_L PCout(3)=0        //Ƭѡ����
 
#define CLK_H PAout(4)=1       //ʱ������
#define CLK_L PAout(4)=0       //ʱ������
 
#define DI_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* DI�˿�ʱ��*/
#define DI_GPIO_PORT    	GPIOC		                /* DI�˿� */
#define DI_GPIO_PIN		    GPIO_Pin_2			        /* DI�Ķ˿� */

#define DO_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* DO�˿�ʱ��*/
#define DO_GPIO_PORT    	GPIOC		                /* DO�˿� */
#define DO_GPIO_PIN		    GPIO_Pin_1			        /* DO�Ķ˿� */

#define CS_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* CS�˿�ʱ��*/
#define CS_GPIO_PORT    	GPIOC		                /* CS�˿� */
#define CS_GPIO_PIN		    GPIO_Pin_3			        /* CS�Ķ˿� */

#define CLK_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* CLK�˿�ʱ��*/
#define CLK_GPIO_PORT    	GPIOA		                /* CLK�˿� */
#define CLK_GPIO_PIN	    GPIO_Pin_4			        /* CLK�Ķ˿� */


// ������ȡ����ʱ��ȡ

u8 Comd[2]={0x01,0x42};	// ��ʼ�����������
u8 Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; // ���ݴ洢����
u16 MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	// ����ֵ
 
/* PS2�ֱ�IO�ڳ�ʼ�� */
void PS2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(DI_GPIO_CLK | DO_GPIO_CLK | CS_GPIO_CLK | CLK_GPIO_CLK, ENABLE);  //����ȫ��ʱ��
	
	/* DI����Ϊ�������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //��������
	GPIO_InitStructure.GPIO_Pin = DI_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DI_GPIO_PORT, &GPIO_InitStructure);
	
	/* DO����Ϊ������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = DO_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DO_GPIO_PORT, &GPIO_InitStructure);
	
	/* CS����Ϊ������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = CS_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CS_GPIO_PORT, &GPIO_InitStructure);
	
	/* CLK����Ϊ������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = CLK_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CLK_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_SetBits(CS_GPIO_PORT, CS_GPIO_PIN);
	GPIO_SetBits(CLK_GPIO_PORT, CLK_GPIO_PIN);
	GPIO_ResetBits(DO_GPIO_PORT, DO_GPIO_PIN);
}
//���ֱ���������
void PS2_Cmd(u8 CMD)
{
	volatile u16 ref=0x01;
	Data[1] = 0;       
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H;                   //���һλ����λ
		}
		else DO_L;
 
		CLK_H;                        //ʱ������
		DELAY_SHORT;
		CLK_L;
		DELAY_SHORT;
		CLK_H;
		if(DI)
			Data[1] = ref|Data[1];
	}
	delay_us(16);
}
//�ж��Ƿ�Ϊ���ģʽ,0x41= �̵�ģʽ ,0x73=���ģʽ
//����ֵ;0,���ģʽ
//		  ����,����ģʽ
u8 PS2_RedLight(void)
{
	CS_L;               //Ƭѡ����
	PS2_Cmd(Comd[0]);   //��ʼ����
	PS2_Cmd(Comd[1]);   //��������
	CS_H;               //Ƭѡ����
	if( Data[1] == 0X73)   return 0 ;
	else return 1;
 
}
//��ȡ�ֱ�����
void PS2_ReadData(void)
{
	volatile u8 byte=0;
	volatile u16 ref=0x01;
	CS_L;
	PS2_Cmd(Comd[0]);  //��ʼ����
	PS2_Cmd(Comd[1]);  //��������
	for(byte=2;byte<9;byte++)          //��ʼ��������
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			CLK_H;
			DELAY_SHORT;
			CLK_L;
			DELAY_SHORT;
			CLK_H;
		      if(DI)
		      Data[byte] = ref|Data[byte];
		}
        delay_us(16);
	}
	CS_H;
}
 
/*
����ֵ����
//�Զ�������PS2���ݽ��д���,ֻ���������� 
//ֻ��һ����������Ϊ0, δ����Ϊ1
���ذ�����Ӧֵ
*/
u8 PS2_DataKey()
{
	static u16 Handkey;
	static u8 index;
	
	Handkey=(Data[4]<<8)|Data[3];     //����16������  ����Ϊ0, δ����Ϊ1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;          //û�а����κΰ���
}
 
//�õ�һ��ң��ģ����  ��Χ0~256
u8 PS2_AnologData(u8 button)
{
	return Data[button];
}
 
//������ݻ�����
void PS2_ClearData()
{
	u8 a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}
/******************************************************
Function:    void PS2_Vibration(u8 motor1, u8 motor2)
Description: �ֱ��𶯺�����
Calls:		 void PS2_Cmd(u8 CMD);
Input: motor1:�Ҳ�С�𶯵�� 0x00�أ�������
	   motor2:�����𶯵�� 0x40~0xFF �������ֵԽ�� ��Խ��
******************************************************/
void PS2_Vibration(u8 motor1, u8 motor2)
{
	CS_L;
	delay_us(16);
  PS2_Cmd(0x01);  //��ʼ����
	PS2_Cmd(0x42);  //��������
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);  
}
//short poll
void PS2_ShortPoll(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	CS_H;
	delay_us(16);	
}
//��������
void PS2_EnterConfing(void)
{
  CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}
//����ģʽ����
void PS2_TurnOnAnalogMode(void)
{
	CS_L;
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); //analog=0x01;digital=0x00  ������÷���ģʽ
	PS2_Cmd(0xEE); //Ox03��������,������ͨ������MODE����ģʽ
				         //0xEE�������������,����ͨ��MODE������ģʽ
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}
//������
void PS2_VibrationMode(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	CS_H;
	delay_us(16);	
}
//��ɲ���������
void PS2_ExitConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	CS_H;
	delay_us(16);
}
//�ֱ����ó�ʼ��
void PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		   //��������ģʽ
	PS2_TurnOnAnalogMode();	//���̵�����ģʽ,��ѡ���Ƿ񱣴�
	PS2_VibrationMode();	  //������ģʽ
	PS2_ExitConfing();		  //��ɲ���������
}


void Update_Ps2_Data(void)
{
	PS2_ClearData();
	PS2_ReadData();
}


void Get_Rc_Data(PS2* rc)
{
    rc->ch0 = (int8_t)(Data[5] - 128);
		rc->ch1 = -(int8_t)(Data[6] - 127);
		rc->ch2 = (int8_t)(Data[7] - 128);
		rc->ch3 = -(int8_t)(Data[8] - 127);
		rc->key = PS2_DataKey();
}
