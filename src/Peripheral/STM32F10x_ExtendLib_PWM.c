#include "STM32F10x_ExtendLib_PWM.h"
#include "STM32F10x_ExtendLib_GPIO.h"
#include "MemoryManage.h"
#include "stddef.h"

const uint16_t PWM_MaxDuty = UINT16_MAX;

/**
 * @brief 生成并初始化一个PWM对象
 * 
 * @param TIM_Periph 通过TIM_Initialize或TIM_InitializeAdvance初始化生成的Timer对象
 * @param TIM_Channel_x 要初始化的PWM通道
 * @param GPIO_Periph 通道对应的未初始化过的GPIO对象，当然初始化过的也可以，不过会再初始化一遍
 * @return PWM_TypeDef* 初始化完成的PWM对象
 */
PWM_Object PWM_Initialize(TIM_Object TIM_Periph, uint8_t TIM_Channel_x,GPIO_Object GPIO_Periph){
    void (*TIM_OCxInit)(TIM_TypeDef *, TIM_OCInitTypeDef *);
    void (*TIM_OCxPreloadConfig)(TIM_TypeDef *, uint16_t);
    TIM_Cmd(TIM_Periph->TIMx, DISABLE);
    switch(TIM_Channel_x){
        case TIM_Channel_1:{
            TIM_OCxInit = TIM_OC1Init;
            TIM_OCxPreloadConfig = TIM_OC1PreloadConfig;
        }break;
        case TIM_Channel_2:{
            TIM_OCxInit = TIM_OC2Init;
            TIM_OCxPreloadConfig = TIM_OC2PreloadConfig;
        }break;
        case TIM_Channel_3:{
            TIM_OCxInit = TIM_OC3Init;
            TIM_OCxPreloadConfig = TIM_OC3PreloadConfig;
        } break;
        case TIM_Channel_4:{
            TIM_OCxInit = TIM_OC4Init;
            TIM_OCxPreloadConfig = TIM_OC4PreloadConfig;
        } break;
        default:{
            return EmptyObject;
        }
    }
    switch((intptr_t)TIM_Periph->TIMx){
        case (intptr_t)TIM1:
        case (intptr_t)TIM8:
        case (intptr_t)TIM15:
        case (intptr_t)TIM16:
        case (intptr_t)TIM17:{
            TIM_CtrlPWMOutputs(TIM_Periph->TIMx, ENABLE);
        }break;
        default:
            break;
    }
    GPIO_InitializeObject(GPIO_Periph, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
    
    TIM_OCInitTypeDef TIM_OCInitData;
    TIM_OCStructInit(&TIM_OCInitData);
    TIM_OCInitData.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitData.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitData.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitData.TIM_Pulse = 0;
    TIM_OCxInit(TIM_Periph->TIMx, &TIM_OCInitData);
    
    TIM_OCxPreloadConfig(TIM_Periph->TIMx, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM_Periph->TIMx, ENABLE);
    TIM_Cmd(TIM_Periph->TIMx, ENABLE);
    
    PWM_Object newPWMObj = (PWM_Object)MeM_Request(sizeof(struct PWM_Object_TypeDef));
    newPWMObj->TIM_Periph = TIM_Periph;
    newPWMObj->TIM_Channel = TIM_Channel_x;
    return newPWMObj;
}

/**
 * @brief 设置占空比为duty/65535，不考虑定时器的重装值。
 *        例如，duty=32767，则占空比为50%，无论定时器的重装值是什么
 *        【注意】由于重装值的设置，实际的精度并不会达到1/65536
 * 
 * @param PWM 指定一个PWM对象
 * @param duty 占空比系数
 */
void PWM_SetDuty(PWM_Object PWM, uint16_t duty){
    volatile uint16_t *TIM_CCRx;
    switch(PWM->TIM_Channel){
        case TIM_Channel_1:{
            TIM_CCRx = &PWM->TIM_Periph->TIMx->CCR1;
        }break;
        case TIM_Channel_2:{
            TIM_CCRx = &PWM->TIM_Periph->TIMx->CCR2;
        }break;
        case TIM_Channel_3:{
            TIM_CCRx = &PWM->TIM_Periph->TIMx->CCR3;
        } break;
        case TIM_Channel_4:{
            TIM_CCRx = &PWM->TIM_Periph->TIMx->CCR4;
        } break;
        default:{
            return;
        }
    }
    PWM->duty = duty;
    uint16_t compareVal = TIM_GetReloadValue(PWM->TIM_Periph) * duty / UINT16_MAX;
    *TIM_CCRx = compareVal;
}

/**
 * @brief 设置PWM的周期 
 *        【请注意】这将一并改变同一定时器下其他通道的周期，但是不会影响它们的占空比
 * 
 * @param PWM 指定一个PWM对象
 * @param cycle 周期，单位us
 */
void PWM_SetCycle(PWM_Object PWM, unsigned int cycle){
    TIM_SetInterval(PWM->TIM_Periph, cycle);
}

/**
 * @brief 获取理论占空比...因为设置占空比实际是将0到65535映射到0到重装值，所以会有误差
 *        该函数返回的是设置时的占空比，一般情况这个够用，如果只是想再占空比的基础上加上或减去一个值的话建议用这个
 * 
 * @param PWM 指定一个PWM外设
 * @return uint16_t 理论占空比
 */
uint16_t PWM_GetDuty(PWM_Object PWM){
    return PWM->duty;
}

/**
 * @brief 获取理论占空比...因为设置占空比实际是将0到65535映射到0到重装值，所以会有误差
 *        这个就是把设置的比较值反映射到65535上，是实际的占空比
 * 
 * @param PWM 指定一个PWM外设
 * @return uint16_t 实际占空比
 */
uint16_t PWM_GetActualDuty(PWM_Object PWM){
    uint16_t compareVal;
    switch(PWM->TIM_Channel){
        case TIM_Channel_1:{
            compareVal = PWM->TIM_Periph->TIMx->CCR1;
        }break;
        case TIM_Channel_2:{
            compareVal = PWM->TIM_Periph->TIMx->CCR2;
        }break;
        case TIM_Channel_3:{
            compareVal = PWM->TIM_Periph->TIMx->CCR3;
        } break;
        case TIM_Channel_4:{
            compareVal = PWM->TIM_Periph->TIMx->CCR4;
        } break;
        default:{
            return 0;
        }
    }
    uint16_t reloadVal = TIM_GetReloadValue(PWM->TIM_Periph);
    return UINT16_MAX * compareVal / reloadVal;
}
