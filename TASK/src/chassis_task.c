#include "chassis_task.h"

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

static const Rc_Data_t *rc_data_pt;

static const Servo_Construction_Value_t servo_limit_value_t =
	{
		.max_ = 2395,
		.middle = 2000,
		.min_ = 1600};

static const uint8_t send_buffer_size = 11;
static uint8_t encoder_send_buffer[send_buffer_size];

static void Chassis_Thread(void *param)
{
	rc_data_pt = Get_Rc_Data();
	uint16_t servo_pulse;
	rt_thread_delay(1000);
	(void)servo_pulse;

	for (;;)
	{
		Send_Chessis_Encoder2Ros(encoder_send_buffer, send_buffer_size);
		printf("ch1:%d  ch2:%d  ch3:%d  ch4:%d  \r\n", rc_data_pt->ch1, rc_data_pt->ch2, rc_data_pt->ch3-1000, rc_data_pt->ch4);

		servo_pulse = rc_data_pt->ch1 * 2;

		Set_Racecar_Direction(&servo_pulse);
		Set_Chassis_Motor_Speed(rc_data_pt->ch3 - 1000, rc_data_pt->ch3 - 1000);

		LED_TOGGLE();
		rt_thread_delay(50);
	}
}

void Set_Chassis_Motor_Speed(float left_motor_speed, float right_motor_speed)
{
	static Pid_Position_t motor_left_speed_pid = NEW_POSITION_PID(10, 0, 4.5, 2000, 7000, 0, 1000, 500);
	static Pid_Position_t motor_right_speed_pid = NEW_POSITION_PID(10, 0, 4.5, 2000, 7000, 0, 1000, 500);

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

void Send_Chessis_Encoder2Ros(uint8_t *buffer, const uint8_t size)
{
	static const uint8_t encoder_hader_one = 0xFF;
	static const uint8_t encoder_hader_two = 0xEE;
	static const uint8_t encoder_tail = 0xDD;

	int32_t right_encoder_changed_value = GetMotorRightSpeed();
	int32_t left_encoder_changed_value = GetMotorLeftSpeed();

	buffer[0] = encoder_hader_one;
	buffer[1] = encoder_hader_two;

	buffer[2] = left_encoder_changed_value >> 24;
	buffer[3] = left_encoder_changed_value >> 16;
	buffer[4] = left_encoder_changed_value >> 8;
	buffer[5] = left_encoder_changed_value;

	buffer[6] = right_encoder_changed_value >> 24;
	buffer[7] = right_encoder_changed_value >> 16;
	buffer[8] = right_encoder_changed_value >> 8;
	buffer[9] = right_encoder_changed_value;

	buffer[10] = encoder_tail;

	Usart1_Dma_Send((uint32_t)encoder_send_buffer, size);
}

void Set_Racecar_Direction(uint16_t *pulse)
{
	TIM_SetCompare1(TIM1, *pulse);
	Int16_Constrain(pulse, servo_limit_value_t.min_, servo_limit_value_t.max_);
}
