#ifndef _STM32F10X_EXTEND_LIB_NVIC_H_
#define _STM32F10X_EXTEND_LIB_NVIC_H_
#include "STM32F10x_ExtendLib.h"

typedef enum NVIC_PriorityGroup {
    NVIC_PriorityGroup_1Pre_16Sub = NVIC_PriorityGroup_0,
    NVIC_PriorityGroup_2Pre_8Sub = NVIC_PriorityGroup_1,
    NVIC_PriorityGroup_4Pre_4Sub = NVIC_PriorityGroup_2,
    NVIC_PriorityGroup_8Pre_2Sub = NVIC_PriorityGroup_3,
    NVIC_PriorityGroup_16Pre_1Sub = NVIC_PriorityGroup_4
} NVIC_PriorityGroup;

typedef struct InterruptVector_Object_TypeDef {
    IRQn_Type IRQChannel; //指定一个中断请求通道
    uint8_t preemptionPriority; //主优先级
    uint8_t subPriority; //从优先级
    FunctionalState state;
} * InterruptVector_Object; //中断向量对象

#endif
