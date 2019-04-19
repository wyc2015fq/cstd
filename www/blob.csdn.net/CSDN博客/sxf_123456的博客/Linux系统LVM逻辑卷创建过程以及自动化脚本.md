# Linux系统LVM逻辑卷创建过程以及自动化脚本 - sxf_123456的博客 - CSDN博客
2017年11月28日 13:25:48[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：481
最近在上海新建机房的时候，给了我2台M2机型服务器，在做初始化的时候发现有一堆磁盘：

``
|12345678910111213|[root@Centos64:~]# fdisk -l | grep '300.1 GB' | sortDisk/dev/sda:300.1GB,300069052416bytesDisk/dev/sdb:300.1GB,300069052416bytesDisk/dev/sdc:300.1GB,300069052416bytesDisk/dev/sdd:300.1GB,300069052416bytesDisk/dev/sde:300.1GB,300069052416bytesDisk/dev/sdf:300.1GB,300069052416bytesDisk/dev/sdg:300.1GB,300069052416bytesDisk/dev/sdh:300.1GB,300069052416bytesDisk/dev/sdi:300.1GB,300069052416bytesDisk/dev/sdj:300.1GB,300069052416bytesDisk/dev/sdk:300.1GB,300069052416bytesDisk/dev/sdl:300.1GB,300069052416bytes|
其中挂载的只有 /dev/sda，其他都在那闲着。运管那边告诉我这个机型不能做raid。而根据我这边的业务需求，我并不能一次用到这么多分区，所以必须使用LVM合并使用。
关于LVM的创建，目前网络上一堆详细教程，用起来也非常简单，这里就只贴一下我的过程。
①、用fdisk给每一个磁盘创建一个8e分区：

``
|1234567891011121314151617181920212223242526|[root@Centos64:~]# fdisk /dev/sdbThedevice presentsalogical sectorsize thatissmaller thanthe physicalsector size.Aligning toaphysical sector(oroptimalI/O)size boundaryisrecommended,orperformance maybe impacted.WARNING:DOS-compatiblemode isdeprecated.It's strongly recommended to         switch off the mode (command 'c') and change display units to         sectors (command 'u').Command(mforhelp):n#创建Command actioneextendedpprimary partition(1-4)p#选择主分区Partition number(1-4):1Firstcylinder(1-36481,default1):1Last cylinder,+cylindersor+size{K,M,G}(1-36481,default36481):#直接回车选择最大Usingdefaultvalue36481Command(mforhelp):t#输入t进入类型选择Selected partition1Hexcode(typeLtolist codes):8e#输入8e格式Changed systemtype ofpartition1to8e(Linux LVM)Command(mforhelp):w#w保存|
这样就完成了一块磁盘，接着我们依次将其他 sdc到sdl的磁盘也操作一把。
②、全部完成后，使用 pvcreate 将所有分区转化成物理卷，即添加LVM属性信息并划分PE存储单元：

``
|12345678910111213|[root@Centos64:~]# pvcreate /dev/sdb1 /dev/sdc1 /dev/sdd1 /dev/sde1 /dev/sdf1 /dev/sdg1 /dev/sdh1 /dev/sdi1 /dev/sdj1 /dev/sdk1 /dev/sdl1Physical volume"/dev/sdb1"successfully createdPhysical volume"/dev/sdc1"successfully createdPhysical volume"/dev/sdd1"successfully createdPhysical volume"/dev/sde1"successfully createdPhysical volume"/dev/sdf1"successfully createdPhysical volume"/dev/sdg1"successfully createdPhysical volume"/dev/sdh1"successfully createdPhysical volume"/dev/sdi1"successfully createdPhysical volume"/dev/sdj1"successfully createdPhysical volume"/dev/sdk1"successfully createdPhysical volume"/dev/sdl1"successfully created|
创建完PV之后，就可以使用 pvdisplay 或 pvs 查看详细信息了，篇幅有限，这里我就不贴了。
③、下面我们需要创建一个VG，然后PV都加入到这个卷组当中，卷名可自定义，比如 vg：

``
|123|[root@Centos64:~]# vgcreate vg /dev/sdb1 /dev/sdc1 /dev/sdd1 /dev/sde1 /dev/sdf1 /dev/sdg1 /dev/sdh1 /dev/sdi1 /dev/sdj1 /dev/sdk1 /dev/sdl1Volumegroup"vg"successfully created|
同样，在创建好VG以后，我们也可以使用 vgdisplay 或者 vgs 命来来查看VG的信息（略）
④、接着，使用 lvcreate 命令基于VG创建逻辑卷，vg 和 lvm 我们自定义的名称：

``
|123|[root@Centos64:~]# lvcreate -l 100%VG -n lvm vgLogical volume"lvm"created|
同样我们可以使用 lvdisplay 或者 lvs 命令来查看创建好的逻辑卷的信息（略）
⑤、格式化创建的逻辑卷：

``
|1|[root@Centos64:~]# mkfs.ext4 /dev/vg/lvm|
⑥、挂载分区：

``
|12345678910|[root@Centos64:~]# mkdir /data[root@Centos64:~]# echo '/dev/vg/lvm  /data  ext4 noatime,acl,user_xattr  1 2' >> /dev/fstab[root@Centos64:~]# mount -a[root@Centos64:~]# df -hFilesystem            Size  Used Avail Use%Mounted on/dev/sda19.9G2.1G7.4G22%//dev/sda320G370M19G2%/usr/localtmpfs16G016G0%/dev/shm/dev/sda4244G205M232G1%/data1/dev/mapper/vg-lvm3.1T932M3.0T1%/data|
这样，我们就完成了LVM的手工创建过程了，我还有一台M2要弄，而且听运管说以后会继续交付这类机型，我可不想这么苦逼的操作了，光那个创建8e格式分区就已经很坑了。
所以，就将上面的操作串成脚本，一键完成：

Shell
``
|12345678910111213141516171819202122232425262728293031|#!/bin/bashpartition=/data# 定义最终挂载的名称vgname=vg# 定义逻辑卷组的名称lvname=lvm# 定义逻辑卷的名称code='b c d e f g h i k j l'# 根据分区的实际情况修改disk=foriin$codedofdisk/dev/sd$i<<EOF# 这里自动化完成了所有分区fdisk苦逼的交互步骤np11t8ewEOFdisk="$disk /dev/sd${i}1"# 将所有分区拼起来donepvcreate$diskvgcreate$vgname$disklvcreate-l100%VG-n$lvmname$vgnamemkfs.ext4/dev/$vgname/$lvmnamemkdir-p$partitionecho"/dev/$vgname/$lvmname$partitionext4noatime,acl,user_xattr12'>>/dev/fstabmount-adf-h|
哦了，就写这么多，以备后用。
