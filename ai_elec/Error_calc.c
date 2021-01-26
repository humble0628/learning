#include "Error_calc.h"

extern int real_pwm[2];
extern int stop_flag;
//------------------------------------------------------------------------------------------//
//----------------------------------------ת�򻷼���----------------------------------------//
extern int real_angle_rudder;
extern uint16 peak_max[15];
int angle_error_rudder;
int angle_error_rudder_last;
int final_angle;
float anglepid_para[3] = {1.5, 0, 0.5};   // p,i,d

int angle_pid_rudder(int targetangle)
{
		angle_error_rudder = targetangle - real_angle_rudder;		//E(n)
	  final_angle = anglepid_para[0]*angle_error_rudder + anglepid_para[2]*(angle_error_rudder - angle_error_rudder_last);
		angle_error_rudder_last = angle_error_rudder;
    return final_angle;
}

extern int real_angle_motor;
int angle_error_motor;
int angle_error_motor_last;
int final_angle_motor;
float anglepid_para_motor[3] = {2, 0, 0.5};   // p,i,d
int angle_pid_motor(int targetangle_m)
{
		angle_error_motor = targetangle_m - real_angle_motor;		//E(n)
	  final_angle_motor = anglepid_para_motor[0]*angle_error_motor + anglepid_para_motor[2]*(angle_error_motor - angle_error_motor_last);
		angle_error_motor_last = angle_error_motor;
    return final_angle_motor;
}


extern float rudder_error;
extern float motor_error;
extern int car_speed;
int delta_rudder;
int delta_motor;
void angle_calc(void)  //���ת�򻷣��ֱ�õ��˿���ת�����
{
		delta_rudder = angle_pid_rudder(rudder_error);
		delta_motor = angle_pid_motor(motor_error);
}


//------------------------------------------------------------------------------------------//
//----------------------------------------����������----------------------------------------//


//---------����������ļ���---------//
extern int rudder_pwm;

void rudder_calc(void)
{
		rudder_pwm = middle_rudder + delta_rudder;
}


//---------����������ļ���---------//
extern int motor_pwm[2]; 
int pwm_error[2];  //��ǰƫ��e(n)
int pwm_error_last[2];  //��һ�ε�ƫ��e(n-1)
int pwm_error_lastlast[2];  //���ϴε�ƫ��e(n-2)
int delta_pwm[2];  //pid�������������
int final_pwm[2];  //pid���յ����
float kp[2] = {3, 3};
float ki[2] = {0, 0};
float kd[2] = {0, 0};

int pwm_pid0(int target_pwm0)//����ʽPID�Ӻ���
{
  	pwm_error[0] = target_pwm0 - real_pwm[0];		//E(n)
    delta_pwm[0] = (int)(kp[0]*(pwm_error[0] - pwm_error_last[0]) + ki[0]*pwm_error[0] + kd[0]*(pwm_error[0] - 2*pwm_error_last[0] + pwm_error_lastlast[0]));
		final_pwm[0] += delta_pwm[0];
    pwm_error_lastlast[0] = pwm_error_last[0];			//�������ֵ
		pwm_error_last[0] = pwm_error[0];
    return final_pwm[0];
}

int pwm_pid1(int target_pwm1)//����ʽPID�Ӻ��� 
{
  	pwm_error[1] = target_pwm1 - real_pwm[1];		//E(n)
    delta_pwm[1] = (int)(kp[1]*(pwm_error[1] - pwm_error_last[1]) + ki[1]*pwm_error[1] + kd[1]*(pwm_error[1] - 2*pwm_error_last[1] + pwm_error_lastlast[1]));
		final_pwm[1] += delta_pwm[1];
		pwm_error_lastlast[1] = pwm_error_last[1];			//�������ֵ
		pwm_error_last[1] = pwm_error[1];
		return final_pwm[1];
}

extern int peak_mid_1st;
int target_pwm[2];//����������, 0Ϊ��1Ϊ��
int basic_motor;

void motor_calc(void)//����������ļ���
{
		basic_motor = 12000;
	
//		if(peak_mid_1st > 140)
//		{
//				basic_motor = 24000;
//		}
//		else
//		{
//				basic_motor = 23000;
//		}
		
		target_pwm[0] = basic_motor - (int)(delta_motor);
		target_pwm[1] = basic_motor + (int)(delta_motor);
		
		motor_pwm[0] = pwm_pid0(target_pwm[0]);
		motor_pwm[1] = pwm_pid1(target_pwm[1]);
	
}









//----------------------------------//
//----------------------------------//
extern int forced_stop;  //ǿ��ͣ����־
void error_calc(void)
{
	//  virtual_error();
		if(stop_flag > 0)
		{
				stop_flag++;
		}
		if(forced_stop)//ǿ��ͣ����pwm�������
		{
				rudder_pwm = 0;
				motor_pwm[0] = 0;
				motor_pwm[1] = 0;
			
		}
		else if(stop_flag > 100)
		{
				angle_calc();
				rudder_calc();
				motor_pwm[0] = pwm_pid0(0);
				motor_pwm[1] = pwm_pid1(0);
				rudder_pwm = middle_rudder + delta_rudder;
		}
		
		else   
		{//һ������£����������Ľ���
				angle_calc();
				rudder_calc();
				motor_calc();
		}
}