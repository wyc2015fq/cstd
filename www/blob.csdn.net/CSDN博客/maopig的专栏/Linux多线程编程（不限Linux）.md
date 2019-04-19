# Linux多线程编程（不限Linux） - maopig的专栏 - CSDN博客
2011年07月20日 22:40:12[maopig](https://me.csdn.net/maopig)阅读数：762标签：[linux																[多线程																[编程																[thread																[signal																[join](https://so.csdn.net/so/search/s.do?q=join&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[c/C++																[Linux](https://blog.csdn.net/maopig/article/category/850267)](https://blog.csdn.net/maopig/article/category/847675)
——本文一个例子展开，介绍Linux下面线程的操作、多线程的同步和互斥。
# 前言
线程？为什么有了进程还需要线程呢，他们有什么区别？使用线程有什么优势呢？还有多线程编程的一些细节问题，如线程之间怎样同步、互斥，这些东西将在本文中介绍。我在某QQ群里见到这样一道面试题：
> 
是否熟悉POSIX多线程编程技术？如熟悉，编写程序完成如下功能：
1）有一int型全局变量g_Flag初始值为0；
2） 在主线称中起动线程1，打印“this is thread1”，并将g_Flag设置为1
3） 在主线称中启动线程2，打印“this is thread2”，并将g_Flag设置为2
4） 线程序1需要在线程2退出后才能退出
5） 主线程在检测到g_Flag从1变为2，或者从2变为1的时候退出
我们带着这题开始这篇文章，结束之后，大家就都会做了。本文的框架如下：
- 1、进程与线程 
- 2、使用线程的理由 
- 3、有关线程操作的函数 
- 4、线程之间的互斥 
- 5、线程之间的同步 
- 6、试题最终代码 
# 1、进程与线程
进程是程序执行时的一个实例，即它是程序已经执行到何种程度的数据结构的汇集。从内核的观点看，进程的目的就是担当分配系统资源（CPU时间、内存等）的基本单位。
线程是进程的一个执行流，是CPU调度和分派的基本单位，它是比进程更小的能独立运行的基本单位。一个进程由几个线程组成（拥有很多相对独立的执行流的用户程序共享应用程序的大部分数据结构），线程与同属一个进程的其他的线程共享进程所拥有的全部资源。
> 
"进程——资源分配的最小单位，线程——程序执行的最小单位"
进程有独立的地址空间，一个进程崩溃后，在保护模式下不会对其它进程产生影响，而线程只是一个进程中的不同执行路径。线程有自己的堆栈和局部变量，但线程没有单独的地址空间，一个线程死掉就等于整个进程死掉，所以多进程的程序要比多线程的程序健壮，但在进程切换时，耗费资源较大，效率要差一些。但对于一些要求同时进行并且又要共享某些变量的并发操作，只能用线程，不能用进程。
# 2、使用线程的理由
从上面我们知道了进程与线程的区别，其实这些区别也就是我们使用线程的理由。总的来说就是：进程有独立的地址空间，线程没有单独的地址空间（同一进程内的线程共享进程的地址空间）。（下面的内容摘自[Linux下的多线程编程](http://fanqiang.chinaunix.net/a4/b8/20010811/0905001105.html)）
使用多线程的**理由之一**是和进程相比，它是一种非常"节俭"的多任务操作方式。我们知道，在Linux系统下，启动一个新的进程必须分配给它独立的地址空间，建立众多的数据表来维护它的代码段、堆栈段和数据段，这是一种"昂贵"的多任务工作方式。而运行于一个进程中的多个线程，它们彼此之间使用相同的地址空间，共享大部分数据，启动一个线程所花费的空间远远小于启动一个进程所花费的空间，而且，线程间彼此切换所需的时间也远远小于进程间切换所需要的时间。据统计，总的说来，一个进程的开销大约是一个线程开销的30倍左右，当然，在具体的系统上，这个数据可能会有较大的区别。
使用多线程的**理由之二**是线程间方便的通信机制。对不同进程来说，它们具有独立的数据空间，要进行数据的传递只能通过通信的方式进行，这种方式不仅费时，而且很不方便。线程则不然，由于同一进程下的线程之间共享数据空间，所以一个线程的数据可以直接为其它线程所用，这不仅快捷，而且方便。当然，数据的共享也带来其他一些问题，有的变量不能同时被两个线程所修改，有的子程序中声明为static的数据更有可能给多线程程序带来灾难性的打击，这些正是编写多线程程序时最需要注意的地方。
除了以上所说的优点外，不和进程比较，多线程程序作为一种多任务、并发的工作方式，当然有以下的优点：
- 提高应用程序响应。这对图形界面的程序尤其有意义，当一个操作耗时很长时，整个系统都会等待这个操作，此时程序不会响应键盘、鼠标、菜单的操作，而使用多线程技术，将耗时长的操作（time consuming）置于一个新的线程，可以避免这种尴尬的情况。
- 使多CPU系统更加有效。操作系统会保证当线程数不大于CPU数目时，不同的线程运行于不同的CPU上。 
- 改善程序结构。一个既长又复杂的进程可以考虑分为多个线程，成为几个独立或半独立的运行部分，这样的程序会利于理解和修改。 
=============================
从函数调用上来说，进程创建使用fork()操作；线程创建使用clone()操作。Richard Stevens大师这样说过：
- 
fork is expensive. Memory is copied from the parent to the child, all descriptors are duplicated in the child, and so on. Current implementations use a technique called copy-on-write, which avoids a copy of the parent's data space
 to the child until the child needs its own copy. But, regardless of this optimization,
fork is expensive.
- 
IPC is required to pass information between the parent and child after the
fork. Passing information from the parent to the child before the fork is easy, since the child starts with a copy of the parent's data space and with a copy of all the parent's descriptors. But, returning information from the child to the
 parent takes more work.
Threads help with both problems. Threads are sometimes called lightweight processes since a thread is "lighter weight" than a process. That is, thread creation can be 10–100 times faster than process creation.
All threads within a process share the same global memory. This makes the sharing of information easy between the threads, but along with this simplicity comes the problem of synchronization.
=============================
# 3、有关线程操作的函数
#include <pthread.h>
 
int pthread_create(pthread_t *tid, const pthread_attr_t *attr, void *(*func) (void *), void *arg);  \nint pthread_join (pthread_t tid, void ** status);
pthread_t pthread_self (void);int pthread_detach (pthread_t tid);void pthread_exit (void *status);
pthread_create用于创建一个线程，成功返回0，否则返回Exxx（为正数）。
- pthread_t *tid：线程id的类型为pthread_t，通常为无符号整型，当调用pthread_create成功时，通过*tid指针返回。 
- const pthread_attr_t *attr：指定创建线程的属性，如线程优先级、初始栈大小、是否为守护进程等。可以使用NULL来使用默认值，通常情况下我们都是使用默认值。
- void *(*func) (void *)：函数指针func，指定当新的线程创建之后，将执行的函数。
- void *arg：线程将执行的函数的参数。如果想传递多个参数，请将它们封装在一个结构体中。
pthread_join用于等待某个线程退出，成功返回0，否则返回Exxx（为正数）。
- pthread_t tid：指定要等待的线程ID 
- void ** status：如果不为NULL，那么线程的返回值存储在status指向的空间中（这就是为什么status是二级指针的原因！这种才参数也称为“值-结果”参数）。
pthread_self用于返回当前线程的ID。
pthread_detach用于是指定线程变为**分离**状态，就像进程脱离终端而变为后台进程类似。成功返回0，否则返回Exxx（为正数）。变为分离状态的线程，如果线程退出，它的所有资源将全部释放。而如果不是分离状态，线程必须保留它的线程ID，退出状态直到其它线程对它调用了pthread_join。
> 
进程也是类似，这也是当我们打开进程管理器的时候，发现有很多僵死进程的原因！也是为什么一定要有僵死这个进程状态。
pthread_exit用于终止线程，可以指定返回值，以便其他线程通过pthread_join函数获取该线程的返回值。
- void *status：指针线程终止的返回值。 
知道了这些函数之后，我们试图来完成本文一开始的问题：
1）有一int型全局变量g_Flag初始值为0；
2）在主线称中起动线程1，打印“this is thread1”，并将g_Flag设置为1
3）在主线称中启动线程2，打印“this is thread2”，并将g_Flag设置为2
这3点很简单嘛！！！不就是调用pthread_create创建线程。代码如下：
/*
 * 1）有一int型全局变量g_Flag初始值为0；
 *
 * 2）在主线称中起动线程1，打印“this is thread1”，并将g_Flag设置为1
 *
 * 3）在主线称中启动线程2，打印“this is thread2”，并将g_Flag设置为2
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<unistd.h>
int g_Flag=0;
void* thread1(void*);
void* thread2(void*);
/*
 * when program is started, a single thread is created, called the initial thread or main thread.
 * Additional threads are created by pthread_create.
 * So we just need to create two thread in main().
 */int main(int argc, char** argv)
{
	printf("enter main\n");
	pthread_t tid1, tid2;
	int rc1=0, rc2=0;
	rc2 = pthread_create(&tid2, NULL, thread2, NULL);
	if(rc2 != 0)
		printf("%s: %d\n",__func__, strerror(rc2));
	rc1 = pthread_create(&tid1, NULL, thread1, &tid2);
	if(rc1 != 0)
		printf("%s: %d\n",__func__, strerror(rc1));
	printf("leave main\n");
	exit(0);	
}
/*
 * thread1() will be execute by thread1, after pthread_create()
 * it will set g_Flag = 1;
 */void* thread1(void* arg)
{
	printf("enter thread1\n");
	printf("this is thread1, g_Flag: %d, thread id is %u\n",g_Flag, (unsignedint)pthread_self());
	g_Flag = 1;
	printf("this is thread1, g_Flag: %d, thread id is %u\n",g_Flag, (unsignedint)pthread_self());
	printf("leave thread1\n");
	pthread_exit(0);
}
/*
 * thread2() will be execute by thread2, after pthread_create()
 * it will set g_Flag = 2;
 */void* thread2(void* arg)
{
	printf("enter thread2\n");
	printf("this is thread2, g_Flag: %d, thread id is %u\n",g_Flag, (unsignedint)pthread_self());
	g_Flag = 2;
	printf("this is thread1, g_Flag: %d, thread id is %u\n",g_Flag, (unsignedint)pthread_self());
	printf("leave thread2\n");
	pthread_exit(0);
}
这样就完成了1）、2）、3）这三点要求。编译执行得如下结果：
netsky@ubuntu:~/workspace/pthead_test$ gcc -lpthread test.c
如果程序中使用到了pthread库中的函数，除了要#include<pthread.h>，在编译的时候还有加上-lpthread 选项。
netsky@ubuntu:~/workspace/pthead_test$ ./a.out 
enter main 
enter thread2 
this is thread2, g_Flag: 0, thread id is 3079588720 
this is thread1, g_Flag: 2, thread id is 3079588720 
leave thread2 
leave main 
enter thread1 
this is thread1, g_Flag: 2, thread id is 3071196016 
this is thread1, g_Flag: 1, thread id is 3071196016 
leave thread1 
但是运行结果不一定是上面的，还有可能是：
netsky@ubuntu:~/workspace/pthead_test$ ./a.out 
enter main 
leave main 
enter thread1 
this is thread1, g_Flag: 0, thread id is 3069176688 
this is thread1, g_Flag: 1, thread id is 3069176688 
leave thread1 
或者是：
netsky@ubuntu:~/workspace/pthead_test$ ./a.out 
enter main 
leave main 
等等。这也很好理解因为，这取决于主线程main函数何时终止，线程thread1、thread2是否能够来得急执行它们的函数。这也是多线程编程时要注意的问题，因为有可能一个线程会影响到整个进程中的所有其它线程！如果我们在main函数退出前，sleep()一段时间，就可以保证thread1、thread2来得及执行。
> 
Attention:大家肯定已经注意到了，我们在线程函数thread1()、thread2()执行完之前都调用了pthread_exit。如果我是调用exit()又或者是return会怎样呢？自己动手试试吧！
pthread_exit()用于线程退出，可以指定返回值，以便其他线程通过pthread_join（）函数获取该线程的返回值。 
return是函数返回，只有线程函数return，线程才会退出。 
exit是进程退出，如果在线程函数中调用exit，进程中的所有函数都会退出！
“4） 线程序1需要在线程2退出后才能退出”第4点也很容易解决，直接在thread1的函数退出之前调用pthread_join就OK了。
# 4、线程之间的互斥
上面的代码似乎很好的解决了问题的前面4点要求，其实不然！！！因为g_Flag是一个全局变量，线程thread1和thread2可以同时对它进行操作，需要对它进行加锁保护，thread1和thread2要互斥访问才行。下面我们就介绍如何加锁保护——互斥锁。
> 
#### **互斥锁：**
使用互斥锁（互斥）可以使线程按顺序执行。通常，互斥锁通过确保一次只有一个线程执行代码的临界段来同步多个线程。互斥锁还可以保护单线程代码。
互斥锁的相关操作函数如下：
#include <pthread.h> 
int pthread_mutex_lock(pthread_mutex_t * mptr); 
int pthread_mutex_unlock(pthread_mutex_t * mptr); 
//Both return: 0 if OK, positive Exxx value on error
在对临界资源进行操作之前需要pthread_mutex_lock先加锁，操作完之后pthread_mutex_unlock再解锁。而且在这之前需要声明一个pthread_mutex_t类型的变量，用作前面两个函数的参数。具体代码见第5节。
# 5、线程之间的同步
第5点——主线程在检测到g_Flag从1变为2，或者从2变为1的时候退出。就需要用到线程同步技术！线程同步需要条件变量。
> 
#### **条件变量：**
使用条件变量可以以原子方式阻塞线程，直到某个特定条件为真为止。条件变量始终与互斥锁一起使用。对条件的测试是在互斥锁（互斥）的保护下进行的。
如果条件为假，线程通常会基于条件变量阻塞，并以原子方式释放等待条件变化的互斥锁。如果另一个线程更改了条件，该线程可能会向相关的条件变量发出信号，从而使一个或多个等待的线程执行以下操作：
- 唤醒 
- 再次获取互斥锁 
- 重新评估条件 
在以下情况下，条件变量可用于在进程之间同步线程：
- 线程是在可以写入的内存中分配的 
- 内存由协作进程共享 
“使用条件变量可以以原子方式阻塞线程，直到某个特定条件为真为止。”即可用到第5点，主线程main函数阻塞于等待g_Flag从1变为2，或者从2变为1。条件变量的相关函数如下：
#include <pthread.h>
 
int pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr); 
int pthread_cond_signal(pthread_cond_t *cptr); 
//Both return: 0 if OK, positive Exxx value on error
pthread_cond_wait用于等待某个特定的条件为真，pthread_cond_signal用于通知阻塞的线程某个特定的条件为真了。在调用者两个函数之前需要声明一个pthread_cond_t类型的变量，用于这两个函数的参数。
为什么条件变量始终与互斥锁一起使用，对条件的测试是在互斥锁（互斥）的保护下进行的呢？因为“某个特性条件”通常是在多个线程之间共享的某个变量。互斥锁允许这个变量可以在不同的线程中设置和检测。
通常，pthread_cond_wait只是唤醒等待某个条件变量的一个线程。如果需要唤醒所有等待某个条件变量的线程，需要调用：
intpthread_cond_broadcast (pthread_cond_t * cptr);
默认情况下面，阻塞的线程会一直等待，知道某个条件变量为真。如果想设置最大的阻塞时间可以调用：
intpthread_cond_timedwait (pthread_cond_t * cptr, pthread_mutex_t *mptr, conststruct timespec *abstime);
如果时间到了，条件变量还没有为真，仍然返回，返回值为ETIME。
# 6、试题最终代码
通过前面的介绍，我们可以轻松的写出代码了，如下所示：
/*
 是否熟悉POSIX多线程编程技术？如熟悉，编写程序完成如下功能：
  1）有一int型全局变量g_Flag初始值为0；
  2）在主线称中起动线程1，打印“this is thread1”，并将g_Flag设置为1
  3）在主线称中启动线程2，打印“this is thread2”，并将g_Flag设置为2
  4）线程序1需要在线程2退出后才能退出
  5）主线程在检测到g_Flag从1变为2，或者从2变为1的时候退出
   */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<unistd.h>
typedefvoid* (*fun)(void*);
int g_Flag=0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
void* thread1(void*);
void* thread2(void*);
/*
 *  when program is started, a single thread is created, called the initial thread or main thread.
 *  Additional threads are created by pthread_create.
 *  So we just need to create two thread in main().
 */int main(int argc, char** argv)
{
	printf("enter main\n");
	pthread_t tid1, tid2;
	int rc1=0, rc2=0;
	rc2 = pthread_create(&tid2, NULL, thread2, NULL);
	if(rc2 != 0)
		printf("%s: %d\n",__func__, strerror(rc2));
	rc1 = pthread_create(&tid1, NULL, thread1, &tid2);
	if(rc1 != 0)
		printf("%s: %d\n",__func__, strerror(rc1));
	pthread_cond_wait(&cond, &mutex);
	printf("leave main\n");
	exit(0);	
}
/*
 * thread1() will be execute by thread1, after pthread_create()
 * it will set g_Flag = 1;
 */void* thread1(void* arg)
{
	printf("enter thread1\n");
	printf("this is thread1, g_Flag: %d, thread id is %u\n",g_Flag, (unsignedint)pthread_self());
	pthread_mutex_lock(&mutex);
	if(g_Flag == 2)
		pthread_cond_signal(&cond);
	g_Flag = 1;
	printf("this is thread1, g_Flag: %d, thread id is %u\n",g_Flag, (unsignedint)pthread_self());
	pthread_mutex_unlock(&mutex);
	pthread_join(*(pthread_t*)arg, NULL);
	printf("leave thread1\n");
	pthread_exit(0);
}
/*
 * thread2() will be execute by thread2, after pthread_create()
 * it will set g_Flag = 2;
 */void* thread2(void* arg)
{
	printf("enter thread2\n");
	printf("this is thread2, g_Flag: %d, thread id is %u\n",g_Flag, (unsignedint)pthread_self());
	pthread_mutex_lock(&mutex);
	if(g_Flag == 1)
		pthread_cond_signal(&cond);
	g_Flag = 2;
	printf("this is thread1, g_Flag: %d, thread id is %u\n",g_Flag, (unsignedint)pthread_self());
	pthread_mutex_unlock(&mutex);
	printf("leave thread2\n");
	pthread_exit(0);
}
编译运行可以得到符合要求的结果！
