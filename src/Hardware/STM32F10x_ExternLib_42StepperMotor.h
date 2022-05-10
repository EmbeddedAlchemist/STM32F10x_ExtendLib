#ifndef _STM32F10X_EXTERN_LIB_42_STEPPER_MOTOR_H_
#define _STM32F10X_EXTERN_LIB_42_STEPPER_MOTOR_H_

#include "STM32F10x_ExternLib.h"
#include "rotateDirection.h"

typedef enum StepperMotor42_StatusTypeDef {
    StepperMotor42_Status_Disable, //未在运行的断电状态
    StepperMotor42_Status_Running, //正在运行
    StepperMotor42_Status_Lock     //未在运行，锁死
} StepperMotor42_StatusTypeDef;

typedef struct StepperMotor42_TypeDef {
    struct StepperMotor42_TypeDef *next;
    GPIO_Object GPIO_APositive;          // GPIO A+
    GPIO_Object GPIO_ANegative;          // GPIO A-
    GPIO_Object GPIO_BPositive;          // GPIO B+
    GPIO_Object GPIO_BNegative;          // GPIO B-
    uint32_t stepAngle;                  //步进角，单位0.01°；
    uint32_t easingStep;                 //缓动步数；就是距离开始或结束多少步开始缓动，0为不使用
    RotateDirectionTypeDef direction;    //旋转方向
    int32_t step;                        //当前在4步的第几步
    uint32_t totalStep;                  //总步数
    uint32_t curStep;                    //当前步数
    uint32_t curInterval;                //当前步进时间间隔
    uint32_t skipInterval;               //待跳过的间隔
    uint32_t speed;                      //速度，步每秒
    void (*endCallback)(GenericObject);  //结束回调
    GenericObject endCallbackParam;      //结束回调函数参数
    StepperMotor42_StatusTypeDef status; //该电机是否在工作：

} * StepperMotor42_Object;

typedef struct StepperMotor42Group_typeDef {
    TIM_Object Timer;             // 使用的定时器
    StepperMotor42_Object object; //组中的步进电机对象
} * StepperMotor42Group_Object;

//定时器所用定时器中断周期(us)，降低这个值有助于提升速度精度，但是可能因中断频率过高阻碍正常程序运行...
#define STEPPER_MOTOR_42_DEFAULT_TIMER_INTERVAL 1000

StepperMotor42_Object StepperMotor42_Initialize(GPIO_Object GPIO_APositive, GPIO_Object GPIO_ANegative, GPIO_Object GPIO_BPositive, GPIO_Object GPIO_BNagitive, float stepAngle, uint32_t easing);
StepperMotor42Group_Object StepperMotor42_InitializeGroup(TIM_Object timer);
void StepperMotor42_AddToGroup(StepperMotor42Group_Object group, StepperMotor42_Object motor);
void StepperMotor42_RotateStep(StepperMotor42_Object motorObj, RotateDirectionTypeDef direction, unsigned int step);
void StepperMotor42_RotateAngle(StepperMotor42_Object motorObj, RotateDirectionTypeDef direction, float angle);
void StepperMotor42_RotateCycle(StepperMotor42_Object motorObj, RotateDirectionTypeDef direction, float cycle);
void StepperMotor42_Lock(StepperMotor42_Object MotorObject);
void StepperMotor42_Disable(StepperMotor42_Object MotorObject);
void StepperMotor42_SetSpeedByCycle(StepperMotor42_Object motorObj, float cyclePerSec);
void StepperMotor42_SetSpeedByStep(StepperMotor42_Object motorObj, uint32_t stepPerSec);
void StepperMotor42_SetEndCallback(StepperMotor42_Object MotorObject, void (*endCallback)(GenericObject),GenericObject param);
#endif
