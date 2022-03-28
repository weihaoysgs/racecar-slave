#include "init_task.h"
#include "led.h"
#include "usart4.h"
#include "time2.h"
#include "time3.h"
#include "time8.h"

/**
 * @brief �����ʼ��
 *
 */
void Peripheral_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    Led_Init();
    Uart4_Init();
    Encoder_Init_TIM2();
    Encoder_Init_TIM3();
    Time8_PWM_Init(7200, 0);
}
