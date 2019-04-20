# 线程的取消/撤销（cancel）(转) - h13 - 博客园
**1 线程取消的定义**
一般情况下，线程在其主体函数退出的时候会自动终止，但同时也可以因为接收到另一个线程发来的终止（取消）请求而强制终止。
**2 线程取消的语义**
1. 
线程取消的方法是向目标线程发Cancel信号，但如何处理Cancel信号则由目标线程自己决定，或者忽略（当禁止取消时）、或者立即终止（当在取消点或异步模式下）、或者继续运行至Cancelation-point（取消点，下面将描述），总之由不同的Cancelation状态决定。
2. 
线程接收到CANCEL信号的缺省处理（即pthread_create()创建线程的缺省状态）是继续运行至取消点再处理（退出），或在异步方式下直接退出。一个线程处理cancel请求的退出操作相当于pthread_exit(PTHREAD_CANCELED)。当然线程可以通过设置为PTHREAD_CANCEL_DISABLE来拒绝处理cancel请求，稍后会提及。
**3. 线程的取消与线程的工作方式（joinable或detached）无关。**
**3 取消点**
根据POSIX标准，pthread_join()、pthread_testcancel()、pthread_cond_wait()、 
pthread_cond_timedwait()、sem_wait()、sigwait()等函数以及read()、write()等会引起阻塞的系统调用都是Cancelation-point，而其他pthread函数都不会引起Cancelation动作。
但是pthread_cancel的手 
册页声称，由于LinuxThread库与C库结合得不好，因而**目前C库函数（比如read()）在linux种都不是Cancelation-point**；但CANCEL信号会使线程从阻 
塞的系统调用中退出，并置EINTR错误码，因此可以在需要作为Cancelation-point的系统调用前后调用 
pthread_testcancel()，从而达到POSIX标准所要求的目标，即如下代码段：
```
pthread_testcancel();
              retcode = read(fd, buffer, length);
              pthread_testcancel();
```
**4 程序设计方面的考虑**
1. 
如果线程处于无限循环中，且循环体内没有执行至取消点的必然路径，则线程无法由外部其他线程的取消请求而终止。因此**在这样的循环体的必经路径上应该加入pthread_testcancel()调用**。
2. 
当pthread_cancel()返回时，线程未必已经取消，可能仅仅将请求发送给目标线程，而目标线程目前没有到达取消点，如果要知道线程在何时中止，就需要在取消它之后调用pthread_join()。有一个例外是当线程被detach后，不能这样处理：
           
a) 当join一个已经detached的线程时，返回**EINVAL**；
           b) 
如果join后该线程设置为detached，则detach将不起作用。
因此，**如果知道一个线程可能会以分离方式运行，就不需要在pthread_cancel()后调用pthread_join()**。
**5 与线程取消相关的pthread函数**
int pthread_cancel(pthread_t thread) 
发送终止信号给thread线程，如果成功则返回0，否则为非0值。发送成功并不意味着thread会终止。
int** pthread_setcancelstate**(int state, int *oldstate) 
设置本线程对Cancel信号的反应，state有两种值：PTHREAD_CANCEL_ENABLE（缺省）和 
PTHREAD_CANCEL_DISABLE，分别表示收到信号后设为CANCLED状态和忽略CANCEL信号继续运行；old_state如果不为 
NULL则存入原来的Cancel状态以便恢复。
int **pthread_setcanceltype**(int type, int *oldtype) 
设置本线程取消动作的执行时机，type由两种取值：PTHREAD_CANCEL_DEFFERED和 
PTHREAD_CANCEL_ASYCHRONOUS，仅当Cancel状态为Enable时有效，分别表示收到信号后继续运行至下一个取消点再退出和 
立即执行取消动作（退出）；oldtype如果不为NULL则存入运来的取消动作类型值。
void **pthread_testcancel**(void) 
检查本线程是否处于Canceld状态，如果是，则进行取消动作，否则直接返回。
[http://hi.baidu.com/ganss/blog/item/ff7799f97a87de58242df24c.html](http://hi.baidu.com/ganss/blog/item/ff7799f97a87de58242df24c.html)

