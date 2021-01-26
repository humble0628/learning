#include "Output.h"

//////////////////////////////
///////////������///////////
int rudder_pwm = 0;

void rudder_output(void)
{
	
		//�޷����
		if(rudder_pwm > left_rudder_edge)rudder_pwm = left_rudder_edge;
		if(rudder_pwm < right_rudder_edge)rudder_pwm = right_rudder_edge;
	
		pwm_duty(PWM1_MODULE1_CHA_D14, rudder_pwm);
		
}



//////////////////////////////
///////////������///////////
int motor_pwm[2];   //��������0Ϊ��1Ϊ��

void motor_output(void)
{
	
		//�޷����
		if(motor_pwm[0] > motor_pwm_max)motor_pwm[0] = motor_pwm_max;
		if(motor_pwm[0] < motor_pwm_min)motor_pwm[0] = motor_pwm_min;
		if(motor_pwm[1] > motor_pwm_max)motor_pwm[1] = motor_pwm_max;
		if(motor_pwm[1] < motor_pwm_min)motor_pwm[1] = motor_pwm_min;
		
		if(motor_pwm[1] >= 0)
		{
				pwm_duty(PWM2_MODULE3_CHA_D2, 0);
				pwm_duty(PWM2_MODULE3_CHB_D3, motor_pwm[1]);		
		}
		else
		{
				pwm_duty(PWM2_MODULE3_CHA_D2, -motor_pwm[1]);
				pwm_duty(PWM2_MODULE3_CHB_D3, 0);
		}

		if(motor_pwm[0] >= 0)
		{
				pwm_duty(PWM1_MODULE3_CHA_D0, 0);
				pwm_duty(PWM1_MODULE3_CHB_D1, motor_pwm[0]);
		}
		else
		{
				pwm_duty(PWM1_MODULE3_CHA_D0, -motor_pwm[0]);
				pwm_duty(PWM1_MODULE3_CHB_D1, 0);
		}
		



}



//////////////////////////////
///////////�ϲ����///////////
void output(void)
{
		rudder_output();
	
		motor_output();
	//	pwm_duty(PWM1_MODULE3_CHB_D1, 10000);
}