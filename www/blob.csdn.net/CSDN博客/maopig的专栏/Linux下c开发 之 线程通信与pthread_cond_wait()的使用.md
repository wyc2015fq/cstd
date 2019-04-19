# Linux下c开发 之 线程通信与pthread_cond_wait()的使用 - maopig的专栏 - CSDN博客
2012年02月19日 17:03:39[maopig](https://me.csdn.net/maopig)阅读数：10043
## [pthread_cond_wait()](http://www.cnblogs.com/diyingyun/archive/2011/11/25/2263164.html)
/************pthread_cond_wait()的使用方法**********/
    pthread_mutex_lock(&qlock);    
    pthread_cond_wait(&qready, &qlock);
    pthread_mutex_unlock(&qlock);
/*****************************************************/
The mutex passed to pthread_cond_wait protects the condition.The caller passes it locked to the function, which then atomically places them calling thread on the list of threads waiting for the condition and unlocks the mutex. This closes the window between
 the time that the condition is checked and the time that the thread goes to sleep waiting for the condition to change, so that the thread doesn't miss a change in the condition. When pthread_cond_wait returns, the mutex is again locked.
上面是APUE的原话，就是说pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)函数传入的参数mutex用于保护条件，因为我们在调用pthread_cond_wait时，如果条件不成立我们就进入阻塞，但是进入阻塞这个期间，如果条件变量改变了的话，那我们就漏掉了这个条件。因为这个线程还没有放到等待队列上，所以调用pthread_cond_wait前要先锁互斥量，即调用pthread_mutex_lock(),pthread_cond_wait在把线程放进阻塞队列后，自动对mutex进行解锁，使得其它线程可以获得加锁的权利。这样其它线程才能对临界资源进行访问并在适当的时候唤醒这个阻塞的进程。当pthread_cond_wait返回的时候又自动给mutex加锁。
实际上边代码的加解锁过程如下：
/************pthread_cond_wait()的使用方法**********/
pthread_mutex_lock(&qlock);    /*lock*/
pthread_cond_wait(&qready, &qlock); /*block-->unlock-->wait() return-->lock*/
pthread_mutex_unlock(&qlock); /*unlock*/
/*****************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
void* testThreadPool(int *t);
pthread_mutex_t clifd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clifd_cond = PTHREAD_COND_INITIALIZER;
int a = 0;
int main() {
int sock_fd, conn_fd;
int optval;
socklen_t cli_len;
struct sockaddr_in cli_addr, serv_addr;
sock_fd = socket(AF_INET, SOCK_STREAM, 0);
if (sock_fd < 0) {
   printf("socket\n");
}
optval = 1;
if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *) &optval,
    sizeof(int)) < 0) {
   printf("setsockopt\n");
}
memset(&serv_addr, 0, sizeof(struct sockaddr_in));
serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(4507);
serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
if (bind(sock_fd, (struct sockaddr *) &serv_addr,
    sizeof(struct sockaddr_in)) < 0) {
   printf("bind\n");
}
if (listen(sock_fd, 100) < 0) {
   printf("listen\n");
}
cli_len = sizeof(struct sockaddr_in);
int t;
pthread_t * mythread;
mythread = (pthread_t*) malloc(100 * sizeof(pthread_t));
for (t = 0; t < 5; t++) {
   int *i=(int*)malloc(sizeof(int));
   *i=t;
   if (pthread_create(&mythread[t], NULL, (void*)testThreadPool, (void*)i) != 0) {
    printf("pthread_create");
   }
}
while (1) {
   conn_fd = accept(sock_fd, (struct sockaddr *) &cli_addr, &cli_len);
   if (conn_fd < 0) {
    printf("accept\n");
   }
   printf("accept a new client, ip:%s\n", inet_ntoa(cli_addr.sin_addr));
   pthread_mutex_lock(&clifd_mutex);
   a=conn_fd;
   pthread_cond_signal(&clifd_cond);
   pthread_mutex_unlock(&clifd_mutex);
}
return 0;
}
void* testThreadPool(int *t) {
printf("t is %d\n", *t);
for (;;) {
   pthread_mutex_lock(&clifd_mutex);
   pthread_cond_wait(&clifd_cond, &clifd_mutex);
   printf("a is %d\n", a);
   printf("t is %d\n", *t);
   pthread_mutex_unlock(&clifd_mutex);
   sleep(100);
}
return (void*) 0;
}
了解 pthread_cond_wait() 的作用非常重要 -- 它是 POSIX 线程信号发送系统的核心，也是最难以理解的部分。
首先，让我们考虑以下情况：线程为查看已链接列表而锁定了互斥对象，然而该列表恰巧是空的。这一特定线程什么也干不了 -- 其设计意图是从列表中除去节点，但是现在却没有节点。因此，它只能：
锁定互斥对象时，线程将调用 pthread_cond_wait(&mycond,&mymutex)。pthread_cond_wait() 调用相当复杂，因此我们每次只执行它的一个操作。
pthread_cond_wait() 所做的**第一件事**就是同时对互斥对象解锁（于是其它线程可以修改已链接列表），并等待条件 mycond 发生（这样当 pthread_cond_wait() 接收到另一个线程的“信号”时，它将苏醒）。现在互斥对象已被解锁，其它线程可以访问和修改已链接列表，可能还会添加项。 【**要求解锁并阻塞是一个原子操作**】
此时，pthread_cond_wait() 调用还未返回。对互斥对象解锁会立即发生，但等待条件 mycond 通常是一个阻塞操作，这意味着线程将睡眠，在它苏醒之前不会消耗 CPU 周期。这正是我们期待发生的情况。线程将**一直睡眠，直到特定条件发生**，在这期间不会发生任何浪费 CPU 时间的繁忙查询。从线程的角度来看，它只是在等待 pthread_cond_wait() 调用返回。
现在继续说明，假设另一个线程（称作“2 号线程”）锁定了 mymutex 并对已链接列表添加了一项。在对互斥对象解锁之后，2 号线程会立即调用函数 pthread_cond_broadcast(&mycond)。此操作之后，2 号线程将使所有等待 mycond 条件变量的线程立即苏醒。这意味着第一个线程（仍处于 pthread_cond_wait() 调用中）现在将**苏醒**。
现在，看一下第一个线程发生了什么。您可能会认为在 2 号线程调用 pthread_cond_broadcast(&mymutex) 之后，1 号线程的 pthread_cond_wait() 会立即返回。不是那样！实际上，pthread_cond_wait() 将执行最后一个操作：**重新锁定 mymutex**。一旦 pthread_cond_wait() 锁定了互斥对象，那么它将返回并允许 1 号线程继续执行。那时，它可以马上检查列表，查看它所感兴趣的更改。
**停止并回顾！**
那个过程非常复杂，因此让我们先来回顾一下。第一个线程首先调用： 
pthread_mutex_lock(&mymutex); 
然后，它检查了列表。没有找到感兴趣的东西，于是它调用：
pthread_cond_wait(&mycond, &mymutex); 
然后，pthread_cond_wait() 调用在返回前执行许多操作： 
pthread_mutex_unlock(&mymutex); 
它对 mymutex 解锁，然后进入睡眠状态，等待 mycond 以接收 POSIX 线程“信号”。一旦接收到“信号”（加引号是因为我们并不是在讨论传统的 UNIX 信号，而是来自 pthread_cond_signal() 或 pthread_cond_broadcast() 调用的信号），它就会苏醒。但 pthread_cond_wait() 没有立即返回 -- 它还要做一件事：重新锁定 mutex：
pthread_mutex_lock(&mymutex); 
pthread_cond_wait() 知道我们在查找 mymutex “背后”的变化，因此它继续操作，为我们锁定互斥对象，然后才返回。
1.Linux“线程”
     进程与线程之间是有区别的，不过Linux内核只提供了轻量进程的支持，未实现线程模型。Linux是一种“多进程单线程”的操作系统。Linux本身只有进程的概念，而其所谓的“线程”本质上在内核里仍然是进程。
     大家知道，进程是资源分配的单位，同一进程中的多个线程共享该进程的资源（如作为共享内存的全局变量）。Linux中所谓的“线程”只是在被创建时clone了父进程的资源，因此clone出来的进程表现为“线程”，这一点一定要弄清楚。因此，Linux“线程”这个概念只有在打冒号的情况下才是最准确的。
     目前Linux中最流行的线程机制为LinuxThreads，所采用的就是线程－进程“一对一”模型，调度交给核心，而在用户级实现一个包括信号处理在内的线程管理机制。LinuxThreads由Xavier Leroy (Xavier.Leroy@inria.fr)负责开发完成，并已绑定在GLIBC中发行，它实现了一种BiCapitalized面向Linux的Posix 1003.1c “pthread”标准接口。Linuxthread可以支持Intel、Alpha、MIPS等平台上的多处理器系统。
　　按照POSIX 1003.1c 标准编写的程序与Linuxthread 库相链接即可支持Linux平台上的多线程，在程序中需包含头文件pthread. h，在编译链接时使用命令：
gcc -D -REENTRANT -lpthread xxx. c
　　其中-REENTRANT宏使得相关库函数(如stdio.h、errno.h中函数) 是可重入的、线程安全的(thread-safe)，-lpthread则意味着链接库目录下的libpthread.a或libpthread.so文件。使用Linuxthread库需要2.0以上版本的Linux内核及相应版本的C库(libc 5.2.18、libc 5.4.12、libc 6)。
**2.“线程”控制**
　　线程创建
　　进程被创建时，系统会为其创建一个主线程，而要在进程中创建新的线程，则可以调用pthread_create：
pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *
(start_routine)(void*), void *arg);
　　start_routine为新线程的入口函数，arg为传递给start_routine的参数。
　　每个线程都有自己的线程ID，以便在进程内区分。线程ID在pthread_create调用时回返给创建线程的调用者；一个线程也可以在创建后使用pthread_self()调用获取自己的线程ID：
pthread_self (void) ;
　　线程退出
线程的退出方式有三：
（1）执行完成后隐式退出；
（2）由线程本身显示调用pthread_exit 函数退出；
pthread_exit (void * retval) ;
（3）被其他线程用pthread_cance函数终止：
pthread_cance (pthread_t thread) ;
　　在某线程中调用此函数，可以终止由参数thread 指定的线程。
如果一个线程要等待另一个线程的终止，可以使用pthread_join函数，该函数的作用是调用pthread_join的线程将被挂起直到线程ID为参数thread的线程终止：
pthread_join (pthread_t thread, void** threadreturn);
**3.线程通信**　　线程互斥
　　互斥意味着“排它”，即两个线程不能同时进入被互斥保护的代码。Linux下可以通过pthread_mutex_t 定义互斥体机制完成多线程的互斥操作，该机制的作用是对某个需要互斥的部分，在进入时先得到互斥体，如果没有得到互斥体，表明互斥部分被其它线程拥有，此时欲获取互斥体的线程阻塞，直到拥有该互斥体的线程完成互斥部分的操作为止。
　　下面的代码实现了对共享全局变量x 用互斥体mutex 进行保护的目的：
int x; // 进程中的全局变量
pthread_mutex_t mutex;
pthread_mutex_init(&mutex, NULL); //按缺省的属性初始化互斥体变量mutex
pthread_mutex_lock(&mutex); // 给互斥体变量加锁
… //对变量x 的操作
phtread_mutex_unlock(&mutex); // 给互斥体变量解除锁
　　线程同步
　　同步就是线程等待某个事件的发生。只有当等待的事件发生线程才继续执行，否则线程挂起并放弃处理器。当多个线程协作时，相互作用的任务必须在一定的条件下同步。
Linux下的C语言编程有多种线程同步机制，最典型的是条件变量(condition variable)。pthread_cond_init用来创建一个条件变量，其函数原型为：
pthread_cond_init (pthread_cond_t *cond, const pthread_condattr_t *attr);
　　pthread_cond_wait和pthread_cond_timedwait用来等待条件变量被设置，值得注意的是这两个等待调用需要一个已经上锁的互斥体mutex，这是为了防止在真正进入等待状态之前别的线程有可能设置该条件变量而产生竞争。pthread_cond_wait的函数原型为：
pthread_cond_wait (pthread_cond_t *cond, pthread_mutex_t *mutex);
　　pthread_cond_broadcast用于设置条件变量，即使得事件发生，这样等待该事件的线程将不再阻塞：
pthread_cond_broadcast (pthread_cond_t *cond) ;
　　pthread_cond_signal则用于解除某一个等待线程的阻塞状态：
pthread_cond_signal (pthread_cond_t *cond) ;
　　pthread_cond_destroy 则用于释放一个条件变量的资源。
　　在头文件semaphore.h 中定义的信号量则完成了互斥体和条件变量的封装，按照多线程程序设计中访问控制机制，控制对资源的同步访问，提供程序设计人员更方便的调用接口。
sem_init(sem_t *sem, int pshared, unsigned int val);
　　这个函数初始化一个信号量sem 的值为val，参数pshared 是共享属性控制，表明是否在进程间共享。
sem_wait(sem_t *sem);
　　调用该函数时，若sem为无状态，调用线程阻塞，等待信号量sem值增加(post )成为有信号状态；若sem为有状态，调用线程顺序执行，但信号量的值减一。
sem_post(sem_t *sem);
　　调用该函数，信号量sem的值增加，可以从无信号状态变为有信号状态。
**4.实例**　　下面我们还是以名的生产者/消费者问题为例来阐述Linux线程的控制和通信。一组生产者线程与一组消费者线程通过缓冲区发生联系。生产者线程将生产的产品送入缓冲区，消费者线程则从中取出产品。缓冲区有N 个，是一个环形的缓冲池。
#include <stdio.h>
#include <pthread.h>
#define BUFFER_SIZE 16 // 缓冲区数量
struct prodcons
{
// 缓冲区相关数据结构
int buffer[BUFFER_SIZE]; /* 实际数据存放的数组*/
pthread_mutex_t lock; /* 互斥体lock 用于对缓冲区的互斥操作 */
int readpos, writepos; /* 读写指针*/
pthread_cond_t notempty; /* 缓冲区非空的条件变量 */
pthread_cond_t notfull; /* 缓冲区未满的条件变量 */
};
/* 初始化缓冲区结构 */
void init(struct prodcons *b)
{
pthread_mutex_init(&b->lock, NULL);
pthread_cond_init(&b->notempty, NULL);
pthread_cond_init(&b->notfull, NULL);
b->readpos = 0;
b->writepos = 0;
}
/* 将产品放入缓冲区,这里是存入一个整数*/
void put(struct prodcons *b, int data)
{
pthread_mutex_lock(&b->lock);
/* 等待缓冲区未满*/
if ((b->writepos + 1) % BUFFER_SIZE == b->readpos)
{
pthread_cond_wait(&b->notfull, &b->lock);
}
/* 写数据,并移动指针 */
b->buffer[b->writepos] = data;
b->writepos++;
if (b->writepos > = BUFFER_SIZE)
b->writepos = 0;
/* 设置缓冲区非空的条件变量*/
pthread_cond_signal(&b->notempty);
pthread_mutex_unlock(&b->lock);
} 
/* 从缓冲区中取出整数*/
int get(struct prodcons *b)
{
int data;
pthread_mutex_lock(&b->lock);
/* 等待缓冲区非空*/
if (b->writepos == b->readpos)
{
pthread_cond_wait(&b->notempty, &b->lock);
}
/* 读数据,移动读指针*/
data = b->buffer[b->readpos];
b->readpos++;
if (b->readpos > = BUFFER_SIZE)
b->readpos = 0;
/* 设置缓冲区未满的条件变量*/
pthread_cond_signal(&b->notfull);
pthread_mutex_unlock(&b->lock);
return data;
}
/* 测试:生产者线程将1 到10000 的整数送入缓冲区,消费者线
程从缓冲区中获取整数,两者都打印信息*/
#define OVER ( - 1)
struct prodcons buffer;
void *producer(void *data)
{
int n;
for (n = 0; n < 10000; n++)
{
printf("%d --->\n", n);
put(&buffer, n);
} put(&buffer, OVER);
return NULL;
}
void *consumer(void *data)
{
int d;
while (1)
{
d = get(&buffer);
if (d == OVER)
break;
printf("--->%d \n", d);
}
return NULL;
}
int main(void)
{
pthread_t th_a, th_b;
void *retval;
init(&buffer);
/* 创建生产者和消费者线程*/
pthread_create(&th_a, NULL, producer, 0);
pthread_create(&th_b, NULL, consumer, 0);
/* 等待两个线程结束*/
pthread_join(th_a, &retval);
pthread_join(th_b, &retval);
return 0;
}
5.WIN32、VxWorks、Linux线程类比
　　目前为止，笔者已经创作了《基于嵌入式操作系统VxWorks的多任务并发程序设计》（《软件报》2006年5~12期连载）、《深入浅出Win32多线程程序设计》（天极网技术专题）系列，我们来找出这两个系列文章与本文的共通点。
　　 看待技术问题要瞄准其本质，不管是Linux、VxWorks还是WIN32，其涉及到多线程的部分都是那些内容，无非就是线程控制和线程通信，它们的许多函数只是名称不同，其实质含义是等价的，下面我们来列个三大操作系统共同点详细表单：
|事项|WIN32|VxWorks|Linux|
|----|----|----|----|
|线程创建|CreateThread|taskSpawn|pthread_create|
|线程终止|执行完成后退出；线程自身调用ExitThread函数即终止自己；被其他线程调用函数TerminateThread函数|执行完成后退出；由线程本身调用exit退出；被其他线程调用函数taskDelete终止|执行完成后退出；由线程本身调用pthread_exit 退出；被其他线程调用函数pthread_cance终止|
|获取线程ID|GetCurrentThreadId|taskIdSelf|pthread_self|
|创建互斥|CreateMutex|semMCreate|pthread_mutex_init|
|获取互斥|WaitForSingleObject、WaitForMultipleObjects|semTake|pthread_mutex_lock|
|释放互斥|ReleaseMutex|semGive|phtread_mutex_unlock|
|创建信号量|CreateSemaphore|semBCreate、semCCreate|sem_init|
|等待信号量|WaitForSingleObject|semTake|sem_wait|
|释放信号量|ReleaseSemaphore|semGive|sem_post|
** 6.小结**
　　本章讲述了Linux下多线程的控制及线程间通信编程方法，给出了一个生产者/消费者的实例，并将Linux的多线程与WIN32、VxWorks多线程进行了类比，总结了一般规律。鉴于多线程编程已成为开发并发应用程序的主流方法，学好本章的意义也便不言自明。



