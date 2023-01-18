#include "STM32F10x_ExtendLib_ADC.h"

ADC_Object ADC1Object, ADC2Object;

/**
 * @brief 初始化并生成一个ADC对象
 *
 * @param ADCx
 * @param IRQn_Channel
 * @param preemptionPriority
 * @param subPriority
 * @return ADC_Object
 */

ADC_Object ADC_Initialize(ADC_TypeDef *ADCx, uint8_t IRQn_Channel, uint8_t preemptionPriority, uint8_t subPriority) {
    switch ((intptr_t)ADCx) { //有效性验证
        case (intptr_t)ADC1:
        case (intptr_t)ADC2: {
            ADC_InitTypeDef ADCInitData;
            ADCInitData.ADC_Mode = ADC_Mode_Independent;
            ADCInitData.ADC_ScanConvMode = DISABLE;
            ADCInitData.ADC_ContinuousConvMode = DISABLE;
            ADCInitData.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
            ADCInitData.ADC_DataAlign = ADC_DataAlign_Right;
            ADCInitData.ADC_NbrOfChannel = 1;
            ADC_Init(ADCx, &ADCInitData);
        } break;
        default: {
            return EmptyObject;
        }
    }

    ADC_Object newADCObject = ExtendLib_NewObject(ADC_Object);

    switch ((intptr_t)ADCx) { //时钟配置
        case (intptr_t)ADC1: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
            ADC1Object = newADCObject;
        } break;
        case (intptr_t)ADC2: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
            ADC2Object = newADCObject;
        } break;
    }
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    // TODO:中断

    ADC_DiscModeChannelCountConfig(ADCx, 1);
    ADC_DiscModeCmd(ADCx, ENABLE); //短序列模式，每次一条通道

    newADCObject->ADCx = ADCx;
    for (int i = 0; i < 18; i++) {
        newADCObject->channel[i] = EmptyObject;
    }
    ADC_Cmd(ADCx, ENABLE);
    ADC_ResetCalibration(ADCx);
    while (ADC_GetResetCalibrationStatus(ADCx))
        ;
    ADC_StartCalibration(ADCx);
    while (ADC_GetCalibrationStatus(ADCx))
        ;
    ADC_SoftwareStartConvCmd(ADCx, ENABLE);
    return newADCObject;
}

/**
 * @brief 初始化并生成ADC通道对象
 * 【DMA方式不支持完成回调】
 *
 * @param ADCObj ADC通道所属ADC对象
 * @param ADC_Channel_x ADC通道号
 * @param GPIOObj ADC通道对应GPIO，如没有，置EmptyObject
 * @return ADC_Channel_Object
 */
ADC_Channel_Object ADC_Channel_Initialize(ADC_Object ADCObj, uint8_t ADC_Channel_x, uint8_t ADC_SampleTime_x, GPIO_Object GPIOObj, CallbackFunction_TypeDef callback, GenericObject param) {
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
    newADCChannelObj->callbackFunc = callback;
    newADCChannelObj->callbackFuncParam = param;
    newADCChannelObj->gpio = GPIOObj;
    newADCChannelObj->sampleTime = ADC_SampleTime_x;
    newADCChannelObj->index = index;
    return newADCChannelObj;
}

uint16_t ADC_Channel_GetConversionValue(ADC_Channel_Object channelObj) {
    return channelObj->ADCObj->ConversionValue[channelObj->index];
}

//=============================中断================================

void ADC1_2_IRQHandler() {
    if (ADC_GetITStatus(ADC1, ADC_IT_EOC)) {
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
        ADC1Object->ConversionValue[ADC1Object->curChannel] = ADC_GetConversionValue(ADC1);
        ADC1Object->curChannel++;
        if(ADC1Object->channel[ADC1Object->curChannel]==EmptyObject){
            ADC1Object->curChannel = 0;
        }
        //切换到下一通道
        ADC_RegularChannelConfig(ADC1, ADC1Object->channel[ADC1Object->curChannel]->ADC_Channel_x, 1, ADC1Object->channel[ADC1Object->curChannel]->sampleTime);
    }
    if (ADC_GetITStatus(ADC2, ADC_IT_EOC)) {
        ADC_ClearITPendingBit(ADC2, ADC_IT_EOC);
        ADC2Object->ConversionValue[ADC2Object->curChannel] = ADC_GetConversionValue(ADC2);
        ADC2Object->curChannel++;
        if(ADC2Object->channel[ADC2Object->curChannel]==EmptyObject){
            ADC2Object->curChannel = 0;
        }
        //切换到下一通道
        ADC_RegularChannelConfig(ADC2, ADC2Object->channel[ADC2Object->curChannel]->ADC_Channel_x, 1, ADC2Object->channel[ADC2Object->curChannel]->sampleTime);
    }
}
