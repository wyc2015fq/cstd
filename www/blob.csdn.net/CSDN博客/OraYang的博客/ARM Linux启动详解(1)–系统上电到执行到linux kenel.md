
# ARM Linux启动详解(1)–系统上电到执行到linux kenel - OraYang的博客 - CSDN博客

2017年01月04日 19:11:36[OraYang](https://me.csdn.net/u010665216)阅读数：3124



# 1.引言
所有嵌入式系统都能从系统架构角度上分成四个层次：
1.**引导加载程序（****Boot Loader）：**由可选的固件代码和Boot Loader两大部分，一般情况下都指Boot
 Loader。
2.**Linux****内核（****Linux Kernel）：**区别于通用计算机的系统内核，基于特定的嵌入式板子的定制内核。
3.**文件系统（****File System****）****:**一种基于flash存储设备的文件系统，主要用来存储和组织计算机相关的数据。
4.**用户应用程序****(User Application):**为了完成某项或某几项特定任务而被开发运行于特定操作系统（嵌入式系统）之上的应用程序。众所周知，PC系统的引导过程主要由固化在计算机主板上的固件程序BIOS来完成。BIOS的主要功能是对计算机进行硬件控制从而完成相关的硬件自检与硬件资源的分配，在相关硬件初始化完成后，BIOS会把控制权移交给PC系统的Boot
 Loader。此时Boot Lader将相应的Kernel Image从硬盘上搬运到RAM中，接着程序掉转到Kernel的入口点，接下来便是启动操作系统了。
与PC机的体系结构不同的是嵌入式系统中是不存在BIOS那样的固件程序的，嵌入式系统的整个上电起初的加载任务都是由内嵌在ROM中的一小段程序即Boot
 Loader来完成。以本次课题中的嵌入式系统为例，系统在上电或复位后，系统的Boot Loader程序通常都是从地址0x00处开始执行的。
本节将从Linux 引导流程、Boot Loader的概念、Boot Loader的主要任务、Boot
 Loader的框架结构以及Boot Loader的优化等四个方面来讨论嵌入式系统的Boot Loader及其Fast
 boot优化流程。
# 2.Linux引导流程
早期时，计算机的启动是依赖于一条包含引导程序的纸带或者通过手工使用开关来加载引导程序。尽管目前的计算机已经装备了很多工具来简化引导过程，但是这一切并没有对整个过程进行必要的简化。
让我们先从高级的视角来查看Linux 引导过程，这样就可以看到整个过程的全貌了。然后将回顾一下在各个步骤到底发生了什么。在整个过程中，参考一下内核源代码可以帮助我们更好地了解内核源代码树，并在以后对其进行深入分析。
下图是Linux引导过程的流程图：
![](https://img-blog.csdn.net/20170113122455969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图2.1 Linux 引导流程图
当嵌入式系统断电复位后，嵌入式处理器会执行位于ROM内已知位置处的一段代码。在PC机中，PC系统的引导过程主要由固化在计算机主板上的固件程序BIOS来完成。BIOS的主要功能是对计算机进行硬件控制从而完成相关的硬件自检与硬件资源的分配，在相关硬件初始化完成后，BIOS会把控制权移交给PC系统的Boot Loader。此时Boot Lader将相应的Kernel Image从硬盘上搬运到RAM中，接着程序掉转到Kernel的入口点，接下来便是启动操作系统了。
这就是Linux 引导的整个过程。
## 2.1 Boot Loader的概念
通俗地讲，Boot Loader就是内嵌在ROM区的一小段启动程序。这段程序先于操作系统启动前开始执行。
通过Boot Loader，我们可以初始化硬件设备、建立内存空间的映射图，从而将系统的软硬件环境带到一个合适的状态，以便为最终调用操作系统内核准备好正确的环境[9]。
通常，Boot Loader 是严重地依赖于硬件而实现的，特别是在嵌入式世界。因此，在嵌入式世界里建立一个通用的 Boot Loader 几乎是不可能的[6]。即使如此，我们仍然能顾将 Boot Loader 抽象出一些通用的概念，来指导用户做基于特定的 Boot Loader引导程序的设计与实现。
**1. Boot Loader 所支持的 CPU 和嵌入式板**
一般来说，由于嵌入式产品的多样性，对于不停地CPU架构，诸如ARM7、ARM9、ARM11、ARM15等等不同的ARM内核体系都对应着不同的Boot Loader引导程序。当然了相应的也有一些通用的 Boot Loader引导程序支持多种CPU架构，比如 U-Boot引导程序，既支持 ARM 体系结构又支持MIPS体系结构。嵌入式系统硬件层的配往往也影响到Boot
 Loader引导程序的通用性。这恰恰说明了，当我们移植A-EVM板的Boot Loader引导程序到B-EVM板上时，往往要根据目标板的板极结构对Boot Loader源码作相应的修改。
**2. Boot Loader 的安装媒介（Installation Medium）**
在嵌入式系统板上电重启后，中央处理器会在ROM区的某个特定位置执行程序。比如，Dra7xx-EVM板上的ARM15在上电复位时会从地址 0x00 开始执行程序。基于ARM核的嵌入式系统有诸如ROM、Nand Flash、Nor Flash、EEPROM这类固态存储设备。因此在嵌入式系统断电重启后，CPU首先执行的程序是Boot
 Loader程序。如下图所示，下图1就是固态存储设备的空间分配结构图，从左往右依次是启动引导程序、启动参数、Linux内核、根文件系统镜像。
。
![](https://img-blog.csdn.net/20170113122606455?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图2.2固态存储设备的典型空间分配结构
**3. 用来控制 Boot Loader 的设备或机制**
通常来讲，开发人员一般通过USB串口线连接目标开发板，并通过个人电脑上的专用软件来实现主机与目标板之间的通信，达到打印串口信息，输入控制指令的目的。
**4. Boot Loader 的启动过程是单阶段（Single Stage）还是多阶段（Multi-Stage）**
Boot Loader引导程序的执行不是一蹴而就的，通常需要分成一级或多级阶段。多级启动模式比较复杂，能够支持多种功能，单级阶段做了功能上的优化，比较简单。我们平时使用的Boot Loader一般是两级启动模式，即Stage 1和Stage 2。
**5. Boot Loader 的操作模式 (Operation Mode)**
大多数Boot Loader 都包含两种不同的操作模式："启动加载"模式和"下载"模式，这种区别仅对于开发人员才有意义。但从最终用户的角度看，Boot Loader 的作用就是用来加载操作系统，而并不存在所谓的启动加载模式与下载工作模式的区别。
**启动加载（****Boot loading）模式：**这种模式也称为"自主"（Autonomous）模式。也即 Boot Loader 从目标机上的某个固态存储设备上将操作系统加载到 RAM 中运行，整个过程并没有用户的介入。这种模式是 Boot Loader
 的正常工作模式，因此在嵌入式产品发布的时侯，Boot Loader 显然必须工作在这种模式下。
**下载（****Downloading）模式：**在这种模式下，目标机上的Boot Loader 将通过串口连接或网络连接等通信手段从主机（Host）下载文件，比如：下载内核映像和根文件系统映像等。从主机下载的文件通常首先被 Boot Loader 保存到目标机的
 RAM 中，然后再被 Boot Loader 写到目标机上的FLASH 类固态存储设备中。Boot Loader 的这种模式通常在第一次安装内核与根文件系统时被使用；此外，以后的系统更新也会使用 Boot Loader 的这种工作模式。工作于这种模式下的 Boot Loader 通常都会向它的终端用户提供一个简单的命令行接口。
像Blob 或 U-Boot 等这样功能强大的 Boot Loader 通常同时支持这两种工作模式，而且允许用户在这两种工作模式之间进行切换。比如，Blob 在启动时处于正常的启动加载模式，但是它会延时 10 秒等待终端用户按下任意键而将 blob 切换到下载模式。如果在 10 秒内没有用户按键，则 blob 继续启动 Linux 内核。
**6. BootLoader****与主机之间进行文件传输所用的通信设备及协议**
最常见的情况就是，目标机上的 Boot Loader 通过串口与主机之间进行文件传输，传输协议通常是 xmodem／ymodem／zmodem 协议中的一种。但是，串口传输的速度是有限的，因此通过以太网连接并借助 TFTP 协议来下载文件是个更好的选择。
此外，在论及这个话题时，主机方所用的软件也要考虑。比如，在通过以太网连接和 TFTP 协议来下载文件时，主机方必须有一个软件用来的提供 TFTP 服务。
在讨论了 BootLoader 的上述概念后，下面我们来具体看看 BootLoader 的应该完成哪些任务。
## 2.2 Boot Loader的主要任务与典型结构框架
我们知道除了一开始Boot Loader引导程序是在ROM区运行的，接下来的Linux内核、根文件系统都是被搬到RAM中运行的，因为与ROM相比RAM的读写速度远远高于ROM，所以Boot Loader执行完后要跳转到正确的RAM区继续执行接下来的程序。
为了正确地启动操作系统，Boot Loader引导程序需要准确无误地调用内核，引导操作系统的执行。由于 Boot Loader引导程序的实现依赖于 CPU 的体系结构，因此大多数 Boot Loader 都分为 stage1 和 stage2两大部分。在一级启动中（stage 1），主要执行的是一段汇编代码，完成硬件初始化等功能。在二级启动中(stage2 )
 则通常执行一段C语言程序，其目的主要是为了完成更多复杂的功能，以及便于后期的开发与维护。
Boot Loader引导程序的一级启动（stage 1）通常包括以下步骤：
**（1）第一阶段的功能**
Ø 硬件设备初始化
Ø 加载Boot Loader第二阶段代码到RAM空间
Ø 设置好栈
Ø 跳转到第二阶段代码入口
Boot Loader启动第一阶段流程图如下所示：
![](https://img-blog.csdn.net/20170113122856255?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图2.3Boot Loader启动第一阶段流程图
Boot Loader 的 stage2 通常包括以下步骤(以执行的先后顺序)：
**（2）第二阶段的功能**
Ø 初始化本阶段使用的硬件设备
Ø 检测系统内存映射
Ø 将内核从Flash读取到RAM中
Ø 为内核设置启动参数
Ø 调用内核
U-Boot启动第二阶段流程如下所示：
![](https://img-blog.csdn.net/20170113122800647?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图2.4 U-boot启动第二阶段流程图



