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

// 超声波转头函数
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
void ZYSTM32_run(int speed,int time);       //前进函数
void ZYSTM32_brake(int time);               //刹车函数
void ZYSTM32_Left(int speed,int time);      //左转函数
void ZYSTM32_Spin_Left(int speed,int time); //左旋转函数
void ZYSTM32_Right(int speed,int time);     //右转函数
void ZYSTM32_Spin_Right(int speed,int time);//右旋转函数
void ZYSTM32_back(int speed,int time);      //后退函数
*/

 int main(void)
 {	
   int Q_temp,L_temp,R_temp;
	 delay_init();
	 KEY_Init();
	 IRSearchInit();
	 IRAvoidInit();
   Timerx_Init(5000,7199);                //10Khz的计数频率，计数到5000为500ms 
	 UltrasonicWave_Configuration();        //对超声波模块初始化
	 uart_init(115200);
	 TIM4_PWM_Init(7199,0);                 //初始化PWM
	 TIM5_PWM_Init(9999,143);               //不分频，PWM频率=72*10^6/(9999+1)/(143+1)=50Hz	 
	 ZYSTM32_brake(500);
	 keysacn();		
	while(1)
	{  		
	//	Q_temp = front_detection();
	//  printf("测到的距离值为：%d\n",Q_temp);
		Q_temp = front_detection();
		if(Q_temp<60 && Q_temp>0) //测量距离值	
		{
			ZYSTM32_brake(500);		
			ZYSTM32_back(60,500);	
			ZYSTM32_brake(1000);	
			
			L_temp=left_detection();//测量左边障碍物的距离值
			delay_ms(500);
			R_temp=right_detection();//测量右边障碍物的距离值
			delay_ms(500);
			
			if((L_temp < 60 ) &&( R_temp < 60 ))//当左右两侧均有障碍物靠的比较近
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
