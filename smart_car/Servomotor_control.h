#ifndef _SERVOMOTOR_CONTROL_H
#define _SERVOMOTOR_CONTROL_H

#include "headfile.h"
#include "hardware_init.h"
#include "Misc.h"

#define MOTOR_DJ   PWM4_MODULE2_CHA_C30   //����2�����תPWM����
#define Servo_left      4300    //����:3800  ��:4300
#define Servo_mid       3900    //����:3400  ��:3900
#define Servo_right     3500    //����:3000  ��:3500

void Servomotor_control();
int inside_pid(int target_in);
int outside_pid(int target_out);

#endif