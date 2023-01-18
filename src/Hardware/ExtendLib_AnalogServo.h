#ifndef _STM32F10X_EXTERNLIB_ANALOG_SERVO_H_
#define _STM32F10X_EXTERNLIB_ANALOG_SERVO_H_

#include "STM32F10x_ExtendLib.h"

typedef struct AnalogServo_TypeDef {
    PWM_Object PWM;
    float angel;
    float signalCycle;        //信号周期，单位us,嗯，不要改成uint，因为强制转换会出问题
    float minAngel;           //最小角度
    int32_t minAngelSignalTime; //最小角度对应的信号时长us
    float maxAngel;           //最大角度
    int32_t maxAngelSignalTime; //最大角度对应的信号时长us
} * AnalogServo_Object;

AnalogServo_Object AnalogServo_Initilize(PWM_Object PWM,
                                         uint32_t signalCycle,
                                         float minAngel,
                                         uint32_t minAngelSignalTime,
                                         float maxAngel,
                                         uint32_t maxAngelSignalTime);
float AnalogServo_GetAngel(AnalogServo_Object servo);
void AnalogServo_SetAngel(AnalogServo_Object servo, float angel);
void AnalogServo_RotateAngel(AnalogServo_Object servo, float angel);

#endif
