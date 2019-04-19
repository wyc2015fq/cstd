# Bootloader及u-boot简介/u-boot系统启动流程 - maopig的专栏 - CSDN博客
2011年11月08日 14:30:41[maopig](https://me.csdn.net/maopig)阅读数：1382标签：[语言																[工作																[嵌入式																[汇编																[flash																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=flash&t=blog)](https://so.csdn.net/so/search/s.do?q=汇编&t=blog)](https://so.csdn.net/so/search/s.do?q=嵌入式&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)
个人分类：[Linux](https://blog.csdn.net/maopig/article/category/850267)
[Bootloader及u-boot简介](http://buaadallas.blog.51cto.com/399160/80923)
Bootloader代码是芯片复位后进入操作系统之前执行的一段代码，主要用于完成由硬件启动到操作系统启动的过渡，从而为操作系统提供基本的运行环境，如初始化CPU、堆栈、存储器系统等。Bootloader 代码与CPU 芯片的内核结构、具体型号、应用系统的配置及使用的操作系统等因素有关，其功能类似于PC机的BIOS程序。由于Bootloader和CPU及电路板的配置情况有关，因此不可能有通用的bootloader ，开发时需要用户根据具体情况进行移植。嵌入式Linux系统中常用的bootloader有armboot、redboot、blob、u-boot等，其中u-boot是当前比较流行，功能比较强大的bootloader，可以支持多种体系结构，但相对也比较复杂。Bootloader的实现依赖于CPU的体系结构，大多数bootloader都分为stage
 1和stage2两大部分。Bootloader的基本原理见参考文献。u-boot是sourceforge网站上的一个开放源代码的项目。它可对 PowerPC MPC5XX、MPC8XX、MPC82XX、 MPC7XX、MPC74XX、ARM（ARM7、ARM9、StrongARM、VxWorks、NetBSD、QNX、RTEMS、ARTOS、LynxOS等，主要用来开发嵌入式系统初始化代码bootloader。软件的主站点是[[url]http://sourceforge.net/projects/u-boot[/url]](http://sourceforge.net/projects/u-boot)。u-boot
 最初是由denx的PPC-boot发展而来的，它对PowerPC系列处理器的支持最完善，对Linux操作系统的支持最好。源代码开放的U-boot软件项目经常更新。
u-boot系统启动流程 
大多数bootloader都分为stage1和stage2两大部分，u-boot也不例外。依赖于CPU体系结构的代码（如设备初始化代码等）通常都放在stage1且可以用汇编语言来实现，而stage2则通常用Ｃ语言来实现，这样可以实现复杂的功能，而且有更好的可读性和移植性。
**1 stage1 (start.s代码结构)**
u-boot的stage1代码通常放在start.s文件中，它用汇编语言写成，其主要代码部分如下：
（1）定义入口。由于一个可执行的Image必须有一个入口点，并且只能有一个全局入口，通常这个入口放在ROM(Flash)的0x0地址，因此，必须通知编译器以使其知道这个入口，该工作可通过修改连接器脚本来完成。
（2）设置异常向量。
（3）设置CPU的速度、时钟频率及中断控制寄存器。
（4）初始化内存控制器 。
（5）将ROM中的程序复制到RAM中。
（6）初始化堆栈 。
（7）转到RAM中执行，该工作可使用指令ldr pc来完成。
**2 stage2 C语言代码部分**
lib_arm/board.c中的start_armboot是C语言开始的函数，也是整个启动代码中Ｃ语言的主函数，同时还是整个uboot（armboot）的主函数，该函数主要完成如下操作：
（1）调用一系列的初始化函数。
（2）初始化Flash设备。
（3）初始化系统内存分配函数。
（4）如果目标系统拥有NAND设备，则初始化NAND设备。
（5）如果目标系统有显示设备，则初始化该类设备。
（6）初始化相关网络设备，填写IP、MAC地址等。
（7）进入命令循环（即整个boot的工作循环），接受用户从串口输入的命令，然后进行相应的工作。
