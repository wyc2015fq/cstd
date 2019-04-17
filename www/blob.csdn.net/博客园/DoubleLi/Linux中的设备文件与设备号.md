# Linux中的设备文件与设备号 - DoubleLi - 博客园






## 设备文件与设备号



在Linux下，一切皆文件，设备也不例外，为了管理这些设备，系统为它们各自都编了号，而每个[设备号](http://www.lenky.info/archives/tag/%e8%ae%be%e5%a4%87%e5%8f%b7)又分为[主设备号](http://www.lenky.info/archives/tag/%e4%b8%bb%e8%ae%be%e5%a4%87%e5%8f%b7)和[次设备号](http://www.lenky.info/archives/tag/%e6%ac%a1%e8%ae%be%e5%a4%87%e5%8f%b7)。主设备号用来区分不同类型的设备，而次设备号用来区分同一类型内的多个设备（及其设备分区）。
一个Linux系统，当前所有注册设备的主设备号可以通过/proc接口查看：
[root@localhost lenky]# cat /proc/devices
Character devices:
  1 mem
  4 /dev/vc/0
  4 tty
  4 ttyS
  5 /dev/tty
  5 /dev/console
  5 /dev/ptmx
  7 vcs
 10 misc
 13 input
 14 sound
 21 sg
 29 fb
 99 ppdev
116 alsa
128 ptm
136 pts
162 raw
180 usb
189 usb_device
202 cpu/msr
203 cpu/cpuid
251 hidraw
252 usbmon
253 bsg
254 rtc

Block devices:
  1 ramdisk
  2 fd
259 blkext
  7 loop
  8 sd
  9 md
 11 sr
 65 sd
 66 sd
 67 sd
 68 sd
 69 sd
 70 sd
 71 sd
128 sd
129 sd
130 sd
131 sd
132 sd
133 sd
134 sd
135 sd
253 device-mapper
254 mdp
[root@localhost lenky]#

字符设备与块设备的主设备号并不冲突，所有两个都可以有主设备号为1的设备，如果要继续查看次设备号，那么可以通过直接ls -l来查看，比如查看主设备号为8的设备的次设备号：
[root@localhost lenky]# ls -Rl /dev/* | grep " 8,"
brw-rw----. 1 root disk      8,   0 Jan 12 06:24 /dev/sda
brw-rw----. 1 root disk      8,   1 Jan 12 06:24 /dev/sda1
brw-rw----. 1 root disk      8,   2 Jan 12 06:24 /dev/sda2
brw-rw----. 1 root disk      8,  16 Jan 12 06:25 /dev/sdb
brw-rw----. 1 root disk      8,  17 Jan 12 06:25 /dev/sdb1
brw-rw----. 1 root disk      8,  32 Jan 12 06:29 /dev/sdc
brw-rw----. 1 root disk      8,  33 Jan 12 06:29 /dev/sdc1
brw-rw----. 1 root disk      8,  34 Jan 12 06:29 /dev/sdc2
brw-rw----. 1 root disk      8,  35 Jan 12 06:29 /dev/sdc3
[root@localhost lenky]#

上面的0，1，2，16，17等都是次设备号，用于区分标记各个sd硬盘或分区。查看系统所有的块设备：
[root@localhost lenky]# grep ^ /sys/class/block/*/dev
/sys/class/block/dm-0/dev:253:0
/sys/class/block/dm-1/dev:253:1
/sys/class/block/dm-2/dev:253:2
/sys/class/block/fd0/dev:2:0
/sys/class/block/loop0/dev:7:0
/sys/class/block/loop1/dev:7:1
/sys/class/block/loop2/dev:7:2
/sys/class/block/loop3/dev:7:3
/sys/class/block/loop4/dev:7:4
/sys/class/block/loop5/dev:7:5
/sys/class/block/loop6/dev:7:6
/sys/class/block/loop7/dev:7:7
/sys/class/block/ram0/dev:1:0
/sys/class/block/ram10/dev:1:10
/sys/class/block/ram11/dev:1:11
/sys/class/block/ram12/dev:1:12
/sys/class/block/ram13/dev:1:13
/sys/class/block/ram14/dev:1:14
/sys/class/block/ram15/dev:1:15
/sys/class/block/ram1/dev:1:1
/sys/class/block/ram2/dev:1:2
/sys/class/block/ram3/dev:1:3
/sys/class/block/ram4/dev:1:4
/sys/class/block/ram5/dev:1:5
/sys/class/block/ram6/dev:1:6
/sys/class/block/ram7/dev:1:7
/sys/class/block/ram8/dev:1:8
/sys/class/block/ram9/dev:1:9
/sys/class/block/sda1/dev:8:1
/sys/class/block/sda2/dev:8:2
/sys/class/block/sda/dev:8:0
/sys/class/block/sdb1/dev:8:17
/sys/class/block/sdb/dev:8:16
/sys/class/block/sdc1/dev:8:33
/sys/class/block/sdc2/dev:8:34
/sys/class/block/sdc3/dev:8:35
/sys/class/block/sdc/dev:8:32
/sys/class/block/sr0/dev:11:0
[root@localhost lenky]#

关于每个主设备号:次设备号对应设备的功能在Linux帮助文档里可以找到：[http://lxr.linux.no/#linux+v2.6.38.8/Documentation/devices.txt](http://lxr.linux.no/#linux+v2.6.38.8/Documentation/devices.txt)

在内核2.6.9之后，Linux系统上出现了一种名为device-mapper的存储映射机制，这种机制的作用简单来说就是给用户提供简单方便而又丰富的存储管理接口，在这种机制以及相关工具的帮助下，用户能够方便的自定义存储资源管理策略。

通过一些映射规则，device-mapper机制能够从原有的物理磁盘或逻辑磁盘中划分映射出新的逻辑磁盘，可以看到这是一个递归的映射机制，理论上可无限迭代。举个例子，系统有物理磁盘A和B，从物理磁盘A中映射出新的逻辑磁盘C、D、E，从物理磁盘B中映射出新的逻辑磁盘F、G，又可以从物理磁盘A和逻辑磁盘F中映射出新的逻辑磁盘H，等等。关于这方面，请参考：[http://www.ibm.com/developerworks/cn/linux/l-devmapper/](http://www.ibm.com/developerworks/cn/linux/l-devmapper/)、[http://sources.redhat.com/dm/](http://sources.redhat.com/dm/)等资源，不管是原物理磁盘还是通过device-mappe机制映射出来的新逻辑磁盘，在Linux操作系统看来都一样，一切皆文件，复杂逻辑被隔离在底部。

我系统使用的dm磁盘如下：
[root@localhost lenky]# ls -l /dev/dm-*
brw-rw----. 1 root disk 253, 0 Jan 12 06:24 /dev/dm-0
brw-rw----. 1 root disk 253, 1 Jan 12 06:24 /dev/dm-1
brw-rw----. 1 root disk 253, 2 Jan 12 06:24 /dev/dm-2
[root@localhost lenky]# ls -l /dev/mapper/*
crw-rw----. 1 root root 10, 236 Jan 12 06:24 /dev/mapper/control
lrwxrwxrwx. 1 root root       7 Jan 12 06:24 /dev/mapper/VolGroup-lv_home -> ../dm-2
lrwxrwxrwx. 1 root root       7 Jan 12 06:24 /dev/mapper/VolGroup-lv_root -> ../dm-0
lrwxrwxrwx. 1 root root       7 Jan 12 06:24 /dev/mapper/VolGroup-lv_swap -> ../dm-1
[root@localhost lenky]#

转载请保留地址：[http://www.lenky.info/archives/2012/02/1141](http://www.lenky.info/archives/2012/02/1141) 或[http://lenky.info/?p=1141](http://lenky.info/?p=1141)



**附加笔记：**


[root@localhost lenky]# ls -Rl /dev/* | grep " 8,"
brw-rw----. 1 root disk      8,   0 Jan 12 06:24 /dev/sda
brw-rw----. 1 root disk      8,   1 Jan 12 06:24 /dev/sda1
brw-rw----. 1 root disk      8,   2 Jan 12 06:24 /dev/sda2
一般说的各个分区相加等于硬盘，有个隐含说明就是硬盘内各个分区的硬盘存储空间容量相加等于硬盘的硬盘存储空间容量。



一个硬盘有一个描述硬盘的信息，而一个分区有一描述分区的信息，将硬盘内各个分区的描述分区的信息拼接在一起也得不到关于硬盘的信息，所以给硬盘配上一个次设备号是有必要不多余的。


[root@localhost lenky]# ls -Rl /dev/* | grep " 8,"
brw-rw----. 1 root disk      8,   0 Jan 12 06:24 /dev/sda
brw-rw----. 1 root disk      8,   1 Jan 12 06:24 /dev/sda1
brw-rw----. 1 root disk      8,   2 Jan 12 06:24 /dev/sda2
brw-rw----. 1 root disk      8,  16 Jan 12 06:25 /dev/sdb
brw-rw----. 1 root disk      8,  17 Jan 12 06:25 /dev/sdb1
brw-rw----. 1 root disk      8,  32 Jan 12 06:29 /dev/sdc
brw-rw----. 1 root disk      8,  33 Jan 12 06:29 /dev/sdc1
brw-rw----. 1 root disk      8,  34 Jan 12 06:29 /dev/sdc2
brw-rw----. 1 root disk      8,  35 Jan 12 06:29 /dev/sdc3
我们看到/dev/sd*设备名的设备类型（即指的是IDE硬盘），这里有三个不同的IDE硬盘，其主设备号以及其分区的主设备号都是8。









