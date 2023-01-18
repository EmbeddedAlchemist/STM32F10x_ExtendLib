



# 序

## 速览

​	这是基于STM32F10x标准库构建的扩展库，旨在以**简洁清晰**且**面向对象**的方式快速**初始化和操作单片机外设**和**驱动部分常见硬件**，并基本兼容标准库。

​	本库正在发展中，预计未来将支持大部分外设和常用硬件，并计划推出基于HAL库的版本。

## 关于作者

**GitHub** @Challenger_0 是你李某人

**Email** Challenger__@Outlook.com

**Bilibili** @是你李某人



## 致谢

- **GUET CS Technical Association(GCTA) | 桂林电子科技大学计算机与信息安全学院大学生科技协会**
  - **宣传页** [桂电三院科协 (kexie.space)](https://hello.kexie.space/)
  - **GitHub** [GUET CS Technical Association(GCTA) · GitHub](https://github.com/sanyuankexie)



## 为什么要写这个库

​	李某人也不想写初始化和外设驱动，小细节太多，写一次错一次，就算改现成的代码，也不知道会漏改什么东西，换个芯片还得重新写，还不如一次写好以后都不改了……

## 支持的外设/硬件/软件算法

（持续更新中）

- **外设**
  - GPIO | 通用输入输出接口
  - Timer | 定时器
    - PWM | 脉冲宽度调制
  - USART | 通用同步异步串行通讯
  - [TODO] I2C | 内部集成电路
    - Hardware I2C | 硬件I2C
    - Software I2C | 软件I2C
  - SPI | 串行外设接口
  - [TODO] ADC | 数模转换
  - [TODO] FLASH
  - [TODO] NVIC
- **硬件**
  - Analog Servo | 模拟舵机
  - 42 Stepprt Motor | 42步进电机
  - [TODO…] 超声测距
  - [TODO…] MPU6050 | 6轴加速度传感器
- **软件和算法**
  - Memory Manage | 内存管理
    - 三角函			数表
  - [TODO] “EVENT” | “事件”
  - [TODO] PID
  - [TODO] 矩阵运算
  - [TODO] 卡尔曼滤波







# 引入的新概念

## 对象

​	类似于支持面向对象语言的对象，不多解释。

​	对象都是以结构体指针的方式实现的，你可以使用`->`运算符访问它的成员，但是我**不建议直接修改其中的内容**，除非你知道会造成的影响。用标准库和本扩展库操作对象是安全的，对于外设对象，使用`->`运算符即可取得适用于标准库的参数。

​	==看这里==	例如，GPIO对象`GPIO_Object`本质是`struct GPIO_PeriphTypeDef *`，但是我更希望你在编程时将它看作一个**对象**，或者说是一个**新类型**，而不必惦记着它是一个指针，而产生畏难情绪，同时也方便了代码编写，不必大量使用`*`和`&`，所以才有了这样的包装。

## “事件”

​	本库提出了“**事件**”的概念，是类似于中断的处理突发情况的产物。

​	与中断不同的是：在中断发生时，会打断当前程序的运行，处理完中断任务后才会恢复；“事件”发生时不会阻塞程序运行，而是等待当前主循环执行完一次，或手动运行“事件”处理函数时才会处理“事件”任务。

**大白话：把任务拖到有空的时候再执行**

中断和事件各有其优势：

- **中断**  侧重**实时性高**且**运算简单**、**执行时间短**的任务

- **事件**  侧重**实时性低**但**运算复杂**、**执行耗时**的任务

  一些事件的实现方式一样依赖中断实现。所以，事件并不是取代中断的产物，只是中断的衍生物，旨在更灵活地处理情况。





# 概览 / 使用前必读

## 命名规范、类型和常量定义

### 类型定义

- 定义`void*`类型为**通用对象类型**`GenericObject`。

  任何对象都可以转换为通用对象，通用对象也可以转换为任何对象。

- 定义`char*`类型为**字节缓冲**`ByteBuffer`.

- 定义`void (*)(GenericObject)`类型为**回调函数指针**`CallbackFunc`.

### 常量定义

- 定义`(GenericObject)0`为**空对象常量**`EmptyObject`，类型为通用对象，相当于`NULL`

### 命名规范

- 所有类型和函数都以其外设或硬件的名称开头，例如`GPIO_xxx`。

- 本库中所有的对象类型都以`xxx_Object`命名。

- 初始化函数以`xxx_Initialize`命名。

  如果初始化成功，将返回生成的对象；否则，将返回空对象。

> ### 关于对象其实是对象的引用
>
> ​	这里点出了通用对象和空对象的实现方法是指针，之前也提醒过对象的实现方法是指针，但是在实际使用的时候不需要时刻惦记着他是指针，把它当作一个新类型看待就好。指出的原因是实现时用到了指针，所以它有一些指针的特性，并不是完全的对象，只是对象的引用，例如
>
> ```C
> typedef struct Object_Typedef{
>     int key;
> } *Object;
> 
> ...
> 
> Object ObjectA = ... , ObjectB;//新建两个对象
> ObjectA -> key = 1; //向对象A赋值
> ObjectB = ObjectA; //对象A赋值给对象B
> ObjectB -> key = 0; // 改变对象B的某个成员
> //此时对象A也会一并被改变
> ObjectA -> key == 0; //true
> ```
>
> ​	上面的例子展示了这一特性，对象B的更改一并影响了对象A。因为B和A实际是对同一对象的引用，更改一个对象会影响到引用此对象的其他对象。
>
> ​	所以这里的建议是，不要创建多个引用，一个实际对象对应一个引用，这样即可将引用与对象等同。如果在两个函数中使用了同一个对象，则可以将其设为全局变量，如果使用跨文件，可以使用extern关键字扩展其作用域。一般来说，每个对象对应一个外设，实际情况也不允许为一个外设或硬件创建多个对象，但是可以使用多个引用来操作同一个对象，使用引用显然也更合适。
>
> ​	**如果一定要使用多个引用，需要在编程时记住，它们只是同一个对象的引用而不是互不相干的两个对象**

## 注意事项

- 本库基本与标准库兼容，所以在一般情况下，标准库和扩展库可以混合使用。但是，如果已经使用扩展库为一个外设生成了对象，而且扩展库提供了提供了所需功能，则不应该继续使用标准库初始化和操作该外设；操作对象时，对于扩展库没有提供的功能，可以使用标准库替代，但是如果扩展库提供了类似的功能，则应尽量使用扩展库，否则可能出现意料之外的错误。
- 由于没有出现需要销毁对象的场景，所有暂时还没有提供有效的销毁对象的方法，如果真的要销毁对象，可以调用`DistoryObject(GenericObject object)`函数，但是要保证该对象下的子对象都被正确销毁，**否则会导致内存泄漏**。
- 对象生成的数量是无上限的，而外设的数目是有限的。一般来说，**一个外设对应一个对象**，如果为一个外设生成多个对象，将出现意料之外的问题。



## 库函数部署

1.	将`./src`下的所有文件添加至工程文件中:

- `./src/Periph`下的文件是外设驱动，建议全部包含。
- `./src/Hardware`下的文件是常用硬件的驱动，可以按需删除(不删除也不要紧，反正编译器会优化掉)。
- `./src/Software`下的文件为软件算法，建议全部包含。至少包含`MemoryManage.c` `MemoryManage.h` `Event.c` `Event.h`，部分硬件驱动可能用到其他文件。

2. 在主函数前引用`STM32F10x_ExternLib.h`
3. 在主函数第一行(至少是扩展库函数被调用之前)插入`STM32F10x_ExternLib_Initialize();`，完成扩展库初始化。
4. 在主循环中(一般是第一行)插入`STM32F10x_ExternLib_MainLoopHandler();`。

e.g.

~~~C
//main.c
#include "STM32F10x_ExternLib.h"

void main(){
    //init
    STM32F10x_ExternLib_Initialize();
    for(;;){
        //Main Loop
        STM32F10x_ExternLib_MainLoopHandler();
    }
}
~~~







# 外设部分

## GPIO | 通用输入输出接口

### GPIO对象

GPIO的对象是`GPIO_Object`，它的定义如下

~~~C
typedef struct GPIO_PeriphTypeDef{
    GPIO_TypeDef *GPIOx;//GPIOA,GPIOB...
    uint16_t GPIO_Pin_x;//GPIO_Pin_0,GPIO_Pin_1...
} * GPIO_Object
~~~

------

### GPIO_Initialize

#### 函数功能

该函数可以按照所给出的参数初始化GPIO接口，并生成对应的GPIO对象。

- 打开对应GPIO的时钟
- 如果GPIO模式为复用开漏或复用推挽，还会打开复用时钟
- 将对应GPIO设置为指定的模式和速度。
- 生成对应GPIO对象

#### 函数原型

~~~C
GPIO_Object GPIO_Initialize(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin_x, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed)
~~~

#### 函数参数

- **GPIOx** 指定一组GPIO，可为`GPIOA`至`GPIOG`
- **GPIO_Pin_x** 指定**一个**GPIO，可为`GPIO_Pin_0 `至`GPIO_Pin_15`的**其中之一**
- **GPIO_Mode** 指定GPIO模式
- **GPIO_Speed** 指定GPIO速度

#### 返回值

如果初始化成功，将返回一个GPIO对象，否则，返回空对象。

#### 示例

```c
GPIO_Object GPIO_PB15, GPIO_PC0;//这个对象名字是什么无所谓
//将PB15初始化为50MHZ、复用开漏，并打开GPIOB时钟和复用时钟
GPIO_PB15 = GPIO_Initialize(GPIOB, GPIO_Pin_15, GPIO_Mode_AF_OD, GPIO_Speed_50MHz);
//将PC0初始化为50MHZ、推挽输出，并打开GPIOC时钟。
GPIO_PC0 = GPIO_Initialize(GPIOC, GPIO_Pin_0, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
```

------

###  GPIO_GenerateUninitialize

#### 函数功能

生成一个未初始化的对象，该对象可以被`GPIO_InitializeObject`函数或其他的一些函数初始化。

- 生成对应GPIO对象

#### 函数原型

~~~c
GPIO_GenerateUninitialize(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin_x);
~~~

#### 函数参数

- **GPIOx** 指定一组GPIO，可为`GPIOA`至`GPIOG`
- **GPIO_Pin_x** 指定**一个**GPIO，可为`GPIO_Pin_0 `至`GPIO_Pin_15`

#### 返回值

如果初始化成功，将返回一个GPIO对象，否则，返回空对象。

------

### GPIO_InitializeObject

#### 函数功能

初始化已生成但未初始化的GPIO对象，例如`GPIO_GenerateUninitialize`生成的对象。

- 打开对应GPIO的时钟
- 如果GPIO模式为复用开漏或复用推挽，还会打开复用时钟
- 将对应GPIO设置为指定的模式和速度。

#### 函数原型

```C
void GPIO_InitializeObject(GPIO_Object GPIO_Periph, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed);
```

#### 函数参数

- **GPIO_Periph** 指定一个GPIO对象
- **GPIO_Mode** 指定GPIO模式
- **GPIO_Speed** 指定GPIO速度

------

### GPIO_WritePin

#### 函数功能

将指定GPIO置`0`或`1`。

#### 函数原型

```C
void GPIO_WritePin(GPIO_Object GPIO_Periph, unsigned int val);
```

#### 函数参数

- **GPIO_Periph** 指定一个GPIO对象
- **val**待写入的值
  - **0** 逻辑0
  - **非0** 逻辑1

------

### GPIO_ReadPinInput

#### 函数功能

读GPIO输入值

#### 函数原型

```C
uint8_t GPIO_ReadPinInput(GPIO_Object GPIO_Periph);
```

#### 函数参数

- **GPIO_Periph** 指定一个GPIO对象

#### 返回值

- GPIO输入值
  - **0** 逻辑0
  - **1** 逻辑1

------

### GPIO_ReadPinOutput

#### 函数功能

读GPIO输出值

#### 函数原型

```C
uint8_t GPIO_ReadPinOutput(GPIO_Object GPIO_Periph);
```

#### 函数参数

- **GPIO_Periph**指定一个GPIO对象

#### 返回值

- GPIO输出值
  - **0** 逻辑0
  - **1** 逻辑1



## Timer |  定时器

### Timer对象

Timer的对象是`TIM_Object`，它的定义如下

```C
typedef struct TIM_PeriphTypeDef{
    TIM_TypeDef *TIMx;//TIM1,TIM2...
    uint8_t IRQn_Channel;
    void (*interruptUpdateCallback)(GenericObject);//回调函数
    void *interruptUpdateCallbackParam;//回调函数参数
} *TIM_Object;
```

---

### TIM_Initialize

#### 函数功能

初始化并生成定时器对象。

- 打开指定定时器的时钟
- 将定时器的时钟分割`TIM_ClockDivision`设置为0
- 将定时器计数模式设置为向上计数
- 将定时器的重复计数次数`TIM_RepetitionCounter`设置为0
- 将定时器重装值设置为`TIM_PERIOD_DEFAULT`（定义在.h文件中，默认为7199）
- 计算并设置定时器预分频系数，使定时器的溢出间隔接近指定时间
- 允许在定时器工作时更改预分频系数
- 如果设置了中断回调函数，将使能指定中断通道，并将中断优先级配置为指定的值
- 使能对应定时器
- 生成定时器对象

#### 函数原型

```C
TIM_Object TIM_Initialize(TIM_TypeDef *TIMx, unsigned int interval, void (*interruptUpdateCallback)(GenericObject), GenericObject interruptUpdateCallbackParam, uint8_t IRQnChannel, uint8_t preemptionPriority, uint8_t subPriority);
```

#### 函数参数

- **TIMx** 指定一个定时器外设，可为`TIM1`,`TIM2`…

- **interval** 定时器溢出时间间隔，单位us

- **interruptUpdateCallback** 定时器溢出中断回调函数，有一个类型为`GenericObject`的参数

  如不使用，置`NULL`

- **interruptUpdateCallbackParam** 中断回调函数的参数

  如不使用，置`EmptyObject`，仅在指定了溢出中断回调函数时生效

- **IRQnChannel** 定时器溢出中断通道，例如`TIM3_IRQn`

- **preemptionPriority** 溢出中断主优先级

  仅在指定了溢出中断回调函数时生效

- **subPriority** 溢出中断从优先级

  仅在指定了溢出中断回调函数时生效

#### 返回值

- 如果生成成功，返回生成的定时器对象

  否则，返回空对象

---

### TIM_InitializeAdvance

#### 函数功能

初始化并生成定时器对象（高级版），可以自定义重装值和预分频系数。

- 打开指定定时器的时钟
- 将定时器的时钟分割`TIM_ClockDivision`设置为0
- 将定时器计数模式设置为向上计数
- 将定时器的重复计数次数`TIM_RepetitionCounter`设置为0
- 将定时器重装值设置为指定值
- 将定时器预分频系数设置为指定值
- 允许在定时器工作时更改预分频系数
- 如果设置了中断回调函数，将使能指定中断通道，并将中断优先级配置为指定的值
- 使能对应定时器
- 生成定时器对象

#### 函数原型

```C
TIM_Object TIM_InitializeAdvance(TIM_TypeDef *TIMx, uint16_t period, uint16_t prescaler, void (*interruptUpdateCallback)(GenericObject), GenericObject interruptUpdateCallbackParam, uint8_t IRQnChannel, uint8_t preemptionPriority, uint8_t subPriority);
```

#### 函数参数

- **TIMx** 指定一个定时器外设，可为`TIM1`,`TIM2`…

- **interval** 定时器溢出时间间隔，单位us

- **period** 重装值

- **prescaler** 预分频系数

- **interruptUpdateCallback** 定时器溢出中断回调函数，有一个类型为`GenericObject`的参数

  如不使用，置`NULL`

- **interruptUpdateCallbackParam** 中断回调函数的参数

  如不使用，置`EmptyObject`，仅在指定了溢出中断回调函数时生效

- **IRQnChannel** 定时器溢出中断通道，例如`TIM3_IRQn`

- **preemptionPriority** 溢出中断主优先级

  仅在指定了溢出中断回调函数时生效

- **subPriority** 溢出中断从优先级

  仅在指定了溢出中断回调函数时生效

#### 返回值

- 如果生成成功，返回生成的定时器对象

  否则，返回空对象

---

### TIM_GenerateUninitialize

#### 函数功能

生成一个未初始化的定时器对象

- 生成定时器对象

#### 函数原型

```C
TIM_Object TIM_GenerateUninitialize(TIM_TypeDef *TIMx, uint8_t IRQn_Channel);
```

#### 函数参数

- **TIMx** 指定一个定时器外设，可为`TIM1`,`TIM2`…

- **IRQnChannel** 定时器溢出中断通道，例如`TIM3_IRQn`

#### 返回值

- 如果生成成功，返回生成的定时器对象

  否则，返回空对象

---

### TIM_InitializeObject

#### 函数功能

初始化未初始化的定时器对象

- 打开指定定时器的时钟
- 将定时器的时钟分割`TIM_ClockDivision`设置为0
- 将定时器计数模式设置为向上计数
- 将定时器的重复计数次数`TIM_RepetitionCounter`设置为0
- 将定时器重装值设置为`TIM_PERIOD_DEFAULT`（定义在.h文件中，默认为7199）
- 计算并设置定时器预分频系数，使定时器的溢出间隔接近指定时间
- 允许在定时器工作时更改预分频系数
- 如果设置了中断回调函数，将使能指定中断通道，并将中断优先级配置为指定的值
- 使能对应定时器

#### 函数原型

~~~C
void TIM_InitializeObject(TIM_Object TIM_Periph, unsigned int interval, void (*interruptUpdateCallback)(GenericObject), GenericObject interruptUpdateCallbackParam, uint8_t preemptionPriority, uint8_t subPriority);
~~~

#### 函数参数

- **TIM_Periph** 指定一个未初始化的定时器对象

- **interval** 定时器溢出时间间隔，单位us

- **period** 重装值

- **prescaler** 预分频系数

- **interruptUpdateCallback** 定时器溢出中断回调函数，有一个类型为`GenericObject`的参数

  如不使用，置`NULL`

- **interruptUpdateCallbackParam** 中断回调函数的参数

  如不使用，置`EmptyObject`，仅在指定了溢出中断回调函数时生效

- **preemptionPriority** 溢出中断主优先级

  仅在指定了溢出中断回调函数时生效

- **subPriority** 溢出中断从优先级

  仅在指定了溢出中断回调函数时生效

---

### TIM_InitializeAdvanceObject

#### 函数功能

初始化未初始化的定时器对象（高级版），可以自定义重装值和预分频系数。

- 打开指定定时器的时钟
- 将定时器的时钟分割`TIM_ClockDivision`设置为0
- 将定时器计数模式设置为向上计数
- 将定时器的重复计数次数`TIM_RepetitionCounter`设置为0
- 将定时器重装值设置为指定值
- 将定时器预分频系数设置为指定值
- 允许在定时器工作时更改预分频系数
- 如果设置了中断回调函数，将使能指定中断通道，并将中断优先级配置为指定的值
- 使能对应定时器

#### 函数原型

~~~C
void TIM_InitializeAdvanceObject(TIM_Object TIM_Periph, uint16_t period, uint16_t prescaler, void (*interruptUpdateCallback)(GenericObject), GenericObject interruptUpdateCallbackParam, uint8_t preemptionPriority, uint8_t subPriority);
~~~

#### 函数参数

- **TIM_Periph** 指定一个定时器对象

- **interval** 定时器溢出时间间隔，单位us

- **period** 重装值

- **prescaler** 预分频系数

- **interruptUpdateCallback** 定时器溢出中断回调函数，有一个类型为`GenericObject`的参数

  如不使用，置`NULL`

- **interruptUpdateCallbackParam** 中断回调函数的参数

  如不使用，置`EmptyObject`，仅在指定了溢出中断回调函数时生效

- **preemptionPriority** 溢出中断主优先级

  仅在指定了溢出中断回调函数时生效

- **subPriority** 溢出中断从优先级

  仅在指定了溢出中断回调函数时生效

---

### TIM_GetReloadValue

#### 函数功能

获取定时器的重装值

#### 函数原型

~~~C
uint16_t TIM_GetReloadValue(TIM_Object TIM_Periph);
~~~

#### 函数参数

- **TIM_Periph** 指定一个定时器对象

#### 返回值

定时器的重装值

---

### TIM_GetPrescalerValue

#### 函数功能

获取定时器的预分频系数

#### 函数原型

~~~C
uint16_t TIM_GetPrescalerValue(TIM_Object TIM_Periph);
~~~

#### 函数参数

- **TIM_Periph** 指定一个定时器对象

#### 返回值

定时器的预分频系数

---

### TIM_SetReloadValue

#### 函数功能

设置定时器重装值

#### 函数原型

~~~C
void TIM_SetReloadValue(TIM_Object TIM_Periph, uint16_t reloadValue);
~~~

#### 函数参数

- **TIM_Periph** 指定一个定时器对象
- **reloadValue** 重装值

---

### TIM_SetPrescalerValue

#### 函数功能

设置定时器预分频系数

#### 函数原型

~~~C
void TIM_SetPrescalerValue(TIM_Object TIM_Periph, uint16_t prescalerValue);
~~~

#### 函数参数

- **TIM_Periph** 指定一个定时器对象
- **prescalerValue** 预分频系数

---

### TIM_SetInterval

#### 函数功能

保持重装值不变，通过改变预分频系数设置定时器溢出的时间间隔

#### 函数原型

~~~C
void TIM_SetInterval(TIM_Object TIM_Periph, unsigned int interval);
~~~

#### 函数参数

- **TIM_Periph** 指定一个定时器对象
- **interval** 时间间隔，单位us

---

### TIM_SetIntervalByReload

#### 函数功能

保持预分频系数不变，通过改变重装值设置定时器溢出的时间间隔

#### 函数原型

~~~C
void TIM_SetIntervalByReload(TIM_Object TIM_Periph, unsigned int interval);
~~~

#### 函数参数

- **TIM_Periph** 指定一个定时器对象
- **interval** 时间间隔，单位us





## PWM | 脉冲宽度调制

#### PWM对象

PWM对象是`PWM_Object`，它的定义如下

~~~C
typedef struct PWM_TypeDef{
    struct TIM_PeriphTypeDef *TIM_Periph;//PWM所使用的定时器
    uint16_t TIM_Channel;//在指定计时器的第几个通道
    uint16_t duty;
} *PWM_Object;
~~~



### PWM_Initialize

#### 函数功能

初始化并生成一个PWM对象

- 如果指定的定时器是`TIM1` `TIM8` `TIM15` `TIM16` `TIM17`，则使用`TIM_CtrlPWMOutputs`打开它们的PWM输出
- 初始化通道对应的GPIO接口为复用推挽模式，速度50MHz
- 设置PWM模式为`TIM_OCMode_PWM1`
- 设置有效极性为高电平
- 设置比较值为0
- 使能指定通道的预装载寄存器
- 生成PWM对象

#### 函数原型

~~~C
PWM_Object PWM_Initialize(struct TIM_PeriphTypeDef * TIM_Periph, uint8_t TIM_Channel_x, GPIO_Object GPIO_Periph)
~~~

#### 函数参数

- **TIM_Periph** 指定一个**已初始化**的定时器对象

  PWM的周期取决于该定时器的溢出周期

- **TIM_Channel_x** 指定要使用的PWM通道，例如`TIM_Channel_1`

- **GPIO_Periph** **未初始化**的GPIO对象，该通道对应的GPIO接口



### PWM_SetDuty

#### 函数功能

设置PWM的占空比

#### 函数原型

~~~C
void PWM_SetDuty(PWM_Object PWM, uint16_t duty);
~~~

#### 函数参数

- **PWM** 指定一个PWM对象

- **duty** 占空比

  ==注意== 占空比的取值是`0`到`PWM_MaxDuty`(常量值65535，定义在STM32F10x_ExternLib_PWM.h)，`0`表示0%，`PWM_MaxDuty`表示100%，**无论重装值是多少**。



### PWM_SetCycle

#### 函数功能

设置PWM的周期

==注意== 这将一并改变该定时器下其他PWM的周期

#### 函数原型

~~~C
void PWM_SetCycle(PWM_Object PWM, unsigned int cycle);
~~~

#### 函数参数

- **PWM** 指定一个PWM对象
- **cycle** 周期，单位us



### PWM_GetDuty

#### 函数功能

获取PWM的**理论占空比**，其实是最后一次设置的占空比值

#### 函数原型

~~~C
uint16_t PWM_GetDuty(PWM_Object PWM);
~~~

#### 函数参数

- **PWM** 指定一个PWM对象

#### 返回值

**理论占空比** 

==注意== 占空比的取值是`0`到`PWM_MaxDuty`(常量值65535，定义在STM32F10x_ExternLib_PWM.h)，`0`表示0%，`PWM_MaxDuty`表示100%，**无论重装值是多少**。



### PWM_GetActualDuty

#### 函数功能

获取PWM的**实际占空比**

因为占空比被映射到了`0`到`PWM_MaxDuty`，但是实际上占空比的精度取决于定时器重装值，所以设置的理论占空比会与实际存在偏差。

#### 函数原型

~~~C
uint16_t PWM_GetActualDuty(PWM_Object PWM);
~~~

#### 函数参数

- **PWM** 指定一个PWM对象

#### 返回值

**实际占空比** 

==注意== 占空比的取值是`0`到`PWM_MaxDuty`(常量值65535，定义在STM32F10x_ExternLib_PWM.h)，`0`表示0%，`PWM_MaxDuty`表示100%，**无论重装值是多少**。



## USART | 通用同步异步串行通讯



## NVIC | 中断向量控制器

​	其实这玩意根本没必要做成对象，基本初始化一次就废的玩意，但是为了代码风格同一…



## SPI | 串行外设接口

### 硬件抽象层示意

![spiStruct.drawio](README.assets/spiStruct.drawio.svg)



# 硬件部分

## 模拟舵机

### 模拟舵机对象

模拟舵机对象是`AnalogServo_Object`，它的定义如下

~~~C
typedef struct AnalogServo_TypeDef {
    PWM_Object PWM;
    int32_t angel;
    int32_t signalCycle;        //信号周期，单位us,嗯，不要改成uint，因为强制转换会出问题
    int32_t minAngel;           //最小角度
    int32_t minAngelSignalTime; //最小角度对应的信号时长us
    int32_t maxAngel;           //最大角度
    int32_t maxAngelSignalTime; //最大角度对应的信号时长us
} * AnalogServo_Object;
~~~



### AnalogServo_Initilize

#### 函数功能

初始化一个模拟舵机对象

#### 函数原型

~~~C
AnalogServo_Object AnalogServo_Initilize(PWM_Object PWM,
                                         uint32_t signalCycle,
                                         int32_t minAngel,
                                         uint32_t minAngelSignalTime,
                                         int32_t maxAngel,
                                         uint32_t maxAngelSignalTime);
~~~

#### 函数参数

- **PWM** 指定一个**已初始化**的PWM对象，用作舵机信号的发生器
- **signalCycle** 舵机所需的PWM信号周期，单位us
- **minAngel** 舵机的最小角度(deg，当然rad也可以，不过考虑到是整形，deg好一些)，可以为负，怎么方便怎么来
- **minAngelSignalTime** 最小角度对应的信号长度，单位us
- **maxAngel** 舵机的最大角度，也可以为负，怎么方便怎么来
- **maxAngelSignalTime** 最小角度对应的信号长度，单位us

#### 返回值

如果初始化成功，返回生成的模拟舵机对象，否则，返回空对象。



### AnalogServo_GetAngel

#### 函数功能

获取舵机的角度

==注意== 不是舵机实际的角度，而是最后一次设置的角度。因为舵机是不带反馈的，即使强行掰到偏离设定角度也不会有反馈信号，但是，如果舵机正常工作，它确实会保持在设定的角度。

#### 函数原型

~~~C
int32_t AnalogServo_GetAngel(AnalogServo_Object servo);
~~~

#### 函数参数

- **servo** 指定一个舵机对象

#### 返回值

舵机的角度



### AnalogServo_SetAngel

#### 函数功能

设置舵机的角度

#### 函数原型

~~~C
void AnalogServo_SetAngel(AnalogServo_Object servo, int32_t angel);
~~~

#### 函数参数

- **servo** 指定一个舵机对象
- **angel** 指定一个角度



### AnalogServo_RotateAngel

#### 函数功能

使舵机在当前角度的基础上旋转某个角度

#### 函数原型

~~~C
void AnalogServo_SetAngel(AnalogServo_Object servo, int32_t angel);
~~~

#### 函数参数

- **servo** 指定一个舵机对象
- **angel** 旋转的角度，正加负减



## 42步进电机



# 软件和算法部分

## 内存管理

因为标准库的`malloc`和`free`有时会出问题，然后就有了这个内存管理程序



## 三角函数表



## “EVENT” | 事件

​	其实“事件”本身的事件不依赖任何硬件功能，就归到了软件算法类，但某些事件的触发是要涉及中断的…

