#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   


extern float Temp;        //ʵʱ�¶�

u8 DS18B20_Init(void);			//��ʼ��DS18B20
void DS18B20_GetTemp(void);
void DS18B20_Start(void);		//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);		//����һ���ֽ�
u8 DS18B20_Read_Bit(void);		//����һ��λ
u8 DS18B20_Check(void);			//����Ƿ����DS18B20
void DS18B20_Rst(void);			//��λDS18B20    

#endif














