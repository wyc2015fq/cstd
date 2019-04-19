# Linux 信号应用之黑匣子程序设计 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [倪彦春](http://www.jobbole.com/members/time-track) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
## 1. 何为黑匣子程序及其必要性
飞机上面的黑匣子用于飞机失事后对事故的时候调查，同理，程序的黑匣子用于程序崩溃后对崩溃原因进程定位。其实Linux提供的core dump机制就是一种黑匣子（core文件就是黑匣子文件）。但是core文件并非在所有场景都适用，因为core文件是程序崩溃时的内存映像，如果程序使用的内存空间比较大，那产生的core文件也将会非常大，在64bit的操作系统中，该现象更为显著。但是，其实我们定位程序崩溃的原因一般只需要程序挂掉之前的堆栈信息、内存信息等就足够了。所以有的时候没有必要使用系统自带的core文件机制。
阅读本文前，推荐先看一下我的另外一篇博客《[Linux 的 core 文件](http://blog.jobbole.com/101648/)》，里面讲解了core文件，并介绍了一些Linux信号的基本知识。
## 2. 黑匣子程序设计
程序异常时，往往会产生某种信号，内核会对该信号进行处理。所以设计黑匣子程序的实质就是我们定义自己的信号处理函数，来代替内核的默认处理。在我们的信号处理函数中，我们可以将我们想要的信息保存下来（比如程序崩溃时的堆栈信息），以方便后面问题的定位。
下面我们先给出一个我写的程序，然后边分析程序边讲具体如何设计一个黑匣子程序：


```
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <execinfo.h>
/* 定义一个数据结构用来保存信号 */
typedef struct sigInfo
{
    int     signum;
    char    signame[20];
} sigInfo;
/* 增加我们想要捕捉的异常信号，这里列举了6个 */
sigInfo sigCatch[] = {
    {1, "SIGHUP"}, {2, "SIGINT"}, {3, "SIGQUIT"}, 
    {6, "SIGABRT"}, {8, "SIGFPE"}, {11, "SIGSEGV"} 
};
/* 我们自定义的信号处理函数 */
void blackbox_handler(int sig)
{
    printf("Enter blackbox_handler: ");
    printf("SIG name is %s, SIG num is %d\n", strsignal(sig), sig);
    
    // 打印堆栈信息
    printf("Stack information:\n");
    int j, nptrs;
    #define SIZE 100
    void *buffer[100];
    char **strings;
    
    nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);
    
    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL)
    {
        perror("backtrace_symbol");
        exit(EXIT_FAILURE);
    }
    
    for(j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);
    
    free(strings);
    
    _exit(EXIT_SUCCESS);
}
/* 有bug的程序，调用该程序，将随机产生一些异常信号 */
void bug_func()
{
    int rand;
    struct timeval tpstart;
    
    pid_t  my_pid = getpid();
    
    // 产生随机数
    gettimeofday(&tpstart, NULL);
    srand(tpstart.tv_usec);
    while ((rand = random()) > (sizeof(sigCatch)/sizeof(sigInfo)));
    printf("rand=%d\n", rand);
    //随机产生异常信号
    switch(rand % (sizeof(sigCatch)/sizeof(sigInfo)))
    {
        case 0:
        {
            // SIGHUP
            kill(my_pid, SIGHUP);
            break;
        }
        case 1:
        {
            // SIGINT
            kill(my_pid, SIGINT);
            break;
        }
        case 2:
        {
            // SIGQUIT
            kill(my_pid, SIGQUIT);
            break;
        }
        case 3:
        {
            // SIGABRT
            abort();
            break;
        }
        case 4:
        {
            // SIGFPE
            int a = 6 / 0;
            break;
        }
        case 5:
        {
            // SIGSEGV
            kill(my_pid, SIGSEGV);
            break;
        }
        default:
            return;
    }
    
}
int main()
{
    int i, j;
    struct  sigaction   sa;
    
    // 初始化信号处理函数数据结构
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = blackbox_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    for (i = 0; i < sizeof(sigCatch)/sizeof(sigInfo); i++)
    {
        // 注册信号处理函数
        if(sigaction(sigCatch[i].signum, &sa, NULL) < 0)
        {
            return EXIT_FAILURE;
        }
    }
    bug_func();
    
    while(1);
    
    return EXIT_SUCCESS; 
}
```
### 2.1 定义一些数据结构
这里我们定义了一个sigInfo的数据结构，用来保存信号。利用这个数据结构我们可以将信号值与信号名映射起来。你可以在你的系统中使用 kill –l 命令去查看他们的对应关系。当然，在程序中，如果得到了信号值，也可以使用Linux提供的API函数strsignal来获取信号的名字，其函数原型如下：


```
#include <string.h>
char *strsignal(int sig);
```
之后定义了一个全局变量sigCatch来增加我们想要处理的信号。
### 2.2 sigaction函数
在main函数里面，除了调用一些函数外，主要是注册了一下我们要处理的信号。其实就是将特定的信号与某个信号处理函数关联起来。这里我们所要捕获的信号的信号处理函数都是同一个blackbox_handler，因为我们想在这些信号出现时保存堆栈信息，所以使用同一个函数完全可以。这里需要介绍的是sigaction函数，其函数原型如下：


```
#include <signal.h>
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
```
使用该函数可以改变程序默认的信号处理函数。
第一个参数signum指明我们想要改变其信号处理函数的信号值。注意，这里的信号不能是SIGKILL和SIGSTOP。这两个信号的处理函数不允许用户重写，因为它们给超级用户提供了终止程序的方法（ SIGKILL and SIGSTOP cannot be caught, blocked, or ignored）。
第二个和第三个参数是一个struct sigaction的结构体，该结构体在<signal.h>中定义，用来描述信号处理函数。如果act不为空，则其指向信号处理函数。如果oldact不为空，则之前的信号处理函数将保存在该指针中。如果act为空，则之前的信号处理函数不变。我们可以通过将act置空，oldact非空来获取当前的信号处理函数。
我们来看一下这个重要的结构体：


```
struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;
               int        sa_flags;
               void     (*sa_restorer)(void);   // 该成员现在已废弃
           };
```
可以看到，该结构体共有5个成员：
sa_handler是一个函数指针，指向我们定义的信号处理函数，该值也可以是**SIG_IGN**（忽略信号）或者**SIG_DEL**（使用默认的信号处理函数）。
sa_mask字段说明了一个信号集，信号处理函数执行期间这一信号集要加到进程的信号屏蔽字中。仅当从信号处理函数返回时再将进程的信号屏蔽字复位为原先的值。这样在调用信号处理函数时就能阻塞某些信号。在信号处理函数被调用时，操作系统建立的新信号屏蔽字包括正在被递送的信号。因此保证了在处理一个给定信号时，如果这种信号再次发生，那么它会被阻塞到对前一个信号的处理结束为止。
sa_flags字段指定对信号处理的一些选项，常用的选项及其含义说明如下（在 <signal.h>中定义）：
|选项|含义|
|----|----|
|SA_INTERRUPT|由此信号中断的系统调用不会自动重启|
|SA_NOCLDSTOP|若signo是SIGCHLD，当子进程停止（作业控制）时，不产生此信号。当子进程终止时，仍产生此信号（参加SA_NOCLDWAIT说明）。若已设置此标志，则当停止的进程继续运行时，作为XSI扩展，不发送SIGCHLD信号。|
|SA_NOCLDWAIT|若signo是SIGCHLD，则当调用进程的子进程终止时，不创建僵尸进程。若调用进程在后面调用wait，则调用进程阻塞，直到其所有子进程都终止，此时返回-1，并将errno设置为ECHILD。|
|SA_NODEFER|当捕捉到此信号时，在执行其信号处理函数时，系统不自动阻塞此信号（除非sa_mask包括了此信号）。|
|SA_ONSTACK|若用sigaltstack声明了以替换栈，则将此信号递送给替换栈上的进程。|
|SA_RESETHAND|在此信号处理函数的入口处，将此信号的处理方式复位为SIG_DEF，并清除SA_SIGINFO标志。但是，不能自动复位SIGILL和SIGTRAP这两个信号的配置。设置此标志是sigaction的行为如同SA_NODEFER标志也设置了一样。|
|SA_RESTART|由此信号中断的系统调用会自动重启动。|
|SA_SIGINFO|此选项对信号处理程序提供了附加信息：一个指向siginfo结构的指针以及一个指向进程上下文标识符的指针。|
sa_sigaction是一个替代的信号处理函数，当sa_flags字段设置为SA_SIGINFO时，使用该信号处理函数。需要注意的是，对于sa_sigaction和sa_handler字段，其实现可能使用同一存储区，所以应用程序只能一次使用这两个字段中的一个。通常，按如下方式调用信号处理函数：


```
void handler(int signo);
```
但是，如果设置了SA_SIGINFO标志，则按照如下方式调用信号处理函数：


```
void handler(int signo, siginfo_t *info, void *context);
```
可见第二种方式比第一种方式多了后面两个参数。其中第二个参数为一个siginfo_t结构的指针，该结构描述了信号产生的原因，该结构一般定义如下：


```
struct siginfo_t
{
    int     si_signo;       // signal number
    int     si_errno;       // if nonzero, errno value from <errno.h>
    int     si_code;        // additional info (depends on signal)
    pid_t   si_pid;         // sending process ID
    uid_t   si_uid;         // sending process real user ID
    void    *si_addr;       // address that cased the fault
    int     si_status;      // exit value or signal number
    long    si_band;        // band number for SIGPOLL
    
    /* possibly other fileds also */
}
```
一般siginfo_t结构至少包含si_signo和si_code成员。第三个参数context是一个无类型的指针，它可以被强制转换为ucntext_t结构类型，用于标识信号传递时进程的上下文。
### 2.3 信号集
信号种类数目可能超过一个整型量所包含的位数，所以一般而言，不能用整型量中的一位代表一种信号，也就是不能用一个整型量表示信号集（使用信号集可以表示多个信号）。POSIX.1定义了数据结构sigset_t以包含一个信号集，并且定义了下面5个处理信号集的函数：


```
#include <signal.h>
/* 前四个函数成功返回0，失败返回-1 */
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int signum);
int sigdelset(sigset_t *set, int signum);
/* 真返回1，假返回0，出错返回-1 */
int sigismember(const sigset_t *set, int signum);
```
每一个进程都有一个信号屏蔽字，它规定了当前要阻塞递送到该进程的信号集。对于每种可能的信号，该屏蔽字中都有一位与之对应。对于某种信号，若其对应为已设置，则它当前是被阻塞的。进程可以调用sigprocmask来检测和更改当前信号的屏蔽字。
函数sigemptyset初始化由set指向的信号集，清除其中所有的信号。函数sigfillset初始化由set指向的信号集，使其包括所有信号。所有应用程序在使用信号集前，要对该信号集调用sigemptyset或sigfillset一次。这是因为C编译器把未赋初值的外部和静态变量都初始化为0. 一旦已经初始化了一个信号集，以后就可以在该信号集中增、删特定的信号。函数sigaddset将一个信号添加到现有集中，sigdelset则从信号集中删除一个信号。
### 2.4 kill&&raise&&abort函数
bug_func函数的作用是产生一些异常信号，用于我们的测试。里面有两个注意点：（1）我们使用微秒数来作为随机数种子，这样产生的伪随机数分布会比其他很多方式更均匀一些。（2）我们调用了kill函数和abort函数来产生一些信号。其函数原型如下：


```
#include <signal.h>
int kill(pid_t pid, int sig);
int raise(int sig);
#include <stdlib.h>
void abort(void);
```
kill函数将信号发送给进程或进程组。kill的pid参数有4种不同的情况：
- pid>0. 将该信号发送给进程ID为pid的进程。
- pid==0. 将该信号发送给与发送进程属于同一进程组的所有进程（这些进程的进程组ID等于发送进程的进程组ID），而且发送进程具有向这些进程发送信号的权限。注意，这里的“所有进程”不包括实现定义的系统进程集。对于大多数UNIX系统，系统进程集包括内核进程以及init（pid等于1）进程。
- pid<0. 将该信号发送给其进程组ID等于pid的绝对值，而且发送进程具有向其发送信号的权限。如上所述，“所有进程集”不包括某些系统进程。
- pid==-1. 将该信号发送给发送进程有权限向它们发送信号的系统上所有的进程。不包括某些系统进程。
raise函数等价于kill(getpid(), signo).
abort函数会先清除对SIGABRT信号阻塞（如果有阻塞的话），然后调用raise函数向调用进程发送信号。注意：如果abort函数使得进程终止了，那终止前会刷新和关闭所有打开的流。
### 2.5 backtrace&&backtrace_symbols函数
在黑匣子信号处理函数中我们使用了backtrace和backtrace_symbols函数来获取进程崩溃时的堆栈信息。这两个函数的函数原型如下：


```
#include <execinfo.h>
int backtrace(void **buffer, int size);
char **backtrace_symbols(void *const *buffer, int size);
void backtrace_symbols_fd(void *const *buffer, int size, int fd);
```
backtrace函数会返回进程的调用栈信息，并保存在buffer指向的二维数组中；size指明buffer中可以保存的最大栈帧数目，如果调用栈信息超过了size的值，则只会保存近期的调用栈信息。返回值是保存的栈帧数。
使用backtrace函数得到调用栈信息后，我们就可以使用backtrace_symbols函数将调用栈的地址信息翻译为用符号描述的信息，保存在返回值里面。需要注意的是我们只需要定义返回值的指针，其空间由函数backtrace_symbols自己调用maolloc分配，但是使用完以后的空间由我们负责释放。backtrace_symbols_fd没有返回值，它与backtrace_symbols的不同之处在于它会将翻译的调用栈信息保存在文件里面。
注意：
- 使用backtrace函数时，在编译选项中需要加上 –rdynamic 选项，比如： gcc–rdynamic blackbox.c–oblackbox 。
- backtrace_symbols函数会输出出错时的16进制的地址，此时我们可以使用addr2line命令将其转换为我们具体的代码行数，命令格式为： addr2line–eexecute_file  addr ，比如 addr2line–e./a.out0x400d62 。
在该黑匣子程序中，涉及到了很多Linux信号的知识，以及一些相关的数据结构和API，希望对大家有用。但其实该黑匣子程序在有些极端情况下还是有一定的问题，后面我们会分析并进一步优化。
## 3. Bug分析
在前文中，我们实现了一个黑匣子程序——在进程崩溃后，可以保存进程的调用栈。但是，在文章结尾我们说程序有bug，那bug是什么呢？先看下面一个程序：


```
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <execinfo.h>
void blackbox_handler(int sig)
{
    printf("Enter blackbox_handler: ");
    printf("SIG name is %s, SIG num is %d\n", strsignal(sig), sig);
    
    // 打印堆栈信息
    printf("Stack information:\n");
    int j, nptrs;
#define SIZE 100
    void *buffer[100];
    char **strings;
    
    nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);
    
    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL)
    {
        perror("backtrace_symbol");
        exit(EXIT_FAILURE);
    }
    
    for(j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);
    
    free(strings);
    
    _exit(EXIT_SUCCESS);
}
long count = 0;
void bad_iter()
{
   int a, b, c, d;
   a = b = c = d = 1;
   a = b + 3;
   c = count + 4;
   d = count + 5 * c;
   count++;
   printf("count:%ld\n", count);
   
   bad_iter();
}
int main()
{
    struct  sigaction   sa;
    
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = blackbox_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGSEGV, &sa, NULL) < 0)
    {
        return EXIT_FAILURE;
    }
    
    bad_iter();
    
    while(1);
    
    return EXIT_SUCCESS;
}
```
该程序的执行结果如下：


```
... ...
count:261856
count:261857
count:261858
count:261859
count:261860
count:261861
Segmentation fault (core dumped)
allan@ubuntu:temp$
```
该程序是一种极端情况：我们的程序中使用了无线层次的递归函数，导致栈空间被用尽，此时会产生SIGSEGV信号。但是从输出看，并没有走到我们的信号处理函数里面。这是因为但由于栈空间已经被用完，所以我们的信号处理函数是没法被调用的，这种情况下，我们的黑匣子程序是没法捕捉到异常的。
但是该问题也很好解决，我们可以为我们的信号处理函数在堆里面分配一块内存作为“可替换信号栈”。
## 4. 使用可替换信号栈&&sigaltstack函数
使用可替换栈优化后的程序如下：


```
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <execinfo.h>
void blackbox_handler(int sig)
{
    printf("Enter blackbox_handler: ");
    printf("SIG name is %s, SIG num is %d\n", strsignal(sig), sig);
    
    // 打印堆栈信息
    printf("Stack information:\n");
    int j, nptrs;
#define SIZE 100
    void *buffer[100];
    char **strings;
    
    nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);
    
    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL)
    {
        perror("backtrace_symbol");
        exit(EXIT_FAILURE);
    }
    
    for(j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);
    
    free(strings);
    
    _exit(EXIT_SUCCESS);
}
long count = 0;
void bad_iter()
{
    int a, b, c, d; 
    a = b = c = d = 1;
    a = b + 3;
    c = count + 4;
    d = count + 5 * c;
    count++;
    printf("count:%ld\n", count);
   
    bad_iter();
}
int main()
{
    stack_t ss;
    struct  sigaction   sa;
    
    ss.ss_sp = malloc(SIGSTKSZ);
    ss.ss_size = SIGSTKSZ;
    ss.ss_flags = 0;
    if (sigaltstack(&ss, NULL) == -1)
    {
        return EXIT_FAILURE;
    }
    
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = blackbox_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_ONSTACK;
    
    if (sigaction(SIGSEGV, &sa, NULL) < 0)
    {
        return EXIT_FAILURE;
    }
    
    bad_iter();
    
    while(1);
    
    return EXIT_SUCCESS;
}
```
编译 gcc–rdynamic blackbox_overflow.c 后运行，输出为：


```
... ...
count:261989
count:261990
count:261991
count:261992
Enter blackbox_handler: SIG name is Segmentation fault, SIG num is 11
Stack information:
backtrace() returned 100 addresses
./a.out(blackbox_handler+0x63) [0x400c30]
/lib/x86_64-linux-gnu/libc.so.6(+0x36ff0) [0x7f6e68d74ff0]
/lib/x86_64-linux-gnu/libc.so.6(_IO_file_write+0xb) [0x7f6e68db7e0b]
/lib/x86_64-linux-gnu/libc.so.6(_IO_do_write+0x7c) [0x7f6e68db931c]
/lib/x86_64-linux-gnu/libc.so.6(_IO_file_xsputn+0xb1) [0x7f6e68db84e1]
/lib/x86_64-linux-gnu/libc.so.6(_IO_vfprintf+0x7fa) [0x7f6e68d8879a]
/lib/x86_64-linux-gnu/libc.so.6(_IO_printf+0x99) [0x7f6e68d92749]
./a.out(bad_iter+0x7a) [0x400d62]
./a.out(bad_iter+0x84) [0x400d6c]
./a.out(bad_iter+0x84) [0x400d6c]
./a.out(bad_iter+0x84) [0x400d6c]
./a.out(bad_iter+0x84) [0x400d6c]
./a.out(bad_iter+0x84) [0x400d6c]
... ...
```
可以看到，使用可替换栈以后，虽然同样栈溢出了，但是我们的黑匣子程序还是起作用了。所以这种优化是有效的。下面我们来看优化的代码。
可以看到我们的代码中使用了sigaltstack函数，该函数的作用就是在在堆中为函数分配一块区域，作为该函数的栈使用。所以，虽然递归函数将系统默认的栈空间用尽了，但是当调用我们的信号处理函数时，使用的栈是它实现在堆中分配的空间，而不是系统默认的栈，所以它仍旧可以正常工作。
该函数函数原型如下：


```
#include <signal.h>
int sigaltstack(const stack_t *ss, stack_t *oss);
```
该函数两个个参数为均为stack_t类型的结构体，先来看下这个结构体：


```
typedef struct {
   void  *ss_sp;     /* Base address of stack */
   int    ss_flags;  /* Flags */
   size_t ss_size;   /* Number of bytes in stack */
} stack_t;
```
要想创建一个新的可替换信号栈，ss_flags必须设置为0，ss_sp和ss_size分别指明可替换信号栈的起始地址和栈大小。系统定义了一个常数**SIGSTKSZ**，该常数对极大多数可替换信号栈来说都可以满足需求，**MINSIGSTKSZ**规定了可替换信号栈的最小值。
如果想要禁用已存在的一个可替换信号栈，可将ss_flags设置为**SS_DISABLE**。
而sigaltstack第一个参数为创建的新的可替换信号栈，第二个参数可以设置为NULL，如果不为NULL的话，将会将旧的可替换信号栈的信息保存在里面。函数成功返回0，失败返回-1.
> 
一般来说，使用可替换信号栈的步骤如下：
- 在内存中分配一块区域作为可替换信号栈
- 使用sigaltstack()函数通知系统可替换信号栈的存在和内存地址
- 使用sigaction()函数建立信号处理函数的时候，通过将sa_flags设置为SA_ONSTACK来告诉系统信号处理函数将在可替换信号栈上面运行。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/06/d0134f58cddae83b225e901371227914.png)![](http://jbcdn2.b0.upaiyun.com/2016/06/cc97d8a894fbbdd9169656e9faedc825.jpg)
