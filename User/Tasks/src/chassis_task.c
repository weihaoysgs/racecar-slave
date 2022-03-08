#include "chassis_task.h"

static const int32_t chassis_get_control_data_sig = 0x0001;
extern osThreadId chassisTaskHandle;

static uint16_t *uart4_rxd_len;                           
static uint8_t *uart4_rx_buffer;                          
static const uint32_t chassis_control_data_overtime = 50; 

void StartChassisTask(void const *argument)
{
    Uart4_Rx_Init();
    Uart4_Tx_Init();
    uart4_rx_buffer = Get_Uart4_DMA_RxBuffer();
    uart4_rxd_len = Get_Uart4_Rxd_Data_Len();
    osEvent chassis_control_event;

    for (;;)
    {
        //        ___printf("hello\r\n");
        chassis_control_event = osSignalWait(chassis_get_control_data_sig, osWaitForever);
        if (chassis_control_event.status == osEventSignal)
        {
            if (chassis_control_event.value.signals == chassis_get_control_data_sig)
            {
                ___printf("rx len: %d ;data : %s\r\n",*uart4_rxd_len,uart4_rx_buffer);
            }
        }
        osDelay(50);
    }
}

void Info_Chassis_Task_Get_Control_Data(void)
{
    osSignalSet(chassisTaskHandle, chassis_get_control_data_sig);
}
