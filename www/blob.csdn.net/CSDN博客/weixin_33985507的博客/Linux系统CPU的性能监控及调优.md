# Linux系统CPU的性能监控及调优 - weixin_33985507的博客 - CSDN博客
2017年06月08日 21:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
![5609881-22abc2d5dae3616d.png](https://upload-images.jianshu.io/upload_images/5609881-22abc2d5dae3616d.png)
CPU调优
> 
性能优化是一个老生常谈的话题，典型的性能问题如页面响应慢、接口超时，服务器负载高、并发数低，数据库频繁死锁等。尤其是在“糙快猛”的互联网开发模式大行其道的今天，随着系统访问量的日益增加和代码的臃肿，各种性能问题开始纷至沓来。在系统层面能够影响应用性能的一般包括三个因素：CPU、内存和IO，今天，我们先谈谈CPU性能的监控以及调优。
# CPU性能监控
当程序响应变慢的时候，首先使用top、vmstat、ps等命令查看系统的cpu使用率是否有异常，从而可以判断出是否是cpu繁忙造成的性能问题。其中，主要通过us（用户进程所占的%）这个数据来看异常的进程信息。当us接近100%甚至更高时，可以确定是cpu繁忙造成的响应缓慢。一般说来，cpu繁忙的原因有以下几个：
> - 线程中有无限空循环、无阻塞、正则匹配或者单纯的计算
- 频繁的gc
- 多线程频繁的上下文切换
## top命令
![5609881-dc20ed17e2bdda0c.png](https://upload-images.jianshu.io/upload_images/5609881-dc20ed17e2bdda0c.png)
top命令
对于多个或多核cpu，上面的显示则会是多个cpu所占用的百分比总合。如需查看每个核的消耗情况，可在进入top视图后按1，就会按核来显示cpu的使用情况，如上图。
> - us 表示用户进程处理所占的百分比
- sy 表示为内核线程处理所占的百分比
- ni 表示被nice命令改变优先级的任务所占的百分比
- id 表示cpu的空闲时间所占的百分比
- wa 表示为在执行过程中等待io所占的百分比
- hi 表示为硬件中断所占的百分比
- si 表示为软件中断所占的百分比
- st 表示虚拟cpu等待实际cpu的时间的百分比
## vmstat
![5609881-7194113766fed384.png](https://upload-images.jianshu.io/upload_images/5609881-7194113766fed384.png)
vmstat命令
> - in 每秒CPU的中断次数，包括时间中断
- cs 每秒上下文切换次数，这个值要越小越好，太大了，要考虑调低线程或者进程的数目。每次调用系统函数，我们的代码就会进入内核空间，导致上下文切换，这个是很耗资源，也要尽量避免频繁调用系统函数。上下文切换次数过多表示你的CPU大部分浪费在上下文切换，导致CPU干正经事的时间少了，CPU没有充分利用，是不可取的。
- us 用户CPU时间。
- sy 系统CPU时间，如果太高，表示系统调用时间长，例如是IO操作频繁。
- id  空闲 CPU时间，一般来说，id + us + sy = 100,一般我认为id是空闲CPU使用率，us是用户CPU使用率，sy是系统CPU使用率。
- wt 等待IO CPU时间。
## gstat -gcutil
如果发现是Java进程CPU占用过高，可以使用这个命令查看进程是不是正在频繁GC，如下图所示。
![5609881-0cb543968e72a48f.png](https://upload-images.jianshu.io/upload_images/5609881-0cb543968e72a48f.png)
jstat命令
> - S0  — Heap上的 Survivor space 0 区已使用空间的百分比
- S1  — Heap上的 Survivor space 1 区已使用空间的百分比
- E   — Heap上的 Eden space 区已使用空间的百分比
- O   — Heap上的 Old space 区已使用空间的百分比
- P   — Perm space 区已使用空间的百分比
- YGC — 从应用程序启动到采样时发生 Young GC 的次数
- YGCT– 从应用程序启动到采样时 Young GC 所用的时间(单位秒)
- FGC — 从应用程序启动到采样时发生 Full GC 的次数
- FGCT– 从应用程序启动到采样时 Full GC 所用的时间(单位秒)
- GCT — 从应用程序启动到采样时用于垃圾回收的总时间(单位秒)
# 问题分析
根据上面提供的几个常用命令，定位到问题以后，就可以根据具体问题分析其产生的原因了。
> 
CPU瓶颈表现在两个方面：用户态CPU瓶颈和系统态CPU瓶颈。运行操作系统内核以外的软件时导致的瓶颈为用户态CPU瓶颈，运行操作系统内核的时候导致的瓶颈为系统态CPU瓶颈。用户态CPU和系统态CPU时间比率在3:1到4:1之间是正常的。如果在有瓶颈的系统中，用户和系统时间比率高于这个区间，就应该分析用户态CPU时间增加的原因。
## us过高
当us值过高时，表示运行的应用消耗了大部分的cpu。在这种情况下，对于java应用而言，最重要的是找到具体消耗cpu的线程所执行的代码，可以采用如下方法。
1.使用gstat -gcutil查看JVM是否频繁的进行GC。
2.如果根据gcutil查看，GC并不频繁，请根据[《当CPU飙高时，它在做什么》](https://www.jianshu.com/p/90579ec3113f)提供的方式，查看CPU在执行什么代码，来定位问题。
## sy过高
当sy值过高时，使用vmstat来查看线程切换次数。很可能是linux花费了更多的时间在进行线程切换。java应用造成这种现象的主要原因是启动的线程比较多， 且这些线程多处于不断的阻塞（例如锁等待，io等待）和执行状态的变化过程中，这就导致了操作系统要不断的切换执行的线程， 产生大量的上下文切换。在这种情况下，对java应用而言，最重要的是找出不断切换状态的原因， 可采用的方法为通过kill -3 pid 或jstack -l pid的方法dump出java应用程序的线程信息，查看线程的状态信息以及锁信息， 找出等待状态或锁竞争过多的线程。
# CPU调优
## 设置程序执行的优先级
可以使用nice和renice设置程序执行的优先级。
格式：nice [-n 数值] 命令nice 指令可以改变程序执行的优先权等级。指令让使用者在执行程序时，指定一个优先等级，称之为 nice 值。 这个数值从最高优先级的-20到最低优先级的19。负数值只有 root 才有权力使。 一般使用者，也可使用 nice 指令來做执行程序的优先级管理，但只能将nice值越调越高。
## 使用ulimit限制cpu占用时间
注意，ulimit 限制的是当前shell进程以及其派生的子进程。因此可以在脚本中调用ulimit来限制cpu使用时间。 例如，限制tar的cpu占用时间，单位秒。
`ulimit -SHt 100 tar test.tar.gz test`
如果tar占用时间超过了100秒，tar将会退出，这可能会导致打包不完全，因此不推荐使用ulimit对cpu占用时间进行限制。 另外，通过修改系统的/etc/security/limits配置文件，可以针对用户进行限制。
## 使用程序自带的对cpu使用调整的功能
某些程序自带了对cpu使用调整的功能，比如nginx服务器，通过其配置文件，可以为工作进程指定cpu，如下：
```
worker_processes 4; 
worker_cpu_affinity 0001 0010 0100 1000;
```
这里0001 0010 0100 1000是掩码，分别代表第1、2、3、4颗cpu核心，这就使得cpu的使用比较平均到每个核心上。
使用Nginx时，这种优化方式是比较常见的。
**相关阅读：**
- 
[MySQL索引原理及慢查询优化](https://www.jianshu.com/p/bd9154df8bf0)
- 
[Java程序员最常用的Linux命令](https://www.jianshu.com/p/341bc031ea2b)
- 
[Linux查看实时网卡流量的几种方式](https://www.jianshu.com/p/b9e942f3682c)
- 
[如何优雅的实现文件上传或下载](https://www.jianshu.com/p/fca34c2bf7f1)
**本文首发在 [刘振锋的简书博客](https://www.jianshu.com/u/ffd67abc5f8c)    原文地址为《[Linux系统CPU的性能监控及调优](https://www.jianshu.com/p/6beacca6fdcd)》转载请注明！**
