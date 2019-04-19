# BSD：Berkeley Software Distribution，伯克利软件套件 - Koma Hub - CSDN博客
2019年01月19日 21:44:28[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：68
个人分类：[基础知识																[Just For Fun](https://blog.csdn.net/Rong_Toa/article/category/7589943)](https://blog.csdn.net/Rong_Toa/article/category/7587699)
**目录**
[BSD简介](#BSD%E7%AE%80%E4%BB%8B)
[许可协议](#%E8%AE%B8%E5%8F%AF%E5%8D%8F%E8%AE%AE)
[开源项目](#%E5%BC%80%E6%BA%90%E9%A1%B9%E7%9B%AE)
[BSD分区概述](#BSD%E5%88%86%E5%8C%BA%E6%A6%82%E8%BF%B0)
![](https://img-blog.csdnimg.cn/20190119214352514.jpg)
BSD ([Berkeley](https://baike.baidu.com/item/Berkeley)[Software](https://baike.baidu.com/item/Software)[Distribution](https://baike.baidu.com/item/Distribution)，[伯克利](https://baike.baidu.com/item/%E4%BC%AF%E5%85%8B%E5%88%A9/25375)软件套件)是Unix的衍生系统，在1977至1995年间由[加州大学伯克利分校](https://baike.baidu.com/item/%E5%8A%A0%E5%B7%9E%E5%A4%A7%E5%AD%A6%E4%BC%AF%E5%85%8B%E5%88%A9%E5%88%86%E6%A0%A1/3755024)开发和发布的。历史上， BSD曾经被认为是[UNIX](https://baike.baidu.com/item/UNIX)的一支——"BSD UNIX", 因为它和[AT&T](https://baike.baidu.com/item/AT%26T) UNIX操作系统共享基础代码和设计。在20世纪80年代，衍生出了许多变形的UNIX授权软件。比较著名的如[DEC](https://baike.baidu.com/item/DEC)的Ultrix及Sun公司的[SunOS](https://baike.baidu.com/item/SunOS)。1990年代，BSD很大程度上被System V4.x版以及OSF/1系统所取代，晚期BSD版本为几个开源软件开发提供了平台并且一直沿用至今。今天，“BSD”并不特指任何一个BSD衍生版本，而是类UNIX操作系统中的一个分支的总称。
## BSD简介
Berkeley Software Distribution (BSD，也称Berkeley Unix)
[威廉·纳尔逊·乔伊](https://baike.baidu.com/item/%E5%A8%81%E5%BB%89%C2%B7%E7%BA%B3%E5%B0%94%E9%80%8A%C2%B7%E4%B9%94%E4%BC%8A)（[Bill Joy](https://baike.baidu.com/item/Bill%20Joy)）于1979年在[加州大学伯克利分校](https://baike.baidu.com/item/%E5%8A%A0%E5%B7%9E%E5%A4%A7%E5%AD%A6%E4%BC%AF%E5%85%8B%E5%88%A9%E5%88%86%E6%A0%A1)获得电气工程与计算机科学硕士学位 [1] 。1986年，乔伊因他在BSD操作系统中所做的工作获得了[*Grace Murray Hopper*](https://baike.baidu.com/item/Grace%20Murray%20Hopper)奖。1986年6月，BSD 4.3发布。该版本主要是将BSD 4.2的许多新贡献作性能上的提高，原来的BSD 4.1没有很好地协调。在该版本之前，BSD的[TCP/IP](https://baike.baidu.com/item/TCP%2FIP)实现已经跟BBN的官方实现有较大差异。经过数月测试后，DARPA认为BSD 4.2更合适，所以在BSD 4.3中作了保留。
从UNIX简史图（下）可以看出BSD UNIX最终版本即BSD 4.3,它是混合开源的，即有开源部分也有闭源部分。由BSD UNIX衍生出最初的SCO Xenix（[Microsoft](https://baike.baidu.com/item/Microsoft)及[SCO](https://baike.baidu.com/item/SCO)共同开发）、Sun OS（[*Sun*](https://baike.baidu.com/item/Sun)MicroSystems开发，即[Solaris](https://baike.baidu.com/item/Solaris)前身）、[NEXTSTEP](https://baike.baidu.com/item/NEXTSTEP)（NeXT开发）等[UNIX](https://baike.baidu.com/item/UNIX)版本，这些UNIX版本均[闭源](https://baike.baidu.com/item/%E9%97%AD%E6%BA%90)且受专利保护。
![](https://img-blog.csdnimg.cn/20190119214155946.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
BSD UNIX对现代操作系统的产生与发展产生了巨大的影响，并将[开源](https://baike.baidu.com/item/%E5%BC%80%E6%BA%90)闭源的争议摆在了人们面前。拥护闭源专利的代表[Bill Gates](https://baike.baidu.com/item/Bill%20Gates)领导创建了[Windows](https://baike.baidu.com/item/Windows)等软件获得了巨大成功，并垄断了桌面以及服务器市场，从而极大丰富了软件产业。而捍卫开源共享的代表[Richard Stallman](https://baike.baidu.com/item/Richard%20Stallman)开展[自由软件运动](https://baike.baidu.com/item/%E8%87%AA%E7%94%B1%E8%BD%AF%E4%BB%B6%E8%BF%90%E5%8A%A8)，创建[GNU工程](https://baike.baidu.com/item/GNU%E5%B7%A5%E7%A8%8B),使[自由软件](https://baike.baidu.com/item/%E8%87%AA%E7%94%B1%E8%BD%AF%E4%BB%B6)精神深入人心，并促使了[GNU/Linux](https://baike.baidu.com/item/GNU%2FLinux)等开源操作系统的普及。
**关于BSD 4.3 Tahoe**
BSD 4.3混合了专属的AT&T Unix代码，继续使用就要求从[AT&T](https://baike.baidu.com/item/AT%26T)获得许可证，源码许可证当时非常地昂贵，几个其他组织对单独的网络代码版感兴趣，该部分完全独立于AT&T，这样就可不受许可证的支配。
Computer Consoles公司开发的Power 6/32平台的BSD 4.3 Tahoe，当时看来大有作为，但不久即被他们的开发员所遗弃。1990年初，出现BSD 4.3 Reno，该版本是4.4BSD早期开发的过渡版，使用该版本被戏称为是一种赌博，因为Reno就是[内华达州](https://baike.baidu.com/item/%E5%86%85%E5%8D%8E%E8%BE%BE%E5%B7%9E)的赌城[雷诺](https://baike.baidu.com/item/%E9%9B%B7%E8%AF%BA)。4.4BSD通过一场官司留下了以[BSD许可证](https://baike.baidu.com/item/BSD%E8%AE%B8%E5%8F%AF%E8%AF%81)分发源码的4.4BSD lite。
## 许可协议
[BSD许可证](https://baike.baidu.com/item/BSD%E8%AE%B8%E5%8F%AF%E8%AF%81)是随着加州大学伯克利分校发布BSD UNIX发展起来的，修改版本被[Apple](https://baike.baidu.com/item/Apple)、[Apache](https://baike.baidu.com/item/Apache)所采用。
BSD协议是“宽容自由软件许可证”中的一员，在软件复用上给予了最小限度的限制。BSD协议允许作者使用该协议下的资源，将其并入私人版本的软件，该软件可使用闭源软件协议发布。
## 开源项目
[Darwin](https://baike.baidu.com/item/Darwin)
吉祥物：[Hexley](https://baike.baidu.com/item/Hexley)
NeXT电脑公司开发的用于[NEXTSTEP](https://baike.baidu.com/item/NEXTSTEP)的XNU内核是兼有[Mach](https://baike.baidu.com/item/Mach)3[微内核](https://baike.baidu.com/item/%E5%BE%AE%E5%86%85%E6%A0%B8)和大量来自BSD[宏内核](https://baike.baidu.com/item/%E5%AE%8F%E5%86%85%E6%A0%B8)的元素（进程、网络、虚拟文件系统）以及I/O Kit的[混合内核](https://baike.baidu.com/item/%E6%B7%B7%E5%90%88%E5%86%85%E6%A0%B8)，在*Apple Public Source License 2.0*许可协议下发布。
1989，NEXTSTEP（即[OPENSTEP](https://baike.baidu.com/item/OPENSTEP)的前身）首次发布，最终版本3.3在1995年发布。
1997年OPENSTEP（即Darwin的前身）发布，2000年[Mac OS X](https://baike.baidu.com/item/Mac%20OS%20X)以及[iOS](https://baike.baidu.com/item/iOS)的基础Darwin发布，并以*Apple Public Source License**2.0*发布[源代码](https://baike.baidu.com/item/%E6%BA%90%E4%BB%A3%E7%A0%81)。
## BSD分区概述
大多数的BSD系统使用基于32位间接寻址（IA32）的硬件平台（x86/i386）可以与Microsoft产品共存于一个磁盘上。这时会在磁盘上为其建立一个BSD类型的DOS分区，并在主分区表中对其进行描述，然后在这个BSD类型的DOS分区再按照BSD的分区机制建立相应的区域。
BSD分区体系比 DOS分区体系简单但相对于Apple的分区复杂些。BSD分区被包含在一个由DOS创建的分区中，且只用一个扇区记录必要的分区信息。这样在同一个磁盘上就可以同时安装windows和BSD操作系统，并且当操作系统启动时可以 程序一个启动 选择窗口以供用户选择要启动的操作系统。
参考资料
[Bill Joy](https://baike.baidu.com/redirect/28c6lccAsTgD4s0AbB3ED1ivpFaE_KVwpbnzJlWGmHDNtHmRvQDf9wQOMyFlXB9bunkr7lXTCjK2qhawGROjx2JUDTciki5TMVxatVQBLw0hK2DlbRA) ．加州大学伯克利分校官网[引用日期2016-11-29]
以上内容来自百度百科。
