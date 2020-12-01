#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   


extern float Temp;        //实时温度

u8 DS18B20_Init(void);			//初始化DS18B20
void DS18B20_GetTemp(void);
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);		//读出一个字节
u8 DS18B20_Read_Bit(void);		//读出一个位
u8 DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Rst(void);			//复位DS18B20    

#endif















