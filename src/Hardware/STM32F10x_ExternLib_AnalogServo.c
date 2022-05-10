#include "STM32F10x_ExternLib_AnalogServo.h"

/**
 * @brief 初始化并生成一个模拟舵机对象
 *
 * @param PWM 指定一个初始化后的PWM对象
 * @param signalCycle 舵机一个信号周期时间长度us
 * @param minAngel 舵机的最小角度
 * @param minAngelSignalTime 舵机最小角度所对应的信号长度us
 * @param maxAngel 舵机的最大角度
 * @param maxAngelSignalTime 舵机最大角度所对应的信号长度us
 * @return AnalogServo_Object
 */
AnalogServo_Object AnalogServo_Initilize(PWM_Object PWM,
                                         uint32_t signalCycle,
                                         int32_t minAngel,
                                         uint32_t minAngelSignalTime,
                                         int32_t maxAngel,
                                         uint32_t maxAngelSignalTime) {
    AnalogServo_Object newAnalogServoObj = (AnalogServo_Object)MeM_Request(sizeof(struct AnalogServo_TypeDef));
    newAnalogServoObj->PWM = PWM;
    newAnalogServoObj->maxAngelSignalTime = maxAngelSignalTime;
    newAnalogServoObj->minAngelSignalTime = minAngelSignalTime;
    newAnalogServoObj->maxAngel = maxAngel;
    newAnalogServoObj->minAngel = minAngel;
    newAnalogServoObj->signalCycle = signalCycle;
    if (maxAngel < minAngel) {//贴心的防呆设计...
        newAnalogServoObj->maxAngelSignalTime = minAngelSignalTime;
        newAnalogServoObj->minAngelSignalTime = maxAngelSignalTime;
        newAnalogServoObj->maxAngel = minAngel;
        newAnalogServoObj->minAngel = maxAngel;
    }
    //将默认角度置零或接近0
    if (newAnalogServoObj->minAngel > 0){
        newAnalogServoObj->angel = newAnalogServoObj->minAngel;
    }
    else if(newAnalogServoObj->maxAngel<0){
        newAnalogServoObj->angel = newAnalogServoObj->maxAngel;
    }
    else{
        newAnalogServoObj->angel = 0;
    }
    PWM_SetCycle(newAnalogServoObj->PWM, signalCycle);
    AnalogServo_SetAngel(newAnalogServoObj, newAnalogServoObj->angel);
    return newAnalogServoObj;
}


/**
 * @brief 获取舵机当前的角度，用手掰的不算
 * 
 * @param servo 指定一个模拟舵机对象
 */
int32_t AnalogServo_GetAngel(AnalogServo_Object servo){
    return servo->angel;
}

/**
 * @brief 设置舵机的角度
 * 
 * @param servo 指定一个模拟舵机对象
 * @param angel 指定角度
 */
void AnalogServo_SetAngel(AnalogServo_Object servo,int32_t angel){
    if(angel<servo->minAngel)
        angel = servo->minAngel;
    else if(angel>servo->maxAngel)
        angel = servo->maxAngel;
    servo->angel = angel;
    uint16_t duty = PWM_MaxDuty * (servo->minAngelSignalTime + ((servo->maxAngelSignalTime - servo->minAngelSignalTime) * (angel - servo->minAngel) / (servo->maxAngel - servo->minAngel))) / servo->signalCycle;
    PWM_SetDuty(servo->PWM, duty);
}

/**
 * @brief 舵机在当前角度的基础上旋转某个角度
 * 
 * @param servo 指定一个模拟舵机对象
 * @param angel 指定角度，正增负减
 */
void AnalogServo_RotateAngel(AnalogServo_Object servo,int32_t angel){
    AnalogServo_SetAngel(servo, servo->angel + angel);
}
