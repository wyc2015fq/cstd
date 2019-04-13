
# Linux内核OOM机制的详细分析 - 高科的专栏 - CSDN博客

2014年06月18日 11:25:15[高科](https://me.csdn.net/pbymw8iwm)阅读数：6052标签：[linux																](https://so.csdn.net/so/search/s.do?q=linux&t=blog)[内核																](https://so.csdn.net/so/search/s.do?q=内核&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=linux&t=blog)个人分类：[linux																](https://blog.csdn.net/pbymw8iwm/article/category/863753)


Linux 内核有个机制叫OOM killer（Out-Of-Memory killer），该机制会监控那些占用内存过大，尤其是瞬间很快消耗大量内存的进程，为了防止内存耗尽而内核会把该进程杀掉。典型的情况是：某天一台机器突然ssh远程登录不了，但能ping通，说明不是网络的故障，原因是sshd进程被OOM killer杀掉了（多次遇到这样的假死状况）。重启机器后查看系统日志/var/log/messages会发现Out of Memory: Kill process 1865（sshd）类似的错误信息。

防止重要的系统进程触发(OOM)机制而被杀死：可以设置参数/proc/**PID**/oom_adj为-17，可临时关闭linux内核的OOM机制。内核会通过特定的算法给每个进程计算一个分数来决定杀哪个进程，每个进程的oom分数可以/proc/**PID**/oom_score中找到。我们运维过程中保护的一般是sshd和一些管理agent。
保护某个进程不被内核杀掉可以这样操作：
点击(此处)折叠或打开
echo-17>/proc/$PID/oom_adj

如何防止sshd被杀，可以这样操作：
点击(此处)折叠或打开
pgrep-f"/usr/sbin/sshd"|whileread PID;do echo-17>/proc/$PID/oom_adj;done

可以在计划任务里加入这样一条定时任务，就更安全了：点击(此处)折叠或打开
\#/etc/cron.d/oom_disable
*/1****root pgrep-f"/usr/sbin/sshd"|whileread PID;do echo-17>/proc/$PID/oom_adj;done

为了避免重启失效，可以写入/etc/rc.d/rc.local点击(此处)折叠或打开
echo-17>/proc/$(pidof sshd)/oom_adj

至于为什么用-17而不用其他数值（默认值为0），这个是由linux内核定义的，查看内核源码可知：
以linux-3.3.6版本的kernel源码为例，路径为linux-3.6.6/include/linux/oom.h，阅读内核源码可知oom_adj的可调值为15到-16，其中15最大-16最小，-17为禁止使用OOM。oom_score为2的n次方计算出来的，其中n就是进程的oom_adj值，所以oom_score的分数越高就越会被内核优先杀掉。
![](http://blog.chinaunix.net/attachment/201312/1/29242873_13858814534sQ4.jpg)
当然还可以通过修改内核参数禁止OOM机制
点击(此处)折叠或打开
\# sysctl-w vm.panic_on_oom=1
vm.panic_on_oom=1//1表示关闭，默认为0表示开启OOM
\# sysctl-p

为了验证OOM机制的效果，我们不妨做个测试。首先看看我系统现有内存大小，没错96G多，物理上还要比查看的值大一些。
![](http://blog.chinaunix.net/attachment/201312/1/29242873_13858814655hFC.jpg)
再看看目前进程最大的有哪些，top查看，我目前只跑了两个java程序的进程，分别4.6G，再往后redis进程吃了21m，iscsi服务占了32m，gdm占了25m，其它的进程都是几M而已。
![](http://blog.chinaunix.net/attachment/201312/1/29242873_1385881477B0g3.jpg)
现在我自己用C写一个叫bigmem程序，我指定该程序分配内存85G，呵呵，效果明显，然后执行后再用top查看，排在第一位的是我的bigmem，RES是物理内存，已经吃满了85G。
![](http://blog.chinaunix.net/attachment/201312/1/29242873_1385881485291F.jpg)
继续观察，当bigmem稳定保持在85G一会后，内核会自动将其进程kill掉，增长的过程中没有被杀，如果不希望被杀可以执行
点击(此处)折叠或打开
pgrep-f"bigmem"|whileread PID; do echo-17>/proc/$PID/oom_adj;done

执行以上命令前后，明显会对比出效果，就可以体会到内核OOM机制的实际作用了。
注意：
1.Kernel-2.6.26之前版本的oomkiller算法不够精确，RHEL 6.x版本的2.6.32可以解决这个问题。
2.子进程会继承父进程的oom_adj。
3.OOM不适合于解决内存泄漏(Memory leak)的问题。
4.有时free查看还有充足的内存，但还是会触发OOM，是因为该进程可能占用了特殊的内存地址空间。


