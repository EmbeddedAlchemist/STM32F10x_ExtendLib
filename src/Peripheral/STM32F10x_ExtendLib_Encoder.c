#include "STM32F10x_ExtendLib_Encoder.h"


Encoder_Object Encoder_Initialize(TIM_Object Timer, GPIO_Object GPIO_A, GPIO_Object GPIO_B){
    Encoder_Object newEncoderObj;
    switch((intptr_t)Timer->TIMx){
        case (intptr_t)TIM1:
        case (intptr_t)TIM2:
        case (intptr_t)TIM3:
        case (intptr_t)TIM4:
        case (intptr_t)TIM5:
        case (intptr_t)TIM8:{
            newEncoderObj = (Encoder_Object)MeM_Request(ExtendLib_GetObjectSize(Encoder_Object));
        }break;
        default:{
            return EmptyObject;
        }
    }
    
    GPIO_InitializeObject(GPIO_A, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
    GPIO_InitializeObject(GPIO_B, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
    TIM_InitializeAdvanceObject(Timer, 0xFFFF, 0x00, NULL, EmptyObject, 0, 0);
    TIM_EncoderInterfaceConfig(Timer->TIMx, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    return newEncoderObj;
}
