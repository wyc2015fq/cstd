# linux的mount命令详解 - maopig的专栏 - CSDN博客
2011年09月10日 10:45:47[maopig](https://me.csdn.net/maopig)阅读数：685标签：[linux																[windows																[system																[磁盘																[disk																[dell](https://so.csdn.net/so/search/s.do?q=dell&t=blog)](https://so.csdn.net/so/search/s.do?q=disk&t=blog)](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux](https://blog.csdn.net/maopig/article/category/850267)
linux下挂载（mount）光盘镜像文件、移动硬盘、U盘、Windows和NFS网络共享
linux是一个优秀的开放源码的操作系统，可以运行在大到巨型小到掌上型各类计算机系统上，随着 linux系统的日渐成熟和稳定以及它开放源代码特有的优越性，linux在全世界得到了越来越广泛的应用。现在许多企业的计算机系统都是由UNIX系 统、Linux系统和Windows系统组成的混合系统，不同系统之间经常需要进行数据交换。下面我根据自己的实际工作经验介绍一下如何在linux系统 下挂接(mount)光盘镜像文件、移动硬盘、U盘以及Windows网络共享和UNIX NFS网络共享。   
**挂接命令(mount)**
　　首先，介绍一下挂接(mount)命令的使用方法，mount命令参数非常多，这里主要讲一下今天我们要用到的。
　　命令格式：
　　mount [-t vfstype] [-o options] device dir
　　其中：
　　1.-t vfstype 指定文件系统的类型，通常不必指定。mount 会自动选择正确的类型。常用类型有：
　　光盘或光盘镜像：iso9660
　　DOS fat16文件系统：msdos
　　Windows 9x fat32文件系统：vfat
　　Windows NT ntfs文件系统：ntfs
　　Mount Windows文件网络共享：smbfs
　　UNIX(LINUX) 文件网络共享：nfs
　　2.-o options 主要用来描述设备或档案的挂接方式。常用的参数有：
　　loop：用来把一个文件当成硬盘分区挂接上系统
　　ro：采用只读方式挂接设备
　　rw：采用读写方式挂接设备
　　iocharset：指定访问文件系统所用字符集
　　3.device 要挂接(mount)的设备。
　　4.dir设备在系统上的挂接点(mount point)。
**挂接光盘镜像文件**
　　由于近年来磁盘技术的巨大进步，新的电脑系统都配备了大容量的磁盘系统，在Windows下许多人都习惯把软件和资料做成光盘镜像文件通过虚拟 光驱来使用。这样做有许多好处：一、减轻了光驱的磨损;二、现在硬盘容量巨大存放几十个光盘镜像文件不成问题，随用随调十分方便;三、硬盘的读取速度要远 远高于光盘的读取速度，CPU占用率大大降低。其实linux系统下制作和使用光盘镜像比Windows系统更方便，不必借用任何第三方软件包。
　　1、从光盘制作光盘镜像文件。将光盘放入光驱，执行下面的命令。
　　　#cp /dev/cdrom /home/sunky/mydisk.iso 或
　　　#dd if=/dev/cdrom of=/home/sunky/mydisk.iso
　　　注：执行上面的任何一条命令都可将当前光驱里的光盘制作成光盘镜像文件/home/sunky/mydisk.iso
　　2、将文件和目录制作成光盘镜像文件，执行下面的命令。
　　　#mkisofs -r -J -V mydisk -o /home/sunky/mydisk.iso /home/sunky/ mydir
　　　注：这条命令将/home/sunky/mydir目录下所有的目录和文件制作成光盘镜像文件/home/sunky/mydisk.iso，光盘卷标为：mydisk
　　3、光盘镜像文件的挂接(mount)
　　　#mkdir /mnt/vcdrom
　　　注：建立一个目录用来作挂接点(mount point)
　　　#mount -o loop -t iso9660 /home/sunky/mydisk.iso /mnt/vcdrom
　　　注：使用/mnt/vcdrom就可以访问盘镜像文件mydisk.iso里的所有文件了。
**　挂接移动硬盘**
　　对linux系统而言，USB接口的移动硬盘是当作SCSI设备对待的。插入移动硬盘之前，应先用fdisk –l 或 more /proc/partitions查看系统的硬盘和硬盘分区情况。
　　[root at pldyrouter /]# fdisk -l
　　Disk /dev/sda: 73 dot 4 GB, 73407820800 bytes
　　255 heads, 63 sectors/track, 8924 cylinders
　　Units = cylinders of 16065 * 512 = 8225280 bytes
　　Device Boot Start End Blocks Id System
　　/dev/sda1 1 4 32098+ de Dell Utility
　　/dev/sda2 * 5 2554 20482875 7 HPFS/NTFS
　　/dev/sda3 2555 7904 42973875 83 Linux
　　/dev/sda4 7905 8924 8193150 f Win95 Ext'd (LBA)
　　/dev/sda5 7905 8924 8193118+ 82 Linux swap
　　在这里可以清楚地看到系统有一块SCSI硬盘/dev/sda和它的四个磁盘分区/dev/sda1 -- /dev/sda4, /dev/sda5是分区/dev/sda4的逻辑分区。接好移动硬盘后，再用fdisk –l 或 more /proc/partitions查看系统的硬盘和硬盘分区情况
　　[root at pldyrouter /]# fdisk -l
　　Disk /dev/sda: 73 dot 4 GB, 73407820800 bytes
　　255 heads, 63 sectors/track, 8924 cylinders
　　Units = cylinders of 16065 * 512 = 8225280 bytes
　　Device Boot Start End Blocks Id System
　　/dev/sda1 1 4 32098+ de Dell Utility
　　/dev/sda2 * 5 2554 20482875 7 HPFS/NTFS
　　/dev/sda3 2555 7904 42973875 83 Linux
　　/dev/sda4 7905 8924 8193150 f Win95 Ext'd (LBA)
　　/dev/sda5 7905 8924 8193118+ 82 Linux swap
　　Disk /dev/sdc: 40.0 GB, 40007761920 bytes
　　255 heads, 63 sectors/track, 4864 cylinders
　　Units = cylinders of 16065 * 512 = 8225280 bytes
　　Device Boot Start End Blocks Id System
　　/dev/sdc1 1 510 4096543+ 7 HPFS/NTFS
　　/dev/sdc2 511 4864 34973505 f Win95 Ext'd (LBA)
　　/dev/sdc5 511 4864 34973473+ b Win95 FAT32
　　大家应该可以发现多了一个SCSI硬盘/dev/sdc和它的两个磁盘分区/dev/sdc1?、/dev/sdc2,其中/dev/sdc5是/dev/sdc2分区的逻辑分区。我们可以使用下面的命令挂接/dev/sdc1和/dev/sdc5。
　　　#mkdir -p /mnt/usbhd1
　　　#mkdir -p /mnt/usbhd2
　　　注：建立目录用来作挂接点(mount point)
　　　#mount -t ntfs /dev/sdc1 /mnt/usbhd1
　　　#mount -t vfat /dev/sdc5 /mnt/usbhd2
　　　注：对ntfs格式的磁盘分区应使用-t ntfs 参数，对fat32格式的磁盘分区应使用-t vfat参数。若汉字文件名显示为乱码或不显示，可以使用下面的命令格式。
　　　#mount -t ntfs -o iocharset=cp936 /dev/sdc1 /mnt/usbhd1
　　　#mount -t vfat -o iocharset=cp936 /dev/sdc5 /mnt/usbhd2
　　linux系统下使用fdisk分区命令和mkfs文件系统创建命令可以将移动硬盘的分区制作成linux系统所特有的ext2、ext3格式。这样，在linux下使用就更方便了。使用下面的命令直接挂接即可。
　　　#mount /dev/sdc1 /mnt/usbhd1
**　挂接U盘**
　　和USB接口的移动硬盘一样对linux系统而言U盘也是当作SCSI设备对待的。使用方法和移动硬盘完全一样。插入U盘之前，应先用fdisk –l 或 more /proc/partitions查看系统的硬盘和硬盘分区情况。
　　[root at pldyrouter root]# fdisk -l
　　Disk /dev/sda: 73 dot 4 GB, 73407820800 bytes
　　255 heads, 63 sectors/track, 8924 cylinders
　　Units = cylinders of 16065 * 512 = 8225280 bytes
　　Device Boot Start End Blocks Id System
　　/dev/sda1 1 4 32098+ de Dell Utility
　　/dev/sda2 * 5 2554 20482875 7 HPFS/NTFS
　　/dev/sda3 2555 7904 42973875 83 Linux
　　/dev/sda4 7905 8924 8193150 f Win95 Ext'd (LBA)
　　/dev/sda5 7905 8924 8193118+ 82 Linux swap
　　插入U盘后，再用fdisk –l 或 more /proc/partitions查看系统的硬盘和硬盘分区情况。
　　[root at pldyrouter root]# fdisk -l
　　Disk /dev/sda: 73 dot 4 GB, 73407820800 bytes
　　255 heads, 63 sectors/track, 8924 cylinders
　　Units = cylinders of 16065 * 512 = 8225280 bytes
　　Device Boot Start End Blocks Id System
　　/dev/sda1 1 4 32098+ de Dell Utility
　　/dev/sda2 * 5 2554 20482875 7 HPFS/NTFS
　　/dev/sda3 2555 7904 42973875 83 Linux
　　/dev/sda4 7905 8924 8193150 f Win95 Ext'd (LBA)
　　/dev/sda5 7905 8924 8193118+ 82 Linux swap
　　Disk /dev/sdd: 131 MB, 131072000 bytes
　　9 heads, 32 sectors/track, 888 cylinders
　　Units = cylinders of 288 * 512 = 147456 bytes
　　Device Boot Start End Blocks Id System
　　/dev/sdd1 * 1 889 127983+ b Win95 FAT32
　　Partition 1 has different physical/logical endings:
　　phys=(1000, 8, 32) logical=(888, 7, 31)
　　系统多了一个SCSI硬盘/dev/sdd和一个磁盘分区/dev/sdd1,/dev/sdd1就是我们要挂接的U盘。
　　#mkdir -p /mnt/usb
　　注：建立一个目录用来作挂接点(mount point)
　　#mount -t vfat /dev/sdd1 /mnt/usb
　　注：现在可以通过/mnt/usb来访问U盘了, 若汉字文件名显示为乱码或不显示，可以使用下面的命令。
　　#mount -t vfat -o iocharset=cp936 /dev/sdd1 /mnt/usb
