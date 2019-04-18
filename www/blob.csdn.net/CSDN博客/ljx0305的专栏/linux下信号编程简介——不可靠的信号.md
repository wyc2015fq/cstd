# linux下信号编程简介——不可靠的信号 - ljx0305的专栏 - CSDN博客
2008年05月15日 22:13:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1427标签：[linux																[编程																[signal																[table																[input																[终端](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
linux下信号编程简介——不可靠的信号
( 作者：mikespook | 发布日期：2002-12-8 | 浏览次数：135 )
关键字：linux,信号,signal() 
前言： 
    本文章只是为了给广大和我一样的菜鸟一个指引。如果你是高手，或对编程毫无兴趣。建议请不要在此浪费时间。 
　　信号是什么？其实这是一个很有意思的东西。比如说汽车行驶在路上相互鸣笛这就是信号；同样遇到红灯必须停下来这也是
信号。对应到操作系统中，假设进程就是汽车，那么进程之间可以发送信号；同样如果假设操作系统是公路系统，那么操作系统
也可以向进程发送信号。 
    在linux中信号分为不可靠（或叫不安全）的信号与可靠的信号。后面我会解释为什么这么说。 
    首先，简单介绍一下linux系统中的通用信号： 
    SIGHUP   终端线以挂起。这是指当丢失载波信号时调制解调器线的挂起操作。然而，当它用logout函数关闭时，也将适用于
任一终端设备。 
    SIGINT   终端线已经收到中断字符。 
    SIGQUIT  终端线已经收到退出字符。 
    SIGUSR1  用户定义信号1。 
    SIGUSR2  用户定义信号2。 
    SIGTERM  正被终止的进程。 
    SIGCHLD  一个子进程已经终止。 
    SIGPIPE  半关闭管道发生写操作。 
    SIGALRM  Alarm函数记时器到时。 
    再来看看要用的函数signal()。signal()函数的定义在头文件 signal.h 中，它的函数原形是 sig_t signal(int sig, 
sig_t func); 第一个参数是想要注册的信号，第二个参数是指向信号处理函数的指针，或默认操作。系统提供了两种默认操作：
SIG_DEL 和 SIG_IGN，分别是默认信号操作和忽略信号。 
    好了说了一大堆枯燥无味的东西，有些晕了。单刀直入，看一个程序把。还记得上次那个多进程的程序么？我对其进行了一
些修改。使得当子进程运行结束后不必等父进程顺序运行到 wait() 函数，而是直接转入信号处理函数。唉，复杂说不清楚。自
己看吧！ 
/*--------------------------signal_fork.c------------------------------*/ 
/* mikespook */ 
/* exercise function signal() */ 
/* 2002.7.18 */ 
#include 
#include 
#include 
#include 
#define FAC_N 65535 
/* 子进程调用的函数，这里我为了模拟一个很大的后台操作用了一个循环。 */ 
void big_loop(int n); 
/* 父进程调用的函数，其实不放到函数里也可以，不过为了程序的结构更好看还是放到函数里的好 */ 
void input_information(); 
/* 信号处理函数，当父进程收到SIGCHLD信号的时候就会转入该函数进行操作 */ 
void handler(); 
int main() 
{ 
  /* 进程号 */ 
  pid_t pid; 
  /* 想用信号？那先对信号处理函数进行注册，对应起来 */ 
  signal(SIGCHLD, handler); 
  /* 程序在这里“分叉”，新的进程创建了 */ 
  pid = fork(); 
  /* 通过fork()的返回值来判断是父进程还是子进程 */ 
  switch(pid){ 
    /* 返回 -1，很不幸，创建进程失败了。可能是没有足够的内存空间，也可能已经开起了太多的进程。 */ 
    case -1: 
      perror("fork/n"); 
      break; 
    /* 返回 0，现在是在子进程里运行，那就调用子进程的操作函数。 */ 
    case 0: 
      /* 一个运行65535次的循环，如果你的机子太快，不能看清楚两个进程同时运行的效果，那就再加大循环次数。或用
sleep()函数 */ 
      big_loop(FAC_N); 
      /* 取得子进程的PID，你可以看清楚子进程和父进程的PID是不同的（子进程的PID比父进程的要大，因为是在父进程运行
后才创建的）。*/ 
      printf("PID:%d/n", getpid()); 
      /* 子进程执行完了，发送信号 SIGCHLD。由于前面对这个信号进行了注册，这时就会转入信号处理操作。 */ 
      break; 
    /* 哈哈，返回的即不是错误，又不是子进程，那就是父进程喽。*/ 
    default: 
      /* 这里让用户输入了4个数 */ 
      input_information(); 
      /* 取得子进程的PID。*/ 
      printf("PID:%d/n", getpid()); 
      break; 
  } 
  exit(0);   
} 
/*big_loop: 简单，一看就明白，不解释了。*/ 
void big_loop(int n) 
{ 
  int i; 
  for(i = 0; i < n; i++){ 
    switch(i % 4){ 
      case 0: 
        putchar('-'); 
        break; 
      case 1: 
        putchar('/'); 
        break; 
      case 2: 
        putchar('|'); 
        break; 
      case 3: 
        putchar('//'); 
        break; 
    } 
    putchar('/b'); 
  } 
} 
/*input_information: 简单，一看就明白，也不解释了。*/ 
void input_information() 
{ 
  int n_table[4], i; 
  for(i = 0; i < 4; i++){ 
    printf("Number %d:/t", i); 
    scanf("%d", &n_table[i]); 
  } 
   printf("Number1/tNumber2/tNumber3/tNumber4/n"); 
   printf("%d/t%d/t%d/t%d/n", n_table[0], n_table[1], n_table[2], n_table[3]); 
} 
/* 信号处理函数，当子进程结束的时候会向父进程发送 SIGCHLD 信号，这时程序会转入这个函数里进行处理 */ 
void handler() 
{ 
  /* 父进程确认，子进程退出 */ 
  wait(); 
} 
/*--------------------------signal_fork.c------------------------------*/ 
    看到了吧，信号的使用还是很简单的。但是这是不可靠的信号使用方法。为什么？道理很简单，当你程序运行的时候收到一
个需要处理的信号，你的程序就会转入信号处理函数，对信号进行处理，处理完后再继续执行后续程序。在这个过程中如果正好
是执行 malloc() 这样的函数时收到了信号，那么当信号处理完毕后因为 malloc() 不是一个可重入的函数，程序不会继续执行 
malloc() ，这样空间也许不会分配，而造成错误。所以我们还需要可靠的信号处理方式，也就是说当执行某些不可被中断的操作
的时候屏蔽掉信号处理。直到这些关键操作完成，再对信号进行处理。以后我会给大家详细的解释可靠信号的操作。 
引用：[http://blog.csdn.net/zhoujunyi/archive/2007/04/13/1563180.aspx](http://blog.csdn.net/zhoujunyi/archive/2007/04/13/1563180.aspx)
