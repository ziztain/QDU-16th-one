#include "ad_driver.h"

//????
//AD??????
__IO uint16_t ADCConvertedValue[20];

//??
//???AD
void init_ad(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
 	GPIO_InitTypeDef GPIO_InitStructure;

	//---------------------??AD???--------------------
	//??DMA??
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	//??ADC1??
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	//?????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//DMA1??1??
	DMA_DeInit(DMA1_Channel1);
	//????
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS;
	//????
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCConvertedValue;
	//dma??????
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	//??DMA??????????
	DMA_InitStructure.DMA_BufferSize = 20;
	//??DMA???????,????
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//??DMA???????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//??????
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	//??????
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	//??DMA?????:?????????
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	//??DMA?????
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	//??DMA?2?memory????????
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	//????1
	DMA_Cmd(DMA1_Channel1, ENABLE);

	//ADC1??
	//??????
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	//????
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	//????
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	//??????
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	//?????
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//????????
	ADC_InitStructure.ADC_NbrOfChannel = 2;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//????????
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1 , 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2 , 2, ADC_SampleTime_239Cycles5);
	
	//??ADC1?DMA
	ADC_DMACmd(ADC1, ENABLE);
	
	//??ADC1
	ADC_Cmd(ADC1, ENABLE);
	
	//??ADC1???????  
	ADC_ResetCalibration(ADC1);
	//?????????????
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	//????
	ADC_StartCalibration(ADC1);
	//????????
	while(ADC_GetCalibrationStatus(ADC1));
	 
	//??ADC1?????
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

//??????
float voltage_charge(void)
{
 	uint8_t i = 0;
	uint16_t sum = 0;
	float v = 0;

	//??10????????
	for (i = 0;i < 10;i++)
	{
	 	sum += ADCConvertedValue[i * 2]; 
	}
	sum /= 10;
	//??????
	v = 0.002991 * sum;

	return v;
}

//??????
float voltage_discharge(void)
{
 	uint8_t i = 0;
	uint16_t sum = 0;
	float v = 0;

	//??10????????
	for (i = 0;i < 10;i++)
	{
	 	sum += ADCConvertedValue[i * 2 + 1]; 
	}
	sum /= 10;
	//??????
	v = 0.002991 * sum;

	return v;
}