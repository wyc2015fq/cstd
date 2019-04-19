# STM32的SWD调试方式 - xqhrs232的专栏 - CSDN博客
2015年09月13日 11:21:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：601
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://blog.csdn.net/liuxizhen2009/article/details/8593103](http://blog.csdn.net/liuxizhen2009/article/details/8593103)
相关文章
1、SWD与JTAG之间的区别----[http://wenku.baidu.com/link?url=l90BGOLjugCwC-TQSqpXe2sypmKZ9Av5oLuNIxBz2Rg1St_QRgOgCnog82x5ah4S660EwHRQoc-hKGisUtFHTMZHDmbU3Flhu35K3xxbMyG](http://wenku.baidu.com/link?url=l90BGOLjugCwC-TQSqpXe2sypmKZ9Av5oLuNIxBz2Rg1St_QRgOgCnog82x5ah4S660EwHRQoc-hKGisUtFHTMZHDmbU3Flhu35K3xxbMyG)
2、JLINK 与STM32的SWD连接接线方式----[http://www.openedv.com/posts/list/1747.htm](http://www.openedv.com/posts/list/1747.htm)
3、stm32 JTAG和SWD的使用区别----[http://www.51hei.com/mcu/1955.html](http://www.51hei.com/mcu/1955.html)
[http://www.ichanging.org/stm32-swd.html](http://www.ichanging.org/stm32-swd.html)
 找到一篇比较好的 关于stm32  SWD模式 下载 调试 配置文章 整理如下：
    我们比较常用的是Jlink下载器 ，这种下载器有一个缺点就是使用的Jtag 20PIN接口，太多的PIN会导致一些小型的PCB板很拥挤，也会增加布线的难度。 而使用SWD接口下载调试，只需要要使用4个PIN:  GND, RST, SWDIO, SWDCLK ，而且下载速度可以达到10M/s，优势显而易见。
    以下转自：[http://www.openedv.com/posts/list/187.htm](http://www.openedv.com/posts/list/187.htm)
    我们所使用的Jtag  20PIN引脚图：                         相关原理图：
![jtag IO定义.jpg](http://changing-typecho.stor.sinaapp.com/usr/uploads/2012/09/3300570723.jpg)![Jtag 原理图.jpg](http://changing-typecho.stor.sinaapp.com/usr/uploads/2012/09/751556913.jpg)
**SWD 仿真模式概念简述**
一、SWD 和传统的调试方式区别
- SWD 模式比 JTAG 在高速模式下面更加可靠。 在大数据量的情况下面 JTAG 下载程序会失败, 但是 SWD 发生的几率会小很多。基本使用 JTAG 仿真模式的情况下是可以直接使用 SWD 模式的, 只要你的仿真器支持。 所以推荐大家使用这个模式。
- 在大家 GPIO 刚好缺一个的时候, 可以使用 SWD 仿真, 这种模式支持更少的引脚。
- 在大家板子的体积有限的时候推荐使用 SWD 模式, 它需要的引脚少, 当然需要的 PCB 空间就小啦！ 比如你可以选择一个很小的 2.54 间距的 5 芯端子做仿真接口。
二、仿真器对 SWD 模式支持情况
1. 市面上的常用仿真器对 SWD 模式支持情况
        (1) JLINKV6 支持 SWD 仿真模式， 速度较慢。
        (2) JLINKV7 比较好的支持 SWD 仿真模式, 速度有了明显的提高，速度是 JLINKV6 的 6 倍。 
        (3) JLINKV8 非常好的支持 SWD 仿真模式, 速度可以到 10M。
        (4) ULINK1 不支持 SWD 模式。
        (5) 盗版 ULINK2 非常好的支持 SWD 模式， 速度可以达到 10M。
        (6) 正版 ULINK2 非常好的支持 SWD 模式， 速度可以达到 10M。
2. SWD 硬件接口上的不同
        (1) JLINKV6 需要的硬件接口为: GND, RST, SWDIO, SWDCLK
        (2) JLINKV7 需要的硬件接口为: GND, RST, SWDIO, SWDCLK
        (3) JLINKV8 需要的硬件接口为: VCC, GND, RST, SWDIO, SWDCLK （注：下面有我自己用JLINKV8的实际连线及相应实验结果）
        (4) ULINK1 不支持 SWD 模式
        (5) 盗版 ULINK2 需要的硬件接口为: GND, RST, SWDIO, SWDCLK
        (6) 正版 ULINK2 需要的硬件接口为: GND, RST, SWDIO, SWDCLK
    由此可以看到只有 JLINKV8 需要 5 个引脚， 即多了一个VCC引脚，其好处是: 仿真器对目标板子的仿真需要用到 RST 引脚, 使用仿真器内部的 VCC 作这个功能其实并不是非常美妙。 因此，JLINKV8 选择了只和目标板共 GND, 但不共 VCC。 因此我觉得这种模式最合理, 当然通常情况下仿真器和目标板共 GND 和 VCC 是没有错的。
三、在 MDK 中SWD 模式的设置
    在调试仿真的时候用J-LINK的Cortex-M3方式已经足够，并且在MDK下他的功能已经做得非常的好，用标准20脚的JTAG下载，速度是非常的快，一般初学者都是这样做的。但是SWD方式似乎速度更快、更加方便、简捷、，对于项目中对板子空间要求严格、I/O口资源紧张的用户来说更加的有利，正常的JTAG需要20管脚，而J-Link 的SWD只需要2根线（PA13/JTMS/SWDIO、PA14/JTCK/SWCLK）就够了（加上电源线也就4根），这样就节省了3个I/O口（PA15/JTDI、PB3/JTDO、PB4/JNTRST）为其它所用，并且可节省一部分板子的空间(只需4个口就可以了)。
    下面我说一下SWD两线仿真的一些步骤、注意事项及需要注意的问题。
    接口的连接如下：将JLINK的1、7、9、20分别与自己的开发板上JTAG的VCC、JTMS、JTCK、GND用杜邦线相连即可！
    接下来告诉大家怎么使用SWD设置：
打开工程OPTION设置：
![SWD 设置.jpg](http://changing-typecho.stor.sinaapp.com/usr/uploads/2012/09/1957487761.jpg)
        在设置中按照上图设置成 SWD 模式, 速度你可以按照你的实际需求来设置, 如果你的板子供电系统不是特别稳定, 纹波比较大或者仿真线比较长可以设置成 500K 或者 1M，如果环境很好当然可以选择 10M , 当然速度会飞起来。
    记得不要忽略了左下方的那个USB还是 TCP 模式, 当然我们是 USB 模式, 因为有的时候默认是 TCP 模式, 这个时候我们忽略这个设置后会仿真常常连接不上的。
按SW方式来调试，实际需要接1，7，9，15，20脚，某些资料上说的需要6个脚（第13脚SWO也需要连上）是不对的，SWO不需要连接。
我的Jilnk 和 开发板 接线图：
![Jtag-Jlink-接法.jpg](http://changing-typecho.stor.sinaapp.com/usr/uploads/2012/09/103422703.jpg)
- 白色线 : 3.3v      
- 黑色线 : GND      
- 灰色线：SWIO
- 紫色线：SWCLK
        关于是否要接上 Reset 引脚的问题 我测试时并没有接上  可以正常下载和调试  而且下载速度设置为 10Mhz  仍然没有问题  速度的确快了很多 。。

