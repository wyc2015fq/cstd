# linux信号处理 - 星爷2401 - CSDN博客
2016年12月14日 11:21:18[星爷2401](https://me.csdn.net/leifukes)阅读数：4364
linux处理信号的方式有2种：一种是signal函数处理，另一种是信号集处理。
系统的信号一共有64个如下：
`执行 kill -l`
```
1) SIGHUP	 2) SIGINT	 3) SIGQUIT	 4) SIGILL	 5) SIGTRAP
 6) SIGABRT	 7) SIGBUS	 8) SIGFPE	 9) SIGKILL	10) SIGUSR1
11) SIGSEGV	12) SIGUSR2	13) SIGPIPE	14) SIGALRM	15) SIGTERM
16) SIGSTKFLT	17) SIGCHLD	18) SIGCONT	19) SIGSTOP	20) SIGTSTP
21) SIGTTIN	22) SIGTTOU	23) SIGURG	24) SIGXCPU	25) SIGXFSZ
26) SIGVTALRM	27) SIGPROF	28) SIGWINCH	29) SIGIO	30) SIGPWR
31) SIGSYS	34) SIGRTMIN	35) SIGRTMIN+1	36) SIGRTMIN+2	37) SIGRTMIN+3
38) SIGRTMIN+4	39) SIGRTMIN+5	40) SIGRTMIN+6	41) SIGRTMIN+7	42) SIGRTMIN+8
43) SIGRTMIN+9	44) SIGRTMIN+10	45) SIGRTMIN+11	46) SIGRTMIN+12	47) SIGRTMIN+13
48) SIGRTMIN+14	49) SIGRTMIN+15	50) SIGRTMAX-14	51) SIGRTMAX-13	52) SIGRTMAX-12
53) SIGRTMAX-11	54) SIGRTMAX-10	55) SIGRTMAX-9	56) SIGRTMAX-8	57) SIGRTMAX-7
58) SIGRTMAX-6	59) SIGRTMAX-5	60) SIGRTMAX-4	61) SIGRTMAX-3	62) SIGRTMAX-2
63) SIGRTMAX-1	64) SIGRTMAX
```
1、signal的用法。
按下Kill -10 pid的时候，会执行的内容
```
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
static void pro_func(int signo){
    printf("Receive the Signo:%d",signo);
}
int main(){
    if(signal(SIGUSR1,pro_func) == SIG_ERR){ //信号注册失败
        printf("SIGUSR! is ERROR \n");
    }
    while(1){
        pause();//让进程挂起，直到捕获到一个信号
    }
}
```
2、信号集的用法。
信号集是一个能表示多个信号的数据类型，sigset_t set ；set即一个信号集。
常用函数：
int sigemptyset(sigset_t *set); 将set集合置空
　　int sigfillset(sigset_t *set)； 将所有信号加入set集合
　　int sigaddset(sigset_t *set,int signo); 将signo信号加入到set集合
　　int sigdelset(sigset_t *set，int signo); 从set集合中移除signo信号
　　int sigismember(const sigset_t *set,int signo); signo判断信号是否存在于set集合中
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
static void quit(){
    printf("I am QUIT\n");
}
int main(){
    sigset_t newmask;
    sigset_t oldmask;
    sigset_t pendmask;
    signal(SIGQUIT,quit);
    //添加信号
    sigemptyset(&newmask);
    sigaddset(&newmask,SIGQUIT);
    //信号屏蔽
    sigprocmask(SIG_BLOCK,&newmask,&oldmask);
    printf("sleep start\n");
    sleep(10);
    printf("sleep end\n");   //10s内不会触发SIGQUIT信号
    printf("start get mask\n");
    sigpending(&pendmask); //获取被挂起的信号集，存到pendmask中
    if(sigismember(&pendmask,SIGQUIT)){
        printf("SIGQUIT pending\n");
    }else{
        printf("NO SIGQUIT \n");
    }
    sigprocmask(SIG_SETMASK, &oldmask, NULL);//恢复被屏蔽的信号SIGQUIT
    sleep(10); //10s内会触发SIGQUIT信号
    return (0);
}
```
gcc
 -c set.c
gcc -o set set.o
./set

