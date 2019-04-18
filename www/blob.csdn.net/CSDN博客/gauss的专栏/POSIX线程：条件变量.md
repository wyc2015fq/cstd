# POSIX线程：条件变量 - gauss的专栏 - CSDN博客
2013年01月04日 22:30:41[gauss](https://me.csdn.net/mathlmx)阅读数：268
个人分类：[多线程](https://blog.csdn.net/mathlmx/article/category/954554)
条件变量是 POSIX 线程结构，可以让您在遇到某些条件时“唤醒”线程。可以将它们看作是一种线程安全的信号发送。
如果线程正在等待某个特定条件发生，它应该如何处理这种情况？它可以重复对互斥对象锁定和解锁，每次都会检查共享数据结构，以查找某个值。但这是在浪费时间和资源，而且这种繁忙查询的效率非常低。解决这个问题的最佳方法是使用 pthread_cond_wait() 调用来等待特殊条件发生。了解 pthread_cond_wait() 的作用非常重要；它是 POSIX 线程信号发送系统的核心，也是最难以理解的部分。
首先，让我们考虑以下情况：线程为查看链表而锁定了互斥对象，然而该链表恰巧是空的。这一特定线程什么也干不了：其设计意图是从链表中除去节点，但是现在却没有节点。因此，它只能：
锁定互斥对象时，线程将调用 pthread_cond_wait(&cond, &mutex)。pthread_cond_wait() 调用相当复杂，因此我们每次只执行它的一个操作。
pthread_cond_wait() 所做的第一件事就是**同时**对互斥对象解锁（于是其它线程可以修改链表），并等待条件 cond 发生（这样当 pthread_cond_wait() 接收到另一个线程的“信号”时，它将苏醒）。现在互斥对象已被解锁，其它线程可以访问和修改链表，可能还会添加项。
此 时，pthread_cond_wait() 调用还未返回。对**互斥对象解锁会立即发生，但等待条件cond 通常是一个阻塞操作**，这意味着线程将睡眠，在它苏醒之前不会消耗 CPU 周期。这正是我们期待发生的情况。线程将一直睡眠，直到特定条件发生，在这期间不会发生任何浪费 CPU 时间的繁忙查询。从线程的角度来看，它只是在等待 pthread_cond_wait() 调用返回。
现在继续说明，假设另一个线程（称作“2 号线程”）锁定了mutex 并对链表添加了一项。在对互斥对象解锁之后，2 号线程会立即调用函数 pthread_cond_broadcast(&cond)。此操作之后，2 号线程将使所有等待 cond 条件变量的线程立即苏醒。这意味着第一个线程（仍处于 pthread_cond_wait() 调用中）现在将苏醒。
现 在，看一下第一个线程发生了什么。您可能会认为在 2 号线程调用 pthread_cond_broadcast(&cond) 之后，1 号线程的 pthread_cond_wait() 会立即返回。不是那样的！实际上，pthread_cond_wait() 将执行最后一个操作：重新锁定 mutex。一旦 pthread_cond_wait() 锁定了互斥对象，那么它将返回并允许 1 号线程继续执行。那时，它可以马上检查链表，查看它所感兴趣的更改。
整个过程执行情况如下：
第一个线程首先调用：
**[cpp]**[view plain](http://blog.csdn.net/dipperkun/article/details/6312588#)[copy](http://blog.csdn.net/dipperkun/article/details/6312588#)[print](http://blog.csdn.net/dipperkun/article/details/6312588#)[?](http://blog.csdn.net/dipperkun/article/details/6312588#)
- pthread_mutex_lock(&mutex);  
然后，它检查了链表。没有找到感兴趣的东西，于是它调用：
**[cpp]**[view plain](http://blog.csdn.net/dipperkun/article/details/6312588#)[copy](http://blog.csdn.net/dipperkun/article/details/6312588#)[print](http://blog.csdn.net/dipperkun/article/details/6312588#)[?](http://blog.csdn.net/dipperkun/article/details/6312588#)
- pthread_cond_wait(&cond, &mutex);  
然后，pthread_cond_wait() 调用在返回前执行许多操作：
**[cpp]**[view plain](http://blog.csdn.net/dipperkun/article/details/6312588#)[copy](http://blog.csdn.net/dipperkun/article/details/6312588#)[print](http://blog.csdn.net/dipperkun/article/details/6312588#)[?](http://blog.csdn.net/dipperkun/article/details/6312588#)
- pthread_mutex_unlock(&mutex);  
它对mutex 解锁，然后进入睡眠状态，等待 cond 以接收 POSIX 线程“信号”。一旦接收到“信号”（加引号是因为我们并不是在讨论传统的 UNIX 信号，而是来自 pthread_cond_signal() 或 pthread_cond_broadcast() 调用的信号），它就会苏醒。但 pthread_cond_wait() 没有立即返回，它还要做一件事：重新锁定 mutex：
**[cpp]**[view plain](http://blog.csdn.net/dipperkun/article/details/6312588#)[copy](http://blog.csdn.net/dipperkun/article/details/6312588#)[print](http://blog.csdn.net/dipperkun/article/details/6312588#)[?](http://blog.csdn.net/dipperkun/article/details/6312588#)
- pthread_mutex_lock(&mutex);  
pthread_cond_wait() 知道我们在查找 mutex “背后”的变化，因此它继续操作，为我们锁定互斥对象，然后才返回。
记住：**在调用 pthread_cond_wait() 之前，pthread_cond_wait() 调用返回之后，互斥对象都是处于锁定状态。**
条件变量是一个需要初始化的真实数据结构。以下就是初始化的方法。首先，定义或分配一个条件变量，如下所示：
**[cpp]**[view plain](http://blog.csdn.net/dipperkun/article/details/6312588#)[copy](http://blog.csdn.net/dipperkun/article/details/6312588#)[print](http://blog.csdn.net/dipperkun/article/details/6312588#)[?](http://blog.csdn.net/dipperkun/article/details/6312588#)
- pthread_cond_t cond;  
然后，调用以下函数进行初始化：
**[cpp]**[view plain](http://blog.csdn.net/dipperkun/article/details/6312588#)[copy](http://blog.csdn.net/dipperkun/article/details/6312588#)[print](http://blog.csdn.net/dipperkun/article/details/6312588#)[?](http://blog.csdn.net/dipperkun/article/details/6312588#)
- pthread_cond_init(&cond, NULL);  
在释放或废弃条件变量之前，需要毁坏它，如下所示：
**[cpp]**[view plain](http://blog.csdn.net/dipperkun/article/details/6312588#)[copy](http://blog.csdn.net/dipperkun/article/details/6312588#)[print](http://blog.csdn.net/dipperkun/article/details/6312588#)[?](http://blog.csdn.net/dipperkun/article/details/6312588#)
- pthread_cond_destroy(&cond);  
一旦初始化了互斥对象和条件变量，就可以等待某个条件，如下所示：
**[cpp]**[view plain](http://blog.csdn.net/dipperkun/article/details/6312588#)[copy](http://blog.csdn.net/dipperkun/article/details/6312588#)[print](http://blog.csdn.net/dipperkun/article/details/6312588#)[?](http://blog.csdn.net/dipperkun/article/details/6312588#)
- pthread_cond_wait(&cond, &mutex);  
请注意，代码在逻辑上应该包含 cond 和 mutex。**一个特定条件只能有一个互斥对象**，而且条件变量应该表示互斥数据“内部”的一种特殊的条件更改。**一个互斥对象可以用于许多条件变量**（例 如，cond_empty、cond_full、cond_cleanup），但每个条件变量只能有一个互斥对象。
对于发送信号和广播，需要注意一点。如果线程更改某些共享数据，而且它想要唤醒所有正在等待的线程，则应使用 pthread_cond_broadcast 调用，如下所示：
**[cpp]**[view plain](http://blog.csdn.net/dipperkun/article/details/6312588#)[copy](http://blog.csdn.net/dipperkun/article/details/6312588#)[print](http://blog.csdn.net/dipperkun/article/details/6312588#)[?](http://blog.csdn.net/dipperkun/article/details/6312588#)
- pthread_cond_broadcast(&cond);  
在某些情况下，活动线程只需要唤醒第一个正在睡眠的线程。假设您只对队列添加了一个工作作业。那么只需要唤醒一个工作程序线程（再唤醒其它线程是不礼貌的！）：
**[cpp]**[view plain](http://blog.csdn.net/dipperkun/article/details/6312588#)[copy](http://blog.csdn.net/dipperkun/article/details/6312588#)[print](http://blog.csdn.net/dipperkun/article/details/6312588#)[?](http://blog.csdn.net/dipperkun/article/details/6312588#)
- pthread_cond_signal(&cond);  
此函数只唤醒一个线程。如果 POSIX 线程标准允许指定一个整数，可以让您唤醒一定数量的正在睡眠的线程，那就更完美了。
多线程工作组
在这个方案中，我们创建了许多工作程序线程。每个线程都会检查 wq（“工作队列”），查看是否有需要完成的工作。如果有需要完成的工作，那么线程将从队列中除去一个节点，执行这些特定工作，然后等待新的工作到达。
与此同时，主线程负责创建这些工作程序线程、将工作添加到队列，然后在它退出时收集所有工作程序线程。
需要队列是出于两个原因。首先，需要队列来保存工作作业。还需要可用于跟踪已终止线程的数据结构。
 本文来自：http://blog.csdn.net/dipperkun/article/details/6312588
