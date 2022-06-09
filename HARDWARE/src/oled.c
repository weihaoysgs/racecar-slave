#include "oled.h"

// OLED������ΪPC13��PC14��PC15��PC0
// ����δ��Ӧ�κ�IIC����
// ��ʹ��IO��ģ��iicЭ��


// IO �ڶ�Ӧ�����ſ���
#define OLED_RST_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_13);
#define OLED_RST_Set() GPIO_SetBits(GPIOB, GPIO_Pin_13);

#define OLED_DC_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_14);
#define OLED_DC_Set() GPIO_SetBits(GPIOB, GPIO_Pin_14);

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_15);
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB, GPIO_Pin_15);

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_0);
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB, GPIO_Pin_0);

/**
 * @brief 0.96��OLED��Ļ��ʼ��
 * 
 */
void Oled_Gpio_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_ResetBits(GPIOC, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
