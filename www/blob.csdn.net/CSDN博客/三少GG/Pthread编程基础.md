# Pthread编程基础 - 三少GG - CSDN博客
2011年11月29日 19:25:05[三少GG](https://me.csdn.net/scut1135)阅读数：1617
=============================================================
int pthread_create(
              pthread_t *tid,
              const pthread_attr_t *attr,
              void*(*start_routine)(void*),
              void *arg
              );
//参数tid 用于返回新创建线程的线程号；
//start_routine 是线程函数指针，线程从这个函数开始独立地运行；
//arg 是传递给线程函数的参数。由于start_routine是一个指向参数类型为void*，返回值为void*的指针，所以如果需要传递或返回多个参数时，可以使用强制类型转化。
=============================================================
void pthread_exit(
             void*value_ptr
             );
// 参数value_ptr 是一个指向返回状态值的指针。
=============================================================
int pthread_join(
            pthread_t tid ,
             void**status
             );
// 参数tid 是希望等待的线程的线程号，status 是指向线程返回值的指针，线程的返回值就是pthread_exit 中的value_ptr 参数，或者是return语句中的返回值。该函数可用于线程间的同步。
=============================================================
int pthread_mutex_init(
                  pthread_mutex_t *mutex,
                  const pthread_mutex_attr_t* attr
                  );
//该函数初始化一个互斥体变量，如果参数attr 为NULL，则互斥
//体变量mutex 使用默认的属性。
=============================================================
int pthread_mutex_lock(
                  pthread_mutex_t *mutex
                  );
// 该函数用来锁住互斥体变量。如果参数mutex 所指的互斥体已经
//被锁住了，那么发出调用的线程将被阻塞直到其他线程对mutex 解锁。
=============================================================
int pthread_mutex_trylock(
                     pthread_t *mutex
                     );
//该函数用来锁住mutex 所指定的互斥体，但不阻塞。如果该互斥
//体已经被上锁，该调用不会阻塞等待，而会返回一个错误代码。
=============================================================
int pthread_mutex_unlock(
                    pthread_mutex_t *mutex
                    );
//该函数用来对一个互斥体解锁。如果当前线程拥有参数mutex 所
//指定的互斥体，该调用将该互斥体解锁。
=============================================================
int pthread_mutex_destroy (
                      pthread_mutex_t *mutex
                      );
//该函数用来释放分配给参数mutex的资源。调用成功时返回值为
//0， 否则返回一个非0 的错误代码。
=============================================================
int pthread_cond_init(
                 pthread_cond_t *cond,
                 const pthread_cond_attr_t*attr
                 );
//该函数按参数attr指定的属性创建一个条件变量。调用成功返回，
//并将条件变量ID 赋值给参数cond，否则返回错误代码。
=============================================================
int pthread_cond_wait (
                  pthread_cond_t *cond ,
                  pthread_mutex_t*mutex
                  );
// 该函数调用为参数mutex 指定的互斥体解锁，等待一个事件（由
//参数cond 指定的条件变量）发生。调用该函数的线程被阻塞直到有其他
//线程调用pthread_cond_signal或pthread_cond_broadcast函数置相应的条
//件变量，而且获得mutex 互斥体时才解除阻塞。
=============================================================
int pthread_cond_timewait(
                     pthread_cond_t *cond ,
                     pthread_mutex_t*mutex ,
                     const struct timespec *abstime
                     );
// 该函数与pthread_cond_wait不同的是当系统时间到达abstime 参数指定的时间时，被阻塞线程也可以被唤起继续执行。
=============================================================
int pthread_cond_broadcast(
                      pthread_cond_t *cond
                      );
// 该函数用来对所有等待参数cond所指定的条件变量的线程解除阻塞，调用成功返回0，否则返回错误代码。
=============================================================
int pthread_cond_signal(
                   pthread_cond_t *cond
                   );
// 该函数的作用是解除一个等待参数cond所指定的条件变量的线程的阻塞状态。当有多个线程挂起等待该条件变量，也只唤醒一个线程。
=============================================================
int pthread_cond_destroy(
                    pthread_cond_t *cond
                    );
// 该函数的作用是释放一个条件变量。释放为条件变量cond 所分配的资源。调用成功返回值为0，否则返回错误代码。
=============================================================
int pthread_key_create(
                  pthread_key_t key ,
                  void(*destructor(void*))
                  );
// 该函数创建一个键值，该键值映射到一个专有数据结构体上。如果第二个参数不是NULL，这个键值被删除时将调用这个函数指针来释放数据空间。
=============================================================
int pthread_key_delete(
                  pthread_key_t *key
                  );
// 该函数用于删除一个由pthread_key_create函数调用创建的TSD键。调用成功返回值为0，否则返回错误代码。
=============================================================
int pthread_setspecific(
                   pthread_key_t key ,
                   const void(value)
                   );
// 该函数设置一个线程专有数据的值，赋给由pthread_key_create 创建的TSD 键，调用成功返回值为0，否则返回错误代码。
=============================================================
void *pthread_getspecific(
                   pthread_key_t *key
                   );
// 该函数获得绑定到指定TSD 键上的值。调用成功，返回给定参数key 所对应的数据。如果没有数据连接到该TSD 键，则返回NULL。
=============================================================
int pthread_once(
            pthread_once_t* once_control,
             void(*init_routine)(void)
             );
//该函数的作用是确保init_routine指向的函数，在调用pthread_once的线程中只被运行一次。once_control 指向一个静态或全局的变量。
=============================================================
     在code review中，我会发现很多人喜欢在pthread_mutex_lock()和pthread_mutex_unlock(()之间调用 pthread_cond_signal或者pthread_cond_broadcast函数，从逻辑上来说，这种使用方法是完全正确的。但是在多线程 环境中，这种使用方法可能是低效的。posix1标准说，pthread_cond_signal与pthread_cond_broadcast无需考 虑调用线程是否是mutex的拥有者，也就是所，可以在lock与unlock以外的区域调用。如果我们对调用行为不关心，那么请在lock区域之外调用
 吧。这里举个例子：
       我们假设系统中有线程1和线程2，他们都想获取mutex后处理共享数据，再释放mutex。请看这种序列：
1)线程1获取mutex，在进行数据处理的时候，线程2也想获取mutex，但是此时被线程1所占用，线程2进入休眠，等待mutex被释放。
2)线程1做完数据处理后，调用pthread_cond_signal（）唤醒等待队列中某个线程，在本例中也就是线程2。线程1在调用 pthread_mutex_unlock（）前，因为系统调度的原因，线程2获取使用CPU的权利，那么它就想要开始处理数据，但是在开始处理之 前，mutex必须被获取，很遗憾，线程1正在使用mutex，所以线程2被迫再次进入休眠。
3)然后就是线程1执行pthread_mutex_unlock（）后，线程2方能被再次唤醒。
从这里看，使用的效率是比较低的，如果再多线程环境中，这种情况频繁发生的话，是一件比较痛苦的事情。
所以觉得，如果程序不关心线程可预知的调度 行为，那么最好在锁定区域以外调用他们吧：－）
如果读者喜欢英文的话，可以参考：
[http://www.opengroup.org/onlinepubs/007908799/xsh/pthread_cond_broadcast.html](http://www.opengroup.org/onlinepubs/007908799/xsh/pthread_cond_broadcast.html)
这 里罗嗦几句，对于
int pthread_cond_wait(pthread_cond_t **cond*,pthread_mutex_t **mutex*);
int pthread_cond_timedwait(pthread_cond_t **cond*, 
    pthread_mutex_t **mutex*, conststruct timespec **abstime*);
，一定要在mutex的锁定区域内使用。
如果要正确的使用pthread_mutex_lock与pthread_mutex_unlock，请参考
pthread_cleanup_push 和pthread_cleanup_pop宏，它能够在线程被cancel的时候正确的释放mutex！
[http://blog.csdn.net/hello_wyq/archive/2006/08/23/1108264.aspx](http://blog.csdn.net/hello_wyq/archive/2006/08/23/1108264.aspx)
PTHREAD_COND(3)
名称
pthread_cond_init, pthread_cond_destroy, pthread_cond_signal,
pthread_cond_broadcast, pthread_cond_wait, pthread_cond_timedwait - 状态操作。
大纲
#include 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr);
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, conststruct 
timespec *abstime);
int pthread_cond_destroy(pthread_cond_t *cond);
描述
状态变量是一种同步设备，它允许线程挂起并放弃CPU时间以等待某些共享变量满足状态。操作状态变量的基本操作：当状态满足时发送信号，等待状态满足，在其它线程发送状态信号之前挂起线程。
状态变量通常和互斥体联系在一起，为了避免竞争状态出现，一个线程准备等待一个状态变量之前另外一个线程要先设置好状态信号。
pthread_cond_init初始化状态变量cond，使用cond_attr中定义了的状态属性，如果cond_attr为NULL，将会使用默认属性。LinuxThreads的实现支持没有任何属性的cond_attr，因此，cond_attr就是被忽略的。
pthread_cond_t类型的变量也可以使用PTHREAD_COND_INITIALIZER.常量进行静态的初始化。
pthread_cond_signal函数重新开始一个正在等待cond变量的线程。如果没有线程在等待cond变量，不执行任何操作。如果有多个线程都在等待，某个匹配的线程会被重新开始，但是不一定是哪个。
pthread_cond_broadcast函数重新开始所有在等待cond变量的线程。如果没有线程在等待cond变量，不执行任何操作。
pthread_cond_wait函数对互斥体进行原子的解锁工作（就像pthread_unlock_mutex），然后等待状态信号。线程被挂起并不消耗CPU时间，直到发送状态信号。互斥体必须被调用者锁定。在返回调用线程之前，互斥锁被pthread_cond_wait拥有。
释放互斥体和在状态变量上挂起是自动进行的。因此，如果所有的线程经常在状态信号之前要求互斥体，这会保证在线程在状态变量上锁定互斥体的期间状态变量不会触发信号。
pthread_cond_timedwait函数自动释放互斥体并且等待cond状态，就像pthread_cond_wait所做的一样，但是它限制了最大的等待时间。如果cond没有在一定时间内用abstime中没有指定的时间做标记，互斥体会重新获得，然后返回错误码ETIMEOUT。abstime参数指定绝对时间，和time(2)和gettimeofday(2)的一样：以格林威治时间1970年1月1日零点为起点。
pthread_cond_destroy函数销毁状态变量，释放它可能持有的资源。没有线程必须在这里等待状态变量。在LinuxThreads实现中，状态变量不与任何资源有关系，所以这个接口除了检查状态变量上是否有等待的线程之外不做任何事。
取消
pthread_cond_wait和pthread_cond_timedwait都是取消点。如果某个线程在某个这样的函数中挂起后又被取消，该线程会重新开始执行，然后再锁定互斥体。（尚未完成！）
同步信号安全
状态函数不是同步安全的，不应该出现在信号处理函数中。特别的，从信号处理函数中调用pthread_cond_signal或pthread_cond_broadcast会导致死锁。
返回值
所有状态变量函数在成功的时候返回0，在出错的时候返回出错码。
错误码
pthread_cond_init, pthread_cond_signal, pthread_cond_broadcast和pthread_cond_wait从来都不会返回出错码。
pthread_cond_timedwait函数在出错时返回下面的出错码：
ETIMEOUT：状态变量在abstime限定时间内没有激发信号。
EINTR：pthread_cond_timedwait被信号中断。
pthread_cond_destroy函数在出错时返回下面的出错码：
EBUSY：有线程正在cond上等待。
作者
Xavier Leroy 
参见
pthread_condattr_init(3), pthread_mutex_lock(3), pthread_mutex_unlock(3)
gettimeofday(2), nanosleep(2).
示例
考虑两个共享变量x和y，使用互斥体mut进行保护，一个状态变量cond在x大于y时激发信号。
int x,y;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
一直等待，直到x大于y：
pthread_mutex_lock(&mut);
while (x <= y) {
pthread_cond_wait(&cond, &mut);
}
/* operate on x and y */
pthread_mutex_unlock(&mut);
修改x和y可能会使x大于y。如果需要就应该发送信号：
pthread_mutex_lock(&mut);
/* modify x and y */
if (x > y) pthread_cond_broadcast(&cond);
pthread_mutex_unlock(&mut);
如果可以证明，至多有一个等待中的线程需要醒来（例如，只有两个线程，通过x和y联系起来），pthread_cond_signal可以作为可选的更有效率的轻量级的pthread_cond_broadcast.。如果不能确定，使用pthread_cond_broadcast函数。
要在5秒钟内等待x大于y，像下面：
struct timeval now;
struct timespec timeout;
int retcode;
pthread_mutex_lock(&mut);
gettimeofday(&now);
timeout.tv_sec = now.tv_sec + 5;
timeout.tv_nsec = now.tv_usec * 1000;
retcode = 0;
while (x <= y && retcode != ETIMEDOUT) {
retcode = pthread_cond_timedwait(&cond, &mut, &timeout);
}
if (retcode == ETIMEDOUT) {
/* timeout occurred */
} else {
/* operate on x and y */
}
pthread_mutex_unlock(&mut); 
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[](http://blog.csdn.net/onlyou930/article/details/6432621)
### 条件锁（条件Mutex）pthread_cond_wait、pthread_cond_signal、pthread_cond_broadcast的使用
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
这个例子的意思是，两个线程要修改X和 Y的值，第一个线程当X<=Y时就挂起，直到X>Y时才继续执行（由第二个线程可能会修改X,Y的值，当X>Y时唤醒第一个线程），即 首先初始化一个普通互斥量mut和一个条件变量cond。之后分别在两个线程中分别执行如下函数体：
pthread_mutex_lock(&mut);
              while (x <= y) {
                      pthread_cond_wait(&cond, &mut);
              }
              /* operate on x and y */
              pthread_mutex_unlock(&mut);
和：       pthread_mutex_lock(&mut);
              /* modify x and y */
              if (x > y) pthread_cond_signal(&cond);
              pthread_mutex_unlock(&mut);
    其实函数的执行过程非常简单，在第一个线程执行到pthread_cond_wait(&cond,&mut)时，此时如果X<=Y，则此函数就将mut互斥量解锁 ，再将cond条件变量加锁 ，此时第一个线程挂起 （不占用任何CPU周期）。
    而在第二个线程中，本来因为mut被第一个线程锁住而阻塞，此时因为mut已经释放，所以可以获得锁mut，并且进行修改X和Y的值，在修改之后，一个IF语句判定是不是X>Y，如果是，则此时pthread_cond_signal()函数会唤醒第一个线程 ，并在下一句中释放互斥量mut。然后第一个线程开始从pthread_cond_wait()执行，首先要再次**锁mut**，
 如果锁成功，再进行**条件的判断**（至于为什么用WHILE，即在被唤醒之后还要再判断，后面有原因分析），如果满足条件，则**被唤醒**进行处理，最后释放互斥量mut 。
    至于为什么在被唤醒之后还要再次进行条件判断（即为什么要使用while循环来判断条件），是因为可能有“惊群效应”。有人觉得此处既然是被唤醒的，肯定 是满足条件了，其实不然。如果是多个线程都在等待这个条件，而同时只能有一个线程进行处理，此时就必须要再次条件判断，以使只有一个线程进入临界区处理。 对此，转来一段：
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
1，pthread_cond_signal在多处理器上可能同时唤醒多个线程，当你只能让一个线程处理某个任务时，其它被唤醒的线程就需要继续 wait,while循环的意义就体现在这里了，而且规范要求pthread_cond_signal至少唤醒一个pthread_cond_wait上 的线程，其实有些实现为了简单在单处理器上也会唤醒多个线程. 
2，某些应用，如线程池，pthread_cond_broadcast唤醒全部线程，但我们通常只需要一部分线程去做执行任务，所以其它的线程需要继续wait.所以强烈推荐此处使用while循环.
       其实说白了很简单，就是pthread_cond_signal()也可能唤醒多个线程，而如果你同时只允许一个线程访问的话，就必须要使用while来进行条件判断，以保证临界区内只有一个线程在处理。
