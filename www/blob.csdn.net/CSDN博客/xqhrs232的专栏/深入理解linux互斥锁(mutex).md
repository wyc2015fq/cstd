# 深入理解linux互斥锁(mutex) - xqhrs232的专栏 - CSDN博客
2017年03月31日 16:30:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：716
原文地址::[http://blog.csdn.net/cnbird2008/article/details/11640535](http://blog.csdn.net/cnbird2008/article/details/11640535)
相关文章
1、
# [linux中mutex和semaphore的区别](http://blog.csdn.net/panzhenjie/article/details/10083035)----[http://blog.csdn.net/panzhenjie/article/details/10083035](http://blog.csdn.net/panzhenjie/article/details/10083035)
**2、[DECLARE_MUTEX宏定义](http://blog.csdn.net/wang__rongwei/article/details/52832652)----[http://blog.csdn.net/wang__rongwei/article/details/52832652](http://blog.csdn.net/wang__rongwei/article/details/52832652)**
锁机制，可以说是[Linux](http://lib.csdn.net/base/linux)整个系统的精髓所在，linux内核都是围绕着同步在运转。在多进程和多线程编程中，锁起着极其重要的作用。我这里说的是互斥锁，其实是泛指linux中所有的锁机制。我在这里不讲如果创建锁，关于锁的创建，网上代码很多，我在这里就不多说了。我要谈一谈一个让所有刚刚接触锁机制的程序员都很困惑的问题：如何使用以及锁机制在程序中是如何运作的。
 一、定义：
/linux/include/linux/mutex.h 
47struct mutex {
  48        /* 1: unlocked, 0: locked, negative: locked, possible waiters */
  49        atomic_t                count;
  50        spinlock_t              wait_lock;
  51        struct list_head        wait_list;
  52#ifdef CONFIG_DEBUG_MUTEXES
  53        struct thread_info      *owner;
  54        const char              *name;
  55        void                    *magic;
  56#endif
  57#ifdef CONFIG_DEBUG_LOCK_ALLOC
  58        struct lockdep_map      dep_map;
  59#endif
  60};
二、作用及访问规则：
互斥锁主要用于实现内核中的互斥访问功能。内核互斥锁是在原子 API 之上实现的，但这对于内核用户是不可见的。对它的访问必须遵循一些规则：同一时间只能有一个任务持有互斥锁，而且只有这个任务可以对互斥锁进行解锁。互斥锁不能进行递归锁定或解锁。一个互斥锁对象必须通过其API初始化，而不能使用memset或复制初始化。一个任务在持有互斥锁的时候是不能结束的。互斥锁所使用的内存区域是不能被释放的。使用中的互斥锁是不能被重新初始化的。并且互斥锁不能用于中断上下文。但是互斥锁比当前的内核信号量选项更快，并且更加紧凑，因此如果它们满足您的需求，那么它们将是您明智的选择。
三、各字段详解：
1、atomic_t count;
指示互斥锁的状态：
1 没有上锁，可以获得
0 被锁定，不能获得
负数 被锁定，且可能在该锁上有等待进程
初始化为没有上锁。
2、spinlock_t wait_lock;
等待获取互斥锁中使用的自旋锁。在获取互斥锁的过程中，操作会在自旋锁的保护中进行。初始化为为锁定。
3、struct list_head wait_list;
等待互斥锁的进程队列。
四、操作：
1、定义并初始化： 
struct mutex mutex;
mutex_init(&mutex);
79# define mutex_init(mutex) \
80do { \
81 static struct lock_class_key __key; \
82 \
83 __mutex_init((mutex), #mutex, &__key); \
84} while (0)
42void
43__mutex_init(struct mutex *lock, const char *name, struct lock_class_key *key)
44{
45 atomic_set(&lock->count, 1);
46 spin_lock_init(&lock->wait_lock);
47 INIT_LIST_HEAD(&lock->wait_list);
48
49 debug_mutex_init(lock, name, key);
50}
直接定于互斥锁mutex并初始化为未锁定，己count为1，wait_lock为未上锁，等待队列wait_list为空。
2、获取互斥锁：
(1)具体参见linux/kernel/mutex.c
void inline fastcall __sched mutex_lock(struct mutex *lock);
获取互斥锁。实际上是先给count做自减操作，然后使用本身的自旋锁进入临界区操作。首先取得count的值，在将count置为－1，判断如果原来count的置为1，也即互斥锁可以获得，则直接获取，跳出。否则进入循环反复[测试](http://lib.csdn.net/base/softwaretest)互斥锁的状态。在循环中，也是先取得互斥锁原来的状态，在将其之为－1，判断如果可以获取(等于1)，则退出循环，否则设置当前进程的状态为不可中断状态，解锁自身的自旋锁，进入睡眠状态，待被在调度唤醒时，再获得自身的自旋锁，进入新一次的查询其自身状态(该互斥锁的状态)的循环。
(2)具体参见linux/kernel/mutex.c
int fastcall __sched mutex_lock_interruptible(struct mutex *lock)；
和mutex_lock()一样，也是获取互斥锁。在获得了互斥锁或进入睡眠直到获得互斥锁之后会返回0。如果在等待获取锁的时候进入睡眠状态收到一个信号(被信号打断睡眠)，则返回_EINIR。
(3)具体参见linux/kernel/mutex.c
int fastcall __sched mutex_trylock(struct mutex *lock);
试图获取互斥锁，如果成功获取则返回1，否则返回0，不等待。
3、释放互斥锁：
具体参见linux/kernel/mutex.c
void fastcall mutex_unlock(struct mutex *lock);
释放被当前进程获取的互斥锁。该函数不能用在中断上下文中，而且不允许去释放一个没有上锁的互斥锁。
五、使用形式： 
struct mutex mutex;
mutex_init(&mutex); /*定义*/
...
mutex_lock(&mutex); /*获取互斥锁*/
... /*临界资源*/
mutex_unlock(&mutex); /*释放互斥锁*/
**为什么要使用锁**
这个就比较简单，linux里面，锁的种类很多，包括互斥锁，文件锁，读写锁······其实信号量说白了也是一种锁。使用锁的目的是达到同步的作用，使共享资源在同一时间内，只有能有一个进程或者线程对他进行操作。
**linux是如何通过锁来实现对数据的保护和维护的**
这个问题是我要将的重点。很多刚刚接触锁机制的程序员，都会犯这种错误。比如，此时有2个线程，分别是线程A，线程B。A和B共享了资源M。为了同步A和B，使得同一时刻，同意时刻，只有一个线程对M操作。于是，很自然的会在A中对M资源先lock，等到A对M操作完毕之后，然后做一个操作unlock。B中则因为A加了锁，B就直接操作M。这个时候，你会发现，B同样可以操作到M。这个是为什么呢？
我们利索当然的把检测锁的任务交给了[操作系统](http://lib.csdn.net/base/operatingsystem)，交给了内核。可以翻看APUE上对于所的讲解，其中一部分是这么写的：
This mutual-exclusion mechanism works only if we design our threads to follow the same data-access rules. The operating system doesn't serialize access to data for us. If we allow one thread to access a shared resource without first acquiring a lock, then inconsistencies
 can occur even though the rest of our threads do acquire the lock before attempting to access the shared resource.
这里This mutual-exclusion mechanism指的就是锁机制。说的很清楚，只有程序员设计线程的时候，都遵循同一种数据访问规则，锁机制才会起作用。操作系统不会为我们序列化数据访问，也就是说，操作系统不会为我们拟定任何数据访问顺序，到底是A在先还是B在先，操作系统不会为我们规定。如果我们允许一个线程在没有多的锁(lock)之前，就对共享数据进行访问操作，那么，即使我们其他的线程都在访问之前试图去先锁住资源(获取锁)，同样会导致数据访问不一致，即多个线程同时在操作共享资源。
    从上面文字可以看出，操作系统不会为我们去检查，此时是不是有线程已经把资源锁住了。为了使锁能够正常工作，为了保护共享资源，我们只有在设计线程的时候，所有线程都用同一种方法去访问共享数据，也就是访问数据之前，务必先获取锁，然后再操作，操作完之后要解锁(unlock)。操作系统提供锁机制，就是提供了一种所有程序员都必须遵循的规范。而不是说我们锁住资源，其他线程访问共享资源的时候，让操作系统去为我们检查数据是否有其他的线程在操作。

