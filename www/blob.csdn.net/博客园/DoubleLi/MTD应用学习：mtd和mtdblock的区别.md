# MTD应用学习：mtd和mtdblock的区别 - DoubleLi - 博客园






[http://my.oschina.net/shelllife/blog/123482](http://my.oschina.net/shelllife/blog/123482)

[http://www.cnblogs.com/hnrainll/archive/2011/06/09/2076075.html](http://www.cnblogs.com/hnrainll/archive/2011/06/09/2076075.html)

今天做升级方案用到了mtd-utils中的flash_eraseall和flash_cp两个工具，在进行方案验证的时候，遭遇到各种不解和疑惑，因对MTD的原理不熟悉，所以只能多次尝试，虽然最后把方案搞定了，不过觉得MTD中的mtd和mtdblock区别这块还是值得总结学习一下。这里先说明一下问题现象，然后在进行具体的区别原理解释。



# MTD设备(Nor Flash)使用中的问题现象表现
- mtd-utils工具对mtd和mtdblock分区设备的区别处理



|`1`|`/ $ flash_eraseall /dev/mtdblock/2`|
|----|----|


|`2`|`flash_eraseall: /dev/mtdblock/2: unable to get MTD device info`|
|----|----|


|`3`|`/ $ flash_eraseall /dev/mtdblock/2`|
|----|----|


|`4`|`flash_eraseall: /dev/mtdblock/2: unable to get MTD device info`|
|----|----|


|`5`|`/ $ flash_eraseall /dev/mtd/2`|
|----|----|


|`6`|`Erasing 128 Kibyte @ 8e0000 -- 98 % complete.`|
|----|----|


|`7`|`/ $ ``ls`|
|----|----|






|`1`|`/ $ flashcp rootfs_version /dev/mtdblock2`|
|----|----|


|`2`|`This doesn't seem to be a valid MTD flash device!`|
|----|----|


|`3`|`/ $ flashcp rootfs_version /dev/mtdblock/2`|
|----|----|


|`4`|`This doesn't seem to be a valid MTD flash device!`|
|----|----|


|`5`|`/ $ flashcp rootfs_version /dev/mtd2`|
|----|----|


|`6`|`/ $ ``ls`|
|----|----|




- mtd和mtdblock分区设备mount时的区别



|`1`|`/ $ ``mount``-t jffs2 /dev/mtd/2 qqzm/`|
|----|----|


|`2`|`mount``: Mounting /dev/mtd/2 on qqzm/ failed: Invalid argument`|
|----|----|


|`3`|`/ $ ``mount``-t jffs2 /dev/mtd2 qqzm/`|
|----|----|


|`4`|`mount``: Mounting /dev/mtd2 on qqzm/ failed: Invalid argument`|
|----|----|


|`5`|`/ $ ``mount``-t jffs2 /dev/mtdblock/2 qqzm/`|
|----|----|


|`6`|`/ $ ``ls`|
|----|----|




- mtdblock挂载成功，单擦除后卸载失败



|`01`|`/ $ flash_eraseall /dev/mtd/2 <span></span> Erasing 128 Kibyte @ 8e0000 -- 98 % complete.`|
|----|----|


|`02`|`/qqzm $ ``mount`|
|----|----|


|`03`|`/dev/root on / ``type``jffs2 (rw,noatime)`|
|----|----|


|`04`|`proc on /proc ``type``proc (rw,nodiratime)`|
|----|----|


|`05`|`sysfs on /sys ``type``sysfs (rw)`|
|----|----|


|`06`|`devfs on /dev ``type``devfs (rw)`|
|----|----|


|`07`|`devpts on /dev/pts ``type``devpts (rw)`|
|----|----|


|`08`|`/dev/mmcblk0p1 on /mnt/sd ``type``vfat (rw,nodiratime,fmask=0022,dmask=0022,codepage=cp437,iocharset=iso8859-1)`|
|----|----|


|`09`|`/dev/mtdblock/2 on /qqzm ``type``jffs2 (rw,noatime)`|
|----|----|


|`10`|`none on /qqzm/www/cgi-bin/tmp ``type``ramfs (rw)`|
|----|----|


|`11`|`/qqzm $ ``cd``..`|
|----|----|


|`12`|`/ $ ``umount``/qqzm`|
|----|----|


|`13`|`umount``: Couldn't ``umount``/qqzm: Inappropriate ioctl ``for``device`|
|----|----|


|`14`|`/ $ ``umount``/dev/mtdblock/2`|
|----|----|


|`15`|`umount``: Couldn't ``umount``/dev/mtdblock/2: Inappropriate ioctl ``for``device`|
|----|----|


|`16`|`/ $`|
|----|----|







通过上面的不断尝试和错误反馈，我把方案基本验证通过了，只是对其中的原理不清楚：
- 为什么mtd和mtdblock明明是同一个设备分区却有不同的操作？
- mount命令只能挂载块设备吗？
- 卸载mtdblock设备时，Inappropriate ioctl for device是什么意思？
- unable to get MTD device info，又是什么意思？

# MTD技术的基本原理

MTD(memory technology device内存技术设备)是用于访问memory设备（ROM、flash）的Linux的子系统。MTD的主要目的是为了使新的memory设备的驱动更加简单，为此它在硬件和上层之间提供了一个抽象的接口，并进行了一个层次划分，层次从上到下大致为：设备文件、MTD设备层、MTD原始设备层、硬件驱动层。MTD的所有源代码在/drivers/mtd子目录下。

## 系统中的MTD设备文件



|`01`|`~ $ ``ls``/dev/mtd* -l`|
|----|----|


|`02`|`crw-rw----    1 root     root      90,   0 Jan  1 00:00 /dev/mtd0`|
|----|----|


|`03`|`crw-rw----    1 root     root      90,   1 Jan  1 00:00 /dev/mtd0ro`|
|----|----|


|`04`|`crw-rw----    1 root     root      90,   2 Jan  1 00:00 /dev/mtd1`|
|----|----|


|`05`|`crw-rw----    1 root     root      90,   3 Jan  1 00:00 /dev/mtd1ro`|
|----|----|


|`06`|`crw-rw----    1 root     root      90,   4 Jan  1 00:00 /dev/mtd2`|
|----|----|


|`07`|`crw-rw----    1 root     root      90,   5 Jan  1 00:00 /dev/mtd2ro`|
|----|----|


|`08`|`crw-rw----    1 root     root      90,   6 Jan  1 00:00 /dev/mtd3`|
|----|----|


|`09`|`crw-rw----    1 root     root      90,   7 Jan  1 00:00 /dev/mtd3ro`|
|----|----|


|`10`|`brw-rw----    1 root     root      31,   0 Jan  1 00:00 /dev/mtdblock0`|
|----|----|


|`11`|`brw-rw----    1 root     root      31,   1 Jan  1 00:00 /dev/mtdblock1`|
|----|----|


|`12`|`brw-rw----    1 root     root      31,   2 Jan  1 00:00 /dev/mtdblock2`|
|----|----|


|`13`|`brw-rw----    1 root     root      31,   3 Jan  1 00:00 /dev/mtdblock3`|
|----|----|


|`14`||
|----|----|


|`15`|`/dev/mtd:`|
|----|----|


|`16`|`crw-rw-rw-    1 root     root      90,   0 Jan  1 00:00 0`|
|----|----|


|`17`|`cr--r--r--    1 root     root      90,   1 Jan  1 00:00 0ro`|
|----|----|


|`18`|`crw-rw-rw-    1 root     root      90,   2 Jan  1 00:00 1`|
|----|----|


|`19`|`cr--r--r--    1 root     root      90,   3 Jan  1 00:00 1ro`|
|----|----|


|`20`|`crw-rw-rw-    1 root     root      90,   4 Jan  1 00:00 2`|
|----|----|


|`21`|`cr--r--r--    1 root     root      90,   5 Jan  1 00:00 2ro`|
|----|----|


|`22`|`crw-rw-rw-    1 root     root      90,   6 Jan  1 00:00 3`|
|----|----|


|`23`|`cr--r--r--    1 root     root      90,   7 Jan  1 00:00 3ro`|
|----|----|


|`24`||
|----|----|


|`25`|`/dev/mtdblock:`|
|----|----|


|`26`|`brw-------    1 root     root      31,   0 Jan  1 00:00 0`|
|----|----|


|`27`|`brw-------    1 root     root      31,   1 Jan  1 00:00 1`|
|----|----|


|`28`|`brw-------    1 root     root      31,   2 Jan  1 00:00 2`|
|----|----|


|`29`|`brw-------    1 root     root      31,   3 Jan  1 00:00 3`|
|----|----|


|`30`|`~ $`|
|----|----|




可以看到有mtdN和对应的/dev/mtd/N、mtdblockN和对应的/dev/mtdblock/N两类MTD设备，分别是字符设备，主设备号90和块设备，主设备号31。其中/dev/mtd0和/dev/mtd/0是完全等价的，/dev/mtdblock0和/dev/mtdblock/0是完全等价的，而/dev/mtd0和/dev/mtdblock0则是同一个MTD分区的两种不同应用描述，操作上是有区别的。

## /dev/mtdN设备

/dev/mtdN 是MTD架构中实现的mtd分区所对应的字符设备(将mtd设备分成多个区，每个区就为一个字符设备)，其里面添加了一些ioctl，支持很多命令，如MEMGETINFO，MEMERASE等。

mtd-utils中的flash_eraseall等工具，就是以这些ioctl为基础而实现的工具，实现一些关于Flash的操作。比如，mtd 工具中 flash_eraseall中：



|`1`|`if``(ioctl(fd, MEMGETINFO, &meminfo) != 0)`|
|----|----|


|`2`|`{`|
|----|----|


|`3`|`   ``fprintf``(stderr, ``"%s: %s: unable to get MTD device info\n"``,exe_name, mtd_device);`|
|----|----|


|`4`|`   ``return``1;`|
|----|----|


|`5`|`}`|
|----|----|




MEMGETINFO是Linux MTD中的drivers/mtd/mtdchar.c中的ioctl命令，使用mtd字符设备需要加载mtdchar内核模块。该代码解释了上面的第一个现象。

## /dev/mtdblockN设备

/dev/mtdblockN，是Flash驱动中用add_mtd_partitions()添加MTD设备分区，而生成的对应的块设备。MTD块设备驱动程序可以让flash器件伪装成块设备，实际上它通过把整块的erase block放到ram里面进行访问，然后再更新到flash，用户可以在这个块设备上创建通常的文件系统。

而对于MTD块设备，MTD设备层是不提供ioctl的实现方法的，也就不会有对应的MEMGETINFO命令之类，因此不能使用nandwrite,flash_eraseall,flash_erase等工具去对/dev/mtdblockN去进行操作，否则就会出现上面的现象一，同时也解释了现象3——用mtd2擦除分区后，在用mtdblock2进行umount就会造成混乱。

mtd块设备的大小可以通过proc文件系统进行查看：



|`01`|`~ $ ``cat``/proc/partitions`|
|----|----|


|`02`|`major minor  ``#blocks  name`|
|----|----|


|`03`||
|----|----|


|`04`|`  ``31     0        512 mtdblock0`|
|----|----|


|`05`|`  ``31     1       1024 mtdblock1`|
|----|----|


|`06`|`  ``31     2       5632 mtdblock2`|
|----|----|


|`07`|`  ``31     3       9216 mtdblock3`|
|----|----|


|`08`|` ``254     0   30760960 mmcblk0   `|
|----|----|


|`09`|` ``254     1   30756864 mmcblk0p1`|
|----|----|


|`10`|`~ $`|
|----|----|




后面的两个是SD块设备的分区大小。每个block的大小是1KB。

## MTD设备分区和总结

通过proc文件系统查看mtd设备的分区情况：



|`1`|`~ $ ``cat``/proc/mtd`|
|----|----|


|`2`|`dev:    size   erasesize  name`|
|----|----|


|`3`|`mtd0: 00080000 00020000 ``"boot"`|
|----|----|


|`4`|`mtd1: 00100000 00020000 ``"kernel"`|
|----|----|


|`5`|`mtd2: 00580000 00020000 ``"roofs70"`|
|----|----|


|`6`|`mtd3: 00900000 00020000 ``"app"`|
|----|----|


|`7`|`~ $`|
|----|----|




可以发现，实际上mtdN和mtdblockN描述的是同一个MTD分区，对应同一个硬件分区，两者的大小是一样的，只不过是MTD设备层提供给上层的视图不一样，给上层提供了字符和块设备两种操作视图——为了上层使用的便利和需要，比如mount命令的需求，你只能挂载块设备(有文件系统)，而不能对字符设备进行挂载，否则会出现上面的现象2:无效参数。

这里对于mtd和mtdblock设备的使用场景进行简单总结：
- mtd-utils工具只能应用与/dev/mtdN的MTD字符设备
- mount、umount命令只对/dev/mtdblockN的MTD块设备有效
- /dev/mtdN和/dev/mtdblockN是同一个MTD设备的同一个分区（N一样）









