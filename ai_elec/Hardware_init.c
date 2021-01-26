#include "hardware_init.h"
#include "headfile.h"

/*
    Ӳ��ϵͳ��ʼ��
*/

void hardware_init(void)
{ 
 /*********************************�����ʼ��*********************************/
		pwm_init(PWM1_MODULE3_CHB_D1, 17000, 0);    //��ת����
		pwm_init(PWM1_MODULE3_CHA_D0, 17000, 0);    //��ת����
	
		pwm_init(PWM2_MODULE3_CHB_D3, 17000, 0);    //��ת����
		pwm_init(PWM2_MODULE3_CHA_D2, 17000, 0);    //��ת����
	
/*********************************�����ʼ��*********************************/
		pwm_init(PWM1_MODULE1_CHA_D14, 50, 3550);     // left_edge=3900, middle=3500, right_edge=3100
	
/*********************************��ų�ʼ��*********************************/
    adc_init(ADC_1 , ADC1_CH15_B26 , ADC_8BIT );  //AD1  
	  adc_init(ADC_1 , ADC1_CH0_B27 , ADC_8BIT );   //AD2 
	  adc_init(ADC_1 , ADC1_CH13_B24 , ADC_8BIT );  //AD3 
	  adc_init(ADC_1 , ADC1_CH14_B25 , ADC_8BIT );  //AD4 
	  adc_init(ADC_1 , ADC1_CH11_B22 , ADC_8BIT );  //AD5 
	  adc_init(ADC_1 , ADC1_CH12_B23 , ADC_8BIT );  //AD6 
	  adc_init(ADC_1 , ADC1_CH9_B20 , ADC_8BIT );   //AD7 
	  adc_init(ADC_1 , ADC1_CH10_B21 , ADC_8BIT );  //AD8 
	  adc_init(ADC_1 , ADC1_CH7_B18 , ADC_8BIT );   //AD9 
	  adc_init(ADC_1 , ADC1_CH8_B19 , ADC_8BIT );	  //AD10 
 	  adc_init(ADC_1 , ADC1_CH5_B16 , ADC_8BIT );	  //AD11
	  adc_init(ADC_1 , ADC1_CH6_B17 , ADC_8BIT );	  //AD12
		adc_init(ADC_1 , ADC1_CH3_B14 , ADC_8BIT );	  //AD13
 	  adc_init(ADC_1 , ADC1_CH1_B12 , ADC_8BIT );	  //AD14
	  adc_init(ADC_1 , ADC1_CH4_B15 , ADC_8BIT );	  //AD15

/********************************��������ʼ��********************************/		
    qtimer_quad_init(QTIMER_1,QTIMER1_TIMER0_C0,QTIMER1_TIMER1_C1);    //C0Ϊ��ȡֵ
    qtimer_quad_init(QTIMER_1,QTIMER1_TIMER2_C2,QTIMER1_TIMER3_C24);   //C2Ϊ��ȡֵ
		
/******************************��ʼ����ʱ���ж�******************************/
    pit_init();                     //��ʼ��pit����
    pit_interrupt_ms(PIT_CH0,5);    //��ʼ��pitͨ��0 ����
	  //NVIC_SetPriority(PIT_IRQn,15);  //�����ж����ȼ� ��Χ0-15 ԽС���ȼ�Խ�� ��·PIT����һ��PIT�жϺ���


/*********************************���ڳ�ʼ��*********************************/
		seekfree_wireless_init();   //����ת����ģ��������Ŷ����� wireless.h�ļ���, Ĭ��D16,D17
//		
		simiic_init();
		icm20602_init();

	/*********************************TFT��ʼ��*********************************/

   gpio_init(C20, GPI, 0, GPIO_PIN_CONFIG);
   gpio_init(C22, GPI, 0, GPIO_PIN_CONFIG);
   gpio_init(C29, GPI, 0, GPIO_PIN_CONFIG);
   gpio_init(C27, GPI, 0, GPIO_PIN_CONFIG);
}