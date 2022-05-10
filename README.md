# 序

## 速览

​	这是基于STM32F10x标准库构建的扩展库，旨在以**简洁清晰**且**面向对象**的方式快速**初始化和操作单片机外设**和**驱动部分常见硬件**，并基本兼容标准库。

​	本库正在发展中，预计未来将支持大部分外设和常用硬件，并计划推出基于HAL库的版本。

## 关于作者

**GitHub** @Challenger_0 是你李某人

**Email** Challenger__@Outlook.com

**Bilibili** @是你李某人

## 为什么要写这个库

​	李某人也不想写初始化和外设驱动，小细节太多，写一次错一次，就算改现成的代码，也不知道会漏改什么东西，换个芯片还得重新写，还不如一次写好以后都不改了……



# 概览

## 命名规范、类型和常量定义

- 定义`GenericObject`为**通用对象**。

  任何对象都可以转换为通用对象，通用对象也可以转换为任何对象。

- 定义`EmptyObject`为**空对象常量**，类型为通用对象。



- 所有类型和函数都以其外设或硬件的名称开头，例如`GPIO_xxx`。

- 本库中所有的对象类型都以`xxx_Object`命名。

- 初始化函数以`xxx_Initialize`命名。

  如果初始化成功，将返回生成的对象；否则，将返回空对象。



## 实现方法

​	对象都是以结构体指针的方式实现的，你可以使用`->`运算符访问它的成员，但是我**不建议直接修改其中的内容**，除非你知道会造成的影响。用标准库和本扩展库操作对象是安全的，对于外设对象，使用`->`运算符即可取得适用于标准库的参数。



## 注意事项

​	由于没有出现需要销毁对象的场景，所有暂时还没有提供有效的销毁对象的方法，如果真的要销毁对象，可以调用`DistoryObject(GenericObject object)`函数，但是要保证该对象下的子对象都被正确销毁，**否则会导致内存泄漏**。

​	对象生成的数量是无上限的，而外设的数目是有限的。一般来说，**一个外设对应一个对象**，如果为一个外设生成多个对象，将出现意料之外的问题。

# 外设部分

## GPIO

### GPIO对象

GPIO的对象是`GPIO_Object`，它的定义如下

~~~
typedef struct GPIO_PeriphTypeDef{
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin_x;
} * GPIO_Object
~~~



### GPIO_Object GPIO_Initialize

该函数可以按照所给出的参数初始化GPIO接口，并生成对应的GPIO对象。

#### 函数原型

~~~
GPIO_Object GPIO_Initialize(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin_x, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed)
~~~

#### 函数参数

- **GPIOx** 指定一组GPIO，可为`GPIOA`至`GPIOG`
- **GPIO_Pin_x** 指定**一个**GPIO，可为`GPIO_Pin_0 `至`GPIO_Pin_15`
- **GPIO_Mode** 指定GPIO模式
- **GPIO_Speed** 指定GPIO速度

#### 返回值

如果初始化成功，将返回一个GPIO对象，否则，返回空对象。

#### 函数功能

- 打开对应GPIO的时钟
- 如果GPIO模式为复用开漏或复用推挽，还会打开复用时钟
- 将对应GPIO设置为指定的模式和速度。
- 生成对应GPIO对象



###  GPIO_GenerateUninitialize

​	生成一个未初始化的对象，该对象可以被`GPIO_InitializeObject`函数或其他的一些函数初始化。

#### 函数原型

~~~
GPIO_GenerateUninitialize(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin_x);
~~~

#### 函数参数

- **GPIOx** 指定一组GPIO，可为`GPIOA`至`GPIOG`
- **GPIO_Pin_x** 指定**一个**GPIO，可为`GPIO_Pin_0 `至`GPIO_Pin_15`

#### 返回值

如果初始化成功，将返回一个GPIO对象，否则，返回空对象。

#### 函数功能

- 生成对应GPIO对象

### 别催了在写了



## Timer

## PWM

## USART



# 常用硬件部分

## 模拟舵机

## 42步进电机



# 软件部分

## 内存管理

## 三角函数表

