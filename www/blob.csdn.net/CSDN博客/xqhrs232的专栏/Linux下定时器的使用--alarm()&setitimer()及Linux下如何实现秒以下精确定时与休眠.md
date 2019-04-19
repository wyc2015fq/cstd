# Linux下定时器的使用--alarm()&setitimer()及Linux下如何实现秒以下精确定时与休眠 - xqhrs232的专栏 - CSDN博客
2011年04月26日 14:41:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2306
原文地址::[http://blogold.chinaunix.net/u/19671/showart_724449.html](http://blogold.chinaunix.net/u/19671/showart_724449.html)
## linux下定时器的使用 -- alarm() & setitimer()：
1、alarm
-------------------------------------------
    如果不要求很精确的话，用alarm()和signal()就够了
    unsigned int alarm(unsigned int seconds)
    函数说明: alarm()用来设置信号SIGALRM在经过参数seconds指定的秒数后传送给目前的进程。如果参数seconds为0，则之前设置的闹钟会被取消，并将剩下的时间返回。
    返回值: 返回之前闹钟的剩余秒数，如果之前未设闹钟则返回0。
    alarm()执行后，进程将继续执行，在后期(alarm以后)的执行过程中将会在seconds秒后收到信号SIGALRM并执行其处理函数。
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
void sigalrm_fn(int sig)
{
    printf("alarm!/n");
    alarm(2);
    return;
}
int main(void)
{
    signal(SIGALRM, sigalrm_fn);
**alarm(1)**;
    while(1) pause();
}
2、setitimer()
-------------------------------------------
    int setitimer(int which, const struct itimerval *value, struct itimerval *ovalue));
    setitimer()比alarm功能强大，支持3种类型的定时器：
**ITIMER_REAL** :     以系统真实的时间来计算，它送出SIGALRM信号。
**ITIMER_VIRTUAL** : 
-以该进程在用户态下花费的时间来计算，它送出SIGVTALRM信号。
**ITIMER_PROF** :     以该进程在用户态下和内核态下所费的时间来计算，它送出SIGPROF信号。
    setitimer()第一个参数which指定定时器类型（上面三种之一）；第二个参数是结构itimerval的一个实例；第三个参数可不做处理。
    setitimer()调用成功返回0，否则返回-1。
    下面是关于setitimer调用的一个简单示范，在该例子中，每隔一秒发出一个SIGALRM，每隔0.5秒发出一个SIGVTALRM信号：
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
int sec;
void sigroutine(int signo){
    switch (signo){
        case SIGALRM:
            printf("Catch a signal -- SIGALRM /n");
            signal(SIGALRM, sigroutine);
            break;
        case SIGVTALRM:
            printf("Catch a signal -- SIGVTALRM /n");
            signal(SIGVTALRM, sigroutine);
            break;
    }
    return;
}
int main()
{
    struct itimerval value, ovalue, value2;          //(1)
    sec = 5;
    printf("process id is %d/n", getpid());
    signal(SIGALRM, sigroutine);
    signal(SIGVTALRM, sigroutine);
    value.it_value.tv_sec = 1;
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec = 1;
    value.it_interval.tv_usec = 0;
**setitimer(ITIMER_REAL, &value, &ovalue);    **//(2)
    value2.it_value.tv_sec = 0;
    value2.it_value.tv_usec = 500000;
    value2.it_interval.tv_sec = 0;
    value2.it_interval.tv_usec = 500000;
**setitimer(ITIMER_VIRTUAL, &value2, &ovalue);**
    for(;;)
        ;
}
(1) **struct itimerval**
struct itimerval {
    struct[**timeval**](http://hi.baidu.com/zengzhaonong/blog/item/162213f4acd831ee7609d79c.html)
it_interval; /* timer interval */
    struct timeval it_value;    /* current value */
};
**i**timer**val**:      i   --> interval
                val --> value         
itimerval结构中的it_value是减少的时间,当这个值为0的时候就发出相应的信号了. 然后再将it_value设置为it_interval值.
(2) setitimer()
    setitimer()为其所在进程设置一个定时器，如果itimerval.**it_interval**不为0(it_interval的两个域都不为0)，则该定时器将持续有效(每隔一段时间就会发送一个信号)
    注意：Linux信号机制基本上是从Unix系统中继承过来的。早期Unix系统中的信号机制比较简单和原始，后来在实践中暴露出一些问题，因此，把那些建立在早期机制上的信号叫做"不可靠信号"，信号值小于SIGRTMIN(SIGRTMIN=32，SIGRTMAX=63)的信号都是不可靠信号。这就是"不可靠信号"的来源。它的主要问题是：进程每次处理信号后，就将对信号的响应设置为默认动作。在某些情况下，将导致对信号的错误处理；因此，用户如果不希望这样的操作，那么就要在信号处理函数结尾再一次调用signal()，重新安装该信号。
＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊
## Linux下如何实现秒以下精确定时与休眠
 Linux中提供的休眠函数是sleep和alarm，但是他们仅仅提供以秒为单位的休眠，这中休眠有些进程显然太长了，那么怎样才能使进程以更小的时间分辨率休眠呢？
 我知道的方法有2种，下面就做分别介绍。
 第一种方法是使用定时器，Linux提供的定时器函数是：
> 
int  setitimer(int which, const struct itimerval *value, struct
         itimerval *ovalue);
 which指定那种定时器。Linux提供3种定时器：
 TIMER_REAL: 准确定时器，超时会发出SIGALRM信号；
 TIMER_VIRTUAL: 虚拟定时器，只记进程时间，所以会根据进程执行时间而变化，不能实现准确定时，超时发出SIGVTALRM信号；
 TIMER_PROF: 梗概计时器，它会根据进程时间和系统时间而变化，不能实现准确定时，超时发出SIGPROF信号；
 在进程中应该捕捉所设定时器会发出的信号，因为进程收到定时器超时发出的信号后，默认动作是终止。
 value是设置定时器时间，相关结构如下：
> 
struct itimerval {
      struct timeval it_interval;
      struct timeval it_value; 
 };
 struct timeval {
      long tv_sec;        
      long tv_usec;
 };
  it_interval指定间隔时间，it_value指定初始定时时间。如果只指定it_value，就是实现一次定时；如果同时指定 it_interval，则超时后，系统会重新初始化it_value为it_interval，实现重复定时；两者都清零，则会清除定时器。
 tv_sec提供秒级精度，tv_usec提供微秒级精度，以值大的为先，注意1s = 1000000ms。
 ovalue用来保存先前的值，常设为NULL。
 如果是以setitimer提供的定时器来休眠，只需阻塞等待定时器信号就可以了。
 第二种方法是使用select来提供精确定时和休眠：
> 
int select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
       struct timeval *timeout);
 n指监视的文件描述符范围，通常设为所要select的fd＋1，readfds，writefds和exceptfds分别是读，写和异常文件描述符集，timeout为超时时间。
 可能用到的关于文件描述符集操作的宏有：
> 
FD_CLR(int fd, fd_set *set);    清除fd
 FD_ISSET(int fd, fd_set *set);  测试fd是否设置
 FD_SET(int fd, fd_set *set);     设置fd
 FD_ZERO(fd_set *set);             清空描述符集
 我们此时用不到这些宏，因为我们并不关心文件描述符的状态，我们关心的是select超时。所以我们需要把readfds，writefds和exceptfds都设为NULL，只指定timeout时间就行了。至于n我们可以不关心，所以你可以把它设为任何非负值。实现代码如下：
> 
int msSleep(long ms) {
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = ms;
    return select(0, NULL, NULL, NULL, &tv);
 }
 呵呵，怎么样，是不是很简单？ 
 结语：
  setitimer和select都能实现进程的精确休眠，本文分别对他们进行了简单介绍，并给出了一个简单的给予select的实现。我不推荐使用 setitimer，因为一者Linux系统提供的timer有限（每个进程至多能设3个不同类型的timer），再者ssetitimer实现起来没有 select简单。
