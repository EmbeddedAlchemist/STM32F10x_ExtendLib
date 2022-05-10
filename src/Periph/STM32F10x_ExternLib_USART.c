#include "STM32F10x_ExternLib_USART.h"
#include "stdlib.h"
#include "string.h"

/////////声明/////////////
bool USART_ReadDataAvaliable(USART_Object USARTPeriph);
bool USART_WriteDataAvaliable(USART_Object USARTPeriph);
void USART_ReadBufferWrite(USART_Object USARTPeriph, uint8_t dat);
uint8_t USART_ReadBufferRead(USART_Object USARTPeriph);
void USART_WriteBufferWrite(USART_Object USARTPeriph, const uint8_t *dat, unsigned int num);
uint8_t USART_WriteBufferRead(USART_Object USARTPeriph);
void USART_Write_Base(USART_Object USARTPeriph);
void USART_Read_Base(USART_Object USARTPeriph);

////////正文//////////

/**
 * @brief 完成当前字节传输后停止发送
 *
 */
void USART_StopWrite(USART_Object USARTPeriph) {
    if (!USART_WriteDataAvaliable(USARTPeriph))
        return; //发送完了，那没事了
    USARTPeriph->stopPendingBit = true;
    while (USARTPeriph->stopPendingBit == true && USART_WriteDataAvaliable(USARTPeriph)) {
    }
    USARTPeriph->stopPendingBit = false;
    return;
}

/**
 * @brief 恢复传输
 *
 * @param USARTPeriph
 */
void USART_StartWrite(USART_Object USARTPeriph) {
    if (!USART_WriteDataAvaliable(USARTPeriph))
        return;                    //没啥可发送的，摆烂就好
    USART_Write_Base(USARTPeriph); //手动送第一字节
}

static USART_Object USART1_Periph = NULL, USART2_Periph = NULL, USART3_Periph = NULL;

/**
 * @brief 底层写
 *
 * @param USARTPeriph
 */
void USART_Write_Base(USART_Object USARTPeriph) {
    if (USART_WriteDataAvaliable(USARTPeriph)) {
        USART_SendData(USARTPeriph->USARTx, USART_WriteBufferRead(USARTPeriph));
    }
}

/**
 * @brief 底层读
 *
 * @param USARTPeriph
 */
void USART_Read_Base(USART_Object USARTPeriph) {
    if (USARTPeriph->useHighSpeedBuffer) {
        if (USARTPeriph->highSpeedBufferEndPos < USART_HIGN_SPEED_BUFFER_SIZE) {
            USARTPeriph->highSpeedBuffer[USARTPeriph->highSpeedBufferEndPos] = USART_ReceiveData(USARTPeriph->USARTx);
            USARTPeriph->highSpeedBufferEndPos++;
        } else
            return; //高速缓冲已满，放弃新数据
    } else
        USART_ReadBufferWrite(USARTPeriph, USART_ReceiveData(USARTPeriph->USARTx));
}

/**
 * @brief 查看接收缓冲区是否有新数据可读
 *
 * @param USARTPeriph
 * @return true
 * @return false
 */
bool USART_ReadDataAvaliable(USART_Object USARTPeriph) {
    return !(USARTPeriph->readBufferHead == USARTPeriph->readBufferTail && USARTPeriph->readBufferCurPos == USARTPeriph->readBufferEndPos);
}

/**
 * @brief 查看发送缓冲区是否有数据可读
 *        [ ⚠警告 ]无数据可读时一律返回0x00，清先检查是否有数据可读；读出的数据会立刻从缓冲区内抹掉
 *
 * @param USARTPeriph
 * @return true
 * @return false
 */
bool USART_WriteDataAvaliable(USART_Object USARTPeriph) {
    return !(USARTPeriph->writeBufferHead == USARTPeriph->writeBufferTail && USARTPeriph->writeBufferCurPos == USARTPeriph->writeBufferEndPos);
}
/**
 * @brief 向接收缓冲写一字节（提前malloc警告）
 *
 * @param USARTPeriph
 * @param dat
 * @return true
 * @return false
 */
void USART_ReadBufferWrite(USART_Object USARTPeriph, uint8_t dat) {
    USARTPeriph->readBufferTail->buffer[USARTPeriph->readBufferEndPos] = dat;
    USARTPeriph->readBufferEndPos++;
    if (USARTPeriph->readBufferEndPos == USART_BUFFER_BLOCK_SIZE) {                                               //一个块写完
        USARTPeriph->useHighSpeedBuffer = true;                                                                   //申请新块可能需要时间，此时接收到的数据先存入高速缓存中
        USARTPeriph->readBufferTail->next = (USART_BufferBlockTypedef *)malloc(sizeof(USART_BufferBlockTypedef)); //申请新块
        if (!USARTPeriph->readBufferTail->next) {
            USARTPeriph->useHighSpeedBuffer = false; //恢复正常缓冲模式
            return;                                  //申请不到新块，摆大烂
        }
        USARTPeriph->readBufferTail = USARTPeriph->readBufferTail->next; //尾块指针指向下一块
        USARTPeriph->readBufferEndPos = 0;                               //字节指针归零
        for (unsigned int i = 0; i < USARTPeriph->highSpeedBufferEndPos; i++) {
            USART_ReadBufferWrite(USARTPeriph, USARTPeriph->highSpeedBuffer[i]); //从高速缓冲中取回数据
        }
        USARTPeriph->highSpeedBufferEndPos = 0;
        USARTPeriph->useHighSpeedBuffer = false; //恢复正常缓冲模式
    }
}

/**
 * @brief 从接收缓冲读一字节
 *         [ ⚠警告 ]无数据可读时一律返回0x00，清先检查是否有数据可读；读出的数据会立刻从缓冲区内抹掉
 *
 * @param USARTPeriph
 * @return uint8_t
 */
uint8_t USART_ReadBufferRead(USART_Object USARTPeriph) {
    uint8_t dat = 0x00;
    if (USART_ReadDataAvaliable(USARTPeriph)) {
        dat = USARTPeriph->readBufferHead->buffer[USARTPeriph->readBufferCurPos];
        USARTPeriph->readBufferCurPos += 1;
        if (USARTPeriph->readBufferCurPos == USART_BUFFER_BLOCK_SIZE) {
            USART_BufferBlockTypedef *oldBlock = USARTPeriph->readBufferHead;
            USARTPeriph->readBufferHead = USARTPeriph->readBufferHead->next; //指向下一节点
            USARTPeriph->readBufferCurPos = 0;
            free(oldBlock);
        }
    }
    return dat;
}

/**
 * @brief 向发送缓冲写入（提前malloc警告）
 *
 * @param USARTPeriph
 * @param dat
 * @return true
 * @return false
 */
void USART_WriteBufferWrite(USART_Object USARTPeriph, const uint8_t *dat, unsigned int num) {
    USART_StopWrite(USARTPeriph); //停止传输，防止缓冲区冲突
    for (unsigned int i = 0; i < num; i++) {
        USARTPeriph->writeBufferTail->buffer[USARTPeriph->writeBufferEndPos] = dat[i];
        USARTPeriph->writeBufferEndPos++;
        if (USARTPeriph->writeBufferEndPos == USART_BUFFER_BLOCK_SIZE) {                                               //一个块写完
            USARTPeriph->writeBufferTail->next = (USART_BufferBlockTypedef *)malloc(sizeof(USART_BufferBlockTypedef)); //申请新块
            if (!USARTPeriph->writeBufferTail->next) {
                USART_StartWrite(USARTPeriph); //写完之前剩下的数据
                return;                        //申请不到新块，放弃写入数据
            }
            USARTPeriph->writeBufferTail = USARTPeriph->writeBufferTail->next; //尾块指针指向下一块
            USARTPeriph->writeBufferEndPos = 0;                                //字节指针归零
        }
    }
    USART_StartWrite(USARTPeriph);
}

/**
 * @brief 从发送缓冲读一字节
 *
 * @param USARTPeriph
 * @return uint8_t
 */
uint8_t USART_WriteBufferRead(USART_Object USARTPeriph) {
    uint8_t dat = 0x00;
    if (USART_WriteDataAvaliable(USARTPeriph)) {
        dat = USARTPeriph->writeBufferHead->buffer[USARTPeriph->writeBufferCurPos];
        USARTPeriph->writeBufferCurPos += 1;
        if (USARTPeriph->writeBufferCurPos == USART_BUFFER_BLOCK_SIZE) {
            USART_BufferBlockTypedef *oldBlock = USARTPeriph->writeBufferHead;
            USARTPeriph->writeBufferHead = USARTPeriph->writeBufferHead->next; //指向下一节点
            USARTPeriph->writeBufferCurPos = 0;
            free(oldBlock);
        }
    }
    return dat;
}

//////////////外部接口/////////////////

/**
 * @brief USART写一字节
 *
 * @param USARTPeriph
 * @param dat
 */
void USART_Write(USART_Object USARTPeriph, uint8_t dat) {
    USART_WriteBufferWrite(USARTPeriph, &dat, 1);
}

/**
 * @brief USART读一字节，如无内容可读返回0x00，使用前先验证是否可读
 *
 * @param USARTPeriph
 * @return uint8_t
 */
uint8_t USART_Read(USART_Object USARTPeriph) {
    return USART_ReadBufferRead(USARTPeriph);
}

/**
 * @brief 验证是否有数据可读
 *
 * @param USARTPeriph
 * @return true
 * @return false
 */
bool USART_DataAvaliable(USART_Object USARTPeriph) {
    return USART_ReadDataAvaliable(USARTPeriph);
}

void USART_WriteStr(USART_Object USARTPeriph, const char *str) {
    USART_WriteBufferWrite(USARTPeriph, (const uint8_t *)str, strlen(str));
}

/**
 * @brief 初始化并生成一个USART对象
 * 
 * @param USARTx 指定一个USART外设
 * @param BaudRate 比特率
 * @param WordLength 字长
 * @param Parity 
 * @param StopBit 停止位
 * @param ITPreemptionPriority 中断主优先级
 * @param ITSubPriority 中断从优先级
 * @return USART_PeriphTypedef* 
 */
USART_Object USART_Initialize(USART_TypeDef *USARTx, uint32_t BaudRate, uint16_t WordLength, uint16_t Parity, uint16_t StopBit, uint8_t ITPreemptionPriority, uint8_t ITSubPriority) {
    GPIO_InitTypeDef GPIO_Tx_InitData, GPIO_Rx_InitData;
    USART_InitTypeDef USART_InitData;
    NVIC_InitTypeDef NVIC_InitData;

    USART_InitData.USART_BaudRate = BaudRate;
    USART_InitData.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitData.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitData.USART_Parity = Parity;
    USART_InitData.USART_StopBits = StopBit;
    USART_InitData.USART_WordLength = WordLength;

    GPIO_Tx_InitData.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Tx_InitData.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Rx_InitData.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Rx_InitData.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    NVIC_InitData.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitData.NVIC_IRQChannelPreemptionPriority = ITPreemptionPriority;
    NVIC_InitData.NVIC_IRQChannelSubPriority = ITSubPriority;

    USART_Object *selectedUSARTPeriph;

    switch ((intptr_t)USARTx) {
        case (unsigned int)USART1:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
            RCC_APB2PeriphClockCmd(USART1_PORT_PERIPH, ENABLE);
            GPIO_Tx_InitData.GPIO_Pin = USART1_TX_PIN;
            GPIO_Rx_InitData.GPIO_Pin = USART1_RX_PIN;
            NVIC_InitData.NVIC_IRQChannel = USART1_IRQn;
            selectedUSARTPeriph = &USART1_Periph;
            GPIO_Init(USART1_PORT, &GPIO_Tx_InitData);
            GPIO_Init(USART1_PORT, &GPIO_Rx_InitData);
            break;
        case (unsigned int)USART2:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
            RCC_APB2PeriphClockCmd(USART2_PORT_PERIPH, ENABLE);
            GPIO_Tx_InitData.GPIO_Pin = USART2_TX_PIN;
            GPIO_Rx_InitData.GPIO_Pin = USART2_RX_PIN;
            NVIC_InitData.NVIC_IRQChannel = USART2_IRQn;
            selectedUSARTPeriph = &USART2_Periph;
            GPIO_Init(USART2_PORT, &GPIO_Tx_InitData);
            GPIO_Init(USART2_PORT, &GPIO_Rx_InitData);
            break;
        case (unsigned int)USART3:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
            RCC_APB2PeriphClockCmd(USART3_PORT_PERIPH, ENABLE);
            GPIO_Tx_InitData.GPIO_Pin = USART3_TX_PIN;
            GPIO_Rx_InitData.GPIO_Pin = USART3_RX_PIN;
            NVIC_InitData.NVIC_IRQChannel = USART3_IRQn;
            selectedUSARTPeriph = &USART3_Periph;
            GPIO_Init(USART3_PORT, &GPIO_Tx_InitData);
            GPIO_Init(USART3_PORT, &GPIO_Rx_InitData);
            break;
        default:
            return NULL;
    }

    *selectedUSARTPeriph = (USART_Object)MeM_Request(sizeof(struct USART_PeriphTypedef));

    (*selectedUSARTPeriph)->USARTx = USARTx;

    (*selectedUSARTPeriph)->useHighSpeedBuffer = false;
    (*selectedUSARTPeriph)->highSpeedBufferEndPos = 0;
    (*selectedUSARTPeriph)->stopPendingBit = false;
    (*selectedUSARTPeriph)->readBufferCurPos = (*selectedUSARTPeriph)->readBufferEndPos = 0;                                    //重置字节指针
    (*selectedUSARTPeriph)->readBufferHead = (*selectedUSARTPeriph)->readBufferTail = malloc(sizeof(USART_BufferBlockTypedef)); //分配缓存
    (*selectedUSARTPeriph)->writeBufferCurPos = (*selectedUSARTPeriph)->writeBufferEndPos = 0;                                                    //重置字节指针
    (*selectedUSARTPeriph)->writeBufferHead = (*selectedUSARTPeriph)->writeBufferTail = malloc(sizeof(USART_BufferBlockTypedef));                 //分配缓存
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    NVIC_Init(&NVIC_InitData);
    USART_Init((*selectedUSARTPeriph)->USARTx, &USART_InitData);
    USART_ITConfig((*selectedUSARTPeriph)->USARTx, USART_IT_TC, ENABLE);
    USART_ITConfig((*selectedUSARTPeriph)->USARTx, USART_IT_RXNE, ENABLE);
    USART_Cmd((*selectedUSARTPeriph)->USARTx, ENABLE);

    return *selectedUSARTPeriph;
}

//=============================中断处理(尽量不要更改)===========================//

void USART_IRQHandler(USART_Object USARTPeriph) {
    if (USART_GetITStatus(USARTPeriph->USARTx, USART_IT_RXNE) == SET) { // RI
        USART_ClearITPendingBit(USARTPeriph->USARTx, USART_IT_RXNE);
        USART_Read_Base(USARTPeriph);
    }
    if (USART_GetITStatus(USARTPeriph->USARTx, USART_IT_TC) == SET) { // TI
        USART_ClearITPendingBit(USARTPeriph->USARTx, USART_IT_TC);
        if (USARTPeriph->stopPendingBit == true) {
            USARTPeriph->stopPendingBit = false;
            return;
        }
        USART_Write_Base(USARTPeriph);
    }
}

void USART1_IRQHandler() {
    USART_IRQHandler(USART1_Periph);
}

void USART2_IRQHandler() {
    USART_IRQHandler(USART2_Periph);
}

void USART3_IRQHandler() {
    USART_IRQHandler(USART3_Periph);
}
