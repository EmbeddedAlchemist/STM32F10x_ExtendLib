#include "STM32F10x_ExternLib.h"

const GenericObject EmptyObject = (GenericObject)0;

/**
 * @brief 初始化ExternLib，建议在程序开头调用，相当于其他教程的SysInit
 * 至少需要在一切externLib函数调用前调用
 * 
 */
void STM32F10x_ExternLib_Initialize(){
    MeM_Init();
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

