#ifndef _STM32F10x_EXTEND_LIB_H_
#define _STM32F10x_EXTEND_LIB_H_

//扩展库配置
#include "STM32F10x_ExtendLib_Config.h"

//扩展库定义
#include "ExtendLibDefinition.h"

//必要的组件
#include "MemoryManage.h"
#include "Event.h"

//标准固件库
#include "stm32f10x.h"

//标准库
#include "stdio.h"
#include "stddef.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"



//扩展外设库
#include "STM32F10x_ExtendLib_PWM.h"
#include "STM32F10x_ExtendLib_GPIO.h"
#include "STM32F10x_ExtendLib_Timer.h"
#include "STM32F10x_ExtendLib_USART.h"
#include "STM32F10x_ExtendLib_ADC.h"
#include "STM32F10x_ExtendLib_ADC_DMA.h"
#include "TimingAndDelay.h"

void ExternLib_Initialize(void);
void STM32F10x_ExternLib_MainLoopHandler(void);
void DistoryObject(GenericObject object);

#endif
