# 使用STM32生成随机数的方法----试了可以，但不适用于我的场合！！！ - xqhrs232的专栏 - CSDN博客
2018年10月04日 23:17:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：658
原文地址::[https://blog.csdn.net/qq_26712377/article/details/81138489](https://blog.csdn.net/qq_26712377/article/details/81138489)
相关文章
1、STM32 产生随机数----[https://blog.csdn.net/scliu12345/article/details/39855367](https://blog.csdn.net/scliu12345/article/details/39855367)
2、stm32产生随机数----[https://blog.csdn.net/hes_c/article/details/79094040](https://blog.csdn.net/hes_c/article/details/79094040)
在stm32中产生随机数可以利用C的标准库结合定时器时间来实现.    主要用到的两个函数是srand和rand().
 srand和rand()配合使用产生伪随机数序列。rand函数在产生随机数前，需要系统提供的生成伪随机数序列的种子，rand根据这个种子的值产生一系列随机数。 如果系统提供的种子没有变化，每次调用rand函数生成的伪随机数序列都是一样的。
srand(unsigned seed)通过参数seed改变系统提供的种子值，从而可以使得每次调用rand函数生成的伪随机数序列不同，从而实现真正意义上的“随机”。
 通常可以利用系统时间来改变系统的种子值，即srand(time(NULL))，可以为rand函数提供不同的种子值，进而产生不同的随机数序列。
srand()函数：
    头文件：#include<stdlib.h>
    原型：void  srand(unsigned int seed);
rand()函数：
    头文件：#include<time.h>
    在你的文件里面包含两个头文件，直接调用以上的函数就行，至于需要修改的seed，目前采用的方法主要两种，一种是采用定时器时间seed，一种是采用读取某个悬空引脚adc的数值作为seed.
我使用了一个微秒定时器做seed.
代码
u32 time_us = 0;
//timer4  初始化
void time_init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = arr;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = 0; 
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;               
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;                         
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                           
    NVIC_Init(&NVIC_InitStructure);    
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);        
    TIM_Cmd(TIM4, ENABLE);                                           
}
void TIM4_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
    {
        GPIO_SetBits(GPIOD,GPIO_Pin_6);
        time_us++;
        TIM_ClearITPendingBit(TIM4,TIM_IT_Update);        
    }    
}
int main()
{        
    int p = 0;
    time_init(71,0);
    uart5_init(9600);
    while(1)
    {
       srand(time_us);
        p= rand()%20 +30;      //产生一个30到50的随机数
         printf("%d\r ",p);
        delay_ms(1000);
    }    
}
--------------------- 本文来自 小眼神儿 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/qq_26712377/article/details/81138489?utm_source=copy
