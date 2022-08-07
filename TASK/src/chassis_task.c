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
static const float rosmsg_get_max_servo_angle = 90.0f; //从ros中获取的最大舵机角度
static const Servo_Construction_Value_t servo_limit_value_t =
{
	.max_ = 1830, //最左转
	.middle = 1260, //1000
	.min_ = 690 //最右转
};

static void Chassis_Thread(void *param)
{
	rc_data_pt = Get_Rc_Data();
	ros_msg = Get_Ros_Message();
	static uint16_t servo_pulse; ///< 舵机pwm输出值
	static float true_motors_speed_target[2]; ///< 左右两轮目标速度
	rt_thread_delay(2000);
	double servo_interval_value = servo_limit_value_t.max_ - servo_limit_value_t.min_;

	for (;;)
	{
		// 遥控器输入
		if(Rc_Valid_Status())
		{
			servo_pulse = (int16_t)servo_limit_value_t.middle - (int16_t)(Joystick_Raw_To_Normal_Data(rc_data_pt->ch1)/1.4f);
			true_motors_speed_target[0] = Joystick_Raw_To_Normal_Data(rc_data_pt->ch3) / 1.45678f;
			true_motors_speed_target[1] = Joystick_Raw_To_Normal_Data(rc_data_pt->ch3) / 1.45678f;
		}

		// ROS 上位机控制
		else if(Get_Ros_Message_Status())
		{
			servo_pulse = (int16_t)servo_limit_value_t.middle 
							+ ( (ros_msg->data.angle > 0) \
							? (ros_msg->data.angle * (servo_limit_value_t.max_ - servo_limit_value_t.middle) / rosmsg_get_max_servo_angle) \
							: (ros_msg->data.angle * (servo_limit_value_t.middle - servo_limit_value_t.min_) / rosmsg_get_max_servo_angle) );
			
			// 2040 脉冲 / 圈
			// 0.375m / 圈
			// (0.375 / 2040) m/脉冲
			// (2040/ 0.375) 脉冲/米 x1
			// ros_msg->data.speed(m/s)
			// ros_msg->data.speed * x1 = (脉冲/s) x2
			// x2 * 70ms / 1000ms = 输出的速度
			true_motors_speed_target[0] = ros_msg->data.speed * (2040 / 0.375) * 70.0f / 1000.0f;
			true_motors_speed_target[1] = ros_msg->data.speed * (2040 / 0.375) * 70.0f / 1000.0f;

			// 左右轮差速
			#define Differential_Parameters 0.56789f
			if(ros_msg->data.angle>0) ///< 左转，左轮速度减小
			{
				if(true_motors_speed_target[0] > 0)
				{
					true_motors_speed_target[0] = true_motors_speed_target[0] 
													* (1 - Differential_Parameters * (ros_msg->data.angle / rosmsg_get_max_servo_angle));
				}
			}
			else ///< 右转，右轮速度减小
			{
				if(true_motors_speed_target[1] > 0)
				{
					true_motors_speed_target[1] = true_motors_speed_target[1] 
													* (1 + Differential_Parameters * (ros_msg->data.angle / rosmsg_get_max_servo_angle));
				}
			}
			#undef Differential_Parameters
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

		Float_Constrain(&true_motors_speed_target[0], -456.0, 456.0);
		Float_Constrain(&true_motors_speed_target[1], -456.0, 456.0);
		
		// 通过PID控制电机速度
		Set_Chassis_Motor_Speed(true_motors_speed_target[0],
								true_motors_speed_target[1]);

		rt_thread_delay(20);
	}
}

Pid_Position_t motor_left_speed_pid = NEW_POSITION_PID(20.12f, 5.56f, 0.1, 1567, 7200, 0, 1000, 500);
Pid_Position_t motor_right_speed_pid = NEW_POSITION_PID(20.12f, 5.56f, 0.1, 1567, 7200, 0, 1000, 500);
void Set_Chassis_Motor_Speed(float left_motor_speed, float right_motor_speed)
{
	// motor_left_speed_pid.kp = easy_pid_p;
	// motor_left_speed_pid.ki = easy_pid_i;
	// motor_left_speed_pid.kd = easy_pid_d;
	// motor_right_speed_pid.kp = easy_pid_p;
	// motor_right_speed_pid.ki = easy_pid_i;
	// motor_right_speed_pid.kd = easy_pid_d;
	// motor_left_speed_pid.max_err_integral = easy_pid2_p;
	// motor_right_speed_pid.max_err_integral = easy_pid2_p;
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

void Float_Constrain(float *data, const float min_value, const float max_value)
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
