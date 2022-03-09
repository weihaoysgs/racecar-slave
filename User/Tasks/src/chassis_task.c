#include "chassis_task.h"
#include "motor_speed_timer.h"
#include "uart4.h"
#include "motor.h"


static const int32_t chassis_get_control_data_sig = 0x0001;
extern osThreadId chassisTaskHandle;

static uint16_t *uart4_rxd_len;                           
static uint8_t *uart4_rx_buffer;                          
static const uint32_t chassis_control_data_overtime = 50; 


void StartChassisTask(void const *argument)
{
    StartTimer2Timer3AndStartMotorSpeedCalcTimer();
    
    // LL_TIM_Enable
    Uart4_Rx_Init();
    Uart4_Tx_Init();
    uart4_rx_buffer = Get_Uart4_DMA_RxBuffer();
    uart4_rxd_len = Get_Uart4_Rxd_Data_Len();
    osEvent chassis_control_event;

    for (;;)
    {
        //        ___printf("hello\r\n");
        // chassis_control_event = osSignalWait(chassis_get_control_data_sig, osWaitForever);
        // if (chassis_control_event.status == osEventSignal)
        // {
        //     if (chassis_control_event.value.signals == chassis_get_control_data_sig)
        //     {
        //         // ___printf("rx len: %d ;data : %s\r\n",*uart4_rxd_len,uart4_rx_buffer);
               
        //     }
        // }
        // SetMotorLeftPower(30000);
        // SetMotorRightPower(30000);
        // ___printf("ro: %d %d\r\n", TIM3->CNT, GetMotorRightSpeed());
        ___printf("ro: %d %d %d\r\n", GetMotorLeftSpeed(), GetMotorRightSpeed(), TIM2->CNT);
        osDelay(50);
    }
}

void Info_Chassis_Task_Get_Control_Data(void)
{
    osSignalSet(chassisTaskHandle, chassis_get_control_data_sig);
}
