# 每天一个 Linux 命令（25）：Linux 文件属性详解 - 文章 - 伯乐在线
原文出处： [peida](http://www.cnblogs.com/peida/archive/2012/11/23/2783762.html)
Linux 文件或目录的属性主要包括：文件或目录的节点、种类、权限模式、链接数量、所归属的用户和用户组、最近访问或修改的时间等内容。具体情况如下：
**命令：**
> 
ls -lih
**输出：**


```
[root@localhost test]# ls -lih
总计 316K
2095120 lrwxrwxrwx 1 root root   11 11-22 06:58 linklog.log -> log2012.log
2095112 -rw-r--r-- 1 root root 296K 11-13 06:03 log2012.log
2095110 -rw-r--r-- 1 root root   61 11-13 06:03 log2013.log
2095107 -rw-r--r-- 1 root root    0 11-13 06:03 log2014.log
2095117 -rw-r--r-- 1 root root    0 11-13 06:06 log2015.log
2095118 -rw-r--r-- 1 root root    0 11-16 14:41 log2016.log
2095119 -rw-r--r-- 1 root root    0 11-16 14:43 log2017.log
2095113 drwxr-xr-x 6 root root 4.0K 10-27 01:58 scf
2095109 drwxrwxr-x 2 root root 4.0K 11-13 06:08 test3
2095131 drwxrwxr-x 2 root root 4.0K 11-13 05:50 test4
```
**说明：**
第一列：inode
第二列：文件种类和权限；
第三列： 硬链接个数；
第四列： 属主；
第五列：所归属的组；
第六列：文件或目录的大小；
第七列和第八列：最后访问或修改时间；
第九列：文件名或目录名
我们以 log2012.log 为例：


```
2095112 -rw-r--r-- 1 root root 296K 11-13 06:03 log2012.log
```
inode 的值是：2095112
文件类型：文件类型是-，表示这是一个普通文件； 关于文件的类型，请参考：《[每天一个 Linux 命令（24）：Linux 文件类型与扩展名](http://blog.jobbole.com/109254/)》
文件权限：文件权限是 rw-r–r– ，表示文件属主可读、可写、不可执行，文件所归属的用户组不可写，可读，不可执行，其它用户不可写，可读，不可执行；
硬链接个数： log2012.log这个文件没有硬链接；因为数值是1，就是他本身；
文件属主：也就是这个文件归哪于哪个用户 ，它归于root，也就是第一个root；
文件属组：也就是说，对于这个文件，它归属于哪个用户组，在这里是root用户组；
文件大小：文件大小是296k个字节；
访问可修改时间 ：这里的时间是最后访问的时间，最后访问和文件被修改或创建的时间，有时并不是一致的；
当然文档的属性不仅仅包括这些，这些是我们最常用的一些属性。
**关于inode：**
inode 译成中文就是索引节点。每个存储设备或存储设备的分区（存储设备是硬盘、软盘、U盘等等）被格式化为文件系统后，应该有两部份，一部份是inode，另一部份是Block，Block是用来存储数据用的。而inode呢，就是用来存储这些数 据的信息，这些信息包括文件大小、属主、归属的用户组、读写权限等。inode为每个文件进行信息索引，所以就有了inode的数值。操作系统根据指令， 能通过inode值最快的找到相对应的文件。
做个比喻，比如一本书，存储设备或分区就相当于这本书，Block相当于书中的每一页，inode 就相当于这本书前面的目录，一本书有很多的内容，如果想查找某部份的内容，我们可以先查目录，通过目录能最快的找到我们想要看的内容。虽然不太恰当，但还是比较形象。
当我们用ls 查看某个目录或文件时，如果加上-i 参数，就可以看到inode节点了；比如我们前面所说的例子：


```
[root@localhost test]#  ls -li log2012.log
2095112 -rw-r--r-- 1 root root 302108 11-13 06:03 log2012.log
```
log2012.log 的inode值是 2095112 ； 查看一个文件或目录的inode，要通过ls 命令的的 -i参数。
### 本系列文章：
[每天一个 Linux 命令（1）：ls命令](http://blog.jobbole.com/108531/)
[每天一个 Linux 命令（2）：cd命令](http://blog.jobbole.com/108805/)
[每天一个 Linux 命令（3）：pwd命令](http://blog.jobbole.com/108855/)
[每天一个 Linux 命令（4）：mkdir命令](http://blog.jobbole.com/108871/)
[每天一个 Linux 命令（5）：rm 命令](http://blog.jobbole.com/108901/)
[每天一个 Linux 命令（6）：rmdir 命令](http://blog.jobbole.com/108925/)
[每天一个 Linux 命令（7）：mv命令](http://blog.jobbole.com/108983/)
[每天一个 Linux 命令（8）：cp 命令](http://blog.jobbole.com/108988/)
[每天一个 Linux 命令（9）：touch 命令](http://blog.jobbole.com/108989/)
[每天一个 Linux 命令（10）：cat 命令](http://blog.jobbole.com/108990/)
[每天一个 Linux 命令（11）：nl 命令](http://blog.jobbole.com/108991/)
[每天一个 Linux 命令（12）：more 命令](http://blog.jobbole.com/108992/)
[每天一个 Linux 命令（13）：less 命令](http://blog.jobbole.com/108993/)
[每天一个 Linux 命令（14）：head 命令](http://blog.jobbole.com/108994/)
[每天一个 Linux 命令（15）：tail 命令](http://blog.jobbole.com/108995/)
[每天一个 Linux 命令（16）：which命令](http://blog.jobbole.com/109262/)
[每天一个 Linux 命令（17）：whereis 命令](http://blog.jobbole.com/108993/)
[每天一个 Linux 命令（18）：locate 命令](http://blog.jobbole.com/108994/)
[每天一个 Linux 命令（19）：find 命令概览](http://blog.jobbole.com/109265/)
[每天一个 Linux 命令（20）：find命令之exec](http://blog.jobbole.com/108996/)
[每天一个 Linux 命令（21）：find命令之xargs](http://blog.jobbole.com/108997/)
[每天一个 Linux 命令（22）：find 命令的参数详解](http://blog.jobbole.com/109268/)
[每天一个 Linux 命令（23）：Linux 目录结构](http://blog.jobbole.com/109506/)
[每天一个 Linux 命令（24）：Linux 文件类型与扩展名](http://blog.jobbole.com/109254/)
