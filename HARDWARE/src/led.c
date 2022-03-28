#include "led.h"

/**
 * @brief Led 初始化
 * 
 */
void Led_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能 PB,PE 端口时钟
    GPIO_SetBits(GPIOB, GPIO_Pin_13);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;            // LED0-->PB.5 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;     // IO 口速度为 50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);                //初始化 GPIOB.5
}
