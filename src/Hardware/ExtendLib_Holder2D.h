#ifndef _EXTENDLIB_HOLDER_2D_
#define _EXTENDLIB_HOLDER_2D_

#include "STM32F10x_ExtendLib.h"
#include "ExtendLib_AnalogServo.h"

typedef struct Holder2D_Object_TypeDef{
    AnalogServo_Object servoX;
    AnalogServo_Object servoY;
} * Holder2D_Object;

typedef enum {
    Holder2D_Axis_X, // X轴
    Holder2D_Anis_Y  // Y轴
} Holder2D_Axis;

void Holder2D_Reset(Holder2D_Object obj);
Holder2D_Object Holder2D_Initialize(AnalogServo_Object servoX, AnalogServo_Object servoY);
void Holder2D_SetAngel(Holder2D_Object obj, float angelX, float angelY);
void Holder2D_RotateAngel(Holder2D_Object obj, float angelX, float angelY);
float Holder2D_GetAxisAngel(Holder2D_Object obj, Holder2D_Axis axis);
void Holder2D_SetAxisAngel(Holder2D_Object obj, Holder2D_Axis axis, float angel);
void Holder2D_RotateAxisAngel(Holder2D_Object obj, Holder2D_Axis axis, float angel);

#endif
