# gettid()和pthread_self()的区别 - Koma Hub - CSDN博客
2019年03月09日 15:24:54[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：31
Linux中，每个线程有一个tid，类型long，由sys_gettid()取得。
Linux内核中并没有实现线程，而是由glibc线程库实现的POSIX线程。每个线程也有一个id，类型 pthread_t(unsigned long int)，由pthread_self()取得，该id由线程库维护，其id空间是各个进程独立的（即不同进程中的线程可能有相同的id）。Linux中的POSIX线程库实现的线程在内核中看来也是一个轻量级进程（LWP），但是该进程与主进程（启动线程的进程）共享一些资源，比如代码段，数据段等。
1. gettid()是linux内核实现的函数，在内核看来任何线程也是一个轻量级进程，从下面内核实现的sys_gettid看来，gettid()返回的是内核管理的轻量级进程的进程id。
```cpp
/* Thread ID - the internal kernel "pid" */
asmlinkage long sys_gettid(void)
{
    return current->pid;
}
```
2. pthread_self()在glibc中X86_64平台的实现如下：
```cpp
pthread_t __pthread_self (void)
{
    return (pthread_t) THREAD_SELF;
}
/* Return the thread descriptor for the current thread.
The contained asm must *not* be marked volatile since otherwise
assignments like
pthread_descr self = thread_self();
do not get optimized away. */
# define THREAD_SELF \
    ({ struct pthread *__self; \
    asm ("movq %%fs:%c1,%q0" : "=r" (__self) \
        : "i" (offsetof (struct pthread, header.self))); \
        __self;})
```
从上面代码我们可以知道__pthread_self 得到实际上是线程描述符pthread 指针地址。
从上面我们可以得知，gettid()是内核给线程（轻量级进程）分配的进程id，全局（所有进程中）唯一；pthread_self()是在用户态实现的，获取的id实际上是主线程分配给子线程的线程描述符的地址而已，只是在当前进程空间中是唯一的。
转载自：[https://www.cnblogs.com/jaydenhpj/p/5200062.html](https://www.cnblogs.com/jaydenhpj/p/5200062.html)
