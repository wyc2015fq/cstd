# (转)Cortex-M3 (NXP LPC1788)之ADC数模转换器的应用 - weixin_33985507的博客 - CSDN博客
2012年09月26日 19:18:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
A/D转换器的功能是将模拟输入信号采样得到可以提供计算机进行处理的数字信号。LPC1788的ADC是一个12位的逐次逼近型模数转换器，有8个复用的输入管脚，它的时钟使用PCLK分频得到。开发板的A/D输入模块电路图如下
![](https://img-my.csdn.net/uploads/201207/05/1341469486_6341.jpg)
          要使用ADC转换，也要配置PCONP寄存器，配置ADC的时钟以及管脚和中断等。A/D转换控制寄存器AD0CR用来设置ADC的通道，时钟分频，工作模式和启动A/D转换的方式。开发板使用的是AIN2即A/D转换通道2，管脚为P0[25]。根据之前的文章，配置了系统的PCLK位60M，所以设置时钟分频5分频，将ADC的时钟设置成12M，而ADC完成一次转换需要31个时钟，所以ADC的转换频率约为400KHZ。AD0CR的START和EDGE用来控制ADC转换的触发方式，我们可以配置成通过定时器和外部中断来触发AD转换以及触发的边沿的方式。
        ADC转换完成后，转换的结果保存在A/D数据寄存器，有两个数据寄存器，他们分别是全局数据寄存器AD0GDR和每个通道的数据寄存器AD0DR0~AD0DR7，他们包含了数据转换结果和一些转换的状态标志。
        ADC中断使能寄存器AD0INTEN，控制每个通道转换完成后是否产生中断，以及全局数据寄存器AD0GDR中的DONE标志是否产生中断。
        下面的程序中，我将ADC配置成P2[10]外部中断触发AD转换，AD转换完成后会触发AD中断，在中断中进行数据的处理。
**[cpp]**[view plain](#)[copy](#)[print](#)[?](#)
- #include "LPC1788_REG.h"
- #include "uart.h"
- 
- #define rIOCON_P0_25    (*(volatile unsigned*)(0x4002C064))
- #define rIOCON_P2_10    (*(volatile unsigned *)0x4002C128)
- 
- #define rEXTINT         (*(volatile unsigned *)0x400FC140)
- #define rEXTMODE        (*(volatile unsigned *)0x400FC148)
- #define rEXTPOLAR       (*(volatile unsigned *)0x400FC14C)
- 
- #define rAD0CR      (*(volatile unsigned*)(0x40034000))
- #define rAD0INTEN   (*(volatile unsigned*)(0x4003400C))
- #define rADGDR      (*(volatile unsigned*)(0x40034004))
- #define rAD0DR2     (*(volatile unsigned*)(0x40034018))
- #define rAD0STAT    (*(volatile unsigned*)(0x40034030))
- #define rAD0TRM     (*(volatile unsigned*)(0x40034034))
- 
- unsigned int adc_value;  
- 
- void EINT0_IRQHandler()  
- {  
-     rEXTINT |= (0x1<<0);//中断产生之后必须清除改位
- 
-     Uart2SendS("SW6 ON!\r\n");  
- }  
- 
- void ADC_IRQHandler()  
- {  
- if(rAD0DR2&(1UL<<31))  
-     {  
-         adc_value = ((rAD0DR2)>>4) & 0xFFF;  
-         Uart2SendD(adc_value);  
-         Uart2SendS("\r\n");  
-     }  
- else
-     {  
-         Uart2SendC('x');  
-     }  
- }  
- 
- void EINT_INIT()  
- {  
-     rEXTMODE |= 0x1<<0;     //配置成边沿触发
-     rEXTPOLAR &= ~(0x1<<0); //下降沿触发
- 
-     rISER0 = (0x1<<18);//打开EINT0中断使能
- }  
- 
- int main()  
- {  
- char menu[]={"Press SW6 to get a ADC_Value.\r\n"};  
-     Init_Uart2();  
-     EINT_INIT();  
- 
-     rPCONP |= 0x1<<12;  //使能PCADC功率控制 
- 
-     rIOCON_P2_10 = (rIOCON_P2_10&(~0x7))|(0x1<<0);  //配置FUNC字段为EINT0模式
- 
-     rIOCON_P0_25 &= ~(0x1<<7);  //P0[25]做模拟管脚
-     rIOCON_P0_25 &= ~(0x3<<3);  //无效上下拉电阻
-     rIOCON_P0_25 &= ~0x7;       //P0[25]作为ADC[2]功能
-     rIOCON_P0_25 |= 0x1;  
- 
- 
-     rAD0CR |= 0x1<<2;       //使用通道2
-     rAD0CR |= 0x4<<8;       //A/D转换时钟设置成12M
-     rAD0CR |= 0x1<<21;      //A/D工作在正常模式
-     rAD0INTEN |= 0x1<<2;    //使能A/D通道2转换完中断
- 
-     rAD0INTEN &= ~(0x1<<8);    //无视全局数据寄存器的DONE标志
- 
-     rAD0CR |= 0x1<<27;        
-     rAD0CR &= ~(0x7<<24);  
-     rAD0CR |= 0x2<<24;      //P2[10]下降沿出现时启动转换 
-     rISER0 |= 0x1<<22;      //使能A/D中断   
- 
-     Uart2SendS(menu);  
- while(1);  
- }  
```cpp
#include "LPC1788_REG.h"
#include "uart.h"
#define rIOCON_P0_25    (*(volatile unsigned*)(0x4002C064))
#define	rIOCON_P2_10	(*(volatile unsigned *)0x4002C128)
#define rEXTINT			(*(volatile unsigned *)0x400FC140)
#define rEXTMODE		(*(volatile unsigned *)0x400FC148)
#define rEXTPOLAR		(*(volatile unsigned *)0x400FC14C)
#define rAD0CR      (*(volatile unsigned*)(0x40034000))
#define rAD0INTEN   (*(volatile unsigned*)(0x4003400C))
#define rADGDR      (*(volatile unsigned*)(0x40034004))
#define rAD0DR2     (*(volatile unsigned*)(0x40034018))
#define rAD0STAT    (*(volatile unsigned*)(0x40034030))
#define rAD0TRM     (*(volatile unsigned*)(0x40034034))
unsigned int adc_value;
void EINT0_IRQHandler()
{
	rEXTINT |= (0x1<<0);//中断产生之后必须清除改位
	Uart2SendS("SW6 ON!\r\n");
}
void ADC_IRQHandler()
{
    if(rAD0DR2&(1UL<<31))
    {
        adc_value = ((rAD0DR2)>>4) & 0xFFF;
        Uart2SendD(adc_value);
        Uart2SendS("\r\n");
    }
    else
    {
        Uart2SendC('x');
    }
}
void EINT_INIT()
{
	rEXTMODE |= 0x1<<0;     //配置成边沿触发
	rEXTPOLAR &= ~(0x1<<0); //下降沿触发
	
	rISER0 = (0x1<<18);//打开EINT0中断使能
}
int main()
{
    char menu[]={"Press SW6 to get a ADC_Value.\r\n"};
    Init_Uart2();
    EINT_INIT();
    rPCONP |= 0x1<<12;  //使能PCADC功率控制 
    
    rIOCON_P2_10 = (rIOCON_P2_10&(~0x7))|(0x1<<0);  //配置FUNC字段为EINT0模式
    
    rIOCON_P0_25 &= ~(0x1<<7);  //P0[25]做模拟管脚
    rIOCON_P0_25 &= ~(0x3<<3);  //无效上下拉电阻
    rIOCON_P0_25 &= ~0x7;       //P0[25]作为ADC[2]功能
    rIOCON_P0_25 |= 0x1;
    
    
    rAD0CR |= 0x1<<2;       //使用通道2
    rAD0CR |= 0x4<<8;       //A/D转换时钟设置成12M
    rAD0CR |= 0x1<<21;      //A/D工作在正常模式
    rAD0INTEN |= 0x1<<2;    //使能A/D通道2转换完中断
    rAD0INTEN &= ~(0x1<<8);    //无视全局数据寄存器的DONE标志
    rAD0CR |= 0x1<<27;      
    rAD0CR &= ~(0x7<<24);
    rAD0CR |= 0x2<<24;      //P2[10]下降沿出现时启动转换 
    rISER0 |= 0x1<<22;      //使能A/D中断   
    
    Uart2SendS(menu);
    while(1);
}
```
        程序运行的效果是，按下控制P2[10]的按键，每按一次得到一个AD采样的结果，改变R94的输入，可以得到不同的结构，最大最小分别是0和4095。
![](https://img-my.csdn.net/uploads/201207/05/1341469637_8240.jpg)
        程序中需要注意的有如下几点：
        1，P2[10]必须配置成EINT0模式，才能用于在边沿上触发ADC转换。
        2，必须关闭ADC输入管脚的上拉或下拉电阻，否则AD采样会有偏差。
        3，ADC转换完成的中断使能，最好把AD0INTEN的最高位无效，否则需要在ADC转换完成后同时清除通道数据寄存器AD0DRn和全局数据寄存器AD0GDR中的DONE标志(该标志通过读取数据寄存器清除)。可以尝试把程序中的rAD0INTEN &= ~(0x1<<8)屏蔽，可以看到串口一直在输出'x'，即AD数据没转换完成，也有中断一直产生。原因就是AD0GDR中的DONE标志引起。
