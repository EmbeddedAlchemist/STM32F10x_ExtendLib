#ifndef _STM32F10X_EXTEND_LIB_ADC_DMA_H_
#define _STM32F10X_EXTEND_LIB_ADC_DMA_H_

#include "STM32F10x_ExtendLib.h"
#include "STM32F10x_ExtendLib_ADC.h"


typedef struct ADC_Object_TypeDef *ADC_Object;

typedef struct ADC_Channel_Object_TypeDef *ADC_Channel_Object;

ADC_Object ADC_DMA_Initialize(ADC_TypeDef *ADCx); 
ADC_Channel_Object ADC_DMA_Channel_Initialize(ADC_Object ADCObj, uint8_t ADC_Channel_x, uint8_t ADC_SampleTime_x, GPIO_Object GPIOObj);
void ADC_DMA_Start(ADC_Object ADCObj);

#endif
