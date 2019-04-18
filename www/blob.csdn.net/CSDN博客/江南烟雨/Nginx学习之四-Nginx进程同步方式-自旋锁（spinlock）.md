# Nginx学习之四-Nginx进程同步方式-自旋锁（spinlock） - 江南烟雨 - CSDN博客
2013年07月02日 10:27:31[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：7000
所属专栏：[Nginx模块开发与原理剖析](https://blog.csdn.net/column/details/nginxroad.html)
自旋锁简介
Nginx框架使用了三种消息传递方式：共享内存、套接字、信号。
Nginx主要使用了三种同步方式：原子操作、信号量、文件锁。
基于原子操作，nginx实现了一个自旋锁。自旋锁是一种非睡眠锁。如果某进程视图获得自旋锁，当发现锁已经被其他进程获得时，那么不会使得当前进程进入睡眠状态，而是始终保持进程在可执行状态，每当内核调度到这个进程执行时就持续检查是否可以获取到所锁。
自旋锁的应用场景
自旋锁主要是为多处理器操作系统而设置的，他要解决的共享资源保护场景就是进程使用锁的时间非常短（如果锁的使用时间很久，自旋锁不合适，因为会占用大量的CPU资源）。
大部分情况下Nginx的worker进程最好都不要进入睡眠状态，因为它非常繁忙，在这个进程的epoll上可能有十万甚至百万的TCP连接等待着处理，进程一旦睡眠后必须等待其他事件的唤醒，这中间及其频繁的进程间切换带来的负载消耗可能无法让用户接受。
自旋锁源码分析
下面通过源代码看自旋锁的具体实现（Nginx_spinlock.c，Nginx1.4.1版本）：
```cpp
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */
#include <ngx_config.h>
#include <ngx_core.h>
//函数：基于原子操作的自旋锁方法ngx_spinlock的实现
//参数解释：
//lock：原子变量表达的锁
//value：标志位，锁是否被某一进程占用
//spin：在多处理器系统内，当ngx_spinlock方法没有拿到锁时，当前进程在内核的一次调度中该方法等待其他处理器释放锁的时间
void
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{
#if (NGX_HAVE_ATOMIC_OPS)//支持原子操作
    ngx_uint_t  i, n;
    //一直处于循环中，直到获取到锁
    for ( ;; ) {
        //lock为0表示没有其他进程持有锁，这时将lock值设置为value参数表示当前进程持有了锁
        if (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            return;
        }
        //如果是多处理器系统
        if (ngx_ncpu > 1) {
            for (n = 1; n < spin; n <<= 1) {
                //随着等待的次数越来越多，实际去检查锁的间隔时间越来越大
                for (i = 0; i < n; i++) {
                    ngx_cpu_pause();//告诉CPU现在处于自旋锁等待状态
                }
                //检查锁是否被释放
                if (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    return;
                }
            }
        }
        //当前进程让出处理器，但仍然处于可执行状态
        ngx_sched_yield();
    }
#else
#if (NGX_THREADS)
#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !
#endif
#endif
}
```
上面的代码需要注意的是：在多处理器下，当发现锁被其他进程占用时，当前进程并不是立刻让出正在使用的CPU处理器，而是等待一段时间，看看其他处理器上的进程是否会释放锁，这会减少进程间切换的次数。
函数ngx_cpu_pause()：是虚度哦架构体系中专门为了自旋锁而提供的指令，它会告诉CPU现在处于自旋锁等待状态，通常一个CPU会将自己置于节能状态，降低功耗。但是当前进程并没有让出正在使用的处理器。
函数 ngx_sched_yield()：当前进程仍仍然处于可执行状态，但暂时让出处理器，使得处理器优先调度其他可执行状态的进程，这样，在进程被内核再次调度时，在for循环代码中可以期望其他进程释放锁。
