#include "oled.h"

// OLED的引脚为PC13、PC14、PC15、PC0
// 引脚未对应任何IIC外设
// 故使用IO口模拟iic协议


// IO 口对应的引脚控制
#define OLED_RST_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_13);
#define OLED_RST_Set() GPIO_SetBits(GPIOB, GPIO_Pin_13);

#define OLED_DC_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_14);
#define OLED_DC_Set() GPIO_SetBits(GPIOB, GPIO_Pin_14);

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_15);
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB, GPIO_Pin_15);

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_0);
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB, GPIO_Pin_0);

/**
 * @brief 0.96寸OLED屏幕初始化
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
