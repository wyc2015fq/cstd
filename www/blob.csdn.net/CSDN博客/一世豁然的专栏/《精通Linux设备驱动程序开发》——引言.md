# 《精通Linux设备驱动程序开发》——引言 - 一世豁然的专栏 - CSDN博客





2014年10月27日 14:04:41[一世豁然](https://me.csdn.net/Explorer_day)阅读数：672标签：[linux内核																[Linux驱动](https://so.csdn.net/so/search/s.do?q=Linux驱动&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)
个人分类：[linux驱动](https://blog.csdn.net/Explorer_day/article/category/2652125)








                
一、相关介绍

 1、Linux发展




 2、GUN  Copyleft




 3、kernel.org




 4、邮件列表和论坛




 5、Llinux发行版





二、查看源代码

 1、获取Linux源代码




 2、打补丁

   1）、为了生成一个包含X.Y.Z-aa-bb补丁的内核，应该首先下载X.Y.Z内核的完整代码，在打上X.Y.Z-aa补丁，最后打上X.Y.Z-aa-bb补丁。


   2）、补丁提交。




 3、查看内核源代码的布局

 1、内核源代码树


   1）、arch。该目录包含了与体系结构相关的文件。可在arch/目录下看到针对ARM、Motorola、68K、s390、MIPS、Alpha、SPARC和IA64等处理器的子目录。

   2）、blcok。该目录主要包含存储设备I/O调度算法的实现。

   3）、crypto。该目录实现了密码操作以及与加密相关的API，他们可被应用于WiFi设备驱动的加密算法等场合。

   4）、Documentation。该目录包含了内核中各个子系统的简要描述，它是研究内核方面问题的参考。

   5）、drivers。此目录包含了大量设备类和外设控制器的驱动。

   6）、fs。这个目录包含了EXT3、EXT4、reiserfs、FAT、VFAT、sysfs、procfs、isofs、IFFS32、XFS、NTFS和NFS等文件系统的实现。

   7）、include。内核头文件位于此目录。

   8）、init。此目录包含了高级别初始化和启动代码。

   9）、ipc。此目录包含了对消息队列、信号、共享内存等进程间通信（IPC）机制的支持。

   10）、kernel。基于内核中与体系架构无关的部分。

   11）、lib。通用内核对象（kobject）处理程序、循环冗余码校验（CRC）计算和拿书等库函数例程位于此目录。

   12）、mm。此目录包含了内存管理的实现。

   13）、net。该目录实现了网络协议。

   14）、scripts。内核编译过程中要使用的脚本位于此目录。

   15）、security。此目录包含了针对安全的框架。

   16）、sound。Linux音频子系统位于此目录。

   17）、此目录包含了initramfs的实现。




 2、源代码游览工具（P6  表1-1）








三、编译内核和可加载模块（略）






