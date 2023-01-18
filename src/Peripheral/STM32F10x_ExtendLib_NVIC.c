#include "STM32F10x_ExtendLib_NVIC.h"

__INLINE void NVIC_SetPriorityGroup(NVIC_PriorityGroup group){
    NVIC_PriorityGroupConfig(group);
}

InterruptVector_Object NVIC_GenerateObject(IRQn_Type IRQChannel, uint8_t preemptionPriority, uint8_t subPriority){
    InterruptVector_Object newIVObj = (InterruptVector_Object)MeM_Request(sizeof(struct InterruptVector_Object_TypeDef));
    if(newIVObj==EmptyObject){
        return EmptyObject;
    }
    newIVObj->IRQChannel = IRQChannel;
    newIVObj->preemptionPriority = preemptionPriority;
    newIVObj->subPriority = subPriority;
    newIVObj->state = DISABLE;
    return newIVObj;
}


/**
 * @brief 配置中断通道状态，打开或关闭
 * 
 * @param IVObj 指定一个中断向量对象
 * @param state ENABLE或DISABLE
 */
void NVIC_SetState(InterruptVector_Object IVObj, FunctionalState state){
    IVObj->state = state;
    NVIC_InitTypeDef NVICInitData = {IVObj->IRQChannel, IVObj->preemptionPriority, IVObj->subPriority, state};
    NVIC_Init(&NVICInitData);
}


/**
 * @brief 设置中断优先级 与NVIC_SetPriority重名了QAQ
 * 
 * @param IVObj 
 * @param preemptionPriority 
 * @param subPriority 
 */
void NVIC_SetPrior(InterruptVector_Object IVObj, uint8_t preemptionPriority, uint8_t subPriority){
    IVObj->preemptionPriority = preemptionPriority;
    IVObj->subPriority = subPriority;
    NVIC_InitTypeDef NVICInitData = {IVObj->IRQChannel, IVObj->preemptionPriority, IVObj->subPriority, DISABLE};
    NVIC_Init(&NVICInitData);
    NVICInitData.NVIC_IRQChannelCmd = IVObj->state;
    NVIC_Init(&NVICInitData);
}
