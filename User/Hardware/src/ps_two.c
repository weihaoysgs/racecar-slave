#include "ps_two.h"
#include "delay_us.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"

#define DELAY_SHORT delay_us(4); // 4us  250KHz
#define DELAY_LONG delay_us(16); //实际15us

#define DI PCin(2) // DAT输入   ,手柄到单片机

#define DO_H PCout(1) = 1 // CMD输出高 ,单片机到手柄
#define DO_L PCout(1) = 0 // CMD输出低

#define CS_H PCout(3) = 1 //片选拉高
#define CS_L PCout(3) = 0 //片选拉低

#define CLK_H PAout(4) = 1 //时钟拉高
#define CLK_L PAout(4) = 0 //时钟拉低

// #define DI_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* DI端口时钟*/
#define DI_GPIO_PORT GPIOC		  //! DI端口 */
#define DI_GPIO_PIN LL_GPIO_PIN_2 //! DI的端口 */

// #define DO_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* DO端口时钟*/
#define DO_GPIO_PORT GPIOC		  //! DO端口 */
#define DO_GPIO_PIN LL_GPIO_PIN_1 //! DO的端口 */

// #define CS_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* CS端口时钟*/
#define CS_GPIO_PORT GPIOC		  //! CS端口 */
#define CS_GPIO_PIN LL_GPIO_PIN_3 //! CS的端口 */

// #define CLK_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* CLK端口时钟*/
#define CLK_GPIO_PORT GPIOA		   //! CLK端口 */
#define CLK_GPIO_PIN LL_GPIO_PIN_4 //! CLK的端口 */

// 按键读取，零时存取
uint8_t Comd[2] = {0x01, 0x42};											  // 开始命令，请求数据
uint8_t Data[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 数据存储数组
uint16_t MASK[] = {
	PSB_SELECT,
	PSB_L3,
	PSB_R3,
	PSB_START,
	PSB_PAD_UP,
	PSB_PAD_RIGHT,
	PSB_PAD_DOWN,
	PSB_PAD_LEFT,
	PSB_L2,
	PSB_R2,
	PSB_L1,
	PSB_R1,
	PSB_GREEN,
	PSB_RED,
	PSB_BLUE,
	PSB_PINK}; // 按键值

/* PS2手臂IO口初始化 */
void PS2_Init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	// RCC_APB2PeriphClockCmd(DI_GPIO_CLK | DO_GPIO_CLK | CS_GPIO_CLK | CLK_GPIO_CLK, ENABLE);  //开启全部时钟

	/* DI设置为下拉输入 */
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT; //下拉输入
	GPIO_InitStruct.Pin = DI_GPIO_PIN;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	LL_GPIO_Init(DI_GPIO_PORT, &GPIO_InitStruct);

	/* DO设置为推挽输出 */
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pin = DO_GPIO_PIN;
	GPIO_InitStruct.Speed = LL_GPIO_MODE_OUTPUT_50MHz;
	LL_GPIO_Init(DO_GPIO_PORT, &GPIO_InitStruct);

	/* CS设置为推挽输出 */
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pin = CS_GPIO_PIN;
	GPIO_InitStruct.Speed = LL_GPIO_MODE_OUTPUT_50MHz;
	LL_GPIO_Init(CS_GPIO_PORT, &GPIO_InitStruct);

	/* CLK设置为推挽输出 */
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pin = CLK_GPIO_PIN;
	GPIO_InitStruct.Speed = LL_GPIO_MODE_OUTPUT_50MHz;
	LL_GPIO_Init(CLK_GPIO_PORT, &GPIO_InitStruct);

	LL_GPIO_SetOutputPin(CS_GPIO_PORT, CS_GPIO_PIN);
	LL_GPIO_SetOutputPin(CLK_GPIO_PORT, CLK_GPIO_PIN);
	LL_GPIO_ResetOutputPin(DO_GPIO_PORT, DO_GPIO_PIN);
	// GPIO_SetBits(CS_GPIO_PORT, CS_GPIO_PIN);
	// GPIO_SetBits(CLK_GPIO_PORT, CLK_GPIO_PIN);
	// GPIO_ResetBits(DO_GPIO_PORT, DO_GPIO_PIN);
}
//向手柄发送命令
void PS2_Cmd(uint8_t CMD)
{
	volatile uint16_t ref = 0x01;
	Data[1] = 0;
	for (ref = 0x01; ref < 0x0100; ref <<= 1)
	{
		if (ref & CMD)
		{
			DO_H; //输出一位控制位
		}
		else
			DO_L;

		CLK_H; //时钟拉高
		DELAY_SHORT;
		CLK_L;
		DELAY_SHORT;
		CLK_H;
		if (DI)
			Data[1] = ref | Data[1];
	}
	delay_us(16);
}
//判断是否为红灯模式,0x41= 绿灯模式 ,0x73=红灯模式
//返回值;0,红灯模式
//		  其他,其他模式
uint8_t PS2_RedLight(void)
{
	CS_L;			  //片选拉低
	PS2_Cmd(Comd[0]); //开始命令
	PS2_Cmd(Comd[1]); //请求数据
	CS_H;			  //片选拉高
	if (Data[1] == 0X73)
		return 0;
	else
		return 1;
}
//读取手柄数据
void PS2_ReadData(void)
{
	volatile uint8_t byte = 0;
	volatile uint16_t ref = 0x01;
	CS_L;
	PS2_Cmd(Comd[0]);				 //开始命令
	PS2_Cmd(Comd[1]);				 //请求数据
	for (byte = 2; byte < 9; byte++) //开始接受数据
	{
		for (ref = 0x01; ref < 0x100; ref <<= 1)
		{
			CLK_H;
			DELAY_SHORT;
			CLK_L;
			DELAY_SHORT;
			CLK_H;
			if (DI)
				Data[byte] = ref | Data[byte];
		}
		delay_us(16);
	}
	CS_H;
}

/*
按键值解析
//对读出来的PS2数据进行处理,只处理按键部分
//只有一个按键按下为0, 未按下为1
返回按键对应值
*/
uint8_t PS2_DataKey()
{
	static uint16_t Handkey;
	static uint8_t index;

	Handkey = (Data[4] << 8) | Data[3]; //这是16个按键  按下为0, 未按下为1
	for (index = 0; index < 16; index++)
	{
		if ((Handkey & (1 << (MASK[index] - 1))) == 0)
			return index + 1;
	}
	return 0; //没有按下任何按键
}

//得到一个遥感模拟量  范围0~256
uint8_t PS2_AnologData(uint8_t button)
{
	return Data[button];
}

//清除数据缓冲区
void PS2_ClearData()
{
	uint8_t a;
	for (a = 0; a < 9; a++)
		Data[a] = 0x00;
}
/******************************************************
Function:    void PS2_Vibration(uint8_t motor1, uint8_t motor2)
Description: 手柄震动函数，
Calls:		 void PS2_Cmd(uint8_t CMD);
Input: motor1:右侧小震动电机 0x00关，其他开
	   motor2:左侧大震动电机 0x40~0xFF 电机开，值越大 震动越大
******************************************************/
void PS2_Vibration(uint8_t motor1, uint8_t motor2)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01); //开始命令
	PS2_Cmd(0x42); //请求数据
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
// short poll
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
//进入配置
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
//发送模式配置
void PS2_TurnOnAnalogMode(void)
{
	CS_L;
	PS2_Cmd(0x01);
	PS2_Cmd(0x44);
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); // analog=0x01;digital=0x00  软件设置发动模式
	PS2_Cmd(0xEE); // Ox03锁存设置,即不可通过按下MODE设置模式
				   // 0xEE不锁存软件设置,可以通过MODE键设置模式
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}
//振动设置
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
//完成并保存配置
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
//手柄配置初始化
void PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		//进入配置模式
	PS2_TurnOnAnalogMode(); //红绿灯配置模式,并选择是否保存
	PS2_VibrationMode();	//开启振动模式
	PS2_ExitConfing();		//完成并保存配置
}

void Update_Ps2_Data(void)
{
	PS2_ClearData();
	PS2_ReadData();
}

void Get_Rc_Data(PS2 *rc)
{
	rc->ch0 = (int8_t)(Data[5] - 128);
	rc->ch1 = -(int8_t)(Data[6] - 127);
	rc->ch2 = (int8_t)(Data[7] - 128);
	rc->ch3 = -(int8_t)(Data[8] - 127);
	rc->key = PS2_DataKey();
}
