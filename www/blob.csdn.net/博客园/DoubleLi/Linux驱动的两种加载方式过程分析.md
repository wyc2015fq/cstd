# Linux驱动的两种加载方式过程分析 - DoubleLi - 博客园






一、概念简述

在Linux下可以通过两种方式加载驱动程序：静态加载和动态加载。

静态加载就是把驱动程序直接编译进内核，系统启动后可以直接调用。静态加载的缺点是调试起来比较麻烦，每次修改一个地方都要重新编译和下载内核，效率较低。若采用静态加载的驱动较多，会导致内核容量很大，浪费存储空间。

动态加载利用了Linux的module特性，可以在系统启动后用insmod命令添加模块（.ko），在不需要的时候用rmmod命令卸载模块，采用这种动态加载的方式便于驱动程序的调试，同时可以针对产品的功能需求，进行内核的裁剪，将不需要的驱动去除，大大减小了内核的存储容量。

在台式机上，一般采用动态加载的方式；在嵌入式产品里，可以先采用动态加载的方式进行调试，调试成功后再编译进内核。

Linux下PCI设备驱动程序之注册详解 [http://www.linuxidc.com/Linux/2014-02/97074.htm](http://www.linuxidc.com/Linux/2014-02/97074.htm)

裸机驱动与Linux设备驱动的区别 [http://www.linuxidc.com/Linux/2013-08/88799.htm](http://www.linuxidc.com/Linux/2013-08/88799.htm)

Linux设备驱动开发详解(第2版)源代码 下载 [http://www.linuxidc.com/Linux/2013-07/86977.htm](http://www.linuxidc.com/Linux/2013-07/86977.htm)

Linux设备驱动开发详解(第2版)高清PDF [http://www.linuxidc.com/Linux/2013-07/86976.htm](http://www.linuxidc.com/Linux/2013-07/86976.htm)

二、实例分析

下面以Linux下音频驱动的加载为例，分析两种方式的加载过程。

1、静态加载

1）解压内核，修改硬件架构和编译器；

将内核压缩文件linux-2.6.8.1-zzm.tar.bz2解压到/home/sxy/目录下，命令是![](http://www.linuxidc.com/upload/2014_06/140623090499681.png)，解压后得到内核源码目录文件linux-2.6.8.1-zzm，进入该目录，编辑Makefile文件，将ARCH改为arm，CROSS_CPMPILE改为arm-linux-，如下图所示：

![](http://www.linuxidc.com/upload/2014_06/140623090499682.png)

保存后退出。

2）配置内核；

在内核源码树目录下，输入make menuconfig命令，进入内核配置界面，进入“Load an Alternate Configuration File”选项，载入配置文件kernel_2410.cfg，保存退出，过程如下图所示：

![](http://www.linuxidc.com/upload/2014_06/140623090499683.png)

![](http://www.linuxidc.com/upload/2014_06/140623090499684.png)

![](http://www.linuxidc.com/upload/2014_06/140623090499685.png)

再次输入make menuconfig命令，编辑sound选项，**将其编译进内核（*）**，结果如下图所示，最后保存配置，退出。

![](http://www.linuxidc.com/upload/2014_06/140623090499686.png)

3）编译内核；

在源码树目录下输入make zImage命令，编译完成后可以在/arch/arm/boot/目录下生成zImage镜像文件。

4）下载内核

将内核镜像文件zImage下载到开发板上，当串口终端显示如下信息时，表示驱动加载成功。

![](http://www.linuxidc.com/upload/2014_06/140623090499687.png)

2、动态加载

1）解压内核，过程与静态编译时一样，略；

2） 配置内核，前面过程与静态编译时一样，再次输入命令make menuconfig，配置sound选项时，**将其编译成模块（M）**，结果如下图所示，最后保存配置，退出；

![](http://www.linuxidc.com/upload/2014_06/140623090426341.png)

这样就将声卡驱动编译成模块，可以动态选择是否加载到内核中。

3）下载内核

将内核镜像文件zImage下载到开发板上，验证能否驱动声卡的过程如下：

![](http://www.linuxidc.com/upload/2014_06/140623090426342.png)

说明：首先，将虚拟机下的/home/目录挂载到开发板上的/tmp/目录下，然后**先后**加载soundcore.ko和s3c2410-oss.ko两个模块，最后通过lsmod命令查看是否加载上声卡驱动，结果显示加载成功，这样就可以在应用空间编程，实现音频的录放等操作。

**PS：①采用make menuconfig命令时，选项*代表Y，表示将驱动编译进内核；M表示将驱动编译成模块；空代表N，表示不编译；**

**②内核文件与模块两者有很多东西必须匹配，编译器版本、源码版本、编译时的配置等，所以当内核文件修改了，譬如修改了驱动的编译选项（Y、M、N），那么就必须重新编译和下载内核，否则会出错。**

三、遇到的问题

**问题：**动态加载过程中，出现下面错误：

![](http://www.linuxidc.com/upload/2014_06/140623090426343.png)

错误：注册和注销设备的符号未知。

**解决方法：**寻找依赖关系，查看几个符号的定义，发现在soundcore.c文件中定义了以上几个函数，同时导出了符号，以register_sound_dsp为例，如下图所示：

![](http://www.linuxidc.com/upload/2014_06/140623090426344.png)

所以应该先加载soundcore.ko，后加载s3c2410-oss.ko。

注意：在Kconfig和Makefile文件中定义了依赖关系，也可以查找到问题的原因。

**本文永久更新链接地址**：[http://www.linuxidc.com/Linux/2014-06/103569.htm](http://www.linuxidc.com/Linux/2014-06/103569.htm)









