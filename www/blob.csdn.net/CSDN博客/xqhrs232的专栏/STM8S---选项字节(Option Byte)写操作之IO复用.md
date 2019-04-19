# STM8S---选项字节(Option Byte)写操作之IO复用 - xqhrs232的专栏 - CSDN博客
2017年06月29日 11:41:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2466
原文地址::[http://blog.csdn.net/FreeApe/article/details/47008033](http://blog.csdn.net/FreeApe/article/details/47008033)
相关文章
1、[[STM8]](http://bbs.21ic.com/icfilter-typeid-49-39.html)**继续针对STM8S的Option byte寻求答案----[http://bbs.21ic.com/icview-243976-3-1.html](http://bbs.21ic.com/icview-243976-3-1.html)**
# 功能实现目标
  通过对选项字节的写操作来实现TIM2的CH3通道的PWM输出IO复用，可以设置为PA3或者PD2输出。 
[通过STVP方式操作链接](http://blog.csdn.net/freeape/article/details/46773155)
# 选项字节
  选项字节包括芯片硬件特性的配置和存储器的保护信息，这些字节保存在存储器中一个专用的块内。除了ROP（读出保护）字节，每个选项字节必须被保存两次，一个是通常的格式（OPTx）和一个用来备份互补格式的（NOPTx）。选项字节可以通过应用程序在IAP模式下修改，但是ROP选项只能在ICP模式（通过SWIM）下被修改。有关SWIM编程过程的内容可以参考STM8S闪存编程手册（PM0051）和STM8 SWIM通信协议和调试模块用户手册（UM0470）。 
  不同的芯片的选项字节大小不一样，具体的可以参考芯片数据手册。如用STM8S103F3来举例，选项字节如下：
![选项字节](https://img-blog.csdn.net/20150722185435754)
  STM8S103F对于20脚封装的产品的复用功能重定义位：
![STM8S103F复用功能重定义位](https://img-blog.csdn.net/20150722185535534)
  由此可知我们要将OPT2字节中的AFR1位进行写操作，通过写0，则端口A3复用功能TIM2_CH3，通过写1，端口D2复用功能TIM2_CH3。接下来通过程序实现这个功能，可以修改AFR1的值来看PWM输出是否切换了管脚，如果能，则是实现了写操作。 
  选项字节存放在EEPROM中，所以可以通过读写EEPROM一样的操作方式来修改选项字节。应用程序可以直接向目标地址进行写操作。地址从上面的图中我们已经知道了：0x4803,0x4804。寄存器的配置可以查阅参考手册（RM0016）。 
  相关寄存器操作：
![FLASH_CR2](https://img-blog.csdn.net/20150722185705420)
![FLASH_NCR2](https://img-blog.csdn.net/20150722185740203)
# 测试程序实现
注意：实现程序擦写Option Bytes时，不能运行应用程序，否则会出现错误！不过还是觉得这点麻烦啊，还不如用STVP来擦写，要是能够放在应用程序中共存来擦写就好了，用程序实现复用就这么麻烦吗？希望能探索找到好的方法，最后只找到了个不靠谱的，就是在擦写后加上延时，但是这个时管用时不管用。还是再查阅资料看看是怎么回事？
![ST Visual Develop](https://img-blog.csdn.net/20150722190244872)
  但是用STVP擦写时又遇到了这个错误：
```
Error : Error on Option Bytes (complementary bytes). Reprogram Option Bytes of device
Error : < OPTION BYTE verifying failed.
```
  用STVP来擦写Option Bytes了，先将ROP设置为ON，然后再擦写Option Bytes，会出现两个提示框，选择是(Y)，再之后又将ROP设置为OFF，再次擦写Option Bytes，则又可以用STVD通过stlink来烧写程序并仿真了。
![STVP program option bytes](https://img-blog.csdn.net/20150722190450455)
![T1](https://img-blog.csdn.net/20150722190530697)
![T2](https://img-blog.csdn.net/20150722190602848)
```
测试程序：
```
```cpp
/* MAIN.C file
Functons    :   操作option byte字节,设置IO复用,来修改TIM2的CH3通道PWM输出管脚PA3 or PD2
Date        :   2015年7月22日
Author      :   yicm
Notes       :   
 */
#include<stm8s003f3p.h>
void CLK_init(void)
{
    CLK_ICKR |= 0X01;           //使能内部高速时钟 HSI
    CLK_CKDIVR = 0x08;          //16M内部RC经2分频后系统时钟为8M
    while(!(CLK_ICKR&0x02));    //HSI准备就绪 
    CLK_SWR=0xe1;               //HSI为主时钟源 
}
void Init_GPIO(void)
{
    /*设置为推挽输出,PD2接了LED灯*/
    PD_DDR |= 0X04;             //设置PD2端口为输出模式
    PD_CR1 |= 0X04;             //设置PD2端口为推挽输出模式
    PD_CR2 &= 0XFD;
    PA_DDR |= 0X08;             //设置PA3端口为输出模式
    PA_CR1 |= 0X08;             //设置PA3端口为推挽输出模式
    PA_CR2 |= 0XF7;
}
void Init_Tim2(void)
{
    TIM2_CCMR3 |=   0X70;   //设置定时器2三通道(PD2)输出比较三模式
    TIM2_CCMR3 |= 0X04;     //输出比较3预装载使能
    TIM2_CCER2 |= 0x03;     //通道3使能，低电平有效，配置为输出
    // 初始化时钟分频器为1，即计数器的时钟频率为Fmaster=8M/64=0.125MHZ
    TIM2_PSCR = 0X07;   
    //初始化自动装载寄存器，决定PWM 方波的频率，Fpwm=0.125M/62500=2HZ
    TIM2_ARRH = 62500/256;
    TIM2_ARRL = 62500%256;
    //初始化比较寄存器，决定PWM 方波的占空比：5000/10000 = 50%
    TIM2_CCR3H = 31250/256;
    TIM2_CCR3L = 31250%256;
    //启动计数;更新中断失能
    TIM2_CR1 |= 0x81;
    //TIM2_IER |= 0x00;        
}
void Write_Option_Byte(void)
{   
    unsigned char opt[6] = {0,0,0x00,0,0,0};
    /*解锁Flash*/
    do
    {
        FLASH_DUKR = 0xAE;
        FLASH_DUKR = 0x56;      
    }
    while(!(FLASH_IAPSR & 0X08));
    /*对选项字节进行写操作使能*/
    FLASH_CR2 = 0X80;
    /*互补控制寄存器*/
    FLASH_NCR2 = 0X7F;
    /*写操作,0x02:PD2。0x00:PA3*/
    *((unsigned char *)0x4800) = opt[0];
    *((unsigned char *)0x4801) = opt[1];
    *((unsigned char *)0x4802) = ~opt[1];
    *((unsigned char *)0x4803) = opt[2];
    *((unsigned char *)0x4804) = ~opt[2];
    *((unsigned char *)0x4805) = opt[3];
    *((unsigned char *)0x4806) = ~opt[3];
    *((unsigned char *)0x4807) = opt[4];
    *((unsigned char *)0x4808) = ~opt[0];
    *((unsigned char *)0x4809) = opt[5];
    *((unsigned char *)0x480A) = ~opt[5];  
    /*等待写结束*/
    while(!(FLASH_IAPSR & 0x04));
}
main()
{
    int i;
    Write_Option_Byte();    //运行程序时，屏蔽  
    for(i=0;i<10000;++i);   //延时效果，有时加上延时，能够使擦写和应用程序同时不屏蔽也能管用
    CLK_init();             //擦写时屏蔽，否则下次stlink仿真时会出错
    Init_GPIO();            //擦写时屏蔽，否则下次stlink仿真时会出错
    Init_Tim2();            //擦写时屏蔽，否则下次stlink仿真时会出错
    while (1);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
![](http://static.blog.csdn.net/images/save_snippets.png)
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
[](http://blog.csdn.net/FreeApe/article/details/47008033#)[](http://blog.csdn.net/FreeApe/article/details/47008033#)[](http://blog.csdn.net/FreeApe/article/details/47008033#)[](http://blog.csdn.net/FreeApe/article/details/47008033#)[](http://blog.csdn.net/FreeApe/article/details/47008033#)[](http://blog.csdn.net/FreeApe/article/details/47008033#)
