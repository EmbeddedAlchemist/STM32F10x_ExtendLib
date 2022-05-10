#ifndef _STM32F10X_EXTERN_LIB_TIMER_H_
#define _STM32F10X_EXTERN_LIB_TIMER_H_

#include "STM32F10x_ExternLib.h"

typedef struct TIM_PeriphTypeDef{
    TIM_TypeDef *TIMx;
    uint8_t IRQn_Channel;
    void (*interruptUpdateCallback)(GenericObject);//回调函数
    void *interruptUpdateCallbackParam;//回调函数参数
} *TIM_Object;



//默认的预分频数值,7199针对100微秒的整数倍表现优秀
#define TIM_PERIOD_DEFAULT ((uint16_t)7199)



void TIM_Deinitialize(TIM_Object TIM_Periph);
TIM_Object TIM_InitializeAdvance(TIM_TypeDef *TIMx, uint16_t period, uint16_t prescaler, void (*interruptUpdateCallback)(GenericObject), GenericObject interruptUpdateCallbackParam, uint8_t IRQnChannel, uint8_t preemptionPriority, uint8_t subPriority);
TIM_Object TIM_Initialize(TIM_TypeDef *TIMx, unsigned int interval, void (*interruptUpdateCallback)(GenericObject), GenericObject interruptUpdateCallbackParam, uint8_t IRQnChannel, uint8_t preemptionPriority, uint8_t subPriority);
TIM_Object TIM_GenerateUninitialize(TIM_TypeDef *TIMx, uint8_t IRQn_Channel);
void TIM_InitializeObject(TIM_Object TIM_Periph, unsigned int interval, void (*interruptUpdateCallback)(GenericObject), GenericObject interruptUpdateCallbackParam, uint8_t preemptionPriority, uint8_t subPriority);
void TIM_InitializeAdvanceObject(TIM_Object TIM_Periph, uint16_t period, uint16_t prescaler, void (*interruptUpdateCallback)(GenericObject), GenericObject interruptUpdateCallbackParam, uint8_t preemptionPriority, uint8_t subPriority);
uint16_t TIM_GetReloadValue(TIM_Object TIM_Periph);
uint16_t TIM_GetPrescalerValue(TIM_Object TIM_Periph);
void TIM_SetReloadValue(TIM_Object TIM_Periph, uint16_t reloadValue);
void TIM_SetPrescalerValue(TIM_Object TIM_Periph, uint16_t prescalerValue);
void TIM_SetInterval(TIM_Object TIM_Periph, unsigned int interval);
void TIM_SetIntervalByReload(TIM_Object TIM_Periph, unsigned int interval);
#endif
