# pthread_cond_wait()用法分析 - h13 - 博客园
很久没看APUE，今天一位朋友问道关于一个mutex的问题，又翻到了以前讨论过的东西，为了不让自己忘记，把曾经的东西总结一下。
先大体看下网上很多地方都有的关于pthread_cond_wait()的说明：
条件变量   
  条件变量是利用线程间共享的全局变量进行同步的一种机制，主要包括两个动作：一个线程等待"条件变量的条件成立"而挂起；另一个线程使"条件成立"（给出条件成立信号）。为了防止竞争，条件变量的使用总是和一个互斥锁结合在一起。  
  1．   创建和注销   
  条件变量和互斥锁一样，都有静态动态两种创建方式，静态方式使用PTHREAD_COND_INITIALIZER常量，如下：     
  pthread_cond_t   cond=PTHREAD_COND_INITIALIZER     
  动态方式调用pthread_cond_init()函数，API定义如下：     
  int   pthread_cond_init(pthread_cond_t   *cond,   pthread_condattr_t   *cond_attr)    
  尽管POSIX标准中为条件变量定义了属性，但在LinuxThreads中没有实现，因此cond_attr值通常为NULL，且被忽略。   
  注销一个条件变量需要调用pthread_cond_destroy()，只有在没有线程在该条件变量上等待的时候才能注销这个条件变量，否则返回EBUSY。因为Linux实现的条件变量没有分配什么资源，所以注销动作只包括检查是否有等待线程。API定义如下：    
  int   pthread_cond_destroy(pthread_cond_t   *cond)     
  2．   等待和激发   
int   pthread_cond_wait(pthread_cond_t   *cond,   pthread_mutex_t   *mutex)   
  int   pthread_cond_timedwait(pthread_cond_t   *cond,   pthread_mutex_t   *mutex,   const   struct   timespec   *abstime)    
  等待条件有两种方式：无条件等待pthread_cond_wait()和计时等待pthread_cond_timedwait()，其中计时等待方式如果在给定时刻前条件没有满足，则返回ETIMEOUT，结束等待，其中abstime以与time()系统调用相同意义的绝对时间形式出现，0表示格林尼治时间1970年1月1日0时0分0秒。  
  无论哪种等待方式，都必须和一个互斥锁配合，以防止多个线程同时请求pthread_cond_wait()（或pthread_cond_timedwait()，下同）的竞争条件（Race   Condition）。mutex互斥锁必须是普通锁（PTHREAD_MUTEX_TIMED_NP）或者适应锁（PTHREAD_MUTEX_ADAPTIVE_NP），且在调用pthread_cond_wait()前必须由本线程加锁（pthread_mutex_lock()），而在更新条件等待队列以前，mutex保持锁定状态，并在线程挂起进入等待前解锁。在条件满足从而离开pthread_cond_wait()之前，mutex将被重新加锁，以与进入pthread_cond_wait()前的加锁动作对应。   
  激发条件有两种形式，pthread_cond_signal()激活一个等待该条件的线程，存在多个等待线程时按入队顺序激活其中一个；而pthread_cond_broadcast()则激活所有等待线程。  
现在来看一段典型的应用：看注释即可。
[view plain](http://blog.csdn.net/hairetz/article/details/4535920)
- #include <pthread.h>
- #include <unistd.h>
- 
- static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;  
- static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;  
- 
- struct node {  
- int n_number;  
- struct node *n_next;  
- } *head = NULL;  
- 
- /*[thread_func]*/
- staticvoid cleanup_handler(void *arg)  
- {  
-     printf("Cleanup handler of second thread./n");  
-     free(arg);  
-     (void)pthread_mutex_unlock(&mtx);  
- }  
- staticvoid *thread_func(void *arg)  
- {  
- struct node *p = NULL;  
- 
-     pthread_cleanup_push(cleanup_handler, p);  
- while (1) {  
-     pthread_mutex_lock(&mtx);           //这个mutex主要是用来保证pthread_cond_wait的并发性
- while (head == NULL)   {               //这个while要特别说明一下，单个pthread_cond_wait功能很完善，为何这里要有一个while (head == NULL)呢？因为pthread_cond_wait里的线程可能会被意外唤醒，如果这个时候head != NULL，则不是我们想要的情况。这个时候，应该让线程继续进入pthread_cond_wait
-         pthread_cond_wait(&cond, &mtx);         // pthread_cond_wait会先解除之前的pthread_mutex_lock锁定的mtx，然后阻塞在等待对列里休眠，直到再次被唤醒（大多数情况下是等待的条件成立而被唤醒，唤醒后，该进程会先锁定先pthread_mutex_lock(&mtx);，再读取资源
- //用这个流程是比较清楚的/*block-->unlock-->wait() return-->lock*/
-     }  
-         p = head;  
-         head = head->n_next;  
-         printf("Got %d from front of queue/n", p->n_number);  
-         free(p);  
-         pthread_mutex_unlock(&mtx);             //临界区数据操作完毕，释放互斥锁
-     }  
-     pthread_cleanup_pop(0);  
- return 0;  
- }  
- 
- int main(void)  
- {  
-     pthread_t tid;  
- int i;  
- struct node *p;  
-     pthread_create(&tid, NULL, thread_func, NULL);   //子线程会一直等待资源，类似生产者和消费者，但是这里的消费者可以是多个消费者，而不仅仅支持普通的单个消费者，这个模型虽然简单，但是很强大
- /*[tx6-main]*/
- for (i = 0; i < 10; i++) {  
-         p = malloc(sizeof(struct node));  
-         p->n_number = i;  
-         pthread_mutex_lock(&mtx);             //需要操作head这个临界资源，先加锁，
-         p->n_next = head;  
-         head = p;  
-         pthread_cond_signal(&cond);  
-         pthread_mutex_unlock(&mtx);           //解锁
-         sleep(1);  
-     }  
-     printf("thread 1 wanna end the line.So cancel thread 2./n");  
-     pthread_cancel(tid);             //关于pthread_cancel，有一点额外的说明，它是从外部终止子线程，子线程会在最近的取消点，退出线程，而在我们的代码里，最近的取消点肯定就是pthread_cond_wait()了。关于取消点的信息，有兴趣可以google,这里不多说了
-     pthread_join(tid, NULL);  
-     printf("All done -- exiting/n");  
- return 0;  
- }  
- 

