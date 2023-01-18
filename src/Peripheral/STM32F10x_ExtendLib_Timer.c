#include "STM32F10x_ExtendLib_Timer.h"
#include "ExtendLibDefinition.h"
#include "MemoryManage.h"

static TIM_Object TIM1Periph, TIM2Periph, TIM3Periph, TIM4Periph, TIM5Periph,
    TIM6Periph, TIM7Periph, TIM8Periph, TIM9Periph, TIM10Periph,
    TIM11Periph, TIM12Periph, TIM13Periph, TIM14Periph, TIM15Periph,
    TIM16Periph, TIM17Periph;

/**
 * @brief 复位一个定时器
 *
 * @param TIM_Periph 指定一个定时器外设
 */
void TIM_Deinitialize(TIM_Object TIM_Periph) {
    if (!TIM_Periph) {
        return;
    }
    switch ((intptr_t)TIM_Periph->TIMx) {
        case (intptr_t)TIM1:
        case (intptr_t)TIM2:
        case (intptr_t)TIM3:
        case (intptr_t)TIM4:
        case (intptr_t)TIM5:
        case (intptr_t)TIM6:
        case (intptr_t)TIM7:
        case (intptr_t)TIM8:
        case (intptr_t)TIM9:
        case (intptr_t)TIM10:
        case (intptr_t)TIM11:
        case (intptr_t)TIM12:
        case(intptr_t)TIM13:
        case (intptr_t)TIM14:
        case (intptr_t)TIM15:
        case (intptr_t)TIM16:
        case (intptr_t)TIM17: {
            TIM_DeInit(TIM_Periph->TIMx);
        } break;
        default: {
            return;
        }
    }
    MeM_Release((void *)TIM_Periph);
}

/**
 * @brief 初始化一个定时器(高级版，需要自定义预分频系数和重装值)
 *
 * @param TIMx 指定一个定时器外设
 * @param period 重装值
 * @param prescaler 预分频系数
 * @param IRQnChannel 中断通道
 * @param preemptionPriority 中断的主优先级
 * @param subPriority 中断的从优先级
 * @return TIM_PeriphTypeDef*
 */
TIM_Object TIM_InitializeAdvance(TIM_TypeDef *TIMx, uint16_t period, uint16_t prescaler, void (*interruptUpdateCallback)(GenericObject), GenericObject interruptUpdateCallbackParam, uint8_t IRQnChannel, uint8_t preemptionPriority, uint8_t subPriority) {
    TIM_Object *selectedTIM;
    switch ((intptr_t)TIMx) {
        case (intptr_t)TIM1: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
            selectedTIM = &TIM1Periph;
        } break;
        case (intptr_t)TIM2: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
            selectedTIM = &TIM2Periph;
        } break;
        case (intptr_t)TIM3: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
            selectedTIM = &TIM3Periph;
        } break;
        case (intptr_t)TIM4: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
            selectedTIM = &TIM4Periph;
        } break;
        case (intptr_t)TIM5: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
            selectedTIM = &TIM5Periph;
        } break;
        case (intptr_t)TIM6: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
            selectedTIM = &TIM6Periph;
        } break;
        case (intptr_t)TIM7: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
            selectedTIM = &TIM7Periph;
        } break;
        case (intptr_t)TIM8: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
            selectedTIM = &TIM8Periph;
        } break;
        case (intptr_t)TIM9: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
            selectedTIM = &TIM9Periph;
        } break;
        case (intptr_t)TIM10: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
            selectedTIM = &TIM10Periph;
        } break;
        case (intptr_t)TIM11: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
            selectedTIM = &TIM11Periph;
        }
        case (intptr_t)TIM12: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
            selectedTIM = &TIM12Periph;
        } break;
        case (intptr_t)TIM13: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
            selectedTIM = &TIM13Periph;
        } break;
        case (intptr_t)TIM14: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
            selectedTIM = &TIM14Periph;
        } break;
        case (intptr_t)TIM15: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
            selectedTIM = &TIM15Periph;
        } break;
        case (intptr_t)TIM16: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
            selectedTIM = &TIM16Periph;
        } break;
        case (intptr_t)TIM17: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
            selectedTIM = &TIM17Periph;
        } break;
        default: {
            return NULL;
        }
    }

    TIM_DeInit(TIMx);                  //先复位
    if (*selectedTIM) {                 //之前被初始化过
        TIM_Deinitialize(*selectedTIM); //清除之前的初始化
    }
    *selectedTIM = ExtendLib_NewObject(TIM_Object); //创建新对象
    if (!*selectedTIM) {                                                        //创建失败
        return NULL;
    }
    (*selectedTIM)->TIMx = TIMx;
    (*selectedTIM)->interruptUpdateCallback = interruptUpdateCallback;
    (*selectedTIM)->interruptUpdateCallbackParam = interruptUpdateCallbackParam;
    (*selectedTIM)->IRQn_Channel = IRQnChannel;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitData;
    TIM_TimeBaseInitData.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分割0
    TIM_TimeBaseInitData.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
    TIM_TimeBaseInitData.TIM_Period = period;                  //计数重装值
    TIM_TimeBaseInitData.TIM_RepetitionCounter = 0;            //这个只有高级定时器才有用...写死是0了
    TIM_TimeBaseInitData.TIM_Prescaler = prescaler;            //预分频系数
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitData);
    TIM_ARRPreloadConfig(TIMx, ENABLE);

    if (interruptUpdateCallback) {//中断初始化
        NVIC_InitTypeDef NVIC_InitData;
        NVIC_InitData.NVIC_IRQChannel = IRQnChannel;
        NVIC_InitData.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitData.NVIC_IRQChannelPreemptionPriority = preemptionPriority;
        NVIC_InitData.NVIC_IRQChannelSubPriority = subPriority;
        NVIC_Init(&NVIC_InitData);
        TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
    }

    TIM_Cmd(TIMx, ENABLE);

    return *selectedTIM;
}

/**
 * @brief 初始化一个定时器...每一个定时器只能初始化一次，再次初始化会导致之前的初始化失效
 *
 * @param TIMx 指定一个定时器外设
 * @param interval 定时器溢出时间间隔，单位us，可能因误差偏小，需要精确计时请使用TIM_InitializeAdvance精确指定重装值和预分频系数
 * @param interruptUpdateCallback 溢出回调函数，如果为NULL则不会初始化中断，后三个参数失效
 * @param IRQnChannel 中断通道
 * @param preemptionPriority 中断的主优先级
 * @param subPriority 中断的从优先级
 * @return TIM_Object*
 */
TIM_Object TIM_Initialize(TIM_TypeDef *TIMx, unsigned int interval, void (*interruptUpdateCallback)(GenericObject), GenericObject interruptUpdateCallbackParam, uint8_t IRQnChannel, uint8_t preemptionPriority, uint8_t subPriority) {
    SystemCoreClockUpdate();
    return TIM_InitializeAdvance(TIMx, TIM_PERIOD_DEFAULT, (SystemCoreClock / 1000000) * interval / TIM_PERIOD_DEFAULT, interruptUpdateCallback, interruptUpdateCallbackParam, IRQnChannel, preemptionPriority, subPriority);
}


/**
 * @brief 生成一个未初始化的对象
 * 
 * @param TIMx 
 */
TIM_Object TIM_GenerateUninitialize(TIM_TypeDef *TIMx,uint8_t IRQn_Channel){
    TIM_Object newTimerObject = ExtendLib_NewObject(TIM_Object);
    newTimerObject->TIMx = TIMx;
    newTimerObject->IRQn_Channel = IRQn_Channel;
    return newTimerObject;
}


/**
 * @brief 
 * 
 * @param TIM_Periph 
 * @param period 
 * @param prescaler 
 * @param interruptUpdateCallback 
 * @param interruptUpdateCallbackParam 
 * @param IRQnChannel 
 * @param preemptionPriority 
 * @param subPriority 
 */
void TIM_InitializeAdvanceObject(TIM_Object TIM_Periph, uint16_t period, uint16_t prescaler, void (*interruptUpdateCallback)(GenericObject), GenericObject interruptUpdateCallbackParam, uint8_t preemptionPriority, uint8_t subPriority){
    TIM_Object *selectedTIM;
    switch ((intptr_t)TIM_Periph->TIMx) {
        case (intptr_t)TIM1: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
            selectedTIM = &TIM1Periph;
        } break;
        case (intptr_t)TIM2: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
            selectedTIM = &TIM2Periph;
        } break;
        case (intptr_t)TIM3: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
            selectedTIM = &TIM3Periph;
        } break;
        case (intptr_t)TIM4: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
            selectedTIM = &TIM4Periph;
        } break;
        case (intptr_t)TIM5: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
            selectedTIM = &TIM5Periph;
        } break;
        case (intptr_t)TIM6: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
            selectedTIM = &TIM6Periph;
        } break;
        case (intptr_t)TIM7: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
            selectedTIM = &TIM7Periph;
        } break;
        case (intptr_t)TIM8: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
            selectedTIM = &TIM8Periph;
        } break;
        case (intptr_t)TIM9: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
            selectedTIM = &TIM9Periph;
        } break;
        case (intptr_t)TIM10: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
            selectedTIM = &TIM10Periph;
        } break;
        case (intptr_t)TIM11: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
            selectedTIM = &TIM11Periph;
        }
        case (intptr_t)TIM12: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
            selectedTIM = &TIM12Periph;
        } break;
        case (intptr_t)TIM13: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
            selectedTIM = &TIM13Periph;
        } break;
        case (intptr_t)TIM14: {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
            selectedTIM = &TIM14Periph;
        } break;
        case (intptr_t)TIM15: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
            selectedTIM = &TIM15Periph;
        } break;
        case (intptr_t)TIM16: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
            selectedTIM = &TIM16Periph;
        } break;
        case (intptr_t)TIM17: {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
            selectedTIM = &TIM17Periph;
        } break;
        default: {
            return;
        }
    }

    TIM_DeInit(TIM_Periph->TIMx);                  //先复位
    if (*selectedTIM) {                 //之前被初始化过
        TIM_Deinitialize(*selectedTIM); //清除之前的初始化
    }

    (*selectedTIM) = TIM_Periph;
    (*selectedTIM)->TIMx = TIM_Periph->TIMx;
    (*selectedTIM)->interruptUpdateCallback = interruptUpdateCallback;
    (*selectedTIM)->interruptUpdateCallbackParam = interruptUpdateCallbackParam;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitData;
    TIM_TimeBaseInitData.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分割0
    TIM_TimeBaseInitData.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
    TIM_TimeBaseInitData.TIM_Period = period;                  //计数重装值
    TIM_TimeBaseInitData.TIM_RepetitionCounter = 0;            //这个只有高级定时器才有用...写死是0了
    TIM_TimeBaseInitData.TIM_Prescaler = prescaler;            //预分频系数
    TIM_TimeBaseInit(TIM_Periph->TIMx, &TIM_TimeBaseInitData);
    TIM_ARRPreloadConfig(TIM_Periph->TIMx, ENABLE);

    if (interruptUpdateCallback) {//中断初始化
        NVIC_InitTypeDef NVIC_InitData;
        NVIC_InitData.NVIC_IRQChannel = (*selectedTIM)->IRQn_Channel;
        NVIC_InitData.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitData.NVIC_IRQChannelPreemptionPriority = preemptionPriority;
        NVIC_InitData.NVIC_IRQChannelSubPriority = subPriority;
        NVIC_Init(&NVIC_InitData);
        TIM_ITConfig(TIM_Periph->TIMx, TIM_IT_Update, ENABLE);
    }

    TIM_Cmd(TIM_Periph->TIMx, ENABLE);
}


/**
 * @brief 初始化已生成但未初始化的对象
 * 
 * @param TIM_Periph 
 * @param interval 
 * @param interruptUpdateCallback 
 * @param interruptUpdateCallbackParam 
 * @param IRQnChannel 
 * @param preemptionPriority 
 * @param subPriority 
 */
void TIM_InitializeObject(TIM_Object TIM_Periph, unsigned int interval, void (*interruptUpdateCallback)(GenericObject), GenericObject interruptUpdateCallbackParam, uint8_t preemptionPriority, uint8_t subPriority){
    SystemCoreClockUpdate();
    TIM_InitializeAdvanceObject(TIM_Periph, TIM_PERIOD_DEFAULT, (SystemCoreClock / 1000000) * interval / TIM_PERIOD_DEFAULT, interruptUpdateCallback, interruptUpdateCallbackParam, preemptionPriority, subPriority);
}

/**
 * @brief 获取定时器重装值
 * 
 * @param TIM_Periph 指定一个定时器外设
 * @return uint16_t 重装值
 */
uint16_t TIM_GetReloadValue(TIM_Object TIM_Periph){
    return TIM_Periph->TIMx->ARR;
}

/**
 * @brief 获取定时器预分频系数
 * 
 * @param TIM_Periph 指定一个定时器外设
 * @return uint16_t 预分频系数
 */
uint16_t TIM_GetPrescalerValue(TIM_Object TIM_Periph){
    return TIM_Periph->TIMx->PSC;
}

/**
 * @brief 设置定时器重装值
 * 
 * @param reloadValue 重装值
 */
void TIM_SetReloadValue(TIM_Object TIM_Periph, uint16_t reloadValue){
    TIM_Periph->TIMx->ARR = reloadValue;
}

/**
 * @brief 设置预分频系数
 * 
 * @param prescalerValue 预分频系数
 */
void TIM_SetPrescalerValue(TIM_Object TIM_Periph, uint16_t prescalerValue){
    TIM_Periph->TIMx->PSC = prescalerValue;
}

/**
 * @brief 保持重装值不变，通过调整预分频系数来设置定时器的溢出时间间隔
 *        这对调整PWM的周期和一般情况非常有效
 * 
 * @param TIM_Periph 指定一个定时器外设
 * @param interval 时间间隔，单位us
 */
void TIM_SetInterval(TIM_Object TIM_Periph,unsigned int interval){
    uint16_t reloadVal = TIM_GetReloadValue(TIM_Periph);//为什么标准库没有获取重装值的函数
    SystemCoreClockUpdate();
    TIM_SetPrescalerValue(TIM_Periph, (SystemCoreClock / 1000000) * interval / (reloadVal + 1) - 1);
}

/**
 * @brief 保持预分频系数不变，通过调整重装值设置定时器溢出时间间隔
 *        更推荐使用TIM_SetInterval，通过此函数调整可能影响工作在该定时器上的PWM
 * 
 * @param TIM_Periph 指定一个定时器外设
 * @param interval 时间间隔，单位us
 */
void TIM_SetIntervalByReload(TIM_Object TIM_Periph,unsigned int interval){
    uint16_t prescalerVal = TIM_GetPrescalerValue(TIM_Periph);
    SystemCoreClockUpdate();
    TIM_SetReloadValue(TIM_Periph, (SystemCoreClock / 1000000) * interval / (prescalerVal + 1) - 1);
}

/**
 * @brief 获取定时器溢出的时间间隔
 * 
 * @param TIM_Periph 
 * @return unsigned int 时间间隔，单位us
 */
unsigned int TIM_GetInterval(TIM_Object TIM_Periph){
    uint16_t prescalerVal=TIM_GetPrescalerValue(TIM_Periph);
    uint16_t reloadVal = TIM_GetReloadValue(TIM_Periph);
    SystemCoreClockUpdate();
    return (prescalerVal + 1) * (reloadVal + 1) / (SystemCoreClock / 1000000);
}

/*==============================================================
                     中断处理（非必要不要修改）
===============================================================*/

void TIM1_UP_IRQHandler() {
    if (TIM_GetITStatus(TIM1, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        if (TIM1Periph->interruptUpdateCallback) {
            TIM1Periph->interruptUpdateCallback(TIM1Periph->interruptUpdateCallbackParam);
        }
    }
}

void TIM2_IRQHandler() {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        if (TIM2Periph->interruptUpdateCallback) {
            TIM2Periph->interruptUpdateCallback(TIM2Periph->interruptUpdateCallbackParam);
        }
    }
}

void TIM3_IRQHandler() {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        if (TIM3Periph->interruptUpdateCallback) {
            TIM3Periph->interruptUpdateCallback(TIM3Periph->interruptUpdateCallbackParam);
        }
    }
}

void TIM4_IRQHandler() {
    if (TIM_GetITStatus(TIM4, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        if (TIM4Periph->interruptUpdateCallback) {
            TIM4Periph->interruptUpdateCallback(TIM4Periph->interruptUpdateCallbackParam);
        }
    }
}

void TIM5_IRQHandler() {
    if (TIM_GetITStatus(TIM5, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
        if (TIM5Periph->interruptUpdateCallback) {
            TIM5Periph->interruptUpdateCallback(TIM5Periph->interruptUpdateCallbackParam);
        }
    }
}

void TIM6_IRQHandler() {
    if (TIM_GetITStatus(TIM6, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        if (TIM6Periph->interruptUpdateCallback) {
            TIM6Periph->interruptUpdateCallback(TIM6Periph->interruptUpdateCallbackParam);
        }
    }
}

void TIM6_DAC_IRQHandler() {
    if (TIM_GetITStatus(TIM6, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        if (TIM6Periph->interruptUpdateCallback) {
            TIM6Periph->interruptUpdateCallback(TIM6Periph->interruptUpdateCallbackParam);
        }
    }
}

void TIM7_IRQHandler() {
    if (TIM_GetITStatus(TIM7, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
        if (TIM7Periph->interruptUpdateCallback) {
            TIM7Periph->interruptUpdateCallback(TIM7Periph->interruptUpdateCallbackParam);
        }
    }
}

void TIM8_UP_IRQHandler() {
    if (TIM_GetITStatus(TIM8, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
        if (TIM8Periph->interruptUpdateCallback) {
            TIM8Periph->interruptUpdateCallback(TIM8Periph->interruptUpdateCallbackParam);
        }
    }
}

void TIM1_BRK_TIM9_IRQHandler() {
    if (TIM_GetITStatus(TIM9, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
        if (TIM9Periph->interruptUpdateCallback) {
            TIM9Periph->interruptUpdateCallback(TIM9Periph->interruptUpdateCallbackParam);
        }
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_Break)) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
    }
}

void TIM1_UP_TIM10_IRQHandler() {
    if (TIM_GetITStatus(TIM1, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        if (TIM1Periph->interruptUpdateCallback) {
            TIM1Periph->interruptUpdateCallback(TIM1Periph->interruptUpdateCallbackParam);
        }
    }
    if (TIM_GetITStatus(TIM10, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
        if (TIM10Periph->interruptUpdateCallback) {
            TIM10Periph->interruptUpdateCallback(TIM10Periph->interruptUpdateCallbackParam);
        }
    }
}

void TIM1_TRG_COM_TIM11_IRQHandler() {
    if (TIM_GetITStatus(TIM11, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM11, TIM_IT_Update);
        if (TIM11Periph->interruptUpdateCallback) {
            TIM11Periph->interruptUpdateCallback(TIM11Periph->interruptUpdateCallbackParam);
        }
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_COM)) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_COM);
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_Trigger)) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Trigger);
    }
}

void TIM12_IRQHandler() {
    if (TIM_GetITStatus(TIM12, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
        if (TIM12Periph->interruptUpdateCallback) {
            TIM12Periph->interruptUpdateCallback(TIM12Periph->interruptUpdateCallbackParam);
        }
    }
}

void TIM8_BRK_TIM12_IRQHandler() {
    if (TIM_GetITStatus(TIM12, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
        if (TIM12Periph->interruptUpdateCallback) {
            TIM12Periph->interruptUpdateCallback(TIM12Periph->interruptUpdateCallbackParam);
        }
    }
    if (TIM_GetITStatus(TIM8, TIM_IT_Break)) {
        TIM_ClearITPendingBit(TIM8, TIM_IT_Break);
    }
}

void TIM13_IRQHandler() {
    if (TIM_GetITStatus(TIM13, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM13, TIM_IT_Update);
        if (TIM13Periph->interruptUpdateCallback) {
            TIM13Periph->interruptUpdateCallback(TIM13Periph->interruptUpdateCallbackParam);
        }
    }
}

void TIM8_UP_TIM13_IRQHandler() {
    if (TIM_GetITStatus(TIM8, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
        if (TIM8Periph->interruptUpdateCallback) {
            TIM8Periph->interruptUpdateCallback(TIM8Periph->interruptUpdateCallbackParam);
        }
    }
    if (TIM_GetITStatus(TIM13, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM13, TIM_IT_Update);
        if (TIM13Periph->interruptUpdateCallback) {
            TIM13Periph->interruptUpdateCallback(TIM13Periph->interruptUpdateCallbackParam);
        }
    }
}

void TIM14_IRQHandler() {
    if (TIM_GetITStatus(TIM14, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
        if (TIM14Periph->interruptUpdateCallback) {
            TIM14Periph->interruptUpdateCallback(TIM14Periph->interruptUpdateCallbackParam);
        }
    }
}

void TIM8_TRG_COM_TIM14_IRQHandler() {
    if (TIM_GetITStatus(TIM14, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
        if (TIM14Periph->interruptUpdateCallback) {
            TIM14Periph->interruptUpdateCallback(TIM14Periph->interruptUpdateCallbackParam);
        }
    }
    if (TIM_GetITStatus(TIM8, TIM_IT_COM)) {
        TIM_ClearITPendingBit(TIM8, TIM_IT_COM);
    }
    if (TIM_GetITStatus(TIM8, TIM_IT_Trigger)) {
        TIM_ClearITPendingBit(TIM8, TIM_IT_Trigger);
    }
}

void TIM1_BRK_TIM15_IRQHandler() {
    if (TIM_GetITStatus(TIM15, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM15, TIM_IT_Update);
        if (TIM15Periph->interruptUpdateCallback) {
            TIM15Periph->interruptUpdateCallback(TIM15Periph->interruptUpdateCallbackParam);
        }
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_Break)) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
    }
}

void TIM1_UP_TIM16_IRQHandler() {
    if (TIM_GetITStatus(TIM1, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        if (TIM1Periph->interruptUpdateCallback) {
            TIM1Periph->interruptUpdateCallback(TIM1Periph->interruptUpdateCallbackParam);
        }
    }
    if (TIM_GetITStatus(TIM16, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
        if (TIM16Periph->interruptUpdateCallback) {
            TIM16Periph->interruptUpdateCallback(TIM16Periph->interruptUpdateCallbackParam);
        }
    }
}

void TIM1_TRG_COM_TIM17_IRQHandler() {
    if (TIM_GetITStatus(TIM17, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM17, TIM_IT_Update);
        if (TIM17Periph->interruptUpdateCallback) {
            TIM17Periph->interruptUpdateCallback(TIM17Periph->interruptUpdateCallbackParam);
        }
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_COM)) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_COM);
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_Trigger)) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Trigger);
    }
}
