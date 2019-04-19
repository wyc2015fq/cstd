# linux 信号详解 - fanyun的博客 - CSDN博客
2016年10月30日 20:51:46[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：505
信号机制是进程之间相互传递消息的一种方法，信号全称为软中断信号，也有人称作软中断。从它的命名可以看出，它的实质和使用很象中断。所以，信号可以说是进程控制的一部分。 
一、信号的基本概念 
1、基本概念 
软中断信号（signal，又简称为信号）用来通知进程发生了异步事件。进程之间可以互相通过系统调用kill发送软中断信号。内核也可以因为内部事件而给进程发送信号，通知进程发生了某个事件。注意，信号只是用来通知某进程发生了什么事件，并不给该进程传递任何数据。 
收到信号的进程对各种信号有不同的处理方法。处理方法可以分为三类：
第一种是类似中断的处理程序，对于需要处理的信号，进程可以指定处理函数，由该函数来处 理。
第二种方法是，忽略某个信号，对该信号不做任何处理，就象未发生过一样。
第三种方法是，对该信号的处理保留系统的默认值，这种缺省操作，对大部分的信 号的缺省操作是使得进程终止。
进程通过系统调用signal来指定进程对某个信号的处理行为。 
在进程表的表项中有一个软中断信号域，该域中每一位对应一个信号，当有信号发送给进程时，对应位置位。由此可以看出，进程对不同的信号可以同时保留，但对于同一个信号，进程并不知道在处理之前来过多少个。 
2、信号的类型 
发出信号的原因很多，这里按发出信号的原因简单分类，以了解各种信号： 
（1） 与进程终止相关的信号。当进程退出，或者子进程终止时，发出这类信号。 
（2） 与进程例外事件相关的信号。如进程越界，或企图写一个只读的内存区域（如程序正文区），或执行一个特权指令及其他各种硬件错误。 
（3） 与在系统调用期间遇到不可恢复条件相关的信号。如执行系统调用exec时，原有资源已经释放，而目前系统资源又已经耗尽。 
（4） 与执行系统调用时遇到非预测错误条件相关的信号。如执行一个并不存在的系统调用。 
（5） 在用户态下的进程发出的信号。如进程调用系统调用kill向其他进程发送信号。 
（6） 与终端交互相关的信号。如用户关闭一个终端，或按下break键等情况。 
（7） 跟踪进程执行的信号。 
Linux支持的信号列表如下。很多信号是与机器的体系结构相关的，首先列出的是POSIX.1中列出的信号： 
信号 值 处理动作 发出信号的原因 
--------------------------------------------------------------------- 
SIGHUP 1 A 终端挂起或者控制进程终止 
SIGINT 2 A 键盘中断（如break键被按下） 
SIGQUIT 3 C 键盘的退出键被按下 
SIGILL 4 C 非法指令 
SIGABRT 6 C 由abort(3)发出的退出指令 
SIGFPE 8 C 浮点异常 
SIGKILL 9 AEF Kill信号 
SIGSEGV 11 C 无效的内存引用 
SIGPIPE 13 A 管道破裂: 写一个没有读端口的管道 
SIGALRM 14 A 由alarm(2)发出的信号 
SIGTERM 15 A 终止信号 
SIGUSR1 30,10,16 A 用户自定义信号1 
SIGUSR2 31,12,17 A 用户自定义信号2 
SIGCHLD 20,17,18 B 子进程结束信号 
SIGCONT 19,18,25 进程继续（曾被停止的进程） 
SIGSTOP 17,19,23 DEF 终止进程 
SIGTSTP 18,20,24 D 控制终端（tty）上按下停止键 
SIGTTIN 21,21,26 D 后台进程企图从控制终端读 
SIGTTOU 22,22,27 D 后台进程企图从控制终端写 
下面的信号没在POSIX.1中列出，而在SUSv2列出 
信号 值 处理动作 发出信号的原因 
-------------------------------------------------------------------- 
SIGBUS 10,7,10 C 总线错误(错误的内存访问) 
SIGPOLL A Sys V定义的Pollable事件，与SIGIO同义 
SIGPROF 27,27,29 A Profiling定时器到 
SIGSYS 12,-,12 C 无效的系统调用 (SVID) 
SIGTRAP 5 C 跟踪/断点捕获 
SIGURG 16,23,21 B Socket出现紧急条件(4.2 BSD) 
SIGVTALRM 26,26,28 A 实际时间报警时钟信号(4.2 BSD) 
SIGXCPU 24,24,30 C 超出设定的CPU时间限制(4.2 BSD) 
SIGXFSZ 25,25,31 C 超出设定的文件大小限制(4.2 BSD) 
（对于SIGSYS，SIGXCPU，SIGXFSZ，以及某些机器体系结构下的SIGBUS，Linux缺省的动作是A (terminate)，SUSv2 是C (terminate and dump core)）。 
下面是其它的一些信号 
信号 值 处理动作 发出信号的原因 
--------------------------------------------------------------------- 
SIGIOT 6 C IO捕获指令，与SIGABRT同义 
SIGEMT 7,-,7 
SIGSTKFLT -,16,- A 协处理器堆栈错误 
SIGIO 23,29,22 A 某I/O操作现在可以进行了(4.2 BSD) 
SIGCLD -,-,18 A 与SIGCHLD同义 
SIGPWR 29,30,19 A 电源故障(System V) 
SIGINFO 29,-,- A 与SIGPWR同义 
SIGLOST -,-,- A 文件锁丢失 
SIGWINCH 28,28,20 B 窗口大小改变(4.3 BSD, Sun) 
SIGUNUSED -,31,- A 未使用的信号(will be SIGSYS) 
（在这里，- 表示信号没有实现；有三个值给出的含义为，第一个值通常在Alpha和Sparc上有效，中间的值对应i386和ppc以及sh，最后一个值对应mips。信号29在Alpha上为SIGINFO / SIGPWR ，在Sparc上为SIGLOST。） 
处理动作一项中的字母含义如下 
A 缺省的动作是终止进程 
B 缺省的动作是忽略此信号 
C 缺省的动作是终止进程并进行内核映像转储（dump core） 
D 缺省的动作是停止进程 
E 信号不能被捕获 
F 信号不能被忽略 
上面介绍的信号是常见系统所支持的。以表格的形式介绍了各种信号的名称、作用及其在默认情况下的处理动作。各种默认处理动作的含义是：
终止程序是指进程退 出；忽略该信号是将该信号丢弃，不做处理；
停止程序是指程序挂起，进入停止状况以后还能重新进行下去，一般是在调试的过程中（例如ptrace系统调 用）；
内核映像转储是指将进程数据在内存的映像和进程在内核结构中存储的部分内容以一定格式转储到文件系统，并且进程退出执行，这样做的好处是为程序员提供了方便，使得他们可以得到进程当时执行时的数据值，允许他们确定转储的原因，并且可以调试他们的程序。 
注意信号SIGKILL和SIGSTOP既不能被捕捉，也不能被忽略。信号SIGIOT与SIGABRT是一个信号。可以看出，同一个信号在不同的系统中值可能不一样，所以建议最好使用为信号定义的名字，而不要直接使用信号的值。 
二、信 号 机 制 
上 一节中介绍了信号的基本概念，在这一节中，我们将介绍内核如何实现信号机制。即内核如何向一个进程发送信号、进程如何接收一个信号、进程怎样控制自己对信 号的反应、内核在什么时机处理和怎样处理进程收到的信号。还要介绍一下setjmp和longjmp在信号中起到的作用。 
1、内核对信号的基本处理方法 
内 核给一个进程发送软中断信号的方法，是在进程所在的进程表项的信号域设置对应于该信号的位。这里要补充的是，如果信号发送给一个正在睡眠的进程，那么要看 该进程进入睡眠的优先级，如果进程睡眠在可被中断的优先级上，则唤醒进程；否则仅设置进程表中信号域相应的位，而不唤醒进程。这一点比较重要，因为进程检 查是否收到信号的时机是：一个进程在即将从内核态返回到用户态时；或者，在一个进程要进入或离开一个适当的低调度优先级睡眠状态时。 
内核处理一个进程收到的信号的时机是在一个进程从内核态返回用户态时。所以，当一个进程在内核态下运行时，软中断信号并不立即起作用，要等到将返回用户态时才处理。进程只有处理完信号才会返回用户态，进程在用户态下不会有未处理完的信号。 
内 核处理一个进程收到的软中断信号是在该进程的上下文中，因此，进程必须处于运行状态。前面介绍概念的时候讲过，处理信号有三种类型：进程接收到信号后退 出；进程忽略该信号；进程收到信号后执行用户设定用系统调用signal的函数。当进程接收到一个它忽略的信号时，进程丢弃该信号，就象没有收到该信号似 的继续运行。如果进程收到一个要捕捉的信号，那么进程从内核态返回用户态时执行用户定义的函数。而且执行用户定义的函数的方法很巧妙，内核是在用户栈上创 建一个新的层，该层中将返回地址的值设置成用户定义的处理函数的地址，这样进程从内核返回弹出栈顶时就返回到用户定义的函数处，从函数返回再弹出栈顶时，
 才返回原先进入内核的地方。这样做的原因是用户定义的处理函数不能且不允许在内核态下执行（如果用户定义的函数在内核态下运行的话，用户就可以获得任何权 限）。 
在信号的处理方法中有几点特别要引起注意：
第一，在一些系统中，当一个进程处理完中断信号返回用户态之前， 内核清除用户区中设 定的对该信号的处理例程的地址，即下一次进程对该信号的处理方法又改为默认值，除非在下一次信号到来之前再次使用signal系统调用。这可能会使得进程 在调用signal之前又得到该信号而导致退出。在BSD中，内核不再清除该地址。但不清除该地址可能使得进程因为过多过快的得到某个信号而导致堆栈溢 出。为了避免出现上述情况。在BSD系统中，内核模拟了对硬件中断的处理方法，即在处理某个中断时，阻止接收新的该类中断。 
第二个要 引起注意的是，如果要捕捉的信号发生于进程正在一个系统调用中时，并且该进程睡眠在可中断的优先级上，这时该信号引起进程作一次longjmp，跳出睡眠 状态，返回用户态并执行信号处理例程。当从信号处理例程返回时，进程就象从系统调用返回一样，但返回了一个错误代码，指出该次系统调用曾经被中断。这要注 意的是，BSD系统中内核可以自动地重新开始系统调用。 
第三个要注意的地方：若进程睡眠在可中断的优先级上，则当它收到一个要忽略的信号时，该进程被唤醒，但不做longjmp，一般是继续睡眠。但用户感觉不到进程曾经被唤醒，而是象没有发生过该信号一样。 
第 四个要注意的地方：内核对子进程终止（SIGCLD）信号的处理方法与其他信号有所区别。当进程检查出收到了一个子进程终止的信号时，缺省情况下，该进程 就象没有收到该信号似的，如果父进程执行了系统调用wait，进程将从系统调用wait中醒来并返回wait调用，执行一系列wait调用的后续操作（找 出僵死的子进程，释放子进程的进程表项），然后从wait中返回。SIGCLD信号的作用是唤醒一个睡眠在可被中断优先级上的进程。如果该进程捕捉了这个 信号，就象普通信号处理一样转到处理例程。如果进程忽略该信号，那么系统调用wait的动作就有所不同，因为SIGCLD的作用仅仅是唤醒一个睡眠在可被
 中断优先级上的进程，那么执行wait调用的父进程被唤醒继续执行wait调用的后续操作，然后等待其他的子进程。 
如果一个进程调用signal系统调用，并设置了SIGCLD的处理方法，并且该进程有子进程处于僵死状态，则内核将向该进程发一个SIGCLD信号。 
2、setjmp和longjmp的作用 
前面在介绍信号处理机制时，多次提到了setjmp和longjmp，但没有仔细说明它们的作用和实现方法。这里就此作一个简单的介绍。 
在 介绍信号的时候，我们看到多个地方要求进程在检查收到信号后，从原来的系统调用中直接返回，而不是等到该调用完成。这种进程突然改变其上下文的情况，就是 使用setjmp和longjmp的结果。setjmp将保存的上下文存入用户区，并继续在旧的上下文中执行。这就是说，进程执行一个系统调用，当因为资 源或其他原因要去睡眠时，内核为进程作了一次setjmp，如果在睡眠中被信号唤醒，进程不能再进入睡眠时，内核为进程调用longjmp，该操作是内核 为进程将原先setjmp调用保存在进程用户区的上下文恢复成现在的上下文，这样就使得进程可以恢复等待资源前的状态，而且内核为setjmp返回1，使
 得进程知道该次系统调用失败。这就是它们的作用。 
三、有关信号的系统调用 
前面两节已经介绍了有关信号的大部分知 识。这一节我们来了解一下这些系统调用。其中，系统调用signal是进程用来设定某个信号的处理方法，系统调用kill是用来发送信号给指定进程的。这 两个调用可以形成信号的基本操作。后两个调用pause和alarm是通过信号实现的进程暂停和定时器，调用alarm是通过信号通知进程定时器到时。所 以在这里，我们还要介绍这两个调用。 
1、signal 系统调用 
系统调用signal用来设定某个信号的处理方法。该调用声明的格式如下： 
void (*signal(int signum, void (*handler)(int)))(int); 
在使用该调用的进程中加入以下头文件： 
#include <signal.h> 
上述声明格式比较复杂，如果不清楚如何使用，也可以通过下面这种类型定义的格式来使用（POSIX的定义）： 
typedef void (*sighandler_t)(int); 
sighandler_t signal(int signum, sighandler_t handler); 
但这种格式在不同的系统中有不同的类型定义，所以要使用这种格式，最好还是参考一下联机手册。 
在调用中，参数signum指出要设置处理方法的信号。第二个参数handler是一个处理函数，或者是 
SIG_IGN：忽略参数signum所指的信号。 
SIG_DFL：恢复参数signum所指信号的处理方法为默认值。 
传递给信号处理例程的整数参数是信号值，这样可以使得一个信号处理例程处理多个信号。系统调用signal返回值是指定信号signum前一次的处理例程或者错误时返回错误代码SIG_ERR。下面来看一个简单的例子： 
#include <signal.h> 
#include <unistd.h> 
#include <stdio.h> 
void sigroutine(int dunno) { /* 信号处理例程，其中dunno将会得到信号的值 */ 
switch (dunno) { 
case 1: 
printf("Get a signal -- SIGHUP "); 
break; 
case 2: 
printf("Get a signal -- SIGINT "); 
break; 
case 3: 
printf("Get a signal -- SIGQUIT "); 
break; 
} 
return; 
} 
int main() { 
printf("process id is %d ",getpid()); 
signal(SIGHUP, sigroutine); //* 下面设置三个信号的处理方法 
signal(SIGINT, sigroutine); 
signal(SIGQUIT, sigroutine); 
for (;;) ; 
} 
其中信号SIGINT由按下Ctrl-C发出，信号SIGQUIT由按下Ctrl-发出。该程序执行的结果如下： 
localhost:~$ ./sig_test 
process id is 463 
Get a signal -SIGINT //按下Ctrl-C得到的结果 
Get a signal -SIGQUIT //按下Ctrl-得到的结果 
//按下Ctrl-z将进程置于后台 
[1]+ Stopped ./sig_test 
localhost:~$ bg 
[1]+ ./sig_test & 
localhost:~$ kill -HUP 463 //向进程发送SIGHUP信号 
localhost:~$ Get a signal – SIGHUP 
kill -9 463 //向进程发送SIGKILL信号，终止进程 
localhost:~$ 
2、kill 系统调用 
系统调用kill用来向进程发送一个信号。该调用声明的格式如下： 
int kill(pid_t pid, int sig); 
在使用该调用的进程中加入以下头文件： 
#include <sys/types.h> 
#include <signal.h> 
该 系统调用可以用来向任何进程或进程组发送任何信号。如果参数pid是正数，那么该调用将信号sig发送到进程号为pid的进程。如果pid等于0，那么信 号sig将发送给当前进程所属进程组里的所有进程。如果参数pid等于-1，信号sig将发送给除了进程1和自身以外的所有进程。如果参数pid小于- 1，信号sig将发送给属于进程组-pid的所有进程。如果参数sig为0，将不发送信号。该调用执行成功时，返回值为0；错误时，返回-1，并设置相应 的错误代码errno。下面是一些可能返回的错误代码： 
EINVAL：指定的信号sig无效。 
ESRCH：参数pid指定的进程或进程组不存在。注意，在进程表项中存在的进程，可能是一个还没有被wait收回，但已经终止执行的僵死进程。 
EPERM： 进程没有权力将这个信号发送到指定接收信号的进程。因为，一个进程被允许将信号发送到进程pid时，必须拥有root权力，或者是发出调用的进程的UID 或EUID与指定接收的进程的UID或保存用户ID（savedset-user-ID）相同。如果参数pid小于-1，即该信号发送给一个组，则该错误 表示组中有成员进程不能接收该信号。 
3、pause系统调用 
系统调用pause的作用是等待一个信号。该调用的声明格式如下： 
int pause(void); 
在使用该调用的进程中加入以下头文件： 
#include <unistd.h> 
该调用使得发出调用的进程进入睡眠，直到接收到一个信号为止。该调用总是返回-1，并设置错误代码为EINTR（接收到一个信号）。下面是一个简单的范例： 
#include <unistd.h> 
#include <stdio.h> 
#include <signal.h> 
void sigroutine(int unused) { 
printf("Catch a signal SIGINT "); 
} 
int main() { 
signal(SIGINT, sigroutine); 
pause(); 
printf("receive a signal "); 
} 
在这个例子中，程序开始执行，就象进入了死循环一样，这是因为进程正在等待信号，当我们按下Ctrl-C时，信号被捕捉，并且使得pause退出等待状态。 
4、alarm和 setitimer系统调用 
系统调用alarm的功能是设置一个定时器，当定时器计时到达时，将发出一个信号给进程。该调用的声明格式如下： 
unsigned int alarm(unsigned int seconds); 
在使用该调用的进程中加入以下头文件： 
#include <unistd.h> 
系 统调用alarm安排内核为调用进程在指定的seconds秒后发出一个SIGALRM的信号。如果指定的参数seconds为0，则不再发送 SIGALRM信号。后一次设定将取消前一次的设定。该调用返回值为上次定时调用到发送之间剩余的时间，或者因为没有前一次定时调用而返回0。 
注意，在使用时，alarm只设定为发送一次信号，如果要多次发送，就要多次使用alarm调用。 
对于alarm，这里不再举例。现在的系统中很多程序不再使用alarm调用，而是使用setitimer调用来设置定时器，用getitimer来得到定时器的状态，这两个调用的声明格式如下： 
int getitimer(int which, struct itimerval *value); 
int setitimer(int which, const struct itimerval *value, struct itimerval *ovalue); 
在使用这两个调用的进程中加入以下头文件： 
#include <sys/time.h> 
该系统调用给进程提供了三个定时器，它们各自有其独有的计时域，当其中任何一个到达，就发送一个相应的信号给进程，并使得计时器重新开始。三个计时器由参数which指定，如下所示： 
TIMER_REAL：按实际时间计时，计时到达将给进程发送SIGALRM信号。 
ITIMER_VIRTUAL：仅当进程执行时才进行计时。计时到达将发送SIGVTALRM信号给进程。 
ITIMER_PROF：当进程执行时和系统为该进程执行动作时都计时。与ITIMER_VIR-TUAL是一对，该定时器经常用来统计进程在用户态和内核态花费的时间。计时到达将发送SIGPROF信号给进程。 
定时器中的参数value用来指明定时器的时间，其结构如下： 
struct itimerval { 
struct timeval it_interval; /* 下一次的取值 */ 
struct timeval it_value; /* 本次的设定值 */ 
}; 
该结构中timeval结构定义如下： 
struct timeval { 
long tv_sec; /* 秒 */ 
long tv_usec; /* 微秒，1秒 = 1000000 微秒*/ 
}; 
在setitimer 调用中，参数ovalue如果不为空，则其中保留的是上次调用设定的值。定时器将it_value递减到0时，产生一个信号，并将it_value的值设 定为it_interval的值，然后重新开始计时，如此往复。当it_value设定为0时，计时器停止，或者当它计时到期，而it_interval 为0时停止。调用成功时，返回0；错误时，返回-1，并设置相应的错误代码errno： 
EFAULT：参数value或ovalue是无效的指针。 
EINVAL：参数which不是ITIMER_REAL、ITIMER_VIRT或ITIMER_PROF中的一个。 
下面是关于setitimer调用的一个简单示范，在该例子中，每隔一秒发出一个SIGALRM，每隔0.5秒发出一个SIGVTALRM信号： 
#include <signal.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/time.h> 
int sec; 
void sigroutine(int signo) { 
switch (signo) { 
case SIGALRM: 
printf("Catch a signal -- SIGALRM "); 
break; 
case SIGVTALRM: 
printf("Catch a signal -- SIGVTALRM "); 
break; 
} 
return; 
} 
int main() { 
struct itimerval value,ovalue,value2; 
sec = 5; 
printf("process id is %d ",getpid()); 
signal(SIGALRM, sigroutine); 
signal(SIGVTALRM, sigroutine); 
value.it_value.tv_sec = 1; 
value.it_value.tv_usec = 0; 
value.it_interval.tv_sec = 1; 
value.it_interval.tv_usec = 0; 
setitimer(ITIMER_REAL, &value, &ovalue); 
value2.it_value.tv_sec = 0; 
value2.it_value.tv_usec = 500000; 
value2.it_interval.tv_sec = 0; 
value2.it_interval.tv_usec = 500000; 
setitimer(ITIMER_VIRTUAL, &value2, &ovalue); 
for (;;) ; 
} 
该例子的屏幕拷贝如下： 
localhost:~$ ./timer_test 
process id is 579 
Catch a signal – SIGVTALRM 
Catch a signal – SIGALRM 
Catch a signal – SIGVTALRM 
Catch a signal – SIGVTALRM 
Catch a signal – SIGALRM 
Catch a signal –GVTALRM 
以下将剖析 ANSI <signal.h>库并示范如何使用其接口。进而讨论 POSIX 信号处理 API。
　　信号处理类似硬件中断。它们促使某个进程从当前的执行控制流程中跳出，以实现特定的行为，待特定处理完成后，再恢复到中断点继续执行。本文将剖析 ANSI <signal.h>库并示范如何使用其接口。然后，本文将进而讨论 [POSIX 信号处理 API](http://www.burningvoid.com/iaq/posix-signals.html)。 默认情况下，某些信号导致进程终止。例如，试图存取进程不拥有的内存将触发
 SIGSEGV （“段故障”）信号，这时该信号会终止进程的执行。许多应用程序都有这个问题，这是我们不希望看到的。调试，仿真和事务处理系统必须处理这样的信号以便让 进程继续执行。那么我们如何防止这种发生呢？
　　答案是安装一个处理器处理进来的信号并在发生时捕获它们
第一步：建立信号处理器
　　信号是内核传给某个进程的一个整数。当进程接收到信号，它便以以下方式之一响应：
- 忽略该信号；
- 让内核完成与该信号关联的[默认操作](http://trident.mcs.kent.edu/~farrell/sys95/notes/examples/prog/signal/)；
- 捕获该信号，即让内核将控制传给信号处理例程，等信号处理例程执行完毕，然后又从中断的地方恢复程序的执行。
　　所谓信号处理例程是一个函数，当某个信号发生时，内核会自动调用该函数。signal()函数为给定的信号注册一个处理例程：
typedef void (*handler)(void);
void * signal(int signum, handler);
　　第一个参数是信号编码。第二个参数用户定义的函数地址，当信号 signum 产生时，handler 所指向的函数被调用。
除了函数地址之外，第二个参数也可以是两个特殊的值：SIG_IGN 和 SIG_DFL。SIG_IGN 表示该信号应被忽略（注意：SIGKILL 和 SIGSTOP 在无论如何都是不能被阻塞、捕获或忽略的）；SIG_DFL 指示内核该信号产生时完成默认行为。
第二步：发信号
向某个进程发信号有三种方式：
- 进程通过条用 raise() 显式地发送信号给自己；
- 信号从另一个进程发送，比方说通过 [kill()](http://www.mcsr.olemiss.edu/cgi-bin/man-cgi?kill+2) 系统调用或者 [Perl
 脚本](http://www.ccsf.cc.ca.us/Pub/Perl/perlipc/Signals.html)；
- 信号从内核发送。例如，当进程试图存取不属于自己的内存，或在系统关闭期间存取内存时；
第三步：产生和处理信号
下面程序注册 SIGTERM 处理器。然后产生一个 SIGTERM 信号，从而导致该处理器运行：
#include <csignal>
#include <iostream>
using namespace std;
void term(int sig)
{
	//..necessary cleanup operations before terminating
	cout << "handling signal no." <<sig <<endl;
}
int main()
{
	signal(SIGTERM, term); // register a SIGTERM handler
	raise(SIGTERM); // will cause term() to run
}        
ANSI <signal.h> 的局限
　　当进入就绪状态的某个进程准备运行一个 SIGx 信号处理例程时又接收到另一个 SIGx 信号，这时会发生什么情况呢？一个方法是让内核中断该进程并再次运行该信号处理例程。为此，这个处理例程必须是可重入的（[re-entrant](http://www.vckbase.com/bbs/prime/viewprime.asp?id=632)）。但是，设计可重入的处理例程决非易事。ANSI
 C 解决[重现信号（recurring signals）](http://www.linux-mag.com/1999-12/compile_01.html)问题的方法是在执行用户定义的处理例程前，将处理例程重置为 STG_DFL。这样做是有问题的。
　　当两个信号快速产生时，内核运行第一个信号的处理例程，而对第二个信号则进行默认处理，这样有可能终止该进程。
　　在过去的三十年中出现了几个可以信号处理框架，每一种框架对重现信号的处理问题提供了不同的解决方法。[POSIX 信号 API](http://www.burningvoid.com/iaq/posix-signals.html) 是其中最为成熟的和可移植的一个。
POSIX 信号
　　POSIX 信号处理函数操作一组打包在 sigset_t 数据类型中信号：
- int sigemptyset(sigset_t * pset); 清除 pset 中的所有信号。
- int sigfillset(sigset_t * pset); 用可获得的信号填充 pset。
- int sigaddset(sigset_t * pset, int signum); 将 signum 添加到 pset。
- int sigdelset(sigset_t * pset, int signum); 从 pset 中删除 signum。
- int sigismember(const sigset_t * pset, int signum); 如果 signum 包含在 pset 中，则返回非零，否则返回 0。
Sigaction() 为特定的信号注册处理例程：
int sigaction(int signum, struct sigaction * act, struct sigaction *prev); 
sigaction 结构描述内核处理 signum 的信息：
struct sigaction
{
	sighanlder_t sa_hanlder; 
	sigset_t sa_mask;          // 阻塞信号的清单
	unsigned long sa_flags;    // 阻塞模式
	void (*sa_restorer)(void); // 未使用
};
　　sa_hanlder 保存函数的地址，该函数带一个整型参数，没有返回值。它还可以是两个特别值之一：SIG_DFL 和 SIG_IGN。
额外特性
　　POSIX API 提供多种 ANSI 库中所没有的服务。其中包括阻塞进入的信号并获取当前未决信号。 
阻塞信号
　　sigprocmask() 阻塞和取消阻塞信号：
int sigprocmask(int mode, const sigset_t* newmask,sigset_t * oldmask); 
mode 可取以下值之一：
SIG_BLOCK —— 将 newmask 中的信号添加到当前的信号挡板中。
SIG_UNBLOCK —— 从当前的信号挡板中删除 newmask 信号。
SIG_SETMASK —— 仅阻塞 newmask 中的信号。
获取未决信号
　　阻塞的信号处于等待状态，直到进程就绪接收它们。这样的信号被称为未决信号，可以通过调用 sigpending() 来获取。
int sigpending(sigset_t * pset); 
