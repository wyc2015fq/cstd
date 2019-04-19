# Linux 线程同步的三种方法 - xqhrs232的专栏 - CSDN博客
2015年11月29日 22:22:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：337
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/zsf8701/article/details/7844316](http://blog.csdn.net/zsf8701/article/details/7844316)
相关文章
1、[Linux多线程同步机制](http://blog.csdn.net/h_armony/article/details/6766505)----[http://blog.csdn.net/h_armony/article/details/6766505](http://blog.csdn.net/h_armony/article/details/6766505)
2、[Linux多线程同步的几种方式](http://blog.chinaunix.net/uid-24919665-id-2689939.html) ----[http://blog.chinaunix.net/uid-24919665-id-2689939.html](http://blog.chinaunix.net/uid-24919665-id-2689939.html)
3、[再解Linux之线程同步](http://www.cnblogs.com/yc_sunniwell/archive/2010/06/21/1762056.html)----[http://www.cnblogs.com/yc_sunniwell/archive/2010/06/21/1762056.html](http://www.cnblogs.com/yc_sunniwell/archive/2010/06/21/1762056.html)
4、[Linux线程同步------屏障](http://m.blog.csdn.net/blog/xieshangjian/40861967#)----[http://m.blog.csdn.net/blog/xieshangjian/40861967](http://m.blog.csdn.net/blog/xieshangjian/40861967)
线程的最大特点是资源的共享性，但资源共享中的同步问题是多线程编程的难点。linux下提供了多种方式来处理线程同步，最常用的是互斥锁、条件变量和信号量。
### 一、互斥锁(mutex)
通过锁机制实现线程间的同步。
- 初始化锁。在Linux下，线程的互斥量数据类型是pthread_mutex_t。在使用前,要对它进行初始化。
静态分配：pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
动态分配：int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutex_attr_t *mutexattr);
- 加锁。对共享资源的访问，要对互斥量进行加锁，如果互斥量已经上了锁，调用线程会阻塞，直到互斥量被解锁。
int pthread_mutex_lock(pthread_mutex *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
- 解锁。在完成了对共享资源的访问后，要对互斥量进行解锁。
int pthread_mutex_unlock(pthread_mutex_t *mutex);
- 销毁锁。锁在是使用完成后，需要进行销毁以释放资源。
int pthread_mutex_destroy(pthread_mutex *mutex);
**[csharp]**[view
 plain](http://blog.csdn.net/zsf8701/article/details/7844316#)[copy](http://blog.csdn.net/zsf8701/article/details/7844316#)
- #include <cstdio>
- #include <cstdlib>
- #include <unistd.h>
- #include <pthread.h>
- #include "iostream"
- usingnamespace std;  
- pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  
- int tmp;  
- void* thread(void *arg)  
- {  
-     cout << "thread id is " << pthread_self() << endl;  
-     pthread_mutex_lock(&mutex);  
-     tmp = 12;  
-     cout << "Now a is " << tmp << endl;  
-     pthread_mutex_unlock(&mutex);  
- return NULL;  
- }  
- int main()  
- {  
-     pthread_t id;  
-     cout << "main thread id is " << pthread_self() << endl;  
-     tmp = 3;  
-     cout << "In main func tmp = " << tmp << endl;  
- if (!pthread_create(&id, NULL, thread, NULL))  
-     {  
-         cout << "Create thread success!" << endl;  
-     }  
- else
-     {  
-         cout << "Create thread failed!" << endl;  
-     }  
-     pthread_join(id, NULL);  
-     pthread_mutex_destroy(&mutex);  
- return 0;  
- }  
- //编译：g++ -o thread testthread.cpp -lpthread
### 二、条件变量(cond)
互斥锁不同，条件变量是用来等待而不是用来上锁的。条件变量用来自动阻塞一个线程，直到某特殊情况发生为止。通常条件变量和互斥锁同时使用。条件变量分为两部分: 条件和变量。条件本身是由互斥量保护的。线程在改变条件状态前先要锁住互斥量。条件变量使我们可以睡眠等待某种条件出现。条件变量是利用线程间共享的全局变量进行同步的一种机制，主要包括两个动作：一个线程等待"条件变量的条件成立"而挂起；另一个线程使"条件成立"（给出条件成立信号）。条件的检测是在互斥锁的保护下进行的。如果一个条件为假，一个线程自动阻塞，并释放等待状态改变的互斥锁。如果另一个线程改变了条件，它发信号给关联的条件变量，唤醒一个或多个等待它的线程，重新获得互斥锁，重新评价条件。如果两进程共享可读写的内存，条件变量可以被用来实现这两进程间的线程同步。
- 初始化条件变量。
静态态初始化，pthread_cond_t cond = PTHREAD_COND_INITIALIER;
动态初始化，int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr);
- 等待条件成立。释放锁,同时阻塞等待条件变量为真才行。timewait()设置等待时间,仍未signal,返回ETIMEOUT(加锁保证只有一个线程wait)
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_timewait(pthread_cond_t *cond,pthread_mutex *mutex,const timespec *abstime);
- 激活条件变量。pthread_cond_signal,pthread_cond_broadcast（激活所有等待线程）
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond); //解除所有线程的阻塞
- 清除条件变量。无线程等待,否则返回EBUSY
int pthread_cond_destroy(pthread_cond_t *cond);
**[cpp]**[view
 plain](http://blog.csdn.net/zsf8701/article/details/7844316#)[copy](http://blog.csdn.net/zsf8701/article/details/7844316#)
- #include <stdio.h>
- #include <pthread.h>
- #include "stdlib.h"
- #include "unistd.h"
- pthread_mutex_t mutex;  
- pthread_cond_t cond;  
- void hander(void *arg)  
- {  
-     free(arg);  
-     (void)pthread_mutex_unlock(&mutex);  
- }  
- void *thread1(void *arg)  
- {  
-     pthread_cleanup_push(hander, &mutex);  
- while(1)  
-     {  
-         printf("thread1 is running\n");  
-         pthread_mutex_lock(&mutex);  
-         pthread_cond_wait(&cond, &mutex);  
-         printf("thread1 applied the condition\n");  
-         pthread_mutex_unlock(&mutex);  
-         sleep(4);  
-     }  
-     pthread_cleanup_pop(0);  
- }  
- void *thread2(void *arg)  
- {  
- while(1)  
-     {  
-         printf("thread2 is running\n");  
-         pthread_mutex_lock(&mutex);  
-         pthread_cond_wait(&cond, &mutex);  
-         printf("thread2 applied the condition\n");  
-         pthread_mutex_unlock(&mutex);  
-         sleep(1);  
-     }  
- }  
- int main()  
- {  
-     pthread_t thid1,thid2;  
-     printf("condition variable study!\n");  
-     pthread_mutex_init(&mutex, NULL);  
-     pthread_cond_init(&cond, NULL);  
-     pthread_create(&thid1, NULL, thread1, NULL);  
-     pthread_create(&thid2, NULL, thread2, NULL);  
-     sleep(1);  
- do
-     {  
-         pthread_cond_signal(&cond);  
-     }while(1);  
-     sleep(20);  
-     pthread_exit(0);  
- return 0;  
- }  
**[cpp]**[view
 plain](http://blog.csdn.net/zsf8701/article/details/7844316#)[copy](http://blog.csdn.net/zsf8701/article/details/7844316#)
- #include <pthread.h>
- #include <unistd.h>
- #include "stdio.h"
- #include "stdlib.h"
- static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;  
- static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;  
- struct node  
- {  
- int n_number;  
- struct node *n_next;  
- }*head = NULL;  
- 
- staticvoid cleanup_handler(void *arg)  
- {  
-     printf("Cleanup handler of second thread./n");  
-     free(arg);  
-     (void)pthread_mutex_unlock(&mtx);  
- }  
- staticvoid *thread_func(void *arg)  
- {  
- struct node *p = NULL;  
-     pthread_cleanup_push(cleanup_handler, p);  
- while (1)  
-     {  
- //这个mutex主要是用来保证pthread_cond_wait的并发性
-         pthread_mutex_lock(&mtx);  
- while (head == NULL)  
-         {  
- //这个while要特别说明一下，单个pthread_cond_wait功能很完善，为何
- //这里要有一个while (head == NULL)呢？因为pthread_cond_wait里的线
- //程可能会被意外唤醒，如果这个时候head != NULL，则不是我们想要的情况。
- //这个时候，应该让线程继续进入pthread_cond_wait
- // pthread_cond_wait会先解除之前的pthread_mutex_lock锁定的mtx，
- //然后阻塞在等待对列里休眠，直到再次被唤醒(大多数情况下是等待的条件成立
- //而被唤醒，唤醒后，该进程会先锁定先pthread_mutex_lock(&mtx);，再读取资源
- //用这个流程是比较清楚的
-             pthread_cond_wait(&cond, &mtx);  
-             p = head;  
-             head = head->n_next;  
-             printf("Got %d from front of queue/n", p->n_number);  
-             free(p);  
-         }  
-         pthread_mutex_unlock(&mtx); //临界区数据操作完毕，释放互斥锁
-     }  
-     pthread_cleanup_pop(0);  
- return 0;  
- }  
- int main(void)  
- {  
-     pthread_t tid;  
- int i;  
- struct node *p;  
- //子线程会一直等待资源，类似生产者和消费者，但是这里的消费者可以是多个消费者，而
- //不仅仅支持普通的单个消费者，这个模型虽然简单，但是很强大
-     pthread_create(&tid, NULL, thread_func, NULL);  
-     sleep(1);  
- for (i = 0; i < 10; i++)  
-     {  
-         p = (struct node*)malloc(sizeof(struct node));  
-         p->n_number = i;  
-         pthread_mutex_lock(&mtx); //需要操作head这个临界资源，先加锁，
-         p->n_next = head;  
-         head = p;  
-         pthread_cond_signal(&cond);  
-         pthread_mutex_unlock(&mtx); //解锁
-         sleep(1);  
-     }  
-     printf("thread 1 wanna end the line.So cancel thread 2./n");  
- //关于pthread_cancel，有一点额外的说明，它是从外部终止子线程，子线程会在最近的取消点，退出
- //线程，而在我们的代码里，最近的取消点肯定就是pthread_cond_wait()了。
-     pthread_cancel(tid);  
-     pthread_join(tid, NULL);  
-     printf("All done -- exiting/n");  
- return 0;  
- }  
### 三、信号量(sem)
如同进程一样，线程也可以通过信号量来实现通信，虽然是轻量级的。信号量函数的名字都以"sem_"打头。线程使用的基本信号量函数有四个。
- 信号量初始化。
int sem_init (sem_t *sem , int pshared, unsigned int value);
这是对由sem指定的信号量进行初始化，设置好它的共享选项(linux 只支持为0，即表示它是当前进程的局部信号量)，然后给它一个初始值VALUE。
- 等待信号量。给信号量减1，然后等待直到信号量的值大于0。
int sem_wait(sem_t *sem);
- 释放信号量。信号量值加1。并通知其他等待线程。
int sem_post(sem_t *sem);
- 销毁信号量。我们用完信号量后都它进行清理。归还占有的一切资源。
int sem_destroy(sem_t *sem);
**[cpp]**[view
 plain](http://blog.csdn.net/zsf8701/article/details/7844316#)[copy](http://blog.csdn.net/zsf8701/article/details/7844316#)
- #include <stdlib.h>
- #include <stdio.h>
- #include <unistd.h>
- #include <pthread.h>
- #include <semaphore.h>
- #include <errno.h>
- #define return_if_fail(p) if((p) == 0){printf ("[%s]:func error!/n", __func__);return;}
- typedefstruct _PrivInfo  
- {  
-     sem_t s1;  
-     sem_t s2;  
- time_t end_time;  
- }PrivInfo;  
- 
- staticvoid info_init (PrivInfo* thiz);  
- staticvoid info_destroy (PrivInfo* thiz);  
- staticvoid* pthread_func_1 (PrivInfo* thiz);  
- staticvoid* pthread_func_2 (PrivInfo* thiz);  
- 
- int main (int argc, char** argv)  
- {  
-     pthread_t pt_1 = 0;  
-     pthread_t pt_2 = 0;  
- int ret = 0;  
-     PrivInfo* thiz = NULL;  
-     thiz = (PrivInfo* )malloc (sizeof (PrivInfo));  
- if (thiz == NULL)  
-     {  
-         printf ("[%s]: Failed to malloc priv./n");  
- return -1;  
-     }  
-     info_init (thiz);  
-     ret = pthread_create (&pt_1, NULL, (void*)pthread_func_1, thiz);  
- if (ret != 0)  
-     {  
-         perror ("pthread_1_create:");  
-     }  
-     ret = pthread_create (&pt_2, NULL, (void*)pthread_func_2, thiz);  
- if (ret != 0)  
-     {  
-         perror ("pthread_2_create:");  
-     }  
-     pthread_join (pt_1, NULL);  
-     pthread_join (pt_2, NULL);  
-     info_destroy (thiz);  
- return 0;  
- }  
- staticvoid info_init (PrivInfo* thiz)  
- {  
-     return_if_fail (thiz != NULL);  
-     thiz->end_time = time(NULL) + 10;  
-     sem_init (&thiz->s1, 0, 1);  
-     sem_init (&thiz->s2, 0, 0);  
- return;  
- }  
- staticvoid info_destroy (PrivInfo* thiz)  
- {  
-     return_if_fail (thiz != NULL);  
-     sem_destroy (&thiz->s1);  
-     sem_destroy (&thiz->s2);  
-     free (thiz);  
-     thiz = NULL;  
- return;  
- }  
- staticvoid* pthread_func_1 (PrivInfo* thiz)  
- {  
-     return_if_fail(thiz != NULL);  
- while (time(NULL) < thiz->end_time)  
-     {  
-         sem_wait (&thiz->s2);  
-         printf ("pthread1: pthread1 get the lock./n");  
-         sem_post (&thiz->s1);  
-         printf ("pthread1: pthread1 unlock/n");  
-         sleep (1);  
-     }  
- return;  
- }  
- staticvoid* pthread_func_2 (PrivInfo* thiz)  
- {  
-     return_if_fail (thiz != NULL);  
- while (time (NULL) < thiz->end_time)  
-     {  
-         sem_wait (&thiz->s1);  
-         printf ("pthread2: pthread2 get the unlock./n");  
-         sem_post (&thiz->s2);  
-         printf ("pthread2: pthread2 unlock./n");  
-         sleep (1);  
-     }  
- return;  
- }  
