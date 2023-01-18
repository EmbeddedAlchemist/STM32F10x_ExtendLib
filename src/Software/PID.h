#include <stdint.h>

typedef struct PID_Object_TypeDef {
    float (*UpdateFunc)(struct PID_Object_TypeDef* PIDObj, float actualVal);
    float Kp;
    float ki;
    float Kd;
    float targetVal;
    float outputVal;
} *PID_Object;


typedef enum PID_Mode {
    PID_Mode_Increasement, //增量式
    PID_Mode_Location      //位置式
} PID_Mode;
