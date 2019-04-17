# linux下dd命令详解及应用实例 - DoubleLi - 博客园






名称: dd
使用权限: 任何使用者dd 这个指令在 manual 里的定义是 convert and copy a file
使用方式:
dd [option]
查看帮助说明
dd --help
或是
info dd

查看版本:
dd --version
输入或输出
dd if=[STDIN] of=[STDOUT]
强迫输入或输出的Size为多少Bytes
bs: dd -ibs=[BYTE] -obs=[SIZE]
强迫一次只做多少个 Bytes
cbs=BYTES
跳过一段以后才输出
seek=BLOCKS
跳过一段以后才输入
skip=BLOCKS
当然您能够拿这个来方便的拷贝光碟(注意，您的光碟是标准的 iso9660格式才能够这么做唷！)
dd if=/dev/cdrom of=cdrom.iso
其中 if 后面连同 of 后面的内容依您的需求调整。
然后给系统这个指令就能够烧了:
cdrecord -v cdrom.iso
这篇不是在讲 cdrecord 的，所以上面的指令是最为简单但是不一定能符合您的硬件环境...
功
能：把指定的输入文档拷贝到指定的输出文档中，并且在拷贝过程中能够进行格式转换。能够用该命令实现DOS下的diskcopy命令的作用。先用dd命令
把软盘上的数据写成硬盘的一个寄存文档，再把这个寄存文档写入第二张软盘上，完成diskcopy的功能。需要注意的是，应该将硬盘上的寄存文档用rm
命令删除掉。系统默认使用标准输入文档和标准输出文档。
语法：dd [选项]
if =输入文档（或设备名称）。
of =输出文档（或设备名称）。
ibs = bytes 一次读取bytes字节，即读入缓冲区的字节数。
skip = blocks 跳过读入缓冲区开头的ibs*blocks块。
obs = bytes 一次写入bytes字节，即写入缓冲区的字节数。
bs = bytes 同时配置读/写缓冲区的字节数（等于配置ibs和obs）。
cbs = byte 一次转换bytes字节。
count=blocks 只拷贝输入的blocks块。
conv = ASCII 把EBCDIC码转换为ASCIl码。
conv = ebcdic 把ASCIl码转换为EBCDIC码。
conv = ibm 把ASCIl码转换为alternate EBCDIC码。
conv = block 把变动位转换成固定字符。
conv = ublock 把固定位转换成变动位。
conv = ucase 把字母由小写转换为大写。
conv = lcase 把字母由大写转换为小写。
conv = notrunc 不截短输出文档。
conv = swab 交换每一对输入字节。
conv = noerror 出错时不停止处理。
conv = sync 把每个输入记录的大小都调到ibs的大小（用NUL填充）。

例1：要把一张软盘的内容拷贝到另一张软盘上，利用/tmp作为临时存储区。把源盘插入驱动器中，输入下述命令：
$ dd if =/dev/fd0 of = /tmp/tmpfile
拷贝完成后，将源盘从驱动器中取出，把目标盘插入，输入命令：
$ dd if = /tmp/tmpfile of =/dev/fd0
软盘拷贝完成后，应该将临时文档删除：
$ rm /tmp/tmpfile

例2：把net.i这个文档写入软盘中，并设定读/写缓冲区的数目。
（注意：软盘中的内容会被完全覆盖掉）
$ dd if = net.i of = /dev/fd0 bs = 16384



例3：商业数据毁尸灭迹

dd if=/dev/zero of=/dev/sda conv=notrunc



例4：制作镜像

dd if=/dev/hda1 of=hdimage.img conv=noerror,notrunc



例5：克隆硬盘

（这里以从虚拟机sda(2G大小)往一只U盘sdb(2G大小)克隆Fedora为例）

dd if=/dev/sda of=/dev/sdb notrunc,noerror

注：如果80G的硬盘(假设只有一个分区)克隆到250G的分区，250G的分区会变成80G，导致剩余的空间未分配。这种情况可以先用Partition Magic或者是Acronis Disk Director将较大的硬盘分好区



例6：网络克隆
目标机上运行 netcat -l -p 1234 | dd of=/dev/hdc bs=16065b
源机上运行dd if=/dev/hda bs=16065b | netcat targethost-192.168.3.33 1234

注：netcat堪称网络调试的瑞士军刀，详细请参考下文。





参考文章：

http://www.idcnews.net/html/edu/20070101/291368.html

http://www.linuxdiyf.com/bbs/thread-106225-1-1.html









