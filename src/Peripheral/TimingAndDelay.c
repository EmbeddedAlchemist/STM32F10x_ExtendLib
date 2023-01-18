#include "TimingAndDelay.h"

#define TIMING_RTC_CONFIGURED_BKP_FLAG    0xA5A5
#define TIMING_RTC_CONFIGURED_BKP_FLAG_DR BKP_DR1

//32位无符号整数形式的时间的基准年份，毋须担心千年虫问题，32位无符号整数能表示136年，足够把我送走
#define TIMING_UINT32_TIME_BASE_YEAR 1970

const uint8_t Timing_MonthDaysTable[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

uint32_t Timing_TimeToUInt32(Time_Typedef *time);
Time_Typedef Timing_UInt32ToTime(uint32_t time);

//现在的时间，由RTC秒中断处理函数维护
Time_Typedef Timing_NowTime;

volatile uint32_t TimingAndDelay_Millisecond = 0;

void Timing_RTCSecondInterruptHandler(void);
void TimingAndDelay_InterruptHandler(){
    static int count = 0;
    TimingAndDelay_Millisecond++;
    count++;
    if(count==1000){
        count = 0;
        Timing_RTCSecondInterruptHandler();
    }
}

void TimingAndDelay_SystickInit(){
    //SYSTICK初始化
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);//1ms
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//直接使用AHB时钟，即系统时钟不分频
    
}

void TimingAndDelay_RTCInit(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);//打开时钟
	PWR_BackupAccessCmd(ENABLE);	//允许写后备寄存器 
	if (BKP_ReadBackupRegister(TIMING_RTC_CONFIGURED_BKP_FLAG_DR) != TIMING_RTC_CONFIGURED_BKP_FLAG){//电源掉电			
		BKP_DeInit();//复位
		RCC_LSEConfig(RCC_LSE_ON);//打开外部低速晶振
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
            ;
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//时钟源选择外部低速晶振
		RCC_RTCCLKCmd(ENABLE);
		RTC_WaitForLastTask();
		RTC_WaitForSynchro();//等待同步 //失去同步//狗头
		RTC_ITConfig(RTC_IT_SEC, ENABLE);//打开秒中断
		RTC_WaitForLastTask();
		RTC_EnterConfigMode();//进入配置模式
		RTC_SetPrescaler(32767);//1s
		RTC_WaitForLastTask();
		RTC_ExitConfigMode();
		BKP_WriteBackupRegister(TIMING_RTC_CONFIGURED_BKP_FLAG_DR, TIMING_RTC_CONFIGURED_BKP_FLAG);	//ÏòÖ¸¶¨µÄºó±¸¼Ä´æÆ÷ÖÐÐ´ÈëÓÃ»§³ÌÐòÊý¾Ý
	}
	else{//常规配置
		RTC_WaitForSynchro();
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		RTC_WaitForLastTask();
	}
    NVIC_InitTypeDef NVIC_InitData;
	NVIC_InitData.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitData.NVIC_IRQChannelPreemptionPriority = 0;//最高优先级爱抢抢吧
	NVIC_InitData.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitData.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitData);
    Time_Typedef time;
    time = Timing_UInt32ToTime(RTC_GetCounter());//读入时间
    Timing_SetTime(&time);
}


/**
 * @brief 初始化systick及其中断
 * 
 */
void TimingAndDelay_Init(){
    TimingAndDelay_SystickInit();
    //TimingAndDelay_RTCInit(); RTC晶振故障
    Timing_NowTime.day = 2;
    Timing_NowTime.month = 3;
    Timing_NowTime.hour = 10;
    Timing_NowTime.minute = 8;
    Timing_NowTime.second = 0;
    Timing_NowTime.week = 3;
    Timing_NowTime.year = 2022;
}


/**
 * @brief 判断当年是不是闰年
 * 
 * @param year 
 * @return true 是闰年
 * @return false 不是闰年
 */
bool Timing_IsLeapYear(uint16_t year){
    return (!(year % 4) && (year % 100)) || (!year % 400);
}

/**
 * @brief 利用基姆拉尔森计算公式计算指定日期的星期数
 * 
 * @param year 
 * @param month 
 * @param day 
 * @return uint8_t 
 */
uint8_t Timing_GetWeekDay(uint16_t year,uint8_t month,uint8_t day){
    if(month<=2)
        month += 12;
    return (day + 2 * month + 3 * (month + 1) / 5 + year + year / 4 - year / 100 + year / 400 + 1) % 7;//基姆拉尔森计算公式
}

//日期永远是个脑壳痛的东西
/**
 * @brief 将时间从结构体转化为32位无符号整数的形式
 * 
 * @param time 
 * @return uint32_t 
 */
uint32_t Timing_TimeToUInt32(Time_Typedef *time){
    static const uint16_t daysBeforeTheMonthTable[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    uint32_t sec = 0;
    for (uint16_t year = TIMING_UINT32_TIME_BASE_YEAR; year < time->year;year++){
        if(Timing_IsLeapYear(year))
            sec += 31622400;
        else
            sec += 31536000;
    }
    sec += daysBeforeTheMonthTable[time->month] + ((time->month > 2 && Timing_IsLeapYear(time->year)) ? 1 : 0);
    sec += time->day * 86400;
    sec += time->hour * 3600;
    sec += time->minute * 60;
    sec += time->second;
    return sec;
}

/**
 * @brief 将时间从32位无符号整数转为结构体的形式。运算比较复杂一般情况使用结构体形式的就好，整数形式的仅在读写rtc时钟时使用
 * 
 * @param time 
 * @return Time_Typedef 
 */
Time_Typedef Timing_UInt32ToTime(uint32_t time){
    unsigned int days = time / 86400,
                 hours = (time - days * 86400) / 3600,
                 minutes = (time - days * 86400 - hours * 3600) / 60,
                 seconds = time - hours * 86400 - minutes * 3600 - minutes * 60;
    unsigned int year;
    bool isLeapYear;
    for (year = TIMING_UINT32_TIME_BASE_YEAR ; days >= (isLeapYear = Timing_IsLeapYear(year + 1) ? 366 : 365) /*这里表示下一年是不是闰年*/; year++) {
        if(!isLeapYear)//明年时闰年今年肯定不是闰年
            days -= 365;//减去今年的天数
        else
            days -= 366;
    }
    //出循环后余下的days为不满一年的天数
    isLeapYear = Timing_IsLeapYear(year);//判断当年是不是闰年
    unsigned int month;
    for (month = 1; days + 1 >= Timing_MonthDaysTable[month - 1];month++){
        if(month==2){
            if(isLeapYear)
                days -= 29;
            else
                days -= 28;
        }
        else
            days -= Timing_MonthDaysTable[month - 1];
    }
    //此时剩余的days就是当月落单的天数了
    days++;//+1得到日期数
    Time_Typedef timeStrust;
    timeStrust.year = year;
    timeStrust.month = month;
    timeStrust.day = days;
    timeStrust.hour = hours;
    timeStrust.minute = minutes;
    timeStrust.second = seconds;
    timeStrust.week = Timing_GetWeekDay(year, month, days);
    return timeStrust;
}



void Timing_RTCSecondInterruptHandler(){
    //我知道这难看的要死，但这个简单的进位根本不需要维护，当看不见就好
    Timing_NowTime.second++;
    if(Timing_NowTime.second==60){
        Timing_NowTime.second = 0;
        Timing_NowTime.minute++;
        if(Timing_NowTime.minute==60){
            Timing_NowTime.minute = 0;
            Timing_NowTime.hour++;
            if(Timing_NowTime.hour==24){
                Timing_NowTime.hour = 0;
                Timing_NowTime.day++;
                if (Timing_NowTime.day == Timing_MonthDaysTable[Timing_NowTime.month] + 1 + ((Timing_NowTime.month == 2 && Timing_IsLeapYear(Timing_NowTime.year) ? 1 : 0))){
                    Timing_NowTime.day = 1;
                    Timing_NowTime.month++;
                    if(Timing_NowTime.month==13){
                        Timing_NowTime.month = 1;
                        Timing_NowTime.year++;
                    }
                }
            }
        }
    }
}

/**
 * @brief 延时指定毫秒
 * 
 * @param ms 
 */
void Delay_Millisecond(uint32_t ms){
    uint32_t endTime = Timing_GetMillisecond() + ms;
    while(Timing_GetMillisecond()<endTime)
        ;
}

/**
 * @brief 设置时间，指针指向的结构体的星期值和不合法之处会被修正。
 * 
 * @param time 
 */
void Timing_SetTime(Time_Typedef *time){
    if(time->day>Timing_MonthDaysTable[time->month-1]+ ((time->month == 2 && Timing_IsLeapYear(time->year) ? 1 : 0))){
        time->day = Timing_MonthDaysTable[time->month - 1] + ((time->month == 2 && Timing_IsLeapYear(time->year) ? 1 : 0));
    }
    time->week = Timing_GetWeekDay(time->year, time->month, time->day);
    RTC_SetCounter(Timing_TimeToUInt32(time));
    Timing_NowTime = *time;
}


void Delay_MicroSecond(uint32_t us) {
    while (us--) {
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP(); 
        __NOP();
    }
}



/**
 * @brief 获取程序运行的微秒数
 * 
 */
uint32_t Timing_GetMicrosecond(){
    return Timing_GetMillisecond() * 1000 + 1000 * (SysTick->LOAD - SysTick->VAL) / SysTick->LOAD;
}


void RTC_IRQHandler(void){
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET){
        Timing_RTCSecondInterruptHandler();
    }
	RTC_ClearITPendingBit(RTC_IT_SEC); 
	RTC_WaitForLastTask();
}



