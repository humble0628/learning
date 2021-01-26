#include "Get_sensor.h"
/*
�Դ�������ֵ���вɼ���������
1.���ֵ��get_adc��;
2.��������get_speed��;
3.�����ǣ�get_angle��;
*/


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////��Ųɼ�//////////////////////////////////////////////////

uint16 sensor[AdcNum][SampleNum] = {0}; //���ڵ�вɼ����ź�Ϊ�����źţ����Կ��Զ�β�ֵ����һ���ź�����(sensor[][SampleNum],����SampleNumΪ�ɼ��������������趨)
uint16 peak[AdcNum] = {0};//��ȡ�������ֵ����Сֵ������õ����ֵ(peak[])
uint16 peak_max[15]= {0};
int count;
uint8 CalcMaxToMin(uint16 data[SampleNum]) //����50������ֵ�����ֵ����Сֵ�����
{
    uint16 Max = 0;
    uint16 Min = 0;
    Max = data[0];
    Min = data[0];
    
    for(uint16 i = 0; i < SampleNum; i++)
		{
         if(data[i] > Max)   Max = data[i];
         if(data[i] < Min)   Min = data[i];
    }
    return Max-Min;
} 

void get_adc(void) 
{
   for(int i=0;i < SampleNum;i++)
	 {   
				sensor[0][i] = adc_convert(ADC_1,ADC1_CH15_B26);		//AD1
			  sensor[1][i] = adc_convert(ADC_1,ADC1_CH0_B27);     //AD2
			  sensor[2][i] = adc_convert(ADC_1,ADC1_CH13_B24);    //AD3
			  sensor[3][i] = adc_convert(ADC_1,ADC1_CH14_B25);    //AD4
        sensor[4][i] = adc_convert(ADC_1,ADC1_CH11_B22);		//AD5
			  sensor[5][i] = adc_convert(ADC_1,ADC1_CH12_B23);    //AD6
			  sensor[6][i] = adc_convert(ADC_1,ADC1_CH9_B20);     //AD7
			  sensor[7][i] = adc_convert(ADC_1,ADC1_CH10_B21);    //AD8
				sensor[8][i] = adc_convert(ADC_1,ADC1_CH7_B18);     //AD9
			  sensor[9][i] = adc_convert(ADC_1,ADC1_CH8_B19);     //AD10
			  sensor[10][i] = adc_convert(ADC_1,ADC1_CH5_B16);    //AD11
			  sensor[11][i] = adc_convert(ADC_1,ADC1_CH6_B17);    //AD12
				sensor[12][i] = adc_convert(ADC_1,ADC1_CH3_B14);    //AD13
			  sensor[13][i] = adc_convert(ADC_1,ADC1_CH1_B12);    //AD14
			  sensor[14][i] = adc_convert(ADC_1,ADC1_CH4_B15);    //AD15
    }
    
    for(int i=0; i<15; i++)
		{
       peak[i] = CalcMaxToMin(sensor[i]);
    } 
		
//				if(count<5000)	
//		{
//		   for(int i=0; i<15; i++)
//		   {
//			   if(peak_max[i]<peak[i])   peak_max[i] = peak[i];
//		   }
//			 count++;
//	  }
//		if(count==5000)
//		{
//			for(int i=0; i<15; i++)
//		   {
//			   peak[i] = 158*peak[i]/(peak_max[i]*1.0);
//		   }
//		}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////�������ɼ�/////////////////////////////////////////////////

int real_pwm[2];   //�ɱ�������ȡ����ʵ�ٶ� 0Ϊ���֣�1Ϊ����
int car_speed;

void get_speed(void)
{
		//��ȡ����������ֵ
		real_pwm[0] = -200 * qtimer_quad_get(QTIMER_1,QTIMER1_TIMER2_C2 ); //������Ҫע��ڶ������������дA������
		real_pwm[1] = 200 * qtimer_quad_get(QTIMER_1,QTIMER1_TIMER0_C0);
		
		qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER0_C0);
		qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER2_C2);
	
		car_speed = (real_pwm[0] + real_pwm[1]) / 2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////�����ǲɼ�/////////////////////////////////////////////////

int real_angle;  //�������ǻ�ȡ�ĳ�ģ��ת��
int real_angle_rudder;
int real_angle_motor;
void get_angle(void)
{
	get_icm20602_gyro();
		real_angle_rudder = icm_gyro_z / 50;
	  real_angle_motor = (icm_gyro_z / 10) * 25;
}


