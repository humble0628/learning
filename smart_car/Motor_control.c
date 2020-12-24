#include "Motor_control.h"
#include "UI.h"

/******************************************************************************
* ���PID
*******************************************************************************/
int speed_L = 0;     //��������ֵ
int speed_R = 0;     //��������ֵ
int piderror[2] = {0};//��ǰƫ��e(n)
int piderror_last[2] = {0};//��һ�ε�ƫ��e(n-1)
int piderror_lastlast[2] = {0};//���ϴε�ƫ��e(n-2)
int delta_speed[2] = {0};    //pid�������������
float kp[2] = {5.0,5.0};  
float ki[2] = {0,0};
float kd[2] = {2.5,2.5};
int final_speed[2];

int PID1(int target_speed0)//����ʽPID�Ӻ���
{
  	piderror[0] = target_speed0 - speed_L;		//E(n)
        delta_speed[0] = (int)(kp[0]*(piderror[0] - piderror_last[0]) + ki[0]*piderror[0] + kd[0]*(piderror[0] - 2*piderror_last[0]+piderror_lastlast[0]));
	final_speed[0] += delta_speed[0];
        piderror_lastlast[0] = piderror_last[0];			//�������ֵ
	piderror_last[0] = piderror[0];
        return final_speed[0];
}

int PID2(int target_speed1)//����ʽPID�Ӻ��� 
{
  	piderror[1] = target_speed1 - speed_R;		//E(n)
        delta_speed[1] = (int)(kp[1]*(piderror[1] - piderror_last[1]) + ki[1]*piderror[1] + kd[1]*(piderror[1] - 2*piderror_last[1]+piderror_lastlast[1]));
	final_speed[1] += delta_speed[1];
        piderror_lastlast[1] = piderror_last[1];			//�������ֵ
	piderror_last[1] = piderror[1];
        return final_speed[1];
}

/******************************************************************************
* ����PID
*******************************************************************************/
int motor_piderror[2];          //��ǰƫ��e(n)
int motor_piderror_last[2];     //��һ�ε�ƫ��e(n-1)
float motor_kp[2] = {3.0, 3.0}; //3 �� 0 �� 5 , 0 , 0 , 0
float motor_ki[2] = {0, 0};
float motor_kd[2] = {0.0, 4.0};
int motor_final_speed[2];
float p_motor_gyro = 0.5;
int motor_inside_pid(int motor_target_in)
{
    motor_piderror[0] = motor_target_in; //E(n)
    motor_final_speed[0] = (int)(motor_kp[0] * (motor_piderror[0]) + motor_kd[0] * (motor_piderror[0] - motor_piderror_last[0]));
    motor_piderror_last[0] = motor_piderror[0]; //�������ֵ
    return motor_final_speed[0];
}
int motor_outside_pid(int motor_target_out)
{
    motor_piderror[1] = motor_target_out + p_motor_gyro * icm_gyro_z; //E(n)
    motor_final_speed[1] = (int)(motor_kp[1] * motor_piderror[1]) + motor_kd[1] * (motor_piderror[1] - motor_piderror_last[1]);
    motor_piderror_last[1] = motor_piderror[1];
    return motor_final_speed[1];
}

/******************************************************************************
* ͣ��PID
*******************************************************************************/
int car_speed = 0;
int stop_error = 0;
int stop_final = 0;
int stop_last = 0;
float stop_pid[3] = {3 , 0 , 0.0};
int PID_STOP(int stop_target_speed){
        car_speed = (speed_L + speed_R) / 2;
        stop_error = stop_target_speed - car_speed;
        stop_final = (float)(stop_pid[0]*stop_error) + stop_pid[2]*(stop_error - stop_last);
        stop_last = stop_error;
        return stop_final;
}
/******************************************************************************
* �޷�����
*******************************************************************************/
float Range_Limit(float value, float range)
{
      range = fabs(range);
    if(fabs(value) > range){           //΢�����޷�
      if(value > 0) value = range;
      else value = -range;
    }
     return value; 
  
}

/******************************************************************************
* ����������
*******************************************************************************/
int32 speed1_power;     //�����ٶ�
int32 speed2_power;     //�ҵ���ٶ�
int     base_speed = 20000; //��������ٶ�Ϊ10000
float   mid_error_last = 0;
float   turn_error = 0;
float   speed_down = 0.5;
extern int start;
extern int times;
extern int hill;
extern int have_circle;
extern int circle;      /* ������־λ */
extern int in_out;      /* ����������־λ */
extern int have_park;
extern int stop_flag;
extern int right_num;    /* �ұ߽������� */
extern int left_num;     /* ��߽������� */
extern int crossroad;
extern float mid_error;
extern float predict_mid_error;
extern float speed_up_error;
void speed_set()
{
        
        if(stop_flag == 0){
          
            if(fabs(speed_up_error) < 30 && right_num > 50 && left_num > 50){base_speed = 28000;}
            else if(have_circle == 1 && in_out == 0){base_speed = flash_speed;}
            else if(have_circle == 1 && in_out == 1){base_speed = flash_speed + 1000;}
            else{base_speed = flash_speed;}
            
            turn_error = 2 * motor_outside_pid(motor_inside_pid(mid_error));
            speed_down  = 0.5 + ((float)fabs(speed_up_error) / 1000);
            if(speed_down > 0.8){speed_down = 0.8;}
            
            if(turn_error >= 0){
                speed1_power = PID1(base_speed + (1 - speed_down) * turn_error);
                speed2_power = PID2(base_speed - speed_down * turn_error);
            }
            else{
                speed1_power = PID1(base_speed + speed_down * turn_error);
                speed2_power = PID2(base_speed - (1 - speed_down) * turn_error);
            }
        }
        if (stop_flag == 1){
          if(have_park == 1){
              speed1_power = PID_STOP(-10000);
              speed2_power = PID_STOP(-10000);
              if(fabs((speed_L + speed_R) / 2) < 5000){
                  speed1_power = PID_STOP(0);
                  speed2_power = PID_STOP(0);
              }
          }
          else{
              speed1_power = PID_STOP(-800);
              speed2_power = PID_STOP(-800);
          }
        }
        
        if(times <= 200 && start == 1){
              speed1_power = Range_Limit(speed1_power , 49500);
              speed2_power = Range_Limit(speed2_power , 49500);
        }
        else{
              speed1_power = Range_Limit(speed1_power , 40000);
              speed2_power = Range_Limit(speed2_power , 40000);
        }
        
        mid_error_last = mid_error;
        
}

void motor_output()
{
        speed_set();  //�ú���Ϊ���ֳ�ת�򻷣����ֳ��ٶȻ�����������ǰ���á�
          
        //��ο��Ƶ��������ת
        //ÿ������������������źţ���ʵ���ǿ��������������źŵ�ռ�ձȣ����ߵ�ƽʱ�䣩
        //���籾������ʹ�ö�ʱ��1��0��1ͨ����������������0ռ�ձ�Ϊ50%��1ͨ��Ϊ%0,��ô�����ת
        //����0ռ�ձ�Ϊ0%��1ͨ��Ϊ%50,��ô�����ת�������Ϊ0������ֹͣ
        
        if(0<=speed1_power) //���1   ��ת ����ռ�ձ�Ϊ �ٷ�֮ (1000/TIMER1_PWM_DUTY_MAX*100)
        {
            pwm_duty(MOTOR1_A, speed1_power);
            pwm_duty(MOTOR1_B, 0);
        }
        else                //���1   ��ת
        {
            pwm_duty(MOTOR1_A, 0);
            pwm_duty(MOTOR1_B, -speed1_power);
        }
        
        if(0<=speed2_power) //���2   ��ת
        {
            pwm_duty(MOTOR2_A, speed2_power);
            pwm_duty(MOTOR2_B, 0);
        }
        else                //���2   ��ת
        {
            pwm_duty(MOTOR2_A, 0);
            pwm_duty(MOTOR2_B, -speed2_power);
        }
}