# Posix多线程编程学习笔记（三）—信号灯（3） - ljx0305的专栏 - CSDN博客
2008年08月01日 11:12:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：627
**4.posix****有名信号灯应用于多进程<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />**
下面就是应用Posix有名信号灯的一个小程序。用它来限制访问共享代码的进程数目。
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
void print(pid_t);
sem_t *sem; /*定义Posix有名信号灯*/
int val; /*定义信号灯当前值*/
int main(int argc,char *argv[])
{
int n=0;
if(argc!=2)
{
printf(“please input a file name!/n”);
exit(1);
}
sem=sem_open(argv[1],O_CREAT,0644,2); /*打开一个信号灯, 初值设为2*/
while(n++<5) /*循环创建5个子进程，使它们同步运行*/
{
if(fork()==0) 
{
sem_wait(sem); /*申请信号灯*/
print(getpid()); /*调用共享代码段*/
sleep(1); 
sem_post(sem); /*释放信号灯*/
printf(“I’m finished,my pid is %d/n”,getpid());
return 0; 
}
wait(); /*等待所有子进程结束*/
sem_close(sem);
sem_unlink(argv[1]);
exit(0);
}
void print(pid_t pid)
{
printf(“I get it,my pid is %d/n”,pid);
sem_getvalue(sem,&val);
printf(“Now the value have %d/n”,val);
}
程序编译后运行会得到如下结果：
#./8_2 8_2.c
I get it,my tid is 1082330304
Now the value have 1
I get it,my tid is 1090718784
Now the value have 0
I finished,my pid is 1082330304
I finished,my pid is 1090718784
I get it,my tid is 1099107264
Now the value have 1
I get it,my tid is 1116841120
Now the value have 0
I finished,my pid is 1099107264
I finished,my pid is 1116841120
I get it,my tid is 1125329600
Now the value have 1
I finished,my pid is 1125329600
**二、基于内存的信号灯**
前面已经介绍了Posix有名信号灯。这些信号灯由一个name参数标识，它通常指代文件系统中的某个文件。然而Posix也提供基于内存的信号灯，它们由应用程序分配信号灯的内存空间，然后由系统初始化它们的值。
7.
|名称:：|sem_init/sem_destroy|
|----|----|
|功能：|初始化/关闭信号等|
|头文件：|#include <semaphore.h>|
|函数原形：|int sem_init(sem_t *sem,int shared,unsigned int value);int sem_getvalue(sem_t *sem);|
|参数：|sem指向信号灯的指针shared作用范围value信号灯初始值|
|返回值：|若成功则返回0，否则返回-1。|
基于内存的信号灯是由sem_init初始化的。sem参数指向必须由应用程序分配的sem_t变量。如果shared为0，那么待初始化的信号灯是在同一进程的各个线程共享的，否则该信号灯是在进程间共享的。当shared为零时，该信号灯必须存放在即将使用它的所有进程都能访问的某种类型的共享内存中。跟sem_open一样，value参数是该信号灯的初始值。
使用完一个基于内存的信号灯后，我们调用sem_destroy关闭它。
除了sem_open和sem_close外，其它的poisx有名信号灯函数都可以用于基于内存的信号灯。
注意：posix基于内存的信号灯和posix有名信号灯有一些区别，我们必须注意到这些。
1.sem_open不需要类型与shared的参数，有名信号灯总是可以在不同进程间共享的。
2.sem_init不使用任何类似于O_CREAT标志的东西，也就是说，sem_init总是初始化信号灯的值。因此，对于一个给定的信号灯，我们必须小心保证只调用一次sem_init。
3.sem_open返回一个指向某个sem_t变量的指针，该变量由函数本身分配并初始化。但sem_init的第一个参数是一个指向某个sem_t变量的指针，该变量由调用者分配，然后由sem_init函数初始化。
4.posix有名信号灯是通过内核持续的，一个进程创建一个信号灯，另外的进程可以通过该信号灯的外部名（创建信号灯使用的文件名）来访问它。posix基于内存的信号灯的持续性却是不定的，如果基于内存的信号灯是由单个进程内的各个线程共享的，那么该信号灯就是随进程持续的，当该进程终止时它也会消失。如果某个基于内存的信号灯是在不同进程间同步的，该信号灯必须存放在共享内存区中，这要只要该共享内存区存在，该信号灯就存在。
5.基于内存的信号灯应用于线程很麻烦（待会你会知道为什么），而有名信号灯却很方便，基于内存的信号灯比较适合应用于一个进程的多个线程。
下面是posix基于内存的信号灯实现一个进程的各个线程间的互次。
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#incude <stdlib.h>
void *thread_function(void *arg); /*线程入口函数*/
void print(void); /*共享资源函数*/
sem_t bin_sem; /*定义信号灯*/
int value; /*定义信号量的灯*/
int main()
{
int n=0;
pthread_t a_thread; 
if((sem_init(&bin_sem,0,2))!=0) /*初始化信号灯，初始值为2*/
{
perror(“sem_init”);
exit(1);
}
while(n++<5) /*循环创建5个线程*/
{
if((pthread_create(&a_thread,NULL,thread_function,NULL))!=0) 
{
perror(“Thread creation failed”);
exit(1);
}
}
pthread_join(a_thread,NULL);/*等待子线程返回*/
}
void *thread_function(void *arg)
{
sem_wait(&bin_sem); /*等待信号灯*/
print();
sleep(1);
sem_post(&bin_sem); /*挂出信号灯*/
printf(“I finished,my pid is %d/n”,pthread_self());
pthread_exit(arg);
}
void print()
{
printf(“I get it,my tid is %d/n”,pthread_self());
sem_getvalue(&bin_sem,&value); /*获取信号灯的值*/
printf(“Now the value have %d/n”,value);
}
posix基于内存的信号灯和有名信号灯基本是一样的，上面的几点区别就可以了。
下面是运行结果：
#gcc –lpthread –o seminitthread seminitthread.c
#./seminitthread 
I get it,my tid is 1082330304
Now the value have 1
I get it,my tid is 1090718784
Now the value have 0
I finished,my pid is 1082330304
I finished,my pid is 1090718784
I get it,my tid is 1099107264
Now the value have 1
I get it,my tid is 1116841120
Now the value have 0
I finished,my pid is 1099107264
I finished,my pid is 1116841120
I get it,my tid is 1125329600
Now the value have 1
I finished,my pid is 1125329600
下面的程序并不能得到我们想要的结果。
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
void print(pid_t);
sem_t *sem; /*定义Posix有名信号灯*/
int val; /*定义信号灯当前值*/
int main(int argc,char *argv[])
{
int n=0;
sem=sem_open(argv[1],O_CREAT,0644,3); /*打开一个信号灯*/
sem_getvalue(sem,&val); /*查看信号灯的值*/
printf(“The value have %d/n”,val);
while(n++<5) /*循环创建5个子进程，使它们同步运行*/
{
if(fork()==0) 
{
sem_wait(sem); /*申请信号灯*/
print(getpid()); /*调用共享代码段*/
sleep(1); 
sem_post(sem); /*释放信号灯*/
printf(“I’m finished,my pid is %d/n”,getpid());
return 0; 
}
wait(); /*等待所有子进程结束*/
return 0;
}
void print(pid_t pid)
{
printf(“I get it,my pid is %d/n”,pid);
sem_getvalue(sem,&val);
printf(“Now the value have %d/n”,val);
}
下面是运行结果：
#cc –lpthread –o sem sem.c
#./sem 
The value have 3
I get it,my pid is 2236
Now the value have 2
I get it,my pid is 2237
Now the value have 2
I get it,my pid is 2238
Now the value have 2
I get it,my pid is 2239
Now the value have 2
Iget it,my pid is 2240
Now the value have 2
I’m finished,my pid is 2236
I’m finished,my pid is 2237
I’m finished,my pid is 2238
I’m finished,my pid is 2239
I’m finished,my pid is 2240
问题在于sem信号灯不在共享内存区中。fork出来的子进程通常不共享父进程的内存空间。子进程是在父进程内存空间的拷贝上启动的，它跟共享内存不是一回事。
引用:http://blog.chinaunix.net/u/22935/showart_340400.html
