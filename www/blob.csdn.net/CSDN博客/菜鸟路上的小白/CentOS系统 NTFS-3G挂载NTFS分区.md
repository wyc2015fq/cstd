# CentOS系统 NTFS-3G挂载NTFS分区 - 菜鸟路上的小白 - CSDN博客





2016年09月06日 15:00:19[somnus_小凯](https://me.csdn.net/u012486840)阅读数：5386








CentOS系统中挂载硬盘是非常的方便了，当然怎么方便也没有windows方便了，下面我来介绍在CentOS系统使用NTFS-3G挂载NTFS分区的步骤与方法，希望文章对各位朋友会有所帮助。



首先必须安装了rpmforge软件库的源

1、下载rpmforge的rpm文件包

32位系统

wget http://pkgs.repoforge.org/rpmforge-release/rpmforge-release-0.5.2-1.el6.rf.i686.rpm

64位系统

wget http://pkgs.repoforge.org/rpmforge-release/rpmforge-release-0.5.2-2.el6.rf.x86_64.rpm

2、安装rpmforge的rpm文件包

rpm -ivh rpmforge-release-0.5.2-1.el6.rf.i686.rpm

3、安装ntfs-3g

yum install fuse-ntfs-3g

4、挂载目录

mount -t ntfs-3g /dev/sdb1 /windows_dir


**挂载NTFS文件系统**

假设你的NTFS文件系统是 /dev/sda1 而你将会把它挂载在 /mnt/Windows/SYSTEM，请执行以下步骤。

1. 创建挂载点。（一般建议挂在mnt目录里）

mkdir /mnt/Windows/SYSTEM

2. 挂载NTFS分区：

mount -t ntfs-3g /dev/sda1 /mnt/Windows/SYSTEM

注：这里的sda1是指SATA或SCSI硬盘的第一个分区，如果是老式的IDE硬盘，会是hda1，具体请用fdisk -l查看。如我的实验室机：

 [root@[CentOS](http://www.111cn.net/list-232/) ~]# fdisk -l

Disk /dev/sda: 160.0 GB, 160041885696 bytes

255 heads, 63 sectors/track, 19457 cylinders

Units = cylinders of 16065 * 512 = 8225280 bytes

Sector size (logical/physical): 512 bytes / 512 bytes

I/O size (minimum/optimal): 512 bytes / 512 bytes

Disk identifier: 0xf0b1ebb0

Device Boot      Start         End      Blocks   Id  System

/dev/sda1   *           1        2550    20482843+   7  HPFS/NTFS

/dev/sda2            2551       19457   135805447    f  W95 Ext'd (LBA)

3. 如果想开机系统启动时自动挂载NTFS分区，编辑vim /etc/fstab文件，加入类似如下内容：

只读式挂载：/dev/sda1 /mnt/Windows/SYSTEM ntfs-3g ro,umask=0222,defaults 0 0

读写式挂载：/dev/sda1 /mnt/Windows/SYSTEM ntfs-3g rw,umask=0000,defaults 0 0

或者： /dev/sda1 /mnt/Windows/SYSTEM ntfs-3g defaults 0 0

**补充:**

另外一种是安装ntfs-3g，使用ntfs-3g挂载的分区就是可读写的了。可在终端下使用 yum install ntfs-3g 安装。如果找不到软件包，可能需要修改下更新源，编辑 /etc/yum.repos.d/CentOS-Base.repo ，将内容改为：

[base] 

name=CentOS-5 – Base 

#mirrorlist=http://mirrorlist.centos.org/?release=$releasever5&arch=$basearch&repo=os 

#baseurl=http://mirror.centos.org/centos/$releasever/os/$basearch/ 

baseurl=http://ftp.sjtu.edu.cn/centos/5.3/os/$basearch/ 

gpgcheck=1 

gpgkey=http://mirror.centos.org/centos/RPM-GPG-KEY-centos6

#released updates 

[update] 

name=CentOS-5 – Updates 

#mirrorlist=http://mirrorlist.centos.org/?release=4&arch=$basearch&repo=updates 

baseurl=http://ftp.sjtu.edu.cn/centos/5.3/updates/$basearch/ 

gpgcheck=1 

gpgkey=http://mirror.centos.org/centos/RPM-GPG-KEY-centos5

#packages used/produced in the build but not released 

[addons] 

name=CentOS-5 – Addons 

#mirrorlist=http://mirrorlist.centos.org/?release=4&arch=$basearch&repo=addons

baseurl=http://ftp.sjtu.edu.cn/centos/5.3/addons/$basearch/

好了到这里就挂载NTFS分区教程就结了，希望这篇[linux](http://www.111cn.net/list-156/)教程对各位会带来帮助哦。



