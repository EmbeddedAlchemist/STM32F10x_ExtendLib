#ifndef _MEMORY_MANAGE_H_
#define _MEMORY_MANAGE_H_

#include "STM32F10x_ExtendLib.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef MEMORY_MANAGE_MEMORY_REQUEST_STRATEGY_FAST_SPEED
#ifndef MEMORY_MAMAGE_MEMORY_REQUEST_STRATEGY_LESS_FRAGMENTATION
#error Please choose one from MEMORY_MANAGE_MEMORY_REQUEST_STRATEGY_FAST_SPEED and MEMORY_MAMAGE_MEMORY_REQUEST_STRATEGY_LESS_FRAGMENTATION for memory request strategy
#endif
#endif

#ifdef MEMORY_MANAGE_MEMORY_REQUEST_STRATEGY_FAST_SPEED
#ifdef MEMORY_MAMAGE_MEMORY_REQUEST_STRATEGY_LESS_FRAGMENTATION
#error MEMORY_MANAGE_MEMORY_REQUEST_STRATEGY_FAST_SPEED and MEMORY_MAMAGE_MEMORY_REQUEST_STRATEGY_LESS_FRAGMENTATION only one can be chosen!
#endif
#endif

/**
 * NOTICE:千万不能越界！！！块头数据被覆盖了整个内存池就寄掉了！！！
 */

void MeM_Init(void);
void MeM_Release(void *addr);
void *MeM_Request(unsigned int size);

#endif
