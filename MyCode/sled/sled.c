#include "sys.h"
#include "sled.h"


#define SDATA0  PBout(9) 
#define SDATA1  PBout(7) 
#define SDATA2  PBout(3) 
#define SDATA3  PBout(5) 
#define SDATA4  PBout(6) 
#define SDATA5  PBout(8) 
#define SDATA6  PAout(10) 
#define SDATA7  PBout(4) 

#define SELECT0  PBout(0) 
#define SELECT1  PBout(1) 
#define SELECT2  PBout(10) 
#define SELECT3  PBout(12) 
#define SELECT4  PBout(11) 
#define SELECT5  PBout(15) 
#define SELECT6  PBout(14) 
#define SELECT7  PBout(13) 

unsigned char  NumTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
//显示缓存
unsigned char DBUF[8]={0};

//写入数据到数码管端口
void SLED_Write(unsigned char dis)
{
	if(dis&0x01)
		SDATA0=1;
	else	
		SDATA0=0;
	
	if(dis&0x02)
		SDATA1=1;
	else	
		SDATA1=0;

	if(dis&0x04)
		SDATA2=1;
	else	
		SDATA2=0;	
	
	if(dis&0x08)
		SDATA3=1;
	else	
		SDATA3=0;	
	
	if(dis&0x10)
		SDATA4=1;
	else	
		SDATA4=0;	
	
	if(dis&0x20)
		SDATA5=1;
	else	
		SDATA5=0;	
	
	if(dis&0x40)
		SDATA6=1;
	else	
		SDATA6=0;	

	if(dis&0x80)
		SDATA7=1;
	else	
		SDATA7=0;	
}



void SLED_PortInit(void)
{

//PB0 1 3 4 5 6 7 8 9 10 11 12 13 14 15 
//PA10

  GPIO_InitTypeDef   GPIO_InitStructure;	
 

//PB 
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7); 

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_8| GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15); 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA,GPIO_Pin_10);   

}

void SLED_Ref(void)
{
	static unsigned char num=0;
	//全部关闭防止出现鬼影
	SELECT0=0;SELECT1=0;SELECT2=0;SELECT3=0;SELECT4=0;SELECT5=0;SELECT6=0;SELECT7=0; 
	SLED_Write(DBUF[num]); //发送数据
	switch(num)	//选择打开
	{
		case 0: SELECT0=1;SELECT1=0;SELECT2=0;SELECT3=0;SELECT4=0;SELECT5=0;SELECT6=0;SELECT7=0; break;
		case 1: SELECT0=0;SELECT1=1;SELECT2=0;SELECT3=0;SELECT4=0;SELECT5=0;SELECT6=0;SELECT7=0; break;
		case 2: SELECT0=0;SELECT1=0;SELECT2=1;SELECT3=0;SELECT4=0;SELECT5=0;SELECT6=0;SELECT7=0; break;
		case 3: SELECT0=0;SELECT1=0;SELECT2=0;SELECT3=1;SELECT4=0;SELECT5=0;SELECT6=0;SELECT7=0; break;
		case 4: SELECT0=0;SELECT1=0;SELECT2=0;SELECT3=0;SELECT4=1;SELECT5=0;SELECT6=0;SELECT7=0; break;
		case 5: SELECT0=0;SELECT1=0;SELECT2=0;SELECT3=0;SELECT4=0;SELECT5=1;SELECT6=0;SELECT7=0; break;
		case 6: SELECT0=0;SELECT1=0;SELECT2=0;SELECT3=0;SELECT4=0;SELECT5=0;SELECT6=1;SELECT7=0; break;
		case 7: SELECT0=0;SELECT1=0;SELECT2=0;SELECT3=0;SELECT4=0;SELECT5=0;SELECT6=0;SELECT7=1; break;
		default:SELECT0=0;SELECT1=0;SELECT2=0;SELECT3=0;SELECT4=0;SELECT5=0;SELECT6=0;SELECT7=0;num=0;break;
	}
	num++;
	if(num>7)
		num=0;
	
}















