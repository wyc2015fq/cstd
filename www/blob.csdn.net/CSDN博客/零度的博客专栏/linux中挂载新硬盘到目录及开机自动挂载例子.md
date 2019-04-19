# linux中挂载新硬盘到目录及开机自动挂载例子 - 零度的博客专栏 - CSDN博客
2017年02月16日 13:32:43[零度anngle](https://me.csdn.net/zmx729618)阅读数：3193
                
挂载硬盘对于linux系统来讲是一个比较常见的问题了，下面我们来看看linux中挂载新硬盘到目录，并开机自动挂载例子紧，   
今天，在整理图片时，发现根目录硬盘空间已经所剩无几了，而网站和所有相关文件主要放置在根下data目录（/data）里，但/home下是有空间，而又不想把文件放到/home下面去，只有再加一个新硬盘，对以后/data目录的文件管理也比较好。
那么就先关掉主机，再加上新硬盘（其它电脑使用过的500G硬盘），插上后再启动电脑。
接下来，就开始工作了：
1. 查看硬盘信息：
feigo@figo:~$ sudo df -h
文件系统                                                容量  已用  可用 已用% 挂载点
rootfs                                                  102G   92G  4.7G   96% /
udev                                                     10M     0   10M    0% /dev
tmpfs                                                   799M  1.8M  798M    1% /run
/dev/disk/by-uuid/3640f24a-4870-42e2-ae07-109cb9c35164  102G   92G  4.7G   96% /
tmpfs                                                   5.0M     0  5.0M    0% /run/lock
tmpfs                                                   4.6G  224K  4.6G    1% /run/shm
/dev/sda7                                                92M   26M   62M   30% /boot
/dev/sda9                                               201G   23G  169G   12% /home
feigo@figo:~$ sudo fdisk -l
Disk /dev/sdb: 500.1 GB, 500107862016 bytes
255 heads, 63 sectors/track, 60801 cylinders, total 976773168 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0x3f1e271a
   Device Boot      Start         End      Blocks   Id  System
/dev/sdb1              63   102414374    51207156    7  HPFS/NTFS/exFAT
/dev/sdb2       102414375   976768064   437176845    f  W95 Ext'd (LBA)
/dev/sdb5       102414438   409625369   153605466    b  W95 FAT32
/dev/sdb6       409625433   716836364   153605466    b  W95 FAT32
/dev/sdb7       716836428   976768064   129965818+   7  HPFS/NTFS/exFAT
Disk /dev/sda: 500.1 GB, 500107862016 bytes
255 heads, 63 sectors/track, 60801 cylinders, total 976773168 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0xc35aebd4
   Device Boot      Start         End      Blocks   Id  System
/dev/sda1   *          63   104872319    52436128+   7  HPFS/NTFS/exFAT
/dev/sda2       104872381   976771071   435949345+   f  W95 Ext'd (LBA)
/dev/sda5       104872383   231962534    63545076    7  HPFS/NTFS/exFAT
/dev/sda6       231962598   300929579    34483491    b  W95 FAT32
/dev/sda7       300931072   301125631       97280   83  Linux
/dev/sda8       301127680   517726207   108299264   83  Linux
/dev/sda9       517728256   945461247   213866496   83  Linux
/dev/sda10      945463296   976771071    15653888   82  Linux swap / Solaris
我们可以看到有两个硬盘，sda和sdb，其中sda是目前使用中的硬盘，sdb是新加上去的硬盘。
2. 针对sdb创建新硬盘分区：
先说明一下，fdisk 可以用 m 命令来看 fdisk 命令的内部命令：
a ：命令指定启动分区
d ：命令删除一个存在的分区
l ：命令显示分区 ID 号的列表
m ：查看 fdisk 命令帮助
n ：命令创建一个新分区
p ：命令显示分区列表
t ：命令修改分区的类型 ID 号
w ：命令是将对分区表的修改存盘让它发生作用
feigo@figo:~$ su
密码：
root@figo:/home/feigo# fdisk /dev/sdb
Command (m for help): m
Command action
   a   toggle a bootable flag
   b   edit bsd disklabel
   c   toggle the dos compatibility flag
   d   delete a partition
   l   list known partition types
   m   print this menu
   n   add a new partition
   o   create a new empty DOS partition table
   p   print the partition table
   q   quit without saving changes
   s   create a new empty Sun disklabel
   t   change a partition's system id
   u   change display/entry units
   v   verify the partition table
   w   write table to disk and exit
   x   extra functionality (experts only)
由于sdb硬盘上已经有几个分区，人们需要先删除这些分区：
Command (m for help): d                # 删除分区，依次输入数字
Partition number (1-7): 7
Command (m for help): d
Partition number (1-6): 6
Command (m for help): d
Partition number (1-5): 5
Command (m for help): d
Partition number (1-5): 2
Command (m for help): d
Selected partition 1
Command (m for help): w                    # 保存
The partition table has been altered!
Calling ioctl() to re-read partition table.
Syncing disks.
再次查看硬盘分区信息，将会看到所有分区已被删除：
root@figo:/home/feigo# fdisk -l
Disk /dev/sdb: 500.1 GB, 500107862016 bytes
255 heads, 63 sectors/track, 60801 cylinders, total 976773168 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0x3f1e271a
   Device Boot      Start         End      Blocks   Id  System
Disk /dev/sda: 500.1 GB, 500107862016 bytes
255 heads, 63 sectors/track, 60801 cylinders, total 976773168 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0xc35aebd4
   Device Boot      Start         End      Blocks   Id  System
/dev/sda1   *          63   104872319    52436128+   7  HPFS/NTFS/exFAT
/dev/sda2       104872381   976771071   435949345+   f  W95 Ext'd (LBA)
/dev/sda5       104872383   231962534    63545076    7  HPFS/NTFS/exFAT
/dev/sda6       231962598   300929579    34483491    b  W95 FAT32
/dev/sda7       300931072   301125631       97280   83  Linux
/dev/sda8       301127680   517726207   108299264   83  Linux
/dev/sda9       517728256   945461247   213866496   83  Linux
/dev/sda10      945463296   976771071    15653888   82  Linux swap / Solaris
在之前的命令列表上面看到，n代表创建新硬盘分区，那么接下我们输入n命令来创建新分区：
root@figo:/home/feigo# fdisk /dev/sdb
Command (m for help): n
Partition type:
   p   primary (0 primary, 0 extended, 4 free)
   e   extended
Select (default p): p
Partition number (1-4, default 1): 1
First sector (2048-976773167, default 2048): 
Using default value 2048
Last sector, +sectors or +size{K,M,G} (2048-976773167, default 976773167): 
Using default value 976773167
Command (m for help): w
The partition table has been altered!
Calling ioctl() to re-read partition table.
Syncing disks.
完成，再查看，已经发现多了一个sdb1的分区：
root@figo:/home/feigo# fdisk -l
Disk /dev/sdb: 500.1 GB, 500107862016 bytes
81 heads, 63 sectors/track, 191411 cylinders, total 976773168 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0x3f1e271a
   Device Boot      Start         End      Blocks   Id  System
/dev/sdb1            2048   976773167   488385560   83  Linux
Disk /dev/sda: 500.1 GB, 500107862016 bytes
255 heads, 63 sectors/track, 60801 cylinders, total 976773168 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0xc35aebd4
   Device Boot      Start         End      Blocks   Id  System
/dev/sda1   *          63   104872319    52436128+   7  HPFS/NTFS/exFAT
/dev/sda2       104872381   976771071   435949345+   f  W95 Ext'd (LBA)
/dev/sda5       104872383   231962534    63545076    7  HPFS/NTFS/exFAT
/dev/sda6       231962598   300929579    34483491    b  W95 FAT32
/dev/sda7       300931072   301125631       97280   83  Linux
/dev/sda8       301127680   517726207   108299264   83  Linux
/dev/sda9       517728256   945461247   213866496   83  Linux
/dev/sda10      945463296   976771071    15653888   82  Linux swap / Solaris
3. 格式化分区为ext4：
root@figo:/home/feigo# mkfs.ext4 /dev/sdb1
mke2fs 1.42.5 (29-Jul-2012)
文件系统标签=
OS type: Linux
块大小=4096 (log=2)
分块大小=4096 (log=2)
Stride=0 blocks, Stripe width=0 blocks
30531584 inodes, 122096390 blocks
6104819 blocks (5.00%) reserved for the super user
第一个数据块=0
Maximum filesystem blocks=4294967296
3727 block groups
32768 blocks per group, 32768 fragments per group
8192 inodes per group
Superblock backups stored on blocks: 
 32768, 98304, 163840, 229376, 294912, 819200, 884736, 1605632, 2654208, 
 4096000, 7962624, 11239424, 20480000, 23887872, 71663616, 78675968, 
 102400000
Allocating group tables: 完成                            
正在写入inode表: 完成                            
Creating journal (32768 blocks): 完成
Writing superblocks and filesystem accounting information: 完成
格式化完成，可以使用parted命令查看/dev/sdb的分区格式：
root@figo:/home/feigo# parted
GNU Parted 2.3
Using /dev/sda
Welcome to GNU Parted! Type 'help' to view a list of commands.
(parted) help                                                             
  align-check TYPE N                        check partition N for TYPE(min|opt) alignment
  check NUMBER                             do a simple check on the file system
  cp [FROM-DEVICE] FROM-NUMBER TO-NUMBER   copy file system to another partition
  help [COMMAND]                           print general help, or help on COMMAND
  mklabel,mktable LABEL-TYPE               create a new disklabel (partition table)
  mkfs NUMBER FS-TYPE                      make a FS-TYPE file system on partition NUMBER
  mkpart PART-TYPE [FS-TYPE] START END     make a partition
  mkpartfs PART-TYPE FS-TYPE START END     make a partition with a file system
  move NUMBER START END                    move partition NUMBER
  name NUMBER NAME                         name partition NUMBER as NAME
  print [devices|free|list,all|NUMBER]     display the partition table, available devices, free space, all found partitions, or a particular partition
  quit                                     exit program
  rescue START END                         rescue a lost partition near START and END
  resize NUMBER START END                  resize partition NUMBER and its file system
  rm NUMBER                                delete partition NUMBER
  select DEVICE                            choose the device to edit
  set NUMBER FLAG STATE                    change the FLAG on partition NUMBER
  toggle [NUMBER [FLAG]]                   toggle the state of FLAG on partition NUMBER
  unit UNIT                                set the default unit to UNIT
  version                                  display the version number and copyright information of GNU Parted
(parted) select /dev/sdb                                                  
Using /dev/sdb
(parted) print                                                            
Model: ATA ST3500413AS (scsi)
Disk /dev/sdb: 500GB
Sector size (logical/physical): 512B/512B
Partition Table: msdos
Number  Start   End    Size   Type     File system  Flags
 1      1049kB  500GB  500GB  primary  ext4
(parted) quit
其中上面parted的内部命令：
help 代表帮助，将会打印所有内部命令
select /dev/sdb 代表切换到硬盘sdb
print 代表打印分区文件系统类型
quit 代表退出
那么我们已经知道了sdb已被格式化为ext4，接下来就要挂载硬盘来目录了：
root@figo:/home/feigo# mkdir /data1
root@figo:/home/feigo# mount /dev/sdb1 /data1
root@figo:/home/feigo# df -Th
文件系统                                               类型      容量  已用  可用 已用% 挂载点
rootfs                                                 rootfs    102G   93G  4.5G   96% /
udev                                                   devtmpfs   10M     0   10M    0% /dev
tmpfs                                                  tmpfs     799M  3.0M  796M    1% /run
/dev/disk/by-uuid/3640f24a-4870-42e2-ae07-109cb9c35164 ext4      102G   93G  4.5G   96% /
tmpfs                                                  tmpfs     5.0M     0  5.0M    0% /run/lock
tmpfs                                                  tmpfs     4.6G  376K  4.6G    1% /run/shm
/dev/sda7                                              ext4       92M   26M   62M   30% /boot
/dev/sda9                                              ext4      201G   23G  169G   12% /home
/dev/sdb1                                              ext4      459G  198M  435G    1% /data1
可以看到/dev/sdb1已经被挂载到/data1下了，剩余的工作就是迁移数据了。
4. 最后一步就是实现开机自动挂载/dev/sdb1到/data1下：
编辑/etc/fstab文件，加入下面两行内容：
root@figo:/home/feigo# vi /etc/fstab
# load /dev/sdb1 to /data1 when the system startup
/dev/sdb1 /data1                                          ext4    defaults        1       2
然后再用mount -a确定/etc/fstab设置没有错误，再用mount查看已挂载的设备：
root@figo:/home/feigo# mount -a
root@figo:/home/feigo# mount
sysfs on /sys type sysfs (rw,nosuid,nodev,noexec,relatime)
proc on /proc type proc (rw,nosuid,nodev,noexec,relatime)
udev on /dev type devtmpfs (rw,relatime,size=10240k,nr_inodes=1020978,mode=755)
devpts on /dev/pts type devpts (rw,nosuid,noexec,relatime,gid=5,mode=620,ptmxmode=000)
tmpfs on /run type tmpfs (rw,nosuid,noexec,relatime,size=818060k,mode=755)
/dev/disk/by-uuid/3640f24a-4870-42e2-ae07-109cb9c35164 on / type ext4 (rw,relatime,errors=remount-ro,user_xattr,barrier=1,data=ordered)
tmpfs on /run/lock type tmpfs (rw,nosuid,nodev,noexec,relatime,size=5120k)
tmpfs on /run/shm type tmpfs (rw,nosuid,nodev,noexec,relatime,size=4766880k)
/dev/sda7 on /boot type ext4 (rw,relatime,user_xattr,barrier=1,data=ordered)
/dev/sda9 on /home type ext4 (rw,relatime,user_xattr,barrier=1,data=ordered)
rpc_pipefs on /var/lib/nfs/rpc_pipefs type rpc_pipefs (rw,relatime)
binfmt_misc on /proc/sys/fs/binfmt_misc type binfmt_misc (rw,nosuid,nodev,noexec,relatime)
/dev/sdb1 on /data1 type ext4 (rw,relatime,user_xattr,barrier=1,data=ordered)
