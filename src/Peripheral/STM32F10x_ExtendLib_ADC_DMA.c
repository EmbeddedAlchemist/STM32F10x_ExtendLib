#include "STM32F10x_ExtendLib_ADC_DMA.h"

ADC_Object ADC_DMA_Initialize(ADC_TypeDef *ADCx) {
    ADC_Object newADCObj;
    switch ((intptr_t)ADCx) { //有效性验证
        case (intptr_t)ADC1: {
            newADCObj = ExtendLib_NewObject(ADC_Object);
        } break;
        default: {
            return EmptyObject;
        }
    }
    newADCObj->ADCx = ADCx;
    return newADCObj;
}

ADC_Channel_Object ADC_DMA_Channel_Initialize(ADC_Object ADCObj, uint8_t ADC_Channel_x, uint8_t ADC_SampleTime_x, GPIO_Object GPIOObj) {
    ADC_Channel_Object newADCChannelObj = ExtendLib_NewObject(ADC_Channel_Object);
    int index;
    if (GPIOObj != EmptyObject) {
        GPIO_InitializeObject(GPIOObj, GPIO_Mode_AIN, GPIO_Speed_50MHz);
    }
    for (index = 0; index < 18; index++) {
        if (ADCObj->channel[index] == EmptyObject)
            break;
    }
    ADCObj->channel[index] = newADCChannelObj;
    newADCChannelObj->ADCObj = ADCObj;
    newADCChannelObj->ADC_Channel_x = ADC_Channel_x;
    newADCChannelObj->gpio = GPIOObj;
    newADCChannelObj->sampleTime = ADC_SampleTime_x;
    newADCChannelObj->index = index;
    ADC_RegularChannelConfig(ADCObj->ADCx, ADC_Channel_x, index + 1, ADC_SampleTime_x);
    return newADCChannelObj;
}

void ADC_DMA_Start(ADC_Object ADCObj) {
    int nbr;
    for (nbr = 0; nbr < 18; nbr++) {
        if (ADCObj->channel[nbr] == EmptyObject)
            break;
    }
    DMA_InitTypeDef DMAInitData;
    DMAInitData.DMA_PeripheralBaseAddr = (uint32_t)(&(ADCObj->ADCx->DR));
    DMAInitData.DMA_MemoryBaseAddr = (uint32_t)ADCObj->ConversionValue;
    DMAInitData.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMAInitData.DMA_BufferSize = nbr + 1;
    DMAInitData.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMAInitData.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMAInitData.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMAInitData.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMAInitData.DMA_Mode = DMA_Mode_Circular;
    DMAInitData.DMA_Priority = DMA_Priority_High;
    DMAInitData.DMA_M2M = DMA_M2M_Disable;
    switch ((intptr_t)ADCObj->ADCx) { //有效性验证
        case (intptr_t)ADC1: {

            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
            DMA_DeInit(DMA1_Channel1);
            DMA_Init(DMA1_Channel1, &DMAInitData);
            DMA_Cmd(DMA1_Channel1, ENABLE);
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
        } break;
        default: {
            return;
        }
    }
    ADC_InitTypeDef ADCInitData;
    ADCInitData.ADC_Mode = ADC_Mode_Independent;
    ADCInitData.ADC_ScanConvMode = ENABLE;
    ADCInitData.ADC_ContinuousConvMode = ENABLE;
    ADCInitData.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADCInitData.ADC_DataAlign = ADC_DataAlign_Right;
    ADCInitData.ADC_NbrOfChannel = nbr + 1;
    ADC_Init(ADCObj->ADCx, &ADCInitData);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    // for(uint8_t i = 0; i < ADCObj->channelNumber; i++){
    //     ADC_RegularChannelConfig(ADCPeriph->ADCx, ADCPeriph->channels->channel, i + 1, ADCPeriph->channels->sampleTime);
    // }
    ADC_DMACmd(ADCObj->ADCx, ENABLE);
    ADC_Cmd(ADCObj->ADCx, ENABLE);
    ADC_ResetCalibration(ADCObj->ADCx);
    while (ADC_GetResetCalibrationStatus(ADCObj->ADCx))
        ;
    ADC_StartCalibration(ADCObj->ADCx);
    while (ADC_GetCalibrationStatus(ADCObj->ADCx))
        ;
    ADC_SoftwareStartConvCmd(ADCObj->ADCx, ENABLE);
}
