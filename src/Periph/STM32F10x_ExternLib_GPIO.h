#ifndef _STM32F10X_EXTERN_LIB_GPIO_H_
#define _STM32F10X_EXTERN_LIB_GPIO_H_
#include "stm32f10x.h"

typedef struct GPIO_PeriphTypeDef{
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin_x;
} * GPIO_Object;

GPIO_Object GPIO_Initialize(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin_x, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed);
GPIO_Object GPIO_GenerateUninitialize(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin_x);
void GPIO_InitializeObject(GPIO_Object GPIO_Periph, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed);
void GPIO_WritePin(GPIO_Object GPIO_Periph, unsigned int val);
uint8_t GPIO_ReadPinInput(GPIO_Object GPIO_Periph);
uint8_t GPIO_ReadPinOutPut(GPIO_Object GPIO_Periph);
#endif
