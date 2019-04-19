# Linux下定时器使用 - xqhrs232的专栏 - CSDN博客
2011年04月26日 14:37:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：863
原文地址::[http://blog.csdn.net/gudulyn/archive/2006/07/06/885334.aspx](http://blog.csdn.net/gudulyn/archive/2006/07/06/885334.aspx)
Linux下的定时器有两种，以下分别介绍： 
       1、alarm 
       如果不要求很精确的话，用 alarm() 和 signal() 就够了 
           unsigned int alarm(unsigned int seconds) 
       专门为SIGALRM信号而设，在指定的时间seconds秒后，将向进程本身发送SIGALRM信号，又称为闹钟时间。进程调用alarm后，任何以前的alarm()调用都将无效。如果参数seconds为零，那么进程内将不再包含任何闹钟时间。如果调用alarm（）前，进程中已经设置了闹钟时间，则返回上一个闹钟时间的剩余时间，否则返回0。
       示例： 
       #include <stdio.h> 
       #include <unistd.h> 
       #include <signal.h> 
       void sigalrm_fn(int sig) 
       { 
               /* Do something */ 
               printf("alarm!/n"); 
               alarm(2); 
               return; 
       } 
       int main(void) 
       { 
               signal(SIGALRM, sigalrm_fn); 
               alarm(2); 
               /* Do someting */ 
               while(1) pause(); 
       } 
       2、setitimer 
       int setitimer(int which, const struct itimerval *value, struct itimerval *ovalue));
       setitimer()比alarm功能强大，支持3种类型的定时器： 
       ITIMER_REAL :  以系统真实的时间来计算，它送出SIGALRM信号。   
       ITIMER_VIRTUAL :  以该行程真正有执行的时间来计算，它送出SIGVTALRM信号。   
       ITIMER_PROF :  以行程真正有执行及在核心中所费的时间来计算，它送出SIGPROF信号。   
       Setitimer()第一个参数which指定定时器类型（上面三种之一）；第二个参数是结构itimerval的一个实例；第三个参数可不做处理。 
       Setitimer()调用成功返回0，否则返回-1。 
       下面是关于setitimer调用的一个简单示范，在该例子中，每隔一秒发出一个SIGALRM，每隔0.5秒发出一个SIGVTALRM信号：： 
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
           struct itimerval value, ovalue, value2; 
           sec = 5; 
           printf("process id is %d ", getpid()); 
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
           for(;;) 
               ; 
       } 
       　该例子的屏幕拷贝如下： 
       localhost:~$ ./timer_test 
       process id is 579 
       Catch a signal – SIGVTALRM 
       Catch a signal – SIGALRM 
       Catch a signal – SIGVTALRM 
       Catch a signal – SIGVTALRM 
       Catch a signal – SIGALRM 
       Catch a signal –GVTALRM 
       注意：Linux信号机制基本上是从Unix系统中继承过来的。早期Unix系统中的信号机制比较简单和原始，后来在实践中暴露出一些问题，因此，把那些建立在早期机制上的信号叫做"不可靠信号"，信号值小于SIGRTMIN(Red hat 7.2中，SIGRTMIN=32，SIGRTMAX=63)的信号都是不可靠信号。这就是"不可靠信号"的来源。它的主要问题是：进程每次处理信号后，就将对信号的响应设置为默认动作。在某些情况下，将导致对信号的错误处理；因此，用户如果不希望这样的操作，那么就要在信号处理函数结尾再一次调用signal()，重新安装该信号。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/gudulyn/archive/2006/07/06/885334.aspx](http://blog.csdn.net/gudulyn/archive/2006/07/06/885334.aspx)
