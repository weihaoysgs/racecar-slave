#ifndef _PSTWO_H_
#define _PSTWO_H_

#include "stm32f10x.h"
#include "sys.h"
/*
���ģʽʱ�� ����ҡ�˷���ģ��ֵ�� 0x00-0xFF ֮�䣬��ҡ�˰��µļ�ֵ L3�� R3 ��Ч��
�̵�ģʽʱ�� ����ҡ��ģ��ֵΪ��Ч���Ƶ�����ʱ����Ӧ���� UP�� RIGHT�� DOWN��LEFT��
���� ���� X�� �������� L3�� R3 ��Ч��
*/
 
//These are our button constants
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8

#define PSB_L2          9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16
 
#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      16
 
//#define WHAMMY_BAR		8
 
//These are stick values
#define PSS_RX 5             //��ҡ��X������  
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8
 
extern u8 Data[9];
extern u16 MASK[16];


typedef struct
{
	int8_t ch0;  //��X Data[5];
	int8_t ch1;  //��y Data[6];
	int8_t ch2;  //��x Data[7];
	int8_t ch3;  //��y Data[8];
	
	uint8_t key;  //����ֵ
}PS2;


void PS2_Init(void);
u8 PS2_RedLight(void);       //�ж��Ƿ�Ϊ���ģʽ
void PS2_ReadData(void);     //���ֱ�����
void PS2_Cmd(u8 CMD);		      //���ֱ���������
u8 PS2_DataKey(void);		        //����ֵ��ȡ
u8 PS2_AnologData(u8 button);   //�õ�һ��ҡ�˵�ģ����
void PS2_ClearData(void);	      //������ݻ�����
void PS2_Vibration(u8 motor1, u8 motor2);   //������motor1  0xFF���������أ�motor2  0x40~0xFF
 
void PS2_EnterConfing(void);	    //��������
void PS2_TurnOnAnalogMode(void);   //����ģ����
void PS2_VibrationMode(void);      //������
void PS2_ExitConfing(void);	       //�������
void PS2_SetInit(void);		         //���ó�ʼ��
 
 
void Update_Ps2_Data(void);
void Get_Rc_Dataps2(PS2* rc);
#endif //_PSTWO_H_
