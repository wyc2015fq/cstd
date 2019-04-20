# Solaris 线程编程 - h13 - 博客园
本章比较了 Solaris 线程和 POSIX 线程的应用程序编程接口 (application programming interface, API)，并介绍了 POSIX 线程中没有的 Solaris 功能。本章讨论以下主题：
- 
[比较 Solaris 线程和 POSIX 线程的 API](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbj?a=view)
- 
[Solaris 线程的独有函数](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbk?a=view)
- 
[相似的同步函数－读写锁](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbl?a=view)
- 
[相似的 Solaris 线程函数](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbm?a=view)
- 
[相似的同步函数－互斥锁](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbn?a=view)
- 
[相似的同步函数：条件变量](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbo?a=view)
- 
[相似的同步函数：信号](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbp?a=view)
- 
[**fork()** 和 Solaris 线程的特殊问题](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbr?a=view)
# 比较 Solaris 线程和 POSIX 线程的 API
Solaris 线程 API 和 pthread API 是同一问题的两种不同解决方案，即在应用程序软件中建立并行性。尽管每个 API 都是完整的，但是可以安全地在同一程序中混合使用 Solaris 线程函数和 pthread 函数。
不过，这两个 API 并不完全匹配。Solaris 线程支持 pthread 中没有的函数，而 pthread 中则包括 Solaris 接口不支持的函数。对于那些**匹配**的函数，尽管信息内容实际相同，但是关联参数可能并不相同。
通过合并这两个 API，可以使用仅存在于其中一个 API 中的功能来增强另一个 API。同样，在同一个系统中还可以同时运行仅使用 Solaris 线程的应用程序和仅使用 pthread 的应用程序。
## API 的主要差异
Solaris 线程和 pthread 在 API 操作和语法方面非常相似。[表 8–1](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbj?a=view#sthreads-60367) 中列出了两者之间的主要差异。 
表 8–1 Solaris 线程和 pthread 的独有功能 
|Solaris 线程|POSIX 线程|
|----|----|
|使用 thr_ 前缀表示线程函数的名称，使用 sema_ 前缀表示信号函数的名称|使用 pthread_ 前缀表示 pthread 函数的名称，使用 sem_ 前缀表示信号函数的名称|
|能够创建“守护进程”线程|取消语义|
|暂停和继续执行线程|调度策略|
## 函数比较表
下表对 Solaris 线程函数和 pthread 函数进行了比较。请注意，即使 Solaris 线程函数和 pthread 函数看上去完全相同，但是其参数可以不同。 
如果某个进行比较的接口在 pthread 或 Solaris 线程中不可用，则会在相应的列中显示一个连字符 "-"。pthread 列中后跟 "POSIX.1b" 的各项属于 POSIX 实时标准规范，而不属于 pthread。
表 8–2 Solaris 线程和 POSIX pthread 的比较 
|Solaris 线程|pthread|
|----|----|
|**thr_create()**|**pthread_create()**|
|**thr_exit()**|**pthread_exit()**|
|**thr_join()**|**pthread_join()**|
|**thr_yield()**|**sched_yield()** POSIX.1b|
|**thr_self()**|**pthread_self()**|
|**thr_kill()**|**pthread_kill()**|
|**thr_sigsetmask()**|**pthread_sigmask()**|
|**thr_setprio()**|**pthread_setschedparam()**|
|**thr_getprio()**|**pthread_getschedparam()**|
|**thr_setconcurrency()**|**pthread_setconcurrency()**|
|**thr_getconcurrency()**|**pthread_getconcurrency()**|
|**thr_suspend()**|-|
|**thr_continue()**|-|
|**thr_keycreate()**|**pthread_key_create()**|
|-|**pthread_key_delete()**|
|**thr_setspecific()**|**pthread_setspecific()**|
|**thr_getspecific()**|**pthread_getspecific()**|
|-|**pthread_once()**|
|-|**pthread_equal()**|
|-|**pthread_cancel()**|
|-|**pthread_testcancel()**|
|-|**pthread_cleanup_push()**|
|-|**pthread_cleanup_pop()**|
|-|**pthread_setcanceltype()**|
|-|**pthread_setcancelstate()**|
|**mutex_lock()**|**pthread_mutex_lock()**|
|**mutex_unlock()**|**pthread_mutex_unlock()**|
|**mutex_trylock()**|**pthread_mutex_trylock()**|
|**mutex_init()**|**pthread_mutex_init()**|
|**mutex_destroy()**|**pthread_mutex_destroy()**|
|**cond_wait()**|**pthread_cond_wait()**|
|**cond_timedwait()**|**pthread_cond_timedwait()**|
|**cond_reltimedwait()**|**pthread_cond_reltimedwait_np()**|
|**cond_signal()**|**pthread_cond_signal()**|
|**cond_broadcast()**|**pthread_cond_broadcast()**|
|**cond_init()**|**pthread_cond_init()**|
|**cond_destroy()**|**pthread_cond_destroy()**|
|**rwlock_init()**|**pthread_rwlock_init()**|
|**rwlock_destroy()**|**pthread_rwlock_destroy()**|
|**rw_rdlock()**|**pthread_rwlock_rdlock()**|
|**rw_wrlock()**|**pthread_rwlock_wrlock()**|
|**rw_unlock()**|**pthread_rwlock_unlock()**|
|**rw_tryrdlock()**|**pthread_rwlock_tryrdlock()**|
|**rw_trywrlock()**|**pthread_rwlock_trywrlock()**|
|-|**pthread_rwlockattr_init()**|
|-|**pthread_rwlockattr_destroy()**|
|-|**pthread_rwlockattr_getpshared()**|
|-|**pthread_rwlockattr_setpshared()**|
|**sema_init()**|**sem_init()** POSIX.1b|
|**sema_destroy()**|**sem_destroy()** POSIX.1b|
|**sema_wait()**|**sem_wait()** POSIX.1b|
|**sema_post()**|**sem_post()** POSIX.1b|
|**sema_trywait()**|**sem_trywait()** POSIX.1b|
|**fork1()**|**fork()**|
|-|**pthread_atfork()**|
|**forkall()**，多线程副本|-|
|-|**pthread_mutexattr_init()**|
|-|**pthread_mutexattr_destroy()**|
|**mutex_init()** 中的 type 参数|**pthread_mutexattr_setpshared()**|
|-|**pthread_mutexattr_getpshared()**|
|-|**pthread_mutex_attr_settype()**|
|-|**pthread_mutex_attr_gettype()**|
|-|**pthread_condattr_init()**|
|-|**pthread_condattr_destroy()**|
|**cond_init()** 中的 type 参数|**pthread_condattr_setpshared()**|
|-|**pthread_condattr_getpshared()**|
|-|**pthread_attr_init()**|
|-|**pthread_attr_destroy()**|
|**thr_create()** 中的 THR_BOUND 标志|**pthread_attr_setscope()**|
|-|**pthread_attr_getscope()**|
|-|**pthread_attr_setguardsize()**|
|-|**pthread_attr_getguardsize()**|
|**thr_create()** 中的 stack_size 参数|**pthread_attr_setstacksize()**|
|-|**pthread_attr_getstacksize()**|
|**thr_create()** 中的 stack_addr 参数|**pthread_attr_setstack()**|
|-|**pthread_attr_getstack()**|
|**thr_create()** 中的 THR_DETACH 标志|**pthread_attr_setdetachstate()**|
|-|**pthread_attr_getdetachstate()**|
|-|**pthread_attr_setschedparam()**|
|-|**pthread_attr_getschedparam()**|
|-|**pthread_attr_setinheritsched()**|
|-|**pthread_attr_getinheritsched()**|
|-|**pthread_attr_setsschedpolicy()**|
|-|**pthread_attr_getschedpolicy()**|
要使用本章中介绍的用于 Solaris 9 和以前发行版的 Solaris 线程函数，必须使用 Solaris 线程库 **-lthread** 进行链接。
对于 Solaris 线程和 pthread 来说，即使函数名或参数可能会有所不同，但是操作实际上是相同的。此处仅提供了一个简单的示例，其中包括正确的头文件和函数原型。如果没有为 Solaris 线程函数提供返回值，请参见[《man pages section 3: Basic Library Functions》](http://docs.sun.com/app/docs/doc/816-5168)中的相应页以获取函数的返回值。
有关 Solaris 相关函数的更多信息，请参见相关 pthread 文档中以类似方式命名的函数。
如果 Solaris 线程函数所提供的功能在 pthread 中不可用，则会提供这些函数的完整说明。
# Solaris 线程的独有函数
本节介绍 Solaris 线程的独有函数：用于暂停执行线程和继续执行暂停的线程。
## 暂停执行线程
[thr_suspend(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hs0r?a=view) 可用来立即暂停执行 target_thread 所指定的线程。如果从 **thr_suspend()** 成功返回，则将不再执行暂停的线程。 
因为 **thr_suspend()** 在暂停目标线程时不会考虑该线程可能持有的锁，所以，在使用 **thr_suspend()** 时一定要格外小心。如果要暂停的线程调用的函数需要由已暂停的目标线程拥有的锁，则将产生死锁。
### thr_suspend 语法
#include <thread.h>
int thr_suspend(thread_t tid);
线程暂停之后，以后调用 **thr_suspend()** 将不起任何作用。信号无法唤醒暂停的线程。线程恢复执行之前，信号将一直保持暂挂状态。 
在以下概要中，pthread 中定义的 `pthread_t`tid 与 Solaris 线程中定义的 `thread_t`tid 相同。这两个 tid 值可以通过赋值或通过使用强制转换来互换使用。 
thread_t tid; /* tid from thr_create() */
/* pthreads equivalent of Solaris tid from thread created */
/* with pthread_create() */
pthread_t ptid; 
int ret;
ret = thr_suspend(tid);
/* using pthreads ID variable with a cast */
ret = thr_suspend((thread_t) ptid); 
### thr_suspend 返回值
**thr_suspend()** 在成功完成之后返回零。其他任何返回值都表示出现了错误。如果出现以下情况，**thr_suspend()** 将失败并返回对应的值。
ESRCH
**描述: **
当前的进程中找不到 tid。
## 继续执行暂停的线程
[thr_continue(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hs0a?a=view) 可用来恢复执行暂停的线程。继续执行暂停的线程之后，以后调用 **thr_continue()** 将不起任何作用。 
### thr_continue 语法
#include <thread.h>
int thr_continue(thread_t tid);
信号无法唤醒暂停的线程。**thr_continue()** 继续执行暂停的线程之前，信号将一直保持暂挂状态。 
在 pthread 中定义的 `pthread_t`tid 与在 Solaris 线程中定义的 `thread_t`tid 相同。这两个 tid 值可以通过赋值或通过使用强制转换来互换使用。
thread_t tid; /* tid from thr_create()*/
/* pthreads equivalent of Solaris tid from thread created */
/* with pthread_create()*/
pthread_t ptid; 
int ret;
ret = thr_continue(tid);
/* using pthreads ID variable with a cast */
ret = thr_continue((thread_t) ptid) 
### thr_continue 返回值
**thr_continue()** 在成功完成之后返回零。其他任何返回值都表示出现了错误。如果出现以下情况，**thr_continue()** 将失败并返回对应的值。
ESRCH
**描述: **
当前的进程中找不到 tid。
# 相似的同步函数－读写锁
读写锁允许多个线程同时进行读取访问，但限制每次只能对一个线程进行写入访问。本节讨论了以下主题：
- 
初始化读取器/写入器锁
- 
获取读锁
- 
尝试获取读锁
- 
获取写锁
- 
尝试获取写锁
- 
解除锁定读取器/写入器锁
- 
销毁读取器/写入器锁的状态
一个线程持有读锁时，其他线程也可以获取读锁，但要获取写锁则必须等待。 如果一个线程持有写锁或者正在等待获取写锁，则其他线程必须等待才能获取读锁或写锁。 
相比互斥锁，读写锁速度较慢。但是，如果许多并发线程读取使用锁保护的数据，但不经常进行写入，则使用读写锁可以提高性能。
使用读写锁可以同步此进程和其他进程中的线程。读写锁是在可以写入并在协作进程之间共享的内存中分配的。有关针对此行为映射读写锁的信息，请参见 [mmap(2)](http://docs.sun.com/app/docs/doc/816-5167/6mbb2jahm?a=view) 手册页。
缺省情况下，多个线程正在等待读写锁时，获取锁的顺序是不确定的。但是，为了避免写入器资源匮乏，对于优先级相同的写入器和读取器，Solaris 线程软件包中写入器往往优先于读取器。
读写锁在使用之前必须先初始化。
## 初始化读写锁
使用 [rwlock_init(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrpf?a=view) 可以初始化 rwlp 所指向的读写锁并将锁的状态设置为未锁定。
### rwlock_init 语法
#include <synch.h>  （或  #include <thread.h>）
int rwlock_init(rwlock_t *rwlp, int type, void * arg);
type 可以是以下值之一：
- 
USYNC_PROCESS。读写锁可用来同步此进程和其他进程中的线程。arg 会被忽略。 
- 
USYNC_THREAD。读写锁可用来仅同步此进程中的线程。arg 会被忽略。 
同一个读写锁不能同时由多个线程初始化。读写锁还可以通过在清零的内存中进行分配来初始化，在这种情况下假设 type 为 USYNC_THREAD。对于其他线程可能正在使用的读写锁，不得重新初始化。
对于 POSIX 线程，请参见[pthread_rwlock_init 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpal?a=view#sync-51)。
#### 初始化进程内读写锁
#include <thread.h>
rwlock_t rwlp;
int ret;
/* to be used within this process only */
ret = rwlock_init(&rwlp, USYNC_THREAD, 0); 
#### 初始化进程间读写锁
#include <thread.h>
rwlock_t rwlp;
int ret;
/* to be used among all processes */
ret = rwlock_init(&rwlp, USYNC_PROCESS, 0); 
### rwlock_init 返回值
**rwlock_init()** 在成功完成之后返回零。其他任何返回值都表示出现了错误。如果出现以下任一情况，该函数将失败并返回对应的值。
EINVAL
**描述: **
参数无效。
EFAULT
**描述: **
rwlp 或 arg 指向的地址非法。
## 获取读锁
使用 [rw_rdlock(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrpg?a=view) 可以获取 rwlp 所指向的读写锁中的读锁。 
### rw_rdlock 语法
#include <synch.h> （或  #include <thread.h>）
int rw_rdlock(rwlock_t *rwlp);
如果读写锁中的写锁已经锁定，则调用线程将阻塞，直到释放写锁为止。否则，将获取读锁。对于 POSIX 线程，请参见[pthread_rwlock_rdlock 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpal?a=view#sync-65)。
### rw_rdlock 返回值
**rw_rdlock()** 在成功完成之后返回零。其他任何返回值都表示出现了错误。如果出现以下任一情况，该函数将失败并返回对应的值。
EINVAL
**描述: **
参数无效。
EFAULT
**描述: **
rwlp 指向的地址非法。
## 尝试获取读锁
使用 [rw_tryrdlock(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrph?a=view) 可以尝试获取 rwlp 所指向的读写锁中的读锁。
### rw_tryrdlock 语法
#include <synch.h>  （或  #include <thread.h>）
int rw_tryrdlock(rwlock_t *rwlp);
如果读写锁中的写锁已经锁定，则 **rw_tryrdlock()** 将返回错误。否则，将获取读锁。对于 POSIX 线程，请参见[pthread_rwlock_tryrdlock 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpal?a=view#sync-67)。
### rw_tryrdlock 返回值
**rw_tryrdlock()** 在成功完成之后返回零。其他任何返回值都表示出现了错误。如果出现以下任一情况，该函数将失败并返回对应的值。
EINVAL
**描述: **
参数无效。
EFAULT
**描述: **
rwlp 指向的地址非法。
EBUSY
**描述: **
rwlp 所指向的读写锁已经锁定。
## 获取写锁
使用 [rw_wrlock(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrpk?a=view) 可以获取 rwlp 所指向的读写锁中的写锁。
### rw_wrlock 语法
#include <synch.h>  （或  #include <thread.h>）
int rw_wrlock(rwlock_t *rwlp);
如果读写锁中的读锁或写锁已经锁定，则调用线程将阻塞，直到释放所有的读锁和写锁为止。读写锁中的写锁一次只能由一个线程持有。对于 POSIX 线程，请参见[pthread_rwlock_wrlock 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpal?a=view#sync-71)。
### rw_wrlock 返回值
**rw_wrlock()** 在成功完成之后返回零。其他任何返回值都表示出现了错误。如果出现以下任一情况，该函数将失败并返回对应的值。
EINVAL
**描述: **
参数无效。
EFAULT
**描述: **
rwlp 指向的地址非法。
## 尝试获取写锁
使用 [rw_trywrlock(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrpi?a=view) 可以尝试获取 rwlp 所指向的读写锁中的写锁。
### rw_trywrlock 语法
#include <synch.h>  （或  #include <thread.h>）
int rw_trywrlock(rwlock_t *rwlp);
如果读写锁上的读锁或写锁已经锁定，则 **rw_trywrlock()** 将返回错误。对于 POSIX 线程，请参见[pthread_rwlock_trywrlock 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpal?a=view#sync-72)。
### rw_trywrlock 返回值
**rw_trywrlock()** 在成功完成之后返回零。其他任何返回值都表示出现了错误。如果出现以下任一情况，该函数将失败并返回对应的值。
EINVAL
**描述: **
参数无效。
EFAULT
**描述: **
rwlp 指向的地址非法。
EBUSY
**描述: **
rwlp 所指向的读写锁已经锁定。
## 解除锁定读写锁
使用 [rw_unlock(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrpj?a=view) 可以解除锁定 rwlp 所指向的读写锁。
### rw_unlock 语法
#include <synch.h>  （或  #include <thread.h>）
int rw_unlock(rwlock_t *rwlp);
读写锁必须处于锁定状态，并且调用线程必须持有读锁或写锁。如果还有其他线程正在等待读写锁成为可用，则其中一个线程将被解除阻塞。对于 POSIX 线程，请参见[pthread_rwlock_unlock 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpal?a=view#sync-74)。
### rw_unlock 返回值
**rw_unlock()** 在成功完成之后返回零。其他任何返回值都表示出现了错误。如果出现以下任一情况，该函数将失败并返回对应的值。
EINVAL
**描述: **
参数无效。
EFAULT
**描述: **
rwlp 指向的地址非法。
## 销毁读写锁的状态
使用 [rwlock_destroy(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrpe?a=view) 可以销毁与 rlwp 所指向的读写锁相关联的任何状态。
### rwlock_destroy 语法
#include <synch.h>  （或  #include <thread.h>）
int rwlock_destroy(rwlock_t *rwlp);
用来存储读写锁的空间不会释放。对于 POSIX 线程，请参见[pthread_rwlock_destroy 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpal?a=view#sync-63)。
[示例 8–1](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbl?a=view#sthreads-ex-18) 使用银行帐户来说明读写锁。尽管该程序可能会允许多个线程对帐户余额进行并行只读访问，但是仅允许使用一个写入器。请注意，**get_balance()** 函数需要使用该锁才能确保以原子方式添加支票帐户余额和储蓄帐户余额。 
##### 示例 8–1 读写银行帐户
rwlock_t account_lock;
float checking_balance = 100.0;
float saving_balance = 100.0;
...
rwlock_init(&account_lock, 0, NULL);
...
float
get_balance() {
    float bal;
    rw_rdlock(&account_lock);
    bal = checking_balance + saving_balance;
    rw_unlock(&account_lock);
    return(bal);
}
void
transfer_checking_to_savings(float amount) {
    rw_wrlock(&account_lock);
    checking_balance = checking_balance - amount;
    saving_balance = saving_balance + amount;
    rw_unlock(&account_lock);
}
### rwlock_destroy 返回值
**rwlock_destroy()** 在成功完成之后返回零。其他任何返回值都表示出现了错误。如果出现以下任一情况，该函数将失败并返回对应的值。
EINVAL
**描述: **
参数无效。
EFAULT
**描述: **
rwlp 指向的地址非法。
# 相似的 Solaris 线程函数
表 8–3 相似的 Solaris 线程函数 
|操作|相关函数说明|
|----|----|
|创建线程|[thr_create 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbm?a=view#sthreads-69878)|
|获取最小的栈大小|[thr_min_stack 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbm?a=view#sthreads-81368)|
|获取线程标识符|[thr_self 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbm?a=view#sthreads-64159)|
|停止执行线程|[thr_yield 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbm?a=view#sthreads-17506)|
|向线程发送信号|[thr_kill 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbm?a=view#sthreads-26201)|
|访问调用线程的信号掩码|[thr_sigsetmask 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbm?a=view#sthreads-31515)|
|终止线程|[thr_exit 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbm?a=view#sthreads-85402)|
|等待线程终止|[thr_join 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbm?a=view#sthreads-53721)|
|创建线程特定的数据键|[thr_keycreate 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbm?a=view#sthreads-11038)|
|设置线程特定数据|[thr_setspecific 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbm?a=view#sthreads-33444)|
|获取线程特定数据|[thr_getspecific 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbm?a=view#sthreads-32584)|
|设置线程优先级|[thr_setprio 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbm?a=view#sthreads-11547)|
|获取线程优先级|[thr_getprio 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbm?a=view#sthreads-16796)|
## 创建线程
thr_create(3C) 例程是 Solaris 线程接口中最详细的所有例程其中之一。 
使用 [thr_create(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hs0b?a=view) 可以向当前的进程中增加新的受控线程。对于 POSIX 线程，请参见[pthread_create 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpa9?a=view#tlib-95520)。
### thr_create 语法
#include <thread.h>
int thr_create(void *stack_base, size_t stack_size,
    void *(*start_routine) (void *), void *arg, long flags,
    thread_t *new_thread);
size_t thr_min_stack(void);
请注意，新线程不会继承暂挂的信号，但确实会继承优先级和信号掩码。 
stack_base。包含新线程所使用的栈的地址。如果 stack_base 为 NULL，则 **thr_create()** 会为新线程分配一个至少为 **stack_size** 字节的栈。 
stack_size。包含新线程所使用的栈的大小（以字节数表示）。如果 stack_size 为零，则使用缺省大小。在大多数情况下，零值最适合。如果 stack_size 不为零，则 stack_size 必须大于 **thr_min_stack()** 返回的值。 
通常，无需为线程分配栈空间。系统会为每个线程的没有保留交换空间的栈分配 1 MB 的虚拟内存。系统使用 [mmap(2)](http://docs.sun.com/app/docs/doc/816-5167/6mbb2jahm?a=view) 的 **-MAP_NORESERVE** 选项来进行分配。 
start_routine。包含用以开始执行新线程的函数。**start_routine()** 返回时，该线程将退出，并且其退出状态会设置为 **start_routine** 返回的值。请参见[thr_exit 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbm?a=view#sthreads-85402)。 
arg。可以是 void 所描述的任何变量，通常是任何大小为 4 字节的值。对于较大的值，必须通过将该参数指向对应的变量来间接传递。
请注意，仅可以提供一个参数。要在程序中采用多个参数，请将多个参数编码为单个参数，如通过将这些参数放在一个结构中。 
flags。指定所创建的线程的属性。在大多数情况下，最适合使用零值。 
flags 中的值是通过对以下参数执行按位或运算（包含边界值）来构造的：
- 
THR_SUSPENDED。暂停新线程，并且不执行 **start_routine**，直到 **thr_continue()** 启动该线程为止。使用 THR_SUSPENDED 可以在运行该线程之前对其执行操作，如更改其优先级。 
- 
THR_DETACHED。分离新线程，以便在该线程终止之后，可以立即重用其线程 ID 和其他资源。如果不想等待线程终止，可以设置 THR_DETACHED。 
**注 – **
如果没有明确分配同步，则未暂停的分离线程会失败。如果失败，则从 **thr_create()** 返回该线程的创建者之前，会将该线程 ID 重新指定给另一个新线程。 
- 
THR_BOUND。将新线程永久绑定到 LWP。新线程是**绑定线程**。从 Solaris 9 发行版开始，系统不再区分绑定线程和非绑定线程，所有的线程均视为绑定线程。 
- 
THR_DAEMON。将新线程标记为守护进程。守护进程线程始终处于分离状态。THR_DAEMON 表示 THR_DETACHED。所有非守护进程线程退出时，该进程也会随之退出。守护进程线程不会影响进程的退出状态，并且在退出对线程数进行计数时会被忽略。 
进程的退出方法有两种，一是调用 **exit()**，二是使用进程中不是通过 THR_DAEMON 标志创建的每个线程来调用 thr_exit(3C)。 进程所调用的应用程序或库可以创建一个或多个线程，在确定是否退出时应当忽略（不计数）这些线程。THR_DAEMON 标志可标识在进程退出条件中不计数的线程。 
new_thread。如果 new_thread 不为 NULL，则它将指向 **thr_create()** 成功时用来存储新线程 ID 的位置。调用方负责提供该参数所指向的存储空间。线程 ID 仅在调用进程中有效。 
如果对该标识符不感兴趣，请向 **new_thread** 提供一个 NULL 值。
### thr_create 返回值
**thr_create()** 函数在成功完成之后返回零。其他任何返回值都表示出现了错误。如果检测到以下情况之一，**thr_create()** 将失败并返回对应的值。
EAGAIN
**描述: **
超出了系统限制，如创建的 LWP 过多。
ENOMEM
**描述: **
可用内存不足，无法创建新线程。
EINVAL
**描述: **
stack_base 不为 NULL，并且 stack_size 小于 **thr_min_stack()** 返回的值。 
## 获取最小栈大小
使用 [thr_min_stack(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hs0k?a=view) 可以获取线程的最小栈大小。
Solaris 线程中的栈行为通常与 pthread 中的栈行为相同。有关设置和操作栈的更多信息，请参见[关于栈](http://docs.sun.com/app/docs/doc/819-7051/6n919hpac?a=view#attrib-33670)。 
### thr_min_stack 语法
#include <thread.h>
size_t thr_min_stack(void);
**thr_min_stack()** 会返回执行空线程所需的空间量。创建空线程的目的是执行空过程。由于有用线程需要的栈要大于绝对最小栈，因此在减小栈大小时请务必小心。 
执行非空过程的线程所分配的栈大小应大于 **thr_min_stack()** 的大小。
如果某个线程是借助于用户提供的栈创建的，则用户必须保留足够的空间才能运行该线程。动态链接的执行环境会增加确定线程最小栈要求的难度。 
可以通过两种方法来指定自定义栈。第一种方法是为栈位置提供 NULL，从而要求运行时库为该栈分配空间，但是向 **thr_create()** 提供 stacksize 参数中所需的大小。 
另一种方法是全面负责栈管理的各个方面，并向 **thr_create()** 提供一个指向该栈的指针。这意味着不但需要负责分配栈，还需要负责取消分配栈。线程终止时，必须安排对该线程的栈进行处理。 
当您分配自己的栈时，请确保通过调用 mprotect(2) 在该栈末尾附加一个红色区域。 
大多数用户都不应当通过用户提供的栈来创建线程。用户提供的栈之所以存在，只是为了支持要求对其执行环境进行完全控制的应用程序。
相反，用户应当由系统来管理对栈的分配。系统提供的缺省栈应当能够满足所创建的任何线程的要求。
### thr_min_stack 返回值
未定义任何错误。
## 获取线程标识符
使用 [thr_self(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hs0l?a=view) 可以获取调用线程的 ID。对于 POSIX 线程，请参见[pthread_self 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpa9?a=view#tlib-89129)。
### thr_self 语法
#include <thread.h>
thread_t thr_self(void);
### thr_self 返回值
未定义任何错误。
## 停止执行线程
[thr_yield(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hs0s?a=view) 会导致当前的线程停止执行，以便执行另一个具有相同或更高优先级的线程。 否则，**thr_yield()** 不起任何作用。但是，调用 **thr_yield()** 无法保证当前的线程会停止执行。
### thr_yield 语法
#include <thread.h>
void thr_yield(void);
### thr_yield 返回值
**thr_yield()** 不会返回任何内容并且不会设置 errno。
## 向线程发送信号
[thr_kill(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hs0i?a=view) 可用来向线程发送信号。对于 POSIX 线程，请参见[pthread_self 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpa9?a=view#tlib-89129)。
### thr_kill 语法
#include <thread.h>
#include <signal.h>
int thr_kill(thread_t target_thread, int sig);
### thr_kill 返回值
如果成功完成，**thr_kill()** 将返回 0。如果检测到以下任一情况，**thr_kill()** 将失败并返回对应的值。如果失败，则不发送任何信号。
ESRCH
**描述: **
未找到与 thread ID 所指定的线程相关联的线程。
EINVAL
**描述: **
sig 参数值不为零。sig 无效或者是不支持的信号编号。
## 访问调用线程的信号掩码
使用 [thr_sigsetmask(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hs0p?a=view) 可以更改或检查调用线程的信号掩码。 
### thr_sigsetmask 语法
#include <thread.h>
#include <signal.h>
int thr_sigsetmask(int how, const sigset_t *set, sigset_t *oset);
**thr_sigsetmask()** 可用来更改或检查调用线程的信号掩码。每个线程都有自已的信号掩码。新线程会继承调用线程的信号掩码和优先级，但不会继承暂挂的信号。新线程的暂挂信号将为空。
如果参数 set 的值不为 NULL，则 set 会指向一组信号，这组信号可用来修改当前阻塞的信号组。如果 set 的值为 NULL，则 how 的值没有意义，并且不会修改线程的信号掩码。使用此行为可了解有关当前阻塞的信号的情况。
how 的值可指定更改信号组的方法。how 可以为以下值之一。
- 
SIG_BLOCK。set 对应于一组要阻塞的信号。这些信号会添加到当前的信号掩码中。
- 
SIG_UNBLOCK。set 对应于一组要解除阻塞的信号。这些信号会从当前的信号掩码中删除。
- 
SIG_SETMASK。set 对应于新的信号掩码。当前的信号掩码将替换为 set。
### thr_sigsetmask 返回值
如果成功完成，**thr_sigsetmask()** 将返回 0。如果检测到以下任一情况，**thr_sigsetmask()** 将失败并返回对应的值。
EINVAL
**描述: **
set 不为 NULL，并且 how 的值未定义。 
## 终止线程
使用 [thr_exit(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hs0c?a=view) 可以终止线程。对于 POSIX 线程，请参见[pthread_exit 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpa9?a=view#tlib-36372)。
### thr_exit 语法
#include <thread.h>
void thr_exit(void *status);
### thr_exit 返回值
**thr_exit()** 不返回到其调用方。
## 等待线程终止
使用 [thr_join(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hs0g?a=view) 可以等待目标线程终止。对于 POSIX 线程，请参见[pthread_join 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpa9?a=view#tlib-89762)。
### thr_join 语法
#include <thread.h>
int thr_join(thread_t tid, thread_t *departedid, void **status);
目标线程必须是当前进程的成员，而不能是分离线程或守护进程线程。
多个线程不能等待同一个线程完成，否则仅有一个线程会成功完成。其他线程将终止，并返回 ESRCH 错误。
如果目标线程已经终止，**thr_join()** 将不会阻塞对调用线程的处理。
#### thr_join，加入特定线程
#include <thread.h>
thread_t tid;
thread_t departedid;
int ret;
void *status;
/* waiting to join thread "tid" with status */
ret = thr_join(tid, &departedid, &status);
/* waiting to join thread "tid" without status */
ret = thr_join(tid, &departedid, NULL);
/* waiting to join thread "tid" without return id and status */
ret = thr_join(tid, NULL, NULL); 
如果 **tid** 为 `(thread_t)0`，则 **thread_join()** 将等待进程中的任何非分离线程终止。换句话说，如果未指定线程标识符，则任何未分离的线程退出都将导致返回 **thread_join()**。 
#### thr_join，加入任何线程
#include <thread.h>
thread_t tid;
thread_t departedid;
int ret;
void *status;
/* waiting to join any non-detached thread with status */
ret = thr_join(0, &departedid, &status); 
通过在 Solaris **thr_join()** 中使用 0 来表示线程 ID，进程中的任何非分离线程退出时都将执行加入操作。**departedid** 表示现有线程的线程 ID。
### thr_join 返回值
**thr_join()** 在成功运行后返回 0。如果检测到以下任一情况，**thr_join()** 将失败并返回对应的值。
ESRCH
**描述: **
未找到与目标线程 ID 对应的非分离线程。
EDEADLK
**描述: **
检测到死锁，或者目标线程的值指定了调用线程。 
## 创建线程特定的数据键
使用 [thr_keycreate(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hs0h?a=view) 可分配键，用于标识进程中线程特定数据。键可全局应用于进程中的所有线程。创建键时，每个线程都会将一个值与其绑定。
除了函数的名称和参数以外，Solaris 线程的线程特定数据与 POSIX 线程的线程特定数据完全相同。本节概述了 Solaris 函数。 对于 POSIX 线程，请参见[pthread_key_create 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpa9?a=view#tlib-22206)。
### thr_keycreate 语法
#include <thread.h>
int thr_keycreate(thread_key_t *keyp,
    void (*destructor) (void *value));
keyp 为每个绑定线程单独维护特定的值。所有的线程最初都会绑定到专用元素 keyp，该元素可用于访问其线程特定数据。创建键时，对于所有活动线程，将为新键赋予值 NULL。此外在创建线程时，还会为以前在新线程中创建的所有键赋予值 NULL。
destructor 函数是可选的，可以将其与每个 keyp 相关联。线程退出时，如果 keyp 具有非 NULL 的 destructor，并且线程具有与 keyp 相关联的非 NULL value，则 destructor 将用当前的关联 value 进行调用。如果线程退出时存在多个 destructor 与其相关，则 destructor 的调用顺序是不确定的。
### thr_keycreate 返回值
**thr_keycreate()** 在成功运行后返回 0。如果检测到以下任一情况，**thr_keycreate()** 将失败并返回对应的值。
EAGAIN
**描述: **
系统资源不足，无法创建另一个线程特定的数据键，或者键数目超过了 PTHREAD_KEYS_MAX 的每进程限制。
ENOMEM
**描述: **
可用内存不足，无法将 value 与 keyp 相关联。
## 设置线程特定的数据值
[thr_setspecific(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hs0o?a=view) 可用来将 **value** 绑定到线程特定的数据键（对于调用线程来说为 **key**）。对于 POSIX 线程，请参见[pthread_setspecific 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpa9?a=view#tlib-44687)。 
### thr_setspecific 语法
#include <thread.h>
int thr_setspecific(thread_key_t key, void *value);
### thr_setspecific 返回值
**thr_setspecific()** 在成功运行后返回 0。如果检测到以下任一情况，**thr_setspecific()** 将失败并返回对应的值。
ENOMEM
**描述: **
可用内存不足，无法将 value 与 keyp 相关联。
EINVAL
**描述: **
keyp 无效。
## 获取线程特定的数据值
[thr_getspecific(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hs0f?a=view) 可用来将当前绑定到调用线程的 **key** 的值存储到 **valuep** 所指向的位置。对于 POSIX 线程，请参见[pthread_getspecific 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpa9?a=view#tlib-81719)。
### thr_getspecific 语法
#include <thread.h>
int thr_getspecific(thread_key_t key, void **valuep);
### thr_getspecific 返回值
**thr_getspecific()** 在成功运行后返回 0。如果检测到以下任一情况，**thr_getspecific()** 将失败并返回对应的值。
ENOMEM
**描述: **
可用内存不足，无法将 value 与 keyp 相关联。
EINVAL
**描述: **
keyp 无效。
## 设置线程的优先级
在 Solaris 线程中，对于在创建时与父线程具有不同优先级的线程，可以在 SUSPEND 模式下创建。在暂停之后，可以通过使用 thr_setprio(3C) 函数调用来修改线程的优先级。**thr_setprio()** 完成之后，线程可恢复执行。 
争用同步对象时，高优先级的线程优先于低优先级的线程。
### thr_setprio 语法
[thr_setprio(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hs0n?a=view) 可用来将当前进程内 tid 所指定线程的优先级更改为 newprio 所指定的优先级。对于 POSIX 线程，请参见[pthread_setschedparam 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpa9?a=view#tlib-19798)。
#include <thread.h>
int thr_setprio(thread_t tid, int newprio)
缺省情况下，线程是基于范围从 0（最不重要）到 127（最重要）的固定优先级来调度的。 
thread_t tid;
int ret;
int newprio = 20;
/* suspended thread creation */
ret = thr_create(NULL, NULL, func, arg, THR_SUSPENDED, &tid);
/* set the new priority of suspended child thread */
ret = thr_setprio(tid, newprio);
/* suspended child thread starts executing with new priority */
ret = thr_continue(tid);
### thr_setprio 返回值
**thr_setprio()** 在成功运行后返回 0。如果检测到以下任一情况，**thr_setprio()** 将失败并返回对应的值。
ESRCH
**描述: **
tid 指定的值不引用现有的线程。
EINVAL
**描述: **
priority 的值对于与 tid 相关联的调度类没有意义。
## 获取线程的优先级
使用 [thr_getprio(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hs0e?a=view) 可以获取线程的当前优先级。每个线程都从其创建者继承优先级。**thr_getprio()** 会将当前的优先级 tid 存储到 newprio 所指向的位置。对于 POSIX 线程，请参见[pthread_getschedparam 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpa9?a=view#tlib-12846)。
### thr_getprio 语法
#include <thread.h>
int thr_getprio(thread_t tid, int *newprio)
### thr_getprio 返回值
**thr_getprio()** 在成功运行后返回 0。如果检测到以下情况，**thr_getprio()** 将失败并返回对应的值。
ESRCH
**描述: **
tid 指定的值不会引用现有的线程。
# 相似的同步函数－互斥锁
- 
初始化互斥锁
- 
销毁互斥锁
- 
获取互斥锁
- 
释放互斥锁
- 
尝试获取互斥锁
## 初始化互斥锁
使用 [mutex_init(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrhb?a=view) 可以初始化 mp 所指向的互斥锁。对于 POSIX 线程，请参见[初始化互斥锁](http://docs.sun.com/app/docs/doc/819-7051/6n919hpag?a=view#sync-112)。 
### mutex_init(3C) 语法
#include <synch.h> 
#include <thread.h>
int mutex_init(mutex_t *mp, int type, void *arg)); 
type 可以是以下值之一： 
- 
USYNC_PROCESS。互斥锁可用来同步此进程和其他进程中的线程。arg 会被忽略。 
- 
USYNC_PROCESS_ROBUST。可使用互斥来在此进程和其他进程中**可靠地**同步线程。arg 会被忽略。 
- 
USYNC_THREAD。互斥锁可用来仅同步此进程中的线程。arg 会被忽略。
如果进程在持有 USYNC_PROCESS 锁时失败，该锁以后的请求者都将挂起。对于与客户机进程共享锁的系统，此行为会产生问题，因为客户机进程会异常中止。为了避免出现停用进程所持有的锁定挂起的问题，请使用 USYNC_PROCESS_ROBUST 来锁定互斥锁。USYNC_PROCESS_ROBUST 增加了两个功能：
- 
如果进程停用，则系统将解除锁定该进程所持有的全部锁。
- 
请求失败进程所拥有的任何锁的下一个请求者将获取该锁。但是，拥有该锁时会返回一个错误，指明以前的属主在拥有该锁时失败。 
互斥锁还可以通过在清零的内存中进行分配来初始化，在这种情况下假定 type 为 USYNC_THREAD。
多个线程决不能同时初始化同一个互斥锁。如果其他线程正在使用互斥锁，则不得将该互斥锁重新初始化。
#### 进程内的互斥锁
#include <thread.h>
mutex_t mp;
int ret;
/* to be used within this process only */
ret = mutex_init(&mp, USYNC_THREAD, 0); 
#### 进程间的互斥锁
#include <thread.h>
mutex_t mp;
int ret;
/* to be used among all processes */
ret = mutex_init(&mp, USYNC_PROCESS, 0); 
#### 强健的进程间互斥
#include <thread.h>
mutex_t mp;
int ret;
/* to be used among all processes */
ret = mutex_init(&mp, USYNC_PROCESS_ROBUST, 0); 
### mutex_init 返回值
**mutex_init()** 在成功运行后返回 0。如果检测到以下任一情况，**mutex_init()** 将失败并返回对应的值。
EFAULT
**描述: **
mp 指向的地址非法。
EINVAL
**描述: **
mp 指定的值无效。
ENOMEM
**描述: **
系统内存不足，无法初始化互斥锁。
EAGAIN
**描述: **
系统资源不足，无法初始化互斥锁。
EBUSY
**描述: **
系统检测到重新初始化活动互斥锁的尝试。
## 销毁互斥锁
使用 [mutex_destroy(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrha?a=view) 可以销毁与 mp 所指向的互斥锁相关联的任何状态。用来存储该互斥锁的空间不会释放。对于 POSIX 线程，请参见[pthread_mutex_destroy 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpag?a=view#sync-20140)。
### mutex_destroy 语法
#include <thread.h>
int mutex_destroy (mutex_t *mp);
### mutex_destroy 返回值
**mutex_destroy()** 在成功运行后返回 0。如果检测到以下情况，**mutex_destroy()** 将失败并返回对应的值。
EFAULT
**描述: **
mp 指向的地址非法。
## 获取互斥锁
使用 [mutex_lock(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrhc?a=view) 可以锁定 mp 所指向的互斥锁。如果该互斥锁已经锁定，调用线程将会阻塞，直到该互斥锁成为可用为止。调用线程会在具有优先级的队列中等待。对于 POSIX 线程，请参见[pthread_mutex_lock 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpag?a=view#sync-117)。
### mutex_lock 语法
#include <thread.h>
int mutex_lock(mutex_t *mp);
### mutex_lock 返回值
**mutex_lock()** 在成功运行后返回 0。如果检测到以下任一情况，**mutex_lock()** 将失败并返回对应的值。
EFAULT
**描述: **
mp 指向的地址非法。
EDEADLK
**描述: **
互斥锁已经锁定并且由调用线程拥有。
## 释放互斥锁
使用 [mutex_unlock(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrhe?a=view) 可以解除锁定 mp 所指向的互斥锁。该互斥锁必须锁定。调用线程必须是最后一个锁定该互斥锁的线程，即该互斥锁的属主。对于 POSIX 线程，请参见[pthread_mutex_unlock 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpag?a=view#sync-61163)。
### mutex_unlock 语法
#include <thread.h>
int mutex_unlock(mutex_t *mp);
### mutex_unlock 返回值
**mutex_unlock()** 在成功运行后返回 0。如果检测到以下任一情况，**mutex_unlock()** 将失败并返回对应的值。
EFAULT
**描述: **
mp 指向的地址非法。
EPERM
**描述: **
调用线程不拥有该互斥锁。
## 尝试获取互斥锁
使用 [mutex_trylock(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrhd?a=view) 可以尝试锁定 mp 所指向的互斥锁。此函数是 **mutex_lock()** 的非阻塞版本。对于 POSIX 线程，请参见[pthread_mutex_trylock 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpag?a=view#sync-24727)。
### mutex_trylock 语法
#include <thread.h>
int mutex_trylock(mutex_t *mp);
### mutex_trylock 返回值
**mutex_trylock()** 在成功运行后返回 0。如果检测到以下任一情况，**mutex_trylock()** 将失败并返回对应的值。
EFAULT
**描述: **
mp 指向的地址非法。
EBUSY
**描述: **
系统检测到重新初始化活动互斥锁的尝试。
# 相似的同步函数：条件变量
- 
初始化条件变量
- 
销毁条件变量
- 
等待条件
- 
等待绝对时间
- 
等待时间间隔
- 
解除阻塞一个线程
- 
解除阻塞所有线程
## 初始化条件变量
使用 [cond_init(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hr5a?a=view) 可以初始化 **cv** 所指向的条件变量。
### cond_init 语法
#include <thread.h>
int cond_init(cond_t *cv, int type, int arg);
type 可以是以下值之一：
- 
USYNC_PROCESS。条件变量可用来同步此进程和其他进程中的线程。arg 会被忽略。 
- 
USYNC_THREAD。条件变量可用来仅同步此进程中的线程。arg 会被忽略。 
条件变量还可以通过在清零的内存中进行分配来初始化，在这种情况下假设 type 为 USYNC_THREAD。 
多个线程决不能同时初始化同一个条件变量。对于其他线程可能正在使用的条件变量，不得重新初始化。
对于 POSIX 线程，请参见[pthread_condattr_init 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpah?a=view#sync-72041)。
#### 进程内条件变量
#include <thread.h>
cond_t cv;
int ret;
/* to be used within this process only */
ret = cond_init(cv, USYNC_THREAD, 0); 
#### 进程间条件变量
#include <thread.h>
cond_t cv;
int ret;
/* to be used among all processes */
ret = cond_init(&cv, USYNC_PROCESS, 0); 
### cond_init 返回值
**cond_init()** 在成功运行后返回 0。如果检测到以下任一情况，**cond_init()** 将失败并返回对应的值。
EFAULT
**描述: **
cv 指向的地址非法。
EINVAL
**描述: **
type 不是可以识别的类型。
## 销毁条件变量
使用 [cond_destroy(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hr59?a=view) 可以销毁与 cv 所指向的条件变量相关联的状态。用来存储该条件变量的空间不会释放。对于 POSIX 线程，请参见[pthread_condattr_destroy 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpah?a=view#sync-64435)。
### cond_destroy 语法
#include <thread.h>
int cond_destroy(cond_t *cv);
### cond_destroy 返回值
**cond_destroy()** 在成功运行后返回 0。如果检测到以下任一情况，**cond_destroy()** 将失败并返回对应的值。
EFAULT
**描述: **
cv 指向的地址非法。
EBUSY
**描述: **
系统检测到销毁活动条件变量的尝试。
## 等待条件
使用 [cond_wait(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hr5e?a=view) 可以原子方式释放 **mp** 所指向的互斥锁，并导致调用线程基于 **cv** 所指向的条件变量阻塞。阻塞的线程可以由 **cond_signal()** 或 **cond_broadcast()** 唤醒，也可以在信号传送或 **fork()** 将其中断时唤醒。
**cond_wait()** 每次返回时，互斥锁均处于锁定状态并由调用线程拥有，即使返回错误时也是如此。
### cond_wait 语法
#include <thread.h>
int cond_wait(cond_t *cv, mutex_t *mp);
### cond_wait 返回值
**cond_wait()** 在成功运行后返回 0。如果检测到以下任一情况，**cond_wait()** 将失败并返回对应的值。
EFAULT
**描述: **
cv 指向的地址非法。
EBUSY
**描述: **
等待过程已被信号或 **fork()** 中断。
## 等待绝对时间
[cond_timedwait(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hr5d?a=view) 与 **cond_wait()** 非常相似，区别在于 **cond_timedwait()** 经过 abstime 指定的时间之后不会阻塞。对于 POSIX 线程，请参见[pthread_cond_timedwait 语法](http://docs.sun.com/app/docs/doc/819-7051/6n919hpai?a=view#sync-46756)。
### cond_timedwait 语法
#include <thread.h>
int cond_timedwait(cond_t *cv, mutex_t *mp, timestruct_t abstime);
**cond_timedwait()** 每次返回时，互斥锁均会锁定并由调用线程拥有，即使返回错误时也是如此。
**cond_timedwait()** 函数会一直阻塞，直到该条件获得信号，或者经过最后一个参数所指定的时间为止。超时以具体的时间指定，这样即可在不重新计算超时值的情况下高效地重新测试条件。
### cond_timedwait 返回值
**cond_timedwait()** 在成功运行后返回 0。如果检测到以下任一情况，**cond_timedwait()** 将失败并返回对应的值。
EFAULT
**描述: **
cv 指向的地址非法。
ETIME
**描述: **
由 abstime 指定的时间已过期。
EINVAL
**描述: **
abstime 无效。
## 等待时间间隔
[cond_reltimedwait(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hr5b?a=view) 与 **cond_timedwait()** 非常相似，区别在于第三个参数的值不同。**cond_reltimedwait()** 的第三个参数采用相对时间间隔值，而不是绝对时间值。对于 POSIX 线程，请参见 pthread_cond_reltimedwait_np(3C) 手册页。
**cond_reltimedwait()** 每次返回时，互斥锁均会锁定并由调用线程拥有，即使返回错误时也是如此。**cond_reltimedwait()** 函数一直阻塞，直到该条件获得信号，或者经过最后一个参数所指定的时间间隔为止。
### cond_reltimedwait 语法
#include <thread.h>
int cond_reltimedwait(cond_t *cv, mutex_t *mp,
    timestruct_t reltime);
### cond_reltimedwait 返回值
**cond_reltimedwait()** 在成功运行后返回 0。如果检测到以下任一情况，**cond_reltimedwait()** 将失败并返回对应的值。
EFAULT
**描述: **
cv 指向的地址非法。
ETIME
**描述: **
由 reltime 指定的时间已过期。
## 解除阻塞一个线程
对于基于 cv 所指向的条件变量阻塞的线程，使用 [cond_signal(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hr5c?a=view) 可以解除阻塞该线程。如果没有线程基于该条件变量阻塞，则调用 **cond_signal()** 不起任何作用。
### cond_signal 语法
#include <thread.h>
int cond_signal(cond_t *cv);
### cond_signal 返回值
**cond_signal()** 在成功运行后返回 0。如果检测到以下情况，**cond_signal()** 将失败并返回对应的值。
EFAULT
**描述: **
cv 指向的地址非法。
## 解除阻塞所有线程
对于基于 cv 所指向的条件变量阻塞的全部线程，使用 [cond_broadcast(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hr58?a=view) 可以解除阻塞这些线程。如果没有线程基于该条件变量阻塞，则调用 **cond_broadcast()** 不起任何作用。 
### cond_broadcast 语法
#include <thread.h>
int cond_broadcast(cond_t *cv);
### cond_broadcast 返回值
**cond_broadcast()** 在成功运行后返回 0。如果检测到以下情况，**cond_broadcast()** 将失败并返回对应的值。
EFAULT
**描述: **
cv 指向的地址非法。
# 相似的同步函数：信号
信号操作在 Solaris 操作环境和 POSIX 环境中均相同。 Solaris 操作环境中的函数名 sema_ 在 pthread 中会更改为 sem_。本节讨论了以下主题：
- 
初始化信号
- 
增加信号
- 
基于信号计数阻塞
- 
减小信号计数
- 
销毁信号状态
## 初始化信号
使用 [sema_init(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrq3?a=view) 可以通过 count 值来初始化 sp 所指向的信号变量。 
### sema_init 语法
#include <thread.h>
int sema_init(sema_t *sp, unsigned int count, int type,
    void *arg);
type 可以是以下值之一：
- 
USYNC_PROCESS。信号可用来同步此进程和其他进程中的线程。信号只能由一个进程来初始化。arg 会被忽略。 
- 
USYNC_THREAD。信号可用来仅同步此进程中的线程。arg 会被忽略。 
多个线程决不能同时初始化同一个信号。不得对其他线程正在使用的信号重新初始化。
#### 进程内信号
#include <thread.h>
sema_t sp;
int ret;
int count;
count = 4;
/* to be used within this process only */
ret = sema_init(&sp, count, USYNC_THREAD, 0); 
#### 进程间信号
#include <thread.h>
sema_t sp;
int ret;
int count;
count = 4;
/* to be used among all the processes */
ret = sema_init (&sp, count, USYNC_PROCESS, 0); 
### sema_init 返回值
**sema_init()** 在成功运行后返回 0。如果检测到以下任一情况，**sema_init()** 将失败并返回对应的值。
EINVAL
**描述: **
sp 引用的信号无效。
EFAULT
**描述: **
sp 或 arg 指向的地址非法。
## 增加信号
使用 [sema_post(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrq5?a=view) 可以原子方式增加 sp 所指向的信号。如果多个线程基于该信号阻塞，则系统会解除阻塞其中一个线程。
### sema_post 语法
#include <thread.h>
int sema_post(sema_t *sp);
### sema_post 返回值
**sema_post()** 在成功运行后返回 0。如果检测到以下任一情况，**sema_post()** 将失败并返回对应的值。
EINVAL
**描述: **
sp 引用的信号无效。
EFAULT
**描述: **
sp 指向的地址非法。
EOVERFLOW
**描述: **
sp 指向的信号值超过了 SEM_VALUE_MAX。
## 基于信号计数阻塞
使用 [sema_wait(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrq7?a=view) 可以一直阻塞调用线程，直到 sp 所指向的信号的计数变得大于零为止。计数变得大于零时，系统会以原子方式减小计数。
### sema_wait 语法
#include <thread.h>
int sema_wait(sema_t *sp);
### sema_wait 返回值
**sema_wait()** 在成功运行后返回 0。如果检测到以下任一情况，**sema_wait()** 将失败并返回对应的值。
EINVAL
**描述: **
sp 引用的信号无效。
EINTR
**描述: **
等待过程已被信号或 **fork()** 中断。
## 减小信号计数
使用 [sema_trywait(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrq6?a=view) 可以在计数大于零时，以原子方式减小 sp 所指向的信号的计数。此函数是 **sema_wait()** 的非阻塞版本。 
### sema_trywait 语法
#include <thread.h>
int sema_trywait(sema_t *sp);
### sema_trywait 返回值
**sema_trywait()** 在成功运行后返回 0。如果检测到以下任一情况，**sema_trywait()** 将失败并返回对应的值。
EINVAL
**描述: **
sp 指向的信号无效。
EBUSY
**描述: **
sp 所指向的信号的计数为零。
## 销毁信号状态
使用 [sema_destroy(3C)](http://docs.sun.com/app/docs/doc/816-5168/6mbb3hrq2?a=view) 可以销毁与 sp 所指向的信号相关联的任何状态。不会释放用来存储该信号的空间。
### sema_destroy(3C) 语法
#include <thread.h>
int sema_destroy(sema_t *sp);
### sema_destroy(3C) 返回值
**sema_destroy()** 在成功运行后返回 0。如果检测到以下情况，**sema_destroy()** 将失败并返回对应的值。
EINVAL
**描述: **
sp 指向的信号无效。
# 跨进程边界同步
每个同步元语都可以设置为跨进程边界使用。可通过以下方法来设置跨边界同步：确保同步变量位于共享内存段中，并在 type 设置为 USYNC_PROCESS 的情况下调用相应的 init 例程。 
如果 type 设置为 USYNC_PROCESS，则针对同步变量执行的操作与 **type** 为 USYNC_THREAD 时针对变量执行的操作相同。 
mutex_init(&m, USYNC_PROCESS, 0);
rwlock_init(&rw, USYNC_PROCESS, 0);
cond_init(&cv, USYNC_PROCESS, 0);
sema_init(&s, count, USYNC_PROCESS, 0);
## 生成方和使用者问题示例
[示例 8–2](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbq?a=view#sthreads-ex-31) 说明了生成方和使用者位于不同进程时的生成方和使用者问题。主例程将与其子进程共享的全零内存段映射到其地址空间。请注意，必须调用 **mutex_init()** 和 **cond_init()**，因为同步变量的 type 为 USYNC_PROCESS。 
创建子进程是为了运行使用者，父进程则运行生成方。
此示例还说明了生成方和使用者的驱动程序。producer_driver 从 stdin 读取字符并调用 producer。consumer_driver 通过调用 consumer 来获取字符并将这些字符写入 stdout 中。
[示例 8–2](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbq?a=view#sthreads-ex-31) 的数据结构与用于解析条件变量的数据结构相同。请参见[嵌套锁定和单链接列表的结合使用示例](http://docs.sun.com/app/docs/doc/819-7051/6n919hpag?a=view#sync-83550)。
##### 示例 8–2 使用 USYNC_PROCESS 时的生成方和使用者问题
main() {
    int zfd;
    buffer_t *buffer;
    zfd = open(“/dev/zero”, O_RDWR);
    buffer = (buffer_t *)mmap(NULL, sizeof(buffer_t),
        PROT_READ|PROT_WRITE, MAP_SHARED, zfd, 0);
    buffer->occupied = buffer->nextin = buffer->nextout = 0;
    mutex_init(&buffer->lock, USYNC_PROCESS, 0);
    cond_init(&buffer->less, USYNC_PROCESS, 0);
    cond_init(&buffer->more, USYNC_PROCESS, 0);
    if (fork() == 0)
        consumer_driver(buffer);
    else
        producer_driver(buffer);
}
void producer_driver(buffer_t *b) {
    int item;
    while (1) {
        item = getchar();
        if (item == EOF) {
            producer(b, `\0');
            break;
        } else
            producer(b, (char)item);
    }
}
void consumer_driver(buffer_t *b) {
    char item;
    while (1) {
        if ((item = consumer(b)) == '\0')
            break;
        putchar(item);
    }
}
创建子进程是为了运行使用者，父进程则运行生成方。
# **fork()** 和 Solaris 线程的特殊问题
在 Solaris 10 发行版之前，Solaris 线程和 POSIX 线程以不同的方式定义 **fork()** 的行为。有关 **fork()** 问题的详细讨论，请参见[进程创建：exec 和 exit 问题](http://docs.sun.com/app/docs/doc/819-7051/6n919hpar?a=view)。
Solaris libthread 同时支持 **fork()** 和 **fork1()**。**fork()** 调用具有 "Fork-All" 语义。**fork()** 可用来复制进程中的所有内容（包括线程和 LWP），从而创建父进程的准确克隆。**fork1()** 调用所创建的克隆中仅有一个线程，它可复制进程状态和地址空间，但是仅克隆调用线程。
POSIX libpthread 仅支持 **fork()**，该函数与 Solaris 线程中的 **fork1()** 具有相同语义。
**fork()** 具有 "Fork-All" 语义还是 "Fork-One"语义取决于所使用的库。使用 **-lthread** 进行链接可以赋予 **fork()** "Fork-All" 语义；使用 **-lpthread** 进行链接可以赋予 **fork()** "Fork-One"语义。
从 Solaris 10 发行版开始，**fork()** 在 Solaris 线程和 POSIX 线程中具有相同的语义。具体来说，**fork1()** 语义仅复制调用方。对于需要使用“复制全部”语义的应用程序，提供了一个新函数 **forkall()**。 
有关更多详细信息，请参见[使用 libthread 或 libpthread 链接](http://docs.sun.com/app/docs/doc/819-7051/6n919hpbe?a=view#compile-94179)。
