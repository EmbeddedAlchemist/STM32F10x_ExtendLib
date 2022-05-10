#include "MemoryManage.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"

typedef struct MeM_BlockHead {
    struct MeM_BlockHead *next;
    unsigned int available;   //该块是否被占用
    size_t size;              //实际大小，包含块头数据
    uint32_t placeHolder;

} MeM_BlockHead;

//内存池
__align(32) volatile static uint32_t MeM_MemoryPool[MeM_MEMORY_POOL_SIZE/sizeof(uint32_t)];

/**
 * @brief 整理内存碎片
 *
 */
void MeM_Tidy() {
    MeM_BlockHead *curBlock = (MeM_BlockHead *)MeM_MemoryPool; //从头开始找
    while (curBlock->next) {
        if (!curBlock->available) { //块被占用，找下一块
            curBlock = curBlock->next;
            continue;
        } else {                                                        //当前块可用
            if (curBlock->next->available) {                            //下一块也可用，则合并两块
                curBlock->size = curBlock->size + curBlock->next->size; //两块大小一加
                curBlock->next = curBlock->next->next;                  //指针指向下下块，下一块应该就消失了
                continue;
            } else { //下一块不可用
                curBlock = curBlock->next;
                continue;
            }
        }
    }
}

/**
 * @brief
 *
 * @param size 需要找的块大小，包含头数据
 */
void *MeM_Request_Base(size_t size) {
    MeM_BlockHead *curBlock = (MeM_BlockHead *)MeM_MemoryPool;
    void *rtnAddr = NULL; //返回可用的地址

#ifdef MeM_MEMORY_REQUEST_STRATEGY_FAST_SPEED
    while (curBlock) {
        if (curBlock->available == false || curBlock->size < size) {
            curBlock = curBlock->next;
        } else { //找到符合要求的块
            break;
        }
    }
#endif

#ifdef MeM_MEMORY_REQUEST_STRATEGY_LESS_FRAGMENTATION
    MeM_BlockHead *bestBlock = NULL;
    size_t bestBlockSize = MeM_MEMORY_POOL_SIZE;
    while (curBlock) {
        if (curBlock->available == true && curBlock->size >= size) { //找到一块可用
            if(curBlock.size==size){//刚刚好！就是它了
                bestBlock = curBlock;
                bestBlockSize = curBlock->size;
                break;
            }
            if (curBlock->size < bestBlockSize) {
                bestBlock = curBlock;
                bestBlockSize = curBlock->size;
            }
        }
        curBlock = curBlock->next;
    }
    curBlock = bestBlock;

#endif

    if (!curBlock) { //没找到合适的
        return NULL;
    }
    //切割当前块
    curBlock->available = false; //标记为占用
    //将当前块切割成两块，前者被占用，后者为未被占用的空闲空间
    MeM_BlockHead *newBlock = (MeM_BlockHead *)((unsigned int)curBlock + size);
    if (newBlock != curBlock->next) { //此块大小不是刚刚好的，才需要分块，不然可能产生神奇的闭环
        newBlock->available = true;
        newBlock->size = curBlock->size - size;
        newBlock->next = curBlock->next;
        curBlock->size = size;
        curBlock->next = newBlock;
    }
    rtnAddr = (void *)((unsigned int)curBlock + sizeof(MeM_BlockHead));
    return rtnAddr;
}

/**
 * @brief 申请一块内存,相当于malloc，
 *
 * @param size 申请块大小
 * @return void* 块内存地址
 */
void *MeM_Request(size_t size) {
    //实际需要申请的空间大小，包含块头数据，申请的内存大小实际是块大小的整数倍,下面一行就是对齐整数倍用的...不需要太理解，反正最后结果是最接近但不小于实际值的块大小整数倍就是了
    size_t actSize = ((size + sizeof(MeM_BlockHead)) / MeM_MEMORY_BLOCK_SIZE + MeM_MEMORY_REQUEST_TAIL + (((size + sizeof(MeM_BlockHead)) % MeM_MEMORY_BLOCK_SIZE == 0) ? 0 : 1)) * MeM_MEMORY_BLOCK_SIZE;
    void *rtnAddr = NULL; //返回可用的地址
    rtnAddr = MeM_Request_Base(actSize);
    if (rtnAddr == NULL) {                   //没找到可用的块
        MeM_Tidy();                          //整理一次试试
        rtnAddr = MeM_Request_Base(actSize); //再申请一次，还是没有就摆烂了
    }
    return rtnAddr;
}

/**
 * @brief 释放一块内存，相当于free
 *
 * @param addr 待释放内存地址
 */
void MeM_Release(void *addr) {
    addr = (void *)((unsigned int)addr - sizeof(MeM_BlockHead));
    ((MeM_BlockHead *)addr)->available = true;
}

/**
 * @brief 初始化内存管理
 *
 */
void MeM_Init() {
    MeM_BlockHead initHead;
    initHead.available = true;
    initHead.next = NULL;
    initHead.size = MeM_MEMORY_POOL_SIZE;
    MeM_BlockHead *poolHead = (MeM_BlockHead *)&MeM_MemoryPool[0];
    *poolHead = initHead; //"开个好头"
}
