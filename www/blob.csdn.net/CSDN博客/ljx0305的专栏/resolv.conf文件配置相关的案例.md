# resolv.conf文件配置相关的案例 - ljx0305的专栏 - CSDN博客
2012年09月28日 16:17:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1595

**引言**
操作系统中/etc/resolv.conf配置文件中的内容一般为空，如果该文件配置不正确，将导致ssh、route、netstat命令响应慢的问题。
在/etc/resolv.conf添加错误地内容，导致涉及域名解析相关的命令响应慢，是维护工作中较常遇到的问题。
**相关案例**
**现象：**使用其他机器ssh连接一机器，响应很慢，需要10多秒时间才有输入密码的提示响应。
**问题解决：**
被连接机器的/etc/resolv.conf文件中包含了以下内容：
XXXXX:~ # cat /etc/resolv.conf
nameserver 202.102.152.3
search XXX
由于配置了/etc/resolv.conf文件，该机器成为DNS Client，而DNS Server为202.102.152.3这个ip对应的机器。
执行ssh时，该主机会根据以上配置进行域名解析，由于202.102.152.3这台机器ip不可达，主机将尝试多次域名解析，直到达到超时次数。
**route/netstat命令输出慢**
执行route和netstat命令时，也会进行域名解析，像以上那样错误地配置/etc/resolv.conf文件，也会导致route、netstat命令输出缓慢。
而执行route –n和netstat –n时则不会有缓慢的问题，因为加了-n选项，命令执行时将输出XXX.XXX.XXX.XXX点分十进制的ip地址，不进行域名解析。
**strace跟踪分析**
在[《自助Linux之问题诊断工具strace》](http://www.cnblogs.com/bangerlee/archive/2012/02/20/2356818.html)一文中，介绍了strace的用法，像这类命令执行相应慢或执行出错的问题，使用strace进行问题分析再适合不过了。
下面我们将大脑清空，“忘记”上文内容，来学习如何使用strace来分析ssh登陆慢的问题。
首先，我们在使用ssh的机器上使用strace跟一下ssh命令：
linux198 /root# strace -o ssh.strace -fT ssh 10.71.171.140
可以看到输出文件ssh.strace中，ssh命令在底层的一个select调用阻塞了15s：
……
23452 write(3, "\325/S\265\223\246H\241nG\237\4,\2201\23\235]<\5] 7ke\200"..., 912) = 912 <0.000010>
23452select(4, [3], NULL, NULL, NULL)  = 1 (in [3]) <15.098772>
……
尽管select系统调用本身是非阻塞的，ssh仍等待select的返回再往下执行。除此之外没有获得其他有用信息。
这时我们再在被ssh登陆慢的、有问题的机器，跟一下sshd进程，先查找sshd进程的pid：
slot10-sles10sp2:~ # ps aux | grep sshd | grep -v grep
root      5352 0.00.0260681432 ?        Ss   12:270:00 /usr/sbin/sshd -o PidFile=/var/run/sshd.init.pid
root      77610.00.0407922912 ?        Ss   14:550:00 sshd: root@pts/0
再使用-p选项对运行中的进程使用strace跟踪：
slot10-sles10sp2:~ # strace -o sshd.strace -fT -p 5352
Process 5352 attached - interrupt to quit
以上命令执行时会挂住，等待ssh连接的到来。这时我们在另一台机器上使用ssh 10.71.171.140命令连接该机器。
使用ssh成功登陆之后，ctrl+c将strace命令停止，我们可以看到sshd.strace文件中有以下输出：
[]()
16894open("/etc/resolv.conf", O_RDONLY) = 4 <0.000026>
16894 fstat(4, {st_mode=S_IFREG|0644, st_size=40, ...}) = 0 <0.000023>
16894 mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x2b0eb8dbf000 <0.000024>
16894read(4, "nameserver 202.102.152.3\nsearch "..., 4096) = 40 <0.000028>
16894 read(4, "", 4096)                 = 0 <0.000023>
16894 close(4)                          = 0 <0.000024>
16894 munmap(0x2b0eb8dbf000, 4096)      = 0 <0.000026>
16894socket(PF_INET, SOCK_DGRAM, IPPROTO_IP) = 4 <0.000027>
16894connect(4, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("202.102.152.3")}, 28) = 0 <0.000025>
16894 fcntl(4, F_GETFL)                 = 0x2 (flags O_RDWR) <0.000024>
16894 fcntl(4, F_SETFL, O_RDWR|O_NONBLOCK) = 0 <0.000023>
16894 poll([{fd=4, events=POLLOUT, revents=POLLOUT}], 1, 0) = 1 <0.000024>
16894 sendto(4, "\313\367\1\0\0\1\0\0\0\0\0\0\003198\003172\00271\00210"..., 44, 0, NULL, 0) = 44 <0.000117>
16894poll([{fd=4, events=POLLIN}], 1, 5000) = 0 <5.000046>
16894 poll([{fd=4, events=POLLOUT, revents=POLLOUT}], 1, 0) = 1 <0.000024>
16894 sendto(4, "\313\367\1\0\0\1\0\0\0\0\0\0\003198\003172\00271\00210"..., 44, 0, NULL, 0) = 44 <0.000035>
16894poll([{fd=4, events=POLLIN}], 1, 5000) = 0 <4.999760>
16894 close(4)                          = 0 <0.000028>
[]()
从以上输出可以看到，当有连接到来的时候，sshd进程会打开并读取/etc/resolv.conf文件的内容，对其中的DNS nameserver建立socket，然后调用poll函数，每个poll调用5s后超时返回。
进一步地，我们了解/etc/resolv.conf文件的作用，查看/etc/resolv.conf文件的配置，从而解决问题。
**小结**
本文讨论了/etc/resolv.conf文件配置错误，导致ssh登陆慢(包括从其他机器跳转慢、使用putty等软件登陆慢)、reoute/netstat命令执行慢的问题。
通过问题的复现，使用strace进行问题分析，我们也小试了一下牛刀，熟悉了strace的使用方法。
引用:http://www.cnblogs.com/bangerlee/articles/2460619.html
