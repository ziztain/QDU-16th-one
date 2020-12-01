#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h"
#include "led.h"
#include "timer.h"     
#include "string.h"
#include "key.h"
#include "sled.h"
#include "ds18b20.h"
#include "timer_pwm.h"

 


void ReleaseJTAG_Pin(void);  //释放与JTAG调试相关的引脚 
void PageDisplay(void);
void PageInit(void);

unsigned char Mode=1;
unsigned int Time=0;
unsigned int T2MS=0;

//0xBF:-
//0xC6:C

void PWM_Set(void)
{
	if(Time>0)
	{
		if(Mode==1)
			PWM0_Duty=20;
		else if(Mode==2)
			PWM0_Duty=30;
		else if(Mode==3)
			PWM0_Duty=70;
	}
	else
	{
		PWM0_Duty=0;
	}
}

void TimeCount(void)
{
	if(Time>0)
	{
		T2MS++;
		if(T2MS>=500)
		{
			T2MS=0;
			Time--;
		}
	}	
}


u8 Page=0; //0显示模式和时间   1显示温度

void Control(void)
{
	static unsigned char con=0;
	
	if(KeyVal=='2')
	{
		KeyVal=0;
		con++;
		if(con==1)
			Time=60;
		else if(con==2)
			Time=120;
		else Time=0;
		if(con>2)
			con=0;
		T2MS=0;
	}
	
	if(KeyVal=='3')
	{
		KeyVal=0;
		con=0;
		Time=0;
		T2MS=0;
	}
	
	if(KeyVal=='1')
	{
		KeyVal=0;
		Mode++;
		if(Mode>3)
			Mode=1;
	}
	
	if(Time>0)
	{
		if(Mode==1)
		{
			LED1=0;LED2=1;LED3=1;
		}
		else if(Mode==2)
		{
			LED1=1;LED2=0;LED3=1;
		}
		else if(Mode==3)
		{
			LED1=1;LED2=1;LED3=0;
		}		
	}
	else
	{
		LED1=1;LED2=1;LED3=1;
	}
	
	
}



void PageDisplay(void)
{
	if(KeyVal=='4')
	{
		KeyVal=0;
		Page++;
		if(Page>1)
			Page=0;
	}	
	switch(Page)
	{
/*---------------------------------------------*/
		case 0:  //- mode -  灭 00 time
		{
			DBUF[0]=0xBF;
			DBUF[1]=NumTable[Mode];
			DBUF[2]=0xBF;
			DBUF[3]=0xFF; //灭
			DBUF[4]=NumTable[Time/1000];      //时间
			DBUF[5]=NumTable[Time%1000/100];  //时间  
			DBUF[6]=NumTable[Time%100/10];    //时间
			DBUF[7]=NumTable[Time%10];        //时间
		}break;
/*---------------------------------------------*/
		case 1:	 
		{
			DBUF[0]=0xBF;
			DBUF[1]=NumTable[4];
			DBUF[2]=0xBF;
			DBUF[3]=0xFF; //灭
			DBUF[4]=0xFF; //灭
			DBUF[5]=NumTable[(unsigned int)Temp/10];  //时间  
			DBUF[6]=NumTable[(unsigned int)Temp%10];    //时间
			DBUF[7]=0xC6;        //时间
		}break;	
/*---------------------------------------------*/
			
		default:	Page=0;	break;
	}
	
}
 

int main(void)
{		
	u8 i=0;
	delay_init();
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级//
	ReleaseJTAG_Pin();
	KEY_PortInit();
	PortInit();
	TIM2_Int_Init(20-1,7199);  //2MS定时器
	SLED_PortInit();
	PWM_Init();
	while(1)
	{ 
		PWM_Set();
		Control();	
		DS18B20_GetTemp();
		PageDisplay();
		delay_ms(50);  	 
	}
}                              

void ReleaseJTAG_Pin(void)  //释放与JTAG调试相关的引脚
{
  GPIO_InitTypeDef   GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA 
												| RCC_APB2Periph_GPIOB 
												|	RCC_APB2Periph_GPIOC 
												| RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
































