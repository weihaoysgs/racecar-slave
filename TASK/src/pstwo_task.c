#include "pstwo_task.h"

// 重要：Pstwo ---> Sbus遥控器

/* relate to thread BEGIN */
ALIGN(RT_ALIGN_SIZE)
static char pstwo_thread_stack[512];
static struct rt_thread pstwo_thread_object;
static void Pstwo_Thread(void *param);

void Pstwo_Thread_Init(void)
{
    rt_thread_init(&pstwo_thread_object,
                   "pstwo",
                   Pstwo_Thread,
                   RT_NULL,
                   &pstwo_thread_stack[0],
                   sizeof(pstwo_thread_stack),
                   3,
                   20);
}

rt_thread_t Get_Pstwo_Thread_Object(void)
{
    return &pstwo_thread_object;
}
/* relate to thread END */

struct rt_semaphore remoter_get_semaphore; //遥控器收到一帧数据信号量
static uint8_t *uart4_rx_buffer;
static Rc_Data_t rc_data;
static uint8_t rx_connected_status = 0;

static uint8_t Check_Rc_Data_Available(uint8_t* raw_buf);

static void Pstwo_Thread(void *param)
{
    rt_sem_init(&remoter_get_semaphore,
                "remoter_get",
                0,
                RT_IPC_FLAG_PRIO);
    uart4_rx_buffer = Get_Uart4_Rxd_Buffer();
    rt_thread_delay(1000);

    DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE);
    for (;;)
    {
        if(RT_EOK == rt_sem_take(&remoter_get_semaphore, 500))
		{
            if(Check_Rc_Data_Available(uart4_rx_buffer))
            {
                //解析遥控器数据
#define dr16_rbuff uart4_rx_buffer
                /*此解算适用于SBUS(Futaba),Wbus(天地飞)，大疆的DT7不使用，有另外解算*/
                rc_data.ch1 = (dr16_rbuff[1] | (dr16_rbuff[2] << 8)) & 0x07ff;
                rc_data.ch2 = ((dr16_rbuff[2] >> 3) | (dr16_rbuff[3] << 5)) & 0x07ff;
                rc_data.ch3 = ((dr16_rbuff[3] >> 6) | (dr16_rbuff[4] << 2) | (dr16_rbuff[5] << 10)) & 0x07ff;
                rc_data.ch4 = ((dr16_rbuff[5] >> 1) | (dr16_rbuff[6] << 7)) & 0x07ff;
                rc_data.ch5 = ((dr16_rbuff[6] >> 4) | (dr16_rbuff[7] << 4)) & 0x07ff;
                rc_data.ch6 = ((dr16_rbuff[7] >> 7) | (dr16_rbuff[8] << 1) | (dr16_rbuff[9] << 9)) & 0x07ff;
                rc_data.ch7 = ((dr16_rbuff[9] >> 2) | (dr16_rbuff[10] << 6)) & 0x07ff;
                rc_data.ch8 = ((dr16_rbuff[11] << 3) | (dr16_rbuff[10] >> 5)) & 0x07ff;
                rc_data.ch9 = ((dr16_rbuff[12]) | (dr16_rbuff[13] << 8)) & 0x07ff;
                rc_data.ch10 = ((dr16_rbuff[13] >> 13) | (dr16_rbuff[14] << 5)) & 0x07ff;
#undef dr16_rbuff

                // 调试时将下列2行“//”删除，即可打印出遥控器信息
                // printf("ch1:%d  ch2:%d  ch3:%d  ch4:%d  ", rc_data.ch1, rc_data.ch2, rc_data.ch3, rc_data.ch4);
                // printf("ch5:%d  ch6:%d  ch7:%d  ch8:%d  ch9:%d  ch10:%d\r\n", rc_data.ch5, rc_data.ch6, rc_data.ch7, rc_data.ch8, rc_data.ch9, rc_data.ch10);
                rx_connected_status = 1;
            }
            else
            {
                Uart4_DMA_Reset();
            }
		}
		else
		{
            rc_data.ch1 = 1000;
            rc_data.ch2 = 1000;
            rc_data.ch3 = 1000;
            rc_data.ch4 = 1000;
            rx_connected_status = 0;
			//重置遥控器数据
            Uart4_DMA_Reset();
		}
    }
}

/**
 * @brief 获取遥控器数据（RAW）
 * 
 * @return const Rc_Data_t* 
 */
const Rc_Data_t* Get_Rc_Data(void)
{
	return &rc_data;
}


/**
 * @brief 将摇杆的原始数据转换为普遍使用的数据
 * 
 * @param data 
 * @return int16_t 
 */
int16_t Joystick_Raw_To_Normal_Data(uint16_t data)
{
    //               负              正
    // -800 -750          -50 50          750 800
    int16_t normal_data = (int16_t)data - 1000;
    if (normal_data >= 750)
    {
        normal_data = 800;
    }
    else if (normal_data < 750 && normal_data >= 50)
    {
        normal_data = (normal_data - 50) * 800 / 750;
    }
    else if (normal_data < 50 && normal_data > -50)
    {
        normal_data = 0;
    }
    else if (normal_data <= -50 && normal_data > -750)
    {
        normal_data = (normal_data + 50) * 800 / 750;
    }
    else if (normal_data <= -750)
    {
        normal_data = -800;
    }
    return normal_data;
}

/**
 * @brief 检查遥控器数据是否合法
 * 
 * @param raw_buf 
 * @return uint8_t 
 */
static uint8_t Check_Rc_Data_Available(uint8_t* raw_buf)
{
    return ((raw_buf[0] == 0x0F) && (raw_buf[24] == 0x00));
}

uint8_t Rc_Rx_Connected_Status(void)
{
    return rx_connected_status;
}

uint8_t Rc_Valid_Status(void)
{
    if(rx_connected_status == 1 && (rc_data.ch7 < 500 && rc_data.ch8 < 500))
    {
        return 1u;
    }
    return 0u;
}
