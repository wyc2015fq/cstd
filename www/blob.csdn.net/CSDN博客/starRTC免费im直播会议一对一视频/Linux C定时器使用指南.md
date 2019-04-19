# Linux C定时器使用指南 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年06月12日 09:53:57[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：73
个人分类：[c++](https://blog.csdn.net/elesos/article/category/7729692)
使用定时器的目的是为了周期性的执行某一任务，或者是到了某个指定时间去执行某一任务。要达到这一目的，一般有两个常见的方法。一个是用linux内部的三个定时器，另一个是用sleep, usleep函数让进程睡眠一段时间，其实，还有一个方法，那就是用gettimeofday, difftime等自己来计算时间间隔，然后时间到了就执行某一任务，但是这种方法效率低，所以不常用。
## 法一
首先来看看linux操作系统为每一个进程提供的3个内部计时器。
- ITIMER_REAL: 给一个指定的时间间隔，按照实际的时间来减少这个计数，当时间间隔为0的时候发出SIGALRM信号
- ITIMER_VIRTUAL: 给定一个时间间隔，当进程执行的时候才减少计数，时间间隔为0的时候发出SIGVTALRM信号
- ITIMER_PROF: 给定一个时间间隔，当进程执行或者是系统为进程调度的时候，减少计数，时间到了，发出SIGPROF信号，这个和ITIMER_VIRTUAL联合，常用来计算系统内核时间和用户时间。
用到的函数有：
#include <sys/time.h>int getitimer(int which,struct itimerval *value);int setitimer(int which,struct itimerval*newvalue,struct itimerval* oldvalue);
strcut timeval
{    long tv_sec;*/*秒*/*    long tv_usec;*/*微秒*/*};struct itimerval{    struct timeval it_interval;*/*时间间隔*/*    struct timeval it_value;*/*当前时间计数*/*};
it_interval用来指定每隔多长时间执行任务， it_value用来保存当前时间离执行任务还有多长时间。比如， 你指定it_interval为2秒(微秒为0)，开始的时候我们把it_value的时间也设定为2秒（微秒为0），当过了一秒， it_value就减少一个为1， 再过1秒，则it_value又减少1，变为0，这个时候发出信号（告诉用户时间到了，可以执行任务了），并且系统自动把it_value的时间重置为 it_interval的值，即2秒，再重新计数。 为了帮助你理解这个问题，我们来看一个例子：
#include <sys/time.h>#include <stdio.h>#include <unistd.h>#include <signal.h>#include <string.h>staticchar msg[]= “time is running out\n”;staticint len;*// 向标准错误输出信息，告诉用户时间到了*void prompt_info(int signo){write(STDERR_FILENO, msg, len);}*// 建立信号处理机制*void init_sigaction(void){struct sigaction tact;*/*信号到了要执行的任务处理函数为prompt_info*/*tact.sa_handler= prompt_info;tact.sa_flags=0;*/*初始化信号集*/*sigemptyset(&tact.sa_mask);*/*建立信号处理机制*/*sigaction(SIGALRM,&tact, NULL);}void init_time(){struct itimerval value;*/*设定初始时间计数为2秒0微秒*/*value.it_value.tv_sec=2;value.it_value.tv_usec=0;*/*设定执行任务的时间间隔也为2秒0微秒*/*value.it_interval= value.it_value;*/*设置计时器ITIMER_REAL*/*setitimer(ITIMER_REAL,&value, NULL);}int main(){len =strlen(msg);init_sigaction();init_time();while(1);exit(0);}
该程序的ITMER_REAL定时器，每隔2秒钟都会发送一个SIGALRM信号，当主函数接收到了这个信号之后，调用信号处理函数 prompt_info在标准错误上输出time is running out这个字符串。
ITIMER_VIRTUAL和ITIMER_PROF的使用方法类似，当你在setitimer里面设置的定时器为 ITIMER_VIRTUAL的时候，你把sigaction里面的SIGALRM改为SIGVTALARM, 同理，ITIMER_PROF对应SIGPROF。
不过，你可能会注意到，当你用ITIMER_VIRTUAL和ITIMER_PROF的时候，你拿一个秒表，你会发现程序输出字符串的时间间隔会不止2秒，甚至5-6秒才会输出一个，至于为什么，自己好好琢磨一下。
如果函数init_sigaction报错：
解决方法：在文件最开头加上如下代码（在所有#include之前）
#define _XOPEN_SOURCE
## 法二
下面我们来看看用sleep以及usleep怎么实现定时执行任务。
#include <signal.h>#include <unistd.h>#include <string.h>#include <stdio.h>staticchar msg[]= “I received a msg.\n”;int len;void show_msg(int signo){write(STDERR_FILENO, msg, len);}int main(){struct sigaction act;union sigval tsval;act.sa_handler= show_msg;act.sa_flags=0;sigemptyset(&act.sa_mask);sigaction(50,&act, NULL);len =strlen(msg);while(1){
sleep(2);*/*睡眠2秒*/**/*向主进程发送信号，实际上是自己给自己发信号*/*sigqueue(getpid(),50, tsval);}return0;}
看到了吧，这个要比上面的简单多了，而且你用秒表测一下，时间很准，指定2秒到了就给你输出一个字符串。所以，如果你只做一般的定时，到了时间去执行一个任务，这种方法是最简单的。
## 法三
下面我们来看看，通过自己计算时间差的方法来定时：
#include <signal.h>#include <unistd.h>#include <string.h>
#include <stdio.h>#include <time.h>staticchar msg[]= “I received a msg.\n”;int len;static time_t lasttime;void show_msg(int signo){write(STDERR_FILENO, msg, len);}int main(){struct sigaction act;union sigval tsval;act.sa_handler= show_msg;act.sa_flags=0;sigemptyset(&act.sa_mask);sigaction(50,&act, NULL);len =strlen(msg);time(&lasttime);while(1){time_t nowtime;*/*获取当前时间*/*time(&nowtime);*/*和上一次的时间做比较，如果大于等于2秒，则立刻发送信号*/*if(nowtime – lasttime >=2){*/*向主进程发送信号，实际上是自己给自己发信号*/*sigqueue(getpid(),50, tsval);lasttime = nowtime;}}return0;}
这个和上面不同之处在于，是自己手工计算时间差的，如果你想更精确的计算时间差，你可以把 time 函数换成gettimeofday，这个可以精确到微妙。(注：1秒=1000毫秒ms =106微秒µs) 上面介绍的几种定时方法各有千秋，在计时效率上、方法上和时间的精确度上也各有不同，采用哪种方法，就看你程序的需要。
## [[编辑](http://192.168.1.100/wiki_elesos_com/index.php?title=Linux_C%E5%AE%9A%E6%97%B6%E5%99%A8%E4%BD%BF%E7%94%A8%E6%8C%87%E5%8D%97&action=edit&section=4)]附：itimerval时钟的使用
#include<stdio.h>#include<signal.h>#include<sys/time.h>//itimerval结构体的定义int handle_count=0;void set_time(void){struct itimerval itv;   itv.it_interval.tv_sec=10;*//自动装载，之后每10秒响应一次*   itv.it_interval.tv_usec=0;   itv.it_value.tv_sec=5;*//第一次定时的时间*   itv.it_value.tv_usec=0;   setitimer(ITIMER_REAL,&itv,NULL);}void alarm_handle(int sig){
   handle_count++;printf("have handle count is %d**\n**",handle_count);}void main(void){struct itimerval itv;   signal(SIGALRM,alarm_handle);   set_time();while(1){   getitimer(ITIMER_REAL,&itv);printf("pass second is %d**\n**",(int)itv.it_value.tv_sec);   sleep(1);}return;}
使用定时器的目的是为了周期性的执行某一任务，或者是到了某个指定时间去执行某一任务。要达到这一目的，一般有两个常见的方法。一个是用linux内部的三个定时器，另一个是用sleep, usleep函数让进程睡眠一段时间，其实，还有一个方法，那就是用gettimeofday, difftime等自己来计算时间间隔，然后时间到了就执行某一任务，但是这种方法效率低，所以不常用。
## 法一
首先来看看linux操作系统为每一个进程提供的3个内部计时器。
- ITIMER_REAL: 给一个指定的时间间隔，按照实际的时间来减少这个计数，当时间间隔为0的时候发出SIGALRM信号
- ITIMER_VIRTUAL: 给定一个时间间隔，当进程执行的时候才减少计数，时间间隔为0的时候发出SIGVTALRM信号
- ITIMER_PROF: 给定一个时间间隔，当进程执行或者是系统为进程调度的时候，减少计数，时间到了，发出SIGPROF信号，这个和ITIMER_VIRTUAL联合，常用来计算系统内核时间和用户时间。
用到的函数有：
#include <sys/time.h>int getitimer(int which,struct itimerval *value);int setitimer(int which,struct itimerval*newvalue,struct itimerval* oldvalue);
strcut timeval
{    long tv_sec;/*秒*/    long tv_usec;/*微秒*/};struct itimerval{    struct timeval it_interval;/*时间间隔*/    struct timeval it_value;/*当前时间计数*/};
it_interval用来指定每隔多长时间执行任务， it_value用来保存当前时间离执行任务还有多长时间。比如， 你指定it_interval为2秒(微秒为0)，开始的时候我们把it_value的时间也设定为2秒（微秒为0），当过了一秒， it_value就减少一个为1， 再过1秒，则it_value又减少1，变为0，这个时候发出信号（告诉用户时间到了，可以执行任务了），并且系统自动把it_value的时间重置为 it_interval的值，即2秒，再重新计数。 为了帮助你理解这个问题，我们来看一个例子：
#include <sys/time.h>#include <stdio.h>#include <unistd.h>#include <signal.h>#include <string.h>staticchar msg[]= “time is running out\n”;staticint len;// 向标准错误输出信息，告诉用户时间到了void prompt_info(int signo){write(STDERR_FILENO, msg, len);}// 建立信号处理机制void init_sigaction(void){struct sigaction tact;/*信号到了要执行的任务处理函数为prompt_info*/tact.sa_handler= prompt_info;tact.sa_flags=0;/*初始化信号集*/sigemptyset(&tact.sa_mask);/*建立信号处理机制*/sigaction(SIGALRM,&tact, NULL);}void init_time(){struct itimerval value;/*设定初始时间计数为2秒0微秒*/value.it_value.tv_sec=2;value.it_value.tv_usec=0;/*设定执行任务的时间间隔也为2秒0微秒*/value.it_interval= value.it_value;/*设置计时器ITIMER_REAL*/setitimer(ITIMER_REAL,&value, NULL);}int main(){len =strlen(msg);init_sigaction();init_time();while(1);exit(0);}
该程序的ITMER_REAL定时器，每隔2秒钟都会发送一个SIGALRM信号，当主函数接收到了这个信号之后，调用信号处理函数 prompt_info在标准错误上输出time is running out这个字符串。
ITIMER_VIRTUAL和ITIMER_PROF的使用方法类似，当你在setitimer里面设置的定时器为 ITIMER_VIRTUAL的时候，你把sigaction里面的SIGALRM改为SIGVTALARM, 同理，ITIMER_PROF对应SIGPROF。
不过，你可能会注意到，当你用ITIMER_VIRTUAL和ITIMER_PROF的时候，你拿一个秒表，你会发现程序输出字符串的时间间隔会不止2秒，甚至5-6秒才会输出一个，至于为什么，自己好好琢磨一下。
如果函数init_sigaction报错：
解决方法：在文件最开头加上如下代码（在所有#include之前）
#define _XOPEN_SOURCE
## 法二
下面我们来看看用sleep以及usleep怎么实现定时执行任务。
#include <signal.h>#include <unistd.h>#include <string.h>#include <stdio.h>staticchar msg[]= “I received a msg.\n”;int len;void show_msg(int signo){write(STDERR_FILENO, msg, len);}int main(){struct sigaction act;union sigval tsval;act.sa_handler= show_msg;act.sa_flags=0;sigemptyset(&act.sa_mask);sigaction(50,&act, NULL);len =strlen(msg);while(1){
sleep(2);/*睡眠2秒*//*向主进程发送信号，实际上是自己给自己发信号*/sigqueue(getpid(),50, tsval);}return0;}
看到了吧，这个要比上面的简单多了，而且你用秒表测一下，时间很准，指定2秒到了就给你输出一个字符串。所以，如果你只做一般的定时，到了时间去执行一个任务，这种方法是最简单的。
## 法三
下面我们来看看，通过自己计算时间差的方法来定时：
#include <signal.h>#include <unistd.h>#include <string.h>
#include <stdio.h>#include <time.h>staticchar msg[]= “I received a msg.\n”;int len;static time_t lasttime;void show_msg(int signo){write(STDERR_FILENO, msg, len);}int main(){struct sigaction act;union sigval tsval;act.sa_handler= show_msg;act.sa_flags=0;sigemptyset(&act.sa_mask);sigaction(50,&act, NULL);len =strlen(msg);time(&lasttime);while(1){time_t nowtime;/*获取当前时间*/time(&nowtime);/*和上一次的时间做比较，如果大于等于2秒，则立刻发送信号*/if(nowtime – lasttime >=2){/*向主进程发送信号，实际上是自己给自己发信号*/sigqueue(getpid(),50, tsval);lasttime = nowtime;}}return0;}
这个和上面不同之处在于，是自己手工计算时间差的，如果你想更精确的计算时间差，你可以把 time 函数换成gettimeofday，这个可以精确到微妙。(注：1秒=1000毫秒ms =106微秒µs) 上面介绍的几种定时方法各有千秋，在计时效率上、方法上和时间的精确度上也各有不同，采用哪种方法，就看你程序的需要。
## [[编辑](http://192.168.1.100/wiki_elesos_com/index.php?title=Linux_C%E5%AE%9A%E6%97%B6%E5%99%A8%E4%BD%BF%E7%94%A8%E6%8C%87%E5%8D%97&action=edit&section=4)]附：itimerval时钟的使用
#include<stdio.h>#include<signal.h>#include<sys/time.h>//itimerval结构体的定义int handle_count=0;void set_time(void){struct itimerval itv;   itv.it_interval.tv_sec=10;//自动装载，之后每10秒响应一次   itv.it_interval.tv_usec=0;   itv.it_value.tv_sec=5;//第一次定时的时间   itv.it_value.tv_usec=0;   setitimer(ITIMER_REAL,&itv,NULL);}void alarm_handle(int sig){
   handle_count++;printf("have handle count is %d\n",handle_count);}void main(void){struct itimerval itv;   signal(SIGALRM,alarm_handle);   set_time();while(1){   getitimer(ITIMER_REAL,&itv);printf("pass second is %d\n",(int)itv.it_value.tv_sec);   sleep(1);}return;}
