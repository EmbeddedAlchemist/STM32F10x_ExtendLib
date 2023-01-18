#include "STM32F10x_ExtendLib.h"

typedef struct Encoder_Object_TypeDef{
    TIM_Object Timer;
    GPIO_Object GPIO_A;
    GPIO_Object GPIO_B;
} * Encoder_Object;


