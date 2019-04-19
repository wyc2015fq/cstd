# linux多线程编程5--信号量(semaphore) - maopig的专栏 - CSDN博客
2012年12月17日 23:30:01[maopig](https://me.csdn.net/maopig)阅读数：1127
信号量本质上是一个非负的整数计数器，也是UNIX中古老的实现进程互斥和同步的手段，Linux下信号量概念是在线程中，信号则在进程控制中，不过原理差不多，最基本最经典的操作莫过于P、V操作了，能实现进程、线程的互斥和同步操作，非常值得深入理解。
1.P、V操作原语
 P操作：
 proceduce P(var s:semaphore)
{
  begin
      s:=s-1;
      if(s<0)then W(s);
  end
}
V操作：
 proceduce V(var s:semaphore)
{
  begin
      s:=s+1;
      if(s<=0)then R(s);
  end
} 
2.基本操作
数据类型：信号量的数据类型为结构sem_t，它本质上是一个长整型的数。
函数：
（1）sem_init
功能：         用于创建一个信号量，并初始化信号量的值。
头文件：       <semaphore.h>
函数原型：     int sem_init (sem_t* sem, int pshared, unsigned int value);
函数传入值：   sem:信号量。
                   pshared:决定信号量能否在几个进程间共享。由于目前LINUX还没有实现进
                               程间共享信息量，所以这个值只能取0。
                   value：初始计算器
函数返回值：   0：成功。
                   -1：失败。
（2）其他函数。
//等待信号量
int sem_wait       (sem_t* sem);
int sem_trywait   (sem_t* sem);
//发送信号量
int sem_post       (sem_t* sem);
//得到信号量值
int sem_getvalue (sem_t* sem);
//删除信号量
int sem_destroy   (sem_t* sem);
功能：sem_wait和sem_trywait相当于P操作，它们都能将信号量的值减一，两者的区别在
        于若信号量的值小于零时，sem_wait将会阻塞进程，而sem_trywait则会立即返回。
        sem_post相当于V操作，它将信号量的值加一，同时发出唤醒的信号给等待的进程
       （或线程）。
        sem_getvalue 得到信号量的值。
        sem_destroy 摧毁信号量。
函数传入值： sem:信号量。
函数返回值： 同上。
好了，了解完基本操作，继续做一个练习：
这里用信号量实现互斥资源访问的功能：
/*sem.c*/  
#include <stdlib.h>     
#include <stdio.h>     
#include <pthread.h>     
#include <semaphore.h>   
#include <errno.h>      
/*全局变量*/  
int gnum = 0;   
/*信号量*/  
sem_t sem;   
/*声明线程运行服务程序*/  
static void pthread_func_1 (void);      
static void pthread_func_2 (void);      
int main (void)      
{      
 /*线程的标识符*/  
  pthread_t pt_1 = 0;      
  pthread_t pt_2 = 0;      
  int ret = 0;      
  /*信号量初始化*/  
  sem_init(&sem,0,1);   
  /*分别创建线程1、2*/  
  ret = pthread_create (&pt_1,          //线程标识符指针   
                         NULL,          //默认属性   
                        (void *)pthread_func_1,//运行函数   
                        NULL);          //无参数   
  if (ret != 0)      
  {      
     perror ("pthread_1_create");      
  }      
  ret = pthread_create (&pt_2,          //线程标识符指针   
                        NULL,           //默认属性     
                        (void *)pthread_func_2, //运行函数   
                        NULL);          //无参数   
  if (ret != 0)      
  {      
     perror ("pthread_2_create");      
  }      
  /*等待线程1、2的结束*/  
  pthread_join (pt_1, NULL);      
  pthread_join (pt_2, NULL);      
  printf ("main programme exit!/n");     
  return 0;      
}      
/*线程1的服务程序*/  
static void pthread_func_1 (void)      
{      
  int i = 0;      
  for (;;)      
  {      
    printf ("This is pthread1!/n");       
    sem_wait(&sem);     /*等待信号量*/      
    sleep (1);    
    /*临界资源*/  
    gnum++;   
    printf ("Thread1 add one to num:%d/n",gnum);   
    sem_post (&sem);        /*释放信号量*/  
  }      
}      
/*线程2的服务程序*/    
static void pthread_func_2 (void)      
{      
  int i = 0;      
  for (;;)      
  {      
    printf ("This is pthread2!/n");    
    sem_wait(&sem);     /*等待信号量*/  
    sleep (1);   
    /*临界资源*/  
    gnum++;   
    printf ("Thread2 add one to num:%d/n",gnum);   
    sem_post (&sem);        /*释放信号量*/  
  }      
  pthread_exit (0);      
}
编译，运行，可以看出和上次互斥锁结果一样的
