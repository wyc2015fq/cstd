# 用strace排查故障的5种简单方法（每日一译） - DoubleLi - 博客园






原文链接：[5 simple ways to troubleshoot using Strace](http://www.hokstad.com/5-simple-ways-to-troubleshoot-using-strace)

我很意外大部分人都不知道如何使用strace。strace一直是我的首选debug工具，因为它非常的有效，很多问题都能够用它进行排查。

strace是什么？

Strace是一个用来跟踪系统调用的简易工具。它最简单的用途就是跟踪一个程序整个生命周期里所有的系统调用，并把调用参数和返回值以文本的方式输出。

当然它还可以做更多的事情：

strace可以过筛选出特定的系统调用。

strace可以记录系统调用的次数，时间，成功和失败的次数。

strace可以跟踪发给进程的信号。

strace可以通过pid附加到任何正在运行的进程上。

strace类似其他Unix系统上的truss，或者Sun's Dtrace

使用教程

以下这些只是些皮毛的用法：

1）查看初始化时程序读取的配置文件

你是否遇到过程序去错误的位置读取配置文件的情况？

简单的排查方式：
$ strace php 2>&1 | grep php.iniopen("/usr/local/bin/php.ini", O_RDONLY) = -1 ENOENT (No such file or directory)open("/usr/local/lib/php.ini", O_RDONLY) = 4lstat64("/usr/local/lib/php.ini", {st_mode=S_IFLNK|0777, st_size=27, ...}) = 0readlink("/usr/local/lib/php.ini", "/usr/local/Zend/etc/php.ini", 4096) = 27lstat64("/usr/local/Zend/etc/php.ini", {st_mode=S_IFREG|0664, st_size=40971, ...}) = 0So this version of PHP reads php.ini from /usr/local/lib/php.ini (but it tries /usr/local/bin first).
如果只关心特定的系统调用可以使用以下略微复杂的使用方法：
$ strace -e open php 2>&1 | grep php.iniopen("/usr/local/bin/php.ini", O_RDONLY) = -1 ENOENT (No such file or directory)open("/usr/local/lib/php.ini", O_RDONLY) = 4
如果安装多个版本的程序库，想搞清楚自己的程序加载的是哪个也可以如法炮制~。

2）为什么程序打不开这个文件？

是否有遇到过读取文件的时候被拒绝呢，你可以试试下面的命令:
$ strace -e open,access 2>&1 | grep your-filename
查看open()和access()系统调用是否有异常

3）进程现在在做啥？

是否有遇到过进程突然cpu占用率很高？又或者进程莫名其妙被挂起的情况？

找到这个进程的pid，然后执行下列命令：
root@dev:~# strace -p 15427Process 15427 attached - interrupt to quitfutex(0x402f4900, FUTEX_WAIT, 2, NULL Process 15427 detached
在这个示例里进程在调用futex的时候被挂起了。顺带一说，在这个例子里调用futex挂起可能有很多的原因（Futex是Linux的一种线程同步原语）。上述场景是个正常工作等待处理请求的apache子进程。

“strace -p”可以让你省去很多猜测，不需要重新编译，重启应用打log就能够找到问题。

4）统计程序的调用时间

要对程序进行性能分析往往需要重新编译程序，并打开跟踪选项。用strace可以很容易的附加到进程上查看实时的时间消耗。

如下：
root@dev:~# strace -c -p 11084Process 11084 attached - interrupt to quitProcess 11084 detached% time     seconds  usecs/call     calls    errors syscall------ ----------- ----------- --------- --------- ---------------- 94.59    0.001014          48        21           select  2.89    0.000031           1        21           getppid  2.52    0.000027           1        21           time------ ----------- ----------- --------- --------- ----------------100.00    0.001072                    63           totalroot@dev:~# 
启用strace -c -p命令后，在你按ctrl-c退出前程序的调用时间将会打印出来。

在上面的例子里。空闲的Postgres进程大部分时间都在安静的等待select()返回。在每个select()调用中调用getppid() 和time()。这是个标准的event loop方式。

也可以跟踪一次程序的开始和结束，比如下面的“ls”例子
root@dev:~# strace -c >/dev/null ls% time     seconds  usecs/call     calls    errors syscall------ ----------- ----------- --------- --------- ---------------- 23.62    0.000205         103         2           getdents64 18.78    0.000163          15        11         1 open 15.09    0.000131          19         7           read 12.79    0.000111           7        16           old_mmap  7.03    0.000061           6        11           close  4.84    0.000042          11         4           munmap  4.84    0.000042          11         4           mmap2  4.03    0.000035           6         6         6 access  3.80    0.000033           3        11           fstat64  1.38    0.000012           3         4           brk  0.92    0.000008           3         3         3 ioctl  0.69    0.000006           6         1           uname  0.58    0.000005           5         1           set_thread_area  0.35    0.000003           3         1           write  0.35    0.000003           3         1           rt_sigaction  0.35    0.000003           3         1           fcntl64  0.23    0.000002           2         1           getrlimit  0.23    0.000002           2         1           set_tid_address  0.12    0.000001           1         1           rt_sigprocmask------ ----------- ----------- --------- --------- ----------------100.00    0.000868                    87        10 total
如你所料，这个命令花费两个调用用于读取目录实体。

5）为什么连不上服务器？

调试某些进程为什么连不上远程的服务器是个很让人头疼的事情。DNS可能挂了，连接可能断了，服务器可能返回什么无法识别的东西。。。你可以使用tcpdump去分析这些问题，tcpdump是个很棒的工具，但是很多情况下，strace可以给你提供更简洁的信息。如果你的程序里有很多进程连接到某台服务器，用tcpdump来处理就是很蛋疼的事情，因为你会看到太多的信息，没法抓到重点。

以下是一个跟踪“nc”命令连接到www.news.com的80端口的例子：
$ strace -e poll,select,connect,recvfrom,sendto nc www.news.com 80sendto(3, "\\24\\0\\0\\0\\26\\0\\1\\3\\255\\373NH\\0\\0\\0\\0\\0\\0\\0\\0", 20, 0, {sa_family=AF_NETLINK, pid=0, groups=00000000}, 12) = 20connect(3, {sa_family=AF_FILE, path="/var/run/nscd/socket"}, 110) = -1 ENOENT (No such file or directory)connect(3, {sa_family=AF_FILE, path="/var/run/nscd/socket"}, 110) = -1 ENOENT (No such file or directory)connect(3, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("62.30.112.39")}, 28) = 0poll([{fd=3, events=POLLOUT, revents=POLLOUT}], 1, 0) = 1sendto(3, "\\213\\321\\1\\0\\0\\1\\0\\0\\0\\0\\0\\0\\3www\\4news\\3com\\0\\0\\34\\0\\1", 30, MSG_NOSIGNAL, NULL, 0) = 30poll([{fd=3, events=POLLIN, revents=POLLIN}], 1, 5000) = 1recvfrom(3, "\\213\\321\\201\\200\\0\\1\\0\\1\\0\\1\\0\\0\\3www\\4news\\3com\\0\\0\\34\\0\\1\\300\\f"..., 1024, 0, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("62.30.112.39")}, [16]) = 153connect(3, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("62.30.112.39")}, 28) = 0poll([{fd=3, events=POLLOUT, revents=POLLOUT}], 1, 0) = 1sendto(3, "k\\374\\1\\0\\0\\1\\0\\0\\0\\0\\0\\0\\3www\\4news\\3com\\0\\0\\1\\0\\1", 30, MSG_NOSIGNAL, NULL, 0) = 30poll([{fd=3, events=POLLIN, revents=POLLIN}], 1, 5000) = 1recvfrom(3, "k\\374\\201\\200\\0\\1\\0\\2\\0\\0\\0\\0\\3www\\4news\\3com\\0\\0\\1\\0\\1\\300\\f"..., 1024, 0, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("62.30.112.39")}, [16]) = 106connect(3, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("62.30.112.39")}, 28) = 0poll([{fd=3, events=POLLOUT, revents=POLLOUT}], 1, 0) = 1sendto(3, "\\\\\\2\\1\\0\\0\\1\\0\\0\\0\\0\\0\\0\\3www\\4news\\3com\\0\\0\\1\\0\\1", 30, MSG_NOSIGNAL, NULL, 0) = 30poll([{fd=3, events=POLLIN, revents=POLLIN}], 1, 5000) = 1recvfrom(3, "\\\\\\2\\201\\200\\0\\1\\0\\2\\0\\0\\0\\0\\3www\\4news\\3com\\0\\0\\1\\0\\1\\300\\f"..., 1024, 0, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("62.30.112.39")}, [16]) = 106connect(3, {sa_family=AF_INET, sin_port=htons(80), sin_addr=inet_addr("216.239.122.102")}, 16) = -1 EINPROGRESS (Operation now in progress)select(4, NULL, [3], NULL, NULL)        = 1 (out [3])
这个连接尝试访问了/var/run/nscd/socket，这意味着nc命令首先尝试连接NSCD服务（名称缓存守护进程，通常用于NIS,YP,LDAP中的名字查找）。在上面的例子里执行失败。

nc命令转而使用DNS（DNS的端口是53，"sin_port=htons(53)" ）。可以看到程序调用”sendto()"发送包含www.news.com信息的DNS数据包。因为某些原因，尝试了3次。为什么会这样呢？我猜测可能www.news.com是CNAME ，最后，程序连接从DNS获取到的IP，注意这里，返回EINPROGRESS。这意味这这个连接是非堵塞的。连接成功后nc调用select。

添加read和write到跟踪的系统调用列表里，连接上后敲入test，你会看到如下信息：
read(0, "test\\n", 1024)                 = 5write(3, "test\\n", 5)                   = 5poll([{fd=3, events=POLLIN, revents=POLLIN}, {fd=0, events=POLLIN}], 2, -1) = 1read(3, "
你可以看到，程序从标准输入中读取到“test”，写到网络连接中，然后调用poll()等待响应，读取响应然后写入到标准输出中。









