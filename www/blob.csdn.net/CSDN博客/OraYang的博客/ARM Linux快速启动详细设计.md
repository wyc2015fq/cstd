
# ARM Linux快速启动详细设计 - OraYang的博客 - CSDN博客

2017年01月04日 22:20:23[OraYang](https://me.csdn.net/u010665216)阅读数：1332



# 1
# 背景
嵌入式系统(Embedded system)，是一种“嵌入机械或电气系统内部、具有专属功能的计算机系统”。被嵌入的系统通常是包含硬件和机械部件的完整设备。嵌入式系统控制着今天许多常用设备，其中百分之九十八的微处理器被生产用来作为嵌入式系统的组件。与通用计算机如个人计算机相比，嵌入式系统计算机设计灵活，低功耗，体积小以及更低的单位成本，可以处理多种运算情况，以满足广大终端用户的需要。现在常见的很多设备都采用简单的嵌入式系统来控制，以达到最高效。
区别于可以执行多重任务的通用型计算机，嵌入式系统是为某些特定任务而设计的。有些系统则必须满足实时性要求，以确保安全性和可用性；另一些系统则对性能要求很低甚至不要求性能，以简化硬件、降低成本。嵌入式系统的关键特性是专用于处理特定的任务，因此工程师能对其进行优化，以降低产品的体积和成本，提升可靠性和性能。快速启动(Fastboot)作为嵌入式系统优化中极为重要的一环，能够根据客户需求，定制相应功能的系统，旨在优化系统启动时间，从而改善用户体验。
# 2 系统启动时间
本文中，fastboot指最小化系统启动的时间。系统启动的时间定义是从应用上电到系统可用这一过程所消耗的时间。系统可用则可以根据用户的不同需求有着不同的定义:
1. 像手机一样设备主屏幕的出现
2. 汽车后视视频在屏幕上显示
3.一个声音信号的出现或者一个LED信号灯的闪烁
4.一个Linux提示
5. 设备网络上可被发现
6. 关键性的外设可用

## 3 快速启动设计思路
下图给出了引导处理的概述，所有这些步骤将影响启动处理时间。
![](https://img-blog.csdn.net/20170104222809325?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

图3.1 启动处理流程
1、下列是常见用来考虑实现快速启动的事项:
2、从有着更高读写速率的存储器中导入镜像
3、尽可能早地导入必要的软件模块
4、优化Boot loader处理过程
5、优化内核解压过程
6、优化系统服务
7、如果可以的话，卸载一些板极组件对一些控制器(e.g.MCU)的初始化，例如传感器的初始化。
## 3.
## 1 Linux
## 系统启动优化步骤
Linux应用启动流程一般可以分为以下四个部分：
Ø Boot loader
Ø 内核（Kernel）
Ø 系统服务（System services）初始化
Ø 应用
![](https://img-blog.csdn.net/20170113121921820?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3.2  Linux 正常启动流程
接下来将集中对Linux启动流程的四个部分进行优化，以APPE为例，它并不依赖于第三部分——系统服务初始化，因此在优化启动时间的设计中可以先启动APPE应用再启动系统服务。在bootloader阶段大部分执行时间花在了将二进制代码拷贝到DDR或者等待DMA传输二进制代码完成中。Bootloader一般分为两级boot，本论文将集中优化3个部分：
Ø bootloader
Ø Kernel
Ø APPE应用，如下图所示。
![](https://img-blog.csdn.net/20170113122024097?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3.3
 没有系统服务的linux启动流程
## 3.2 Boot Loader
## 快速启动优化流程
(1) Single Boot Loader
Boot Loader 的启动过程启动过程可以分为单阶段(Single Stage)、多阶段(Multi-Stage)两种。通常多阶段的Bootloader能提供更为复杂的功能，以及更好的可移植性。从固态存储设备上启动的Bootloader大多都是
 2 阶段的启动过程。这从前面的硬件实验可以很好地理解这点：第一阶段使用汇编来实现，它完成一些依赖于 CPU 体系结构的初始化，并调用第二阶段的代码。第二阶段则通常使用C语言来实现，这样可以实现更复杂的功能，而且代码会有更好的可读性和可移植性。为了进行优化，本文使用单阶段启动模式即falcon模式。
通常正常两级boot流程如下：
![](https://img-blog.csdn.net/20170113122125113?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3.4正常两级boot
而faclon Boot流程如下：
![](https://img-blog.csdn.net/20170113122215137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3.5faclon
 启动模式
Falcon启动模式如下：
Step 1执行SPL
Step 2检查需要导入哪种镜像
Step 3导入镜像
Step 4装载参数区域及DTB内核
Step 5转移控制给镜像
(2)使能D-Cache及MMU
从上述Boot Loader详细研究中可以发现，在Boot loader中大部分的执行时间主要开销在搬运二进制代码到DDR中，等待DMA传输数据的完成。而在搬运数据的过程中会用到缓存（Cache）。默认情况下，在Boot Loader第一阶段会关闭D-cache以及MMU，众所周知Cache是位于CPU与主存储器DRAM之间的少量超高速静态存储器SRAM（static
 RAM），其是为了解决CPU与主存之间速度匹配问题而设置的。
(a) Cache概述
1. Cache的定位
Cache是位于主存 (即是内存)与CPU内部的寄存器之间的一个存储设施，用来加快cpu与内存之间数据与指令的传输速率，从而加快处理的速度。
2. Cache的作用
根据cache的定位可以看出来，它是用来加快cpu从内存中取出指令的速度，但我们都知道，在设备上电之初，我们的内存初始化比较慢一拍，当cpu初始化了，但内存还没准备好之后，就对内存进行数据读，那么势必会造成了指令取址异常，系统就会挂了。所以，在u-boot的上电之初，就得关闭掉数据cache，
指令的cache关闭与不关闭没有太大的关系。
3.关闭ＭＭＵ
MMU在设备上电之初是没有任何作用的，也就是说，在u-boot的初始化之初执行汇编的那一段代码中，包括后面的初始化一些具体的外设时，访问的都是实际的地址，MMU的打开起不到任何的意义，为了不影响启动之初对程序的启动，关闭掉MMU设备是常用的做法。
Cache又分为I-cache（用来存指令）和D-cache（用来存数据）理论上在打开cache的情况下，能够提高数据搬运速度，而在一级boot下是默认关闭MMU以及D-cache，因此需要在合适的情况下使能D-cache及MMU。在U-boot源码中cache的使能是有封装好的函数直接操作。所以只要在其他设备初始化正确的情况下，配置好TLB及调用。


