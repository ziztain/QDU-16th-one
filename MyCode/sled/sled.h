#ifndef __SLED_H
#define __SLED_H

#include "sys.h"

//#define LED1 PCout(13)// PC13

//void PortInit(void);  //���ж˿ڳ�ʼ��;
void SLED_PortInit(void);
void SLED_Ref(void); 

extern unsigned char  NumTable[];
extern unsigned char DBUF[];

#endif

