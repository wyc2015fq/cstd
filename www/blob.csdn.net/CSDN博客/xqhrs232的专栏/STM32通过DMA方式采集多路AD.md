# STM32通过DMA方式采集多路AD - xqhrs232的专栏 - CSDN博客
2018年09月18日 14:43:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：200
原文地址::[https://blog.csdn.net/c12345423/article/details/52919148](https://blog.csdn.net/c12345423/article/details/52919148)
相关文章
1、（STM32）AD功能的连接配置方式------单AD多通道实现----[https://blog.csdn.net/qq_40199189/article/details/79473245](https://blog.csdn.net/qq_40199189/article/details/79473245)
2、STM32 ADC多路采集----[https://wenku.baidu.com/view/18fe62e69a89680203d8ce2f0066f5335a816739.html](https://wenku.baidu.com/view/18fe62e69a89680203d8ce2f0066f5335a816739.html)
一个项目需求：使用STM32采集多路ＡＤ信号。
一般采集多路信号需要使用ＤＭＡ方式，步骤如下：
1.使能相应ADC通道，这里使用ADC1的4个通道，分别是ADC123_IN0 ~ ADC123_IN3,
注：ADC123_IN0 表示ADC1, 2, 3 都可以使用该通道。
2.配置ADC，
将ADC设为多通道循环扫描模式，设置各通道扫描顺序（人为设定）
3.配置DMA通道
关键点在DMA的配置，ADC的采样值放在相应的数据寄存器中，通过DMA通道将该寄存器的值传输到内存中的某处（定义一个数组），内存地址自增，外设地址固定。则数组中的值就是各通道的采集数据。
代码如下：
adc.c
![](https://img-blog.csdn.net/20161025104455371?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20161025104502261?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20161025104509049?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20161025104514893?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
main.c
![](https://img-blog.csdn.net/20161025104520902?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
