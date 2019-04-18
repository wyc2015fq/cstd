# 什么是Core Dump? - ljx0305的专栏 - CSDN博客
2009年04月11日 16:45:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：5552标签：[file																[unix																[bt																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=bt&t=blog)](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
今天调试一个程序, 用到了core dump, 于是写出来, 记于此.
**什么是Core Dump?**
Core的意思是内存, Dump的意思是扔出来, 堆出来.
开发和使用Unix程序时, 有时程序莫名其妙的down了, 却没有任何的提示(有时候会提示core dumped). 这时候可以查看一下有没有形如core.进程号的文件生成, 这个文件便是操作系统把程序down掉时的内存内容扔出来生成的, 它可以做为调试程序的参考.
core dump又叫核心转储, 当程序运行过程中发生异常, 程序异常退出时, 由操作系统把程序当前的内存状况存储在一个core文件中, 叫core dump.
**如何使用core文件?**
gdb -c core文件路径 [应用程序的路径]
进去后输入where回车, 就可以显示程序在哪一行当掉的, 在哪个函数中.
**为什么没有core文件生成呢?**
有时候程序down了, 但是core文件却没有生成. core文件的生成跟你当前系统的环境设置有关系, 可以用下面的语句设置一下, 然后再运行程序便成生成core文件.
ulimit -c unlimited
core文件生成的位置一般于运行程序的路径相同, 文件名一般为core.进程号
4. 用gdb查看core文件:
下面我们可以在发生运行时信号引起的错误时发生core dump了.
发生core dump之后, 用gdb进行查看core文件的内容, 以定位文件中引发core dump的行.
gdb [exec file] [core file]
如:
gdb ./test test.core
在进入gdb后, 用bt命令查看backtrace以检查发生程序运行到哪里, 来定位core dump的文件->行.
引用：[http://blog.chinaunix.net/u/16292/showart_1844400.html](http://blog.chinaunix.net/u/16292/showart_1844400.html)
