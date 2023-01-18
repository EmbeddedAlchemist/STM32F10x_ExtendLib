#include "STM32F10x_ExtendLib.h"

void *(*ExternLib_MemoryRequest)(size_t) =
#ifdef _MEMORY_MANAGE_H_
    MeM_Request
#else
    malloc
#endif
    ;
void (*ExternLib_MemoryRelease)(void *) =
#ifdef _MEMORY_MANAGE_H_
    MeM_Release
#else
    free
#endif
    ;

/**
 * @brief 设置扩展库使用的内存管理
 * 
 * @param MemoryRequest 
 * @param MemoryRelease 
 */
void ExternLib_SetMemoryManageFunc(void *(*MemoryRequest)(size_t),void (*MemoryRelease)(void *)){
    ExternLib_MemoryRequest = MemoryRequest;
    ExternLib_MemoryRelease = MemoryRelease;
}

/**
 * @brief 初始化ExternLib，建议在程序开头调用，相当于其他教程的SysInit
 * 至少需要在一切externLib函数调用前调用
 * 
 */
void ExternLib_Initialize(){
    MeM_Init();//内存管理初始化
    TimingAndDelay_Init();
}

/**
 * @brief 主循环处理
 * 
 */
void ExternLib_MainLoopHandler(){
}


/**
 * @brief 销毁一个对象
 * 【慎用】除非保证该对象下不包含其他对象，否则直接使用可能造成内存泄漏
 * 
 * @param object 
 */
void DistoryObject(GenericObject object){
    MeM_Release(object);
}

