# pthread_cond_wait - maopig的专栏 - CSDN博客
2012年07月18日 22:57:09[maopig](https://me.csdn.net/maopig)阅读数：3881
**1. 首先pthread_cond_wait 的定义是这样的**
*The pthread_cond_wait()* and*pthread_cond_timedwait()* functions are used to block on a condition variable. They are called with*mutex* locked by the calling thread or undefined
 behaviour will result. 
These functions atomically release*mutex* and cause the calling thread to block on the condition variable*cond* ; atomically here means "atomically with respect to access by another
 thread to the mutex and then the condition variable". That is, if another thread is able to acquire the mutex after the about-to-block thread has released it, then a subsequent call to*[pthread_cond_signal()](http://www.opengroup.org/on%3Cwbr%3Elinepubs/007908775/xsh/pthread_cond_signal.html)*or*[pthread_cond_broadcast()](http://www.opengroup.org/on%3Cwbr%3Elinepubs/007908775/xsh/pthread_cond_broadcast.html)*in
 that thread behaves as if it were issued after the about-to-block thread has blocked.
**2. 由上解释可以看出,pthread_cond_wait() 必须与pthread_mutex 配套使用。**
pthread_cond_wait()函数一进入wait状态就会自动release mutex.
In Thread1:
pthread_mutex_lock(&m_mutex);   
pthread_cond_wait(&m_cond,&m_mutex);   
pthread_mutex_unlock(&m_mutex);   
In Thread2:
pthread_mutex_lock(&m_mutex);   
pthread_cond_signal(&m_cond);   
pthread_mutex_unlock(&m_mutex);   
为什么要与pthread_mutex 一起使用呢？这是为了应对线程1在调用pthread_cond_wait()但线程1还没有进入wait cond的状态的时候，此时线程2调用了 cond_singal 的情况。 如果不用mutex锁的话，这个cond_singal就丢失了。加了锁的情况是，线程2必须等到 mutex 被释放（也就是 pthread_cod_wait() 进入wait_cond状态
 并自动释放mutex） 的时候才能调用cond_singal.
**3. pthread_cond_wait() 一旦wait成功获得cond 条件的时候会自动 lock mutex.**
这就会出现另一个问题。这是因为
*The pthread_cond_wait()* and*pthread_cond_timedwait()* is a cancellation point.
In Thread3:
pthread_cancel(&m_thread);
pthread_join();
因为*pthread_cond_wait()* and*pthread_cond_timedwait()*    是线程退出点函数，因此在Thread3中
可以调用pthread_cancel()来退出线程1。那样显然线程1会在 pthread_cond_wait(&m_cond,&m_mutex);    和 pthread_mutex_unlock(&m_mutex); 之间退出，    pthread_cond_wait() 函数返回后自动lock住了mutex, 这个时候线程1退出（并没有运行到pthread_mutex_unlock（）），如果Thread2这个时候就再也得不到lock状态了。
通常解决这个问题的办法如下
void cleanup(void *arg)
{
   pthread_mutex_unlock(&mutex);
}
void * thread1(void * arg)
{
    pthread_cleanup_push(cleanup, NULL); 
// thread cleanup handler
    pthread_mutex_lock(&mutex);
   pthread_cond_wait(&cond, &mutex);
   pthread_mutex_unlock(&mutex);
   pthread_cleanup_pop(0 );
}

LINUX环境下多线程编程肯定会遇到需要条件变量的情况，此时必然要使用pthread_cond_wait()函数。但这个函数的执行过程比较难于理解。
    pthread_cond_wait()的工作流程如下（以MAN中的EXAMPLE为例）：
       Consider two shared variables x and y, protected by the mutex mut, and a condition vari-
       able cond that is to be signaled whenever x becomes greater than y.
              int x,y;
              pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
              pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
       Waiting until x is greater than y is performed as follows:
              pthread_mutex_lock(&mut);
              while (x <= y) {
                      pthread_cond_wait(&cond, &mut);
              }
              /* operate on x and y */
              pthread_mutex_unlock(&mut);
       Modifications on x and y that may cause x to become greater than y should signal the con-
       dition if needed:
              pthread_mutex_lock(&mut);
              /* modify x and y */
              if (x > y) pthread_cond_broadcast(&cond);
              pthread_mutex_unlock(&mut);
这个例子的意思是，两个线程要修改X和 Y的值，第一个线程当X<=Y时就挂起，直到X>Y时才继续执行（由第二个线程可能会修改X,Y的值，当X>Y时唤醒第一个线程），即首先初始化一个普通互斥量mut和一个条件变量cond。之后分别在两个线程中分别执行如下函数体：
pthread_mutex_lock(&mut);
              while (x <= y) {
                      pthread_cond_wait(&cond, &mut);
              }
              /* operate on x and y */
              pthread_mutex_unlock(&mut);
和：       pthread_mutex_lock(&mut);
              /* modify x and y */
              if (x > y) pthread_cond_signal(&cond);
              pthread_mutex_unlock(&mut);
    其实函数的执行过程非常简单，在第一个线程执行到pthread_cond_wait(&cond,&mut)时，此时如果X<=Y，则此函数就将mut互斥量解锁 ，再将cond条件变量加锁 ，此时第一个线程挂起 （不占用任何CPU周期）。
    而在第二个线程中，本来因为mut被第一个线程锁住而阻塞，此时因为mut已经释放，所以可以获得锁mut，并且进行修改X和Y的值，在修改之后，一个IF语句判定是不是X>Y，如果是，则此时pthread_cond_signal()函数会唤醒第一个线程 ，并在下一句中释放互斥量mut。然后第一个线程开始从pthread_cond_wait()执行，首先要再次**锁mut**，
 如果锁成功，再进行**条件的判断**（至于为什么用WHILE，即在被唤醒之后还要再判断，后面有原因分析），如果满足条件，则**被唤醒**进行处理，最后释放互斥量mut 。
    至于为什么在被唤醒之后还要再次进行条件判断（即为什么要使用while循环来判断条件），是因为可能有“惊群效应”。有人觉得此处既然是被唤醒的，肯定是满足条件了，其实不然。如果是多个线程都在等待这个条件，而同时只能有一个线程进行处理，此时就必须要再次条件判断，以使只有一个线程进入临界区处理。对此，转来一段：
引用下POSIX的RATIONALE：
Condition Wait Semantics 
It is important to note that when pthread_cond_wait() and pthread_cond_timedwait() return without error, the associated predicate may still be false. Similarly, when pthread_cond_timedwait() returns with the timeout error, the associated predicate may be true
 due to an unavoidable race between the expiration of the timeout and the predicate state change.
The application needs to recheck the predicate on any return because it cannot be sure there is another thread waiting on the thread to handle the signal, and if there is not then the signal is lost. The burden is on the application to check the predicate.
Some implementations, particularly on a multi-processor, may sometimes cause multiple threads to wake up when the condition variable is signaled simultaneously on different processors.
In general, whenever a condition wait returns, the thread has to re-evaluate the predicate associated with the condition wait to determine whether it can safely proceed, should wait again, or should declare a timeout. A return from the wait does not imply that
 the associated predicate is either true or false. 
It is thus recommended that a condition wait be enclosed in the equivalent of a "while loop" that checks the predicate.
**从上文可以看出：**
1，pthread_cond_signal在多处理器上可能同时唤醒多个线程，当你只能让一个线程处理某个任务时，其它被唤醒的线程就需要继续 wait,while循环的意义就体现在这里了，而且规范要求pthread_cond_signal至少唤醒一个pthread_cond_wait上的线程，其实有些实现为了简单在单处理器上也会唤醒多个线程.
2，某些应用，如线程池，pthread_cond_broadcast唤醒全部线程，但我们通常只需要一部分线程去做执行任务，所以其它的线程需要继续wait.所以强烈推荐此处使用while循环.
       其实说白了很简单，就是pthread_cond_signal()也可能唤醒多个线程，而如果你同时只允许一个线程访问的话，就必须要使用while来进行条件判断，以保证临界区内只有一个线程在处理。
浅析pthread_cond_wait
条件变量是利用线程间共享的全局变量进行同步的一种机制，主要包括两个动作：一个线程等待"条件变量的条件成立"而挂起；另一个线程使"条件成立"（给出条件成立信号）。为了防止竞争，条件变量的使用总是和一个互斥锁结合在一起。
一 pthread_cond_wait定义： 
函数原型：int   pthread_cond_wait(pthread_cond_t   *cond,   pthread_mutex_t   *mutex)  
参数: cond 条件变量  mutex 互斥锁 
第一个参数*cond是指向一个条件变量的指针。第二个参数*mutex则是对相关的互斥锁的指针。 
二 pthread_cond_wait示例理解 
pthread_cond_wait的机制比较难里理解，是条件变量中重要的成分。条件变量用于线程间同步，那么pthread_cond_wait必须和互斥锁同时作用在一个线程里，它同时起到对资源的加锁和解锁，看下面的示例：
程序创建了2个新线程使他们同步运行，实现进程t_b打印9以内3的倍数，t_a打印其他的数，程序开始线程t_b不满足条件等待，线程t_a运行使a循环加1并打印。直到i为3的倍数时，线程t_a发送信号通知进程t_b，这时t_b满足条件，打印i值。
C++代码 ![复制代码](http://weihe6666.iteye.com/images/icon_copy.gif)![收藏代码](http://weihe6666.iteye.com/images/icon_star.png)![](http://weihe6666.iteye.com/images/spinner.gif)
- 1 #include<pthread.h>   
-   2 #include<unistd.h>   
-   3 #include<stdio.h>   
-   4 #include<stdlib.h>   
-   5    
-   6 pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;/*初始化互斥锁*/
-   7 pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;//init cond
-   8    
-   9 void *thread1(void*);   
-  10 void *thread2(void*);   
-  11    
-  12 **int** i = 1; //global
-  13    
-  14 **int** main(void){   
-  15     pthread_t t_a;   
-  16     pthread_t t_b;//two thread 
-  17    
-  18     pthread_create(&t_a,NULL,thread2,(void*)NULL);   
-  19     pthread_create(&t_b,NULL,thread1,(void*)NULL);//Create thread
-  20    
-  21     pthread_join(t_b,NULL);//wait a_b thread end
-  22     pthread_mutex_destroy(&mutex);   
-  23     pthread_cond_destroy(&cond);   
- 24     exit(0);   
-  25 }   
-  26    
-  27 void *thread1(void *junk){   
-  28     for(i = 1;i<= 9; i++){   
-  29         pthread_mutex_lock(&mutex); //互斥锁
-  30         printf("call thread1 \n");   
-  31         if(i%3 == 0)   
-  32             pthread_cond_signal(&cond); //send sianal to t_b
-  33         else
-  34             printf("thread1: %d\n",i);   
-  35         pthread_mutex_unlock(&mutex);   
-  36         sleep(1);   
-  37     }   
-  38 }   
-  39    
-  40 void *thread2(void*junk){   
-  41     while(i < 9)   
-  42     {   
-  43         pthread_mutex_lock(&mutex);   
-  44         printf("call thread2 \n");   
-  45         if(i%3 != 0)   
-  46             pthread_cond_wait(&cond,&mutex); //wait
-   47         printf("thread2: %d\n",i);   
-  48         pthread_mutex_unlock(&mutex);   
-  49         sleep(1);   
-  50     }   
-  51 }                                      
1 #include<pthread.h>
  2 #include<unistd.h>
  3 #include<stdio.h>
  4 #include<stdlib.h>
  5 
  6 pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;/*初始化互斥锁*/
  7 pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;//init cond
  8 
  9 void *thread1(void*);
 10 void *thread2(void*);
 11 
 12 int i = 1; //global
 13 
 14 int main(void){
 15     pthread_t t_a;
 16     pthread_t t_b;//two thread
 17 
 18     pthread_create(&t_a,NULL,thread2,(void*)NULL);
 19     pthread_create(&t_b,NULL,thread1,(void*)NULL);//Create thread
 20 
 21     pthread_join(t_b,NULL);//wait a_b thread end
 22     pthread_mutex_destroy(&mutex);
 23     pthread_cond_destroy(&cond);
24     exit(0);
 25 }
 26 
 27 void *thread1(void *junk){
 28     for(i = 1;i<= 9; i++){
 29         pthread_mutex_lock(&mutex); //互斥锁
 30         printf("call thread1 \n");
 31         if(i%3 == 0)
 32             pthread_cond_signal(&cond); //send sianal to t_b
 33         else
 34             printf("thread1: %d\n",i);
 35         pthread_mutex_unlock(&mutex);
 36         sleep(1);
 37     }
 38 }
 39 
 40 void *thread2(void*junk){
 41     while(i < 9)
 42     {
 43         pthread_mutex_lock(&mutex);
 44         printf("call thread2 \n");
 45         if(i%3 != 0)
 46             pthread_cond_wait(&cond,&mutex); //wait
  47         printf("thread2: %d\n",i);
 48         pthread_mutex_unlock(&mutex);
 49         sleep(1);
 50     }
 51 }                                    
输出： 
call thread2 
call thread1 
thread1: 1 
call thread1 
thread1: 2 
call thread1 
thread2: 3 
call thread1 
thread1: 4 
call thread2 
call thread1 
thread1: 5 
call thread1 
thread2: 6 
call thread1 
thread1: 7 
call thread2 
call thread1 
thread1: 8 
call thread1 
thread2: 9 
示例的解释： 
call thread2：是线程2即t_b首先上锁，即 pthread_mutex_lock(&mutex);锁住了mutex使得此进程执行线程2中的临界区的代码，当执行到45行：if(i%3 != 0)，此时i=1,满足此条件，则执行46行： pthread_cond_wait(&cond,&mutex); 这句是关键，pthread_cond_wait(&cond,&mutex)操作有两步，是原子操作：第一 解锁，先解除之前的pthread_mutex_lock锁定的mutex；第二 挂起，阻塞并在等待对列里休眠，即线程2挂起，直到再次被唤醒，唤醒的条件是由pthread_cond_signal(&cond);发出的cond信号来唤醒。
call thread1:由于pthread_cond_wait已经对线程2解锁，此时另外的线程只有线程1，那么线程1对mutex上锁，若这时有多个线程，那么线程间上锁的顺序和操作系统有关。
thread1: 1：线程1上锁后执行临界区的代码，当执行到if(i%3 == 0)此时i=1,不满足条件，则pthread_cond_signal(&cond);不被执行，那么线程2仍处于挂起状态，输出thread1: 1后线程1由pthread_mutex_unlock(&mutex);解锁。
thread1: 2：这时此进程中只有2个线程，线程2处于挂起状态，那么只有线程1,则线程1又对mutex上锁，此时同样执行临界区的代码，而且i=2,不满足条件，pthread_cond_signal(&cond);不被执行，那么线程2仍处于挂起状态，输出thread1: 1后线程1由pthread_mutex_unlock(&mutex);解锁。
call thread1：同样由线程1上锁，但此时i=3,满足条件pthread_cond_signal(&cond)被执行，那么pthread_cond_signal(&cond)会发出信号，来唤醒处于挂起的线程2。pthread_cond_signal同样由两个原子操作：1,解锁;2,发送信号;解锁即对线程1解锁，解除对mutex的上锁。发送信号，即给等待signal挂起的线程2发送信号，唤醒挂起的线程2。
thread2: 3：由于pthread_cond_signal唤醒了线程2,即i=3满足条件，pthread_cond_wait(&cond,&mutex);被执行，那么pthread_cond_wait(&cond,&mutex)此时也有一步操作：上锁;即对线程2上锁，此时的pthread_cond_wait(&cond,&mutex)的操作相当与pthread_mutex_lock(&mutex);那么线程2继续执行上锁后的临界区的代码，并由pthread_mutex_unlock(&mutex);对线程2进行解锁。
剩下的输出原理和上面解释的一样。 
纵观pthread_cond_wait，它的理解不可之把它看作一个简单的wait函数，它里面应该是一族函数，不同的函数在不同的条件下执行，理解pthread_cond_wait的机制可以很好的学习条件变量
假如条件变量上有多个线程在等待,pthread_cond_broadcast会唤醒所有的等待线程，而pthread_cond_signal只会唤醒其中一个等待线程
**一、什么是条件变量**
与互斥锁不同，条件变量是用来等待而不是用来上锁的。条件变量用来自动阻塞一个线程，直到某特殊情况发生为止。通常条件变量和互斥锁同时使用。
条件变量使我们可以睡眠等待某种条件出现。条件变量是利用线程间共享的全局变量进行同步的一种机制，主要包括两个动作：一个线程等待"条件变量的条件成立"而挂起；另一个线程使"条件成立"（给出条件成立信号）。
条件的检测是在互斥锁的保护下进行的。如果一个条件为假，一个线程自动阻塞，并释放等待状态改变的互斥锁。如果另一个线程改变了条件，它发信号给关联的条件变量，唤醒一个或多个等待它的线程，重新获得互斥锁，重新评价条件。如果两进程共享可读写的内存，条件变量可以被用来实现这两进程间的线程同步。
使用条件变量之前要先进行初始化。可以在单个语句中生成和初始化一个条件变量如：
pthread_cond_t my_condition=PTHREAD_COND_INITIALIZER;（用于进程间线程的通信）。
也可以利用函数pthread_cond_init动态初始化。
**二、条件变量函数**
1．
|名称：|pthread_cond_init|
|----|----|
|目标：|条件变量初始化|
|头文件：|#include < pthread.h>|
|函数原形：|int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);|
|参数:|cptr条件变量attr条件变量属性|
|返回值：|成功返回0，出错返回错误编号。|

pthread_cond_init函数可以用来初始化一个条件变量。他使用变量attr所指定的属性来初始化一个条件变量，如果参数attr为空，那么它将使用缺省的属性来设置所指定的条件变量。
2.
|名称：|pthread_cond_destroy|
|----|----|
|目标：|条件变量摧毁|
|头文件：|#include < pthread.h>|
|函数原形：|int pthread_cond_destroy(pthread_cond_t *cond);|
|参数:|cptr条件变量|
|返回值：|成功返回0，出错返回错误编号。|
pthread_cond_destroy函数可以用来摧毁所指定的条件变量，同时将会释放所给它分配的资源。调用该函数的进程也并不要求等待在参数所指定的条件变量上。
3.
|名称：|pthread_cond_wait/pthread_cond_timedwait|
|----|----|
|目标：|条件变量等待|
|头文件：|#include < pthread.h>|
|函数原形：|int pthread_cond_wait(pthread_cond_t *cond,pthread_mutex_t *mutex);int pthread_cond_timedwait(pthread_cond_t *cond,pthread_mutex_t mytex,const struct timespec *abstime);|
|参数:|cond条件变量mutex互斥锁|
|返回值：|成功返回0，出错返回错误编号。|

第一个参数*cond是指向一个条件变量的指针。第二个参数*mutex则是对相关的互斥锁的指针。函数pthread_cond_timedwait函数类型与函数pthread_cond_wait,区别在于，如果达到或是超过所引用的参数*abstime,它将结束并返回错误ETIME.pthread_cond_timedwait函数的参数*abstime指向一个timespec结构。该结构如下：
typedef struct timespec{
time_t tv_sec;
long tv_nsex;
}timespec_t;
3.
|名称：|pthread_cond_signal/pthread_cond_broadcast|
|----|----|
|目标：|条件变量通知|
|头文件：|#include < pthread.h>|
|函数原形：|int pthread_cond_signal(pthread_cond_t *cond);int pthread_cond_broadcast(pthread_cond_t *cond);|
|参数:|cond条件变量|
|返回值：|成功返回0，出错返回错误编号。|

参数*cond是对类型为pthread_cond_t
的一个条件变量的指针。当调用pthread_cond_signal时一个在相同条件变量上阻塞的线程将被解锁。如果同时有多个线程阻塞，则由调度策略确定接收通知的线程。如果调用pthread_cond_broadcast,则将通知阻塞在这个条件变量上的所有线程。一旦被唤醒，线程仍然会要求互斥锁。如果当前没有线程等待通知，则上面两种调用实际上成为一个空操作。如果参数*cond指向非法地址，则返回值EINVAL。
下面是一个简单的例子，我们可以从程序的运行来了解条件变量的作用。
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;/*初始化互斥锁*/
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;/*初始化条件变量*/
void *thread1(void *);
void *thread2(void *);
int i=1;
int main(void)
{
pthread_t t_a;
pthread_t t_b;
pthread_create(&t_a,NULL,thread2,(void *)NULL);/*创建进程t_a*/
pthread_create(&t_b,NULL,thread1,(void *)NULL); /*创建进程t_b*/
pthread_join(t_b, NULL);/*等待进程t_b结束*/
pthread_mutex_destroy(&mutex);
pthread_cond_destroy(&cond);
exit(0);
}
void *thread1(void *junk)
{
for(i=1;i<=9;i++) 
{
pthread_mutex_lock(&mutex);/*锁住互斥量*/
if(i%3==0)
pthread_cond_signal(&cond);/*条件改变，发送信号，通知t_b进程*/
else        
printf("thead1:%d\n",i);
pthread_mutex_unlock(&mutex);/*解锁互斥量*/
sleep(1);
}
}
void *thread2(void *junk)
{
while(i<9)
{
pthread_mutex_lock(&mutex);
if(i%3!=0)
pthread_cond_wait(&cond,&mutex);/*等待*/
printf("thread2:%d\n",i);
pthread_mutex_unlock(&mutex);
sleep(1);
}
}
程序创建了2个新线程使他们同步运行，实现进程t_b打印20以内3的倍数，t_a打印其他的数，程序开始线程t_b不满足条件等待，线程t_a运行使a循环加1并打印。直到i为3的倍数时，线程t_a发送信号通知进程t_b，这时t_b满足条件，打印i值。
下面是运行结果：
#cc –lpthread –o cond cond.c
#./cond
thread1:1
thread1:2
thread2:3
thread1:4
thread1:5
thread2:6
thread1:7
thread1:8
thread2:9
