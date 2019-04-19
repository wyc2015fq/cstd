# 高通Android平台硬件调试之Camera篇 - xqhrs232的专栏 - CSDN博客
2011年06月17日 14:14:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：953标签：[平台																[android																[工作																[通讯																[download																[file](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=download&t=blog)](https://so.csdn.net/so/search/s.do?q=通讯&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://hi.baidu.com/leowenj/blog/item/d886c61c396136f9e0fe0bdb.html](http://hi.baidu.com/leowenj/blog/item/d886c61c396136f9e0fe0bdb.html)
之前一段时间有幸在高通android平台上调试2款camera sensor，一款是OV的5M YUV sensor，支持jpeg out，同时也支持AF，调试比较比较简单，因为别的项目已经在使用了，只是把相关的驱动移植过来就好；另一款是Samsung的一款比较新的3M YUV FF sensor，在最新项目中要使用的，本文以调试该sensor为例，从底层驱动的角度分享一下高通android平台下调试camera的经验，而对于高通平台camera部分的架构以及原理不做过多的介绍。
** 一、准备工作**
    从项目中看，在硬件（板子）ready前，软件部分是要准备好的。单独从底层驱动来看，软件部分可以分为2个部分，一个是高通平台相关的，再一个就是sensor部分的，通常的做法就是把sensor相关的设定移植到高通平台的框架之中。这样就需要先拿到sensor的spec以及厂商提供的sensor register setting file。Spec的用途是清楚高通平台和sensor通讯（读写寄存器）的时序以及相关参数设定；而厂商提供的setting file则是在使用camera各个功能（preview、snapshot...）时候需要写入到sensor中的.
    本项目中，高通平台为MSM7X27，camera为Samsung 5CA。从spec中知道，该sensor的I2C ID为0x78，I2C的通信采用双字节方式，另外也弄清楚了读写sensor寄存器的规则，从调试角度看这些基本上够用了。另外厂商提供的setting file，其实就是寄存器列表，告诉我们再什么时候将哪些寄存器写入什么值，通常是一个寄存器地址再加上一个寄存器的值，不过Samsung提供的是PC上调试使用的文本，需要自己转换成c语言中的二维数组。从文件中看，寄存器数据可以分为几个部分：初始化、IQ设定（tuning相关）、clk设定、preview设定、snapshot设定，基本上有这几个就够了，其他的比如调节亮度啦、设定特殊效果啦、设置白平衡啦等等都可以自己通过spec来完成。
    Sensor部分的东西搞定后，接下来就是修改高通camera部分的驱动了，主要有：
**Kernal部分：**
1、检查Sensor的电源配置，并修改软件中的设定。本项目中使用2.8/1.8/1.5共3个电源。
2、检查并修改sensor reset设置。注意reset的时间设定，务必和spec中一致，否则会导致sensor无法工作。
3、修改I2C驱动，使用双字节读写的接口，并完成读取sensor ID的接口。这个用来检验I2C通讯是否OK
4、导入寄存器设定，分别在初始化、preview、snapshot等几个部分写入对应的寄存器值。
注意：reset以及写寄存器部分一定要按照spec的规定加入一些delay，否则会导致sensor工作异常
**User空间部分：**
这个部分主要是根据硬件的规格来配置VFE，如sensor输出数据的格式，接口方式、分辨率大小、同步信号模式等，比较简单，但一定要检查仔细，任何一个地方不对都会导致调试失败。
    到这里为止，软件部分的准备已经告一段落了。
**二、调试环境准备（板子出来了，但sensor sample还没到位）**
    首先，测试点的准备。
    调试前就需要想好，如果sensor无法工作，要怎么去debug，这就需要去测量一些信号，比如power、reset、I2C、M/P CLK、H/V同步信号、数据信号等，要确保这些信号都可以测量到。
    其次要选择软件的调试环境，这里选择在ADB环境中执行高通的mm-qcamera-test程序来调试，相关的trace都可以打印出来。
    这样就万事俱备，只欠sensor了。
** 三、调试（sensor终于拿到了）**
    将sensor接到板子上，开机后，ADB中运行调试程序，preview画面并没有出来，失败，有点小失望，本来觉得可以一气呵成的，但毕竟这是一个全新的sensor，任何一个地方没有想到位做到位都会导致失败。那就找原因吧。
    1、首先从trace得知，I2C已经读到了sensor的ID：0x05CA,这可以说明I2C通讯是没有问题的
    2、接着检查Sensor的电源配置，测量了供给sensor的3个电源，都是OK的。
    3、测量MCLK，这个是提供给sensor使用的，正常（24MHZ）
    4、测量PCLK，这个是sensor输出的，正常（58MHZ，高通上限为96MHZ），和寄存器中配置的一致。
    5、测量H/V同步信号，这个是sensor输出的，正常。和FPS和分辨率一致。
    6、测量数据信号，这个是sensor输出的，正常。（数据信号，示波器上可以看到）
    这样看来，sensor已经在正常工作了，但为何preview画面没有出来呢？继续检查高通这边的设定。
    从trace看，高通的VFE已经reset并且start了，但一直接没有输出preview数据，这就奇怪了，sensor明明已经输出了，为什么VFE接收后并没有把数据吐出来呢，难道这个sensor输出的数据VFE无法识别？为了验证这个问题，我在另一块板子上测量了OV sensor输出数据的波形，主要是M/P clk、H/V同步信号，然后再拿来对比，不过并没有发现异常，只是H/V同步信号有所不同，主要高低的占空比不太一致，会不会是这样信号的问题呢？为了进一步验证，我同时测量了H/V 信号和数据信号，这时发现OV
 sensor输出的数据信号是包在V帧同步信号的低电平中；而Samsung 5CA输出的数据信号是包在V帧同步信号的高电平中，会不会是因为V信号极性设置不对导致VFE没有读取到sensor输出的数据呢？重新检查了一下高通VFE的设定，果然有一个参数是用来设定V信号极性的，这个参数默认是Active Low的，我这边并没有去修改它。接着把这个参数修改为Active High，重新build、download后，开机运行，Ok了，preview画面可以正常显示了。到这里为止sensor的硬件调试可以算作完成了，后续的其他功能也可以慢慢完善了。
*Note：V同步即帧同步信号，代表一帧数据的开始，VFE会根据该信号来读取一帧数据。              H同步即行同步信号，代表一行数据的开始，VFE会根据该信号来读取一行数据。H信号一定是包在V同步中的*。

