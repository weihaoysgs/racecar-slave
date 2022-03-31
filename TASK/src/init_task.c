#include "init_task.h"
#include "led.h"
#include "usart4.h"
#include "time2.h"
#include "time3.h"
#include "time8.h"
#include "mpu9250.h"

/**
 * @brief 外设初始化
 *
 */
void Peripheral_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // 需要使能AFIO时钟
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    
    Led_Init();
    Uart4_Init();
    
    Time8_PWM_Init(7200, 0);
    Init_MPU9250();

    Encoder_Init_TIM2();
    Encoder_Init_TIM3();
}
