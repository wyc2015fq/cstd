# Linux运行与控制后台进程的方法：nohup, setsid, &, disown, screen - DoubleLi - 博客园






我们经常会碰到这样的问题，用ssh登录了远程的Linux服务器，运行了一些耗时较长的任务，结果却由于网络等的不稳定导致任务中途失败。
这是由于在用户注销（logout）或者网络断开时，终端会收到 HUP（hangup）信号从而关闭其所有子进程。
解决办法有两种：让进程忽略HUP信号，或让进程运行在新的会话里从而成为不属于此终端的子进程。

下面是对Linux下运行与控制后台进程的各种方法的介绍：
**1.nohup**
顾名思义，nohup的用途就是让提交的命令忽略所有的hangup信号。
使用方法：nohup COMMAND [ARG]...

**2.setsid**
在一个新的会话中运行命令，从而可以避开当前终端发出的HUP信号。
使用方法：setsid COMMAND [ARG]...

**3.&**
可以结合()产生一个新的子shell并在这个子shell中将任务放置到后台运行，从而不受当前shell终端的HUP信号影响。
使用方法：(COMMAND [ARG]... &)

而我通常的使用方式为：
nohup ./filename.sh > filename.log 2>&1 &
三点理由:
1)nohup保障进程不会被hangup信号异常中断；
2)将任务放置到后台运行，不占用当前的终端；
3)将错误输出也打印到log中，默认>只有标准输出，错误输出没有。


**4.控制进程**
通过以下命令，我们可以对放入到后台的命令进行控制

查看当前终端下的后台进程：
直接执行：jobs

将查看到的某个后台进程放回到前台：
直接输入：fg {jobid} //这里的{jobid}是通过jobs命令中看到的进程前[]中的数字。

将当前正在前台运行的进程放到后台运行:
先敲下快捷键：ctrl +z //暂停当前正在运行的进程。
再执行：bg

终止当前正在前台运行的进程：
直接敲下快捷键：ctrl +c

**5.disown**
亡羊补牢，为没有使用nohup与setsid的进程加上忽略HUP信号的功能。
使用方法：
将当前正在前台运行的进程放到后台运行（ctrl+z和bg）;
然后执行disown -h %{jobid} //这里的{jobid}是通过jobs命令中看到的进程前[]中的数字。

**6.通过screen来实现稳定的后台运行**
screen是建立一个新的全屏虚拟会话终端，这个会话只有在手动输入exit的时候才会退出，在这个会话里执行的命令不用担心HUP信号会对我们的进程造成影响，因此也不用给每个命令前都加上“nohup”或“setsid”了，非常适合我们有规划的执行大量的后台任务，可以非常方便的让我们对这些后台任务进行管理。

使用方法：
screen //立即创建并进入一个会话。
screen -dmS {name} //建立一个处于断开模式下的会话，并根据我们的需要指定其会话名称。
screen -list //列出所有会话。
screen -r {name} //进入指定会话。
ctrl +ad //输入快捷键ctrl +a和d，可暂时退出当前会话。
exit //进入指定会话后执行exit即可关闭该会话。

参考资料：[https://www.ibm.com/developerworks/cn/linux/l-cn-nohup/](https://www.ibm.com/developerworks/cn/linux/l-cn-nohup/)









