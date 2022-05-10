#ifndef _USART_H_
#define _USART_H_

#include "STM32F10x_ExternLib.h"
//串口缓存数据块大小
#define USART_BUFFER_BLOCK_SIZE 32
//串口高速缓冲大小，当读数据时普通缓冲不能写入时，会临时使用高速缓冲，调大缓冲块大小可以改善这一现象
#define USART_HIGN_SPEED_BUFFER_SIZE 16

//USART1
#define USART1_TX_PIN      GPIO_Pin_9
#define USART1_RX_PIN      GPIO_Pin_10
#define USART1_PORT        GPIOA
#define USART1_PORT_PERIPH RCC_APB2Periph_GPIOA

//USART3
#define USART2_TX_PIN      GPIO_Pin_2
#define USART2_RX_PIN      GPIO_Pin_3
#define USART2_PORT        GPIOA
#define USART2_PORT_PERIPH RCC_APB2Periph_GPIOA

//USART3
#define USART3_TX_PIN      GPIO_Pin_10
#define USART3_RX_PIN      GPIO_Pin_11
#define USART3_PORT        GPIOB
#define USART3_PORT_PERIPH RCC_APB2Periph_GPIOB

typedef struct USART_BufferBlockTypedef{
    uint8_t buffer[USART_BUFFER_BLOCK_SIZE];
    struct USART_BufferBlockTypedef *next;
} USART_BufferBlockTypedef;

typedef struct USART_PeriphTypedef{
    USART_TypeDef *USARTx;
    USART_BufferBlockTypedef *readBufferHead;
    USART_BufferBlockTypedef *readBufferTail;
    USART_BufferBlockTypedef *writeBufferHead;
    USART_BufferBlockTypedef *writeBufferTail;
    unsigned int readBufferCurPos;
    unsigned int readBufferEndPos;
    unsigned int writeBufferCurPos;
    unsigned int writeBufferEndPos;
    bool stopPendingBit;
    uint8_t highSpeedBuffer[USART_HIGN_SPEED_BUFFER_SIZE];
    bool useHighSpeedBuffer;
    unsigned int highSpeedBufferEndPos;
} *USART_Object;


USART_Object USART_Initialize(USART_TypeDef *USARTx, uint32_t BaudRate, uint16_t WordLength, uint16_t Parity, uint16_t StopBit, uint8_t ITPreemptionPriority, uint8_t ITSubPriority);
void USART_Write(USART_Object USARTPeriph, uint8_t dat);
uint8_t USART_Read(USART_Object USARTPeriph);
bool USART_DataAvaliable(USART_Object USARTPeriph);
void USART_WriteStr(USART_Object USARTPeriph, const char *str);


#endif
