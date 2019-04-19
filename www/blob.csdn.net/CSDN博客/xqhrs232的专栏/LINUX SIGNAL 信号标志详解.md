# LINUX SIGNAL 信号标志详解 - xqhrs232的专栏 - CSDN博客
2011年07月28日 14:15:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1032
原文地址::[http://blog.csdn.net/nevercgoodbye/article/details/3367853](http://blog.csdn.net/nevercgoodbye/article/details/3367853)
在终端使用kill -l 命令可以显示所有的信号。
$kill -l
1) SIGHUP 2) SIGINT 3) SIGQUIT 4) SIGILL
5) SIGTRAP 6) SIGABRT 7) SIGBUS 8) SIGFPE
9) SIGKILL 10) SIGUSR1 11) SIGSEGV 12) SIGUSR2
13) SIGPIPE 14) SIGALRM 15) SIGTERM 16) SIGSTKFLT
17) SIGCHLD 18) SIGCONT 19) SIGSTOP 20) SIGTSTP
21) SIGTTIN 22) SIGTTOU 23) SIGURG 24) SIGXCPU
25) SIGXFSZ 26) SIGVTALRM 27) SIGPROF 28) SIGWINCH
29) SIGIO 30) SIGPWR 31) SIGSYS 34) SIGRTMIN
35) SIGRTMIN+1 36) SIGRTMIN+2 37) SIGRTMIN+3 38) SIGRTMIN+4
39) SIGRTMIN+5 40) SIGRTMIN+6 41) SIGRTMIN+7 42) SIGRTMIN+8
43) SIGRTMIN+9 44) SIGRTMIN+10 45) SIGRTMIN+11 46) SIGRTMIN+12
47) SIGRTMIN+13 48) SIGRTMIN+14 49) SIGRTMIN+15 50) SIGRTMAX-14
51) SIGRTMAX-13 52) SIGRTMAX-12 53) SIGRTMAX-11 54) SIGRTMAX-10
55) SIGRTMAX-9 56) SIGRTMAX-8 57) SIGRTMAX-7 58) SIGRTMAX-6
59) SIGRTMAX-5 60) SIGRTMAX-4 61) SIGRTMAX-3 62) SIGRTMAX-2
63) SIGRTMAX-1 64) SIGRTMAX
其中前面31个信号为不可靠信号(非实时的，可能会出现信号的丢失)，后面的信号为可靠信号(实时的real_time,对信号
排队，不会丢失)。
1) SIGHUP (挂起) 当运行进程的用户注销时通知该进程，使进程终止
2) SIGINT (中断) 当用户按下时,通知前台进程组终止进程
3) SIGQUIT (退出) 用户按下或时通知进程，使进程终止 
4) SIGILL (非法指令) 执行了非法指令，如可执行文件本身出现错误、试图执行数据段、堆栈溢出
5) SIGTRAP 由断点指令或其它trap指令产生. 由debugger使用
6) SIGABRT (异常中止) 调用abort函数生成的信号
7) SIGBUS 非法地址, 包括内存地址对齐(alignment)出错. eg: 访问一个四个字长的整数, 但其地址不是4的倍数.
8) SIGFPE (算术异常) 发生致命算术运算错误,包括浮点运算错误、溢出及除数为0.
9) SIGKILL (确认杀死) 当用户通过kill -9命令向进程发送信号时，可靠的终止进程
10) SIGUSR1 用户使用
11) SIGSEGV (段越界) 当进程尝试访问不属于自己的内存空间导致内存错误时，终止进程 
12) SIGUSR2 用户使用
13) SIGPIPE 写至无读进程的管道, 或者Socket通信SOCT_STREAM的读进程已经终止，而再写入。
14) SIGALRM (超时) alarm函数使用该信号，时钟定时器超时响应
15) SIGTERM (软中断) 使用不带参数的kill命令时终止进程
17) SIGCHLD (子进程结束) 当子进程终止时通知父进程
18) SIGCONT (暂停进程继续) 让一个停止(stopped)的进程继续执行. 本信号不能被阻塞.
19) SIGSTOP (停止) 作业控制信号,暂停停止(stopped)进程的执行. 本信号不能被阻塞, 处理或忽略.
20) SIGTSTP (暂停/停止) 交互式停止信号, Ctrl-Z 发出这个信号
21) SIGTTIN 当后台作业要从用户终端读数据时, 终端驱动程序产生SIGTTIN信号 
22) SIGTTOU 当后台作业要往用户终端写数据时, 终端驱动程序产生SIGTTOU信号 
23) SIGURG 有"紧急"数据或网络上带外数据到达socket时产生.
24) SIGXCPU 超过CPU时间资源限制. 这个限制可以由getrlimit/setrlimit来读取/改变。
25) SIGXFSZ 当进程企图扩大文件以至于超过文件大小资源限制。
26) SIGVTALRM 虚拟时钟信号. 类似于SIGALRM, 但是计算的是该进程占用的CPU时间.
27) SIGPROF (梗概时间超时) setitimer(2)函数设置的梗概统计间隔计时器(profiling interval timer)
28) SIGWINCH 窗口大小改变时发出.
29) SIGIO(异步I/O) 文件描述符准备就绪, 可以开始进行输入/输出操作.
30) SIGPWR 电源失效/重启动
31) SIGSYS 非法的系统调用。
在以上列出的信号中，
程序不可捕获、阻塞或忽略的信号有：SIGKILL,SIGSTOP
不能恢复至默认动作的信号有：SIGILL,SIGTRAP
默认会导致进程流产的信号有：SIGABRT,SIGBUS,SIGFPE,SIGILL,SIGIOT,SIGQUIT,SIGSEGV,SIGTRAP,SIGXCPU,SIGXFSZ
默认会导致进程退出的信号有：SIGALRM,SIGHUP,SIGINT,SIGKILL,SIGPIPE,SIGPOLL,SIGPROF,SIGSYS,SIGTERM,SIGUSR1,SIGUSR2,SIGVTALRM
默认会导致进程停止的信号有：SIGSTOP,SIGTSTP,SIGTTIN,SIGTTOU
默认进程忽略的信号有：SIGCHLD,SIGPWR,SIGURG,SIGWINCH
此外，SIGIO在SVR4是退出，在4.3BSD中是忽略；SIGCONT在进程挂起时是继续，否则是忽略，不能被阻塞。
在Unix/Linux中signal函数是比较复杂的一个,其定义原型如下:
void (*signal(int signo,void (*func)(int))) (int)
这个函数中,最外层的函数体
void (* XXX )(int)表明是一个指针,指向一个函数XXX的指针,XXX所代表的函数需要一个int型的参数,返回void
signal(int signo, void(*func)(int))是signal函数的主体.
需要两个参数int型的signo以及一个指向函数的函数.
void (*func)(int).
正是由于其复杂性,在[Plauger 1992]用typedef来对其进行简化
typedef void Sigfuc(int);//这里可以看成一个返回值 .
再对signal函数进行简化就是这样的了
Sigfunc *signal(int,Sigfuc *);
在signal.h头文件中还有以下几个定义
#define SIG_ERR (void (*)())-1
#define SIG_DFL (void (*)())0
#define SIG_IGN (void (*)())1
