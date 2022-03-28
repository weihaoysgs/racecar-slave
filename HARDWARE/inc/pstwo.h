#ifndef _PSTWO_H_
#define _PSTWO_H_

#include "stm32f10x.h"
#include "sys.h"
/*
红灯模式时： 左右摇杆发送模拟值， 0x00-0xFF 之间，且摇杆按下的键值 L3、 R3 有效；
绿灯模式时： 左右摇杆模拟值为无效，推到极限时，对应发送 UP、 RIGHT、 DOWN、LEFT、
△、 〇、 X、 □，按键 L3、 R3 无效。
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
#define PSS_RX 5             //右摇杆X轴数据  
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8
 
extern u8 Data[9];
extern u16 MASK[16];


typedef struct
{
	int8_t ch0;  //右X Data[5];
	int8_t ch1;  //右y Data[6];
	int8_t ch2;  //左x Data[7];
	int8_t ch3;  //左y Data[8];
	
	uint8_t key;  //按键值
}PS2;


void PS2_Init(void);
u8 PS2_RedLight(void);       //判断是否为红灯模式
void PS2_ReadData(void);     //读手柄数据
void PS2_Cmd(u8 CMD);		      //向手柄发送命令
u8 PS2_DataKey(void);		        //按键值读取
u8 PS2_AnologData(u8 button);   //得到一个摇杆的模拟量
void PS2_ClearData(void);	      //清除数据缓冲区
void PS2_Vibration(u8 motor1, u8 motor2);   //振动设置motor1  0xFF开，其他关，motor2  0x40~0xFF
 
void PS2_EnterConfing(void);	    //进入配置
void PS2_TurnOnAnalogMode(void);   //发送模拟量
void PS2_VibrationMode(void);      //振动设置
void PS2_ExitConfing(void);	       //完成配置
void PS2_SetInit(void);		         //配置初始化
 
 
void Update_Ps2_Data(void);
void Get_Rc_Data(PS2* rc);
#endif //_PSTWO_H_
