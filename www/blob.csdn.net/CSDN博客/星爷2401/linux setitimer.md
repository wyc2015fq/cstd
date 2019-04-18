# linux setitimer - 星爷2401 - CSDN博客
2017年01月18日 14:08:20[星爷2401](https://me.csdn.net/leifukes)阅读数：1867
setitimer函数原型：
**[cpp]**[view
 plain](http://blog.csdn.net/lixianlin/article/details/25604779#)[copy](http://blog.csdn.net/lixianlin/article/details/25604779#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/340387/fork)
- int setitimer(int which, conststruct itimerval *new_value,  
- struct itimerval *old_value);  
其中which参数表示类型，可选的值有：
**ITIMER_REAL**：**以系统真实的时间来计算**，它送出SIGALRM信号。
**ITIMER_VIRTUAL**：以该进程在用户态下花费的时间来计算，也可以理解为进程占用CPU的时间，它送出SIGVTALRM信号。
**ITIMER_PROF**：以该进程在用户态下和内核态下所费的时间来计算，可以理解为占用CPU和系统调用的时间，它送出SIGPROF信号。
linux对信号的定义：
**SIGALRM**
时钟定时信号, 计算的是实际的时间或时钟时间. alarm函数使用该信号.
**SIGVTALRM**虚拟时钟信号. 类似于SIGALRM, 但是计算的是该进程占用的CPU时间.
**SIGPROF**类似于SIGALRM/SIGVTALRM, 但包括该进程用的CPU时间以及系统调用的时间
itimerval结构体定义：
**[cpp]**[view
 plain](http://blog.csdn.net/lixianlin/article/details/25604779#)[copy](http://blog.csdn.net/lixianlin/article/details/25604779#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/340387/fork)
- struct itimerval {  
- struct timeval it_interval; /* next value */
- struct timeval it_value;    /* current value */
- };  
- 
- struct timeval {  
- time_t      tv_sec;         /* seconds */
-     suseconds_t tv_usec;        /* microseconds */
- };  

