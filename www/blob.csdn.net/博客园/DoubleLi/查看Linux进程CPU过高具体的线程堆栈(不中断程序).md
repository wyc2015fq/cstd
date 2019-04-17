# 查看Linux进程CPU过高具体的线程堆栈(不中断程序) - DoubleLi - 博客园






1、TOP命令，找到占用CPU最高的进程

$ top 

top - 20:11:45 up 850 days,  1:18,  3 users,  load average: 1.04, 1.01, 0.99 
Tasks:  61 total,   1 running,  60 sleeping,   0 stopped,   0 zombie 
Cpu(s):  1.4% us,  0.1% sy,  0.0% ni, 98.3% id,  0.1% wa,  0.0% hi,  0.2% si 
Mem:  16418172k total, 15693376k used,   724796k free,  1146696k buffers 
Swap: 10223608k total,0k used, 10223608k free, 12537692k cached 

PID USER  PR  NI  VIRT  RES  SHR S %CPU %MEMTIME+  COMMAND 
24714 ztgame16   0 1409m 1.2g 4252 S 99.6  7.4   2390:57 IMVChannelServe 
1 root  16   0  4772  520  432 S  0.0  0.0   0:03.43 init
2 root  RT   0 000 S  0.0  0.0   0:05.75 migration/0 
3 root  34  19 000 S  0.0  0.0   5:22.97 ksoftirqd/0 
4 root  RT   0 000 S  0.0  0.0   0:07.90 migration/1 
5 root  34  19 000 S  0.0  0.0   0:00.27 ksoftirqd/1 
6 root  RT   0 000 S  0.0  0.0   0:04.07 migration/2 
7 root  34  19 000 S  0.0  0.0   0:00.47 ksoftirqd/2 
8 root  RT   0 000 S  0.0  0.0   0:04.00 migration/3 
9 root  34  19 000 S  0.0  0.0   0:00.33 ksoftirqd/3 


2、通过TOP -H -p 进程ID，找到具体的线程占用情况，Shift+H可以开启关闭线程显示

$ top -H -p 24714 

top - 20:15:30 up 850 days,  1:22,  3 users,  load average: 1.26, 1.09, 1.02 
Tasks:  16 total,   1 running,  15 sleeping,   0 stopped,   0 zombie 
Cpu(s): 24.8% us,  0.3% sy,  0.0% ni, 73.1% id,  0.0% wa,  0.0% hi,  1.8% si 
Mem:  16418172k total, 15701376k used,   716796k free,  1146704k buffers 
Swap: 10223608k total,0k used, 10223608k free, 12546048k cached 

PID USER  PR  NI  VIRT  RES  SHR S %CPU %MEMTIME+  COMMAND 
24729 ztgame16   0 1409m 1.2g 4252 R 97.0  7.4   2307:22 IMVChannelServe 
24721 ztgame15   0 1409m 1.2g 4252 S  2.0  7.4  84:22.40 IMVChannelServe 
24714 ztgame16   0 1409m 1.2g 4252 S  0.0  7.4   0:03.80 IMVChannelServe 
24716 ztgame15   0 1409m 1.2g 4252 S  0.0  7.4   0:00.00 IMVChannelServe 
24717 ztgame15   0 1409m 1.2g 4252 S  0.0  7.4   0:00.04 IMVChannelServe 
24718 ztgame15   0 1409m 1.2g 4252 S  0.0  7.4   0:00.00 IMVChannelServe 
24719 ztgame15   0 1409m 1.2g 4252 S  0.0  7.4   0:00.08 IMVChannelServe 
24720 ztgame15   0 1409m 1.2g 4252 S  0.0  7.4   0:00.01 IMVChannelServe 
24722 ztgame15   0 1409m 1.2g 4252 S  0.0  7.4   0:00.00 IMVChannelServe 
24723 ztgame16   0 1409m 1.2g 4252 S  0.0  7.4   0:00.00 IMVChannelServe 
24724 ztgame15   0 1409m 1.2g 4252 S  0.0  7.4   0:00.01 IMVChannelServe 
24725 ztgame15   0 1409m 1.2g 4252 S  0.0  7.4   0:09.83 IMVChannelServe 
24726 ztgame15   0 1409m 1.2g 4252 S  0.0  7.4   0:00.00 IMVChannelServe 
24727 ztgame15   0 1409m 1.2g 4252 S  0.0  7.4   0:00.76 IMVChannelServe 
24728 ztgame16   0 1409m 1.2g 4252 S  0.0  7.4   0:00.53 IMVChannelServe 
24730 ztgame16   0 1409m 1.2g 4252 S  0.0  7.4   2:42.18 IMVChannelServe 


3、通过命令pstack 进程ID显示线程堆栈，LWP 24729对应线程ID的堆栈，就是占用CPU最高的堆栈，可以具体分析什么原因造成的。

$ pstack 24714 
Thread 16 (Thread 1084229984 (LWP 24716)): 
#0  0x00000039c5a901d5 in __nanosleep_nocancel () from /lib64/tls/libc.so.6 
#1  0x00000039c5ac3058 in usleep () from /lib64/tls/libc.so.6 
#2  0x00000000005ebe10 in zVerifyThread::run () 
#3  0x00000000005e9d29 in zThread::threadFunc () 
#4  0x00000039c6106137 in start_thread () from /lib64/tls/libpthread.so.0 
#5  0x00000039c5ac9883 in clone () from /lib64/tls/libc.so.6 
Thread 15 (Thread 1094719840 (LWP 24717)): 
#0  0x00000039c5a901d5 in __nanosleep_nocancel () from /lib64/tls/libc.so.6 
#1  0x00000039c5ac3058 in usleep () from /lib64/tls/libc.so.6 
#2  0x00000000005ebe10 in zVerifyThread::run () 
#3  0x00000000005e9d29 in zThread::threadFunc () 
#4  0x00000039c6106137 in start_thread () from /lib64/tls/libpthread.so.0 
#5  0x00000039c5ac9883 in clone () from /lib64/tls/libc.so.6 
Thread 14 (Thread 1105209696 (LWP 24718)): 
#0  0x00000039c5a901d5 in __nanosleep_nocancel () from /lib64/tls/libc.so.6 
#1  0x00000039c5ac3058 in usleep () from /lib64/tls/libc.so.6 
#2  0x00000000005ebe10 in zVerifyThread::run () 
#3  0x00000000005e9d29 in zThread::threadFunc () 
#4  0x00000039c6106137 in start_thread () from /lib64/tls/libpthread.so.0 
#5  0x00000039c5ac9883 in clone () from /lib64/tls/libc.so.6 
Thread 13 (Thread 1115699552 (LWP 24719)): 
#0  0x00000039c5a901d5 in __nanosleep_nocancel () from /lib64/tls/libc.so.6 
#1  0x00000039c5ac3058 in usleep () from /lib64/tls/libc.so.6 
#2  0x00000000005ebe10 in zVerifyThread::run () 
#3  0x00000000005e9d29 in zThread::threadFunc () 
#4  0x00000039c6106137 in start_thread () from /lib64/tls/libpthread.so.0 
#5  0x00000039c5ac9883 in clone () from /lib64/tls/libc.so.6 
Thread 3 (Thread 1220598112 (LWP 24729)): 
#0  0x00000039c5a71e87 in memset () from /lib64/tls/libc.so.6 
#1  0x00000000004fa591 in ChannelTask::forwardToClientByMedia () 
#2  0x0000000000506220 in ChannelTask::parseClientMsg_Normal () 
#3  0x000000000051ef55 in ChannelTask::parseClientMsg () 
#4  0x000000000051f070 in ChannelTask::cmdMsgParse_Forward () 
#5  0x000000000051f1d1 in ChannelTask::cmdMsgParse () 
#6  0x000000000051f414 in ChannelTask::processCmd () 
#7  0x0000000000523ea8 in ChannelTaskManager::processCmd () 
#8  0x0000000000525ddd in ChannelTimeTick::run () 
#9  0x00000000005e9d29 in zThread::threadFunc () 
#10 0x00000039c6106137 in start_thread () from /lib64/tls/libpthread.so.0 
#11 0x00000039c5ac9883 in clone () from /lib64/tls/libc.so.6 
Thread 2 (Thread 1231087968 (LWP 24730)): 
#0  0x00000039c610af8b in __lll_mutex_lock_wait () 
#1  0x0000000000000001 in ?? () 
#2  0x0000000000000065 in ?? () 
#3  0x00000039c6107d87 in pthread_mutex_lock () from /lib64/tls/libpthread.so.0 
#4  0x0000003a500ae29e in operator delete () from /usr/lib64/libstdc++.so.6 
#5  0x000000000053f59d in ChannelLoadClient::processCmd_DB () 
#6  0x00000000005986c9 in GameAppClient::processTaskCmd_DB () 
#7  0x00000039c5a901e3 in __nanosleep_nocancel () from /lib64/tls/libc.so.6 
#8  0x0000000000000000 in ?? () 
Thread 1 (Thread 182894183104 (LWP 24714)): 
#0  0x00000039c5ac9c5c in epoll_wait () from /lib64/tls/libc.so.6 
#1  0x0000000000620cac in zTCPServer::accept () 
#2  0x00000000005f9c0d in zNetService::serviceCallback () 
#3  0x00000000005f89e3 in zService::main () 
#4  0x0000000000564298 in main ()


本文永久更新地址：[http://www.linuxdiyf.com/linux/18227.html](http://www.linuxdiyf.com/linux/18227.html)









