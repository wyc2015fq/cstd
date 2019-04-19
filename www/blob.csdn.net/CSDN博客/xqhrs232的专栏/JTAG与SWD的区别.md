# JTAG与SWD的区别 - xqhrs232的专栏 - CSDN博客
2015年09月14日 10:08:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：450
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://www.openedv.com/posts/list/187.htm](http://www.openedv.com/posts/list/187.htm)
对于JTAG和SWD的使用区别，觉得下面这篇文章讲的比较清晰了，所以转帖到这里，希望对新手有所帮助。
 SWD与JTAG区别及使用情况[转载]
![](http://www.openedv.com/upload/2011/1/5/3e7a1df2b89658e65d299e874aa784da_960.jpg)
    上图是SEGGER说明书中给出的Jlink引脚图，可以对照着看SWD引脚与JTAG引脚的关系。
![](http://www.openedv.com/upload/2011/1/5/afcaeb8a313498ceceda0b008c07243f_85.jpg)
   这是我手边开发板上的JTAG连接图，这个肯定是能用的。
![](http://www.openedv.com/upload/2011/1/5/3e4a2bff496058c8b1a9b7e4f3610156_745.jpg)
    这个是从网上找来的标准的JTAG连接图，供对照参考。
    调试方式既可以用JTAG，也可以用SWD。
以下是一段转自：(http://showvi.com/Blog/ViewAirticle/186)
**SWD 仿真模式概念简述**
**一、SWD 和传统的调试方式区别**
    1. SWD 模式比 JTAG 在高速模式下面更加可靠。 在大数据量的情况下面 JTAG 下载程序会失败, 但是 SWD 发生的几率会小很多。基本使用 JTAG 仿真模式的情况下是可以直接使用 SWD 模式的, 只要你的仿真器支持。 所以推荐大家使用这个模式。
    2. 在大家 GPIO 刚好缺一个的时候, 可以使用 SWD 仿真, 这种模式支持更少的引脚。
    3. 在大家板子的体积有限的时候推荐使用 SWD 模式, 它需要的引脚少, 当然需要的 PCB 空间就小啦！ 比如你可以选择一个很小的 2.54 间距的 5 芯端子做仿真接口。
**二、仿真器对 SWD 模式支持情况**
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
**三、在 MDK 中SWD 模式的设置**
    在调试仿真的时候用J-LINK的Cortex-M3方式已经足够，并且在MDK下他的功能已经做得非常的好，用标准20脚的JTAG下载，速度是非常的快，一般初学者都是这样做的。但是SWD方式似乎速度更快、更加方便、简捷、，对于项目中对板子空间要求严格、I/O口资源紧张的用户来说更加的有利，正常的JTAG需要20管脚，而J-Link 的SWD只需要2根线（PA13/JTMS/SWDIO、PA14/JTCK/SWCLK）就够了（加上电源线也就4根），这样就节省了3个I/O口（PA15/JTDI、PB3/JTDO、PB4/JNTRST）为其它所用，并且可节省一部分板子的空间(只需4个口就可以了)。
    下面我说一下SWD两线仿真的一些步骤、注意事项及需要注意的问题。
    接口的连接如下：将JLINK的1、7、9、20分别与自己的开发板上JTAG的VCC、JTMS、JTCK、GND用杜邦线相连即可！
    接下来告诉大家怎么使用SWD设置：
打开工程OPTION设置：
![](http://www.openedv.com/upload/2011/1/5/09b712afa1ed70371e6dea0982373564_972.jpg)
    在设置中按照上图设置成 SWD 模式, 速度你可以按照你的实际需求来设置, 如果你的板子供电系统不是特别稳定, 纹波比较大或者仿真线比较长可以设置成 500K 或者 1M，如果环境很好当然可以选择 10M , 当然速度会飞起来。
    记得不要忽略了左下方的那个USB还是 TCP 模式, 当然我们是 USB 模式, 因为有的时候默认是 TCP 模式, 这个时候我们忽略这个设置后会仿真常常连接不上的。
![](http://www.openedv.com/upload/2011/1/5/5b0e150c65905983a8ec77fab5159490_636.jpg)
/////转载结束
下面是我自己的试验(用JLINK V8)
（1）按SW方式来调试，实际需要接1，7，9，15，20脚，某些资料上说的需要6个脚（第13脚SWO也需要连上）是不对的，SWO不需要连接。
![](http://www.openedv.com/upload/2011/1/5/a1221e1d19b82896278f6d4e4bf544d4_854.jpg)
    按上所述连接5个引脚时出现的画面
   1脚不接时出现的画面
![](http://www.openedv.com/upload/2011/1/5/a40259920cdc0cf0965d5189bfb84cab_265.jpg)
（2）用两种方式的速度没有太大的区别。
再次测试：
（1）不接第15脚，即RESET脚，仅接4条引线，即Vref，SWDIO,SWCLK,GND。
（2）设置时Reset位选择：AutoDetect，可以正常下载，仿真。
（3）如果Reset位选选择：HW Reset ，则不能正常下载。
（4）速度远没有上面转载中说的那样快，擦除和写入ucosDemo工程约需40多s。以下是这个工程的代码量：
Program Size: Code=96830 RO-data=143650 RW-data=1016 ZI-data=16440
   但是有个现象，即下载一次后退出，然后再次进入（没有重编译时），似乎没有擦除和下载的过程，时间飞快。也许这就是上面所说的时间快的原因？
////以下来自21icbbs// http://bbs.21ic.com/icview-127364-1-1.html
    问：一直在核心板上玩 103ZE，都是些小程序 都是在RAM中调试的，而且只接三根线GND SWDIO SWCLK　(VCC 直接接JLINK内部的)，很爽利。不过今天要试IIS 要放一个大的数据进去，于是要烧到FLASH中去， 发现很快进度条走完， 然后显示失败，放狗一搜,说要接RESET,于是接上,果然下进去了...惨了,我PCB都画好了,都只有4线(VCC SWDIO SWCLK GND)，以后怎么办呢?
    答：可以不接，可以在MDK仿真器的设置里面不使用硬件复位，而是用system reset或者vect reset，前者适用的范围更广
/////转载结束
    JTAG引脚可以被复用为IO口，但是这样一来，JLINK就不能够连上芯片了。解决的方法有两种：
    （1）另写一段程序，不要将JTAG复用为I/O口，然后将这段程序用串口工具写入芯片中；
    （2）将BOOT0/BOOT1设置成为内部RAM启动，那么上电后就不会执行FLASH中的程序，这样JLINK就能顺利“接管”JTAG引脚。
（原链接转载页面打开时弹出了好多广告页面，所以只好黏贴过来了，图片贴一遍，嗯呵）
