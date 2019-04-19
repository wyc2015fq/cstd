# Linux 的 core 文件 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [倪彦春](http://www.jobbole.com/members/time-track) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
## 1. 什么是core文件
core 文件是大多数 UNIX 系统实现的一种特性，当进程崩溃时，操作系统会将进程当前的内存映像和一部分相关的调试信息写入 core 文件，方便人们后面对问题进行定位。
## 2. 哪些信号可能会产生core文件
操作系统里面有很多信号（每个信号都有一个名字，且已SIG开头，用正整数表示，Linux系统一般在<bits/signum.h>头文件中定义），分别代表了不同的含义，在Linux系统中，我们可以通过shell命令 kill -l 来查看系统有哪些信号。操作系统收到信号时，内核会按照以下三种方式之一去对信号进行处理：
- 忽略此信号。大多数的信号都可以用这种方式去处理，即内核收到此信号时，对进程不做任何处理，直接忽略。但是SIGKILL和SIGSTOP这两个信号不能被忽略，因为它们向超级用户提供了使进程终止或停止的可靠方法。
- 捕捉信号。即我们向内核注册一个信号处理函数，当内核收到某个信号时，就去调用注册的信号处理函数对信号进行处理。比如我们经常使用的命令kill默认发的是SIGTERM终止信号。注意，不能捕捉SIGKILL和SIGSTOP信号。
- 执行默认动作。每个系统都有一套自己默认的信号处理函数，即如果我们不显式的去捕捉信号，那内核收到信号时，要么忽略此信号，要么执行默认的操作。可以理解为操作系统有自己默认的信号处理函数。
Linux信号有很多，这里我们列举出默认动作中可能产生core文件的信号（摘自《UNIX环境高级编程》第二版）
|信号名字|说明|默认动作|
|----|----|----|
|SIGABRT|异常终止（调用abort函数产生此信号）|终止+core|
|SIGBUS|硬件故障，比如出现某些内存故障|终止+core|
|SIGEMT|硬件故障|终止+core|
|SIGFPE|算术异常，比如除以0，浮点溢出等|终止+core|
|SIGILL|非法硬件指令|终止+core|
|SIGIOT|硬件故障|终止+core|
|SIGQUIT|终端退出符，比如Ctrl+C|终止+core|
|SIGSEGV|无效内存引用|终止+core|
|SIGSYS|无效系统调用|终止+core|
|SIGXCPU|超过CPU限制（setrlimit）|终止+core/忽略|
|SIGXFSZ|超过文件长度限制（setrlimit）|终止+core/忽略|
如果我们没有定义上述信号的信号处理函数，那默认情况下，内核收到这些信号，将终止进程，并产生该进程的core文件（该进程的内存映像以及一些调试信息便保存在该core文件中）。
## 3. 如何开启与关闭core文件
类UNIX操作系统为我们提供了一个可以打开与关闭core文件的开关，因为并非所有场景我们都希望可以生成core文件。类UNIX操作系统为我们提供了一个工具ulimit可以用来设置和查看文件大小的限制，所以我们也可以用这个工具来查看和设置core大小与限制。使用 ulimit -a 可以查看系统上面所有的文件大小的限制，比如下面是我的系统的输出结果：


```
allan@ubuntu:~$ ulimit -a
core file size          (blocks, -c) 0
data seg size           (kbytes, -d) unlimited
scheduling priority             (-e) 0
file size               (blocks, -f) unlimited
pending signals                 (-i) 7725
max locked memory       (kbytes, -l) 64
max memory size         (kbytes, -m) unlimited
open files                      (-n) 1024
pipe size            (512 bytes, -p) 8
POSIX message queues     (bytes, -q) 819200
real-time priority              (-r) 0
stack size              (kbytes, -s) 8192
cpu time               (seconds, -t) unlimited
max user processes              (-u) 7725
virtual memory          (kbytes, -v) unlimited
file locks                      (-x) unlimited
```
显示结果分为三部分，比如第一行，第一部分是“core file size”，第二部分是“（blocks, -c）”，第三部分是“0”。这一行的含义是说“core文件的大小限制为0（即关闭core文件/不产生core文件），单位是blocks；使用ulimit -c命令可以改变此值”。这里我们还可以看到系统很多其他文件相关的设置，比如数据段大小无限制，文件大小无限制，最多打开的文件数为1024，栈大小为8192KB等等。
所以很明显，我们可以使用 ulimit -c core_size/unlimited 去设置core文件的大小，注意core_size单位是字节。如果core_size为0，则表示不生成core文件，unlimited表示对core文件大小不做限制。我们可以将该命令现在某个用户的环境变量里面去对不同的用户进行设置，比如写在/home/allan/.bashrc文件里面只对allan用户有效；也可以写在系统的环境变量里面，对所有用户有效，比如/etc/.profile。
## 4. 如何自定义core文件名和目录
（1） /proc/sys/kernel/core_uses_pid 可以控制产生的core文件的文件名中是否添加pid作为扩展，如果添加则文件内容为1，否则为0。如果添加了pid，生成的core文件一般为core.xxx（xxx为core dump的进程号）。默认为0，即不添加进程pid.
（2） /proc/sys/kernel/core_pattern 可以设置格式化的core文件保存位置或文件名，比如我的系统的默认值为：


```
|/usr/share/apport/apport %p %s %c %P
```
我们可以使用以下命令修改此文件：
echo “/corefile/core-%e-%p-%t” > core_pattern ，这样可以将core文件统一生成到/corefile目录下，产生的文件名为core-命令名-pid-时间戳。
这里列举一下常用的参数：


```
%p - insert pid into filename 添加pid
%u - insert current uid into filename 添加当前uid
%g - insert current gid into filename 添加当前gid
%s - insert signal that caused the coredump into the filename 添加导致产生core的信号
%t - insert UNIX time that the coredump occurred into filename 添加core文件生成时的unix时间
%h - insert hostname where the coredump happened into filename 添加主机名
%e - insert coredumping executable name into filename 添加命令名
```
5. 如何查看core文件
我们可以使用gdb来查看core文件（core文件必须是完好的，比如如果我们限制的core文件大小比较小，导致core文件被截断，则gdb查看时将出错）。查看语法如下：
> 
gdb [exec file] [core file]
比如：  gdb ./test core.3937 。gdb进去以后，可以使用bt或where来查看进程崩溃之前的堆栈信息。
## 6. 一些注意事项
在Linux下要保证程序崩溃时生成Coredump要注意这些问题：
- 要保证存放Coredump的目录存在且进程对该目 录有写权限。存放Coredump的目录即进程的当前目录，一般就是当初发出命令启动该进程时所在的目录。但如果是通过脚本启动，则脚本可能会修改当前目 录，这时进程真正的当前目录就会与当初执行脚本所在目录不同。这时可以查看”/proc/<进程pid>/cwd“符号链接的目标来确定进程 真正的当前目录地址。通过系统服务启动的进程也可通过这一方法查看。
- 若程序调用了seteuid()/setegid()改变 了进程的有效用户或组，则在默认情况下系统不会为这些进程生成Coredump。很多服务程序都会调用seteuid()，如MySQL，不论你用什么用 户运行mysqld_safe启动MySQL，mysqld进行的有效用户始终是msyql用户。如果你当初是以用户A运行了某个程序，但在ps里看到的 这个程序的用户却是B的话，那么这些进程就是调用了seteuid了。为了能够让这些进程生成core dump，需要将/proc/sys/fs /suid_dumpable文件的内容改为1（一般默认是0）。
- 这个一般都知道，就是要设置足够大的Core文件大小限制 。程序崩溃时生成的Core文件大小即为程序运行时占用的内存大小。但程序崩溃时的行为不可按平常时的行为来估计，比如缓冲区溢出等错误可能导致堆栈被 破坏，因此经常会出现某个变量的值被修改成乱七八糟的，然后程序用这个大小去申请内存就可能导致程序比平常时多占用很多内存。因此无论程序正常运行时占用 的内存多么少，要保证生成Core文件还是将大小限制设为unlimited为好。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/06/d0134f58cddae83b225e901371227914.png)![](http://jbcdn2.b0.upaiyun.com/2016/06/cc97d8a894fbbdd9169656e9faedc825.jpg)
