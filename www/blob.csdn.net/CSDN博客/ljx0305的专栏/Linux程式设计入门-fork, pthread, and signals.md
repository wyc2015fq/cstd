# Linux程式设计入门 - fork, pthread, and signals  - ljx0305的专栏 - CSDN博客
2008年07月30日 16:47:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：540标签：[linux																[signal																[kill																[null																[thread																[profiling](https://so.csdn.net/so/search/s.do?q=profiling&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=kill&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
在UNIX程式设计中，学会fork及signal的运用，算是相当基本的功夫。 
fork()及signal经常运用在daemon守护神这一类常驻程式，另外像 a4c.tty/yact/chdrv这些中文终端机程式也有用到，一般如 Mozilla/Apache/Squid等大程式几乎都一定会用到。 
虽然在UNIX下的程式写作，对thread的功能需求并非很大，但thread在现代的 作业系统中，几乎都已经存在了。pthread是Linux上的thread函数库，如果您 要在Linux下撰写多线程式，例如MP3播放程式，熟悉pthread的用法是必要的。 
pthread及signal都可以用一大章来讨论。在这里，我只谈及最简单及常用的技 巧，当您熟悉这些基本技巧的运用後，再找一些专门深入探讨pthread及signal 程式写作的书籍来研究。这些进阶的写法，用到的机会较少，将层次分明，学 习速度应该会比较快。 
程序分歧fork() 
fork()会产生一个与父程序相同的子程序，唯一不同之处在於其process id(pid)。 
如果我们要撰写守护神程式，或是例如网路伺服器，需要多个行程来同时提供 多个连线，可以利用fork()来产生多个相同的行程。 
函数宣告 
pid_t fork(void); 
pid_t vfork(void); 
返回值: 
-1 : 失败。 
0 : 子程序。 
>0 : 将子程序的process id传回给父程序。 
在Linux下fork()及vfork()是相同的东西。 
范例一: fork.c 
在这个范例中，我们示范fork()的标准用法。 
#include 
#include 
#include 
void main(void) 
{ 
pid_t pid; 
printf("hello/n"); 
pid = fork(); 
switch (pid) { 
case -1: printf("failure!/n"); break; 
case 0: printf("I am child!/n"); break; 
default: printf("my child is %d/n",pid); break; 
} 
for (;;) { /* do something here */ } 
} 
编译: 
gcc -o ex1 fork.c 
执行结果: 
./ex1 & 
hello 
my child is 8650 
I am child! 
我们可以见到，使用fork()，可将一个程式分岐成两个。在分歧之前的程式码 
只执行一次。 
检验行程: 
ps | grep ex1 
8649 p0 R 0:40 ./ex1 
8650 p0 R 0:40 ./ex1 
8649是父程序的pid，8650则为子程序的pid。 您会需要用到"killall ex1"来杀掉两个行程。 
范例二: daemon.c 
在UNIX中，我们一般都利用fork()，来实作所谓的"守护神程式"，也就是DOS中 所谓的"常驻程式"。一般的技巧是将父程序结束，而子程序便成为"守护神"。 
这个范例中，示范一般标准的daemon写法。 
#include 
#include 
#include 
void main(void) 
{ 
pid_t pid; 
pid = fork(); 
if (pid>0) { 
printf("daemon on duty!/n"); 
exit(0); 
} else 
if (pid<0) { 
printf("Can't fork!/n"); 
exit(-1); 
} 
for (;;) { 
printf("I am the daemon!/n"); 
sleep(3); 
/* do something your own here */ 
} 
} 
编译: 
gcc -o ex2 daemon.c 
执行结果: 
./ex2 
daemon on duty! 
I am the daemon! 
接下来每三秒钟，都会出现一个"I am the daemon!"的讯息，这表示您的程式 已经"长驻"在系统中了。 
检验行程: 
ps | grep ex2 
8753 p0 S 0:00 ./ex2 
注意到在范例一中，我们下的指令为"./ex1 &"，而在范例二中为"./ex2"，没有"&"符号。 
范例三: lock.c 
许多的时候，我们希望"守护神"在系统中只有一个，这时候会需要用到pid lock的技巧。如果您注意到/var/run目录中的内容，您会发现到有许多的*.pid 档，观看其内容都是一些数字，这些数字其实就是该行程的pid。 
#include 
#include 
#include 
void main(void) 
{ 
FILE *fp; 
pid_t pid; 
exit(-1); 
} 
act.sa_handler = quit; 
act.sa_flags = 0; 
sigemptyset(&act.sa_mask); 
sigaction(SIGTERM,&act,NULL); 
sigaction(SIGHUP,&act,NULL); 
sigaction(SIGINT,&act,NULL); 
sigaction(SIGQUIT,&act,NULL); 
sigaction(SIGUSR1,&act,NULL); 
sigaction(SIGUSR2,&act,NULL); 
for (;;) { 
sleep(3); 
} 
} 
编译: 
gcc -o ex1 lock.c 
执行 
./ex1 
daemon on duty! 
送信号 
我们先找出该守护神程式的pid 
PID=`cat /var/run/lock.pid` 
接下来利用kill来送信号 
kill  $PID 
Receive signal 15 
程式将会结束，并且/var/run/lock.pid将会被删除掉，以便下一次daemon再启动。注意到如果quit函数内，没有放exit()，程式将永远杀不掉。 
接下来送一些其它的信号试试看。 
./ex1 
PID=`cat /var/run/lock.pid` 
kill -HUP  $PID 
Receive signal 1 
您可以自行试试 
kill -INT  $PID 
kill -QUIT  $PID 
kill -ILL  $PID 
. 
. 
. 
等等这些信号，看看他们的结果如何。 
信号的定义 
在/usr/include/signum.h中有各种信号的定义 
#define SIGHUP 1 /* Hangup (POSIX). */ 
#define SIGINT 2 /* Interrupt (ANSI). */ 
#define SIGQUIT 3 /* Quit (POSIX). */ 
#define SIGILL 4 /* Illegal instruction (ANSI). */ 
#define SIGTRAP 5 /* Trace trap (POSIX). */ 
#define SIGABRT 6 /* Abort (ANSI). */ 
#define SIGIOT 6 /* IOT trap (4.2 BSD). */ 
#define SIGBUS 7 /* BUS error (4.2 BSD). */ 
#define SIGFPE 8 /* Floating-point exception (ANSI). 
*/ 
#define SIGKILL 9 /* Kill, unblockable (POSIX). */ 
#define SIGUSR1 10 /* User-defined signal 1 (POSIX). */ 
#define SIGSEGV 11 /* Segmentation violation (ANSI). */ 
#define SIGUSR2 12 /* User-defined signal 2 (POSIX). */ 
#define SIGPIPE 13 /* Broken pipe (POSIX). */ 
#define SIGALRM 14 /* Alarm clock (POSIX). */ 
#define SIGTERM 15 /* Termination (ANSI). */ 
#define SIGSTKFLT 16 /* ??? */ 
#define SIGCLD SIGCHLD /* Same as SIGCHLD (System V). */ 
#define SIGCHLD 17 /* Child status has changed (POSIX). 
*/ 
#define SIGCONT 18 /* Continue (POSIX). */ 
#define SIGSTOP 19 /* Stop, unblockable (POSIX). */ 
#define SIGTSTP 20 /* Keyboard stop (POSIX). */ 
#define SIGTTIN 21 /* Background read from tty (POSIX). 
*/ 
#define SIGTTOU 22 /* Background write to tty (POSIX). 
*/ 
#define SIGURG 23 /* Urgent condition on socket (4.2 
BSD). */ 
#define SIGXCPU 24 /* CPU limit exceeded (4.2 BSD). */ 
#define SIGXFSZ 25 /* File size limit exceeded (4.2 
BSD). */ 
#define SIGVTALRM 26 /* Virtual alarm clock (4.2 BSD). */ 
#define SIGPROF 27 /* Profiling alarm clock (4.2 BSD). 
*/ 
#define SIGWINCH 28 /* Window size change (4.3 BSD, Sun). 
*/ 
#define SIGPOLL SIGIO /* Pollable event occurred (System 
V). */ 
#define SIGIO 29 /* I/O now possible (4.2 BSD). */ 
#define SIGPWR 30 /* Power failure restart (System V). 
*/ 
#define SIGUNUSED 31 
函数宣告: 
Signal Operators 
int sigemptyset(sigset_t *set); 
int sigfillset(sigset_t *set); 
int sigaddset(sigset_t *set, int signum); 
int sigdelset(sigset_t *set, int signum); 
int sigismember(const sigset_t *set, int signum); 
Signal Handling Functions 
int sigaction(int signum, const struct sigaction *act,struct 
sigaction *oldact); 
int sigprocmask(int how, const sigset_t *set, sigset_t 
*oldset); 
int sigpending(sigset_t *set); 
int sigsuspend(const sigset_t *mask); 
Structure Signal Action 
struct sigaction { 
void (*sa_handler)(int); 
sigset_t sa_mask; 
int sa_flags; 
void (*sa_restorer)(void); 
} 
引用:http://www.chinalinuxpub.com/read.php?wid=102
