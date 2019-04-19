# U-Boot简介 - xqhrs232的专栏 - CSDN博客
2011年07月25日 17:20:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：632标签：[嵌入式操作系统																[flash																[linux内核																[嵌入式																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=嵌入式&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)](https://so.csdn.net/so/search/s.do?q=flash&t=blog)](https://so.csdn.net/so/search/s.do?q=嵌入式操作系统&t=blog)
个人分类：[Bootloader/Eboot/Uboot](https://blog.csdn.net/xqhrs232/article/category/6159968)
原文地址::[http://www.unixreference.net/articles/linux/embedded/bootloader/u-boot/2007/0125/558.html](http://www.unixreference.net/articles/linux/embedded/bootloader/u-boot/2007/0125/558.html)
相关文章::U-Boot 简介，特点，主要功能 的概述
[http://hi.baidu.com/%D7%F4x%D6%FA/blog/item/1a008b660d2ab3cae7113a99.html](http://hi.baidu.com/%D7%F4x%D6%FA/blog/item/1a008b660d2ab3cae7113a99.html)
pdf资料::UBOOT功能介绍.pdf
**1 U-Boot简介**
　　U-Boot，全称Universal Boot Loader，是遵循GPL条款的开放源码项目。从FADSROM、8xxROM、PPCBOOT逐步发展演化而来。其源码目录、编译形式与Linux内核很相似，事实上，不少U-Boot源码就是相应的Linux内核源程序的简化，尤其是一些设备的驱动程序，这从U-Boot源码的注释中能体现这一点。但是U-Boot不仅仅支持嵌入式Linux系统的引导，当前，它还支持NetBSD, VxWorks, QNX, RTEMS, ARTOS, LynxOS嵌入式操作系统。其目前要支持的目标操作系统是OpenBSD,
 NetBSD, FreeBSD,4.4BSD, Linux, SVR4, Esix, Solaris, Irix, SCO, Dell, NCR, VxWorks, LynxOS, pSOS, QNX, RTEMS, ARTOS。这是U-Boot中Universal的一层含义，另外一层含义则是U-Boot除了支持PowerPC系列的处理器外，还能支持MIPS、x86、ARM、NIOS、XScale等诸多常用系列的处理器。这两个特点正是U-Boot项目的开发目标，即支持尽可能多的嵌入式处理器和嵌入式操作系统。就目前来看，U-Boot对PowerPC系列处理器支持最为丰富，对Linux的支持最完善。其它系列的处理器和操作系统基本是在2002年11月PPCBOOT改名为U-Boot后逐步扩充的。从PPCBOOT向U-Boot的顺利过渡，很大程度上归功于U-Boot的维护人德国DENX软件工程中心Wolfgang
 Denk[以下简称W.D]本人精湛专业水平和持着不懈的努力。当前，U-Boot项目正在他的领军之下，众多有志于开放源码BOOT LOADER移植工作的嵌入式开发人员正如火如荼地将各个不同系列嵌入式处理器的移植工作不断展开和深入，以支持更多的嵌入式操作系统的装载与引导。
　　选择U-Boot的理由：
　　① 开放源码；
　　② 支持多种嵌入式操作系统内核，如Linux、NetBSD, VxWorks, QNX, RTEMS, ARTOS, LynxOS；
　　③ 支持多个处理器系列，如PowerPC、ARM、x86、MIPS、XScale；
　　④ 较高的可靠性和稳定性；
　　④ 较高的可靠性和稳定性；
　　⑤ 高度灵活的功能设置，适合U-Boot调试、操作系统不同引导要求、产品发布等；
　　⑥ 丰富的设备驱动源码，如串口、以太网、SDRAM、FLASH、LCD、NVRAM、EEPROM、RTC、键盘等；
　　⑦ 较为丰富的开发调试文档与强大的网络技术支持；
**2 U-Boot主要目录结构**
- board 目标板相关文件，主要包含SDRAM、FLASH驱动； 
- common 独立于处理器体系结构的通用代码，如内存大小探测与故障检测； 
- cpu 与处理器相关的文件。如mpc8xx子目录下含串口、网口、LCD驱动及中断初始化等文件； 
- driver 通用设备驱动，如CFI FLASH驱动(目前对INTEL FLASH支持较好) 
- doc U-Boot的说明文档； 
- examples可在U-Boot下运行的示例程序；如hello_world.c,timer.c； 
- include U-Boot头文件；尤其configs子目录下与目标板相关的配置头文件是移植过程中经常要修改的文件； 
- lib_xxx 处理器体系相关的文件，如lib_ppc, lib_arm目录分别包含与PowerPC、ARM体系结构相关的文件； 
- net 与网络功能相关的文件目录，如bootp,nfs,tftp； 
- post 上电自检文件目录。尚有待于进一步完善； 
- rtc RTC驱动程序； 
- tools 用于创建U-Boot S-RECORD和BIN镜像文件的工具； 
**3 U-Boot支持的主要功能**
　　U-Boot可支持的主要功能列表
- 系统引导 支持NFS挂载、RAMDISK(压缩或非压缩)形式的根文件系统 
- 支持NFS挂载、从FLASH中引导压缩或非压缩系统内核； 
- 基本辅助功能 强大的操作系统接口功能；可灵活设置、传递多个关键参数给操作系统，适合系统在不同开发阶段的调试要求与产品发布，尤 
- Linux支持最为强劲； 
- 支持目标板环境参数多种存储方式，如FLASH、NVRAM、EEPROM； 
- CRC32校验，可校验FLASH中内核、RAMDISK镜像文件是否完好； 
- 设备驱动 串口、SDRAM、FLASH、以太网、LCD、NVRAM、EEPROM、键盘、USB、PCMCIA、PCI、RTC等驱动支持； 
- 上电自检功能 SDRAM、FLASH大小自动检测；SDRAM故障检测；CPU型号； 
- 特殊功能 XIP内核引导； 
