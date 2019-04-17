# mkfs 的使用 - DoubleLi - 博客园






使用方法:


`[root@localhost beinan]# mkfs -t 文件系统  存储设备`

注：
这里的文件系统是要指定的，比如 ext3 ；reiserfs ；ext2 ；fat32 ；msdos 等... ...
设备比如是一个硬盘的分区，软盘，光驱等.. ... 在格式化分区之前，您得懂得如何查看硬盘分区情况，并有针对性的格式化；比如用 fdisk -l 来查看； 请参考：[《Linux 查看磁盘分区、文件系统、使用情况的命令和相关工具介绍》](http://www.linuxsir.org/main/?q=node/41) 比如我想格式化一个移动U盘中的一个分区；全景应该是：


```
[root@localhost beinan]# fdisk -l

Disk /dev/hda: 80.0 GB, 80026361856 bytes
255 heads, 63 sectors/track, 9729 cylinders
Units = cylinders of 16065 * 512 = 8225280 bytes

Device Boot      Start         End      Blocks   Id  System
/dev/hda1   *           1         765     6144831    7  HPFS/NTFS
/dev/hda2             766        2805    16386300    c  W95 FAT32 (LBA)
/dev/hda3            2806        9729    55617030    5  Extended
/dev/hda5            2806        3825     8193118+  83  Linux
/dev/hda6            3826        5100    10241406   83  Linux
/dev/hda7            5101        5198      787153+  82  Linux swap / Solaris
/dev/hda8            5199        6657    11719386   83  Linux
/dev/hda9            6658        7751     8787523+  83  Linux
/dev/hda10           7752        9729    15888253+  83  Linux

Disk /dev/sda: 1035 MB, 1035730944 bytes
256 heads, 63 sectors/track, 125 cylinders
Units = cylinders of 16128 * 512 = 8257536 bytes

Device Boot      Start         End      Blocks   Id  System
/dev/sda1               1          25      201568+  83  Linux
/dev/sda2              26         125      806400    5  Extended
/dev/sda5              26          50      201568+  83  Linux
/dev/sda6              51          76      200781     83  Linux
```


我们可以看到有sda这个设备，所以可以用 fdisk -l /dev/sda专门来显示他的分区情况；比如我想格式化 /dev/sda6 分区为 ext3文件系统，则为：


```
[root@localhost beinan]# mkfs -t ext3  /dev/sda6
mke2fs 1.37 (21-Mar-2005)
Filesystem label=
OS type: Linux
Block size=1024 (log=0)
Fragment size=1024 (log=0)
50200 inodes, 200780 blocks
10039 blocks (5.00%) reserved for the super user
First data block=1
Maximum filesystem blocks=67371008
25 block groups
8192 blocks per group, 8192 fragments per group
2008 inodes per group
Superblock backups stored on blocks:
8193, 24577, 40961, 57345, 73729

Writing inode tables: done
Creating journal (4096 blocks): done
Writing superblocks and filesystem accounting information:  注：在这里直接回车；
done

This filesystem will be automatically checked every 26 mounts or
180 days, whichever comes first.  Use tune2fs -c or -i to override.
```


这样就格式化好了，sda6现在就是ext3文件系统了；我们就可以用mount 加载这个分区，然后使用这个文件系统；



```
[root@localhost beinan]# mkdir /mnt/sda6 
[root@localhost beinan]# chmod 777 /mnt/sda6 
[root@localhost beinan]# mount /dev/sda6   /mnt/sda6
```


当然您也可以把分区格式化成其它的文件系统；比如我们把 /dev/sda6格式化为ext3 、ext2、reiserfs、fat32、msdos 文件系统，命令格式如下；


```
[root@localhost beinan]# mkfs -t ext3  /dev/sda6 
[root@localhost beinan]# mkfs -t ext2  /dev/sda6
[root@localhost beinan]# mkfs -t reiserfs  /dev/sda6
[root@localhost beinan]# mkfs -t fat32   /dev/sda6
[root@localhost beinan]# mkfs -t msdos   /dev/sda6
... ...
```



2）mkfs.ext3 mkfs.reiserfs mkfs.ext2 mkfs.msdos mkfs.vfat mke2fs 的介绍；
我们先说了一个mkfs 工具后，我们再来介绍 mkfs.ext3 mkfs.reiserfs mkfs.ext2 mkdosfs mkfs.msdos mkfs.vfat ，其实mkfs 在执行的命令的时候，也是调用的这个工具，这也是我先把mkfs介绍的主要原因； 通过文件名，我们就知道这些工具是支持什么文件系统；这些命令为我们提供了更多的方便；




```
[root@localhost beinan]# mkfs.ext3    /dev/sda6     注：把该设备格式化成ext3文件系统
[root@localhost beinan]# mke2fs -j   /dev/sda6       注：把该设备格式化成ext3文件系统
[root@localhost beinan]# mkfs.ext2  /dev/sda6       注：把该设备格式化成ext2文件系统
root@localhost beinan]# mke2fs    /dev/sda6          注：把该设备格式化成ext2文件系统 
[root@localhost beinan]# mkfs.reiserfs  /dev/sda6   注：把该设备格式化成reiserfs文件系统
[root@localhost beinan]# mkfs.vfat   /dev/sda6        注：把该设备格式化成fat32文件系统
[root@localhost beinan]# mkfs.msdos   /dev/sda6   注：把该设备格式化成fat16文件系统,msdos文件系统就是fat16；
[root@localhost beinan]# mkdosfs   /dev/sda6         注：把该设备格式化成fat16文件系统，同mkfs.msdos 
... ...
```





2）mkswap 把一个分区格式化成为swap交换区；




```
[root@localhost beinan]# mkswap /dev/sda6  注：创建此分区为swap 交换分区 
[root@localhost beinan]# swapon  /dev/sda6  注：加载交换分区；
[root@localhost beinan]# swapoff  /dev/sda6  注：关闭交换分区；
```


我们查看系统已经加载的swap交换分区；


```
[root@localhost beinan]# swapon  /dev/sda6  注：加载交换分区；
[root@localhost beinan]# swapon -s                
Filename                                Type            Size    Used    Priority
/dev/hda7                               partition       787144  0       -1
/dev/sda6                               partition       225144  0       -3
<code>

为什么我的系统有两个交换分区？因为我用移动U盘做的实验，主要是为写教程之用；sda6是我在U盘上建的swap分区；

如果让swap开机就加载，应该改 /etc/fstab文件，加类似如下一行；

<code>
/dev/sda6         swap                    swap    defaults        0 0  注：把此行中的/dev/hda7 改为您的交换分区就行；
```


或者把命令行直接写入 /etc/rc.d/rc.local（ubuntu系统为/etc/rc.local）中也行；


`swapon  /dev/sda6 `

如果您的硬盘不能再分区，您可以创建一个swap文件


```
[root@localhost beinan]# dd if=/dev/zero of=/tmp/swap bs=1024 count=524288 注：创建一个大小为512M 的swap 文件，在/tmp目录中；您可以根据自己的需要的大小来创建swap 文件；

读入了 524288+0 个块
输出了 524288+0 个块
[root@localhost beinan]# mkswap /tmp/swap 注：把/tmp/swap 文件，创建成swap 交换区 
Setting up swapspace version 1, size = 536866 kB
no label, UUID=d9d8645d-92cb-4d33-b36e-075bb0a2e278
[root@localhost beinan]# swapon /tmp/swap   注：挂载swap 

[root@localhost beinan]# swapon -s
Filename Type Size Used Priority
/dev/hda7 partition 787144 888 -1
/tmp/swap file 524280 0 -2
```


注意：其实我们在安装系统的时候，就已经划分了交换分区；查看/etc/fstab，应该swap的行；如果您在安装系统时没有添加swap，可以通过这种办法来添加









