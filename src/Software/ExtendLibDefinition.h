/**
 * @file ExternLibDefinition.h
 * @author 
 * @brief 扩展库相关定义
 * @version 0.1
 * @date 2022-06-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _EXTERN_LIB_DEFINITION_H_
#define _EXTERN_LIB_DEFINITION_H_

#ifndef NULL
    #define NULL 0
#endif

#define StaticObject(type, name)                      \
    static struct type##_TypeDef name##_Storage; \
    type name = &name##_Storage;                 \


#define ExtendLib_NewObject(type) (type)MeM_Request(ExtendLib_GetObjectSize(type))
#define ExtendLib_GetObjectSize(type) sizeof(struct type##_TypeDef)


typedef void *GenericObject; //通用对象
extern const GenericObject EmptyObject; //空对象
typedef char *ByteBuffer;
typedef void (*CallbackFunction_TypeDef)(GenericObject);
extern const float INF;//无穷大


#endif

