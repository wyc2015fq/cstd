# 高效线程池之无锁化实现(Linux C) - DoubleLi - 博客园






笔者之前照着通用写法练手写过一个小的线程池版本，最近几天复习了一下，发现大多数线程池实现都离不开锁的使用，如互斥量pthread_mutex*结合条件变量pthread_cond*。众所周知，锁的使用对于程序性能影响较大，虽然现有的pthread_mutex*在锁的申请与释放方面做了较大的优化，但仔细想想，线程池的实现是可以做到无锁化的，于是有了本文。

### 1.常见线程池实现原理

![](http://mmbiz.qpic.cn/mmbiz_jpg/Izj5340ib4qMiblvn7HH9DJ7M4ibWRpfMJyvibS7SrPCrdwKhDr9Ro5Ona7yoRicgEibCbNdVKlhw8ghZNWdpt8k2oDQ/0?wx_fmt=jpeg)

如上图所示，工作队列由主线程和工作者线程共享，主线程将任务放进工作队列，工作者线程从工作队列中取出任务执行。共享工作队列的操作需在互斥量的保护下安全进行，主线程将任务放进工作队列时若检测到当前待执行的工作数目小于工作者线程总数，则需使用条件变量唤醒可能处于等待状态的工作者线程。当然，还有其他地方可能也会使用到互斥量和条件变量，不再赘述。

### 2.无锁化线程池实现原理

![](http://mmbiz.qpic.cn/mmbiz_jpg/Izj5340ib4qMiblvn7HH9DJ7M4ibWRpfMJypzn0GuMZmm99iaHe61ibNJLmr7WGxbdaAabzg7Bz13pYzxexSsdKduLA/0?wx_fmt=jpeg)

为解决无锁化的问题，需要避免共享资源的竞争，因此将共享工作队列加以拆分成每工作线程一个工作队列的方式。对于主线程放入工作和工作线程取出任务的竞争问题，可以采取环形队列的方式避免。在解决了锁机制之后，就只剩下条件变量的问题了，条件变量本身即解决条件满足时的线程通信问题，而信号作为一种通信方式，可以代替之，其大体编程范式为：

```
sigemptyset (&oldmask);
sigemptyset (&signal_mask);
sigaddset (&signal_mask, SIGUSR1);
rc = pthread_sigmask(SIG_BLOCK, &signal_mask, NULL);
if (rc != 0) {
    debug(TPOOL_ERROR, "SIG_BLOCK failed");
    return -1;
}
...

while (!condition) {
    rc = sigwait (&signal_mask, NULL);
    if (rc != 0) {
        debug(TPOOL_ERROR, "sigwait failed");
        return -1;
    }
}

rc = pthread_sigmask(SIG_SETMASK, &oldmask, NULL);
if (rc != 0) {
    debug(TPOOL_ERROR, "SIG_SETMASK failed");
    return -1;
}
```

### 3.无锁化线程池具体实现

在无锁线程池中，区别于常见线程池的地方主要在于信号与条件变量、任务调度算法、增加或减少线程数目后的任务迁移，另外还有一点就是环形队列的实现参考了Linux内核中的kfifo实现。

(1)   信号与条件变量

信号与条件变量的区别主要在于条件变量的唤醒(signal)对于接收线程而言可以忽略，而在未设置信号处理函数的情况下信号的接收会导致接收线程甚至整个程序的终止，因此需要在线程池产生线程之前指定信号处理函数，这样新生的线程会继承这个信号处理函数。多线程中信号的发送主要采用pthread_kill，为避免使用其他信号，本程序中使用了SIGUSR1。

(2)   任务调度算法

常见线程池实现的任务调度主要在操作系统一级通过线程调度实现。考虑到负载均衡，主线程放入任务时应采取合适的任务调度算法将任务放入对应的工作者线程队列，本程序目前已实现Round-Robin和Least-Load算法。Round-Robin即轮询式地分配工作，Least-Load即选择当前具有最少工作的工作者线程放入。

(3)   任务迁移

在线程的动态增加和减少的过程中，同样基于负载均衡的考量，涉及到现有任务的迁移问题。负载均衡算法主要基于平均工作量的思想，即统计当前时刻的总任务数目，均分至每一个线程，求出每个工作者线程应该增加或减少的工作数目，然后从头至尾遍历，需要移出工作的线程与需要移入工作的线程执行任务迁移，相互抵消。最后若还有多出来的工作，再依次分配。迁入工作不存在竞态，因为加入工作始终由主线程完成，而迁出工作则存在竞态，因为在迁出工作的同时工作者线程可能在同时执行任务。所以需要采用原子操作加以修正，其主要思想即预取技术，大致实现为：

```
do {
    work = NULL;
    if (thread_queue_len(thread) <= 0)  //also atomic
        break;
    tmp = thread->out;
    //prefetch work
    work = &thread->work_queue[queue_offset(tmp)];
} while (!__sync_bool_compare_and_swap(&thread->out, tmp, tmp + 1));
if (work) {
    // do something
```
在线程的动态减少后，原先线程上未能执行完的任务只需要由
    //主线程再次根据任务调度算法重新分配至其他存活的工作者线程队列中即可，不
    //存在上述问题，当然，此时可以同时执行负载均衡算法加以优化。
}
(4)   环形队列

源码中环形队列实现主要参考了linux内核中kfifo的实现，如下图所示：

![](http://mmbiz.qpic.cn/mmbiz_jpg/Izj5340ib4qMiblvn7HH9DJ7M4ibWRpfMJy3gXv9WXLqygqf6icxS9J4ibcR834pV7JLTOLfibib47uHCL0TfvIyUUg2g/0?wx_fmt=jpeg)

队列长度为2的整次幂，out和in下标一直递增至越界后回转，其类型为unsigned int，即out指针一直追赶in指针，out和in映射至FiFo的对应下标处，其间的元素即为队列元素。

以上主要是一些方案性的说明，至于具体细节的实现有兴趣的读者可以参考https://github.com/xhjcehust/LFTPool，有问题欢迎随时联系讨论.

**注:本文两年前曾发布在http://blog.csdn.net/xhjcehust/article/details/45844901，最近有读者反馈一些问题，也修复了一些bug，再次分享，希望更多的读者能有收获。**

**from:https://mp.weixin.qq.com/s?__biz=MzIxNzg5ODE0OA==&mid=2247483699&idx=1&sn=6b8bebc33525bddd167a90a3119b3873&chksm=97f38cf8a08405eed3783f122dba8f07ea78b42fd7c08d0011adf3ca00b9cf5140cb7a6567f7#rd**









