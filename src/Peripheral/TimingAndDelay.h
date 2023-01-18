/**
 * @file TimingAndDelay.h
 * @author your name (you@domain.com)
 * @brief 本文件将接管systick和RTC时钟，以实现延时和计时,并提供基础的日期时间计算功能
 * @version 0.1
 * @date 2022-02-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _TIMING_AND_DELAY_H_
#define _TIMING_AND_DELAY_H_

#include "stm32f10x.h"
#include "stdbool.h"

typedef struct Time_Typedef{
    uint16_t year;
    uint8_t month;
    uint8_t week;//0 SUN  1 MON  ...  6 SAT
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
}Time_Typedef;

extern volatile uint32_t TimingAndDelay_Millisecond;
extern Time_Typedef Timing_NowTime;
extern const uint8_t Timing_MonthDaysTable[];

void TimingAndDelay_Init(void);
void TimingAndDelay_InterruptHandler(void);
#define Timing_GetMillisecond() TimingAndDelay_Millisecond/* !< 获取程序运行的毫秒数 */
void Delay_Millisecond(uint32_t ms);
#define Timing_GetTime() Timing_NowTime
bool Timing_IsLeapYear(uint16_t year);
#define Timing_GetMonthDays(month) (Timing_MonthDaysTable[month-1])
void Timing_SetTime(Time_Typedef *time);
uint8_t Timing_GetWeekDay(uint16_t year, uint8_t month, uint8_t day);
uint32_t Timing_GetMicrosecond(void);
void Delay_MicroSecond(uint32_t us);

#endif // !_TIMING_AND_DELAY_H_
