#ifndef _EVENT_H_
#define _EVENT_H_

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "ExtendLibDefinition.h"
#include "MemoryManage.h"

typedef struct Event_TypeDef{
    bool enable;//事件是否启用
    bool allowRepeat;//是否允许重复执行 true:处理事件之前，事件触发了多少次，回调函数就执行多少次 false:无论触发了多少次，只要触发过，回调函数只执行一次
    unsigned int repeatTimes;//重复次数
    void (*callback)(GenericObject param);
    GenericObject callbackParam;
    struct Event_TypeDef *next;
} *Event_Object;



void Event_Trig(Event_Object evObj);
int Event_Run(Event_Object evObj);
void Event_RunAll(void);
Event_Object Event_Initialize(void (*callback)(GenericObject param), GenericObject callbackParam, bool allowRepeat, bool enable);
void Event_Distory(Event_Object evObj);
void Event_Config(Event_Object evObj, bool enable);

#endif


