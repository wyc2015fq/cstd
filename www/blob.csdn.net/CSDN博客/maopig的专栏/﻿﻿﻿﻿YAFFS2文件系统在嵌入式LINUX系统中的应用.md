# ﻿﻿﻿﻿YAFFS2文件系统在嵌入式LINUX系统中的应用 - maopig的专栏 - CSDN博客
2012年12月05日 00:11:45[maopig](https://me.csdn.net/maopig)阅读数：1632
### [YAFFS2文件系统在嵌入式LINUX系统中的应用](http://blog.csdn.net/yangjun_0621/article/details/6292550)
2011-03-31 19:59
181人阅读[评论](http://blog.csdn.net/yangjun_0621/article/details/6292550#comments)(0)[收藏]()[举报](http://blog.csdn.net/yangjun_0621/article/details/6292550#report)
1．文件系统简述
随着32位CPU价格不断下跌，片上存储设备的容量相比越来越大，越来越多的嵌入式系统开始应用各种嵌入式操作系统。一般在嵌入式领域，只要应用到操作系统的必然会需要一个，甚至是多个文件系统。所谓文件系统，实际上就是在一个具体的存储设备上任何文件的组织和目录。这个设备可能是各种RAM、NAND FLASH、NOR FLASH等DOC设备，还有以NAND FLASH 为基础的各种存储卡等[1]。目前，针对不同的设备和应用有很多文件系统可供选择，例如EXT2、TEMPFS、RAMDISK、CRAMFS、
 JFFS1/2、YAFFS1/2、XFS等。除此之外，还有很多供应商提供的商用文件系统。
选择文件系统，要根据具体应用的需求。一般，首先要考虑的就是可靠性，健壮性和增强的需求。如果是像工控这样的不需经常更新控制程序的应用来说，我们选择CARAMFS这样的只读文件系统已经足够了，而且它还可以带来的另外一个好处就是 CRAMFS的压缩率高达50%，可以大大节省我们的存储空间。但是如果是像涉及到数据采集这类需要保存数据的应用来说，只读的文件系统就很难满足应用系统的需求，我们可以选择JFFS或者YAFFS这样的可读写的文件系统。
 不过在实际应用中，需要考虑的因素还应该更多。
YAFFS ， Yet Another Flash File System ，是一种类似于 JFFS/JFFS2 的专门为 Flash 设计的嵌入式文件系统。与 JFFS 相比，它减少了一些功能，因此速度更快、占用内存更少。 YAFFS 和 JFFS 都提供了写均衡，垃圾收集等底层操作。它们的不同之处在于：
1 ）、 JFFS 是一种日志文件系统，通过日志机制保证文件系统的稳定性。 YAFFS 仅仅借鉴了日志系统的思想，不提供日志机能，所以稳定性不如 JAFFS ，但是资源占用少。
2 ）、 JFFS 中使用多级链表管理需要回收的脏块，并且使用系统生成伪随机变量决定要回收的块，通过这种方法能提供较好的写均衡，在 YAFFS 中是从头到尾对块搜索，所以在垃圾收集上 JFFS 的速度慢，但是能延长 NAND 的寿命。
3 ）、 JFFS 支持文件压缩，适合存储容量较小的系统； YAFFS 不支持压缩，更适合存储容量大的系统。
YAFFS 还带有 NAND 芯片驱动，并为嵌入式系统提供了直接访问文件系统的 API ，用户可以不使用 Linux 中的 MTD 和 VFS ，直接对文件进行操作。 NAND Flash 大多采用 MTD+YAFFS 的模式。 MTD （ Memory Technology Devices ，内存技术设备）是对 Flash 操作的接口，提供了一系列的标准函数，将硬件驱动设计和系统程序设计分开。
YAFFS2 是 YAFFS 的升级版，能更好的支持 NAND FLASH ，我们采用最新的 YAFFS2 文件系统。我们首先下载 YAFFS 文件系统。http://www.aleph1.co.uk/cgi-bin/viewcvs.cgi/yaffs2.tar.gz?view=tar
2．文件系统的实现
2．1文件系统的选择
在本文涉及的应用中，根文件系统并不需要频繁的更改，但是却涉及到一些数据的存取。考虑到的存储设备是NAND FLASH，所以在根文件系统格式的选择上可以用压缩率比较高的CRAMFS，而在根文件系统下挂载一个可读写的YAFFS2文件系统。
YAFFS2（Yet Another nandFlash FileSytem2）是专门针对NAND设备的一种文件系统。YAFFS2类似于JFFS/JFFS2文件系统，与YAFFS2不同的是JFFSS1 /2文件系统最初是针对NOR FLASH的应用场合设计的。而YAFFS2针对NAND FLASH的特点采用增强的碎片回收和均衡磨损技术，大大提高了读写速度，延长了存储设备的使用寿命[2],可以更好的支持大容量的NAND
 FLASH芯片。而且在断电可靠性上，YAFFS2的优势更加明显[3]。
         2．2建立根文件系统
Linux内核启动以后，根据参数linux_cmd_line的指示跳转到根文件系统的地址，完成文件系统的初始化和挂载任务。根文件系统的建立可以借助各种开发套件来完成，也可以完全由自己搭建一个。这里介绍如何通过开发套件的帮助来完成基本的根文件系统。Busybox是由Bruce Perens发起的一项计划，该计划最初的目的是协助Debian发行套件建立安装磁盘。由于Busybox小巧但功能不失强大，所以在嵌入式系统中有很广的应用。下载Busybox稳定版（www.busybox.net），解压后直接make
 menuconfig 进入Busybox配置菜单，这个界面根内核差不多。在Applets下面选择你需要命令，然后退出保存, 执行make , make install。在Busybox的目录下就会生成_install目录，这个目录就包含了将要配置的命令的工具包。把这个目录拷贝到工作目录，更名为 rootfs, 这个目录就是要建立的根文件系统的雏形——但是它还不能工作，需要进一步的完善。
在rootfs/下，首先在/rootfs/dev下建立必须的设备文件。关于设备的建立，可以参考Filesystem Hierarchy Standard来确定需要的哪些设备文件(http://www.pathname.com/fhs/)。下一步就是完成/rootfs/etc这个目录下的3个比较重要的文件inittab,fstab,init.d/rcS(或者是rc.d/rc.sysinit,这个由inittab决定)。下面给出可供参考的样例：
Inittab文件
#System initialization .
::sysinit:/etc/init.d/rcS
::askfirst:/bin/sh
::restart:/sbin/init
::shutdown:/bin/umount –a –r
::shutdown:/sbin/swapoff –a
Fstab文件
none    /proc   proc   defaults     0   0
tmpfs   /temp    tmfs   defaults     0   0
fstab指定了设备的挂载点。要了解关于fstab文件的格式以及详细信息，可以参考
[http://www.tuxfiles.org/linuxhelp/fstab.html](http://www.tuxfiles.org/linuxhelp/fstab.html)
Init.d/rcS文件
#！/bin/sh
Mount –o remount, rw /
/bin/mount –a
>/etc/mtab
Echo  “messages”
Hostname  “hostname”
在这个脚本中，还可以定义内核的其他一些动作，这在后面挂载YAFFS2文件系统中显得非常有用。
最后，加入需要的链接库文件就可以了。通常这部分会比较麻烦，因为你并不知道你的应用具体需要哪些库文件，可以通过两种途径解决这个问题：一是利用工具readelf来找出应用程序依存哪些链接库；二是参考linux相关计划网站上的提供的文件系统，来确定需要的链接库文件[4]。
2．3制作YAFFS2文件系统
根文件系统做好后，可以通过NFS（[http://nfs.sourceforge.net/nfs-howto/](http://nfs.sourceforge.net/nfs-howto/)）来测试文件系统是否可行，但是最后我们还是要把这个文件系统烧进目标板。
可以有两种方式来应用YAFFS2文件系统，一是CRAMFS+YAFFS2的组合，一种是纯YAFFS2文件系统格式 。由于在笔者的应用中，内核和根文件系统并不需要频繁更新，所以从节省存储空间和保护根文件系统的角度来考虑，我们选用CRAMFS这样的高压缩率、只读文件系统，而应用程序，用户空间使用YAFFS2文件系统格式。
对于CRAMFS，可以直接用mkcramfs  /rootfs  ~/root.img来制作cramfs镜像，/rootfs是前面建立的根文件系统目录，~/root.img指定做好镜像的保存目录。由于 YAFFS2文件系统当初设计就是针对nand设备的，在笔者的应用中，文件系统是在NANDFLASH中启动，因此要使用YAFFS2文件系统,就必须先在内核添加NAND设备驱动[5]，并修改arch/arm/mach-s3c2410/devs.c文件,增加nand
 flash分区。一般情况下bootloader, kernel, root 三个分区是必须的，也有的加了param分区来保存内核参数，user分区来作为用户空间的，但是也可以不要。
在[www.aleph.co.uk](http://www.aleph.co.uk/)下载YAFFS2（GPL）源码，解压后在YAFFS2目录下执行
patch-ker.sh  ~/Linux2.6.14.2
~/Linux2.6.14.2是内核的路径，笔者的内核是2.6.14.2。patch后在内核的fs目录下就会生成yaffs2目录，这样内核就已经得到YAFFS2的支持了。
接下来make menuconfig ,配置内核时在
Device Drivers->
Memory Technology Device (MTD)->
下选中MTD Partitioning Surpport,这样我们的分区信息才有效。在
File System->
Miscellaneous filesystems->下配置YAFFS2如下:
<>EFS file system support(read only) (EXPERIMENTAL)
<*>YAFFS2 file system support
--- 512 byte / page devices
[*]Lets Yaffs do its own ECC
[*]Use the same ecc byte order as Steven Hill’s nand_ecc.c
--- 2048 byte (or larger) / page devices
[*]Autoselect yaffs2 format
[*]Disable lazy loading
[*]Turn off wide tnodes
[*]Turn off debug chunk erase check
[*]Cache short names in RAM
<>Jounalling Flash File System (JFFS) support
保存配置，退出，执行make。Make结束后就得到了一个具备挂载yaffs2文件系统能力的内核镜像 。
在bootloader源文件中，将nand flash分成几个分区，前面提到的devs.c文件里的分区也必须与bootloader的分区一致 ，因为bootloader指定了内核和文件系统的地址，最后启动时也会到相应的地址寻找指令。在我们的应用中，将flash分成bootloader, param,
 kernel, root, user。其中root用来挂载CRAMFS根文件系统，user分区挂载YAFFS2文件系统。
在YAFFS2源文件的utils目录下，执行make就可以生成 mkyaffs2image工具，执行
./mkyaffs2image （要制作yaffs2的目录） (目标镜像)
生成yaffs2文件系统，不过因为前面根文件系统是CRAMFS格式，YAFFS2可以作为模块来加载，这样更加方便，更加灵活。加载模块也可以在后面给出的脚本中定义。
最后一步就是要让内核在启动后挂载文件系统，挂载文件系统有两种方式：手动和自动挂载。在实际应用中当然是要系统在无需人工干预的情况下自动挂载文件系统。在Debian系统中，初始化脚本是/etc/init.d/rcS，在Rad Hat中是/etc/rc.d/rc.sysinit。这里面包含了装入文件系统，设置时间，打开交换分区，得到主机名等等内容。回顾前面给出的 Inittab和init.d/rcS文件就知道，系统的Init进程是如何动作的。对照前面的rcS文件这里给出一个更加详细的并给出注释。
#! /bin/sh                                     //busybox提供的SHELL
PATH=/sbin:/bin:/usr/sbin:/usr/bin             //设置系统路径
runlevel=S                                     //运行级别
prevlevel=N                                        
umask 022                                      //缺省访问权限为022
export PATH runlevel prevlevel
/bin/mount -f -t cramfs -o remount,ro /dev/mtdblock/3 /
                                             //cramfs作为根文件系统挂到第四个分区下,即root分区。
/sbin/insmod -f /lib/yaffs2.ko                 //加载yaffs2模块
/sbin/mount -t yaffs2 /dev/mtdblock/4 /usr       //yaffs2挂到第五个分区，作为/usr
/sbin/mount -t proc none /proc                 //mount命令由busybox提供
/sbin/mount -t tmpfs none /root                //挂载tmpfs到指定目录,下同
/sbin/mount -t tmpfs none /tmp
/sbin/mount -t tmpfs none /var
/sbin/mkdir -p /var/lib                         //创建目录
/sbin/mkdir -p /var/run
/sbin/mkdir -p /var/log
/etc/rc.d/init.d/leds start                    //初试用户程序leds
/sbin/ifconfig lo 127.0.0.1                    //指定lo设备的IP地址为127.0.0.1
#/sbin/ifconfig eth0 192.168.0.2
/sbin/hostname -F /etc/sysconfig/HOSTNAME       //指定主机名文件路径.
将文件系统烧进目标板对应分区，boot,即可运行。至此，目标板可以通过/usr进行读写文件，目标板完成对yaffs2的支持。
