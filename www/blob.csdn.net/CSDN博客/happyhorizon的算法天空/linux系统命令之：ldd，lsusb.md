# linux系统命令之：ldd，lsusb - happyhorizon的算法天空 - CSDN博客
2017年09月29日 13:28:16[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：240
- [ldd](#ldd)- [ldd命令原理介绍](#ldd命令原理介绍)
- [ps](#ps)- [ps -l 将属于当前用户自己的PID和相关信息列出来](#ps-l-将属于当前用户自己的pid和相关信息列出来)
- [ps aux 列出所有目前正在内存当中的程序](#ps-aux-列出所有目前正在内存当中的程序)
写在最前面： 非常好的linux命令大全，全中文，且排版简洁，清晰。 
[http://man.linuxde.net](http://man.linuxde.net)
# ldd
`ldd(选项)(参数)`
**作用：**用来查看程序运行所需的共享库,常用来解决程序因缺少某个库文件而不能运行的一些问题。
**选项：**
–version：打印指令版本号； 
 -v：详细信息模式，打印所有相关信息；  
 -u：打印未使用的直接依赖；  
 -d：执行重定位和报告任何丢失的对象；  
 -r：执行数据对象和函数的重定位，并且报告任何丢失的对象和函数；  
 –help：显示帮助信息。
**参数：**
文件：指定可执行程序或者文库。
## ldd命令原理介绍
1、首先ldd不是一个可执行程序，而只是一个shell脚本
2、ldd能够显示可执行模块的dependency，其原理是通过设置一系列的环境变量，如下：LD_TRACE_LOADED_OBJECTS、LD_WARN、LD_BIND_NOW、LD_LIBRARY_VERSION、LD_VERBOSE等。当 
LD_TRACE_LOADED_OBJECTS环境变量不为空时，任何可执行程序在运行时，它都会只显示模块的dependency，而程序并不真正执行。要不你可以在shell终端测试一下，如下：
(1) `export LD_TRACE_LOADED_OBJECTS=1`
(2) 再执行任何的程序，如ls等，看看程序的运行结果
3、ldd显示可执行模块的dependency的工作原理，其实质是通过ld-linux.so（elf动态库的装载器）来实现的。我们知道，ld-linux.so模块会先于executable模块程序工作，并获得控制权，因此当上述的那些环境变量被设置时，ld-linux.so选择了显示可执行模块的dependency。
4、实际上可以直接执行ld-linux.so模块，如：/lib/ld-linux.so.2 –list program（这相当于ldd program）
# ps
linux下的进程管理命令。参数非常多。比较常用的如下：
## ps -l 将属于当前用户自己的PID和相关信息列出来。
- F 代表这个程序的旗标 (flag)， 4 代表使用者为 superuser；
- S 代表这个程序的状态 (STAT)；
- UID 代表执行者身份 
- PID 进程的ID号！底下的 PPID 则父进程的ID；
- C CPU 使用的资源百分比
- PRI指进程的执行优先权(Priority的简写)，其值越小越早被执行；
- NI 这个进程的nice值，其表示进程可被执行的优先级的修正数值。
- ADDR 这个是内核函数，指出该程序在内存的那个部分。如果是个执行的程序，一般就是『 - 』
- SZ 使用掉的内存大小；
- WCHAN 目前这个程序是否正在运作当中，若为 - 表示正在运作；
- TTY 登入者的终端机位置啰；
- TIME 使用掉的 CPU 时间。
- CMD 所下达的指令名称
## ps aux 列出所有目前正在内存当中的程序
- USER：该进程属于那个使用者账号的？
- PID ：该进程的进程ID号。 
%CPU：该进程使用掉的 CPU 资源百分比； 
%MEM：该进程所占用的物理内存百分比；
- VSZ ：该进程使用掉的虚拟内存量 (Kbytes)
- RSS ：该进程占用的固定的内存量 (Kbytes)
- TTY ：该进程是在那个终端机上面运作，若与终端机无关，则显示 ?，另外， tty1-tty6 是本机上面的登入者程序，若为 pts/0 等等的，则表示为由网络连接进主机的程序。
- STAT：该程序目前的状态，主要的状态有： 
 R ：该程序目前正在运作，或者是可被运作； 
 S ：该程序目前正在睡眠当中 (可说是 idle 状态啦！)，但可被某些讯号(signal) 唤醒。 
 T ：该程序目前正在侦测或者是停止了； 
 Z ：该程序应该已经终止，但是其父程序却无法正常的终止他，造成 zombie (疆尸) 程序的状态
- START：该进程被触发启动的时间；
- TIME ：该进程实际使用 CPU 运作的时间。
- 
COMMAND：该程序的实际指令为什么？
另外还有几个比较常用的，比如： 
`ps -axjf` 以类似程序树的方式显示 
`ps aux | egrep '(cron|syslog)'` 找出于cron和syslog相关的PID号码。
