#ifndef _MEMORY_MANAGE_H_
#define _MEMORY_MANAGE_H_

#include "STM32F10x_ExternLib.h"

#ifndef NULL
#define NULL ((void *)0)
#endif


//受MemoryManage管理的内存大小，建议是32的整数倍
#define MeM_MEMORY_POOL_SIZE 8192
//内存块大小,建议是4的整数倍，且大于块头部数据。该值太小可能增加产生内存碎片的概率
#define MeM_MEMORY_BLOCK_SIZE 16
//在每次申请的内存结尾追加至少多长字节的空内存，一般不需要，除非担心程序会访问越界
#define MeM_MEMORY_REQUEST_TAIL 0

#define MeM_MEMORY_REQUEST_STRATEGY_FAST_SPEED //更快速度，提升申请内存的速度，但是可能更高频率地触发内存碎片整理，导致后期申请时速度变慢
//#define MeM_MEMORY_REQUEST_STRATEGY_LESS_FRAGMENTATION //更少内存碎片，尽量减少内存碎片的产生概率，但会降低申请时的速度

#ifndef MeM_MEMORY_REQUEST_STRATEGY_FAST_SPEED
#ifndef MeM_MEMORY_REQUEST_STRATEGY_LESS_FRAGMENTATION
#error Please choose one from MeM_MEMORY_REQUEST_STRATEGY_FAST_SPEED and MeM_MEMORY_REQUEST_STRATEGY_LESS_FRAGMENTATION for memory request strategy
#endif
#endif

#ifdef MeM_MEMORY_REQUEST_STRATEGY_FAST_SPEED
#ifdef MeM_MEMORY_REQUEST_STRATEGY_LESS_FRAGMENTATION
#error MeM_MEMORY_REQUEST_STRATEGY_FAST_SPEED and MeM_MEMORY_REQUEST_STRATEGY_LESS_FRAGMENTATION only one can be chosen!
#endif
#endif

/**
 * NOTICE:千万不能越界！！！块头数据被覆盖了整个内存池就寄掉了！！！
 */

void MeM_Init(void);
void MeM_Release(void *addr);
void *MeM_Request(unsigned int size);

#endif
