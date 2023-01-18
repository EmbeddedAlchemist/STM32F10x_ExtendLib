#include "Event.h"

static Event_Object EventLinkHead = NULL; //通过链表管理所有事件

/**
 * @brief 触发事件
 *
 * @param evObj
 */
__inline void Event_Trig(Event_Object evObj) {
    evObj->repeatTimes++;
}

/**
 * @brief 执行指定事件
 *
 * @param evObj
 * @return int 事件回调函数执行次数 0:0次，事件在此之前未被触发过 非0：触发过且已执行 -1：事件被禁用
 */
int Event_Run(Event_Object evObj) {
    if(!evObj->enable){
        return -1;
    }
    int tmp = evObj->repeatTimes;
    if (evObj->allowRepeat) {
        while(evObj->repeatTimes){
            evObj->repeatTimes--;
            evObj->callback(evObj->callbackParam);
        }
        return tmp;
    } else {
        if(evObj->repeatTimes){
            evObj->callback(evObj->callbackParam);
            evObj->repeatTimes = 0;
            return 1;
        }
    }
    return 0;
}

/**
 * @brief 执行全部事件
 *
 */
void Event_RunAll() {
    Event_Object tmp = EventLinkHead;
    while(tmp){
        Event_Run(tmp);
        tmp = tmp->next;
    }
}

Event_Object Event_Initialize(void (*callback)(GenericObject param), GenericObject callbackParam, bool allowRepeat, bool enable) {
    Event_Object newEvObj = (Event_Object)MeM_Request(sizeof(struct Event_TypeDef));
    newEvObj->callback = callback;
    newEvObj->callbackParam = callbackParam;
    newEvObj->allowRepeat = allowRepeat;
    newEvObj->enable = enable;
    newEvObj->next = EmptyObject;

    if (EventLinkHead == EmptyObject) {
        EventLinkHead = newEvObj;
    } else {
        Event_Object tmp = EventLinkHead;
        while (tmp->next != EmptyObject) {
            tmp = tmp->next;
        }
        tmp->next = newEvObj;
    }
    return newEvObj;
}

void Event_Distory(Event_Object evObj) {
    Event_Object tmp = EventLinkHead;
    while (tmp->next != evObj && tmp->next != EmptyObject) {
        tmp = tmp->next;
    }
    if (tmp->next != EmptyObject) {
        tmp->next = tmp->next->next;
    }
    DistoryObject(evObj);
}

__inline void Event_Config(Event_Object evObj, bool enable) {
    evObj->enable = enable;
}
