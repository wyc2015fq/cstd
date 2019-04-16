# MCU上的代码执行时间 - 我相信...... - CSDN博客





2018年07月13日 22:00:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：809
所属专栏：[IoT与智能硬件](https://blog.csdn.net/column/details/19754.html)









在许多实时应用程序中，二八原则并不生效，CPU 可以花费95%(或更多)的时间在不到5% 的代码上。电动机控制、引擎控制、无线通信以及其他许多对时间敏感的应用程序都是如此。这些嵌入式系统通常是用c编写的，而且开发人员常常被迫对代码进行手工优化，可能会回到汇编语言，以满足性能的需求。测量代码部分的实际执行时间可以帮助找到代码中的热点。本文将说明如何可以方便地测量和显示在基于Cortex-M MCU的实时执行时间。

## 测量代码的执行时间

测量代码执行时间的方法有很多。作为一个嵌入式工程师，经常使用一个或多个数字输出和一个示波器。需要在执行要监视的代码之前设置一个高的输出，然后将输出降低。当然，在做这些之前有相当多的设置工作: 找到一个或多个自由输出，确保它们可以轻松访问，将端口配置为输出，编写代码，编译，设置范围等等。一旦有了一个信号，你可能需要对它进行一段时间的监视，以便看到最小值和最大值。 数字存储示波器使这个过程更容易，但是还有其他更简单的方法。

另一种测量执行时间的方法是使用可跟踪调试接口。只需要运行代码，查看跟踪，计算 delta时间(通常是手动的) ，并将CPU周期转换为微秒。不幸的是，这个跟踪给了一个执行的实例，可能不得不在追踪捕获中进一步查找最坏情况下的执行时间。这是一个乏味的过程。

## Cortex-M 周期计数器

在大多数Cortex-M的处理器中调试端口包含一个32位的自由运行计数器，它可以计算 CPU 的时钟周期。计数器是 Debug 观察和跟踪(DWT)模块的一部分，可以很容易地用于测量代码的执行时间。下面的代码是启用和初始化这个特性非常有用。

```
#define  ARM_CM_DEMCR      (*(uint32_t *)0xE000EDFC)

#define  ARM_CM_DWT_CTRL   (*(uint32_t *)0xE0001000)

#define  ARM_CM_DWT_CYCCNT (*(uint32_t *)0xE0001004)


if (ARM_CM_DWT_CTRL != 0) {        // See if DWT is available

    ARM_CM_DEMCR      |= 1 << 24;  // Set bit 24

    ARM_CM_DWT_CYCCNT  = 0;

    ARM_CM_DWT_CTRL   |= 1 << 0;   // Set bit 0

}
```

## 使用DWT周期计数器来测量代码执行时间

可以通过在目标代码之前和之后读取周期计数器的值来测量和计算代码段的执行时间，如下所示。 当然，这意味着必须设置代码，但能够得到一个非常准确的值。

```
uint32_t  start;

uint32_t  stop;

uint32_t  delta;


start = ARM_CM_DWT_CYCCNT;

// Code to measure

stop  = ARM_CM_DWT_CYCCNT;

delta = stop – start;
```

因为使用的是无符号运算，delta表示所测量代码的实际执行时间(CPU 时钟周期)。

在测量开始和停止读数之间的代码执行时间时，可能会发生中断，所以每次执行这个序列很可能会有不同的值。在这种情况下，可能希望在测量过程中禁用中断，但是要清楚禁用中断是暂时的，只用于测量。尽管如此，也许应该把中断的任务包括进来，因为它们会影响到代码的最后执行时间。

```
Disable Interrupts;

start = ARM_CM_DWT_CYCCNT;

// Code to measure

stop  = ARM_CM_DWT_CYCCNT;

Enable Interrupts;

delta = stop – start;
```

如果所测代码包含条件语句、循环或任何可能导致变化的东西，那么获得的值可能不代表最坏情况下的执行时间。为了纠正这个问题，需要添加一个峰值检测器，如下图所示。当然，在进行任何测量之前，需要将 max 声明并初始化为最小值(即0)。

```
start = ARM_CM_DWT_CYCCNT;

// Code to measure

stop  = ARM_CM_DWT_CYCCNT;

delta = stop – start;

if (max < delta) {

    max = delta;

}
```

同样，知道最短执行时间也是有趣且有用的 在进行任何测量之前，只需要声明和初始化最大可能值(即0xFFFFFFFF)。下面是新的代码: ``` tart = ARM*CM*DWT_CYCCNT;

// Code to measure

stop = ARM*CM*DWT_CYCCNT;

delta = stop – start;

if (max < delta) {
`max = delta;`
}

if (min > delta) {
`min = delta;`
} ``` 就像 Cortex-M4处理器和 Cortex-M7那样，执行时间还取决于CPU是否配备了缓存。如果系统中使用了指令或数据缓存，对同一段代码的多重测量可能不一致。这时，可以考虑禁用缓存以测量最坏的情况。

大多数调试器允许显示这些变量值。如果是这样，则需要在全局范围内声明显示变量，以保留它们的值并允许实时监控。不幸的是，这些值代表的是CPU时钟周期，而且大多数调试器还不够成熟，无法为了显示目的而对变量进行缩放。假设一个16兆赫的CPU时钟速度，显示70.19微秒比显示1123个周期要方便得多。实际上还有一种更好的方法来显示这些变量，这也提供了规模化能力，可以以一种更加可读的形式看待它们。

## 经过的时间模块

当然，可以将代码片段嵌入到应用程序中，但还可以可以使用一个简单的模块。 elapsed*time.c与elapsed*time.h，它仅由4个函数组成。

方法如下：
- 
按照惯例，#include 

- 
在使用elapsed*time.c 中的其他函数之前，调用 elapsed*time_init()

- 
通过设置"ELAPSED*TIME*MAX_SECTIONS"来定义时间测量结构的最大数目。这与用 stop/start代码包装的不同代码段相对应

- 
调用elapsed*time*start()并传递要监视的代码片段的索引(即0 到ELAPSED*TIME*MAX_SECTIONS-1)

- 
调用elapsed*time*stop()并传递在运行时启动时所使用的相同索引

- 
如果调试器允许监视变量(即当目标正在运行时) ，则可以显示elapsed*time*tbl[]，并查看对应索引的运行时间结构

- 
重复执行步骤4到6，并将代码置于最坏和最好的情况下，以便ELAPSED_TIME数据结构中的Min 和max 字段可以很好地表示所测量代码片段的执行时间


需要注意的是， 没有在测量过程中禁用中断，因为ISR可能会涉及到，也需要了解这会如何影响感知的执行时间。

```
void  main (void)
{

    // Some code

    elapsed_time_init();         // Iitialize the module

    // Some code

}

void  MyCode (void)
{

    // Some code here

    elapsed_time_start(0);    // Start measurement of code snippet #0

    // Code being measured

    elapsed_time_stop(0);     // Stop and

    // Some other code

}
```

当然，最小和最大的执行时间取决于测量的频率，以及代码是否分别受到最佳和最差条件的限制。

另外，没有必要显示起始字段，因为它只用于在测量开始时记录DWT周期计数器的值，然而，启动字段可以用来显示出来。换句话说，当看到这个值变化时，就会知道测量正在发生。

## 使用 uc / probe 的示例显示

使用了elapsed_time.c 和 uc/probe，来测量一下代码片段的执行时间。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqaAehUA2U0qUGxh3qOjcbb5vGLmIyFW2icrclkfzIr8ACcR2iadkmsvmQUSVZsGaUOAiceRFw67bs2Q/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqaAehUA2U0qUGxh3qOjcbbuKCEue4RMBw1Lw6NNU5EPFhBSoWmjeFjR10riaGFpzIJbftYOibloTSg/640?wx_fmt=png)

图1| IAR 和 uc/probe 的树视图

图1显示了使用IAR的LiveWatch (左)和 uc / probe 的树视图(右）。截图是在不同的时间拍摄的，是一个存储不同代码片段的测量值的数组。

可以将min/max/current分配给计量表和数字指示器，如图2所示。CPU 运行在80mhz，这些值以微秒显示，应用了0.0125的缩放因子。左侧的按钮用于重置统计数据，从而迫使重新计算最小值和最大值。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqaAehUA2U0qUGxh3qOjcbb83rZT8EEWMtmADKDUN45xDbGyCmZ7ANSznl5UcJ0ppC1JMtkOAdf0g/640?wx_fmt=png)

图2 | 使用uc/probe 的仪表显示最大执行时间

Uc/probe 的一个强大特性是能够与微软的 Excel 对接，从而在电子表格中显示这些值(实时) ，如图3所示。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqaAehUA2U0qUGxh3qOjcbbpe7zbBr3PA4ePCrRibZZOG47cNhUqsMyzf0oqL8zIXRbnWErmIltwmQ/640?wx_fmt=png)

图3 | 使用 Excel 显示实时数据

## 小结

作为嵌入式开发人员，有许多工具可以用来测试和验证设计。对于代码执行时间，可以很容易地使用 Cortex-M 处理器众多特性中的一个，即DWT周期计数器。

uc/probe 提供了很多功能，允许使用计量表、仪表盘、数字指示器、 Excel界面或图表来监控应用程序中的许多变量。通过内置的示波器功能，一旦触发条件满足，还可以捕获多达7个额外变量值。

## 附录代码

elapsed_time.c

```
#include  <stdint.h>
#include  <elapsed_time.h>

/*
********************************************************************************
*                           CORTEX-M - DWT TIMER
********************************************************************************
*/

#define  ARM_CM_DEMCR      (*(uint32_t *)0xE000EDFC)
#define  ARM_CM_DWT_CTRL   (*(uint32_t *)0xE0001000)
#define  ARM_CM_DWT_CYCCNT (*(uint32_t *)0xE0001004)

/*
********************************************************************************
*                             Data Structure
********************************************************************************
*/

typedef  struct  elapsed_time {
    uint32_t  start;
    uint32_t  current;
    uint32_t  max;
    uint32_t  min;
} ELAPSED_TIME;

/*
********************************************************************************
*                      STORAGE FOR ELAPSED TIME MEASUREMENTS
********************************************************************************
*/

static  ELAPSED_TIME  elapsed_time_tbl[ELAPSED_TIME_MAX_SECTIONS];

/*
********************************************************************************
*                              MODULE INITIALIZATION
*
* Note(s): Must be called before any of the other functions in this module
********************************************************************************
*/

void  elapsed_time_init (void)         
{
    uint32_t  i;
    
    
    if (ARM_CM_DWT_CTRL != 0) {                  // See if DWT is available
        ARM_CM_DEMCR      |= 1 << 24;            // Set bit 24
        ARM_CM_DWT_CYCCNT  = 0;                
        ARM_CM_DWT_CTRL   |= 1 << 0;             // Set bit 0
    }
    for (i = 0; i < ELAPSED_TIME_MAX_SECTIONS; i++) {
        elapsed_time_clr(i);
    }
}

/*
********************************************************************************
*                  START THE MEASUREMENT OF A CODE SECTION
********************************************************************************
*/

void  elapsed_time_start (uint32_t  i)  
{
    elapsed_time_tbl[i].start = ARM_CM_DWT_CYCCNT;
}

/*
********************************************************************************
*           STOP THE MEASUREMENT OF A CODE SECTION AND COMPUTE STATS
********************************************************************************
*/

void  elapsed_time_stop (uint32_t  i)  
{
    uint32_t       stop; 
    ELAPSED_TIME  *p_tbl;
    

    stop           = ARM_CM_DWT_CYCCNT;   
    p_tbl          = &elapsed_time_tbl[i];
    p_tbl->current = stop - p_tbl->start;
    if (p_tbl->max < p_tbl->current) {
        p_tbl->max = p_tbl->current;
    }
    if (p_tbl->min > p_tbl->current) {
        p_tbl->min = p_tbl->current;
    }
}

/*
********************************************************************************
*                      CLEAR THE MEASUREMENTS STATS
********************************************************************************
*/

void  elapsed_time_clr (uint32_t  i)         
{
    ELAPSED_TIME  *p_tbl;
    
    
    p_tbl          = &elapsed_time_tbl[i];
    p_tbl->start   = 0;
    p_tbl->current = 0;
    p_tbl->min     = 0xFFFFFFFF;
    p_tbl->max     = 0;
}
```

elapsed_time.h

```
/*
********************************************************************************
*                       MODULE TO MEASURE EXECUTION TIME
********************************************************************************
*/

/*
********************************************************************************
*                MAXIMUM NUMBER OF ELAPSED TIME MEASUREMENT SECTIONS
********************************************************************************
*/

#define  ELAPSED_TIME_MAX_SECTIONS  10

/*
********************************************************************************
*                             FUNCTION PROTOTYPES
********************************************************************************
*/

void  elapsed_time_clr   (uint32_t  i);      // Clear measured values
void  elapsed_time_init  (void);             // Module initialization
void  elapsed_time_start (uint32_t  i);      // Start measurement 
void  elapsed_time_stop  (uint32_t  i);      // Stop  measurement
```

参考文献

https://www.micrium.com/ucprobe/about/

https://www.iar.com/iar-embedded-workbench/

https://www.arm.com/products/processors/cortex-m

（本文编译自 http://www.embedded-computing.com/hardware/measuring-code-execution-time-on-arm-cortex-m-mcus）





