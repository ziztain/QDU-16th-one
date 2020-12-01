#include "key.h"



unsigned char KeyVal = 0;
unsigned char KeyState = KEY_IDLE;
unsigned char KeyTime = 1;  
unsigned char DOUBLE_EN = 0;


//-----------------------------------6个按键驱动程序------------------------------------ 
#define KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define KEY2  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define KEY3  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define KEY4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)


void KEY_PortInit(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
}

void KEY_Scan(void)	 //20-50MS执行一次
{
  switch(KeyState)
  {
	case KEY_IDLE:	 //等待按键按下
		{
		  if(KEY1==0||KEY2==0||KEY3==0||KEY4==0)
		    KeyState=KEY_ENSURE;
		  else
		    KeyState=KEY_IDLE;
		}break;
	case KEY_ENSURE:  //确认按下
		{
		  if(KEY1==0||KEY2==0||KEY3==0||KEY4==0)
		    KeyState=KEY_PRESS;
		  else
		    KeyState=KEY_IDLE;	
		}break;
	case KEY_PRESS:		 
		{
		  if(KEY1==0)
		    KeyVal = '1';
		  else if(KEY2==0)
		    KeyVal = '2';
		  else if(KEY3==0)
		    KeyVal = '3';
		  else if(KEY4==0)
		    KeyVal = '4';
		  else 
		    KeyState=KEY_IDLE;	 //消除抖动
		  
		  if(KeyState==KEY_PRESS)   
		  {
		    KeyState=KEY_WAIT_UP;
		  }
//*----------------------------------------------------------*/
		}break;
	case KEY_WAIT_UP:    //等待抬起
	   {
		 if(KeyTime)	 //连按计时 
		 {
		   KeyTime++;
		   if(KeyTime>=5)
		   {
		     KeyTime = 0;
			 KeyState=KEY_IDLE;
		   }
		 }
		 else	
		 {
		   if(KEY1 && KEY2 && KEY3 && KEY4)
		     KeyState=KEY_IDLE; 
		   else
		     KeyState=KEY_WAIT_UP;  
		 }
	   }break;
	default:
	   {
	     KeyState=KEY_IDLE;
	   }break;
  }
}































