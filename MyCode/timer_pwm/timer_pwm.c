#include "timer_pwm.h"
#include "timer.h" 
#include "sys.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

//TIM3_Int_Init(2,3600);

#include "stm32f10x.h"

unsigned char PWM0_Duty=0;  //pwm0 占空比
unsigned char PWM_Cycle=0;

#define PWM0_ON       GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define PWM0_OFF      GPIO_SetBits(GPIOA,GPIO_Pin_2)  

//#define LED1 PAout(3) 

#define STEST PAout(3) 

//需要定时器进行初始化
void PWM_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;	
	
//	TIM3_Int_Init(1,360);  //定时器初始化
	TIM3_Int_Init(1,1800);  //定时器初始化	
/*------------------PWM引脚初始化-------------------------------------------*/	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_2 | GPIO_Pin_3); 
	
}


void PWM_Control(void)
{
	PWM_Cycle++;
	
	if(PWM_Cycle==PWM0_Duty)
		PWM0_OFF;
	
	if(PWM_Cycle>=100)
	{
		PWM_Cycle=0;
		if(PWM0_Duty>0)
			PWM0_ON;
		else
			PWM0_OFF;
	}

}

void TIM3_IRQHandler(void)    
{
 
	if (TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)  
	{ 
/*--------------------------------------------------*/
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  
		PWM_Control();
	}
}















