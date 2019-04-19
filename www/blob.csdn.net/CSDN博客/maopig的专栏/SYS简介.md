# SYS简介 - maopig的专栏 - CSDN博客
2012年03月26日 23:34:02[maopig](https://me.csdn.net/maopig)阅读数：981标签：[output																[attributes																[数据结构																[interface																[video																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=video&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)](https://so.csdn.net/so/search/s.do?q=output&t=blog)
个人分类：[Linux](https://blog.csdn.net/maopig/article/category/850267)
"sysfs is a ram-based filesystem initially based on ramfs. It provides a means
to export kernel data structures, their attributes, and the linkages between them to
userspace.” --- documentation/filesystems/sysfs.txt
可以先把documentation/filesystems/sysfs.txt读一遍。文档这种东西，真正读起来就嫌少了。Sysfs文件系统是一个类似于proc文件系统的特殊文件系统，用于将系统中的设备组织成层次结构，并向用户模式程序提供详细的内核数据结构信息。
去/sys看一看，
localhost:/sys#ls /sys/
block/ bus/ class/ devices/ firmware/ kernel/ module/ power/
Block目录：包含所有的块设备
Devices目录：包含系统所有的设备，并根据设备挂接的总线类型组织成层次结构
Bus目录：包含系统中所有的总线类型
Drivers目录：包括内核中所有已注册的设备驱动程序
Class目录：系统中的设备类型（如网卡设备，声卡设备等） 
sys下面的目录和文件反映了整台机器的系统状况。比如bus，
localhost:/sys/bus#ls
i2c/ ide/ pci/ pci express/ platform/ pnp/ scsi/ serio/ usb/
里面就包含了系统用到的一系列总线，比如pci, ide, scsi, usb等等。比如你可以在usb文件夹中发现你使用的U盘，USB鼠标的信息。
我们要讨论一个文件系统，首先要知道这个文件系统的信息来源在哪里。所谓信息来源是指文件组织存放的地点。比如，我们挂载一个分区，
mount -t vfat /dev/hda2 /mnt/C
我们就知道挂载在/mnt/C下的是一个vfat类型的文件系统，它的信息来源是在第一块硬盘的第2个分区。
但是，你可能根本没有去关心过sysfs的挂载过程，她是这样被挂载的。
mount -t sysfs sysfs /sys
ms看不出她的信息来源在哪。sysfs是一个特殊文件系统，并没有一个实际存放文件的介质。断电后就玩完了。简而言之，sysfs的信息来源是kobject层次结构，读一个sysfs文件，就是动态的从kobject结构提取信息，生成文件。
所以，首先，我要先讲一讲sysfs文件系统的信息来源 -- kobject层次结构。kobject层次结构就是linux的设备模型。
 牛人博客：[http://blog.csdn.net/fudan_abc/article/details/1768277](http://blog.csdn.net/fudan_abc/article/details/1768277)
## [SysFs](http://www.cnblogs.com/noaming1900/archive/2010/11/04/1868784.html)
V4L2 specifications allow switching output and standard using IOCTLs. FBDev specifications allow
switching of resolutions at the output, but not the output itself. In the past, proprietary IOCTLs were added
in FBDev to allow output switching. Instead of abusing the FBDev interface with proprietary IOCTLs, it was
decided to remove this functionality from V4L2 and FBDev and implement the same functionality as a
SysFs driver attribute. This can be extended to support simple functions like enable/disable display,
control brightness, hue, etc. The LSP 2.00 DaVinci Video Sysfs User's Guide (SPRUG95)explains the
procedure to change the output and standard to work with the current display device.
