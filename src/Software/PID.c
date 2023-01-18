#include "PID.h"
#include "MemoryManage.h"
#include "ExtendLibDefinition.h"

/**
 * @brief 增量式PID定义
 * 
 */
/**
 * @brief 增量式PID定义
 *
 */
typedef struct PID_Object_Increasement_TypeDef {
    float (*UpdateFunc)(struct PID_Object_Increasement_TypeDef* PIDObj, float actualVal);
    float Kp;
    float ki;
    float Kd;
    float targetVal;
    float outputVal;
    float err1, err2, err3;
} *PID_Object_Increasement;


float PID_Update_Increasement(PID_Object_Increasement PIDObj, float ActuralVal);


/**
 * @brief 初始化一个PID对象
 *
 * @param mode 指定PID模式
 * @return PID_Object
 */
PID_Object PID_Initialize(PID_Mode mode, float Kp, float Ki, float Kd) {
    PID_Object newPIDObj = NULL;
    switch (mode) {
    case PID_Mode_Increasement: {
        newPIDObj = (PID_Object)malloc(sizeof(struct PID_Object_Increasement_TypeDef));
        ((PID_Object_Increasement)newPIDObj)->err1 = 0;
        ((PID_Object_Increasement)newPIDObj)->err2 = 0;
        ((PID_Object_Increasement)newPIDObj)->err3 = 0;
        ((PID_Object_Increasement)newPIDObj)->UpdateFunc = PID_Update_Increasement;
    } break;
    case PID_Mode_Location: {

    }break;
    }
    newPIDObj->Kp = Kp;
    newPIDObj->ki = Ki;
    newPIDObj->Kd = Kd;
    newPIDObj->outputVal = 0;
    newPIDObj->targetVal = 0;
    return newPIDObj;
}


/**
 * @brief 增量式PID更新函数
 *
 * @param PIDObj
 * @param ActuralVal
 * @return float
 */
float PID_Update_Increasement(PID_Object_Increasement PIDObj, float ActuralVal) {
    PIDObj->err1 = PIDObj->targetVal - ActuralVal;
    float increasement = PIDObj->Kp * (PIDObj->err1 - PIDObj->err2) + PIDObj->ki * PIDObj->err1 + PIDObj->Kd * (PIDObj->err1 - 2 * PIDObj->err2 + PIDObj->err3);
    PIDObj->outputVal += increasement;
    PIDObj->err3 = PIDObj->err2;
    PIDObj->err2 = PIDObj->err1;
    return PIDObj->outputVal;
}



/**
 * @brief 更新PID值，输入实际值，输出更新后的输出值
 *
 * @param PIDObj 指定一个PID对象
 * @param ActualVal 实际值
 * @return float 输出值
 */
float PID_Update(PID_Object PIDObj, float ActualVal) {
    return PIDObj->UpdateFunc(PIDObj, ActualVal);
}


/**
 * @brief 设定PID目标值
 *
 * @param PIDObj 指定一个PID对象
 * @param targetVal 指定目标值
 */
void PID_SetTargetValue(PID_Object PIDObj, float targetVal) {
    PIDObj->targetVal = targetVal;
}


/**
 * @brief 获取输出值
 *
 * @param PIDObj 指定一个PID对象
 * @return float 输出值
 */
float PID_GetOutputValue(PID_Object PIDObj) {
    return PIDObj->outputVal;
}



/**
 * @brief 设置PID参数
 *
 * @param PIDObj 指定一个PID对象
 * @param Kp 比例系数
 * @param Ki 积分系数
 * @param Kd 微分系数
 */
void PID_SetParam(PID_Object PIDObj, float Kp, float Ki, float Kd) {

}
