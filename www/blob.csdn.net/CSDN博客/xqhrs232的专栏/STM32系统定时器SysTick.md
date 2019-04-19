# STM32系统定时器SysTick - xqhrs232的专栏 - CSDN博客
2019年03月19日 11:32:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：48
原文地址::[https://blog.csdn.net/qq_29344757/article/details/75090137](https://blog.csdn.net/qq_29344757/article/details/75090137)
相关文章
1、STM32的SYSTICK 定时器 玩了这么久单片机，这次终于搞懂！有栗子啊----[https://blog.csdn.net/qq_38405680/article/details/82078238](https://blog.csdn.net/qq_38405680/article/details/82078238)
2、时钟，定时器 玩了这么久单片机，这次终于搞懂！ STM32的SYSTICK----[https://blog.csdn.net/qq_15079039/article/details/77247888](https://blog.csdn.net/qq_15079039/article/details/77247888)
1. SysTick系统定时器概述
学习完STM32的中断，下来就要学习STM32的定时器。就像电话最基本的功能是与人通话一样，定时器最基本的功能就是定时(STM32有些定时器的功能强大得超乎想象，当然不是今天要学的SysTick)，定时器的使用步骤无非就是设置定时时间，然后等待超时，超时会触发中断或者设置某个标志位：若是触发中断自然要去执行中断处理函数，处理函数和中断源的绑定工作在启动文件的中断向量表已经明确，我们负责实现与之对应的中断处理函数即可；若是查询标志位则轮询访问该标志位，发生改变后再执行其他处理操作。
定时器涉及到中断，那就跟前面学习的EXTI、NVIC以及中断优先级等概念扯上关系了。系统定时器SysTick是属于CM3内核中的一个外设，相关寄存器内嵌在NVIC中，所有基于CM3内核的单片机都具有这个系统定时器，这使得软件(OS)在CM3单片机可以十分容易的移植。SysTick一般用于操作系统的产生时基功能，以维持操作系统的“心跳”。
2. SysTick系统定时器相关寄存器
下图摘自《Cortex™-M3技术参考手册.pdf》-P153 
标准库中core_cm3.h中对SysTick描述结构体放入封装与之对应：
typedef struct
{
  __IO uint32_t DHCSR;                        /*!< Offset: 0x00  Debug Halting Control and Status Register    */
  __O  uint32_t DCRSR;                        /*!< Offset: 0x04  Debug Core Register Selector Register        */
  __IO uint32_t DCRDR;                        /*!< Offset: 0x08  Debug Core Register Data Register            */
  __IO uint32_t DEMCR;                        /*!< Offset: 0x0C  Debug Exception and Monitor Control Register */
} CoreDebug_Type;
1
2
3
4
5
6
7
2.1 SysTick控制及状态寄存器(SysTick Control and Status Register)
COUNTFLAG: 若上次读取本寄存器后，SysTick已经计到0，则该位为1 
CLKSOURCE: 时钟源选择位，0表AHB / 8，1表处理器时钟AHB 
TICKINT: 1表SysTick倒数计数计到0时产生SysTick异常请求，0表计到0时无动作。也可以通过读取COUNTFLAG来确定计数器是否递减到0 
ENABLE: SysTick定时器使能位 
英文描述如下： 
2.2 SysTick重装载数值寄存器(SysTick Reload Value Register)
RELOAD: 当倒数计到0时，将被重装载的值 
英文描述如下： 
2.3 SysTick当前数值寄存器(SysTick Current Value Register)
CURRENT: 读取时返回当前计数的值，写它则使其清零，同时清除SysTick控制及状态寄存器中的COUNTFLAG状态标志位 
英文描述如下： 
2.4 SysTick校准数值寄存器(SysTick Calibration Value Register)
这个校准不常用，先不理会。 
英文描述如下： 
系统定时器SysTick有一个24Bit的向下的计数器，该计数器的值会被拷贝到重装载数值寄存器中，一般的配置是，当重装载数值寄存器的值递减到0的时候系统定时器就会产生一次中断，以此循环往复。
这里引入一个问题，计数器多久会递减1？ 
图为《STM32中文参考手册_V10.pdf》中时钟树。Cortex系统时钟即是指SysTick的时钟，很明显被设置为HCLK / 8，也就是9MHZ。再看外设库中对SysTick时钟源的设置是否为HCLK / 8？ 
在Libraries\STM32F10x_StdPeriph_Driver\src\misc.c，竟然有专门用于设置SysTick的时钟源的函数，
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource)
{
    /* Check the parameters */
    assert_param(IS_SYSTICK_CLK_SOURCE(SysTick_CLKSource));
    if (SysTick_CLKSource == SysTick_CLKSource_HCLK)
    {
        SysTick->CTRL |= SysTick_CLKSource_HCLK;
    }
    else
    {
        SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;
    }
}
1
2
3
4
5
6
7
8
9
10
11
12
13
参数取值为SysTick_CLKSource_HCLK表示时钟源设置为HCLK，取值SysTick_CLKSource_HCLK_Div8表将时钟源设置为HCLK/8。 
在前面的CLKSOURCE寄存器位也明确指明SysTick的时钟源为HCLK和HCLK / 8可选，然而在时钟树上却并没有体现时钟源可选这个关键信息，可见这是数据手册的漏洞，功能实现以寄存器的说明为主而非时钟树：SysTick的时钟源并非限定在HCLK / 8
定义在Libraries\CMSIS\CM3\CoreSupport\core_cm3.h中的SysTick_Config()的函数中，实现相以上关寄存器的配置，这是至关重要的一个函数，
static __INLINE uint32_t SysTick_Config(uint32_t ticks)
{ 
  /* SysTick计数器的最大值为24Bit，超出范围则返回 */
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */
  /* 设置重装载寄存器 */
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
  /* 设置中断源(处理函数)的优先级 */
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
  /* 设置当前数值寄存器的值为0 */
  /* 设置SysTick的时钟源为HCLK=72MHz */
  /* 使能SysTick超时中断 */
  /* 使能SysTick */
  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
  return (0);                                                  /* Function successful */
}
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
注意这里将SysTick的时钟源设置为72MHZ，那么计数器减1间隔的时间为: (1 / 72MHz)s，其它相关配置项已经代码的注释中写出，读者可以参照源码阅读。
3. SysTick系统定时器的重装载寄存器数值
使用标准外设库编程的时候我们只需要调用SysTick_Config(uint32_t ticks)函数即可 ，形参ticks用来设置重装载寄存器的值，最大不可超过2的24次方，当重装载寄存器的值递减到0的时候产生中断，然后重装载寄存器的值又重新装载往下递减计数，以此循环。 
设置ticks的值等价于设置计时时间，中间有一个简单的换算过程：SysTick每递减1需要(1/72MHZ)s，那么定时1s则将ticks取值为72000000，定时1ms则ticks取值为72000，定时10us则ticks取值为720，定时1us则ticks取值为72，总归：
SysTick时钟源 / 1000   =>  定时1ms
SysTick时钟源 / 100000 =>  定时10us
SysTick时钟源 / 1000000    =>  定时1us
1
2
3
4. SysTick系统定时器中断优先级
在SysTick_Config()中还调用NVIC_SetPriority()函数用于设置SysTick的中断优先级，该函数也在core_m3.h中定义，原型如下：
/* 设置中断源的中断优先级 */
static __INLINE void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
  if(IRQn < 0) {
    SCB->SHP[((uint32_t)(IRQn) & 0xF)-4] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff); } /* set Priority for Cortex-M3 System Interrupts */
  else {
    NVIC->IP[(uint32_t)(IRQn)] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff);    }        /* set Priority for device specific Interrupts  */
}
1
2
3
4
5
6
7
8
参数一的类型IRQn_Type结构体，其原型描述了STM32所支持的系统异常和外部中断：
typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                             */
  MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M3 Memory Management Interrupt              */
  BusFault_IRQn               = -11,    /*!< 5 Cortex-M3 Bus Fault Interrupt                      */
  UsageFault_IRQn             = -10,    /*!< 6 Cortex-M3 Usage Fault Interrupt                    */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                       */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                 */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                       */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                   */
/******  STM32 specific Interrupt Numbers *********************************************************/
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                            */
  PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt            */
  TAMPER_IRQn                 = 2,      /*!< Tamper Interrupt                                     */
  RTC_IRQn                    = 3,      /*!< RTC global Interrupt                                 */
  FLASH_IRQn     
  ...
}
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
可见系统异常的取值小于0，外部中断的取值则大于0。 
参数二则表示优先级。 
函数首先首先判断IRQn的大小，若小于0则表示系统异常，系统异常的优先级由内核外设SCB的寄存器SHPRx控制，若大于0则是外部中断，外部中断的优先级由内核外设NVIC中的IPx寄存器控制。有关SHPRx需要参考《Cortex-M3内核编程手册》相关章节。 
Systick属于内核外设，跟普通外设的中断优先级存在差别，它并没有抢占优先级和子优先级的说法。SysTick_Config()函数默认其中断优先级设置为15，它在内核外设中的优先级是最低的，若要修改优先级则通过NVIC_SetPriority()函数的参数二，范围为0~15。 
这里引入了内核外设和片上外设优先级的问题：若系统的中断优先分组为2，内核外设SysTick优先级为函数库默认配置的15，片上外设某个中断源的抢占优先级为1，子优先级也为1，那它们之间的中断优先级该如何比较，若同时发生中断请求，谁的中断处理函数先得到响应？ 
NVIC的中断优先级分组不仅对片上外设有效，同样对内核外设也有效。比较方法是：将SysTick的优先级15转为二进制数值为0b1111，且NVIC的优先级分组为2，那么前两位为0b11，后两位也是0b11，这两个数值的意义等同于片上外设中断源的抢占优先级和子优先级，因此可见，SysTick的抢占优先级和子优先级都低于片上片上外设某个中断源优先级和子优先级。如果当两个的软件优先级都配置成一致，那么就比较它们在中断向量表中的硬件编号，编号越小优先级越高。
5. SysTick系统定时器编程实践
阐述了以上知识点，下来就是编程练习了：利用SysTick系统定时器，实现精准延时，这个在实际工程项目中十分常用。 
硬件平台正点原子MiniSTM32，实现的功能是精准延时让板载的两个LED灯闪烁。编程的要点是调用SysTick_Config()函数设置计数器的重装载值。 
新建文件main.c和systick_test.h分别用于实现主体功能和函数声明：
systick_test.h
#ifndef __SYSTICK_TEST_H__
#define __SYSTICK_TEST_H__
#include "stm32f10x_conf.h"
void Led_CfgInit(void);
void SysTick_Delay_Us(__IO uint32_t us);
void SysTick_Delay_Ms(__IO uint32_t ms);
#endif /* __SYSTICK_TEST_H__ */
main.c
#define ALL_LED_ON  GPIO_ResetBits(GPIOA,GPIO_Pin_8);\
                    GPIO_ResetBits(GPIOD,GPIO_Pin_2)
#define ALL_LED_OFF GPIO_SetBits(GPIOA,GPIO_Pin_8);\
                    GPIO_SetBits(GPIOD,GPIO_Pin_2)
//PA8-->LED0,PD2-->LED1
void Led_CfgInit(void)
{
    GPIO_InitTypeDef GPIO_InitTypeStu;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitTypeStu.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitTypeStu.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitTypeStu.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitTypeStu);
    GPIO_SetBits(GPIOA,GPIO_Pin_8);         
    GPIO_InitTypeStu.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitTypeStu);
    GPIO_SetBits(GPIOD,GPIO_Pin_2);         
}
//配置SysTick的计数器计数的时间是1us，每间隔1us，CTRL的BIT[16]由0刷新为1
void SysTick_Delay_Us(__IO uint32_t us)
{
    uint32_t i;
    SysTick_Config(72);
    for (i = 0; i < us; i++)
    {
        while (!((SysTick->CTRL >> 16) & 0x01));
    }
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
//配置SysTick的计数器计数的时间是1ms，每间隔1us，CTRL的BIT[16]由0刷新为1
void SysTick_Delay_Ms(__IO uint32_t ms)
{
    uint32_t i;
    SysTick_Config(72000);
    for (i = 0; i < ms; i++)
    {
        //while (!(SysTick->CTRL & (1 << 16)));
        while (!((SysTick->CTRL >> 16) & 0x01));
    }
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
int main(void)
{
    SysTick_Delay_Us(10);
    Led_CfgInit();
    while (1)
    {
        ALL_LED_ON;
        SysTick_Delay_Ms(1000);
        ALL_LED_OFF;
        SysTick_Delay_Ms(5000);
    }
}
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
79
80
81
82
SysTick_Delay_Ms()和SysTick_Delay_Us()分别实现n个1Ms定时和n个1Us定时的功能。 
以SysTick_Delay_Ms()为例，其中的SysTick_Config(72000)定义了每间隔1ms触发一次SysTick中断，当然，同时CTRL的BIT[16]会由0变为1，随后计数器又会从重装载器中取出72000开始递减，此时TRL的BIT[16]会由1变为0，如此循环。基于这个特点，这里采用轮询CTRL的BIT[16]的方法以达到延时的目的，这里中断处理函数未使用。
下来采用中断处理函数的方法：
#ifndef __SYSTICK_TEST_H__
#define __SYSTICK_TEST_H__
#include "stm32f10x_conf.h"
void Led_CfgInit(void);
void SysTick_CfgInit(void);
void SysTick_Delay(__IO uint32_t ms);
#endif /* __SYSTICK_TEST_H__ */
main.c
extern uint32_t SystemCoreClock;
uint32_t IT_Cnt;
#define ALL_LED_ON  GPIO_ResetBits(GPIOA,GPIO_Pin_8);\
                    GPIO_ResetBits(GPIOD,GPIO_Pin_2)
#define ALL_LED_OFF GPIO_SetBits(GPIOA,GPIO_Pin_8);\
                    GPIO_SetBits(GPIOD,GPIO_Pin_2)
//PA8-->LED0,PD2-->LED1
void Led_CfgInit(void)
{
    GPIO_InitTypeDef GPIO_InitTypeStu;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitTypeStu.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitTypeStu.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitTypeStu.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitTypeStu);
    GPIO_SetBits(GPIOA,GPIO_Pin_8);         
    GPIO_InitTypeStu.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitTypeStu);
    GPIO_SetBits(GPIOD,GPIO_Pin_2);         
}
void StickTick_CfgInit()
{
    SysTick_Config(SystemCoreClock / 1000);
}
void SysTick_Delay(__IO uint32_t ms)
{
    IT_Cnt = ms;
    while (IT_Cnt != 0);
}
int main(void)
{
    StickTick_CfgInit();    //1ms产生一次SysTick中断  
    while (1)
    {
        ALL_LED_ON;
        SysTick_Delay(1000);
        ALL_LED_OFF;
        SysTick_Delay(1000);
    }
    return 0;
}
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
代码实现的功能是：SysTick系统定时器每1ms产生一次SysTick中断，在中断处理函数中，
void SysTick_Handler(void)
{
    IT_Cnt--;
}
1
2
3
4
只是简单的对毫秒总数递减，SysTick_Delay()阻塞在IT_Cnt != 0，超时后IT_Cnt为0，延时函数退出。逻辑都比较简单，不赘述。 
这里要提的是，SysTick系统定时器超时时会产生中断，也就是说它是属于STM32中断体系中的，而且相关寄存器内嵌在NVIC中，那为什么不对NVIC_InitTypeDef描述结构体进行初始化呢？这个问题的答案可以在前面的SysTick系统定时器中断优先级小节得出答案，我们从另一个特点看，NVIC_InitTypeDef的结构体原型如下：
typedef struct
{
  uint8_t NVIC_IRQChannel;                    
  uint8_t NVIC_IRQChannelPreemptionPriority;  
  uint8_t NVIC_IRQChannelSubPriority;         
  FunctionalState NVIC_IRQChannelCmd;          
} NVIC_InitTypeDef;
1
2
3
4
5
6
7
以成员NVIC_IRQChannel为例，它是指定中断/事件的通道的，为uint8_t类型，但是SysTick_IRQn等于-1，显然与之类型不匹配，加上前面我们讲到，Systick属于内核外设，跟普通外设的中断优先级存在差别，普通外设的中断/事件才需要设置NVIC，所以咧，它并不需要初始化NVIC_InitTypeDef，SysTick相关的初始化操作已经在SysTick_Config()函数中实现了。
系统定时器SysTick其实也不难，STM32支持跑OS，所以SysTick存在最大的意义就是为OS提供时基单元，多个时基单元可以组成一个时间片，时间片就是OS任务线程执行的轮回。学习的重点在于计数值的设置、以及理解与外部中断的差别。先总结到这，下一篇讲通用定时器。
