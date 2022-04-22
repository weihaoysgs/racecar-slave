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

static const Rc_Data_t* rc_data_pt;
static const Ros_message_t* ros_msg;

// 左小右大
static const Servo_Construction_Value_t servo_limit_value_t =
{
	.max_ = 1793,
	.middle = 1460, //1000
	.min_ = 1110
};

static void Chassis_Thread(void *param)
{
	rc_data_pt = Get_Rc_Data();
	ros_msg = Get_Ros_Message();
	static uint16_t servo_pulse; ///< 舵机pwm输出值
	static float true_motors_speed_target[2]; ///< 左右两轮目标速度
	rt_thread_delay(2000);

	for (;;)
	{
		// ROS 上位机控制
		if(Get_Ros_Message_Status())
		{
			servo_pulse = (int16_t)servo_limit_value_t.middle + (int16_t)(ros_msg->data.angle * 8);
			true_motors_speed_target[0] = ros_msg->data.speed * 300;
			true_motors_speed_target[1] = ros_msg->data.speed * 300;
		}

		// 遥控器输入
		else if(Rc_Valid_Status())
		{
			servo_pulse = (int16_t)servo_limit_value_t.middle - (int16_t)(Joystick_Raw_To_Normal_Data(rc_data_pt->ch1)/1.8f);
			true_motors_speed_target[0] = Joystick_Raw_To_Normal_Data(rc_data_pt->ch3) / 2.0f;
			true_motors_speed_target[1] = Joystick_Raw_To_Normal_Data(rc_data_pt->ch3) / 2.0f;
		}

		// 无控制信号输入
		else
		{
			servo_pulse = servo_limit_value_t.middle;
			true_motors_speed_target[0] = 0;
			true_motors_speed_target[1] = 0;
		}

		// 设置舵机角度
		Uint16_Constrain(&servo_pulse, servo_limit_value_t.min_, servo_limit_value_t.max_);
		Set_Racecar_Direction(servo_pulse);

		// 通过PID控制电机速度
		// Int16_Constrain(&true_motors_speed_target[0], -500, 500);
		// Int16_Constrain(&true_motors_speed_target[1], -500, 500);
		Set_Chassis_Motor_Speed(true_motors_speed_target[0],
								true_motors_speed_target[1]);

		rt_thread_delay(20);
	}
}

void Set_Chassis_Motor_Speed(float left_motor_speed, float right_motor_speed)
{
	static Pid_Position_t motor_left_speed_pid = NEW_POSITION_PID(16, 0, 0, 200, 7199, 0, 1000, 500);
	static Pid_Position_t motor_right_speed_pid = NEW_POSITION_PID(16, 0, 0, 200, 7199, 0, 1000, 500);
	int16_t pid_left = Pid_Position_Calc(&motor_left_speed_pid, -left_motor_speed, (float)GetMotorLeftSpeed());
	int16_t pid_right = Pid_Position_Calc(&motor_right_speed_pid, right_motor_speed, (float)GetMotorRightSpeed());
	SetMotorLeftPower(pid_left);
	SetMotorRightPower(pid_right);
}

void Uint16_Constrain(uint16_t *data, const uint16_t min_value, const uint16_t max_value)
{
	if (*data > max_value)
		*data = max_value;
	else if (*data < min_value)
		*data = min_value;
}

void Int16_Constrain(int16_t *data, const int16_t min_value, const int16_t max_value)
{
	if (*data > max_value)
		*data = max_value;
	else if (*data < min_value)
		*data = min_value;
}

void Set_Racecar_Direction(uint16_t pulse)
{
	uint16_t __pulse = pulse;
	Uint16_Constrain(&__pulse, servo_limit_value_t.min_, servo_limit_value_t.max_);
	TIM1->CCR1 = __pulse;
	TIM1->CCR4 = __pulse;
}
