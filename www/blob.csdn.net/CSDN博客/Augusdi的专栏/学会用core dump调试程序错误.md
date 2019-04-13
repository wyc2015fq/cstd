
# 学会用core dump调试程序错误 - Augusdi的专栏 - CSDN博客


2015年04月15日 14:17:28[Augusdi](https://me.csdn.net/Augusdi)阅读数：848


﻿﻿
# 前言
最近在项目中遇到大型程序出现SIGSEGV ，一直不知道用core dump工具来调试程序，花了近一周的时间，才定位问题，老大很生气，后果很严重，呵呵，事后仔细学习了这块的知识，了解一点core dump的知识。
在使用半导体作为内存的材料前，人类是利用线圈当作内存的材料（发明者为王安），线圈就叫作core ，用线圈做的内存就叫作“core memory”。（线圈的单词应该是coil，呵呵）如今，半导体工业澎勃发展，已经没有人用线圈当内存了，不过，在许多情况下，人们还是把内存叫作“core”。 所以注意了：这里的core不是核心，而是内存。不过结合实际来看，好像也有点“内核所占内存”的意思。
core dump又是什么东东？ 我们在开发（或使用）一个程序时，最怕的就是程序莫明其妙地挂掉。虽然系统没事，但我们下次仍可能遇到相同的问题。于是，这时操作系统就会把程序挂掉时的 内存内容写入一个叫做core的文件里（这个写入的动作就叫dump，dump的英语意思是垃圾、倾倒。从这里来看，这些内存的内容是程序错误运行的结果，所以算是垃圾，把他弄出来就好比从大的内存池里“倾倒”。），以便于我们调试。这个过程，因此叫做core dump.在嵌入式系统中，有时core dump直接从串口打印出来，结合objdump查找ra和epa地址，运用栈回溯，可以找到程序出错的地方。

# 如何产生core dump文件
在一般Linux系统中，默认是不会产生core dump文件的，通过ulimit -c来查看core dump文件的大小，一般开始是0，可以设置core文件大小，ulimit -c 1024(kbytes单位)或者ulimit -c unlimited。
# core dump文件输出设置
一般默认是当前目录，可以在/proc/sys/kernel中找`到core-user-pid，通过`
1.使core文件名加上pid号
echo"1"> /proc/sys/kernel/core-user-pid
2.控制core文件保存位置和文件名格式
mkdir -p /root/corefile
echo"/root/corefile/core-%e-%p-%t"> /proc/sys/kernel/core-pattern
`以下是参数列表:`
`%p - insert pid into filename 添加pid`
`%u - insert current uid into filename 添加当前uid`
`%g - insert current gid into filename 添加当前gid`
`%s - insert signal that caused the coredump into the filename 添加导致产生core的信号`
`%t - insert UNIX time that the coredump occurred into filename 添加core文件生成时的unix时间`
`%h - insert hostname where the coredump happened into filename 添加主机名`
`%e - insert coredumping executable name into filename 添加命令名`

# 用gdb查看core文件
`下面我们可以在发生运行时信号引起的错误时发生core dump了.编译时加上-g`
`发生core dump之后, 用gdb进行查看core文件的内容, 以定位文件中引发core dump的行.`
gdb [exec file] [core file]
`如:`
gdb ./test test.core
`在进入gdb后, 用bt命令查看backtrace以检查发生程序运行到哪里, 来定位core dump的文件行.`
# 实例讲解
//test.cvoida()
{char*p = NULL;
   printf("%d/n", *p);
}intmain()
{
    a();return0;
}

编译
gcc -g -o test test.c
运行 ./test
报segmentation fault(core dump)
如果生成的是test.core.
gdb ./test test.core

[http://www.cnblogs.com/ggjucheng/archive/2011/12/20/2294300.html](http://www.cnblogs.com/ggjucheng/archive/2011/12/20/2294300.html)


