#include "stm32f10x.h"
#include "usart4.h"
#include "pstwo.h"
#include "delay.h"
#include "time1.h"
void Delay(u32 count)
{
	u32 i = 0;
	for (; i < count; i++)
		;
}
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Uart4_Init();
	Time1_Pwm_Init(2000, 719); //! 50hz
	PS2 remoter;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |
							   RCC_APB2Periph_GPIOE,
						   ENABLE);					  //ʹ�� PB,PE �˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		  // LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO ���ٶ�Ϊ 50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);			  //��ʼ�� GPIOB.5
	GPIO_SetBits(GPIOB, GPIO_Pin_13);				  // PB.5 �����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		  // LED1-->PE.5 �������
	GPIO_Init(GPIOE, &GPIO_InitStructure);			  //��ʼ�� GPIO
	GPIO_SetBits(GPIOE, GPIO_Pin_13);				  // PE.5 �����
	USART_SendData(UART4, 0x0001);
	PS2_Init();
	TIM_SetCompare1(TIM1, 200);
	
	while (1)
	{
		Update_Ps2_Data();
		Get_Rc_Data(&remoter);

		printf("key: %d\t", remoter.key);
		printf("ch0: %d\t", remoter.ch0);
		printf("ch1: %d\t", remoter.ch1);
		printf("ch2: %d\t", remoter.ch2);
		printf("ch3: %d\r\n", remoter.ch3);

		GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		delay_ms(50);
		// GPIO_SetBits(GPIOE,GPIO_Pin_13);
		GPIO_SetBits(GPIOB, GPIO_Pin_13);
		delay_ms(50);
		// GPIO_ResetBits(GPIOE,GPIO_Pin_13);
	}
}
