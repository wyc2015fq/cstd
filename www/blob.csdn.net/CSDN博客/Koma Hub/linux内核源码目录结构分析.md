# linux内核源码目录结构分析 - Koma Hub - CSDN博客
2019年02月24日 11:12:09[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：28
个人分类：[Linux kernel](https://blog.csdn.net/Rong_Toa/article/category/7348890)
[https://blog.csdn.net/ultraman_hs/article/details/52931121](https://blog.csdn.net/ultraman_hs/article/details/52931121)
> 
注：本文是学习朱老师课程整理的笔记，基于linux2.6.35.7和九鼎X210BV3S开发板进行移植。
**Table of Contents**
[/arch](#%2Farch)
[/block](#%2Fblock)
[/crypto](#%2Fcrypto)
[/Documentation](#%2FDocumentation)
[/drivers](#%2Fdrivers)
[/firmware](#%2Ffirmware)
[/fs](#%2Ffs)
[/include](#%2Finclude)
[/init](#%2Finit)
[/ipc](#%2Fipc)
[/kernel](#%2Fkernel)
[/lib](#%2Flib)
[/mm](#%2Fmm)
[/net](#%2Fnet)
[/scripts](#%2Fscripts)
[/security](#%2Fsecurity)
[/sound](#%2Fsound)
[/tools](#%2Ftools)
[/usr](#%2Fusr)
[/virt](#%2Fvirt)
### **/arch**
arch是architecture的缩写。arch目录下是好多个不同架构的CPU的子目录，譬如arm这种cpu的所有文件都在arch/arm目录下，X86的CPU的所有文件都在arch/x86目录下。
### /block
是块的意思，在linux中block表示块设备，块设备是以数据块方式接收和发送的数据的设备。譬如说SD卡、iNand、Nand、硬盘等都是块设备。几乎可以认为块设备就是存储设备。block目录下放的是一些linux存储体系中关于块设备管理的代码。
### /crypto
是加密的意思。这个目录下放了内核本身所用的加密API，实现了常用的加密和散列算法，还有一些压缩和CRC校验算法。
### /Documentation
里面放了一些帮助文档。
### /drivers
驱动目录，这个目录是内核中最庞大的一个目录，里面分门别类的列出了linux内核支持的所有硬件设备的驱动源代码。
### /firmware
固件。什么是固件？固件其实是软件，不过这个软件是固化到IC里面运行的，为了了让计算机读取和理解从设备发来的信号的代码，就像S5PV210里的iROM代码。
### /fs
fs就是file system，文件系统，里面列出了linux支持的各种文件系统的实现。
### /include
头文件目录，公共的（各种CPU架构共用的）头文件都在这里。每种CPU架构特有的一些头文件在arch/***/include目录及其子目录下。
### /init
init是初始化的意思，这个目录下的代码就是linux内核启动时初始化内核的代码。
### /ipc
ipc就是inter process commuication，进程间通信，里面都是linux支持的IPC的代码实现。
### /kernel
kernel就是内核，内核中最核心的部分，包括进程的调度（sched.c）,以及进程的创建和撤销（fork.c和exit.c）和平台相关的另外一部分核心代码在arch/***/kernel目录下。
### /lib
lib是库的意思，这里面都是一些公用的库函数，注意这里的库函数和C语言的库函数不一样的。在内核编程中是不能用C语言标准库函数，这里的lib目录下的库函数就是用来替代那些标准库函数的。譬如要把字符串转成数字用atoi，内核编程中只能用lib目录下的atoi函数，不能用标准C语言库中的atoi。譬如在内核中要打印信息时不能用printf，而要用printk，这个printk就是我们这个lib目录下的。
### /mm
mm是memory management，内存管理，此目录包含了与体系无关的部分内存管理代码。与体系结构相关的内存管理代码位于arch/***/mm下。
### /net
该目录下是网络相关的代码，譬如TCP/IP协议栈等都在这里。
### /scripts
脚本，这个目录下全部是脚本文件，这些脚本文件不是linux内核工作时使用的，而是用来辅助对linux内核进行配置编译生产的。当运行make menuconfig或者make xconfig之类的命令配置内核时，用户就是和位于这个目录下的脚本进行交互的。
### /security
安全相关的代码。对计算机免于受到病毒和黑客的侵害很重要.
### /sound
音频处理相关的。
### /tools
linux中用到的一些有用工具
### /usr
实现用于打包和压缩的cpio等。这个文件夹中的代码在内核编译完成后创建这些文件。
### /virt
此文件夹包含了虚拟化代码，它允许用户一次运行多个操作系统。通过虚拟化，客户机操作系统就像任何其他运行在Linux主机的应用程序一样运行。
根目录下的一些单独文件：
COPYING：许可和授权信息。Linux内核在GPLv2许可证下授权。该许可证授予任何人有权免费去使用、修改、分发和共享源代码和编译代码。然而，没有人可以出售源代码。
CREDITS ： 贡献者列表。
Kbuild：是kernel build的意思，就是内核编译的意思，设置一些内核设定的脚本。打个比方，这个脚本设定一个ARCH变量，这是开发者想要生成的内核支持的处理器类型。
Kconfig：这个脚本会在开发人员配置内核的时候用到。
MAINTAINERS：这是一个目前维护者列表，他们的电子邮件地址，主页，和他们负责开发和维护的内核的特定部分或文件。当一个开发者在内核中发现一个问题，并希望能够报告给能够处理这个问题的维护者时，这是是很有用的。
Makefile：这个脚本是编译内核的主要文件。这个文件将编译参数和编译所需的文件和必要的信息传给编译器。
README：这个文档提供给开发者想要知道的如何编译内核的信息。
REPORTING-BUGS：这个文档提供如何报告问题的信息。
总结：这么多目录跟我们关系很紧密的就是arch和drivers目录，然后其他有点相关的还有include、block、mm、net、lib等目录。根目录下的单文件比较重要的是Kbuild，Kconfig，Makefile
--------------------- 
作者：Ultraman_hs 
来源：CSDN 
原文：https://blog.csdn.net/ultraman_hs/article/details/52931121 
版权声明：本文为博主原创文章，转载请附上博文链接！
