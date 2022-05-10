#ifndef _STM32F10X_EXTERN_LIB_PWM_H_
#define _STM32F10X_EXTERN_LIB_PWM_H_

#include "STM32F10x_ExternLib.h"

extern const uint16_t PWM_MaxDuty;

typedef struct PWM_TypeDef{
    struct TIM_PeriphTypeDef *TIM_Periph;//PWM所使用的定时器
    uint16_t TIM_Channel;//在指定计时器的第几个通道
    uint16_t duty;
} *PWM_Object;

PWM_Object PWM_Initialize(struct TIM_PeriphTypeDef * TIM_Periph, uint8_t TIM_Channel_x, GPIO_Object GPIO_Periph);
void PWM_SetDuty(PWM_Object PWM, uint16_t duty);
void PWM_SetCycle(PWM_Object PWM, unsigned int cycle);
uint16_t PWM_GetDuty(PWM_Object PWM);
uint16_t PWM_GetActualDuty(PWM_Object PWM);

#endif
