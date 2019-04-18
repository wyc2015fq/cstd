# linux操作系统下c语言编程入门--时间概念  - 深之JohnChen的专栏 - CSDN博客

2007年02月28日 18:02:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1785


linux操作系统下c语言编程入门--时间概念

 前言:[Linux](http://os.rdxx.com/Linux/)下的时间概念
这一章我们学习Linux的时间表示和计算函数
时间的表示
时间的测量
计时器的使用
1。时间表示 在程序当中,我们经常要输出系统当前的时间,比如我们使用date命令
的输出结果.这个时候我们可以使用下面两个函数
#include <time.h>

time_t time(time_t *tloc);
char *ctime(const time_t *clock);
time函数返回从1970年1月1日0点以来的秒数.存储在time_t结构之中.不过这个函数的返
回值对于我们来说没有什么实际意义.这个时候我们使用第二个函数将秒数转化为字符串
.. 这个函数的返回类型是固定的:一个可能值为. Thu Dec 7 14:58:59 2000 这个字符串
的长度是固定的为26
2。时间的测量 有时候我们要计算程序执行的时间.比如我们要对算法进行时间分析
..这个时候可以使用下面这个函数.
#include <sys/time.h>

int gettimeofday(struct timeval *tv,struct timezone *tz);
strut timeval {
long tv_sec; /* 秒数 */
long tv_usec; /* 微秒数 */
};
gettimeofday将时间保存在结构tv之中.tz一般我们使用NULL来代替.
#include <sys/time.h<
#include <stdio.h<
#include <math.h<
void function()
{
unsigned int i,j;
double y;
for(i=0;i<1000;i++)
for(j=0;j<1000;j++)
y=sin((double)i);
}
main()
{
struct timeval tpstart,tpend;
float timeuse;
gettimeofday(&tpstart,NULL);
function();
gettimeofday(&tpend,NULL);
timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+
tpend.tv_usec-tpstart.tv_usec;
timeuse/=1000000;
printf("Used Time:%f/n",timeuse);
exit(0);
}
这个程序输出函数的执行时间,我们可以使用这个来进行系统性能的测试,或者是函数算
法的效率分析.在我机器上的一个输出结果是: Used Time:0.556070
3。计时器的使用 Linux[操作系统](http://os.rdxx.com/)为每一个进程提供了3个内部间隔计时器.
ITIMER_REAL:减少实际时间.到时的时候发出SIGALRM信号.
ITIMER_VIRTUAL:减少有效时间(进程执行的时间).产生SIGVTALRM信号.
ITIMER_PROF:减少进程的有效时间和系统时间(为进程调度用的时间).这个经常和上面一
个使用用来计算系统内核时间和用户时间.产生SIGPROF信号.
具体的操作函数是:
#include <sys/time.h>
int getitimer(int which,struct itimerval *value);
int setitimer(int which,struct itimerval *newval,
struct itimerval *oldval);
struct itimerval {
struct timeval it_interval;
struct timeval it_value;
}
getitimer函数得到间隔计时器的时间值.保存在value中 setitimer函数设置间隔计时器
的时间值为newval.并将旧值保存在oldval中. which表示使用三个计时器中的哪一个. 
itimerval结构中的it_value是减少的时间,当这个值为0的时候就发出相应的信号了. 然
后设置为it_interval值.
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#define PROMPT "时间已经过去了两秒钟/n/a"
char *prompt=PROMPT;
unsigned int len;
void prompt_info(int signo)
{
write(STDERR_FILENO,prompt,len);
}
void init_sigaction(void)
{
struct sigaction act;
act.sa_handler=prompt_info;
act.sa_flags=0;
sigemptyset(&act.sa_mask);
sigaction(SIGPROF,&act,NULL);
}
void init_time()
{
struct itimerval value;
value.it_value.tv_sec=2;
value.it_value.tv_usec=0;
value.it_interval=value.it_value;
setitimer(ITIMER_PROF,&value,NULL);
}
int main()
{
len=strlen(prompt);
init_sigaction();
init_time();
while(1);
exit(0);
}
这个程序每执行两秒中之后会输出一个提示.  

