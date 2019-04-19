# 嵌入式Linux系统基础知识 - maopig的专栏 - CSDN博客
2011年11月10日 00:00:50[maopig](https://me.csdn.net/maopig)阅读数：5187
一、嵌入式Linux系统的构成
1、硬件
2、内核
3、应用程序（形成根文件系统）
二、构建嵌入式Linux系统的主要任务
1、内核部分
2、应用程序部分
嵌入式Linux的开发大致可分为三个层次：引导装载内核、构造文件系统和图形用户界面。作为操作系统重要组成部分的文件系统，决定了操作系统本身的信息和用户的数据在存储设备上的组织形式。对嵌入式文件系统的研究、设计和开发也逐渐成为嵌入式系 统研究领域的一个方向。
三、内核精简
在精简内核在编译内核之前，首先要明确需要那些驱动和模块，然后只选择需要的驱动和模块，
例如，如果系统不需要网络支持，则可以去掉网络模块 。
内核一般是以压缩方式存放的，在系统启动时会自行解压。
内核都是常驻内存的，当需要调用应用程序时，再把需要的程序从磁盘调入内存运行。
构建嵌入式Linux系统－构建内核常用的命令包括：
　　◆ make config：内核配置，调用 ./scripts/Configure 按照 arch/i386/config.in 来进行配置。 
　　◆ make dep：寻找依赖关系。 
　　◆ make clean：清除以前构建内核所产生的所有目标文件、模块文件、以及一些临时文件等。 
　　◆ make rmproper：删除所有因构建内核过程中产生的所有文件，把内核恢复到最原始的状态。 
　　◆ make：构核，通过各目录的Makefile 文件将会在各个目录下产生许多目标文件。如果内核没有错误，将产生文件vmlinux，这就是构建的内核。 
　　◆ make zImage：在make 的基础上产生压缩的内核映象文件./arch/$(ARCH)/boot/zImage 以及在 ./arch/$(ARCH)/boot/compresed/目录下产生临时文件。
　　◆ make bzImage：在make 的基础上产生压缩比例更大的内核映象文件./arch/$(ARCH)/boot/bzImage 以及在 ./arch/$(ARCH)/boot/compresed/目录下产生临时文件。
　　◆ make modules：编译模块文件，在make config 时所配置的所有模块将在这时编译，形成模块目标文件，并把这些目标文件存放在modules 目录中。
　　◆ make modules_install：把上面编译好的模块目标文件放置在目录 ./lib/modules/$KERNEL_VERSION/ 中。上面的编译内核是在没有改变源代码的情况下实现的，如果觉得源代码提供的功能在某些方面不能满足要求，就要修改源代码了。源代码中主要有以下几个关键部分：有关进程管理的task_struct 结构，这个结构几乎包括了与进程有关的所有文件内容，还有任务队列、时钟管理和中断管理，各种进程间的通信机制，内存管理中各种内存分配函数的实现，虚拟文件系统。
四、嵌入式系统的组成
  1、嵌入式硬件（嵌入式处理器和嵌入式外围设备）
  2、嵌入式操作系统
  3、嵌入式应用软件
  嵌入式处理器 
嵌入式系统的核心是各种类型的嵌入式处理器，嵌入式处理器与通用处理器最大的不同点在于，嵌入式CPU大多工作在为特定用户群所专门设计的系统中，它将通用CPU中许多由板卡完成的任务集成到芯片内部，从而有利于嵌入式系统在设计时趋于小型化，同时还具有很高的效率和可靠性。
嵌入式处理器的体系结构经历了从CISC（复杂指令集）至RISC（精简指令集）和Compact RISC的转变，位数则由4位、8位、16位、32位逐步发展到64位。目前常用的嵌入式处理器可分为低端的嵌入式微控制器（Micro Controller Unit，MCU）、中高端的嵌入式微处理器（Embedded Micro Processor Unit，EMPU）、用于计算机通信领域的嵌入式DSP处理器（Embedded Digital Signal Processor，EDSP）和高度集成的嵌入式片上系统（System
 On Chip，SOC）。
目前几乎每个半导体制造商都生产嵌入式处理器，并且越来越多的公司开始拥有自主的处理器设计部门，据不完全统计，全世界嵌入式处理器已经超过1000多种，流行的体系结构有30多个系列，其中以ARM、PowerPC、MC 68000、MIPS等使用得最为广泛。
嵌入式外围设备
在嵌入系统硬件系统中，除了中心控制部件（MCU、DSP、EMPU、SOC）以外，用于完成存储、通信、调试、显示等辅助功能的其他部件，事实上都可以算作嵌入式外围设备。目前常用的嵌入式外围设备按功能可以分为存储设备、通信设备和显示设备三类。
存储设备主要用于各类数据的存储，常用的有静态易失型存储器（RAM、SRAM）、动态存储器（DRAM）和非易失型存储器（ROM、EPROM、EEPROM、FLASH）三种，其中FLASH凭借其可擦写次数多、存储速度快、存储容量大、价格便宜等优点，在嵌入式领域内得到了广泛应用。
目前存在的绝大多数通信设备都可以直接在嵌入式系统中应用，包括RS-232接口（串行通信接口）、SPI（串行外围设备接口）、IrDA（红外线接口）、I2C（现场总线）、USB（通用串行总线接口）、Ethernet（以太网接口）等。
由于嵌入式应用场合的特殊性，通常使用的是阴极射线管（CRT）、液晶显示器（LCD）和触摸板（Touch Panel）等外围显示设备。
嵌入式操作系统 
为了使嵌入式系统的开发更加方便和快捷，需要有专门负责管理存储器分配、中断处理、任务调度等功能的软件模块，这就是嵌入式操作系统。嵌入式操作系统是用来支持嵌入式应用的系统软件，是嵌入式系统极为重要的组成部分，通常包括与硬件相关的底层驱动程序、系统内核、设备驱动接口、通信协议、图形用户界面（GUI）等。嵌入式操作系统具有通用操作系统的基本特点，如能够有效管理复杂的系统资源，能够对硬件进行抽象，能够提供库函数、驱动程序、开发工具集等。但与通用操作系统相比较，嵌入式操作系统在系统实时性、硬件依赖性、软件固化性以及应用专用性等方面，具有更加鲜明的特点。
嵌入式操作系统根据应用场合可以分为两大类：一类是面向消费电子产品的非实时系统，这类设备包括个人数字助理（PDA）、移动电话、机顶盒（STB）等；另一类则是面向控制、通信、医疗等领域的实时操作系统，如WindRiver公司的VxWorks、QNX系统软件公司的QNX等。实时系统（Real Time System）是一种能够在指定或者确定时间内完成系统功能，并且对外部和内部事件在同步或者异步时间内能做出及时响应的系统。在实时系统中，操作的正确性不仅依赖于逻辑设计的正确程度，而且与这些操作进行的时间有关，也就是说，实时系统对逻辑和时序的要求非常严格，如果逻辑和时序控制出现偏差将会产生严重后果。
实时系统主要通过三个性能指标来衡量系统的实时性，即响应时间（Response Time）、生存时间（Survival Time）和吞吐量（Throughput）：
响应时间　　是实时系统从识别出一个外部事件到做出响应的时间； 
生存时间　是数据的有效等待时间，数据只有在这段时间内才是有效的； 
吞吐量　　　是在给定的时间内系统能够处理的事件总数，吞吐量通常比平均响应时间的倒数要小一点。 
实时系统根据响应时间可以分为弱实时系统、一般实时系统和强实时系统三种。弱实时系统在设计时的宗旨是使各个任务运行得越快越好，但没有严格限定某一任务必须在多长时间内完成，弱实时系统更多关注的是程序运行结果的正确与否，以及系统安全性能等其他方面，对任务执行时间的要求相对来讲较为宽松，一般响应时间可以是数十秒或者更长。一般实时系统是弱实时系统和强实时系统的一种折衷，它的响应时间可以在秒的数量级上，广泛应用于消费电子设备中。强实时系统则要求各个任务不仅要保证执行过程和结果的正确性，同时还要保证在限定的时间内完成任务，响应时间通常要求在毫秒甚至微秒的数量级上，这对涉及到医疗、安全、军事的软硬件系统来说是至关重要的。
时限（deadline）是实时系统中的一个重要概念，指的是对任务截止时间的要求，根据时限对系统性能的影响程度，实时系统又可以分为软实时系统（soft real-time-system）和硬实时系统（hard real-time-system）。软实时指的是虽然对系统响应时间有所限定，但如果系统响应时间不能满足要求，并不会导致系统产生致命的错误或者崩溃；硬实时则指的是对系统响应时间有严格的限定，如果系统响应时间不能满足要求，就会引起系统产生致命的错误或者崩溃。如果一个任务在时限到达之时尚未完成，对软实时系统来说还是可以容忍的，最多只会降低系统性能，但对硬实时系统来说则是无法接受的，因为这样带来的后果根本无法预测，甚至可能是灾难性的。在目前实际运用的实时系统中，通常允许软硬两种实时性同时存在，其中一些事件没有时限要求，另外一些事件的时限要求是软实时的，而对系统产生关键影响的那些事件的时限要求则是硬实时的。
嵌入式应用软件 
嵌入式应用软件是针对特定应用领域，基于某一固定的硬件平台，用来达到用户预期目标的计算机软件，由于用户任务可能有时间和精度上的要求，因此有些嵌入式应用软件需要特定嵌入式操作系统的支持。嵌入式应用软件和普通应用软件有一定的区别，它不仅要求其准确性、安全性和稳定性等方面能够满足实际应用的需要，而且还要尽可能地进行优化，以减少对系统资源的消耗，降低硬件成本
四、如何构建嵌入式Linux系统的设计步骤
 嵌入式系统目前主要有：Windows CE、VxWorks、QNX等，它们都具较好的实时性，系统可靠性，任务处理随机性等优点。但是它们的价格普遍偏高，很多开发商承受不起。因而，Linux操作系统成为嵌入式操作系统的首选。以下小编来整理了一些关于如何构建嵌入式Linux系统的资料，方便大家对构建嵌入式Linux系统有一定的认识，希望能给大家有所帮助。
Linux操作系统成为嵌入式操作系统首选的原因
　　在精简内核在编译内核之前，首先要明确需要那些驱动和模块，然后只选择需要的驱动和模块，例如，如果系统不需要网络支持，则可以去掉网络模块 。内核一般是以压缩方式存放的，在系统启动时会自行解压。内核都是常驻内存的，当需要调用应用程序时，再把需要的程序从磁盘调入内存运行。
构建嵌入式Linux系统－构建内核常用的命令包括：
　　◆ make config：内核配置，调用 ./scripts/Configure 按照 arch/i386/config.in 来进行配置。 
　　◆ make dep：寻找依赖关系。 
　　◆ make clean：清除以前构建内核所产生的所有目标文件、模块文件、以及一些临时文件等。 
　　◆ make rmproper：删除所有因构建内核过程中产生的所有文件，把内核恢复到最原始的状态。 
　　◆ make：构核，通过各目录的Makefile 文件将会在各个目录下产生许多目标文件。如果内核没有错误，将产生文件vmlinux，这就是构建的内核。 
　　◆ make zImage：在make 的基础上产生压缩的内核映象文件./arch/$(ARCH)/boot/zImage 以及在 ./arch/$(ARCH)/boot/compresed/目录下产生临时文件。
　　◆ make bzImage：在make 的基础上产生压缩比例更大的内核映象文件./arch/$(ARCH)/boot/bzImage 以及在 ./arch/$(ARCH)/boot/compresed/目录下产生临时文件。
　　◆ make modules：编译模块文件，在make config 时所配置的所有模块将在这时编译，形成模块目标文件，并把这些目标文件存放在modules 目录中。
　　◆ make modules_install：把上面编译好的模块目标文件放置在目录 ./lib/modules/$KERNEL_VERSION/ 中。上面的编译内核是在没有改变源代码的情况下实现的，如果觉得源代码提供的功能在某些方面不能满足要求，就要修改源代码了。源代码中主要有以下几个关键部分：有关进程管理的task_struct 结构，这个结构几乎包括了与进程有关的所有文件内容，还有任务队列、时钟管理和中断管理，各种进程间的通信机制，内存管理中各种内存分配函数的实现，虚拟文件系统。
　　下面来论述了以Linux作为操作系统平台的优势，分析了以PC104卡和CompactFlash卡构造带图形用户界面(GUI)的Linux系统的关键技术. (构建嵌入式Linux系统)
1） 硬件及软件环境
　　Linux系统的硬件环境以PC104卡为核心，卡上集成美国国家半导体公司（NS Geode）GX1 300MHz CPU,SVGA/LCD/LAN接口以及增强型IDE硬盘接口，支持CompactFlash卡。硬盘和光驱接至PC104的IDE1口，设置跳线使硬盘为主设备（master），光驱为从设备（slave），CompactFlash装在IDE2口，设为主设备，这样Linux识别 CompactFlah卡为/dev/hdc,而硬盘和光驱分别为/dev/had,/dev/hdb.我的Linux开发系统是Red
 Hat 7.2版，通过PC104上的光驱安装在硬盘上。
　　CompactFlash卡容量为32M,如果系统的BIOS支持自动检测，应该可以直接探测到它。如果不行的话，需要自己输入卡的各项参数。确保不要带电插拔CompactFlash卡，因为这可能导致卡上数据的丢失。由于像Red Hat或Mandrake这样的Linux分发版体积庞大（如果带X-Windows基本都在1G以上），是不可能装在容量只有32M的CompactFlash卡上的。目前有许多小型的嵌入式Linux版本，比较有名的有：
　　ETLinux — 设计用于在小型工业计算机，尤其是 PC/104 模块上运行的 Linux 的完全分发版。
　　LEM — 运行在 386 上的小型（<8 MB）多用户、网络 Linux 版本。
　　LOAF — "Linux On A Floppy"分发版,运行在 386 上。
　　uClinux — 在没有 MMU 的系统上运行的 Linux。目前支持 Motorola 68K、MCF5206 和 MCF5207 ColdFire 微处理器。
　　出于深入了解Linux系统运行环境和机制的想法，建议自己构建Linux系统，这对于将来系统内核版本的升级和应用功能的拓展很有好处。选择Red Hat 7.2的Linux版本。
2) 系统分区和格式化
　　首先，需要在CompactFlash卡上建立分区和格式化，在Linux下用fdisk命令可以在CompactFlash卡上创建分区，命令格式为fdisk /dev/hdc,然后用mke2fs命令创建ext2文件系统.
系统分区和格式化　
3)构建嵌入式Linux系统－编译内核
　　接下来要做的事情是编译一个自己的内核。内核负责管理系统的进程、内存、设备驱动程序、文件和网络系统，决定着系统的性能和稳定性。从技术上而言， Linux 是一个内核，而且只是一个内核。其余我们通常称之为 "Linux" (例如一个 shell 和编译器)的部分实质上只是整个软件包的一部分，它们从技术上而言是与 Linux (内核)分开的。选择与开发系统一样的内核版本2.4.7，在Linux内核版本发布的官方网站上可以找到内核源代码。在编译内核的时候，由于内核是在特定的机器上使用的，所以应该对机器的情况了如指掌。
　　另一方面，为了降低过高的复杂性，可以不用kernel module支持，把所有需要的东西直接编译到内核里。IDE支持是必不可少的，因为Linux认CompactFlash卡为IDE硬盘，Frame buffer的支持也是必要的，这来源于GUI系统—MiniGUI的要求，网络支持可以不要，但是如果认为显示屏将来有这方面的要求也可以把它加上。
4) 构建嵌入式Linux系统－创建根文件系统（Root Filesystem）
　　在编译好内核后，需要在CompactFlash卡上创建根文件系统。每台机器都有根文件系统(一般在本地盘中，当然也可以在RAM盘或网络盘中)，它包含系统引导和使其他文件系统得以mount所必要的文件，根文件系统应该有单用户状态所必须的足够的内容。还应该包括修复损坏系统、恢复备份等的工具。
　　如果有一个任务比其它任务更能简化从头构建 Linux 根文件系统过程的话，那就是构建和安装 BusyBox 软件包。BusyBox 是一个可执行文件，它提供许多其它常用命令行工具的功能，所有这些功能都合为一体。BusyBox 的文档声称构建一个有效系统所需要的全部就是 BusyBox 和"/dev、/etc 以及内核"— 而且他们没有开玩笑。
　　Busybox 编译出一个单个的独立执行程序，就叫做 busybox。但是它可以根据配置，执行 ash shell 的功能，以及几十个各种小应用程序的功能。这其中包括有一个迷你的 vi 编辑器，系统不可或缺的 /sbin/init 程序，以及其他诸如 sed, ifconfig, halt, reboot, mkdir, mount, ln, ls, echo, cat ... 等等这些都是一个正常的系统上必不可少的。但是如果我们把这些程序的原件拿过来的话，它们的体积加在一起，让人吃不消。可是
 busybox 有全部的这么多功能，大小也不过 100K 左右。而且，用户还可以根据自己的需要，决定到底要在 busybox 中编译进哪几个应用程序的功能。这样的话，busybox 的体积就可以进一步缩小了。Busybox的具体编译和配置方法请参阅Busybox的官方文档，用户可以在上找到.
　　系统启动后内核调用的第一个进程是/sbin/init。init进程是系统所有进程的起点，内核在完成核内引导以后，即在本线程（进程）空间内加载 init程序，它的进程号是1。init程序需要读取/etc/inittab文件作为其行为指针，inittab是以行为单位的描述性（非执行性）文本，inittab文件至少应该包含下面几行：
　　::sysinit:/etc/init.d/rcS //运行系统公共脚本
　　::respawn:/bin/sh //立即进入shell环境
　　::ctrlaltdel:/bin/umount -a //捕捉Ctrl+Alt+Del快捷键，退出系统
由于init进程第一个执行的脚本文件是/etc/init.d/rcS，编辑rcS文件：
　　#! /bin/sh
　　mount -a
接着还需要编写/etc/fstab文件：
　　proc /proc proc defaults 0 0
　　none /var/shm shm defaults 0 0
5) 安装系统引导工具（boot loader）
　　现在CompactFlask卡上新的内核已经有了，文件系统也已经建好了，接着要做的事情是安装系统引导工具（boot loader）。目前可以选用的引导工具很多，象LILO或GRUB都是挺不错的，用户可以按自己的喜好来选择。在这里我们选用LILO，它在硬盘开始的 MBR写入引导代码，这些代码不经过文件系统，直接从硬盘扇区号读内核映象(kernel image)装入内存。那么要如何安装LILO呢？首先需要根据用户系统的搭建情况编辑/etc/lilo.conf文件，下面是我的
 /etc/lilo.conf文件，配有注释，你可以根据自己的情况进行更改，具体你可以参考LILO mini-HOWTO：
　　boot=/dev/hdc #从CompactFlash卡启动
　　disk=/dev/hdc
　　bios=0x80 #使BIOS认CompactFlash卡为启动盘
　　delay=0 #可选，以秒为单位
　　vga=0x311 #可选，参照Frame buffer的配置
　　image=/boot/vmlinuz-2.4.7 #内核文件位置
==========================================================
五、
## 构建嵌入式linux开发环境和应用程序的编写
(2009-09-09 12:37:09)
![](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)转载
|标签：### [linux](http://uni.sina.com.cn/c.php?t=blog&k=linux&ts=bpost&stype=tag)### [杂谈](http://uni.sina.com.cn/c.php?t=blog&k=%D4%D3%CC%B8&ts=bpost&stype=tag)|分类：[linux学习](http://blog.sina.com.cn/s/articlelist_1317470384_2_1.html)|
构建嵌入式linux开发环境和应用程序的编写
实验步骤:（注意：//号后的的内容为注释）
**一、构建嵌入式Linux开发环境1、linux下网络的设置**
（1）启动linux后，选择系统管理网络。（2）选择设备进行IP地址的设置
(3)选择DNS,进行DNS的设置    (4)最后，激活网络设备
**2、minicom的设置**（1）启动linux后，选择应用程序附件终端，单击右键,选择将该启动器添
加到桌面,单击左键,此时,会在桌面上会出现一个终端的快捷键。
（2）双击桌面上的终端，打开一个终端。（可同时打开多个终端）在命令提示符
下，输入:
#minicom–s
    回车，进行minicom的设置：serila device :/dev/ttyS0
                         lockfile location :/var/lock
                         bps/par/bits :115200 8N1
                         Hardware flow control :no
                         Software Flow control :no
    回车,选择save setup as df1
（3）退出设置界面，可以看到minicom启动的画面
（4）连接好开发板与PC机的串口线，启动开发板，看到有1.2.3.4等等各种选项的话，说明minicom已经设置好了。
**3、安装交叉编译器（在PC机终端进行）**软件包：cross_2.95.3.tar.bz2
(1)在/usr/local下建一个名为arm的目录。
#mkdir/usr/local/arm
(2)将cross-2.95.tar.bz2复制到刚刚新建的目录中。
(3)#cd/usr/local/arm//到/usr/local/arm目录下
(4)解压缩该软件包
#tar-xjvf cross-2.95.tar.bz2
(5)解压后在cross-2.95.tar.bz2的同一目录下出现一个2.95.3的目录，将此
目录下的/bin目录添加到环境变量中去。
#export PATH=/usr/local/arm/2.95.3/bin:$PATH
(6)用此方法添加的环境变量在掉点后会丢失，因此，可以使用在“/etc/bashrc”
的最后一行添加以上命令。
#vi/etc/bashrc 
 然后在最后一行加上export PATH=/usr/local/arm/2.95.3/bin:$PATH
  (7)查看该路径是否已添加到环境变量中。
#echo$PATH
usr/local/arm/2.95.3/bin:/usr/kerberos/sbin:/usr/kerberos/bin:/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/usr/X11R6/bin:/root/bin
**4、安装编绎ARM Linux内核（在PC机终端进行**）
软件包：kernel2410.tgz
(1)在/下建一个名为s3c2410的目录。
#mkdir s3c2410//建立s3c2410文件夹
(2)将kernel2410.tgz复制到刚刚新建的目录中
#cd/s3c2410/            //到s3c2410目录下
#tar   zxvf   kernel2410.tgz       //解压kernel2410.tgz
(3)编译内核。
#cd/s3c2410/2.4.18-rmk7     //到2.4.18-rmk7目录下
#make menuconfig //执行make menuconfig命令,进行内核配置.
（4）进入主界面后，装载默认配置文件kernel_2410.cfg。
（当然，我们可根据自身需要定制内核，具体请参考内核配置.txt。）
选择：Load an Alternate Configuration File  回车
然后填写为：kernel_2410.cfg   选择 ok
 回车退出。
（5）设置完成后，建立依赖关系，编译内核
#make dep       //建立依赖关系
#make zImage     //编译内核
（6）编绎结束后，在arch/arm/boot目录下生成了linux内核的压缩映像zImage。
** 5、下载内核映像到开发板：**
**//在PC机终端进行**：
（1）使用tftpcmd网络传输，设置PC机IP地址，将其地址与开发平台的IP地址设置在同一网段内。这里，将PC的IP设为192.168.3.111。并把tftpcmd复制到/bin（PC机的/bin目录）文件夹下。
（2）#cd/s3c2410/2.4.18-rmk7/arch/arm/boot      //到boot目录下
**//在minicom终端进行**
(3)将开发板与PC机用交叉网线连接好，设置好minicom,复位开发板，按住A键，进入BIOS命令行状态提示符,输入命令netload,回车。
**//在PC机终端进行：**（4）打开一个终端，在/s3c2410/2.4.18-rmk7/arch/arm/boot下新建一个down文件
#vi down
在down文件里输入如下内容：
tftpcmd   192.168.3.100  69  put zImage 
（ip 是arm开发板BIOS中的ip地址；zImage则是要下载的文件名）
（5）双击down批处理文件，选择在终端运行，或是输入如图所示命令，可以
看到内核映像下载到了开发板。（注意：down文件要和zImage文件在同一个目
录下）
#./down
（6）传输完后，在minicom终端输入命令“nfprog”,然后回车，然后输入“1”
选择第二个区块，输入“Y”确认将文件烧写到nandflash中。
至此，内核配置，下载，烧写完全了。
**二、应用程序编写**1、Hello,World!
**Step1：编辑源代码//在PC机终端进行**在PC机的Linux下用vi编辑器以下源代码，并保存为helloworld.c
#cd/s3c2410     //到s3c2410目录下
#vi helloworld.c      //编辑helloworld.c程序
源程序如下：
#include<stdio.h>
int main(void)
{
printf("hello,world!\n");
return 0;
}
**Step2：编译helloworld**
使用以下命令编译：arm-linux-gcc –o helloworld  helloworld.c
(arm-linux-gcc:编译命令)
(helloworld : 编译生成的可执行文件的文件名)
(helloworld.c :被编译文件的文件名)
将生成helloworld可执行文件。
**Step3：下载并运行**
**将可执行文件移动到FS2410P目前主要有三两种方式**：
（1）通过网络移动
通过网络下载程序的主要步骤是：先把helloworld复制到ftp共享目录，
然后在FS2410P上使用ftp下载。（请查阅相关资料自行完成）
（2）通过NFS进入映射（可参考文档nfsonARMSYS.pdf）
//在PC机终端进行
编辑PC机端NFS的相关文件---设置NFS共享目录
#vi/etc/exports      //修改PC端/exports文件,在文件中输入以下内容
/s3c2410   192.168.*.*(rw)
(/s3c2410 :   pc 机端欲共享给arm开发板的目录)
(192.168.*.*(rw):   可以用来访问pc机端共享目录的网段)
 #exportfs-rv      //通知系统共享目录改变了,每次修改目录后,要运行此命令来告诉系统此变化
#/etc/rc.d/init.d/nfs start     //启动PC机的NFS服务
启动NFS服务：   [确定]
关掉NFS配额：   [确定]
启动NFS守护进程：   [确定]
启动NFS mountd：   [确定]
关闭防火墙，选择系统－－管理－－安全级别及防火墙，将其防火墙设为禁用。
去掉iptables服务，选择系统－－管理－－services在弹出的界面中，将iptables、ip6tables前面的“√”去掉。
**//在minicom终端进行**
在minicom中，启动linux后，在命令提示符下：
#mount  192.168.3.111:/s3c2410   /tmp
    (192.168.3.111 :为PC机端的IP)
( /s3c2410: PC机端共享给arm板的目录)
( /tmp :ARM板的挂载目录)
#cd/tmp         //到ARM开发板的挂载目录下
#./hellworld             //执行该目录下的helloworld可执行文件。
注意:如果NFS mount不成,
★是否在修改PC机IP之后重新激活网络
★检查PC机和ARM开发板linux系统中的IP是否在同一个段内:
ARM开发板IP查看命令:ifconfig eth0
ARM开发板IP更改命令:ifconfig eth0***.***.***.*
   (***.***.***.*为输入要设的IP地址,每次ARM开发板linux系统重启后要重新设置.或修改根文件系统/usr/etc/rc.local文件中的IP,重新制作根文件系统,烧写到开发板,这样就可以把改变的IP保存到FLASH,根文件系统的制作和烧写办法参考后面)
★可在PC端自已mount一下自已的目录,看是否成功,如果成功,说明NFS配置
正确.
//在PC机终端进行
#mount   192.168.3.111:/s3c2410     /tmp
  (192.168.3.111 为PC机端的IP.)
( /s3c2410 为PC机端共享给ARM板的目录.)
( /tmp 为PC机想要挂载的目录.)
★如果按上述方法配置还mount不成的话,可删去PC端的DNS地址和网关地址
重新激活网络,并重新启动NFS服务：
#/etc/rc.d/init.d/nfs   restart
**（3）将应用程序添加到cramfs根文件系统中及根文件系统的制作：**
（可参考FS2410P实验指导手册v2.1.2.pdf 314页）
//在PC机终端进行
（1）将FS2410XP_camare_demo.cramfs拷贝到/s3c2410目录下。
（2）在该目录下建立两个文件：
#cd/s3c2410/       //到s3c2410目录下
#mkdir chang       //新建目录
#mkdir guo        / /新建guo目录
（3）将FS2410XP_camare_demo.cramfs挂接到chang目录。
#mount  –o  loop  FS2410XP_camare_demo.cramfs   chang
//挂载FS2410XP_camare_demo.cramfs到chang目录下
（4）将chang目录下的内容压缩。
#cd chang     / /到chang目录下
#tar   - cvf   /s3c2410/1.tar    ./         //压缩当前目录为1.tar
(  /s3c2410/1.tar 为生成压缩包的目录,包名为1.tar)
(  ./ 为被压缩的目录,在这是指当前目录)
这时，将在chang的目录产生一个1.tar的包。
#cd..       //回到上一层目录
#mv 1.tar guo      //移动到1.tar到guo目录下
#cd guo       //到guo目录下
#tar-xvf 1.tar        //解压1.tar
#rm 1.tar       //删除1.tar
rm：是否删除一般文件“1.tar”?  y
(5）复制helloworld可执行文件到/s3c2410/guo/bin目录下。
（6）现在开始制作cramfs根文件系统
./mkcramfs   /s3c2410/guo    FS2410XP_camare_demo.cramfs
( ./mkcramfs  :为制作根文件系统的工具,要把mkcramfs这个命令拷贝到当前目录.
( /s3c2410/guo  :为被制作的目录)
( FS2410XP_camare_demo.cramfs  :为制作后生成文件的文件名)
在/s3c2410/guo目录下，生成了新的根文件系统FS2410XP_camare_demo.cramfs，
根文件系统的下载，烧写方法同内核映象一样
将新的根文件系统下载，烧写进nandflash，复位开发板，启动linux,在命令提示符下输入：
//在minicom终端进行
#./helloworld        //执行helloworld程序
Hello,world!    //执行结果

