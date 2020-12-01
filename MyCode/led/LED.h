#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED1 PCout(13) 
#define LED2 PCout(14) 
#define LED3 PCout(15) 
 
void PortInit(void);  //所有端口初始化;
 

#endif

