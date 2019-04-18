# 条件变量、pthread_cond_init - ljx0305的专栏 - CSDN博客
2011年08月16日 09:11:10[ljx0305](https://me.csdn.net/ljx0305)阅读数：1641标签：[signal																[测试																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)
个人分类：[Linux程序及使用																[Linux](https://blog.csdn.net/ljx0305/article/category/364133)](https://blog.csdn.net/ljx0305/article/category/394700)
## 1.初始化条件变量pthread_cond_init
#include <pthread.h>
int pthread_cond_init(pthread_cond_t *cv,
const pthread_condattr_t *cattr);
返回值：函数成功返回0；任何其他返回值都表示错误
初始化一个条件变量。当参数cattr为空指针时，函数创建的是一个缺省的条件变量。否则条件变量的属性将由cattr中的属性值来决定。调用 pthread_cond_init函数时，参数cattr为空指针等价于cattr中的属性为缺省属性，只是前者不需要cattr所占用的内存开销。这个函数返回时，条件变量被存放在参数cv指向的内存中。
可以用宏PTHREAD_COND_INITIALIZER来初始化静态定义的条件变量，使其具有缺省属性。这和用pthread_cond_init函数动态分配的效果是一样的。初始化时不进行错误检查。如：
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
不能由多个线程同时初始化一个条件变量。当需要重新初始化或释放一个条件变量时，应用程序必须保证这个条件变量未被使用。
## 2.阻塞在条件变量上pthread_cond_wait
#include <pthread.h>
int pthread_cond_wait(pthread_cond_t *cv,
pthread_mutex_t *mutex);
返回值：函数成功返回0；任何其他返回值都表示错误
函数将解锁mutex参数指向的互斥锁，并使当前线程阻塞在cv参数指向的条件变量上。
被阻塞的线程可以被pthread_cond_signal函数，pthread_cond_broadcast函数唤醒，也可能在被信号中断后被唤醒。
pthread_cond_wait函数的返回并不意味着条件的值一定发生了变化，必须重新检查条件的值。
pthread_cond_wait函数返回时，相应的互斥锁将被当前线程锁定，即使是函数出错返回。
一般一个条件表达式都是在一个互斥锁的保护下被检查。当条件表达式未被满足时，线程将仍然阻塞在这个条件变量上。当另一个线程改变了条件的值并向条件变量发出信号时，等待在这个条件变量上的一个线程或所有线程被唤醒，接着都试图再次占有相应的互斥锁。
阻塞在条件变量上的线程被唤醒以后，直到pthread_cond_wait()函数返回之前条件的值都有可能发生变化。所以函数返回以后，在锁定相应的互斥锁之前，必须重新测试条件值。最好的测试方法是循环调用pthread_cond_wait函数，并把满足条件的表达式置为循环的终止条件。如：
pthread_mutex_lock();
while (condition_is_false)
pthread_cond_wait();
pthread_mutex_unlock();
阻塞在同一个条件变量上的不同线程被释放的次序是不一定的。
注意：pthread_cond_wait()函数是退出点，如果在调用这个函数时，已有一个挂起的退出请求，且线程允许退出，这个线程将被终止并开始执行善后处理函数，而这时和条件变量相关的互斥锁仍将处在锁定状态。
## 3.解除在条件变量上的阻塞pthread_cond_signal
#include <pthread.h>
int pthread_cond_signal(pthread_cond_t *cv);
返回值：函数成功返回0；任何其他返回值都表示错误
函数被用来释放被阻塞在指定条件变量上的一个线程。
必须在互斥锁的保护下使用相应的条件变量。否则对条件变量的解锁有可能发生在锁定条件变量之前，从而造成死锁。
唤醒阻塞在条件变量上的所有线程的顺序由调度策略决定，如果线程的调度策略是SCHED_OTHER类型的，系统将根据线程的优先级唤醒线程。
如果没有线程被阻塞在条件变量上，那么调用pthread_cond_signal()将没有作用。
## 4.阻塞直到指定时间pthread_cond_timedwait
#include <pthread.h>
#include <time.h>
int pthread_cond_timedwait(pthread_cond_t *cv,
pthread_mutex_t *mp, const structtimespec * abstime);
返回值：函数成功返回0；任何其他返回值都表示错误
函数到了一定的时间，即使条件未发生也会解除阻塞。这个时间由参数abstime指定。函数返回时，相应的互斥锁往往是锁定的，即使是函数出错返回。
注意：pthread_cond_timedwait函数也是退出点。
超时时间参数是指一天中的某个时刻。使用举例：
pthread_timestruc_t to;
to.tv_sec = time(NULL) + TIMEOUT;
to.tv_nsec = 0;
超时返回的错误码是ETIMEDOUT。
## 5.释放阻塞的所有线程pthread_cond_broadcast
#include <pthread.h>
int pthread_cond_broadcast(pthread_cond_t *cv);
返回值：函数成功返回0；任何其他返回值都表示错误
函数唤醒所有被pthread_cond_wait函数阻塞在某个条件变量上的线程，参数cv被用来指定这个条件变量。当没有线程阻塞在这个条件变量上时，pthread_cond_broadcast函数无效。
由于pthread_cond_broadcast函数唤醒所有阻塞在某个条件变量上的线程，这些线程被唤醒后将再次竞争相应的互斥锁，所以必须小心使用pthread_cond_broadcast函数。
## 6.释放条件变量pthread_cond_destroy
#include <pthread.h>
int pthread_cond_destroy(pthread_cond_t *cv);
返回值：函数成功返回0；任何其他返回值都表示错误
释放条件变量。
注意：条件变量占用的空间并未被释放。
## 7.唤醒丢失问题
在线程未获得相应的互斥锁时调用pthread_cond_signal或pthread_cond_broadcast函数可能会引起唤醒丢失问题。
唤醒丢失往往会在下面的情况下发生：
- 一个线程调用pthread_cond_signal或pthread_cond_broadcast函数； 
- 另一个线程正处在测试条件变量和调用pthread_cond_wait函数之间； 
- 没有线程正在处在阻塞等待的状态下。
引用:[http://blog.csdn.net/ffilman/article/details/4871920](http://blog.csdn.net/ffilman/article/details/4871920)
