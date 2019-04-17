# 嵌入式 VFS: Cannot open root device "mtdblock2" or unknown-block(2,0) - DoubleLi - 博客园






系统启动后，虽然nand驱动表现正常，但是最后挂载rootfs时候出错：

Kernel command line: root=/dev/mtdblock2 rw init=/linuxrc console=ttyAMA1,115200 mem=64M rootfstype=yaffs2
。。。。
。。。。

AS353X NAND Driver, (c) 2010 austriamicrosystems
as353x_nand_probe
Nand clock set to 24000000
Nand:res->start at e60000
Nand:mapped registers at c48dc000
as353x_nand_inithw
Nand:initialising set 0 (c3a62800, info c3a934e0)
as353x_nand_init_chip
NAND device: Manufacturer ID: 0xec, Chip ID: 0xd3 (Samsung NAND 1GiB 3,3V 8-bit)
Scanning device for bad blocks
Bad eraseblock 2262 at 0x11ac0000
Bad eraseblock 3151 at 0x189e0000
Bad eraseblock 3649 at 0x1c820000
Bad eraseblock 5198 at 0x289c0000
Bad eraseblock 6842 at 0x35740000
as353x_nand_add_partition
Creating 5 MTD partitions on "NAND 1GiB 3,3V 8-bit":
0x00000000-0x00100000 : "U-Boot"
0x00100000-0x00900000 : "Kernel"
0x00900000-0x0d100000 : "Root filesystem"
0x0d100000-0x11100000 : "Temp"
0x11100000-0x3d700000 : "Data"
Nand:initialised ok
。。。。。。。
Root-NFS: No NFS server available, giving up.
VFS: Unable to mount root fs via NFS, trying floppy.
VFS: Cannot open root device "mtdblock2" or unknown-block(2,0)
Please append a correct "root=" boot option; here are the available partitions:
Kernel panic - not syncing: VFS: Unable to mount root fs on unknown-block(2,0)

【解决过程】

1.开始以为rootfs里面没有/dev/mtdblock2这个设备呢，后去制作rootfs的所用的文件那里查证，rootfs里面是有这些设备的：

[crifan@linux-41lh root]$ls dev/mtd* -l
-rw-r----- 1 eric develop 0 2010-01-12 13:58 dev/mtd0
-rw-r----- 1 eric develop 0 2010-01-12 13:58 dev/mtd1
-rw-r----- 1 eric develop 0 2010-01-12 13:58 dev/mtd2
-rw-r----- 1 eric develop 0 2010-01-12 13:58 dev/mtd3
-rw-r----- 1 eric develop 0 2010-01-12 13:58 dev/mtdblock0
-rw-r----- 1 eric develop 0 2010-01-12 13:58 dev/mtdblock1
-rw-r----- 1 eric develop 0 2010-01-12 13:58 dev/mtdblock2
-rw-r----- 1 eric develop 0 2010-01-12 13:58 dev/mtdblock3

【！！！此理解错误->】此外，最后经证实和确定，系统访问/dev/mtdblock2时候，这些块设备是系统内部生成的，即使你rootfs里面没建立这些设备文件，也是不影响的。

2.以为rootfs数据有问题，后来发现，此时还未涉及到rootfs数据的读写，因为还没挂载成功呢。

3.经过同事提醒，是否设置了MTD的block device支持。所以去内核源码处，查看配置：

make ARCH=arm menuconfig

在device drivers -> Memory Technology Device (MTD) support --->

发现里面有个选项：

Caching block device access to MTD devices

经过看其help：

Memory Technology Device (MTD) support -------------------------------------------+
| CONFIG_MTD:                                                                                                                 | 
|                                                                                                                             | 
| Memory Technology Devices are flash, RAM and similar chips, often                                                           | 
| used for solid state file systems on embedded devices. This option                                                          | 
| will provide the generic support for MTD drivers to register                                                                | 
| themselves with the kernel and for potential users of MTD devices                                                           | 
| to enumerate the devices which are present and obtain a handle on                                                           | 
| them. It will also allow you to select individual drivers for                                                               | 
| particular hardware and users of MTD devices. If unsure, say N.

和网上查找：
|### [7.1 MTD-Supported Devices](http://book.opensourceproject.org.cn/embedded/oreillybuildembed/opensource/belinuxsys-chp-7-sect-1.html)|
|----|

Caching block device access to MTD devices, CONFIG_MTD_BLOCK

This is the configuration option for the read-write block device MTD user module that is visible as /dev/mtdblockN. If you configure this as a module, the module's filename is mtdblock.o.

得知，这个就是对应的，mtd的块设备的选项，因此，选上，重新编译，就可以加入mtd 块设备的支持了。

也就可以访问上面的/dev/mtdblcok2了。

【后记】

1.经过实际测试发现，rootfs中的/dev/mtdblockN，还是有关系的，其系统访问此mtd块设备，还是会去访问此文件的，/dev/mtdblockN会连接到底层对应的块设备，也就是这样是对的：

[root@linux-41lh root]$ls dev/mtd* -l
crw-r----- 1 root root 90, 0 2009-11-16 15:06 dev/mtd0
crw-r----- 1 root root 90, 2 2009-11-16 15:06 dev/mtd1
crw-r----- 1 root root 90, 4 2009-11-16 15:06 dev/mtd2
crw-r----- 1 root root 90, 6 2009-11-16 15:06 dev/mtd3
crwxr-xr-x 1 root root 90, 8 2009-11-16 15:06 dev/mtd4
brw-r----- 1 root root 31, 0 2009-11-16 15:06 dev/mtdblock0
brw-r----- 1 root root 31, 1 2009-11-16 15:06 dev/mtdblock1
brw-r----- 1 root root 31, 2 2009-11-16 15:06 dev/mtdblock2
brw-r----- 1 root root 31, 3 2009-11-16 15:06 dev/mtdblock3
brwxr-xr-x 1 root root 31, 4 2009-11-16 15:06 dev/mtdblock4

而上面我们看到的：

-rw-r----- 1 eric develop 0 2010-01-12 13:58 dev/mtdblock2

这个明显不是设备文件，所以是不正常的。

因此，需要生成对应的设备文件放到rootfs里面，然后重新执着roofs镜像文件（此处我这里是用mkyaffs2image去对root/文件夹及其下面的文件，制作出来yaffs2的rootfs文件的）。

### nfs 挂载根文件系统

最近在实验从nfs服务器上挂载根文件系统。我用的redhat-2.6.18的内核，重新编译内核后。在启动过程中出现以下错误信息：

。。。。。
looking up port of RPC 100003/2 on 192.168.1.2
portmap: RPC call retured error 101
Root-NFS: Unable to get nfsd port number from server, using default
looking up port of RPC 100005/1 on 192.168.1.2
portmap: RPC call retured error 101
Root-NFS: Unable to get mountd port number from server, using default
mount: RPC call returned error 101
Root-NFS: Unable to get mountd port number from server, using default
mount: RPC call returned error 101
Root-NFS:Server returned error -101 while mounting /tftpboot/nfs
VFS: Unable to mount root fs via NFS, trying floppy
VFS: Cannot open root device "nfs" or unknown-block(2,0)
Please append a correct "root=" boot option
kernel panic - not syncing : VFS : Unable to mount root fs on unknown-block(2,0)

我的default 文件配置如下
default local
timeout 100
prompt 1
display msgs/boot.msg
F1 msgs/boot.msg
F2 msgs/general.msg
F3 msgs/expert.msg
F4 msgs/param.msg
F5 msgs/rescue.msg
F7 msgs/snake.msg

label local
   localboot 1

label 0
   localboot 1



label 1
   kernel ../boot/bzImage
   append root=/dev/nfs rw nfsroot=192.168.1.2:/tftpboot/nfs   ip=dhcp

nfs配置文件如下
/tftpboot   *(rw,no_root_squash,sync)

内核编译文件中
CONFIG_ROOT_NFS=Y
CONFIG_IP_PNP=Y
CONFIG_IP_PNP_DHCP=Y
CONFIG_IP_PNP_BOOTP=Y
CONFIG_IP_PNP_RARP=Y

是因为内核中没有编译网卡的驱动，只要在内核中编译进相应的网卡驱动就好了



问题描述：使用SAM9X25  内核版本是2.6.39 ，采用NFS文件系统， 在启动内核时会出现VFS: Cannot open root device "nfs" or unknown-block(0,255)错误

解决办法：step1，在内核配置时候文件系统中选中Root file system on NFS，这个选项depend on IP_PNP,发现IP_PNP在/net/ipv4/Kconfig中定义，故此项也要选中。

                   step2，uboot配置时需要关闭自动配置项，如下关闭eth0的IP自动配置

setenv bootargs root=/dev/nfs rw nfsroot=10.10.150.177:/home/wuer/nfs 

ip=10.10.150.180:10.10.150.177:10.10.150.20:255.255.255.0::eth0::off  console=ttyS0,115200

这样NFS就挂载成功了









