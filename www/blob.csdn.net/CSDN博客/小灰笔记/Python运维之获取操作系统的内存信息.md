# Python运维之获取操作系统的内存信息 - 小灰笔记 - CSDN博客





2017年04月12日 21:58:17[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3729








       要使用Python获取计算机的内存信息需要用到psutil模块，能够获得的内存信息有内存总量、可用内存总量、已用内存量、内存使用百分比等信息。

       写一个简单的小脚本，代码如下：

#!/usr/bin/python



import psutil



pc_mem =psutil.virtual_memory()

div_gb_factor =(1024.0 ** 3)

print("totalmemor: %fGB" % float(pc_mem.total/div_gb_factor))

print("availablememory: %fGB" % float(pc_mem.available/div_gb_factor))

print("usedmemory: %GB" % float(pc_mem.used/div_gb_factor))

print("percentof used memory: %f" % float(pc_mem.percent))

print("freememory:%fGB" % float(pc_mem.free/div_gb_factor))

       程序运行结果如下;

In [1]: %runos_mem.py

total memor:15.868237GB

available memory:13.069672GB

used memory:2.79856B

percent of usedmemory: 17.600000

freememory:13.069672GB

       我电脑配的是16G的内存，也并没有开启多少吃内存比较厉害的应用。大致的情况应该是差不多，也倒不需要再去看看任务管理器查看具体的信息了。



