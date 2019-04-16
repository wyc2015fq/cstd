# 服务器数据库系列 - ulimit的优化和使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年12月08日 20:26:52[initphp](https://me.csdn.net/initphp)阅读数：1522








### ulimit概述

ulimit 是一种 linux 系统的内键功能，它具有一套参数集，用于为由它生成的 shell 进程及其子进程的资源使用设置限制。

例如我们的系统会经常碰上会遇上Socket/File: Can’t open so many files这样的问题，主要是由于Linux设置了文件句柄的限制值，默认情况下是1024，一旦服务器访问量上升，则就会报这个错误。




### ulimit命令参数


|-H|设置硬资源限制，一旦设置不能增加。|ulimit – Hs 64；限制硬资源，线程栈大小为 64K。|
|----|----|----|
|-S|设置软资源限制，设置后可以增加，但是不能超过硬资源设置。|ulimit – Sn 32；限制软资源，32 个文件描述符。|
|-a|显示当前所有的 limit 信息。|ulimit – a；显示当前所有的 limit 信息。|
|-c|最大的 core 文件的大小， 以 blocks 为单位。|ulimit – c unlimited； 对生成的 core 文件的大小不进行限制。|
|-d|进程最大的数据段的大小，以 Kbytes 为单位。|ulimit -d unlimited；对进程的数据段大小不进行限制。|
|-f|进程可以创建文件的最大值，以 blocks 为单位。|ulimit – f 2048；限制进程可以创建的最大文件大小为 2048 blocks。|
|-l|最大可加锁内存大小，以 Kbytes 为单位。|ulimit – l 32；限制最大可加锁内存大小为 32 Kbytes。|
|-m|最大内存大小，以 Kbytes 为单位。|ulimit – m unlimited；对最大内存不进行限制。|
|-n|可以打开最大文件描述符的数量。|ulimit – n 128；限制最大可以使用 128 个文件描述符。|
|-p|管道缓冲区的大小，以 Kbytes 为单位。|ulimit – p 512；限制管道缓冲区的大小为 512 Kbytes。|
|-s|线程栈大小，以 Kbytes 为单位。|ulimit – s 512；限制线程栈的大小为 512 Kbytes。|
|-t|最大的 CPU 占用时间，以秒为单位。|ulimit – t unlimited；对最大的 CPU 占用时间不进行限制。|
|-u|用户最大可用的进程数。|ulimit – u 64；限制用户最多可以使用 64 个进程。|
|-v|进程最大可用的虚拟内存，以 Kbytes 为单位。|ulimit – v 200000；限制最大可用的虚拟内存为 200000 Kbytes。|

如果要查看当前ulimit设置，则命令


`ulimit -a`




```
[root@localhost admin]# ulimit -a
core file size          (blocks, -c) 0
data seg size           (kbytes, -d) unlimited
scheduling priority             (-e) 0
file size               (blocks, -f) unlimited
pending signals                 (-i) 14573
max locked memory       (kbytes, -l) 64
max memory size         (kbytes, -m) unlimited
open files                      (-n) 1024
pipe size            (512 bytes, -p) 8
POSIX message queues     (bytes, -q) 819200
real-time priority              (-r) 0
stack size              (kbytes, -s) 8192
cpu time               (seconds, -t) unlimited
max user processes              (-u) 14573
virtual memory          (kbytes, -v) unlimited
file locks                      (-x) unlimited
```


### ulimit的使用

1. 修改文件描述符限制


为了解决最上面的Socket/File: Can’t open so many files，打开的文件描述符限制的问题，我们一般用以下命令来解决：


`ulimit -SHn 65535`

![](https://img-blog.csdn.net/20141208203626421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




2. 对进程的大小进行限制

一般情况下，这三个参数都设置为 unlimited



`ulimit -d 1024 -m 1024 -v 1024`


![](https://img-blog.csdn.net/20141208205503687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



3. 限制文件创建的大小

一般设置为unlimited



`ulimit -f 10`![](https://img-blog.csdn.net/20141208210319767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


限制后，如果我再去拷贝文件或者创建文件，就会失败：



```
[root@localhost memcache]# ulimit -f 10
[root@localhost memcache]# cp memcache-2.2.7.tgz memcache-test
文件大小超出限制(吐核)
```




### ulimit的长久生效设置

一般我们修改命令ulimit -SHn 65535后，并不能很快生效，这个时候我们需要编辑：/etc/security/limits.conf

其中 * 号代表所有用户。





```
* soft nofile 65536
* hard nofile 65536
* soft nproc 65536
* hard nproc 65536
```







