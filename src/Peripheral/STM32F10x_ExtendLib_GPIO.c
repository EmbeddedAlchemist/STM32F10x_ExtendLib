#include "STM32F10x_ExtendLib_GPIO.h"
#include "ExtendLibDefinition.h"
#include "MemoryManage.h"
/**
 * @brief 初始化GPIO端口
 * 
 * @param GPIOx 指定一个GPIO组，例如：GPIOA
 * @param GPIO_Pin_x 指定【一个】GPIO端口，例如GPIO_Pin_0,不能使用或运算
 * @param GPIO_Mode GPIO模式
 * @param GPIO_Speed GPIO速度
 * @return GPIO_Object_TypeDef* 初始化完毕的对象
 */
GPIO_Object GPIO_Initialize(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin_x, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed) {
    switch ((intptr_t)GPIOx) {
        case (intptr_t)GPIOA: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        } break;
        case (intptr_t)GPIOB: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        } break;
        case (intptr_t)GPIOC: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        } break;
        case (intptr_t)GPIOD: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
        } break;
        case (intptr_t)GPIOE: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
        } break;
        case (intptr_t)GPIOF: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
        } break;
        case (intptr_t)GPIOG: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
        } break;
        default:{//混入了奇怪的东西
            return EmptyObject;
        }
    }
    switch(GPIO_Pin_x){//有效性验证
        case GPIO_Pin_0:
        case GPIO_Pin_1:
        case GPIO_Pin_2:
        case GPIO_Pin_3:
        case GPIO_Pin_4:
        case GPIO_Pin_5:
        case GPIO_Pin_6:
        case GPIO_Pin_7:
        case GPIO_Pin_8:
        case GPIO_Pin_9:
        case GPIO_Pin_10:
        case GPIO_Pin_11:
        case GPIO_Pin_12:
        case GPIO_Pin_13:
        case GPIO_Pin_14:
        case GPIO_Pin_15:{
            break;
        }
        default:{
            return EmptyObject;
        }
    }

    if(GPIO_Mode==GPIO_Mode_AF_OD||GPIO_Mode==GPIO_Mode_AF_PP){//如果为复用开漏或推挽，就顺便打开复用时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    }

   GPIO_Object newGPIOPeriph = (GPIO_Object)MeM_Request(sizeof(struct GPIO_Object_TypeDef));
    if(!newGPIOPeriph){//创建对象失败
        return NULL;
    }
    newGPIOPeriph->GPIO_Pin_x = GPIO_Pin_x;
    newGPIOPeriph->GPIOx = GPIOx;

    GPIO_InitTypeDef GPIO_InitData;
    GPIO_InitData.GPIO_Mode = GPIO_Mode;
    GPIO_InitData.GPIO_Speed = GPIO_Speed;
    GPIO_InitData.GPIO_Pin = GPIO_Pin_x;
    GPIO_Init(GPIOx, &GPIO_InitData);

    return newGPIOPeriph;
}

/**
 * @brief 生成一个未初始化的GPIO对象，某些外设的初始化函数可以自动帮你初始化GPIO，你只需要以这个作为参数就行
 * 
 * @param GPIOx 
 * @param GPIO_Pin_x 
 * @return GPIO_PeriphTypeDef* 
 */
GPIO_Object GPIO_GenerateUninitialize(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin_x){
        switch ((intptr_t)GPIOx) {//有效性验证
        case (intptr_t)GPIOA:
        case (intptr_t)GPIOB:
        case (intptr_t)GPIOC:
        case (intptr_t)GPIOD:
        case (intptr_t)GPIOE:
        case (intptr_t)GPIOF:
        case (intptr_t)GPIOG: {
            break;
        }
        default:{//混入了奇怪的东西
            return NULL;
        }
    }
    switch(GPIO_Pin_x){//有效性验证
        case GPIO_Pin_0:
        case GPIO_Pin_1:
        case GPIO_Pin_2:
        case GPIO_Pin_3:
        case GPIO_Pin_4:
        case GPIO_Pin_5:
        case GPIO_Pin_6:
        case GPIO_Pin_7:
        case GPIO_Pin_8:
        case GPIO_Pin_9:
        case GPIO_Pin_10:
        case GPIO_Pin_11:
        case GPIO_Pin_12:
        case GPIO_Pin_13:
        case GPIO_Pin_14:
        case GPIO_Pin_15:{
            break;
        }
        default:{
            return EmptyObject;
        }
    }
    GPIO_Object newGPIOPeriph = (GPIO_Object)MeM_Request(sizeof(struct GPIO_Object_TypeDef));
    if(!newGPIOPeriph){//创建对象失败
        return EmptyObject;
    }
    newGPIOPeriph->GPIO_Pin_x = GPIO_Pin_x;
    newGPIOPeriph->GPIOx = GPIOx;
    return newGPIOPeriph;
}

/**
 * @brief 初始化已建立但未初始化的GPIO对象，例如通过GPIO_GenerateUninitialize生成的对象
 * 
 * @param GPIO_Periph GPIO对象
 * @param GPIO_Mode GPIO模式
 * @param GPIO_Speed GPIO速度
 */
void GPIO_InitializeObject(GPIO_Object GPIO_Periph, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed){
    switch ((intptr_t)GPIO_Periph->GPIOx) {
        case (intptr_t)GPIOA: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        } break;
        case (intptr_t)GPIOB: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        } break;
        case (intptr_t)GPIOC: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        } break;
        case (intptr_t)GPIOD: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
        } break;
        case (intptr_t)GPIOE: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
        } break;
        case (intptr_t)GPIOF: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
        } break;
        case (intptr_t)GPIOG: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
        } break;
        default:{//混入了奇怪的东西
            return;
        }
    }
    switch(GPIO_Periph->GPIO_Pin_x){//有效性验证
        case GPIO_Pin_0:
        case GPIO_Pin_1:
        case GPIO_Pin_2:
        case GPIO_Pin_3:
        case GPIO_Pin_4:
        case GPIO_Pin_5:
        case GPIO_Pin_6:
        case GPIO_Pin_7:
        case GPIO_Pin_8:
        case GPIO_Pin_9:
        case GPIO_Pin_10:
        case GPIO_Pin_11:
        case GPIO_Pin_12:
        case GPIO_Pin_13:
        case GPIO_Pin_14:
        case GPIO_Pin_15:{
            break;
        }
        default:{
            return;
        }
    }

    if(GPIO_Mode==GPIO_Mode_AF_OD||GPIO_Mode==GPIO_Mode_AF_PP){//如果为复用开漏或推挽，就顺便打开复用时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    }

    GPIO_InitTypeDef GPIO_InitData;
    GPIO_InitData.GPIO_Mode = GPIO_Mode;
    GPIO_InitData.GPIO_Speed = GPIO_Speed;
    GPIO_InitData.GPIO_Pin = GPIO_Periph->GPIO_Pin_x;
    GPIO_Init(GPIO_Periph->GPIOx, &GPIO_InitData);

    return;
}

/**
 * @brief 写GPIO
 * 
 * @param GPIO_Periph GPIO对象
 * @param val 0或非0
 */
__inline void GPIO_WritePin(GPIO_Object GPIO_Periph,unsigned int val){
    GPIO_WriteBit(GPIO_Periph->GPIOx, GPIO_Periph->GPIO_Pin_x, (BitAction) !!val);
}

/**
 * @brief 读GPIO输入的数据
 * 
 * @param GPIO_Periph 
 * @return uint8_t 
 */
__inline uint8_t GPIO_ReadPinInput(GPIO_Object GPIO_Periph){
    return GPIO_ReadInputDataBit(GPIO_Periph->GPIOx, GPIO_Periph->GPIO_Pin_x);
}

/**
 * @brief 读GPIO输出的数据
 * 
 * @param GPIO_Periph 
 * @return uint8_t 
 */
__inline uint8_t GPIO_ReadPinOutput(GPIO_Object GPIO_Periph){
    return GPIO_ReadOutputDataBit(GPIO_Periph->GPIOx, GPIO_Periph->GPIO_Pin_x);
}

