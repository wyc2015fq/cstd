# 不用晶振，STM32内部HSI时钟的倍频使用 - xqhrs232的专栏 - CSDN博客
2019年01月09日 11:03:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：109
原文地址::[https://blog.csdn.net/qhw5279/article/details/77016341](https://blog.csdn.net/qhw5279/article/details/77016341)
相关文章
1、关于如何使用STM32内部晶振，并配置成48M主频----[http://bbs.elecfans.com/jishu_705626_1_5.html](http://bbs.elecfans.com/jishu_705626_1_5.html)
博主最近用STM32F1系列单片机做一个触摸屏USB鼠标，考虑不用外部晶振而使用STM32内部8MHz HSI RC作为时钟源。由于USB协议规定了48MHz 的USB时钟，故必须对HIS作PLL倍频处理后得到PLLCLK作为STSCLK时钟。时钟关系见如下STM32F1时钟树：
类比正点原子STM32F1寄存器版本使用外部晶振的时钟初始化函数，改动得到使用HSI作为时钟源，SYSCLK为48MHz的初始化代码：
void Stm32_Clock_Init(u8 PLL)
{
    unsigned char temp=0;
    MYRCC_DeInit();          //复位并配置向量表
    RCC->CR|=0x00000001;  //内部高速时钟使能HSION
    while(!(RCC->CR>>1));//等待内部时钟就绪
    RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
    PLL-=2;//抵消2个单位
    RCC->CFGR|=PLL<<18;   //设置PLL值 2~16
    RCC->CFGR|=0<<16;      //PLLSRC ON  HSI经2分频后作为PLL输入时钟
    FLASH->ACR|=0x32;      //FLASH 2个延时周期
    RCC->CR|=0x01000000;  //PLLON
    while(!(RCC->CR>>25));//等待PLL锁定
    RCC->CFGR|=0x00000002;//PLL作为系统时钟
    while(temp!=0x02)     //等待PLL作为系统时钟设置成功
    {
    temp=RCC->CFGR>>2;
    temp&=0x03;
    }
}
其中MYRCC_DeInit()代码如下：
void MYRCC_DeInit(void)
{    
     RCC->APB1RSTR = 0x00000000;//复位结束             
    RCC->APB2RSTR = 0x00000000; 
      RCC->AHBENR = 0x00000014;  //睡眠模式闪存和SRAM时钟使能.其他关闭.      
      RCC->APB2ENR = 0x00000000; //外设时钟关闭.               
      RCC->APB1ENR = 0x00000000;   
    RCC->CR |= 0x00000001;     //使能内部高速时钟HSION                                                                  
    RCC->CFGR &= 0xF8FF0000;   //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]                     
    RCC->CR &= 0xFEF6FFFF;     //复位HSEON,CSSON,PLLON
    RCC->CR &= 0xFFFBFFFF;     //复位HSEBYP             
    RCC->CFGR &= 0xFF80FFFF;   //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
    RCC->CIR = 0x00000000;     //关闭所有中断         
    //配置向量表                  
#ifdef  VECT_TAB_RAM
    MY_NVIC_SetVectorTable(0x20000000, 0x0);
#else   
    MY_NVIC_SetVectorTable(0x08000000,0x0);
#endif
}
MY_NVIC_SetVectorTable()代码如下：
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)     
{             
    SCB->VTOR = NVIC_VectTab|(Offset & (u32)0x1FFFFF80);//设置NVIC的向量表偏移寄存器
    //用于标识向量表是在CODE区还是在RAM区
}
(1)使用时在main()函数中应写成：Stm32_Clock_Init(12)，即8/2*12=48MHz.
(2)为使delay_ms()等函数时钟准确，应在system_stm32f10x.c中将
(3)#define SYSCLK_FREQ_48MHz  48000000解除屏蔽
(4)同时要注意在USB时钟初始化函数中不再对PLL时钟分频。即时钟配置寄存器(RCC_CFGR)第22位应设置为1
(5)最后，内部时钟要想正确使用，需要在启动.s文件中屏蔽sysinit
