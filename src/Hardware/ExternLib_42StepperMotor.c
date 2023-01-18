#include "STM32F10x_ExternLib_42StepperMotor.h"

void StepperMotor42_StepHandler(GenericObject object);

/**
 * @brief 初始化并生成一个步进电机对象
 *
 * @param GPIO_APositive A+对应的GPIO
 * @param GPIO_ANegative A-对应的GPIO
 * @param GPIO_BPositive B+对应的GPIO
 * @param GPIO_BNagitive B-对应的GPIO
 * @param stepAngle 步进角
 * @param easing 是否缓动
 * @return StepperMotor42_Object
 */
StepperMotor42_Object StepperMotor42_Initialize(GPIO_Object GPIO_APositive,
                                                GPIO_Object GPIO_ANegative,
                                                GPIO_Object GPIO_BPositive,
                                                GPIO_Object GPIO_BNagitive,
                                                float stepAngle,
                                                uint32_t easing) {
    GPIO_InitializeObject(GPIO_APositive, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
    GPIO_InitializeObject(GPIO_ANegative, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
    GPIO_InitializeObject(GPIO_BPositive, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
    GPIO_InitializeObject(GPIO_BNagitive, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
    StepperMotor42_Object newStepperMotor42Object = (StepperMotor42_Object)MeM_Request(sizeof(struct StepperMotor42_TypeDef)); //新建对象
    if (!newStepperMotor42Object)
        return NULL;
    newStepperMotor42Object->stepAngle = (uint32_t)(stepAngle / 2 * 100);
    newStepperMotor42Object->easingStep = easing;
    newStepperMotor42Object->next = EmptyObject;
    newStepperMotor42Object->GPIO_APositive = GPIO_APositive;
    newStepperMotor42Object->GPIO_ANegative = GPIO_ANegative;
    newStepperMotor42Object->GPIO_BPositive = GPIO_BPositive;
    newStepperMotor42Object->GPIO_BNegative = GPIO_BNagitive;
    return newStepperMotor42Object;
}

StepperMotor42Group_Object StepperMotor42_InitializeGroup(TIM_Object timer) {
    StepperMotor42Group_Object newStepperMotor42GroupObject = (StepperMotor42Group_Object)MeM_Request(sizeof(struct StepperMotor42Group_typeDef));
    TIM_InitializeObject(timer, 1000, StepperMotor42_StepHandler, (GenericObject)newStepperMotor42GroupObject, 0, 0);
    newStepperMotor42GroupObject->Timer = timer;
    newStepperMotor42GroupObject->object = EmptyObject;
    return newStepperMotor42GroupObject;
}

void StepperMotor42_AddToGroup(StepperMotor42Group_Object group, StepperMotor42_Object motor) {
    motor->next = EmptyObject;
    if (group->object == EmptyObject) {
        group->object = motor;
        return;
    }
    StepperMotor42_Object obj = group->object;
    while (obj->next != EmptyObject) {
        obj = obj->next;
    }
    obj->next = motor;
}

/**
 * @brief 步进电机旋转指定步数
 *
 * @param motorObj 指定一个步进电机对象
 * @param direction 方向
 * @param step 步数
 */
void StepperMotor42_RotateStep(StepperMotor42_Object motorObj, RotateDirectionTypeDef direction, unsigned int step) {
    motorObj->totalStep = step;
    motorObj->curStep = 0;
    motorObj->direction = direction;
    motorObj->status = StepperMotor42_Status_Running; //开始运行
}

/**
 * @brief 步进电机旋转指定角度
 *
 * @param motorObj 指定一个步进电机对象
 * @param direction 旋转方向
 * @param angle 角度
 */
void StepperMotor42_RotateAngle(StepperMotor42_Object motorObj, RotateDirectionTypeDef direction, float angle) {
    if (angle < 0) {
        angle = -angle;
        if (direction == RotateDirection_Clockwise)
            direction = RotateDirection_Counterclockwise;
        else
            direction = RotateDirection_Clockwise;
    }
    uint32_t step = (uint32_t)(angle * 100) / motorObj->stepAngle;
    StepperMotor42_RotateStep(motorObj, direction, step);
}

/**
 * @brief 步进电机旋转指定圈数
 *
 * @param motorObj 指定一个步进电机对象
 * @param direction 旋转方向
 * @param cycle 圈数
 */
void StepperMotor42_RotateCycle(StepperMotor42_Object motorObj, RotateDirectionTypeDef direction, float cycle) {
    if (cycle < 0) {
        cycle = -cycle;
        if (direction == RotateDirection_Clockwise)
            direction = RotateDirection_Counterclockwise;
        else
            direction = RotateDirection_Clockwise;
    }
    uint32_t angle;
    angle = (uint32_t)(cycle * 100) * 360;
    StepperMotor42_RotateStep(motorObj, direction, angle / motorObj->stepAngle);
}
void StepperMotor42_SetEndCallback(StepperMotor42_Object MotorObject, void (*endCallback)(GenericObject),GenericObject param){
    MotorObject->endCallback = endCallback;
    MotorObject->endCallbackParam = param;
}
/**
 * @brief 锁定一个步进电机
 *
 * @param motor 指定一个步进电机对象
 */
void StepperMotor42_Lock(StepperMotor42_Object MotorObject) {
    switch (MotorObject->step) {
        case 0: {
            GPIO_WritePin(MotorObject->GPIO_APositive, 1);
            GPIO_WritePin(MotorObject->GPIO_BPositive, 0);
            GPIO_WritePin(MotorObject->GPIO_ANegative, 0);
            GPIO_WritePin(MotorObject->GPIO_BNegative, 0);
        } break;
        case 1: {
            GPIO_WritePin(MotorObject->GPIO_APositive, 1);
            GPIO_WritePin(MotorObject->GPIO_BPositive, 1);
            GPIO_WritePin(MotorObject->GPIO_ANegative, 0);
            GPIO_WritePin(MotorObject->GPIO_BNegative, 0);
        } break;
        case 2: {
            GPIO_WritePin(MotorObject->GPIO_APositive, 0);
            GPIO_WritePin(MotorObject->GPIO_BPositive, 1);
            GPIO_WritePin(MotorObject->GPIO_ANegative, 0);
            GPIO_WritePin(MotorObject->GPIO_BNegative, 0);
        } break;
        case 3: {
            GPIO_WritePin(MotorObject->GPIO_APositive, 0);
            GPIO_WritePin(MotorObject->GPIO_BPositive, 1);
            GPIO_WritePin(MotorObject->GPIO_ANegative, 1);
            GPIO_WritePin(MotorObject->GPIO_BNegative, 0);
        } break;
        case 4: {
            GPIO_WritePin(MotorObject->GPIO_APositive, 0);
            GPIO_WritePin(MotorObject->GPIO_BPositive, 0);
            GPIO_WritePin(MotorObject->GPIO_ANegative, 1);
            GPIO_WritePin(MotorObject->GPIO_BNegative, 0);
        } break;
        case 5: {
            GPIO_WritePin(MotorObject->GPIO_APositive, 0);
            GPIO_WritePin(MotorObject->GPIO_BPositive, 0);
            GPIO_WritePin(MotorObject->GPIO_ANegative, 1);
            GPIO_WritePin(MotorObject->GPIO_BNegative, 1);
        } break;
        case 6: {
            GPIO_WritePin(MotorObject->GPIO_APositive, 0);
            GPIO_WritePin(MotorObject->GPIO_BPositive, 0);
            GPIO_WritePin(MotorObject->GPIO_ANegative, 0);
            GPIO_WritePin(MotorObject->GPIO_BNegative, 1);
        } break;
        case 7: {
            GPIO_WritePin(MotorObject->GPIO_APositive, 1);
            GPIO_WritePin(MotorObject->GPIO_BPositive, 0);
            GPIO_WritePin(MotorObject->GPIO_ANegative, 0);
            GPIO_WritePin(MotorObject->GPIO_BNegative, 1);
        } break;
    }
    MotorObject->status = StepperMotor42_Status_Lock;
}

/**
 * @brief 停用一个步进电机（即断电）
 *
 * @param MotorObject 指定一个步进电机对象
 */
void StepperMotor42_Disable(StepperMotor42_Object MotorObject) {
    GPIO_WritePin(MotorObject->GPIO_APositive, 0);
    GPIO_WritePin(MotorObject->GPIO_BPositive, 0);
    GPIO_WritePin(MotorObject->GPIO_ANegative, 0);
    GPIO_WritePin(MotorObject->GPIO_BNegative, 0);
    MotorObject->status = StepperMotor42_Status_Disable;
}

/**
 * @brief 通过每秒转动的圈数设置步进电机的转动速度，当速度过慢时不一定精确
 *
 * @param motorObj 指定一个步进电机对象
 * @param cyclePerSec 每秒转动的圈数
 */
void StepperMotor42_SetSpeedByCycle(StepperMotor42_Object motorObj, float cyclePerSec) {
    uint32_t stepPerSec = (cyclePerSec * 360 * 100) / motorObj->stepAngle;
    StepperMotor42_SetSpeedByStep(motorObj, stepPerSec);
}

void StepperMotor42_SetSpeedByStep(StepperMotor42_Object motorObj, uint32_t stepPerSec) {
    motorObj->speed = stepPerSec;
}

void StepperMotor42_SetRealSpeed(StepperMotor42_Object motorObj, uint32_t stepPerSec){
    uint32_t tickPerStep = (1000000 / STEPPER_MOTOR_42_DEFAULT_TIMER_INTERVAL) / stepPerSec;
    motorObj->curInterval = tickPerStep;
    motorObj->skipInterval = 0;
}

void StepperMotor42_StepHandler(GenericObject object) {
    StepperMotor42_Object MotorObject = ((StepperMotor42Group_Object)object)->object;
    while (MotorObject) {
        if (MotorObject->status == StepperMotor42_Status_Running) {
            if (MotorObject->skipInterval < MotorObject->curInterval) {
                MotorObject->skipInterval++;
            } else {
                MotorObject->skipInterval = 0;
                switch (MotorObject->step) {
                    case 0: {
                        GPIO_WritePin(MotorObject->GPIO_APositive, 1);
                        GPIO_WritePin(MotorObject->GPIO_BPositive, 0);
                        GPIO_WritePin(MotorObject->GPIO_ANegative, 0);
                        GPIO_WritePin(MotorObject->GPIO_BNegative, 0);
                    } break;
                    case 1: {
                        GPIO_WritePin(MotorObject->GPIO_APositive, 1);
                        GPIO_WritePin(MotorObject->GPIO_BPositive, 1);
                        GPIO_WritePin(MotorObject->GPIO_ANegative, 0);
                        GPIO_WritePin(MotorObject->GPIO_BNegative, 0);
                    } break;
                    case 2: {
                        GPIO_WritePin(MotorObject->GPIO_APositive, 0);
                        GPIO_WritePin(MotorObject->GPIO_BPositive, 1);
                        GPIO_WritePin(MotorObject->GPIO_ANegative, 0);
                        GPIO_WritePin(MotorObject->GPIO_BNegative, 0);
                    } break;
                    case 3: {
                        GPIO_WritePin(MotorObject->GPIO_APositive, 0);
                        GPIO_WritePin(MotorObject->GPIO_BPositive, 1);
                        GPIO_WritePin(MotorObject->GPIO_ANegative, 1);
                        GPIO_WritePin(MotorObject->GPIO_BNegative, 0);
                    } break;
                    case 4: {
                        GPIO_WritePin(MotorObject->GPIO_APositive, 0);
                        GPIO_WritePin(MotorObject->GPIO_BPositive, 0);
                        GPIO_WritePin(MotorObject->GPIO_ANegative, 1);
                        GPIO_WritePin(MotorObject->GPIO_BNegative, 0);
                    } break;
                    case 5: {
                        GPIO_WritePin(MotorObject->GPIO_APositive, 0);
                        GPIO_WritePin(MotorObject->GPIO_BPositive, 0);
                        GPIO_WritePin(MotorObject->GPIO_ANegative, 1);
                        GPIO_WritePin(MotorObject->GPIO_BNegative, 1);
                    } break;
                    case 6: {
                        GPIO_WritePin(MotorObject->GPIO_APositive, 0);
                        GPIO_WritePin(MotorObject->GPIO_BPositive, 0);
                        GPIO_WritePin(MotorObject->GPIO_ANegative, 0);
                        GPIO_WritePin(MotorObject->GPIO_BNegative, 1);
                    } break;
                    case 7: {
                        GPIO_WritePin(MotorObject->GPIO_APositive, 1);
                        GPIO_WritePin(MotorObject->GPIO_BPositive, 0);
                        GPIO_WritePin(MotorObject->GPIO_ANegative, 0);
                        GPIO_WritePin(MotorObject->GPIO_BNegative, 1);
                    } break;
                    default: {
                        MotorObject->step = 0;
                    } break;
                }
                if (MotorObject->direction == RotateDirection_Clockwise) {
                    MotorObject->step++;
                    if (MotorObject->step == 8)
                        MotorObject->step = 0;
                } else if (MotorObject->direction == RotateDirection_Counterclockwise) {
                    MotorObject->step--;
                    if (MotorObject->step == -1)
                        MotorObject->step = 7;
                }

                //计步
                MotorObject->curStep++;
                if (MotorObject->curStep == MotorObject->totalStep) { //转完了指定步数
                    StepperMotor42_Lock(MotorObject);
                    if (MotorObject->endCallback) {
                        MotorObject->endCallback(MotorObject->endCallbackParam);
                    }
                }

                //缓动
                if (MotorObject->easingStep * 2 > MotorObject->totalStep) { //缓动步数大于总步数
                    uint32_t middleStep = MotorObject->totalStep / 2;
                    if (MotorObject->curStep < middleStep) { //前半段
                        StepperMotor42_SetRealSpeed(MotorObject, MotorObject->speed * MotorObject->curStep / MotorObject->easingStep);
                    }
                    else {//后半段
                        StepperMotor42_SetRealSpeed(MotorObject, MotorObject->speed * (MotorObject->totalStep-MotorObject->curStep) / MotorObject->easingStep);
                    }
                } else { //缓动步数大于总步数
                    if(MotorObject->curStep<MotorObject->easingStep){
                        StepperMotor42_SetRealSpeed(MotorObject, MotorObject->speed * MotorObject->curStep / MotorObject->easingStep);
                    }
                    else if (MotorObject->totalStep-MotorObject->curStep<MotorObject->easingStep){
                        StepperMotor42_SetRealSpeed(MotorObject, MotorObject->speed * (MotorObject->totalStep-MotorObject->curStep) / MotorObject->easingStep);
                    }
                    else{
                        StepperMotor42_SetRealSpeed(MotorObject, MotorObject->speed);
                    }
                }
            }
        }
        MotorObject = MotorObject->next;
    }
}
