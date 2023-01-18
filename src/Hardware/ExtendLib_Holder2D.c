#include "ExtendLib_Holder2D.h"


void Holder2D_Reset(Holder2D_Object obj){
    AnalogServo_SetAngel(obj->servoX, 0.0);
    AnalogServo_SetAngel(obj->servoY, 0.0);
}

Holder2D_Object Holder2D_Initialize(AnalogServo_Object servoX, AnalogServo_Object servoY){
    if(servoX==EmptyObject || servoY==EmptyObject)
        return EmptyObject;
    Holder2D_Object newHoder2DObj = ExtendLib_NewObject(Holder2D_Object);
    newHoder2DObj->servoX = servoX;
    newHoder2DObj->servoY = servoY;
    Holder2D_Reset(newHoder2DObj);
    return newHoder2DObj;
}

/**
 * @brief 设置云台的绝对角度
 * 
 * @param obj 指定一个云台对象
 * @param angelX X轴角度
 * @param angelY Y轴角度
 */
void Holder2D_SetAngel(Holder2D_Object obj ,float angelX, float angelY){
    AnalogServo_SetAngel(obj->servoX, angelX);
    AnalogServo_SetAngel(obj->servoY, angelY);
}

void Holder2D_RotateAngel(Holder2D_Object obj, float angelX, float angelY){
    AnalogServo_RotateAngel(obj->servoX, angelX);
    AnalogServo_RotateAngel(obj->servoY, angelY);
}


/**
 * @brief 获取云台指定轴线的角度
 * 
 * @param obj 指定一个云台对象
 * @param axis 指定一个轴线
 * @return float 
 */
float Holder2D_GetAxisAngel(Holder2D_Object obj, Holder2D_Axis axis){
    switch(axis){
        case Holder2D_Axis_X:{
            return AnalogServo_GetAngel(obj->servoX);
        }
        case Holder2D_Anis_Y:{
            return AnalogServo_GetAngel(obj->servoY);
        }
    }
    return 0;
}

/**
 * @brief 设置云台指定轴角度
 * 
 * @param obj 指定一个云台对象
 * @param axis 指定一个轴
 * @param angel 指定一个角度
 */
void Holder2D_SetAxisAngel(Holder2D_Object obj, Holder2D_Axis axis, float angel){
        switch(axis){
        case Holder2D_Axis_X:{
            AnalogServo_SetAngel(obj->servoX,angel);
        }break;
        case Holder2D_Anis_Y:{
            AnalogServo_SetAngel(obj->servoY,angel);
        }break;
    }
}

/**
 * @brief 云台指定轴旋转某个角度
 * 
 * @param obj 指定一个云台对象
 * @param axis 指定一个轴
 * @param angel 指定一个角度
 */
void Holder2D_RotateAxisAngel(Holder2D_Object obj, Holder2D_Axis axis, float angel){
        switch(axis){
        case Holder2D_Axis_X:{
            AnalogServo_RotateAngel(obj->servoX,angel);
        }break;
        case Holder2D_Anis_Y:{
            AnalogServo_RotateAngel(obj->servoY,angel);
        }break;
    }
}





