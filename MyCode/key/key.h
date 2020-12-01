#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

#define KEY_IDLE      0
#define KEY_ENSURE 	  1
#define KEY_PRESS     2
#define KEY_WAIT_UP   3


void KEY_PortInit(void);

extern  unsigned char KeyVal;
extern  unsigned char KeyTime;
void KEY_Scan(void);	

extern unsigned char DOUBLE_EN;


#endif




