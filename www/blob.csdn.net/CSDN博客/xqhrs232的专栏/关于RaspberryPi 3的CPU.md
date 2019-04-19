# 关于RaspberryPi 3的CPU - xqhrs232的专栏 - CSDN博客
2017年03月05日 17:58:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：673
原文地址::[http://blog.csdn.net/siberiabear/article/details/51588456](http://blog.csdn.net/siberiabear/article/details/51588456)
# 关于RaspberryPi 3的CPU
> 
固定链接：[https://www.zybuluo.com/SiberiaBear/note/336984](https://www.zybuluo.com/SiberiaBear/note/336984)
> 
这是一篇翻译文章，翻译自树莓派官网的一篇对树莓派3的介绍，后边添加了一些自己的理解，用3一段时间了，也该了解一下了。 
翻译网址：[https://www.raspberrypi.org/products/raspberry-pi-3-model-b/](https://www.raspberrypi.org/products/raspberry-pi-3-model-b/)
树莓派3是第三代树莓派，它在2016年2月份开始取代掉上一代的树莓派2B，树莓派3代的配置如下：
- 一个1.2GHz 64位四核ARMv8 CPU。(稍后来聊聊CPU的事情)
- 802.11n无线 LAN。
- 蓝牙4.1。
- 低功耗蓝牙。
同时，它和Pi2具有同样的：
- 4个USB端口。
- 40针GPIO。
- HDMI端口。
- 以太网端口。
- 3.5mm音频接口与复合视频输出。
- 摄像头端口（CSI）。
- 显示端口（DSI）。
- Micro SD 卡插槽（插拔式，代替原来的按压式）。可以防止意外将SD卡按出来，我就干过这事。
- VideoCore IV 3D 图像处理核心。
3代树莓派在外形上具有着和2代树莓派完全相同，以及完整的兼容树莓派1和2代。（应该是说在功能上和软件上兼容）
我们推荐在学校和一些常规场合使用树莓派3，如果需要将树莓派使用在[嵌入式](http://lib.csdn.net/base/embeddeddevelopment)场合，比较推荐用Pi Zero（树莓派工业板）或Module
 A+，这样的板卡适合嵌入式工程以及低功耗领域。
完。
接下来我瞎聊聊，官网上对CPU的宣传是：
> 
A 1.2GHz 64-bit quad-core ARM Cortex-A53 CPU (~10x the performance of Raspberry Pi 1)
再翻译一段：来自[https://www.raspberrypi.org/blog/raspberry-pi-3-on-sale/](https://www.raspberrypi.org/blog/raspberry-pi-3-on-sale/)
对于树莓派3，博通公司给我支持了一个新的芯片——BCM2837。这个芯片保留了与BCM2835和BCM2836完全相同的基础[架构](http://lib.csdn.net/base/architecture)。所以，所有的树莓派工程和对树莓派硬件的准确描述教程都会完整的兼容。一个900MHz
 32位的四核Cortex-A7 CPU被一个自定义的1.2GHz 64位四核Cortex-A53 ARM取代。这个新的芯片包含了增强33%的系统时钟速率以及各种架构上的增强，在性能上比树莓派2要提升50%-60%，或者粗略的说要比最原始的树莓派性能提升10倍。
`James Adams`用2015年的后半年时间设计了一系列的原型，将BCM2837芯片与BCM43438无线集成芯片相结合。他努力去将无线功能集成安装到和树莓派1代B+板和2代B板近乎一样的板卡上。唯一的改变是LED灯的位置，将他们的位置移到了SD插槽的另一边，从而为无线天线腾出空间。
```
Roger
 Thornton
```
奔波于许多无线一致性检测的调查（强调很贵），为树莓派3可以在全球几乎所有国家同时登陆。`Phil Elwell`开发了无线局域网和蓝牙的软件部分。
所有的连接器都在不变的位置并且功能也未变，板子仍然运行于一个5V供电的microUSB无线适配器。这一次，如果你需要在树莓派上连接高功率USB设备，我们推荐使用一个2.5A的适配器。
最重要的一句话：
> 
At launch, we are using the same 32-bit Raspbian userland that we use on other Raspberry Pi devices; over the next few months we wil investigate whether there is value in moving to 64-bit mode.
就是说，现在依然是32位的Raspbian系统，在接下来的几个月会提供64位模式。
我在尝试查看我树莓派3的CPU（拿到手还不到2个星期）： 
第一种方法：
`cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq`- 1
- 1
得到的结果是： 600000 
第二种方法：
`vcgencmd get_config arm_freq`- 1
- 1
得到的结果是： arm_freq=800 
第三种方法：
`lscpu`- 1
- 1
得到的结果是： 
第四种方法：
```
Architecture:          armv7l
Byte Order:            Little Endian
CPU(s):                4
On-line CPU(s) list:   0-3
Thread(s) per core:    1
Core(s) per socket:    4
Socket(s):             1
Model name:            ARMv7 Processor rev 4 (v7l)
CPU max MHz:           800.0000
CPU min MHz:           600.0000
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
```matlab
cat cpuinfo
```
- 1
- 1
得到的结果是：
```
processor       : 0
model name      : ARMv7 Processor rev 4 (v7l)
BogoMIPS        : 38.40
Features        : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm crc32
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x0
CPU part        : 0xd03
CPU revision    : 4
processor       : 1
model name      : ARMv7 Processor rev 4 (v7l)
BogoMIPS        : 38.40
Features        : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm crc32
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x0
CPU part        : 0xd03
CPU revision    : 4
processor       : 2
model name      : ARMv7 Processor rev 4 (v7l)
BogoMIPS        : 38.40
Features        : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm crc32
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x0
CPU part        : 0xd03
CPU revision    : 4
processor       : 3
model name      : ARMv7 Processor rev 4 (v7l)
BogoMIPS        : 38.40
Features        : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm crc32
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x0
CPU part        : 0xd03
CPU revision    : 4
Hardware        : BCM2709
Revision        : a22082
Serial          : 00000000972ee6f7
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
仅供参考。（对于工作频率，不要太纠结，我的这个板子是锁频了的，正常状态下最大时钟频率确实是1.2GHz，没问题）
对于为什么硬件是BCM2709而不是BCM2837，我在[这篇文章](http://cedar-renjun.github.io/2015/10/20/something-about-BCM2709-and-BCM2836/)中查到，一个是内核开发者的型号，一个是芯片发布者的型号，但从另外一个[网站](https://wiki.openwrt.org/toh/raspberry_pi_foundation/raspberry_pi)中发现，BCM2837对应的处理器族应该是BCM2710，这里依然是BCM2709是什么意思，我可以保证我的树莓派是3代，因为可以wifi和蓝牙，另外系列号是a22082，也是树莓派3，从启动日志（dmesg）中可以发现也有树莓派3代B板的输出。 
可以从[这个谷歌论坛](https://groups.google.com/forum/?fromgroups#!topic/golang-nuts/sGOMvPS26nY)找找答案，我没耐心看下去了，一些网友说可能是没有修改内核文件，不过我更赞同：
> 
They can ship the same image, and use 32-bit by default, while adding 
a 64-bit kernel in the repository, so then users can just install it 
easily, without having to compile it themselves.
可能是因为为了兼容旧版本的软件和[操作系统](http://lib.csdn.net/base/operatingsystem)，在树莓派3上依然使用了ARMv7的32位指令集，即使芯片支持ARMv8指令集，但由于考虑到兼容性，现在还没有推出64位的系统和软件来运行，所以固件依然保留32位。刚才我翻译过了，后边官方会出64位的固件。（话说不知道会误导多少为了图新鲜买树莓派3却看着2代的图书和网络教程调试的童鞋，将来3代换了64位版本，官方还是要花大力气在教程和引导的环节的）
另外[这个网址](https://www.element14.com/community/community/raspberry-pi/blog/2016/02/29/the-most-comprehensive-raspberry-pi-comparison-benchmark-ever)可以参考，里边对3个版本的树莓派CPU和性能进行了详细的对比。
[这个网址](https://www.raspberrypi.org/forums/viewtopic.php?f=28&t=138453)是我最初开始思考并查看的论坛，名字是：我的Pi3给出了错误信息？可以参考。
另外一些题外话，用了一段时间，3代的板子确实发热严重，必须要配散热片，硬件设计上还有待考究，WIFI用起来比较方便，能省去一个USB口接外置无线网卡。还有，必须至少是2A的电源给它供电，由于有了无线模块，它需要更多电流。
