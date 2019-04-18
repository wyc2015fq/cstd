# UNIX时钟 - Orisun - 博客园







# [UNIX时钟](https://www.cnblogs.com/zhangchaoyang/articles/2296901.html)





sleep(n)和alarm(n)的精度是秒。

usleep(n)挂起进程n微秒或直到有一个不能被忽略的信号到达。

```
#include<stdio.h>
#include<curses.h>
void main(){
    initscr();  //初始化curses库和tty
    clear();    //清屏
    int i;
    for(i=0;i<LINES;i++){
        move(i,i+1); //把光标移到(10,20)的位置
        if(i%2==1)
            standout();//启动standout模式，一般使屏幕反色
        addstr("Hello World");  //在光标添加字符串
        if(i%2==1)
            standend(); //关闭standout模式
        sleep(1);    //sleep和alarm提供的时间精度为秒
        refresh();  //curses保存了两个屏幕：内部屏幕和工作屏幕，必须调用refresh才能用工作屏幕去替换真实屏幕
        move(i,i+1);
        addstr("");    //擦掉字符串
    }
    //getch();    //等待用户按下任意一个键
    endwin();   //关闭curses并重置tty
}
```

注意：使用curses库时编译时要指定-lcurses

对每个进程有3个计时器：
- ITIMER_VIRTUAL:只有进程在用户态运行时才计时。当这个计时器用尽时发送SIGVTALRM信号。
- ITIMER_PROF:进程运行在用户态或由该进程调用而陷入核心态才计时。当这个计时器用尽时发送SIGPROF信号。
- ITIMER_REAL:计时包含了运行于用户态、核心态、以及进程休眠的时间。当这个计时器用尽时发送SIGALRM信号。

除此之外，每个计时器还有两个参数，它们都是struct itimerval中的成员：
- it_value:初始时间
- it_interval:重复间隔时间

比如医生说4小时后开始吃药，以后每隔1小时吃一次。则这时的4小时是初始时间，1小时是间隔时间。

```
#include <sys/time.h>

       int getitimer(int which, struct itimerval *curr_value);
       int setitimer(int which, const struct itimerval *new_value,struct itimerval *old_value);

           struct itimerval {
               struct timeval it_interval; /* next value */
               struct timeval it_value;    /* current value */
           };

           struct timeval {
               long tv_sec;                /* seconds */
               long tv_usec;               /* microseconds */
           };
```

getitimer和setitimer用于获取和设置间隔计时器。which参数指的就是ITIMER_VIRTUAL、ITIMER_PROF或ITIMER_REAL。getitimer把获取到的时间存储到结构变量curr_value中。setitimer把时钟设置为new_value，如果old_value不为NULL，则旧的时钟存储在old_value中。

调用setitimer()后到达指定的时间就会自动触发SIGVTALRM、SIGPROF或SIGALRM信号。

```
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<signal.h>
void countdown(int);
int set_ticker(int);
int main(){
    signal(SIGALRM,countdown);
    if(set_ticker(500)==-1)
        perror("set_ticker");
    else
        while(1)
            pause();
    return 0;
}
void countdown(int signum){
    static int num=10;
    printf("%d..",num--);
    fflush(stdout);
    if(num<0){
        printf("DONE!\n");
        exit(0);
    }
}
int set_ticker(int n_msecs){
    struct itimerval new_timeset;
    long n_sec,n_usecs;
    n_sec=n_msecs/1000;
    n_usecs=(n_msecs%1000)*1000L;
    new_timeset.it_interval.tv_sec=n_sec;
    new_timeset.it_interval.tv_usec=n_usecs;
    new_timeset.it_value.tv_sec=n_sec;
    new_timeset.it_value.tv_usec=n_usecs;
    return setitimer(ITIMER_REAL,&new_timeset,NULL);
}
```












