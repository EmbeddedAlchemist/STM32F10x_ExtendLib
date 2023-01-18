#ifndef _STM32F10X_EXTEND_LIB_ADC_H_
#define _STM32F10X_EXTEND_LIB_ADC_H_

#include "STM32F10x_ExtendLib.h"

typedef struct ADC_Object_TypeDef *ADC_Object;

typedef struct ADC_Channel_Object_TypeDef{
    ADC_Object ADCObj;
    GPIO_Object gpio;
    uint8_t ADC_Channel_x;
    uint8_t sampleTime;
    uint8_t index;
    CallbackFunction_TypeDef callbackFunc;
    GenericObject callbackFuncParam;
} * ADC_Channel_Object;


struct ADC_Object_TypeDef{
    ADC_TypeDef *ADCx;
    ADC_Channel_Object channel[18];//ADC通道
    uint16_t ConversionValue[18]; //ADC转换值
    uint8_t curChannel;
};

ADC_Object ADC_Initialize(ADC_TypeDef *ADCx, uint8_t IRQn_Channel, uint8_t preemptionPriority, uint8_t subPriority);
ADC_Channel_Object ADC_Channel_Initialize(ADC_Object ADCObj, uint8_t ADC_Channel_x, uint8_t ADC_SampleTime_x, GPIO_Object GPIOObj, CallbackFunction_TypeDef callback, GenericObject param);
uint16_t ADC_Channel_GetConversionValue(ADC_Channel_Object channelObj);

#endif
