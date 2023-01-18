/**
 * @file STM32F10x_ExternLib_Config.h
 * @author Challenger--@outlook.com
 * @brief STM32F10x_ExtendLib的配置文件
 * @version 0.1
 * @date 2022-06-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

//======================================//
//======================================//

/**
 * 这是STM32F10x_ExtendLib的配置文件
 * 你可以修改此文件的宏定义来修改扩展库的行为
 * 
 * 每个选项前的标记含义如下
 * [>] 必选/填项
 * [-] 必选单选，一组内必须选择一项
 * [=] 必选多选，一组内选择一项或多项
 * [+] 可选项
 * */

//======================================//
//======================================//





#ifndef _STM32F10X_EXTERN_LIB_CONFIG_
#define _STM32F10X_EXTERN_LIB_CONFIG_



/************************************
* 内存管理配置 | Memory Manage Config *
************************************/

//受MemoryManage内存管理支配的内存大小
//如果没有重定向内存管理函数，那么扩展库创建的对象和申请的内存都将包含在其中
/*[>]*/ #define MEMORY_MANAGE_POOL_SIZE 8192 /* 字节 */

//MemoryManage产生的内存块大小，即分配内存的最小单位
/*[>]*/ #define MEMORY_MANAGE_BLOCK_SIZE 16 /* 字节 */

//MemoryManage在分配内存时在内存块尾部追加的大小，也就是在分配时多分配的内存
/*[>]*/ #define MEMORY_MANAGE_MEMORY_REQUEST_TAIL 0 /* 字节 */

//MemoryManage内存分配策略 
//从下列两项选择其一
//1.更快速度，提升申请内存的速度，但是可能更高频率地触发内存碎片整理，导致后期申请时速度变慢
//2.更少内存碎片，尽量减少内存碎片的产生概率，但会降低申请时的速度
/*[-] 1 */ #define MEMORY_MANAGE_MEMORY_REQUEST_STRATEGY_FAST_SPEED
/*[-] 2 */ //#define MEMORY_MAMAGE_MEMORY_REQUEST_STRATEGY_LESS_FRAGMENTATION 




/************************
 * 调试模式 | Debug Mode *
 ***********************/

//调试模式
//取消以下注释使STM32F10x_ExternLib_Debug下的函数生效
//若调试完成，可注释掉DEBUG_MODE，再次编译后，调试函数将被编译器忽略
/*[+]*/ //#define EXTEND_LIB_DEBUG_MODE

//完整参数验证
//声明EXTEND_LIB_FULL_PARAM_VERIFY可以启用参数验证，调试时可以及时发现错误，但是会降低运行效率
/*[+]*/ //#define EXTEND_LIB_FULL_PARAM_VERIFY

//禁用参数验证
//声明EXTEND_LIB_NO_PARAM_VERIFY可以禁用全部参数验证，包括必要的验证，可能会略微提高运行速度
/*[+]*/ //#define EXTEND_LIB_NO_PARAM_VERIFY


#endif

