# linux 信号量之SIGNAL 0(转) - h13 - 博客园
转自：[http://www.dbafree.net/?p=870](http://www.dbafree.net/?p=870)
我们可以使用kill -l查看所有的信号量解释，但是没有看到SIGNAL 0的解释。
[root@testdb~]# kill -l
 1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL
 5) SIGTRAP      6) SIGABRT      7) SIGBUS       
![8)](http://www.dbafree.net/wp-includes/images/smilies/icon_cool.gif)
 SIGFPE
 9) SIGKILL     10) SIGUSR1     11) SIGSEGV     12) SIGUSR2
13) SIGPIPE     14) SIGALRM     15) SIGTERM     17) SIGCHLD
18) SIGCONT     19) SIGSTOP     20) SIGTSTP     21) SIGTTIN
22) SIGTTOU     23) SIGURG      24) SIGXCPU     25) SIGXFSZ
26) SIGVTALRM   27) SIGPROF     28) SIGWINCH    29) SIGIO
30) SIGPWR      31) SIGSYS      34) SIGRTMIN    35) SIGRTMIN+1
36) SIGRTMIN+2  37) SIGRTMIN+3  38) SIGRTMIN+4  39) SIGRTMIN+5
40) SIGRTMIN+6  41) SIGRTMIN+7  42) SIGRTMIN+8  43) SIGRTMIN+9
44) SIGRTMIN+10 45) SIGRTMIN+11 46) SIGRTMIN+12 47) SIGRTMIN+13
48) SIGRTMIN+14 49) SIGRTMIN+15 50) SIGRTMAX-14 51) SIGRTMAX-13
52) SIGRTMAX-12 53) SIGRTMAX-11 54) SIGRTMAX-10 55) SIGRTMAX-9
56) SIGRTMAX-8  57) SIGRTMAX-7  58) SIGRTMAX-6  59) SIGRTMAX-5
60) SIGRTMAX-4  61) SIGRTMAX-3  62) SIGRTMAX-2  63) SIGRTMAX-1
64) SIGRTMAX
网上搜了下，在这篇文档中找到了signal 0的解释，很不错:
[http://www.linuxjournal.com/content/monitoring-processes-kill-0](http://www.linuxjournal.com/content/monitoring-processes-kill-0)
“signal 0″ is kind of like a moral equivalent of “ping”.
Using “kill -0 NNN” in a shell script is a good way to tell if PID “NNN” is alive or not:
signal 0 is just used to check process is exists or not.
在关其它的信号量说明,原文:[http://hi.baidu.com/syqust/blog/item/cd8f6c633b8a617c0d33fa35.html](http://hi.baidu.com/syqust/blog/item/cd8f6c633b8a617c0d33fa35.html)
信号本质
信号是在软件层次上对中断机制的一种模拟，在原理上，一个进程收到一个信号与处理器收到一个中断请求可以说是一样的。信号是异步的，一个进程不必通过任何操作来等待信号的到达，事实上，进程也不知道信号到底什么时候到达。
信号是进程间通信机制中唯一的异步通信机制，可以看作是异步通知，通知接收信号的进程有哪些事情发生了。信号机制经过POSIX实时扩展后，功能更加强大，除了基本通知功能外，还可以传递附加信息。
使用kill -l就会显示出linux支持的信号列表。
其中列表中，编号为1 ~ 31的信号为传统UNIX支持的信号，是不可靠信号(非实时的)，编号为32 ~ 63的信号是后来扩充的，称做可靠信号(实时信号)。不可靠信号和可靠信号的区别在于前者不支持排队，可能会造成信号丢失，而后者不会。
下面我们对编号小于SIGRTMIN的信号进行讨论（下面的编号 依次对应信号 的数值为1 – 31）。
1) SIGHUP
本信号在用户终端连接(正常或非正常)结束时发出, 通常是在终端的控制进程结束时, 通知同一session内的各个作业, 这时它们与控制终端不再关联。
登录Linux时，系统会分配给登录用户一个终端(Session)。在这个终端运行的所有程序，包括前台进程组和后台进程组，一般都属于这个 Session。当用户退出Linux登录时，前台进程组和后台有对终端输出的进程将会收到SIGHUP信号。这个信号的默认操作为终止进程，因此前台进 程组和后台有终端输出的进程就会中止。不过可以捕获这个信号，比如wget能捕获SIGHUP信号，并忽略它，这样就算退出了Linux登录，wget也 能继续下载。
此外，对于与终端脱离关系的守护进程，这个信号用于通知它重新读取配置文件。
2) SIGINT
程序终止(interrupt)信号, 在用户键入INTR字符(通常是Ctrl-C)时发出，用于通知前台进程组终止进程。
3) SIGQUIT
和SIGINT类似, 但由QUIT字符(通常是Ctrl-\)来控制. 进程在因收到SIGQUIT退出时会产生core文件, 在这个意义上类似于一个程序错误信号。
4) SIGILL
执行了非法指令. 通常是因为可执行文件本身出现错误, 或者试图执行数据段. 堆栈溢出时也有可能产生这个信号。
5) SIGTRAP
由断点指令或其它trap指令产生. 由debugger使用。
6) SIGABRT
调用abort函数生成的信号。
7) SIGBUS
非法地址, 包括内存地址对齐(alignment)出错。比如访问一个四个字长的整数, 但其地址不是4的倍数。它与SIGSEGV的区别在于后者是由于对合法存储地址的非法访问触发的(如访问不属于自己存储空间或只读存储空间)。
![8)](http://www.dbafree.net/wp-includes/images/smilies/icon_cool.gif) SIGFPE
在发生致命的算术运算错误时发出. 不仅包括浮点运算错误, 还包括溢出及除数为0等其它所有的算术的错误。
9) SIGKILL
用来立即结束程序的运行. 本信号不能被阻塞、处理和忽略。如果管理员发现某个进程终止不了，可尝试发送这个信号。
10) SIGUSR1
留给用户使用
11) SIGSEGV
试图访问未分配给自己的内存, 或试图往没有写权限的内存地址写数据.
** 信号 11，即表示程序中可能存在特定条件下的非法内存访问。**
12) SIGUSR2
留给用户使用
13) SIGPIPE
管道破裂。这个信号通常在进程间通信产生，比如采用FIFO(管道)通信的两个进程，读管道没打开或者意外终止就往管道写，写进程会收到SIGPIPE信号。此外用Socket通信的两个进程，写进程在写Socket的时候，读进程已经终止。
14) SIGALRM
时钟定时信号, 计算的是实际的时间或时钟时间. alarm函数使用该信号.
15) SIGTERM
程序结束(terminate)信号, 与SIGKILL不同的是该信号可以被阻塞和处理。通常用来要求程序自己正常退出，shell命令kill缺省产生这个信号。如果进程终止不了，我们才会尝试SIGKILL。
17) SIGCHLD
**子进程结束时, 父进程会收到这个信号。**
如果父进程没有处理这个信号，也没有等待(wait)子进程，子进程虽然终止，但是还会在内核进程表中占有表项，这时的子进程称为僵尸进程。这种情 况我们应该避免(父进程或者忽略SIGCHILD信号，或者捕捉它，或者wait它派生的子进程，或者父进程先终止，这时子进程的终止自动由init进程 来接管)。
18) SIGCONT
让一个停止(stopped)的进程继续执行. 本信号不能被阻塞. 可以用一个handler来让程序在由stopped状态变为继续执行时完成特定的工作. 例如, 重新显示提示符
19) SIGSTOP
停止(stopped)进程的执行. 注意它和terminate以及interrupt的区别:该进程还未结束, 只是暂停执行. 本信号不能被阻塞, 处理或忽略.
20) SIGTSTP
停止进程的运行, 但该信号可以被处理和忽略. 用户键入SUSP字符时(通常是Ctrl-Z)发出这个信号
21) SIGTTIN
当后台作业要从用户终端读数据时, 该作业中的所有进程会收到SIGTTIN信号. 缺省时这些进程会停止执行.
22) SIGTTOU
类似于SIGTTIN, 但在写终端(或修改终端模式)时收到.
23) SIGURG
有”紧急”数据或out-of-band数据到达socket时产生.
24) SIGXCPU
超过CPU时间资源限制. 这个限制可以由getrlimit/setrlimit来读取/改变。
25) SIGXFSZ
当进程企图扩大文件以至于超过文件大小资源限制。
26) SIGVTALRM
虚拟时钟信号. 类似于SIGALRM, 但是计算的是该进程占用的CPU时间.
27) SIGPROF
类似于SIGALRM/SIGVTALRM, 但包括该进程用的CPU时间以及系统调用的时间.
28) SIGWINCH
窗口大小改变时发出.
29) SIGIO
文件描述符准备就绪, 可以开始进行输入/输出操作.
30) SIGPWR
Power failure
31) SIGSYS
非法的系统调用。
在以上列出的信号中，程序不可捕获、阻塞或忽略的信号有：SIGKILL,SIGSTOP
不能恢复至默认动作的信号有：SIGILL,SIGTRAP
默认会导致进程流产的信号有：SIGABRT,SIGBUS,SIGFPE,SIGILL,SIGIOT,SIGQUIT,SIGSEGV,SIGTRAP,SIGXCPU,SIGXFSZ
默认会导致进程退出的信号有：SIGALRM,SIGHUP,SIGINT,SIGKILL,SIGPIPE,SIGPOLL,SIGPROF,SIGSYS,SIGTERM,SIGUSR1,SIGUSR2,SIGVTALRM
默认会导致进程停止的信号有：SIGSTOP,SIGTSTP,SIGTTIN,SIGTTOU
默认进程忽略的信号有：SIGCHLD,SIGPWR,SIGURG,SIGWINCH
此外，SIGIO在SVR4是退出，在4.3BSD中是忽略；SIGCONT在进程挂起时是继续，否则是忽略，不能被阻塞
终止程序的时候在不得已的情况下不能用SIGKILL，因为SIGKILL不会对子进程进行处理，只是把对自己进行处理
Linux支持POSIX标准信号和实时信号。下面给出Linux Signal的简表，详细细节可以查看man 7 signal。
默认动作的含义如下：
Term    终止进程
|**信号**|**取值**|**默认动作**|**含义（发出信号的原因）**|
|----|----|----|----|
|SIGHUP|1|Term|终端的挂断或进程死亡|
|SIGINT|2|Term|来自键盘的中断信号|
|SIGQUIT|3|Core|来自键盘的离开信号|
|SIGILL|4|Core|非法指令|
|SIGABRT|6|Core|来自abort的异常信号|
|SIGFPE|8|Core|浮点例外|
|SIGKILL|9|Term|杀死|
|SIGSEGV|11|Core|段非法错误(内存引用无效)|
|SIGPIPE|13|Term|管道损坏：向一个没有读进程的管道写数据|
|SIGALRM|14|Term|来自alarm的计时器到时信号|
|SIGTERM|15|Term|终止|
|SIGUSR1|30,10,16|Term|用户自定义信号1|
|SIGUSR2|31,12,17|Term|用户自定义信号2|
|SIGCHLD|20,17,18|Ign|子进程停止或终止|
|SIGCONT|19,18,25|Cont|如果停止，继续执行|
|SIGSTOP|17,19,23|Stop|非来自终端的停止信号|
|SIGTSTP|18,20,24|Stop|来自终端的停止信号|
|SIGTTIN|21,21,26|Stop|后台进程读终端|
|SIGTTOU|22,22,27|Stop|后台进程写终端|
|SIGBUS|10,7,10|Core|总线错误（内存访问错误）|
|SIGPOLL|Term|Pollable事件发生(Sys V)，与SIGIO同义| |
|SIGPROF|27,27,29|Term|统计分布图用计时器到时|
|SIGSYS|12,-,12|Core|非法系统调用(SVr4)|
|SIGTRAP|5|Core|跟踪/断点自陷|
|SIGURG|16,23,21|Ign|socket紧急信号(4.2BSD)|
|SIGVTALRM|26,26,28|Term|虚拟计时器到时(4.2BSD)|
|SIGXCPU|24,24,30|Core|超过CPU时限(4.2BSD)|
|SIGXFSZ|25,25,31|Core|超过文件长度限制(4.2BSD)|
|SIGIOT|6|Core|IOT自陷，与SIGABRT同义|
|SIGEMT|7,-,7|Term| |
|SIGSTKFLT|-,16,-|Term|协处理器堆栈错误(不使用)|
|SIGIO|23,29,22|Term|描述符上可以进行I/O操作|
|SIGCLD|-,-,18|Ign|与SIGCHLD同义|
|SIGPWR|29,30,19|Term|电力故障(System V)|
|SIGINFO|29,-,-|与SIGPWR同义| |
|SIGLOST|-,-,-|Term|文件锁丢失|
|SIGWINCH|28,28,20|Ign|窗口大小改变(4.3BSD, Sun)|
|SIGUNUSED|-,31,-|Term|未使用信号(will be SIGSYS)|
说明：
一些信号的取值是硬件结构相关的（一般alpha和sparc架构用第一个值，i386、ppc和sh架构用中间值，mips架构用第三个值， - 表示相应架构的取值未知）。
蓝色的是POSIX.1-1990标准信号。
SIGKILL和SIGSTOP信号不能被挂钩、阻塞或忽略。
青色的是SUSv2和POSIX.1-2001定义的信号。
在Linux 2.2（包括）内核之前，SIGSYS、SIGXCPU、SIGXFSZ和SIGBUS （SPARC和MIPS架构除外）的默认动作是终止进程，但没有core dump。Linux 2.4遵循POSIX.1-2001要求，这些信号的默认动作改为：终止进程同时做core dump。
橙色的是其他常见的信号。
信号29在Alpha上为SIGINFO / SIGPWR ，在Sparc上为SIGLOST。
SIGEMT没有在POSIX.1-2001中说明，但是在大多数的Unices中仍然能见到，典型的默认动作是终止进程并做core dump。
SIGPWR没有在POSIX.1-2001中说明，在使用它的一些Unices中典型的默认动作是忽略。
SIGIO没有在POSIX.1-2001中说明，在使用它的一些Unices中典型的默认动作是忽略。
进程可以通过使用sigaction和signal系统调用来改变信号的默认处理方式（使用signal的可移植性差）。进程可以选择下列3种信号处理方式中的一种：
1、执行默认操作；
2、忽略该信号；
3、捕获该信号，但是通过信号句柄来调用自定义的处理函数。
信号可能被阻塞。进程中的每个线程拥有独立的信号掩码，用来表示本线程的信号被阻塞。线程通过pthread_sigmask来设置它的信号掩码。单线程程序可以用sigprocmask来操作信号掩码。在多线程程序中，所有线程处理一个指定信号的默认行为都是一样的。
补充:
SIG_DFL,SIG_IGN 分别表示无返回值的函数指针，指针值分别是0和1，这两个指针值逻辑上讲是实际程序中不可能出现的函数地址值。
SIG_DFL：默认信号处理程序
SIG_IGN：忽略信号的处理程序
