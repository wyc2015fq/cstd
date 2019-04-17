# Linux常见设备及相应/dev/xxx文件名、Mount Point、挂载点、Mount命令、fstab、挂载分区 - DoubleLi - 博客园






Linux 中的设备有2种类型：字符设备(无缓冲且只能顺序存取)、块设备(有缓冲且可以随机存取)。这些设备中，有些设备是对实际存在的物理硬件的抽象，而有些设备则是内核自身提供的功能(不依赖于特定的物理硬件，又称为"虚拟设备")。每个设备在 /dev 目录下都有一个对应的文件(节点)。 
**常见设备及相应/dev/下的文件名：**
- /dev/usb/hiddev0 | /dev/usb/hiddev1 --- USB 字符设备（鼠标/键盘/游戏杆/手写版等人操作计算机的设备）
- /dev/uba | /dev/ubb         --- USB 块设备(U盘之类)
- /dev/sda | /dev/sdb         --- 第n个 SCSI 磁盘(整个磁盘)
- /dev/hda | /dev/hdb         --- 第n个 IDE 磁盘(整个磁盘)
- /dev/sdc1 | /dev/sdc2       --- 第3个 SCSI 磁盘的第n个 分区  Linux/i386来说，分区1-4是主分区，5-15是逻辑分区。
- /dev/scd0 | /dev/scd1       --- 第n个 SCSI CD-ROM
- /dev/tty0 | /dev/tty1       --- 当前虚拟控制台、第n个虚拟控制台 TTY(终端)设备


Linux系统Mount点：mount的时候，mount的目标文件夹/media/cdrom要手动创建 
mount /dev/cdrom /media/cdrom 

输入不带参数的mount命令，显示所以已经挂载好的文件系统 

**Mount命令：**挂载(mount)光盘镜像文件、移动硬盘、U盘以及Windows网络共享和UNIX NFS网络共享  
命令格式：mount [-t vfstype] [-o options] device dir 
其中： 
　　1.-t vfstype 指定文件系统的类型，通常不必指定。mount 会自动选择正确的类型。常用类型有： 
- 光盘或光盘镜像：iso9660
- DOS fat16文件系统：msdos
- Windows 9x fat32文件系统：vfat
- Windows NT ntfs文件系统：ntfs
- Mount Windows文件网络共享：smbfs
- UNIX(LINUX) 文件网络共享：nfs


　　2.-o options 主要用来描述设备或档案的挂接方式。常用的参数有： 
- loop：用来把一个文件当成硬盘分区挂接上系统
- ro：采用只读方式挂接设备
- rw：采用读写方式挂接设备
- iocharset：指定访问文件系统所用字符集


　　3.device 要挂载(mount)的设备。 
　　4.dir设备在系统上的挂接点(mount point)。 

**/etc/fstab:**
![](http://dl.iteye.com/upload/attachment/0073/2780/6ebf91ee-00ce-3c35-ab23-349f5847563b.png)
这个文件是系统启动时，需要挂载的各个分区。 
- 第一列就是分区的label；
- 第二列是挂载点；
- 第三列是分区的格式；
- 第四列则是mount的一些挂载参数，等下会详细介绍一下有哪些参数，一般情况下，直接写defaults即可；
- 第五列的数字表示是否被dump备份，是的话这里就是1，否则就是0；
- 第六列是开机时是否自检磁盘，就是刚才讲过的那个fsck检测。1，2都表示检测，0表示不检测，在Redhat中，这个1，2还有个说法，/ 分区必须设为1，而且整个fstab中只允许出现一个1，这里有一个优先级的说法。1比2优先级高，所以先检测1，然后再检测2，如果有多个分区需要开机检测那么都设置成2吧，1检测完了后会同时去检测2。下面该说说第四列中常用到的参数了。


- async/sync ：async表示和磁盘和内存不同步，系统每隔一段时间把内存数据写入磁盘中，而sync则会时时同步内存和磁盘中数据；
- auto/noauto ：开机自动挂载/不自动挂载；
- default：按照大多数永久文件系统的缺省值设置挂载定义，它包含了rw, suid, dev, exec, auto, nouser,async ；
- ro：按只读权限挂载 ；
- rw：按可读可写权限挂载 ；
- exec/noexec ：允许/不允许可执行文件执行，但千万不要把根分区挂载为noexec，那就无法使用系统了，连mount命令都无法使用了，这时只有重新做系统了；
- user/nouser ：允许/不允许root外的其他用户挂载分区，为了安全考虑，请用nouser ；
- suid/nosuid ：允许/不允许分区有suid属性，一般设置nosuid ；
- usrquota ：启动使用者磁盘配额模式，磁盘配额相关内容在后续章节会做介绍；
- grquota ：启动群组磁盘配额模式；


学完这个/etc/fstab后，我们就可以自己修改这个文件，增加一行来挂载新增分区。例如，笔者增加了这样一行 
/dev/hdb1 /test1 ext3 defaults 0 0 
那么系统再重启时就会挂载这个分区了、或者直接运行mount -a 来挂载你增加的那行，这样就不用重启啦。









