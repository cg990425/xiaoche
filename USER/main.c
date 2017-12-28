#include "stm32f10x.h"
#include "delay.h"
#include "motor.h"
#include "keysacn.h"
#include "IRSEARCH.h"
#include "IRAvoid.h"
#include "usart.h"
#include "UltrasonicWave.h"
#include "timer.h"
#include "Server.h"

// ������תͷ����
int front_detection()
{

//	ZYSTM32_brake(0);
	SetJointAngle(90);
	delay_ms(100);
	return UltrasonicWave_StartMeasure();
}
int left_detection()
{

//	ZYSTM32_brake(0);
	SetJointAngle(175);
	delay_ms(300);
	return UltrasonicWave_StartMeasure();
}
int right_detection()
{

//	ZYSTM32_brake(0);
	SetJointAngle(5);
	delay_ms(300);
	return UltrasonicWave_StartMeasure();
}
//extern int U_temp;
/*
void ZYSTM32_run(int speed,int time);       //ǰ������
void ZYSTM32_brake(int time);               //ɲ������
void ZYSTM32_Left(int speed,int time);      //��ת����
void ZYSTM32_Spin_Left(int speed,int time); //����ת����
void ZYSTM32_Right(int speed,int time);     //��ת����
void ZYSTM32_Spin_Right(int speed,int time);//����ת����
void ZYSTM32_back(int speed,int time);      //���˺���
*/

 int main(void)
 {	
   int Q_temp,L_temp,R_temp;
	 delay_init();
	 KEY_Init();
	 IRSearchInit();
	 IRAvoidInit();
   Timerx_Init(5000,7199);                //10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
	 UltrasonicWave_Configuration();        //�Գ�����ģ���ʼ��
	 uart_init(115200);
	 TIM4_PWM_Init(7199,0);                 //��ʼ��PWM
	 TIM5_PWM_Init(9999,143);               //����Ƶ��PWMƵ��=72*10^6/(9999+1)/(143+1)=50Hz	 
	 ZYSTM32_brake(500);
	 keysacn();		
	while(1)
	{  		
	//	Q_temp = front_detection();
	//  printf("�⵽�ľ���ֵΪ��%d\n",Q_temp);
		Q_temp = front_detection();
		if(Q_temp<60 && Q_temp>0) //��������ֵ	
		{
			ZYSTM32_brake(500);		
			ZYSTM32_back(60,500);	
			ZYSTM32_brake(1000);	
			
			L_temp=left_detection();//��������ϰ���ľ���ֵ
			delay_ms(500);
			R_temp=right_detection();//�����ұ��ϰ���ľ���ֵ
			delay_ms(500);
			
			if((L_temp < 60 ) &&( R_temp < 60 ))//��������������ϰ��￿�ıȽϽ�
			{
				ZYSTM32_Spin_Left(60,500);
			}				
      else if(L_temp > R_temp)
			{
				ZYSTM32_Left(60,700);
				ZYSTM32_brake(500);
			}	
      else
			{
				ZYSTM32_Right(60,700);
				ZYSTM32_brake(500);					
			}							
		}	
		else
		{
			ZYSTM32_run(60,10);
		}
	}
 }
