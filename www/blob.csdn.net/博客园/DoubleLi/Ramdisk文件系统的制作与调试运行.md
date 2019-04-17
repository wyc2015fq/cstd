# Ramdisk文件系统的制作与调试运行 - DoubleLi - 博客园






开发环境：Fedora 9
交叉编译工具链：arm-linux-gcc 4.3.2 with EABI
嵌入式Linux内核版本：2.6.29.4-FriendlyARM。昨天写贴子的时候具体记不清了，今天起来启动开发板用uname -r查一下，就是叫做2.6.29.4-FriendlyARM，帖子已经改好了。本文就是友善之臂的2.6.29.4-FriendlyARM的那个版本的内核的基础上改的。其它版本的应该也类似，仅供参考。
开发板：mini2440-128M Nand Flash
Bootloader：u-boot-2009.11

具体步骤如下：
**1.解压内核源码树**
解压linux-2.6.29-mini2440-20090708.tgz到自己的工作目录，会生成一个友善之臂修改过的并且有几个mini2440默认配置文件的内核源码目录linux-2.6.29。具体步骤参照友善之臂mini2440开发板用户手册，具体不详述了。

**2.修改内核配置选项**
进入内核源码目录linux-2.6.29目录
#cp config_mini2440_t35 .config
#make menuconfig ARCH=arm
打开配置菜单，修改两个配置项，分别是：
a）：General setup-->选择 Initial RAM filesystem and RAM disk...... 项
b）：Device Drivers-->Block devices-->选择 RAM block device support 项

并检查Optimize for size是否被选中，如果没有则选中，此项优化内核大小，根据需要进行配置。
修改（8192）Default RAM disk size kbytes选项为**（4096）Default RAM disk size kbytes**，之所以修改是因为我之后制作的ramdisk是4096KB大小的。当然如果你想制作8192KB大小的ramdisk，这里就要对应为8192了，以此类推。但是最小系统嘛，是不用那么大的ramdisk的。此项的默认配置就是（4096），以前我改过这个配置，所以是（8192）了。如果这个大小和你做的ramdisk不匹配，则启动时仍然会出现kernel panic内核恐慌，提示ramdisk格式不正确，挂载不上ramdisk。
**然后特别要注意的一点是，ramdisk是一种内存虚拟磁盘技术，实质上并不是一种文件系统，它使用的文件系统时ext2文件系统。所以一定要在make menuconfig ARCH=arm的时候进入File systems菜单，选上<*> Second extended fs support。以提供内核对ext2文件系统的支持。**我以前添加过了ext2文件系统了，所以开始的时候在此没有说明，在此为了说明为什么有的人照着我的方法做了，但是仍然kernel panic，特别把这一步也加上。
然后保存配置退出。
这样就为内核添加好了ramdisk启动功能和ramdisk的驱动支持了。

**3.修改内核启动参数**
方法有二：
a）：修改.config的第310行，修改CONFIG_CMDLINE=""的定义
修改为CONFIG_CMDLINE="initrd=0x31000000,0x200000 root=/dev/ram rw init=/linuxrc console=ttySAC0 mem=64M"
保存。
意思为从ramdisk启动，ramdisk压缩文件起始地址在内存地址0x31000000处，文件大小为0x200000。
此参数也可以在make menuconfig ARCH=arm时进入Boot options菜单，然后在Default kernel command string里修改。效果是一样的。
b）：或者不修改.config的的第310行CMDLINE定义,而是用u-boot的bootargs环境变量来传递启动参数。
同样也是修改该环境变量为bootargs=initrd=0x31000000,0x200000 root=/dev/ram rw init=/linuxrc console=ttySAC0 mem=64M
并saveenv保存u-boot环境变量
以上a），b）的效果是一样的。

**4.编译内核**
#make zImage ARCH=arm CROSS_COMPILE=arm-linux-
然后是二十分钟左右的等待。
编译完成后在当前目录下就出现了zImage内核映像了。
好像友善之臂把内核源码目录里的uImage目标给注释了，以前在论坛里看到过有人说，直接make uImage好像提示没有uImage的目标。所以我就先制作zImage，然后再用u-boot的mkimage工具转化为uImage。其实uImage就是在zImage的开头部分增加了一个64字节的内核映像说明。

**5.制作uImage内核映像**
由于我使用的Bootloader是u-boot，所以要将zImage转化为uImage，方法如下：
#mkimage -A arm -O linux -T kernel -C none -a 0x30008000 -e 0x30008000 -n "Linux kernel Image" -d zImage uImage-ramdisk
说明：mkimage工具是u-boot格式uImage内核映像制作工具。如果成功编译u-boot之后，它会在u-boot源码树下的tools目录之下。建议将其拷贝到宿主机的/sbin/目录下，以方便使用。mkimage使用时的具体参数不再详述，不清楚的请自己查阅。

**6.制作ramdisk根文件系统该过程是制作ramdisk根文件系统的核心步骤**，方法如下：
a）创建根文件系统目录：
#cd转入到你的工作目录。
#mkdir rootfs
#cd rootfs
#mkdir bin dev etc lib proc sbin sys usr mnt tmp var
#mkdir usr/bin usr/lib usr/sbin lib/modules

b）创建最基本的设备文件：
#cd dev
#mknod -m 666 console c 5 1
#mknod -m 666 null c 1 3
#cd ..

c）安装/etc配置文件：
这里可以直接把友善之臂的root_qtopia里的几个基本的配置文件拷贝过来，只拷贝必要的即可，并对其内容进行删减，因为我做的ramdisk并不包含Qtopia等，全拷贝过来也没有用。
我是从网上找的最小系统的etc配置文件直接解压到我制作的根文件系统里了，并做参照友善之臂的root_qtopia添加了一些内容，见最后的说明。
操作如下：
#tar etc.tar.gz -C /xxx/rootfs
xxx表示你要制作的rootfs所在的目录。

d）编译内核模块：
方法是如下：
进入Linux内核源码目录（linux-2.6.29）
#make modules ARCH=arm CROSS_COMPILE=arm-linux-

e）安装内核模块：
#make modules_install ARCH=arm INSTALL_MOD_PATH=/xxx/rootfs
xxx表示你要制作的rootfs所在的目录。

f）配置busybox
进入busybox目录执行#make menuconfig
　　进入Busybox Settings -> build Options ->选中"Build busybox as a static binary“，即静态链接，免去拷贝大量的库文件。
　　Installation Options -> 选中"Don't use /usr"，以免busybox不慎被安装到宿主机系统的相应目录下，破坏宿主机系统。
　　Busybox Installation Prefix （/xxx/rootfs），修改该选项表明编译后的busybox将安装到该位置。

g）编译、安装busybox
#make ARCH=arm CROSS_COMPILE=arm-linux-
几分钟编译完成后
#make install
安装到Busybox Installation Prefix （/xxx/rootfs）设定的目录里。当前为我要制作的根文件系统目录（/xxx/rootfs）。

h）制作ramdisk根文件系统镜像
方法如下：
上[http://genext2fs.sourceforge.net/](http://genext2fs.sourceforge.net/)下载可以简单方便的制作ramdisk文件系统的工具genext2fs，这样就不用像网上大多数说的那样繁琐的制作ramdisk映像了，当前最新版本为genext2fs-1.4.1.tar.gz ，下载具体地址：http://sourceforge.net/projects/genext2fs/?source=dlp
编译生成该工具genext2fs，并将其放入宿主机的/sbin/目录下以方便使用。
跳转到要制作的rootfs的上一级目录
#genext2fs -b 4096 -d rootfs ramdisk
-b是指制作的ramdisk大小为4096K字节
-d是指要制作成ramdisk的根文件系统目录
最后的ramdisk是制作出来的ramdisk的名字，当然可以改名了。
#gzip -9 -f ramdisk
将该ramdisk以最优方式压缩为ramdisk.gz

**7.下载内核映像和ramdisk映像**
启动u-boot，暂停u-boot自动启动，在u-boot命令行中键入如下命令下载内核和ramdisk映像：
[u-boot@MINI2440]# tftp 0x32000000 uImage-ramdisk
dm9000 i/o: 0x20000300, id: 0x90000a46
DM9000: running in 16 bit mode
MAC: 08:08:11:18:12:27
operating at 100M full duplex mode
Using dm9000 device
TFTP from server 192.168.31.117; our IP address is 192.168.31.230
Filename 'uImage-ramdisk'.
Load address: 0x32000000
Loading: T T #################################################################
         #################################################################
         #
done
Bytes transferred = 1917752 (1d4338 hex)
[u-boot@MINI2440]# tftp 0x31000000 ramdisk.gz
dm9000 i/o: 0x20000300, id: 0x90000a46
DM9000: running in 16 bit mode
MAC: 08:08:11:18:12:27
operating at 100M full duplex mode
Using dm9000 device
TFTP from server 192.168.31.117; our IP address is 192.168.31.230
Filename 'ramdisk.gz'.
Load address: 0x31000000
Loading: T T #################################################################
         ####
done
Bytes transferred = 1002594 (f4c62 hex)

**8）使用ramdisk根文件系统启动内核**
在u-boot命令行下键入如下命令启动系统：
bootm 0x32000000
## Booting kernel from Legacy Image at 32000000 ...
   Image Name:   Linux-2.6.29.4-FriendlyARM
   Created:      2010-04-09  15:13:52 UTC
   Image Type:   ARM Linux Kernel Image (uncompressed)
   Data Size:    1917688 Bytes =  1.8 MB
   Load Address: 30008000
   Entry Point:  30008000
   Verifying Checksum ... OK
   Loading Kernel Image ... OK
OK

Starting kernel ...

Uncompressing Linux.............................................................
................................................................ done, booting t
he kernel.
Linux version 2.6.29.4-FriendlyARM ([root@localhost.loca](mailto:root@localhost.loca)ldomain) (gcc version 4.3
.2 (Sourcery G++ Lite 2008q3-72) ) #3 Fri Apr 9 23:13:36 CST 2010
CPU: ARM920T [41129200] revision 0 (ARMv4T), cr=c0007177
CPU: VIVT data cache, VIVT instruction cache
Machine: FriendlyARM Mini2440 development board
Memory policy: ECC disabled, Data cache writeback
……中间过长的内容在此省略了……
RAMDISK: Compressed image found at block 0
VFS: Mounted root (ext2 filesystem) on device 1:0.
Freeing init memory: 136K
eth0: link down

Processing /etc/profile... Done

# eth0: link up, 100Mbps, full-duplex, lpa 0x45E1

至此系统启动成功。

----------------------------------------------------------------------------------
**后记：**由于友善之臂mini2440默认的配置文件是将驱动等直接编译到内核里去了，并没有使用内核模块。所以上述第6步制作根文件系统中的d）编译和e）安装内核模块的步骤可以不做。我就没有编译内核模块。因为参照友善之臂的root_qtopia根文件系统发现/lib/modules/`uname -r`里根本没有东西。而且以前用友善之臂的config_mini2440_t35默认配置编译内核模块的时候。发现友善只做了个hello_modules的内核模块示例。在此我们不需要使用这个hello_modules，所以这两步就略过去了。

**说明：**由于最小系统里的etc配置文件没有写自动启动网卡的脚本，所以我做的ramdisk开始时并不能自动启动网卡。后来参照友善之臂的root_qtopia根文件系统里的脚本做如下修改，即可自动启动脚本。
修改如下：
修改要制作的ramdisk根文件系统的rootfs下的/etc/init.d/rcS文件
在最下边添加如下两行命令即可自动启动网卡：
/sbin/ifconfig lo 127.0.0.1
/sbin/ifconfig eth0 192.168.31.230 netmask 255.255.255.0 up

这个方法是通过分析友善之臂的root_qtopia根文件系统的启动流程时发现的。
通过分析友善之臂的root_qtopia根文件系统，我们还能发现更多的内容。在此不在详述。
锻炼一下自己分析解决问题的能力吧。呵呵。



附加：

 另外可以使用以下方法把kernel和ramdisk都烧写到nand flash中，修改bootcmd，这样u-boot就可以自动启动该系统了。以下仅仅是方法，没有具体说明。大家可以自己做一下。
方法如下：

u-boot# tftp 0x32000000 zImage
u-boot# nand erase 0x00200000 0x200000
u-boot# nand write 0x32000000 0x00200000 0x00200000
u-boot# setenv bootcmd nand read 0x32000000 0x00200000 0x00200000\;go 0x30008000
u-boot# boot
注意 0x200000 是2M之内的内核 若你的内核大于2M则把 0x200000 换成 0x300000 （比如说内核大小在3M内）
其实就是uboot中的活用 go 命令，启动 zImage 格式的内核的方法。
####################################################################
u-boot# tftp 0x32000000 uImage
u-boot# nand erase 0x00200000 0x200000
u-boot# nand write 0x32000000 0x00200000 0x00200000
u-boot# setenv bootcmd nand read 0x32000000 0x00200000 0x00200000\;bootm 0x30008000
u-boot# boot
注意 0x200000 是2M之内的内核 若你的内核大于2M则把 0x200000 换成 0x300000 （比如说内核大小在3M内）
其实就是uboot中使用 bootm 命令启动 uImage 格式的内核的方法。
====================================================================
注：nand erase 要擦除的起始地址 要擦除的字节数
　　nand write 要写入的起始地址 要写入的字节数
　　另外要擦除/写入的起始地址和要擦除/写入的字节数都必须是你的u-boot擦除和写入的最小块大小的整数倍。例如tekkaman大侠的u-boot-2009.11_tekkaman的Erase和Write的最小块大小都是128KB，所以以上的四个数(要擦除/写入的起始地址、要擦除/写入的字节数)都必须是128KB的整数倍。



**说明：本博客转载自http://www.arm9home.net/read.php?tid-5610.html 技术论坛，感觉博主，文章写的很不错，具有很强的实践意义，最近在看ramdisk,发现了这篇文章，与大家分享。**









