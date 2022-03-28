#include "led.h"

/**
 * @brief Led ��ʼ��
 * 
 */
void Led_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ�� PB,PE �˿�ʱ��
    GPIO_SetBits(GPIOB, GPIO_Pin_13);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;            // LED0-->PB.5 �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;     // IO ���ٶ�Ϊ 50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);                //��ʼ�� GPIOB.5
}