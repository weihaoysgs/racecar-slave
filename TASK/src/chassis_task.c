#include "chassis_task.h"
#include "motor_speed_timer.h"
#include "usart4.h"

/* relate to thread BEGIN */
ALIGN(RT_ALIGN_SIZE)
static char chassis_thread_stack[512];
static struct rt_thread chassis_thread_object;
static void Chassis_Thread(void *param);

void Chassis_Thread_Init(void)
{
	rt_thread_init(&chassis_thread_object,
				   "chassis",
				   Chassis_Thread,
				   RT_NULL,
				   &chassis_thread_stack[0],
				   sizeof(chassis_thread_stack),
				   2,
				   20);
}

rt_thread_t Get_Chassis_Thread_Object(void)
{
	return &chassis_thread_object;
}
/* relate to thread END */

PS2 *remoter;
static const uint16_t servo_midle_value = 2000;
static const uint16_t servo_min_value = 1600;
static const uint16_t servo_max_value = 2395;
static uint8_t encoder_send_buffer[11];

static uint8_t encoder_hader_one = 0xFF;
static uint8_t encoder_hader_two = 0xEE;
static uint8_t encoder_tail = 0xDD;

static int32_t left_encoder_changed_value;
static int32_t right_encoder_changed_value;

static void Chassis_Thread(void *param)
{
	remoter = Get_Remoter_Data();
	uint16_t servo_pulse;
	rt_thread_delay(1000);
	(void)servo_pulse;
	(void)servo_midle_value;
	(void)servo_min_value;
	(void)servo_max_value;
	for (;;)
	{
		LED_TOGGLE();
		encoder_send_buffer[0] = encoder_hader_one;
		encoder_send_buffer[1] = encoder_hader_two;

		right_encoder_changed_value = GetMotorRightSpeed();
		left_encoder_changed_value = GetMotorLeftSpeed();

		encoder_send_buffer[2] = left_encoder_changed_value >> 24;
		encoder_send_buffer[3] = left_encoder_changed_value >> 16;
		encoder_send_buffer[4] = left_encoder_changed_value >> 8;
		encoder_send_buffer[5] = left_encoder_changed_value;

		encoder_send_buffer[6] = right_encoder_changed_value >> 24;
		encoder_send_buffer[7] = right_encoder_changed_value >> 16;
		encoder_send_buffer[8] = right_encoder_changed_value >> 8;
		encoder_send_buffer[9] = right_encoder_changed_value;
		encoder_send_buffer[10] = encoder_tail;

		// int32_t left = encoder_send_buffer[2] << 24 | encoder_send_buffer[3] << 16 | encoder_send_buffer[4] << 8 | encoder_send_buffer[5];
		// int32_t right = encoder_send_buffer[6] << 24 | encoder_send_buffer[7] << 16 | encoder_send_buffer[8] << 8 | encoder_send_buffer[9];
		// printf("%d %d \r\n", left, right);

		// Usart4_Dma_Send((uint32_t )encoder_send_buffer, 11);

		// servo_pulse = 1501 + (uint16_t)((remoter->ch0 + 128) * 3.90);
		// Int16_Constrain(&servo_pulse, servo_min_value, servo_max_value);
		// TIM_SetCompare1(TIM1, servo_pulse);
		// Set_Chassis_Motor_Speed(remoter->ch3 * 30, remoter->ch3 * 30);

		rt_thread_delay(50); 
	}
}

static Pid_Position_t motor_left_speed_pid = NEW_POSITION_PID(11, 0, 4.5, 2000, 7000, 0, 1000, 500);
static Pid_Position_t motor_right_speed_pid = NEW_POSITION_PID(11, 0, 4.5, 2000, 7000, 0, 1000, 500);

void Set_Chassis_Motor_Speed(float left_motor_speed, float right_motor_speed)
{
	int16_t pid_left = Pid_Position_Calc(&motor_left_speed_pid, left_motor_speed, (float)GetMotorLeftSpeed());
	int16_t pid_right = Pid_Position_Calc(&motor_right_speed_pid, right_motor_speed, (float)GetMotorRightSpeed());
	SetMotorLeftPower(pid_left);
	SetMotorRightPower(pid_right);
}

void Int16_Constrain(uint16_t *data, const uint16_t min_value, const uint16_t max_value)
{
	if (*data > max_value)
		*data = max_value;
	else if (*data < min_value)
		*data = min_value;
}
