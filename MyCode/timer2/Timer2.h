#ifndef __Timer2_H
#define __Timer2_H	

#include "sys.h"
/*��ʼ����ʱ��2
��:TIM2_Int_Init(100,7199);10Khz�ļ���Ƶ�ʣ�������100Ϊ10ms 
��Ҫ��ʱΪ5ms��TIM2_Int_Init(50,7199);����
*/
void TIM2_Int_Init(u16 arr,u16 psc);

#endif
