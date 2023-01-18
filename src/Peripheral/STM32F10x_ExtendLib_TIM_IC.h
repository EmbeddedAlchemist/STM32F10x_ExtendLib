#ifndef _STM32F10X_EXTEND_LIB_TIM_IC_H_
#define _STM32F10X_EXTEND_LIB_TIM_IC_H_

#include "STM32F10x_ExtendLib.h"

typedef struct TIM_IC_Object_TypeDef{
    TIM_Object timer;
    uint16_t TIM_Channel_x;
    GPIO_Object gpio;
} * TIM_IC_Object;

#endif
