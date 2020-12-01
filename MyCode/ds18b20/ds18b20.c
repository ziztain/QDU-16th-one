#include "ds18b20.h"
#include "delay.h"	


 
////DS18B20------------>PA0

#define	 DS18B20_DQ_IN  	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)  //���ݶ˿�
#define  DS18B20_DQ_1  		GPIO_SetBits(GPIOA,GPIO_Pin_0) 
#define  DS18B20_DQ_0   	GPIO_ResetBits(GPIOA,GPIO_Pin_0)

float Temp=0;        //ʵʱ�¶�

//��������
void DS18B20_DIR_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void DS18B20_DIR_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void DS18B20_Rst(void)	   
{                 
	DS18B20_DIR_OUT();  //���ģʽ     //DS18B20_IO_OUT(); //SET PA0 OUTPUT
	DS18B20_DQ_0;       //DS18B20_DQ_OUT=0; //����DQ
	delay_us(750);      //����750us
	DS18B20_DQ_1;       //DS18B20_DQ_OUT=1; //DQ=1 
	delay_us(15);       //15US
}

//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;

	DS18B20_DIR_IN();   //����ģʽ         DS18B20_IO_IN();//SET PA0 INPUT	 
	while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)
		return 1;
	else
		retry=0;
	while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)
		return 1;	    
	
	return 0;
}


//��DS18B20��ȡһ��λ
//����ֵ��1/0
u8 DS18B20_Read_Bit(void) 			 // read one bit
{
	u8 data;
	
	DS18B20_DIR_OUT();   //���ģʽ  DS18B20_IO_OUT();//SET PA0 OUTPUT
	DS18B20_DQ_0;        //DS18B20_DQ_OUT=0; 
	delay_us(2);
	DS18B20_DQ_1;        //DS18B20_DQ_OUT=1; 
	DS18B20_DIR_IN();   //DS18B20_IO_IN();//SET PA0 INPUT
	delay_us(12);
	if(DS18B20_DQ_IN)
		data=1;
	else 
		data=0;	   
	delay_us(50);           
	return data;
}




//��DS18B20��ȡһ���ֽ�
//����ֵ������������
u8 DS18B20_Read_Byte(void)    // read one byte
{        
	u8 i,j,dat;
	
	dat=0;
	for (i=1;i<=8;i++) 
	{
		j=DS18B20_Read_Bit();
		dat=(j<<7)|(dat>>1);
	}						    
	return dat;
}


//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
void DS18B20_Write_Byte(u8 dat)     
{             
	u8 j;
	u8 testb;
	
	DS18B20_DIR_OUT();  //DS18B20_IO_OUT();//SET PA0 OUTPUT;
	for (j=1;j<=8;j++) 
	{
		testb=dat&0x01;
		dat=dat>>1;
		if (testb) 
		{
			DS18B20_DQ_0; //DS18B20_DQ_OUT=0;// Write 1
			delay_us(2);                            
			DS18B20_DQ_1; //DS18B20_DQ_OUT=1;
			delay_us(60);             
		}
		else 
		{
			DS18B20_DQ_0;//DS18B20_DQ_OUT=0;// Write 0
			delay_us(60);             
			DS18B20_DQ_1;//DS18B20_DQ_OUT=1;
			delay_us(2);                          
		}
	}
}
//��ʼ�¶�ת��
void DS18B20_Start(void)// ds1820 start convert
{   						               
	DS18B20_Rst();	   
	DS18B20_Check();	 
	DS18B20_Write_Byte(0xcc);// skip rom
	DS18B20_Write_Byte(0x44);// convert
} 
//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����    	 
u8 DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PORTA��ʱ�� 
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//PORTA0 �������
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_0);    //���1

	DS18B20_Rst();
	return DS18B20_Check();
}  


void DS18B20_GetTemp(void)
{
	u8 tempL,tempH;
	static unsigned char con=20;
	float stemp=0;
	con++;
	if(con>=10)
	{
		con=0;

		DS18B20_Start ();  
		DS18B20_Rst();
		DS18B20_Check();	 
		DS18B20_Write_Byte(0xcc);// skip rom
		DS18B20_Write_Byte(0xbe);// convert	    
		tempL=DS18B20_Read_Byte(); // LSB   
		tempH=DS18B20_Read_Byte(); // MSB 
		if((tempH & 0xf0) == 0x00)	 //??????
		{
			stemp =  16*tempH + 0.0625*tempL;
		}
		else 
		{
			tempH = ~tempH;
			tempL = ~tempL+0x01;    
			stemp = (-1)*(16*tempH + 0.0625*tempL);
		}
		if(stemp>0 && stemp<50)
			Temp=stemp;
	}

}







//unsigned char AlarmTemp[2]={0};

//void TempSet(void)
//{
///*-------------------------------------------------------*/
//	if(KeyVal=='1')
//	{
//		KeyVal=0;
//		if(AlarmTemp[0]<100)
//			AlarmTemp[0]++;
//		WrToROM(AlarmTemp,0,2);
//	}
///*-------------------------------------------------------*/
//	if(KeyVal=='2')
//	{
//		KeyVal=0;
//		if(AlarmTemp[0]>(AlarmTemp[1]+2))
//			AlarmTemp[0]--;
//		WrToROM(AlarmTemp,0,2);
//	}
//	
///*-------------------------------------------------------*/
//	if(KeyVal=='3')
//	{
//		KeyVal=0;
//		if((AlarmTemp[1]+2)<AlarmTemp[0])
//			AlarmTemp[1]++;
//		WrToROM(AlarmTemp,0,2);
//	}
///*-------------------------------------------------------*/
//	if(KeyVal=='4')
//	{
//		KeyVal=0;
//		if(AlarmTemp[1]>0)
//			AlarmTemp[1]--;
//		WrToROM(AlarmTemp,0,2);
//	}
//	
//}

















 
