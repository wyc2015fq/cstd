# 【Linux】线程同步之信号量同步 - 江南烟雨 - CSDN博客
2013年01月05日 11:19:36[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：10834
linux中两种基本的同步方法是信号量和互斥量。这两种方法很相似，而且它们可以相互通过对方来实现。
## **信号量概述**
下面介绍用信号量进行同步。
信号量概念由荷兰科学家Dijkstra首先提出。信号量是一个特殊类型的变量，它可以被增加或者减少。但对其的关键访问被保证是原子操作，即使在一个多线程程序中也是如此。
信号量有两种类型：
（1）二进制信号量。它只有0和1两种取值。
（2）计数信号量。它可以有更大的取值范围。
如果要用信号量来保护一段代码，使其每次只能被一个执行线程运行，就要用到二进制信号量、。
如果要允许有限数目的线程执行一段指定的代码，就需要用到计数信号量。
由于计数信号量并不常用，而且它实际上仅仅是二进制信号量的一种扩展，这里之介绍二进制信号量。
## **信号量的相关函数**
信号量函数的名字都以sem_开头。线程中使用的基本函数有4个。
注意，需要包含头文件：
```cpp
#include<semaphore.h>
```
### 创建信号量
```cpp
int sem_init(sem_t *sem, int pshared, unsigned int value);
```
**函数解释：**
sem_init() 初始化一个定位在 sem 的匿名信号量。value 参数指定信号量的初始值。 pshared 参数指明信号量是由进程内线程共享，还是由进程之间共享。如果 pshared 的值为 0，那么信号量将被进程内的线程共享，并且应该放置在所有线程都可见的地址上(如全局变量，或者堆上动态分配的变量)。
　　如果 pshared 是非零值，那么信号量将在进程之间共享，并且应该定位共享内存区域(见 shm_open(3)、mmap(2) 和 shmget(2))。(因为通过 fork(2) 创建的孩子继承其父亲的内存映射，因此它也可以见到这个信号量。)所有可以访问共享内存区域的进程都可以使用sem_post(3)、sem_wait(3) 等等操作信号量。初始化一个已经初始的信号量其结果未定义。
**返回值**
　　sem_init() 成功时返回 0；错误时，返回 -1，并把 errno 设置为合适的值。
**错误**
　　EINVAL
　　value 超过 SEM_VALUE_MAX。
　　ENOSYS
　　pshared 非零，但系统还没有支持进程共享的信号量。
下面是控制信号量的两个函数：
### 信号量减一操作
```cpp
int sem_wait(sem_t * sem);
```
**函数说明**
　　sem_wait函数也是一个原子操作，它的作用是从信号量的值减去一个“1”，但它永远会先等待该信号量为一个非零值才开始做减法。也就是说，如果你对一个值为2的信号量调用sem_wait(),线程将会继续执行，这信号量的值将减到1。如果对一个值为0的信号量调用sem_wait()，这个函数就 会地等待直到有其它线程增加了这个值使它不再是0为止。如果有两个线程都在sem_wait()中等待同一个信号量变成非零值，那么当它被第三个线程增加 一个“1”时，等待线程中只有一个能够对信号量做减法并继续执行，另一个还将处于等待状态。
**返回值**
　　所有这些函数在成功时都返回 0；错误保持信号量值没有更改，-1 被返回，并设置 errno 来指明错误。
**错误**
　　EINTR
　　这个调用被信号处理器中断，
　　EINVAL
　　sem 不是一个有效的信号量。
### 信号量加一操作
```cpp
int sem_post(sem_t * sem);
```
说明
　　sem_post函数的作用是给信号量的值加上一个“1”，它是一个“原子操作”－－－即同时对同一个信号量做加“1”操作的两个线程是不会冲突的；而同 时对同一个文件进行读、加和写操作的两个程序就有可能会引起冲突。信号量的值永远会正确地加一个“2”－－因为有两个线程试图改变它。
返回值
　　sem_post() 成功时返回 0；错误时，信号量的值没有更改，-1 被返回，并设置 errno 来指明错误。
错误
　　EINVAL
　　sem 不是一个有效的信号量。
　　EOVERFLOW
　　信号量允许的最大值将要被超过。
### 清理信号量
```cpp
int sem_destroy (sem_t *sem);
```
 这个函数也使用一个信号量指针做参数，归还自己战胜的一切资源。在清理信号量的时候如果还有线程在等待它，用户就会收到一个错误。
        与其它的函数一样，这些函数在成功时都返回“0”。
## **信号量的使用**
下面主线程中创建了一个新线程，用来统计输入的字符串中字符的个数。信号量用来控制两个线程对存储字符串数组的访问。
代码：
```cpp
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
//线程函数 
void *thread_function(void *arg);
sem_t bin_sem;//信号量对象
#define WORK_SIZE 1024
char work_area[WORK_SIZE];//工作区
int main()
{
    int res;
    pthread_t a_thread;
    void *thread_result;
    res = sem_init(&bin_sem,0,0);//初始化信号量对象
    if(res)//初始化信号量失败
    {
	perror("Semaphore initialization failed\n");
	exit(EXIT_FAILURE);
    }
    //创建新线程
    res = pthread_create(&a_thread,NULL,thread_function,NULL);
    if(res)
    {
	perror("Thread creation failed\n");
	exit(EXIT_FAILURE);
    }
    printf("Input some text.Enter 'end' to finish\n");
    while(strncmp("end",work_area,3) != 0)
    {//输入没有结束
	fgets(work_area,WORK_SIZE,stdin);
	sem_post(&bin_sem);//给信号量值加一
    }
    printf("waiting for thread to finish\n");
    //等待子线程结束，收集子线程信息
    res = pthread_join(a_thread,&thread_result);
    if(res)
    {
	perror("Thread join failed\n");
	exit(EXIT_FAILURE);
    }
    printf("Thread joined\n");
    //销毁信号量对象
    sem_destroy(&bin_sem);
    exit(EXIT_SUCCESS);
}
void *thread_function(void *arg)
{
	sem_wait(&bin_sem);//将信号量值减一。
	while(strncmp("end",work_area,3))
	{
	    printf("You input %d characters\n",strlen(work_area) - 1);
	    sem_wait(&bin_sem);
	}
	pthread_exit(NULL);//线程终止执行
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201301/05/1357356272_3059.png)
