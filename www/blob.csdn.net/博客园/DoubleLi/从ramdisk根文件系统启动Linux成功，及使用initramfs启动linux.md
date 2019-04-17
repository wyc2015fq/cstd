# 从ramdisk根文件系统启动Linux成功，及使用initramfs启动linux - DoubleLi - 博客园







下面两篇文章是ARM9论坛上的讲解ramdisk文件系统的很不错的文章

今天做了个试验，让Linux2.6.29.4从ramdisk根文件系统启动成功，总结一下。

其中涉及的内容较多，很多东西不再详述，如需深入研究请查阅相关资料（百度或谷歌一下一大堆）。
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

好像友善之臂把内核源码目录里的uImage目标给注释了，以前在论坛里看到过有人说，直接make uImage好像提示没有uImage的目标。所以我就先制作zImage，然后再用u-boot的mkimage工具转化为uImage。其实uImage就是在zImage的开头部分增加了一个64字节的内核映像说明。说明一下：zImage是ARM Linux常用的一种压缩映像文件，uImage是U-boot专用的映像文件，它是在zImage之前加上一个长度为0x40的“头”，说明这个映像文件的类型、加载位置、生成时间、大小等信息。换句话说，如果直接从uImage的0x40位置开始执行，zImage和uImage没有任何区别。另外，Linux2.4内核不支持uImage，Linux2.6内核加入了很多对嵌入式系统的支持，但是uImage的生成也需要设置。

**5.制作uImage内核映像**
由于我使用的Bootloader是u-boot，所以要将zImage转化为uImage，方法如下：
#mkimage -A arm -O linux -T kernel -C none -a 0x30008000 -e 0x30008000 -n "Linux kernel Image" -d zImage uImage-ramdisk
说明：mkimage工具是u-boot格式uImage内核映像制作工具。如果成功编译u-boot之后，它会在u-boot源码树下的tools目录之下。建议将其拷贝到宿主机的/sbin/目录下，以方便使用。mkimage使用时的具体参数不再详述，不清楚的请自己查阅。

**6.制作ramdisk根文件系统(实际上是建立根文件系统）该过程是制作ramdisk根文件系统的核心步骤**，方法如下：
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
#make modules_install ARCH=arm INSTALL_MOD_PATH=/xxx/rootfs//嵌入式中的内核模块都要在文件系统中
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
//完成根文件系统的创建
h）制作ramdisk根文件系统镜像
方法如下：
上http://genext2fs.sourceforge.net/下载可以简单方便的制作ramdisk文件系统的工具genext2fs，这样就不用像网上大多数说的那样繁琐的制作ramdisk映像了，当前最新版本为genext2fs-1.4.1.tar.gz 。
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
   Created:       2010-04-09   15:13:52 UTC
   Image Type:   ARM Linux Kernel Image (uncompressed)
   Data Size:     1917688 Bytes =   1.8 MB
   Load Address: 30008000
   Entry Point:   30008000
   Verifying Checksum ... OK
   Loading Kernel Image ... OK
OK

Starting kernel ...

Uncompressing Linux.............................................................
................................................................ done, booting t
he kernel.
Linux version 2.6.29.4-FriendlyARM (root@localhost.localdomain) (gcc version 4.3
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



# 使用initramfs启动Linux成功，再次总结一下


前天做了个从ramdisk根文件系统启动Linux的实验，写了个帖子。经总版主kasim大侠的提携，得知现在基于Linux的发行版通常采用initramfs代替initrd，架构更简单，应用也更灵活一些。正好今天晚上没事，就做了一下使用initramfs启动Linux，写个帖子总结一下。
这个帖子不会详述具体的每一个步骤，仅仅说明它和使用ramdisk启动系统时所需要做的步骤的区别。事实上我也是在前天配置编译内核使用ramdisk启动系统的步骤的基础上改的。这里只总结使用initramfs和使用ramdisk各自配置内核选项的主要区别和遇到的一些问题。

参照我的另一个帖子《从ramdisk根文件系统启动Linux成功，总结一下》
http://www.arm9home.net/read.php?tid-5610.html


开发环境：Fedora 9
交叉编译工具链：arm-linux-gcc 4.3.2 with EABI
嵌入式Linux内核版本：2.6.29.4-FriendlyARM。本文就是友善之臂的2.6.29.4-FriendlyARM的那个版本config_mini2440_t35的内核的基础上改的。其它版本的应该也类似，仅供参考。
开发板：mini2440-128M Nand Flash
Bootloader：u-boot-2009.11


主要的不同点：
**步骤2.修改内核配置选项**
进入内核源码目录linux-2.6.29目录
#cp config_mini2440_t35 .config
#make menuconfig ARCH=arm
打开配置菜单，配置使用ramdisk启动系统时需要设置两个选项，**这里只需要配置一个配置项：General setup-->选择 Initial RAM filesystem and RAM disk...... 项**
原因很简单，我们使用的是initramfs，而不是ramdisk，所以不用配置ramdisk的驱动支持项Device Drivers-->Block devices-->RAM block device support 项。相应的之后的（4096）Default RAM disk size kbytes等相关默认配置选项就不会再出现了。

**另外initramfs技术和ramdisk技术的另一个重要的区别就是initramfs并不是在内存中模拟出一个磁盘，所以也就不在需要ramdisk中所需的ext2驱动支持。所以，File systems菜单下的ext2文件系统支持< > Second extended fs support选项就可以取消了。**

在这一步**还有一个重要的区别就是需要在General setup-->Initial RAM filesystem and RAM disk...... 项的相关项(/work/rootfs) Initramfs source file(s)中填写你要制作成initramfs格式的根文件系统目录**，在这里我要做的根文件系统的目录是/work/rootfs。

**步骤6.制作initramfs根文件系统h）制作initramfs根文件系统镜像**
制作最小系统根文件系统的步骤和以前制作ramdisk根文件系统的步骤基本一致，这里只说明最后一步有何不同。
因为initramfs根文件系统启动时执行的第一个程序是init，而不是linuxrc，所以在此，我们制作的根文件系统需要**添加一个init文件**，相应的linuxrc文件就不再需要了。
按照如下方式修改根文件系统
#cd /work/rootfs
#ln -s bin/busybox init 
这样就为busybox创建了一个软链接init，这个文件就是我们要创建的init文件。

另外就是我们制作ramdisk根文件系统镜像的时候使用的是genext2fs工具，在此我们制作initramfs根文件系统镜像时，并不需要额外的步骤，而是在你编译Linux内核的时候就自动生成了。**自动生成的initramfs根文件系统镜像在Linux源码树的usr目录下。名字叫initramfs_data.cpio.gz**，它是gz格式的压缩文件。

这样就有一个问题，在**编译可以使用initramfs启动的内核的时候，它的配置选项有一个相关项，就是要在(/work/rootfs) Initramfs source file(s)中填写你要制作成initramfs格式的根文件系统目录。这样就要求我们在编译内核的时候，首先先把根文件系统做好。值得注意的是我们按照这个方法制作出来的内核镜像实际上比原来的大了许多，这是因为我们在做这一步的时候，实际上是将initramfs根文件系统直接合并到内核镜像里边了。这样，合二为一的镜像就不再需要单独烧写根文件系统镜像了，相应的，启动内核时的参数就不需要添加initrd=……来指定initramfs的位置了。当然如果你不想将initramfs合并到内核中，直接用配置ramdisk的内核来启动系统即可，不过这时你就必须使用initrd=……来指定initramfs的位置了，并且第二个参数initramfs根文件系统映像的大小必须指定为实际大小，否则提示映像校验错误，无法启动系统。**

其它的就没有区别了。

-------------------------------------------------------------------
一切工作做好了，uImage和initramfs_data.cpio.gz都已经编译出来了。
用u-boot下载内核镜像和initramfs根文件系统镜像，此时启动系统，最终内核恐慌kernel panic启动失败。
在超级终端的最后一行显示错误如下：
Unpacking initramfs...<0>Kernel panic - not syncing: bad gzip magic numbers

上网查阅了相关的错误，解决方案如下：
　　“这个问题已经解决； 
       是因为，我在u-boot传递给内核的initrd=bufptr，size; 
　　size这个参数中，大小比实际的initramfs的大小还大； 
　　导致unpack_to_rootfs在调用gunzip进行initramfs压缩包进行解压时，gunzip无法判断到initramfs压缩包的结束地址；进行重复的解　　压导致的；（gunzip这个东东真不智能啊，呵呵） 

　　将size设置和initramfs压缩包的大小一致就OK了；”

如法炮制，系统启动成功。







注意生成dev 下的设备节点，否则卡死









