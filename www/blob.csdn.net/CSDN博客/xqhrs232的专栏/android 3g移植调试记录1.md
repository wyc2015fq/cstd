# android 3g移植调试记录1 - xqhrs232的专栏 - CSDN博客
2011年12月11日 22:15:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1027
原文地址::[http://hi.baidu.com/huntington/blog/item/955776cfa8834a27f8dc61ad.html](http://hi.baidu.com/huntington/blog/item/955776cfa8834a27f8dc61ad.html)
from：[http://blogold.chinaunix.net/u4/124208/showart_2442922.html](http://blogold.chinaunix.net/u4/124208/showart_2442922.html)
 今天ril还是没调通，不过有进展。
 看了几篇相关的文章和一点代码，对ril有了进一步的了解。
[http://www.netmite.com/android/mydroid/development/pdk/docs/telephony.html](http://www.netmite.com/android/mydroid/development/pdk/docs/telephony.html)
[http://blog.csdn.net/wswupeng/archive/2010/12/11/6069927.aspx](http://blog.csdn.net/wswupeng/archive/2010/12/11/6069927.aspx)
[http://blog.chinaunix.net/u3/118873/showart_2339510.html](http://blog.chinaunix.net/u3/118873/showart_2339510.html)
[http://yangyangzhao.blog.163.com/blog/static/175816366201011542451166/](http://yangyangzhao.blog.163.com/blog/static/175816366201011542451166/)
 尽量把理解的表述清楚，这是一种锻炼。
 ril是radio interface layer的缩写，是android里电话部分很重要的一层，如图：
![](http://hiphotos.baidu.com/huntington/pic/item/0a249458b4125bb69c82048a.jpg)
(1) ril层的作用就是消息中转，承上启下，和之前的红外的lirc差不多。
3G模块的驱动就是一个usbserial.ko，USB转串的驱动，insmod后会生成ttyUSB*节点，作为AT命令通道和数据传输通道，至于对应关系还要在研究，是否可以只用一个ttyUSB0还要研究，不过发送at命令至ttyUSB0还是没问题的，
cat /dev/ttyUSB0 &  然后再echo "at+cgmi" > /dev/ttyUSB0会返回
Manufacturer
OK
之所以会要有ril这一层来处理主要是考虑不同的通信模块AT命令集和格式不通，通过ril可以做一个中转，
当上层要打电话或3g上网时，将产生的消息转化为相应的AT命令发送到串口，再传给下层通信模块，反过来，当通信模块把数据传到串口时，ril再解析数据，将之转化为上层可是别的格式，以消息的方式发出，这就是ril的基本功能。
(2)rild守护进程（代码位置/hardware/ril/rild/rild.c）需要在init.rc中启动，
     属于native framework层
         service ril-daemon /system/bin/rild -l /system/lib/libreference-ril.so --
                                  -d /dev/ttyUSB*
         socket rild stream 660 root radio
         socket rild-debug stream 660 radio system
         user root
         group radio cache inet misc audio
         这里通过-l /system/lib/libreference-ril.so指定ril所用的功能库libreference-ril.so，很重要，负责解释上层的命令同时读取解析AT端口ttyUSB*信息，并上报给上层，代码位置/hardware/ril/reference-ril/reference- ril.c,
接下来建立了两个socket，用来和上层的java framework层通信
 socket rild stream 660 root radio
 socket rild-debug stream 660 radio system
那rild做了什么呢，这个得在好好读程序分析，现在网上看来就是：
获取ril的参数->打开功能库->建立时间循环线程->执行RIL_Init->RIL_register。
首先从/proc/cmdline 的android.ril=读取tty节点信息，但是我的板子却没有android.ril参数，所以怀疑这是问题的一个关键所在，
代码中有个
#define  KERNEL_OPTION  "android.ril="
不知是要kernel配置是弄好才会产生还是这里加上/dev/ttyUSB0，明天试一下，因为proc文件系统是一个伪文件系统，它只存在内存当中，而不占用外存空间，运行时才产生的。
另外rild有个测试程序radiooptions(hardware/ril/rild/radiooptions.c)今天也编好了，但具体用的时候还有问题，明天再研究,今天先到这里。
