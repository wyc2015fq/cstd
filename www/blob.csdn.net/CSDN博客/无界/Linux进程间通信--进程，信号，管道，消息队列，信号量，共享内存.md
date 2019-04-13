
# Linux进程间通信--进程，信号，管道，消息队列，信号量，共享内存 - 无界 - CSDN博客

置顶2012年04月19日 22:43:28[21aspnet](https://me.csdn.net/21aspnet)阅读数：43314


**Linux进程间通信--进程，信号，管道，消息队列，信号量，共享内存**
参考：《linux编程从入门到精通》,《Linux C程序设计大全》,《unix环境高级编程》
参考：[C和指针学习](http://blog.csdn.net/21aspnet/article/details/2583048)
说明：本文非常的长，也是为了便于查找和比较，所以放在一起了

Linux 传统的进程间通信有很多，如各类管道、消息队列、内存共享、信号量等等。但它们都无法介于内核态与用户态使用，原因如表
|通信方法|无法介于内核态与用户态的原因|
|管道（不包括命名管道）|局限于父子进程间的通信。|
|消息队列|在硬、软中断中无法无阻塞地接收数据。|
|信号量|无法介于内核态和用户态使用。|
|内存共享|需要信号量辅助，而信号量又无法使用。|
|套接字|在硬、软中断中无法无阻塞地接收数据。|

**一.进程**
**1.进程表**
ps显示正在运行的进程
\# ps -ef
![](https://img-my.csdn.net/uploads/201204/22/1335097783_6983.gif)
TIME 进程目前占用的cpu时间，CMD显示启动进程所使用的命令

\#ps ax
![](https://img-my.csdn.net/uploads/201204/22/1335097943_8807.gif)
STAT表明进程的状态
S 睡眠，s进程是会话期首进程；R 运行；D 等待；T 停止；Z 僵尸；N 低优先级任务,nice；W 分页；
+进程属于前台进程组；l 进程是多线程；<高优先级任务

\#ps -l  或\#ps -al
![](https://img-my.csdn.net/uploads/201204/22/1335099194_7030.gif)
表现良好的程序为nice程序，系统根据进程的nice值决定他的优先级
-f是长格式

**2.父子进程id**
pid当前进程的；
uid当前进程的实际用户
eid当前进程的有效用户
\#include <stdio.h>
\#include <unistd.h>
main()
{
printf("process id=%d\n",getpid());
printf("parent process id=%d\n",getppid());
printf("process group id=%d\n",getpgrp());
printf("calling process's real user id=%d\n",getuid());
printf("calling process's real group id=%d\n",getgid());
printf("calling process's effective user id=%d\n",geteuid());
printf("calling process's effective group id=%d\n",getegid());
}
运行结果：
![](https://img-my.csdn.net/uploads/201204/22/1335087800_7796.gif)

**3.设置进程组id以及进程sleep**
setpgid使当前进程为新进程组的组长
\#include <stdio.h>
\#include <unistd.h>
main()
{
setpgid(0,0);  //设置当前进程为新进程组的组长
sleep(8);         //休眠8秒
}
说明：setpgid(0,0)等价于setpgrp(0,0)
setpgid(0,0)第1个参数用于指定要修改的进程id。如果为0，则指当前进程。第2个参数用于指定新的进程组id。如果为0，则指当前进程。
先运行程序
\#./example13_2
再查看进程
\#ps alef
![](https://img-my.csdn.net/uploads/201204/19/1334846678_4051.gif)
\#ps -ao pid,pgrp,cmd|grep 13_2
或者
\#ps -ao pid,pgrp,cmd
![](https://img-my.csdn.net/uploads/201204/19/1334846691_6615.gif)

**4.子进程**
fork为0说明是父进程
\#include <stdio.h>
\#include <unistd.h>
\#include <stdlib.h>
main()
{
fprintf(stderr,"fork...\n");
if(fork() == 0 )
{
wait();
exit(0);
}
printf("AA");
sleep(10);
exit(0);
}
输出
fork...
AA

注意：
警告： 隐式声明与内建函数 ‘exit’ 不兼容
警告： 隐式声明与内建函数 ‘sprintf’ 不兼容
警告： 隐式声明与内建函数 ‘printf’ 不兼容
加入这两个头文件就可以了！
\#include <stdio.h>
\#include <stdlib.h>

\#ps -ao pid,pgrp,cmd
![](https://img-my.csdn.net/uploads/201204/20/1334876856_4669.gif)
3165就是子进程

\#ps alef![](https://img-my.csdn.net/uploads/201204/20/1334876867_6931.gif)

**5.进程会话**
setsid的调用进程应该不是某个进程组的组长进程；
setsid调用成功后生成新会话，新会话id是调用进程的进程id；
新会话只包含一个进程组一个进程即调用进程，没有控制终端。
setid主要是实现进程的后台运行
\#include <stdio.h>
\#include <unistd.h>
\#include <sys/param.h>
\#include <stdlib.h>
main()
{
int n;
__pid_t nPid;
__pid_t nGroupId;
if((nPid = fork()) < 0)
{
perror("fork");
exit(0);
}
if(nPid != 0)//父进程
exit(0);
nGroupId = setsid();//新会话
if(nGroupId == -1)
{
perror("setsid");
exit(0);
}
for(n=0;n<10;n++)
sleep(3);
}

修改后的程序
\#include <stdio.h>
\#include <unistd.h>
\#include <sys/param.h>
\#include <stdlib.h>
main()
{
int n;
__pid_t nPid;
__pid_t nGroupId;
if((nPid = fork()) < 0)
{
printf("Error");
exit(0);
}
if(nPid != 0)//父进程
{
perror("aaa");
wait(0);
//exit(0);//父进程退出
}
else
{
perror("bbb");
nGroupId = setsid();//新会话
if(nGroupId == -1)
{
perror("setsid");
exit(0);
}
perror("fff");
sleep(3);
perror("ggg");
exit(0);
}
perror("kkk");
}
**父进程必须调用wait等待子进程推出，如果没有子进程退出exit，则wait进入阻塞！**
**6.进程的控制终端**
\#tty
![](https://img-my.csdn.net/uploads/201204/20/1334902192_2932.gif)
在secureCRT中观看其他的会输出
/dev/pts/1等依次类推

\#ps -ax
查看进程的控制终端
![](https://img-my.csdn.net/uploads/201204/20/1334909930_2070.gif)
有列tty的就是控制终端，有值表明进程有控制终端，无则表明是后台进程。
延伸：[php的POSIX 函数以及进程测试](http://blog.csdn.net/21aspnet/article/details/7482733)

**7.进程的状态**
可运行；
等待；
暂停；
僵尸；
进程在终止前向父进程发送SIGCLD信号，父进程调用wait等待子进程的退出！
如果，父进程没有调用wait而子进程已经退出，那么父进程成为僵尸进程；
如果，父进程没有等子进程退出自己已经先退出，那么子进程成为孤儿进程；
通过top命令看到
![](https://img-my.csdn.net/uploads/201204/20/1334911080_1364.gif)

**8.进程的优先级**
优先级数值越低，则优先级越高！
优先级由优先级别（PR）+进程的谦让值（NI）联合确定。
PR值是由父进程继承而来，是不可修改的。
Linux提供nice系统调用修改自身的NI值；setpriority系统调用可以修改其他进程以及进程组的NI值。
\#include <unistd.h>
\#include <errno.h>
\#include <sys/resource.h>
\#include <stdlib.h>
\#include <stdio.h>
main()
{
int nPr;
if(nice(3) == -1)//进程的谦让值是3，优先级降低
{
perror("nice");
exit(0);
}
errno = 0;
nPr = getpriority(PRIO_PROCESS,getpid());//获取当前进程的谦让值
if(errno != 0)
{
perror("getpriority");
exit(0);
}
printf("priority is %d\n",nPr);
}
输出：
priority is 3

**9.用fork创建进程**
调用fork一次返回2次，分别在父进程和子进程中返回，父进程中其返回值是子进程的进程标识符，子进程中其返回值是0。
\#include <stdio.h>
\#include <signal.h>
\#include <sys/types.h>
\#include <unistd.h>
\#include <stdlib.h>
int main()
{
pid_t pid;
signal(SIGCLD, SIG_IGN);//信号处理，忽略SIGCLD信号，避免形成僵尸进程
switch(pid=fork())//创建子进程
{
case -1:
perror("fork");
break;
case 0://子进程
printf("子进程：进程ID=%d\n",getpid());
printf("pid=%d\n", pid);
exit(0);
break;
default://父进程
printf("父进程：进程ID=%d\n",getpid());
printf("pid=%d\n", pid);
sleep(5);
break;
}
}
（注意保存为UTF-8格式，因为有中文）
输出：
![](https://img-my.csdn.net/uploads/201204/22/1335102540_9081.gif)

**10.vfork和fork之间的区别**
vfork用于创建一个新进程，而该新进程的目的是exec一个新进程，vfork和fork一样都创建一个子进程，但是它并不将父进程的地址空间完全复制到子进程中，不会复制页表。因为子进程会立即调用exec，于是也就不会存放该地址空间。不过在子进程中调用exec或exit之前，他在父进程的空间中运行。
为什么会有vfork，因为以前的fork当它创建一个子进程时，将会创建一个新的地址空间，并且拷贝父进程的资源，而往往在子进程中会执行exec调用，这样，前面的拷贝工作就是白费力气了，这种情况下，聪明的人就想出了vfork，它产生的子进程刚开始暂时与父进程共享地址空间（其实就是线程的概念了），因为这时候子进程在父进程的地址空间中运行，所以子进程不能进行写操作，并且在儿子“霸占”着老子的房子时候，要委屈老子一下了，让他在外面歇着（阻塞），一旦儿子执行了exec或者exit后，相当于儿子买了自己的房子了，这时候就相当于分家了。
vfork和fork之间的另一个区别是： vfork保证子进程先运行，在她调用exec或exit之后父进程才可能被调度运行。如果在调用这两个函数之前子进程依赖于父进程的进一步动作，则会导致死锁。
由此可见，这个系统调用是用来启动一个新的应用程序。其次，子进程在vfork()返回后直接运行在父进程的栈空间，并使用父进程的内存和数据。这意味着子进程可能破坏父进程的数据结构或栈，造成失败。
为了避免这些问题，需要确保一旦调用vfork()，子进程就不从当前的栈框架中返回，并且如果子进程改变了父进程的数据结构就不能调用exit函数。子进程还必须避免改变全局数据结构或全局变量中的任何信息，因为这些改变都有可能使父进程不能继续。
通常，如果应用程序不是在fork()之后立即调用exec()，就有必要在fork()被替换成vfork()之前做仔细的检查。
用fork函数创建子进程后，子进程往往要调用一种exec函数以执行另一个程序，当进程调用一种exec函数时，该进程完全由新程序代换，而新程序则从其main函数开始执行，因为调用exec并不创建新进程，所以前后的进程id 并未改变，exec只是用另一个新程序替换了当前进程的正文，数据，堆和栈段。

**11.exec**
清除父进程的可执行代码影像，用新代码覆盖父进程。
参考：[Linux exec与重定向](http://blog.csdn.net/21aspnet/article/details/7487770)
\#include <stdio.h>
\#include <sys/types.h>
\#include <unistd.h>
main()
{
pid_t pid;
char *para[]={"ls","-a",NULL};
if((pid = fork()) < 0)
{
perror("fork");
exit(0);
}
if(pid == 0)
{
if(execl("/bin/ls","ls","-l",(char *)0) == -1)
{
perror("execl");
exit(0);
}
}
if((pid = fork()) < 0)
{
perror("fork");
exit(0);
}
if(pid == 0)
{
if(execv("/bin/ls",para) == -1)
{
perror("execl");
exit(0);
}
}
return;
}

**12.system创建进程**
system系统调用是为了方便调用外部程序，执行完毕后返回调用进程。
\#include <stdio.h>
\#include <stdlib.h>
main()
{
printf("call ls return %d\n",system("ls -l"));
}
输出：
![](https://img-my.csdn.net/uploads/201204/22/1335106308_3851.gif)

**13.退出进程**
调用exit退出进程
调用wait等待进程退出
\#include <stdio.h>
\#include <sys/types.h>
\#include <sys/wait.h>
\#include <unistd.h>
\#include <signal.h>
\#include <stdlib.h>
void handle_sigcld(int signo)
{
pid_t pid;
int status;
if((pid = wait(&status)) != -1)
{
printf("子进程%d退出\n",pid);
}
if(WIFEXITED(status))
{
printf("子进程返回%d\n",WEXITSTATUS(status));
}
if(WIFSIGNALED(status))
{
printf("子进程被信号ť%d结束\n",WTERMSIG(status));
}
}
main()
{
pid_t pid;
signal(SIGCLD,handle_sigcld);
if((pid = fork()) < 0)
{
perror("fork");
exit(0);
}
if(pid == 0)
{
exit(123);
}
sleep(5);
}
输出：
![](https://img-my.csdn.net/uploads/201204/22/1335106777_6531.gif)

**二.信号**
信号又称软终端，通知程序发生异步事件，程序执行中随时被各种信号中断，进程可以忽略该信号，也可以中断当前程序转而去处理信号，引起信号原因：
1).程序中执行错误码；
2).其他进程发送来的；
3).用户通过控制终端发送来；
4).子进程结束时向父进程发送SIGCLD；
5).定时器生产的SIGALRM；

**1.信号分类**
\#kill -l
获取信号列表，信号值)  信号名
![](https://img-my.csdn.net/uploads/201204/24/1335255634_2170.jpg)
1-31是不可靠信号（可能丢失）；32-64是可靠信号（操作系统保证不丢失）
信号列表参考：[http://blog.csdn.net/21aspnet/article/details/7494565](http://blog.csdn.net/21aspnet/article/details/7494565)
信号安装：定义进程收到信号后的处理方法
**signal**系统调用安装信号
\#include <stdio.h>
\#include <signal.h>
void HandleSigint(int signo)//信号处理函数
{
printf("receive signal %d\n",signo);
}
main()
{
if(signal(SIGINT,HandleSigint)  == SIG_ERR)//安装信号
{
perror("signal");
exit(0);
}
pause();//暂停进程等待信号
}
输出：
按Ctrl+C
receive signal 2

**sigaction**系统调用(更多的控制，完全可以替代signal)
\#include <stdio.h>
\#include <signal.h>
void HandleSigint(int signo,siginfo_t *info,void *none)
{
printf("receive signal %d,addtional data is %d\n",signo,info->si_value.sival_int);
}
main()
{
struct sigaction act,oact;//信号处理函数结构
memset(&act,0x00,sizeof(struct sigaction));//清空结构
sigemptyset(&act.sa_mask);//清空信号处理掩码
act.sa_sigaction = HandleSigint;//定义信号处理函数
act.sa_flags = SA_SIGINFO;//指定发送信号时可以附加数据
if(sigaction(SIGINT,&act,&oact) == -1)//安装
{
perror("sigaction");
exit(0);
}
pause();//暂停
}

输出：
按Ctrl+C
receive signal 2,addtional data is 12364176
**2.信号处理方式**3种：
1.忽略信号-大多可以忽略，只有SIGKILL和SIGSTOP除外；
2.捕捉信号-先安装
3.默认操作

**3.信号阻塞**
阻塞是指系统内核暂停向进程发送指定信号，由内核对进程接收到的信号缓存，直到解除阻塞为止。
信号3种进入阻塞的情况：
1.信号处理函数执行过程中，该信号将阻塞；
2.通过sigaction信号安装，如果设置了sa_mask阻塞信号集；
3.通过系统调用**sigprocmask**
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
参数：
how：用于指定信号修改的方式，可能选择有三种
SIG_BLOCK //加入信号到进程屏蔽。
SIG_UNBLOCK //从进程屏蔽里将信号删除。
SIG_SETMASK //将set的值设定为新的进程屏蔽。
set：为指向信号集的指针，在此专指新设的信号集，如果仅想读取现在的屏蔽值，可将其置为NULL。
oldset：也是指向信号集的指针，在此存放原来的信号集。
\#include <stdio.h>
\#include <signal.h>
void handle_sigint(int signo)
{
printf("receive signal %d\n",signo);
}
main()
{
sigset_t mask;
sigset_t omask;
signal(SIGINT,handle_sigint);
sigemptyset(&mask);//清空信号处理掩码
sigaddset(&mask,SIGINT);//向掩码中增加信号
sigprocmask(SIG_BLOCK,&mask,&omask);//设置掩码，设置完成后SIGINT信号被阻塞
sleep(10);
sigprocmask(SIG_SETMASK,&omask,NULL);//恢复原有的信号处理掩码
}
输出：如果不输入Ctrl+C则10秒后程序结束；如果期间有Ctrl+C则会10秒结束，之后输出Creceive signal 2

注意：子进程会继承父进程的信号掩码

**4.信号集操作**
对信号集中所有信号处理
数据类型 sigset_t
清空信号集sigemptyset
信号集填充全部信号sigfillset
信号集增加信号sigaddset
信号集中删除信号sigdelset
判断信号集是否包含某信号的sigismember

**5.未决信号**
信号产生后到信号被接收进程处理前的过渡状态，未决状态时间很短。
sigprocmask阻塞某种信号，则向进程发送这种信号处于未决状态。
sigpending获取当前进程中处于未决状态的信号
\#include <stdio.h>
\#include <signal.h>
void handle_sigint(int signo)
{
printf("receive signal %d\n",signo);
}
main()
{
sigset_t mask;
sigset_t omask;
sigset_t pmask;
signal(SIGINT,handle_sigint);
sigemptyset(&mask);
sigaddset(&mask,SIGINT);
sigprocmask(SIG_BLOCK,&mask,&omask);
sleep(10);
if(sigpending(&pmask) < 0)//获取当前未决的信号集
{
perror("sigpending");
exit(0);
}
if(sigismember(&pmask,SIGINT))//判定SIGINT是否在未决信号集中
{
printf("SIGINT signal is pending.\n");
}
}

**6.等待信号**
阻塞式系统如果没有符合条件的数据将休眠，直到数据到来，例如socket上读取数据。有2种状态可以中断该操作
1.网络上有数据，读操作获取数据后返回
2.当前进程接收信号，读操作被中断返回失败，错误码errno为EINTR
**pause**系统调用可以让程序暂停执行进入休眠，等待信号到来。
\#include <stdio.h>
\#include <signal.h>
int nInterrupt;
void handle_sigint(int signo)
{
nInterrupt = 1;
}
main()
{
sigset_t mask,omask;
signal(SIGINT,handle_sigint);
sigemptyset(&mask);
sigaddset(&mask,SIGINT);
sigprocmask(SIG_BLOCK,&mask,&omask);//阻塞
nInterrupt = 0;
while(!nInterrupt)//循环调用sigsuspend等待信号，直到收到SIGINT，nInterrupt为1
sigsuspend(&omask);//阻塞信号直到有信号到达
printf("process return.\n");
}

**7.信号发送**
两种方式
**kill**不可附加数据
**sigqueue**可附加数据
\#include <stdio.h>
\#include <stdlib.h>
\#include <signal.h>
void HandleSigint(int signo,siginfo_t *info,void *none)
{
printf("receive addtional data is %d\n",info->si_value.sival_int);
exit(0);
}
main()
{
int pid;
struct sigaction act;
union sigval sigvalPara;
if((pid = fork()) == 0)
{
memset(&act,0x00,sizeof(struct sigaction));
sigemptyset(&act.sa_mask);
act.sa_sigaction = HandleSigint;
act.sa_flags = SA_SIGINFO;
if(sigaction(SIGINT,&act,NULL) == -1)
{
perror("sigaction");
exit(0);
}
pause();//暂停子进程，等待信号
}
else
{
sigvalPara.sival_int = 123;//设置附加数据为123
if(sigqueue(pid,SIGINT,sigvalPara) == -1)//向子进程发送信号SIGINT，并附加数据
{
perror("sigqueue");
exit(0);
}
}
}
输出：receive addtional data is 123

**8.sigalarm信号**
阻塞式系统调用，为避免无限期等待，可以设置定时器信号，alarm调用
\#include <stdio.h>
\#include <stdlib.h>
\#include <signal.h>
int timeout;
void Handle_Alarm(int signo)
{
timeout = 1;
printf("SIGALRM received.\n");
}
main()
{
if(signal(SIGALRM,Handle_Alarm) ==SIG_ERR )//安装SIGALRM信号
{
perror("signal");
exit(0);
}
timeout = 0;//设置超时标志为0
alarm(10);//启动定时器
pause();//阻塞进程，等待信号
if(timeout)//如果超时
{
printf("Pause time out.\n");
}
}
输出：
SIGALRM received.
Pause time out.

**9.sigcld信号**
父进程捕获子进程的退出信号
子进程发送SIGCLD信号进入僵尸状态；父进程接收到该信号处理，子进程结束
\#include <stdio.h>
\#include <stdlib.h>
\#include <signal.h>
void Handle_Sigcld(int signo)
{
int pid,status;
pid = waitpid(-1,&status,0);
printf("Child process %d exit with code %d\n",pid,status);
}
main()
{
int i,pid;
signal(SIGCLD,Handle_Sigcld);
for(i=0;i<5;i++)
{
if((pid = fork()) == 0)//子进程
{
srand(getpid());//产生随机数
exit((int)(rand()/1024));//退出子进程，退出码为上步随机数
}
else
{//父进程
sleep(1);//休眠
continue;//继续
}
}
}
输出：
![](https://img-my.csdn.net/uploads/201204/25/1335344896_5948.jpg)

**三.管道**
单向，一段输入，另一端输出，先进先出FIFO。管道也是文件。管道大小4096字节。
特点：管道满时，写阻塞；空时，读阻塞。
分类：普通管道（仅父子进程间通信）位于内存，命名管道位于文件系统，没有亲缘关系管道只要知道管道名也可以通讯。
**1.pipe建立管道**
\#include <stdio.h>
\#include <stdlib.h>
\#include <unistd.h>
\#include <string.h>
\#include <sys/wait.h>
int main(int argc, char *argv[])
{
int pfd[2];//保存打开管道后的两个文件描述符
pid_t cpid;//保存进程标识符
char buf;
if(argc != 2)//判断命令行参数是否符合
{
fprintf(stderr,"Usage: %s <string>\n",argv[0]);
exit(0);
}
if (pipe(pfd) == -1)//建立管道
{
perror("pipe");
exit(EXIT_FAILURE);
}
cpid = fork();
if (cpid == -1)
{
perror("fork");
exit(EXIT_FAILURE);
}
if (cpid == 0)//子进程
{
close(pfd[1]);          //关闭管道写，引用计数-1
while (read(pfd[0], &buf, 1) > 0)//从管道循环读取数据
write(STDOUT_FILENO, &buf, 1);//输出读到的数据
write(STDOUT_FILENO, "\n", 1);//输出从管道读取的数据
close(pfd[0]);//关闭管道读，引用计数-1
exit(0);
}
else
{//父进程
close(pfd[0]);
write(pfd[1], argv[1], strlen(argv[1]));//向管道写入命令行参数1
close(pfd[1]);
wait(NULL);           //等待子进程退出
exit(0);
}
}
说明：每调用一次fork  都要关闭一次进程描述符
执行
\#./a.out   www
输出
\#www

**2.dup**
\#include <stdio.h>
\#include <stdlib.h>
\#include <unistd.h>
\#include <string.h>
\#include <sys/wait.h>
int main()
{
int pfds[2];
if ( pipe(pfds) == 0 )
{
if ( fork() == 0 )//子进程
{
close(1);//关闭标准输出
dup2( pfds[1], 1 );//管道的写文件描述符复制到进程的输出
close( pfds[0] );//关闭管道读
execlp( "ls", "ls","-l", NULL );//执行ls -l 输出写入管道
}
else
{
close(0);
dup2( pfds[0], 0 );//管道的读文件描述符复制到进程的输入
close( pfds[1] );
execlp( "wc", "wc", "-l", NULL );//执行wc -l 将管道读取数据作为wc命令的输入
}
}
return 0;
}
输出：129
[Linux execlp函数](http://blog.csdn.net/21aspnet/article/details/7568067)说明：相当于执行\# ls -l |wc -l统计当前目录下文件数量；ls -l 列出当前文件详细信息；wc -l
wc参考[http://blog.csdn.net/21aspnet/article/details/7515442](http://blog.csdn.net/21aspnet/article/details/7515442)
linux命令集锦[http://blog.csdn.net/21aspnet/article/details/1534099](http://blog.csdn.net/21aspnet/article/details/1534099)
linux常用命令[http://linux.chinaitlab.com/special/linuxcom/](http://linux.chinaitlab.com/special/linuxcom/)
**3.popen() 函数**
用于创建一个管道，其内部实现为调用 fork 产生一个子进程，执行一个 shell 以运行命令来开启一个进程，这个进程必须由 pclose() 函数关闭。
\#include <stdlib.h>
\#include <stdio.h>
\#include <string.h>
\#include <unistd.h>
int main( void )
{
FILE   *stream;//文件流
char   buf[1024];//读写缓冲区
memset( buf, '\0', sizeof(buf) );//清空
stream = popen( "wc -l", "w" );
for(;;)
{
memset(buf,0x00,sizeof(buf));
scanf("%s",buf);//接受输入
if(strcmp(buf,"k") == 0)//如果是k就退出
{
break;
}
fprintf(stream,"%s\n",buf);//写入
}
pclose( stream );//关闭
return 0;
}
输出：
![](https://img-my.csdn.net/uploads/201205/16/1337165220_5019.gif)

**4.命名管道**
**mknod**
mknod 管道名称 p
\#include <sys/types.h>
\#include <sys/stat.h>
\#include <fcntl.h>
\#include <unistd.h>
int mknod(const char *pathname, mode_t mode, dev_t dev);

**mkfifo**
mkfifo -m 权限 管道名称
\#include<sys/types.h>
\#include<sys/stat.h>

int mkfifo(const char * pathname,mode_t mode);
mknod和mkfifo的区别
mknod系统调用会产生由参数path锁指定的文件，生成文件类型和访问权限由参数mode决定。
在很多unix的版本中有一个C库函数mkfifo，与mknod不同的是多数情况下mkfifo不要求用户有超级用户的权限

利用命令创建命名管道p1.
\#mkfifo -m 0644 p1
\#mknod p2 p
\#ll
![](https://img-my.csdn.net/uploads/201205/16/1337166491_2946.gif)
\#include <stdio.h>
\#include <stdlib.h>
\#include <sys/types.h>
\#include <sys/stat.h>
main()
{
if(mkfifo("p1",0644) < 0)
{
perror("mkfifo");
exit(-1);
}
return;
}

**5.管道读写**
通过open打开，默认是阻塞方式打开，如果open指定O_NONBLOCK则以非阻塞打开。
O_NONBLOCK和O_NDELAY所产生的结果都是使I/O变成非搁置模式(non-blocking)，在读取不到数据或是写入缓冲区已满会马上return，而不会搁置程序动作，直到有数据或写入完成。

它们的差别在于设立O_NDELAY会使I/O函式马上回传0，但是又衍生出一个问题，因为读取到档案结尾时所回传的也是0，这样无法得知是哪中情况；因此，O_NONBLOCK就产生出来，它在读取不到数据时会回传-1，并且设置errno为EAGAIN。

不过需要注意的是，在GNU C中O_NDELAY只是为了与BSD的程序兼容，实际上是使用O_NONBLOCK作为宏定义，而且O_NONBLOCK除了在ioctl中使用，还可以在open时设定。
\#include <stdlib.h>
\#include <stdio.h>
\#include <unistd.h>
\#include <sys/stat.h>
\#include <sys/types.h>
\#include <fcntl.h>
main()
{
int fd;
if((fd = open("p1",O_RRONLY,0)) < 0)//只读打开管道
// if((fd = open("p1",O_WRONLY,0)) < 0)//只写打开管道
{
perror("open");
exit(-1);
}
printf("open fifo p1 for write success!\n");
close(fd);
}

**四.IPC对象**
查看ipc对象信息
\#ipcs
![](https://img-my.csdn.net/uploads/201205/16/1337169016_4277.gif)
查看全部ipc对象信息
\#ipcs -a
查看消息队列信息
\#ipcs -q
查看共享内存信息
\#ipcs -m
查看信号量信息
\#ipcs -s
删除IPC对象的ipcrm
ipcrm -[smq] ID 或者ipcrm -[SMQ] Key
-q  -Q删除消息队列信息  例如ipcrm -q 98307
-m -M删除共享内存信息
-s -S删除信号量信息

**ftok函数**
产生一个唯一的关键字值
ftok原型如下：
key_t ftok( char * fname, int id )

fname就是你指定的文件名(该文件必须是存在而且可以访问的)，id是子序号，虽然为int，但是只有8个比特被使用(0-255)。

当成功执行的时候，一个key_t值将会被返回，否则 -1 被返回。

在一般的UNIX实现中，是将文件的索引节点号取出，前面加上子序号得到key_t的返回值。如指定文件的索引节点号为65538，换算成16进制为 0x010002，而你指定的ID值为38，换算成16进制为0x26，则最后的key_t返回值为0x26010002。
查询文件索引节点号的方法是： ls -i

以下为测试程序：
ftok.c
\#include <stdio.h>
\#include <sys/types.h>
\#include <sys/ipc.h>

\#define IPCKEY 0x11
int main( void )
{
int i=0;
for ( i = 1; i < 256; ++ i )
printf( "key = %x\n", ftok( "/tmp", i ) );

return 0;
}
\#ls -i ftok.c
\#./a.out
![](https://img-my.csdn.net/uploads/201205/16/1337170029_1006.gif)

**五.消息队列**
消息队列是先进先出FIFO原则
**1.消息结构模板**
strut msgbuf
{
long int  mtype;//消息类型
char mtext[1];//消息内容
}

**2.msgget创建消息**
\#include <sys/msg.h>
int msgget(key_t key, int flag);
此函数返回key指定消息的标识符
key 一般有ftok函数产生 ，该函数为key_t ftok(const char *pathname, int proj_id)；
该函数把从pathname导出的信息与id低8位组合成一个整数IPC键， 调用时pathname必须存在，若不存在ftok调用失败，返回-1，成功返回该整数IPC键值
flag 为该消息队列的读写权限组合，可以与IPC_CREAT 或IPC_EXCL相与，其中创建对列时都要使用IPC_CREAT,其中IPC_CREAT|IPC_EXCL含义是若已有该队列则返回错误
此函数成功时，返回非负队列标识符；失败时返回-1

\#include <stdio.h>
\#include <stdlib.h>
\#include <errno.h>
\#include <sys/types.h>
\#include <sys/ipc.h>
\#include <sys/msg.h>
main()
{
key_t lKey;
int nMsgId;
if((lKey = ftok("/etc/profile",1)) == -1)
{
perror("ftok");
exit(1);
}
//带参数IPC_CREAT和IPC_EXCL，如果队列不存在则创建队列，已存在则返回EEXIST
if((nMsgId = msgget(lKey,IPC_CREAT|IPC_EXCL|0666)) == -1)
{
if(errno != EEXIST)//创建失败且不是由于队列已存在
{
perror("msgget");
exit(2);
}
if((nMsgId = msgget(lKey,0)) == -1)//已存在
{
perror("msgget");
exit(3);
}
}
printf("MsgID=%d\n",nMsgId);
return 0;
}
![](https://img-my.csdn.net/uploads/201205/16/1337171342_7361.gif)

**3.msgsnd消息发送**
int msgsnd(int msqid, const void *ptr, size_t length, int flag);
此函数发送消息到指定的消息对列
\#include <stdio.h>
\#include <stdlib.h>
\#include <errno.h>
\#include <sys/types.h>
\#include <sys/ipc.h>
\#include <sys/msg.h>
\#include <string.h>
typedef struct
{
long int nType;
char szText[256];
}MSG;
main()
{
key_t lKey;
int nMsgId;
MSG msg;
if((lKey = ftok("/etc/profile",1)) == -1)//生成键值
{
perror("ftok");
exit(1);
}
if((nMsgId = msgget(lKey,IPC_CREAT|IPC_EXCL|0666)) == -1)//创建消息队列
{
if(errno != EEXIST)
{
perror("msgget");
exit(2);
}
if((nMsgId = msgget(lKey,0)) == -1)
{
perror("msgget");
exit(3);
}
}
memset(&msg,0x00,sizeof(MSG));//清空队列
msg.nType = 2;//指定消息类型为2
memcpy(msg.szText,"123456",6);//指定消息内容
if(msgsnd(nMsgId,(const void *)&msg,strlen(msg.szText),IPC_NOWAIT) < 0)//非阻塞发送消息
{
perror("msgsnd");
}
return 0;
}
![](https://img-my.csdn.net/uploads/201205/16/1337173062_3433.gif)
队列中已经有一条消息，长度6字节

4.msgrcv消息发送
\#include <stdio.h>
\#include <stdlib.h>
\#include <errno.h>
\#include <sys/types.h>
\#include <sys/ipc.h>
\#include <sys/msg.h>
\#include <string.h>
typedef struct
{
long int nType;
char szText[256];
}MSG;
main()
{
key_t lKey;
int n,nMsgId;
MSG msg;
if((lKey = ftok("/etc/profile",1)) == -1)
{
perror("ftok");
exit(1);
}
if((nMsgId = msgget(lKey,0)) == -1)
{
perror("ftok");
exit(2);
}
memset(&msg,0x00,sizeof(MSG));
if((n = msgrcv(nMsgId,(void *)&msg,sizeof(msg.szText),2L,0)) < 0)//从队列接收消息，读出以后就不存在了
{
perror("msgrcv");
}
else
{
printf("msgrcv return length=[%d] text=[%s]\n",n,msg.szText);//输出
}
return 0;
}
输出：
msgrcv return length=[6] text=[123456]
**5.msgctl控制消息**
int msgctl(int msqid, int cmd, struct msqid_ds *buf);
消息队列控制函数
其中msqid为消息队列描述符
cmd有以下三种：
IPC_RMID：删除msgid指定的消息队列，当前在该队列上的任何消息都被丢弃，对于该命令，buf参数可忽略
IPC_SET:设置消息队列msgid_ds结构体的四个成员：msg_perm.uid，msg_perm_gid，msg_perm.mode和msg_qbytes。它们的值来自由buf指向的结构体中的相应成员。
IPC_STAT:给调用者通过buf返回指定消息队列当前对应msgid_ds结构体
函数执行成功返回0，失败返回-1

\#include <stdio.h>
\#include <stdlib.h>
\#include <errno.h>
\#include <sys/types.h>
\#include <sys/ipc.h>
\#include <sys/msg.h>
\#include <string.h>
typedef struct
{
long int nType;
char szText[256];
}MSG;
main()
{
key_t lKey;
int n,nMsgId;
MSG msg;
struct msqid_ds qds;
if((lKey = ftok("/etc/profile",1)) == -1)
{
perror("ftok");
exit(1);
}
if((nMsgId = msgget(lKey,0)) == -1)
{
perror("ftok");
exit(2);
}
memset(&qds,0x00,sizeof(struct msqid_ds));
if(msgctl(nMsgId,IPC_STAT,&qds) < 0)//获取消息队列属性，获取状态放pds中
{
perror("msgctl IPC_STAT");
exit(3);
}
printf("msg_perm.mode=%d\n",qds.msg_perm.mode);
qds.msg_perm.mode &= (~0222);//去除消息队列的写权限
if(msgctl(nMsgId,IPC_SET,&qds) < 0)//设置消息队列权限
{
perror("msgctl IPC_SET");
exit(4);
}
memset(&msg,0x00,sizeof(MSG));
msg.nType = 2;
memcpy(msg.szText,"12345",5);
if(msgsnd(nMsgId,(void *)&msg,5,0) < 0)//发送消息
{
perror("msgsnd");
}
if(msgctl(nMsgId,IPC_RMID,NULL) < 0)//删除消息
{
perror("msgctl IPC_RMID");
exit(5);
}
return 0;
}
说明： (~0222)取反后做与实际上就是去除其他用户的写权限，在C语言中，八进制常用用前缀表示

**六.共享内存**
共享内存是分配一块能被其他进程访问的内存，实现是通过将内存去映射到共享它的进程的地址空间，使这些进程间的数据传送不再涉及内核，即，进程间通信不需要通过进入内核的系统调用来实现；
共享内存与其他的进程间通信最大的优点是：数据的复制只有两次，一次是从输入文件到共享内存区，一次从共享内存区到输出文件
而其他的则是需要复制4次：服务器将输入文件读入自己的进程空间，再从自己的进程空间写入管道/消息队列等；客户进程从管道/消息队列中读出数据到自己的进程空间，最后输出到客户指定的文件中；
要使用共享内存，应该有如下步骤:
1.开辟一块共享内存     shmget()
2.允许本进程使用共某块共享内存  shmat()
3.写入/读出
4.禁止本进程使用这块共享内存   shmdt()
5.删除这块共享内存     shmctl()或者命令行下ipcrm

**1.shmget创建共享内存**
\#include <sys/shm.h>
int    shmget( key_t shmkey , int shmsiz , int flag );
shmget()是用来开辟/指向一块共享内存的函数。参数定义如下：
key_t shmkey 是这块共享内存的标识符。如果是父子关系的进程间通信的话，这个标识符用IPC_PRIVATE来代替
int shmsiz 是这块内存的大小．
int flag 是这块内存的模式(mode)以及权限标识
模式可取如下值：       新建：IPC_CREAT
使用已开辟的内存：IPC_ALLOC
如果标识符以存在，则返回错误值：IPC_EXCL
然后将“模式” 和“权限标识”进行“或”运算，做为第三个参数
如：    IPC_CREAT| IPC_EXCL | 0666
这个函数成功时返回共享内存的ID，失败时返回-1。
\#include <stdio.h>
\#include <stdlib.h>
\#include <sys/types.h>
\#include <sys/ipc.h>
\#include <sys/shm.h>
main()
{
key_t lKey;
int nShmId;
if((lKey = ftok("/etc/profile",1)) < 0)
{
perror("ftok");
exit(1);
}
if((nShmId = shmget(lKey,256,IPC_CREAT|0666)) == -1)//创建
{
perror("shmget");
exit(2);
}
printf("Shmid=%d\n",nShmId);
return 0;
}

**2.shmat映射共享内存**
shmat()是用来允许本进程访问一块共享内存的函数。
void *shmat(int _shmid,_const void *_shmaddr,int _shmflg);
int shmid是那块共享内存的ID。
char *shmaddr是共享内存的起始地址
int shmflag是本进程对该内存的操作模式。如果是SHM_RDONLY的话，就是只读模式。其它的是读写模式
成功时，这个函数返回共享内存的起始地址。失败时返回-1。
\#include <stdio.h>
\#include <stdlib.h>
\#include <sys/types.h>
\#include <sys/ipc.h>
\#include <sys/shm.h>
\#include <string.h>
typedef struct
{
int n;
char str[256];
} ShmStru;
main()
{
key_t lKey;
int nShmId;
ShmStru *pstru;
if((lKey = ftok("/etc/profile",2)) < 0)
{
perror("ftok");
exit(1);
}
if((nShmId = shmget(lKey,sizeof(ShmStru),IPC_CREAT|0666)) == -1)//创建共享内存
{
perror("shmget");
exit(2);
}
if((pstru = shmat(nShmId,NULL,0)) == (void *)-1)//映射共享内存到本地
{
perror("shmat");
exit(3);
}
pstru->n = 1;//修改共享内存
strcpy(pstru->str,"123456");//向共享内存写入数据
if( shmdt(pstru) == -1)//解除共享内存映射
{
perror("shmdt");
exit(4);
}
return 0;
}
**3.shmdt删除共享内存映射**
shmdt()解除共享内存与进程地址空间的映射
int shmdt(_const void * _shmaddr);
参数char *shmaddr是那块共享内存的起始地址
成功时返回0。失败时返回-1。
**4.shmctl控制共享内存映射**
shmctl()函数如下：
\#include <sys/types.h>
\#include <sys/ipc.h>
\#include <sys/shm.h>

int    shmctl( int shmid , int cmd , struct shmid_ds *buf );
int shmid是共享内存的ID。
int cmd是控制命令，可取值如下：
IPC_STAT        得到共享内存的状态
IPC_SET           改变共享内存的状态
IPC_RMID        删除共享内存
struct shmid_ds *buf是一个结构体指针。IPC_STAT的时候，取得的状态放在这个结构体中。如果要改变共享内存的状态，用这个结构体指定
返回值：成功：0
失败：-1

在使用共享内存，结束程序退出后。如果你没在程序中用shmctl()删除共享内存的话，一定要在命令行下用ipcrm命令删除这块共享内存。你要是不管的话，它就一直在那儿放着了。
简单解释一下ipcs命令和ipcrm命令。

取得ipc信息：
ipcs [-m|-q|-s]
-m      输出有关共享内存(shared memory)的信息
-q      输出有关信息队列(message queue)的信息
-s      输出有关“遮断器”(semaphore)的信息
%ipcs -m

删除ipc
ipcrm -m|-q|-s shm_id
%ipcrm -m 105

\#include <stdio.h>
\#include <stdlib.h>
\#include <errno.h>
\#include <sys/types.h>
\#include <sys/ipc.h>
\#include <sys/shm.h>
\#include <string.h>
typedef struct
{
int n;
char str[256];
} ShmStru;
main()
{
key_t lKey;
int nShmId;
struct shmid_ds sds;
if((lKey = ftok("/etc/profile",3)) == -1)
{
perror("ftok");
exit(1);
}
if((nShmId = shmget(lKey,sizeof(ShmStru),IPC_CREAT|0666)) == -1)
{
perror("shmget");
exit(2);
}
memset(&sds,0x00,sizeof(struct shmid_ds));
if(shmctl(nShmId,IPC_STAT,&sds) < 0)//获取共享内存属性
{
perror("shmctl IPC_STAT");
exit(3);
}
printf("First shm_perm.mode=0%o\n",sds.shm_perm.mode);
sds.shm_perm.mode &= (~0002);//去除其他用户的写权限
if(shmctl(nShmId,IPC_SET,&sds) < 0)
{
perror("shmctl IPC_SET");
exit(4);
}
memset(&sds,0x00,sizeof(struct shmid_ds));
if(shmctl(nShmId,IPC_STAT,&sds) < 0)
{
perror("shmctl IPC_STAT");
exit(5);
}
printf("Second shm_perm.mode=0%o\n",sds.shm_perm.mode);//输出共享内存的访问权限信息
if(shmctl(nShmId,IPC_RMID,NULL) < 0)//删除共享内存
{
perror("shmctl IPC_RMID");
exit(6);
}
return 0;
}
说明： (~0222)取反后做与实际上就是去除其他用户的写权限，在C语言中，八进制常用用前缀表示

**七.信号量**
信号量是一种用于提供不同进程间或一个进程间的不同线程间线程同步手段的原语，systemV信号量在内核中维护

二值信号量 ： 其值只有0、1 两种选择，0表示资源被锁，1表示资源可用；
计数信号量：其值在0 和某个限定值之间，不限定资源数只在0 1 之间；
计数信号量集 ；多个信号量的集合组成信号量集
**1.信号量集结构semid_ds**
内核维护的信号量集结构信息如下
定义在头文件<sys/sem.h>
struct semid_ds {
struct ipc_perm sem_perm;       /* permissions .. see ipc.h */
__kernel_time_t sem_otime;      /* last semop time */
__kernel_time_t sem_ctime;      /* last change time */
struct sem  *sem_base;      /* ptr to first semaphore in array */
struct sem_queue *sem_pending;      /* pending operations to be processed */
struct sem_queue **sem_pending_last;    /* last pending operation */
struct sem_undo *undo;          /* undo requests on this array */
unsigned short  sem_nsems;      /* no. of semaphores in array */
};
其中ipc_perm 结构是内核给每个进程间通信对象维护的一个信息结构，其成员包含所有者用户id，所有者组id、创建者及其组id，以及访问模式等；

semid_ds结构体中的sem结构是内核用于维护某个给定信号量的一组值的内部结构，其结构定义：

struct sem {
int semval;     /* current value */
int sempid;     /* pid of last operation */
struct list_head sem_pending; /* pending single-sop operations */
};
其中senval变量代表当前信号量的值，sempid 为最后一个成功操作该信号量的进程id，该结构体在内核以双向链表进行  维护
semid_ds结构体中的sem_nsems成员代表该信号量标示符的信号量个数

**求信号量极值**
\# sysctl -a|grep sem
![](https://img-my.csdn.net/uploads/201205/18/1337294699_7942.gif)
说明：输出格式是  SEMMSL   SEMMNS  SEMOPM  SEMMNI
每个信号量集最大信号量数目   整个系统可以创建的信号量最大数目  每次semop对信号量操作的最大值  系统中可以创建的信号量集中的最大数目

**2.semget创建信号量**
int semget(key_t key, int nsems, int semflg);
该函数执行成功返回信号量标示符，失败返回-1
参数key是通过调用ftok函数得到的键值，nsems代表创建信号量的个数，如果只是访问而不创建则可以指定该参数为0，我们一旦创建了该信号量，就不能更改其信号量个数，只要你不删除该信号量，你就是重新调用该函数创建该键值的信号量，该函数只是返回以前创建的值，不会重新创建；
semflg 指定该信号量的读写权限，当创建信号量时不许加IPC_CREAT ，若指定IPC_CREAT |IPC_EXCL则创建是存在该信号量，创建失败；
\#include <stdio.h>
\#include <stdlib.h>
\#include <sys/types.h>
\#include <sys/sem.h>
main()
{
int semid;
key_t semkey;
if((semkey = ftok("/etc/profile",1)) < 0)
{
perror("ftok");
exit(1);
}
if((semid = semget(semkey,1,IPC_CREAT|0666)) < 0)//创建包含信号量的信号集，权限666，信号量集中的信号量数目是1个
{
perror("semget");
exit(1);
}
printf("semid=%d\n",semid);
}
输出：
![](https://img-my.csdn.net/uploads/201205/18/1337294544_7045.gif)
**3.semop改变信号量值**
信号量操作是PV 操作，“互斥”与“同步”
int semop(int semid, struct sembuf *sops, unsigned nsops);
该函数执行成功返回0，失败返回-1；
第一个参数semid 为信号量标示符；nops为第二个参数的操作数组的个数，第二个参数sops为一个结构体数组指针，结构体定义在sys/sem.h中，结构体如下
struct sembuf {
unsigned short  sem_num;    /* semaphore index in array */
short       sem_op;     /*信号量操作数 */
short       sem_flg;    /*信号量操作标志 */
};

sem_num 操作信号的下标，其值可以为0 到nops

sem_flg为该信号操作的标志：其值可以为0、IPC_NOWAIT 、 SEM_UNDO
0  在对信号量的操作不能执行的情况下，该操作阻塞到可以执行为止；
IPC_NOWAIT 在对信号量的操作不能执行的情况下，该操作立即返回；
SEM_UNDO当操作的进程推出后，该进程对sem进行的操作将被取消；
sem_op取值 >0 则信号量加上它的值，等价于进程释放信号量控制的资源
sem_op取值 =0若没有设置IPC_NOWAIT， 那么调用进程 将进入睡眠状态，直到信号量的值为0，否则进程直接返回
sem_op取值 <0则信号量加上它的值，等价于进程申请信号量控制的资源，若进程设置IPC_NOWAIT则进程再没有可用资源情况下，进程 阻 塞，否则直接返回

**3.semctl控制信号量**
int semctl(int semid, int semnum, int cmd, ...);
该函数执行成功返回非负值，失败返回-1
参数semid为信号集的标识符；
参数 semnum标识一个特定信号，该参数仅用于 SETVAL、GETVAL、GETPID命令
cmd控制类型；
...说明函数参数是可选的，通过该共用体变量semun选择操作参数，各字段如下：
union semun {
int val;            /* SETVAL控制，用于设置信号量的值 */
struct semid_ds __user *buf;    /* 用于IPC_STAT & IPC_SET ，指向semid_ds结构指针，用于获取或者设置信号量控制结构 */
unsigned short __user *array;   /*用于GETALL & SETALL，指向短整形数组指针，用于获取或者设置信号量集的值 */
struct seminfo __user *__buf;   /* IPC_INFO控制命令，用于返回系统内核定义的信号量极值，为一结构指针，结构类型seminfo */
};
struct seminfo {
int semmap;
int semmni;
int semmns;
int semmnu;
int semmsl;
int semopm;
int semume;
int semusz;
int semvmx;
int semaem;
};

semctl的cmd参数
IPC_STAT读取一个信号量集的数据结构semid_ds，并将其存储在semun中的buf参数中。
IPC_SET设置信号量集的数据结构semid_ds中的元素ipc_perm，其值取自semun中的buf参数。
IPC_RMID将信号量集从系统中删除
GETALL用于读取信号量集中的所有信号量的值，存于semnu的array中
SETALL 设置所指定的信号量集的每个成员semval的值
GETPID返回最后一个执行semop操作的进程的PID。
LSETVAL把的val数据成员设置为当前资源数
GETVAL把semval中的当前值作为函数的返回，即现有的资源数，返回值为非负数
\#include <stdio.h>
\#include <stdlib.h>
\#include <sys/types.h>
\#include <sys/sem.h>
\#include <string.h>
typedef union semun//semctl需要的
{
intval;//保存信号量值
struct semid_ds *buf;//信号量控制结构指针
ushort *array;//无符号短整形变量
} SEMCTL_UNION;
main()
{
int n,semid;//信号量标示符变量
key_t semkey;//键值变量
SEMCTL_UNION semctl_arg;//联合类型变量
struct sembuf buf;//semop调用所需的结构变量
if((semkey = ftok("/etc/profile",1)) < 0)//创建键值
{
perror("ftok");
exit(1);
}
if((semid = semget(semkey,1,0)) < 0)//创建信号量
{
perror("semget");
exit(2);
}
semctl_arg.val = 2;//初始化
if (semctl(semid,0,SETVAL,semctl_arg) < 0)//设置信号量初始值
{
perror("semctl");
exit(3);
}
memset(&buf,0x00,sizeof(struct sembuf));//清空
buf.sem_num = 0;//信号量序号从0开始，第一个
buf.sem_op = -1;//P操作，所以-1
buf.sem_flg = IPC_NOWAIT;//非阻塞
for(n=0;;n++)//循环调用P操作，直到信号量变为0
{
if(semop(semid,&buf,1) == -1)//P操作
{
perror("semop");
break;
}
printf("semop[%d]:current semphore value=%d\n",n,semctl(semid,0,GETVAL,semctl_arg));
}
}
输出：
![](https://img-my.csdn.net/uploads/201205/18/1337295651_3114.gif)
==============================================================
扩展阅读参考：
[Linux 系统内核空间与用户空间通信的实现与分析](http://www.ibm.com/developerworks/cn/linux/l-netlink/index.html)
[Linux系统调用列表](http://www.ibm.com/developerworks/cn/linux/kernel/syscall/part1/appendix.html)
[系统调用跟我学(1)](http://www.ibm.com/developerworks/cn/linux/kernel/syscall/part1/)[系统调用跟我学(2)](http://www.ibm.com/developerworks/cn/linux/kernel/syscall/part2/)[系统调用跟我学(3)](http://www.ibm.com/developerworks/cn/linux/kernel/syscall/part3/)[系统调用跟我学(4)](http://www.ibm.com/developerworks/cn/linux/kernel/syscall/part4/)
[Linux环境进程间通信（二）: 信号（上）](http://www.ibm.com/developerworks/cn/linux/l-ipc/part2/index1.html)
[Linux环境进程间通信（二）: 信号（下）](https://www.ibm.com/developerworks/cn/linux/l-ipc/part2/index2.html)
[Linux环境进程间通信（四）信号灯](https://www.ibm.com/developerworks/cn/linux/l-ipc/part4/)
[Linux环境进程间通信（三）消息队列](https://www.ibm.com/developerworks/cn/linux/l-ipc/part3/)
[Linux环境进程间通信（一）管道及有名管道](http://www.ibm.com/developerworks/cn/linux/l-ipc/part1/)
[Linux环境进程间通信（五）: 共享内存（上）](http://www.ibm.com/developerworks/cn/linux/l-ipc/part5/index1.html)
[Linux环境进程间通信（五）: 共享内存（下）](http://www.ibm.com/developerworks/cn/linux/l-ipc/part5/index2.html)
[Linux 环境进程间通信（六）套接口](http://www.ibm.com/developerworks/cn/linux/l-ipc/part6/index.html?ca=drs-)
[Linux 实时信号程序中锁的探索](http://www.ibm.com/developerworks/cn/linux/l-cn-signallocks/index.html)
[UNIX 共享内存应用中的问题及解决方法](http://www.ibm.com/developerworks/cn/aix/library/au-cn-sharemem/index.html)
[对话 UNIX: 通过共享内存进行进程间通信](http://www.ibm.com/developerworks/cn/aix/library/au-spunix_sharedmemory/index.html?ca=drs-)
[在 Linux 中使用共享对象 让共享内存为您服务，而不是为您制造麻烦](http://www.ibm.com/developerworks/cn/linux/l-shared/index.html)
[Posix线程编程指南(1)](http://www.ibm.com/developerworks/cn/linux/thread/posix_threadapi/part1/)
[Posix线程编程指南(2)](http://www.ibm.com/developerworks/cn/linux/thread/posix_threadapi/part2/)
[Posix线程编程指南(3)](http://www.ibm.com/developerworks/cn/linux/thread/posix_threadapi/part3/)
[Posix线程编程指南(4)](http://www.ibm.com/developerworks/cn/linux/thread/posix_threadapi/part4/)
[Posix线程编程指南(5)](http://www.ibm.com/developerworks/cn/linux/thread/posix_threadapi/part5/)
[Linux 上实现双向进程间通信管道](http://www.ibm.com/developerworks/cn/linux/l-pipebid/index.html)
[POSIX 线程详解 一种支持内存共享的简捷工具](http://www.ibm.com/developerworks/cn/linux/thread/posix_thread1/)
[POSIX 线程详解，第 2部分 称作互斥对象的小玩意](http://www.ibm.com/developerworks/cn/linux/thread/posix_thread2/)
[POSIX 线程详解，第 3 部分 使用条件变量提高效率](http://www.ibm.com/developerworks/cn/linux/thread/posix_thread3/)
[尚观进程控制16](http://video.sina.com.cn/v/b/49349875-1855531553.html)


