#include "chassis_task.h"

static const int32_t chassis_get_control_data_sig = 0x0001;
extern osThreadId chassisTaskHandle;

static uint16_t *uart4_rxd_len;
static uint8_t *uart4_rx_buffer;
static const uint32_t chassis_control_data_overtime = 50;
static PS2 *remote_data_pt;
void StartChassisTask(void const *argument)
{
    static uint16_t servo_pwm;
    static float left_speed, right_speed;
    StartTimer2Timer3AndStartMotorSpeedCalcTimer();
    StartServoTimer();
    TimeEightInit();
    Uart4_Rx_Init();
    Uart4_Tx_Init();
    remote_data_pt = Get_Parsed_Ps_Two_Data();
    uart4_rx_buffer = Get_Uart4_DMA_RxBuffer();
    uart4_rxd_len = Get_Uart4_Rxd_Data_Len();
    osEvent chassis_control_event;

    (void)chassis_control_data_overtime;
    // 40-260 // 50-143 ÓÐÐ§
    LL_TIM_OC_SetCompareCH1(TIM1, 96);
    for (;;)
    {
        // chassis_control_event = osSignalWait(chassis_get_control_data_sig, osWaitForever);
        // if (chassis_control_event.status == osEventSignal)
        // {
        //     if (chassis_control_event.value.signals == chassis_get_control_data_sig)
        //     {
        //         ___printf("rx len: %d ;data : %d\r\n", *uart4_rxd_len, uart4_rx_buffer[1] << 8 | uart4_rx_buffer[2]);
        //         LL_TIM_OC_SetCompareCH1(TIM1, uart4_rx_buffer[1] << 8 | uart4_rx_buffer[2]);
        //     }
        // }
        // SetMotorLeftPower(5000);
        // Set_ChassisMotor_Speed(0, 0);
        SetMotorRightPower(5000);
        // ___printf("ro: %d %d\r\n", TIM3->CNT, GetMotorRightSpeed());
        // ___printf("ro: %d %d \r\n", GetMotorLeftSpeed(), GetMotorRightSpeed());
        // ___printf("%d %d %d %d \r\n ", remote_data_pt->ch0,
        //           remote_data_pt->ch1,
        //           remote_data_pt->ch2,
        //           remote_data_pt->key);
        // 2.84

        // remote_data_pt->ch0 = remote_data_pt->ch0 + 128; //! 0-256
        servo_pwm = (uint16_t)((remote_data_pt->ch0 + 128)* 0.75);
        servo_pwm = Servo_Pwm_Limit(&servo_pwm);

        LL_TIM_OC_SetCompareCH1(TIM1, servo_pwm);
        // ___printf("%d %d\r\n", servo_pwm, remote_data_pt->ch0);
        osDelay(10);
    }
}

void Info_Chassis_Task_Get_Control_Data(void)
{
    osSignalSet(chassisTaskHandle, chassis_get_control_data_sig);
}

void Set_ChassisMotor_Speed(float speed_left, float speed_right)
{
    // pid, tar, cur
    // int16_t left_output = Pid_Position_Calc(&motor_bl_speed_pid, speed_left, (float)GetMotorLeftSpeed());
    // int16_t right_output = Pid_Position_Calc(&motor_br_speed_pid, speed_right, (float)GetMotorRightSpeed());
    // SetMotorLeftPower(left_output);
    // SetMotorRightPower(right_output);
}

uint16_t Servo_Pwm_Limit(int16_t *input)
{
    if (*input < 50)
        return 50;
    else if (*input > 140)
        return 140;
    else
        return *input;
}

void Set_Ackerman_Steering_Angle(int16_t angle)
{
}