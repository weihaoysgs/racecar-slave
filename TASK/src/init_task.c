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
#include "adc.h"

/**
 * @brief ���������ʼ��
 *
 */
void Peripheral_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    Led_Init();
    Uart5_Init();
    Uart4_Init();
    
    Adc_Init();

    // ros����
    Usart1_Init_Self();

    //������
    Init_MPU9250();

    //���
    Time8_PWM_Init(7200, 0);

    // ���������
    Encoder_Init_TIM2();
    Encoder_Init_TIM3();

    // ��� 800 - 2200
    Time1_Pwm_Init(4000, 71);
}
