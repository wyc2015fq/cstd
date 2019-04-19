# STM32 中JTAG 引脚作为普通IO口设置方法 - xqhrs232的专栏 - CSDN博客
2018年04月24日 21:55:37[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：269
原文地址::[https://blog.csdn.net/iccome/article/details/32941789](https://blog.csdn.net/iccome/article/details/32941789)
一次画STM32 的PCB ，因为采用了SWD 调试，认为JTAG的引脚PB3,PB4,没有用到就做了普通IO口，麻烦从此引起了。
设置PB3,PB4均为输出口，且输出高电平，用万用表测量，PB4为高，PB3不是高电平，在看MDK 中的寄存器值，PB3,PB4都是高啊？寄存器的值怎么和实际的值不一样了？
唉，都让我怀疑是不是引脚接错了，反复测量后发现引脚确实是对的，但为什么PB3能输出高，而PB4不可呢？
不知道，问度娘，后来在一个帖子上发现了相关回复：
首先，STM32F10x系列的MCU复位后，PA13/14/15 & PB3/4默认配置为JTAG功能。有时我们为了充分利用MCU I/O口的资源，会把这些
端口设置为普通I/O口。具体方法如下：
在GPIO_Configuration(); // 配置使用的 GPIO 口：
GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
// 改变指定管脚的映射 GPIO_Remap_SWJ_Disable SWJ 完全禁用（JTAG+SW-DP）
GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
// 改变指定管脚的映射 GPIO_Remap_SWJ_JTAGDisable ，JTAG-DP 禁用 + SW-DP 使能
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
注意：不要忘记在RCC_Configuration()中开启AFIO时钟，
  //AFIO时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
（以上代码出自 [http://www.cnblogs.com/pulan/archive/2012/03/19/2921587.html](http://www.cnblogs.com/pulan/archive/2012/03/19/2921587.html)）
我就按照上面抄了一遍，发现没有效果，后来发现有个专家说了句：
AFIO 时钟未设置，GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,
 ENABLE) 这句不会生效，也就是要先设置时钟，才能配置相应端口
后变换了下顺序，先设RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,
 ENABLE);，再调用GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE)  就完美可以了

