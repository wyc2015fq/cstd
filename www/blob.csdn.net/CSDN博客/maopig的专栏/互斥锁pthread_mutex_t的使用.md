# 互斥锁pthread_mutex_t的使用 - maopig的专栏 - CSDN博客
2011年07月20日 19:43:29[maopig](https://me.csdn.net/maopig)阅读数：924标签：[api																[linux																[文档																[null																[测试](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)
个人分类：[Linux](https://blog.csdn.net/maopig/article/category/850267)
1. 互斥锁创建
        有两种方法创建互斥锁，静态方式和动态方式。POSIX定义了一个宏PTHREAD_MUTEX_INITIALIZER来静态初始化互斥锁，方法如下：
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
              在LinuxThreads实现中，pthread_mutex_t是一个结构，而PTHREAD_MUTEX_INITIALIZER则是一个结构常量。
　　动态方式是采用pthread_mutex_init()函数来初始化互斥锁，API定义如下：
int 
pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr)
其中mutexattr用于指定互斥锁属性（见下），如果为NULL则使用缺省属性。
pthread_mutex_destroy ()用于注销一个互斥锁，API定义如下：
int pthread_mutex_destroy(pthread_mutex_t *mutex)
销毁一个互斥锁即意味着释放它所占用的资源，且要求锁当前处于开放状态。由于在Linux中，互斥锁并不占用任何资源，因此LinuxThreads中的 pthread_mutex_destroy()除了检查锁状态以外（锁定状态则返回EBUSY）没有其他动作。
　　2． 互斥锁属性
　　互斥锁的属性在创建锁的时候指定，在LinuxThreads实现中仅有一个锁类型属性，不同的锁类型在试图对一个已经被锁定的互斥锁加锁时表现不同。当前（glibc2.2.3,linuxthreads0.9）有四个值可供选择：
　　* PTHREAD_MUTEX_TIMED_NP，这是缺省值，也就是普通锁。当一个线程加锁以后，其余请求锁的线程将形成一个等待队列，并在解锁后按优先级获得锁。这种锁策略保证了资源分配的公平性。
　　* PTHREAD_MUTEX_RECURSIVE_NP，嵌套锁，允许同一个线程对同一个锁成功获得多次，并通过多次unlock解锁。如果是不同线程请求，则在加锁线程解锁时重新竞争。
　　* PTHREAD_MUTEX_ERRORCHECK_NP，检错锁，如果同一个线程请求同一个锁，则返回EDEADLK，否则与PTHREAD_MUTEX_TIMED_NP类型动作相同。这样就保证当不允许多次加锁时不会出现最简单情况下的死锁。
　　* PTHREAD_MUTEX_ADAPTIVE_NP，适应锁，动作最简单的锁类型，仅等待解锁后重新竞争。
　　3． 
锁操作
　　锁操作主要包括加锁pthread_mutex_lock()、解锁pthread_mutex_unlock()和测试加锁 pthread_mutex_trylock()三个，不论哪种类型的锁，都不可能被两个不同的线程同时得到，而必须等待解锁。对于普通锁和适应锁类型，解锁者可以是同进程内任何线程；而检错锁则必须由加锁者解锁才有效，否则返回EPERM；对于嵌套锁，文档和实现要求必须由加锁者解锁，但实验结果表明并没有这种限制，这个不同目前还没有得到解释。在同一进程中的线程，如果加锁后没有解锁，则任何其他线程都无法再获得锁。
　　int pthread_mutex_lock(pthread_mutex_t *mutex)
　　int pthread_mutex_unlock(pthread_mutex_t *mutex)
　　int pthread_mutex_trylock(pthread_mutex_t *mutex)
　　pthread_mutex_trylock()语义与pthread_mutex_lock()类似，不同的是在锁已经被占据时返回EBUSY而不是挂起等待。
