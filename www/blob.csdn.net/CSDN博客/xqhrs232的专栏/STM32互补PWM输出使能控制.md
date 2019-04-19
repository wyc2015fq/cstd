# STM32互补PWM输出使能控制 - xqhrs232的专栏 - CSDN博客
2018年12月06日 16:44:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：68
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[https://blog.csdn.net/MengchiCMC/article/details/54023380](https://blog.csdn.net/MengchiCMC/article/details/54023380)
相关文章
1、分享如何使用stm32的定时器互补PWM通道，驱动IPM模块产生三相相差120°的正弦波，可产生旋转磁场驱动三相电机----[http://www.openedv.com/posts/list/51957.htm](http://www.openedv.com/posts/list/51957.htm)
2、STM32中TIM1-CH1和TIM1-CH1N的区别----[https://zhidao.baidu.com/question/588098076559755685.html?qbl=relate_question_0&word=%BB%A5%B2%B9%CD%A8%B5%C0](https://zhidao.baidu.com/question/588098076559755685.html?qbl=relate_question_0&word=%BB%A5%B2%B9%CD%A8%B5%C0)
3、【STM32】STM32之timer1产生PWM（互补通道）----[https://blog.csdn.net/feilusia/article/details/54430262](https://blog.csdn.net/feilusia/article/details/54430262)
STM32的高级定时器TIM1可以产生互补的PWM，并且可以通过相关寄存器的设置使能或关闭PWM的输出。在编写BLDC的驱动程序时，本人利用TIM1的channel1,2,3三个通道生成了三路互补的PWM波形，定时器驱动程序如下：
void TIM1_Init(u16 arr, u16 psc)
{
GPIO_InitTypeDef         GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
TIM_OCInitTypeDef       TIM_OCInitStructure;
TIM_BDTRInitTypeDef      TIM_BDTRInitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);  
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA,&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
GPIO_Init(GPIOB,&GPIO_InitStructure);
TIM_DeInit(TIM1);
TIM_TimeBaseInitStructure.TIM_Period = arr;     
TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  
TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure); 
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
TIM_OCInitStructure.TIM_Pulse = 0;
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;     
TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
TIM_OC1Init(TIM1, &TIM_OCInitStructure);
TIM_OC2Init(TIM1, &TIM_OCInitStructure);
TIM_OC3Init(TIM1, &TIM_OCInitStructure);
TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
TIM_BDTRInitStructure.TIM_DeadTime = 0;
TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;               
TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;
TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
TIM_Cmd(TIM1, ENABLE);
 TIM_CCPreloadControl(TIM1,ENABLE);
TIM_CtrlPWMOutputs(TIM1, ENABLE);
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
该函数配置了PWM的GPIO，定时器以及死区控制等。产生的波形如图1所示 
当我想要控制某些通道的输出（比如使能通道1的输出CH1，但是关闭其互补输出CH1N），利用下面两个函数发现TIM1依旧输出两路互补的PWM。这个函数最终操作的寄存器是捕获/比较使能寄存器TIMx->CCER。 
TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable); 
TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);
经过调试发现在定时器初始化函数中一个函数的调用：TIM_CCPreloadControl(TIM1,ENABLE); 
注释掉这句之后，或者将ENABLE改为DISABLE，才能实现关闭通道1的互补输出，如图2所示： 
此时通道1CH1输出PWM波形，其互补通道CH1N输出低电平。
分析发现，TIM_CCPreloadControl()函数最终操作的是控制寄存器 2(TIMx_CR2)的第0位CCPC。查阅STM32的数据手册，描述如下： 
CCPC：捕获/比较预装载控制位 (Capture/compare preloaded control) 
0： CCxE， CCxNE和OCxM位不是预装载的； 
1： CCxE， CCxNE和OCxM位是预装载的；设置该位后，它们只在设置了COM位后被更新。 
注：该位只对具有互补输出的通道起作用。
也就是说如果使能了预装载，只有在COM事件发生后值才能写入影子寄存器(影子寄存器才是真正起作用的寄存器)，产生作用。同时该位只对互补输出起作用，因此对于普通定时器来说，无论该位是否被设置，都有可以通过TIM_CCxCmd()函数使能或者关闭PWM的输出。
想要让定时器的PWM输出低电平（高电平），也可以将相对的 寄存器TIMx->CCR1置零（大于arr），但是此时互补通道只能为高电平（低电平）。而通过关闭PWM的输出，可以让两个互补的通道输出更多的电压组合。
