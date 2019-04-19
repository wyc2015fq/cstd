# ubuntu 后台运行_用户5706034127_新浪博客
|||
			注意区分后台运行，和关闭shell，不挂掉程序，两个的区别
后台运行，不一定会忽略 hangup信号。
nohup、setsid、&和disown
2010-04-02 15:16
在执行命令时，偶尔会碰到需要让对应进程无间断执行的情况。前段时间看到IBM官网的一篇文章，由于不让转载（原文位置：[IBM](http://www.ibm.com/developerworks/cn/linux/l-cn-nohup/)），遂简单记录如下：
我们知道，当用户注销（logout）或者网络断开时，终端会收到
HUP（hangup）信号从而关闭其所有子进程。因此，我们的解决办法就有两种途径：要么让进程忽略 HUP
信号，要么让进程运行在新的会话里从而成为不属于此终端的子进程。
1. nohup
显而易见，nohup命令的功能是使进程忽略hangup信号，从而持续执行。nohup 的使用是十分方便的，只需在要处理的命令前加上
nohup 即可，标准输出和标准错误缺省会被重定向到 nohup.out
文件中。一般我们可在结尾加上"&"来将命令同时放入后台运行，也可用">filename
2>&1"来更改缺省的重定向文件名。
如：nohup myUbuntuSourceSyncCmd.pl >./sources/ubuntu/sync.log
2>&1 &
这样起到了三个效果：进程后台执行；忽略hangup信号；输出重定向。
2. setsid
setsid中的sid指的是session
id，意指以该命令运行的进程是一个新的session，因此其父进程id不属于当前终端。实际上，setsid运行的进程，其父进程id（PPID）为
1（init 进程的 PID）。因此，setsid解决问题用的是第二种途径。
如：setsid myUbuntuSourceSyncCmd.pl >./sources/ubuntu/sync.log
2>&1 &
格式与nohup相仿，后台运行也需加上&，但输出重定向必须手动设置。
3. 括号()与&
&代表后台运行（注意输出并没有被重定向）；此外，我们知道，将一个或多个命名包含在“()”中就能让这些命令在子 shell
中运行中，从而扩展出很多有趣的功能，我们现在要讨论的就是其中之一。
当我们将"&"也放入“()”内之后，我们就会发现所提交的作业并不在作业列表中，也就是说，是无法通过jobs来查看的。
如：(myUbuntuSourceSyncCmd.pl >./sources/ubuntu/sync.log
2>&1 &)
以这种方式运行程序，新提交的进程的父 ID为1，并不是当前终端的进程 ID。因此并不属于当前终端的子进程，从而也就不会受到当前终端的
HUP 信号的影响了。
4. disown
如果我们未加任何处理就已经提交了命令，这时想加 nohup 或者 setsid 已经为时已晚，只能通过作业调度和 disown
来解决这个问题了。让我们来看一下 disown 的帮助信息：
disown [-ar] [-h] [jobspec ...]
Without options, each jobspec is 
removed  from 
the  table  of
active  jobs. 
If  the -h option is given, each jobspec is
not
removed from the table, but is marked so 
that  SIGHUP 
is  not
sent  to the job if the shell receives a
SIGHUP.  If no jobspec
is present, and neither the -a nor the -r option 
is  supplied,
the  current 
job  is  used. 
If no jobspec is supplied, the -a
option means to remove or mark all jobs; the -r
option  without
a  jobspec 
argument  restricts operation to running
jobs.  The
return value is 0 unless a jobspec does 
not  specify  a 
valid
job.
可以看出，我们可以用如下方式来达成我们的目的。
a. 用disown -h jobspec 来使某个作业忽略HUP信号。
b. 用disown -ah 来使所有的作业都忽略HUP信号。
c. 用disown -rh 来使正在运行的作业忽略HUP信号。
需要注意的是，当使用过 disown 之后，会将把目标作业从作业列表中移除，我们将不能再使用jobs来查看它，但是依然能够用ps
-ef查找到它。
但是还有一个问题，这种方法的操作对象是作业，如果我们在运行命令时在结尾加了"&"来使它成为一个作业并在后台运行，那么就万事大吉了，我们可
以通过jobs命令来得到所有作业的列表。但是如果并没有把当前命令作为作业来运行，如何才能得到它的作业号呢？答案就是用
CTRL-z（按住Ctrl键的同时按住z键）了！
CTRL-z 的用途就是将当前进程挂起（Suspend），然后我们就可以用jobs命令来查询它的作业号，再用bg jobspec
来将它放入后台并继续运行。需要注意的是，如果挂起会影响当前进程的运行结果，慎用此方法。
示例：
root@ubuntu-server:/mnt/ftp# jobs
[1]+ 
Running 
./sync_mirror.sh &
root@ubuntu-server:/mnt/ftp# disown -h %1
root@ubuntu-server:/mnt/ftp# ps -ef|grep sync_mirror.sh
root 
8650 22271  0 15:08
pts/0 
00:00:00 grep sync_mirror.sh
root 
16748 22271  0 Mar31
pts/0 
00:00:00 /bin/sh -e ./sync_mirror.sh
另外还有一个命令screen。screen 提供了 ANSI/VT100
的终端模拟器，使它能够在一个真实终端下运行多个全屏的伪终端。screen 的参数很多，具有很强大的功能。本篇不做介绍。
