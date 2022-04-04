#include "init_task.h"
#include "led.h"
#include "usart4.h"
#include "time2.h"
#include "time3.h"
#include "time8.h"
#include "mpu9250.h"
#include "time1.h"
#include "usart5.h"
#include "usart1.h"

/**
 * @brief �����ʼ��
 *
 */
void Peripheral_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // ��Ҫʹ��AFIOʱ��
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    Led_Init();
    Uart4_Init();
    Uart5_Init();
    // Usart1_Init();
    Usart1_Init_Qingzhou();

    Time8_PWM_Init(7200, 0);
    Init_MPU9250();

    Encoder_Init_TIM2();
    Encoder_Init_TIM3();

    // 800 - 2200
    Time1_Pwm_Init(4000, 71);
}
