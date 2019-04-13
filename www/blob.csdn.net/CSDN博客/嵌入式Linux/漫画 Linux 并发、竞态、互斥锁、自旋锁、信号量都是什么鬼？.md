
# 漫画|Linux 并发、竞态、互斥锁、自旋锁、信号量都是什么鬼？ - 嵌入式Linux - CSDN博客

2019年02月21日 14:38:40[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：228



### 文章目录
[1. 锁的由来？](#1__1)
[2. 什么是并发和竞态？](#2__6)
[2.1 并发与竞态概念](#21__30)
[3. 讨论下死锁](#3__51)
[3.1 多进程调度导致死锁](#31__53)
[3.2 单线程导致死锁](#32__62)
[4. 互斥锁和自旋锁、信号量的区别？](#4__67)
[5. 如何解决竞态引起的问题？](#5__74)
[1. 中断屏蔽](#1__77)
[2.原子操作](#2_89)
[3. 自旋锁](#3__169)
[4.读写自旋锁](#4_213)
[5.顺序锁](#5_269)
[6. RCU（读-拷贝-更新）](#6_RCU_308)
[7. 信号量](#7__339)
[8.Completion用于同步](#8Completion_365)
[9.读写信号量](#9_389)
[10.互斥体](#10_419)

# 1. 锁的由来？
学习linux的时候，肯定会遇到各种和锁相关的知识，有时候自己学好了一点，感觉半桶水的自己已经可以华山论剑了，又突然冒出一个新的知识点，我看到新知识点的时候，有时间也是一脸的懵逼，在大学开始写单片机的跑裸机代码，完全不懂这个锁在操作系统里面是什么鬼，从单片机到嵌入式Linux，还有一个**多****任务系统**，不懂的同学建议百度看看。
![](https://img-blog.csdnimg.cn/20190221230057915.png)
![](https://img-blog.csdnimg.cn/20190221143612689.png)
# 2. 什么是并发和竞态？
在早期的Linux内核中，并发源相对较少。内核不支持对称多处理器（SMP）系统，唯一导致并发问题的原因是**中断**。
随着处理器的CPU核越来越多，这要求系统对事件迅速做出响应。为适应现代硬件和应用的需求，Linux内核已经发展到可以同时进行更多事情的地步。这种演变带来了更大的可伸缩性。但是，这也大大复杂化了内核编程的任务。设备驱动程序员现在必须从一开始就将并发性考虑到他们的设中，而且他们需要深刻的理解并发问题，并利用内核提供的工具处理这类问题。
![](https://img-blog.csdnimg.cn/20190218204907993.png)
**Spinlocks and Atomic Context**
> Imagine for a moment that your driver acquires a spinlockand goes

> about its business within its critical section. Somewhere in the

> middle, your driver loses the processor.Perhaps it has called a

> function (copy_from_user, say) that puts the process to sleep. Or,

> perhaps, kernel preemption kicks in, and a higher-priority process

> pushes your code aside. Your code is now holding a lockthat it will

> not release any time in the foreseeable future. If some other thread

> tries to obtain the same lock, it will, in the best case, wait

> (spinning in the processor) for a very long time. In the worst

> case,the system could deadlock entirely. Most readers would agree that

> this scenario is best avoided. Therefore, the core rule that applies

> to spinlocks is that any code must, while holding a spinlock, be

> [atomic.It](http://atomic.It)
> cannot sleep; in fact, it cannot relinquish the processor

> for any reason except toservice interrupts (and sometimes not even

> then).

> 2.1 并发与竞态概念

> 上面扯淡完了进入正题

> 什么是并发：
> 并发是指多个执行任务同时、
> 并行
> 被执行。

> 什么是竞态：
> 字面意思是竞争，并发的执行单元对共享资源（硬件资源和软件上的全局变量，静态变量等）的访问容易发生竞态。

> 举例一个字符设备的缺陷：
> 对于一个虚拟的字符设备驱动，假设一个执行单元A对其写入300个字符‘a’，而另一个执行单元B对其写入300个字符‘b’，第三个执行单元读取所有字符。如果A、B被顺序串行执行那么C读出的则不会出错，但如果A、B并发执行，那结果则是我们不可料想的。

> 竞态发生的情况

> 对称多处理器（SMP）的多个CPU：
> SMP是一种紧耦合、共享存储的系统模型，它的特点是多个CPU使用共同的系统总线，因此可以访问共同的外设和存储器。

> 单CPU内进程与抢占它的进程：
> Linux 2.6的内核支持抢占调度，一个进程在内核执行的时候可能被另一高优先级进程打断。

> 中断（硬中断、软中断、tasklet、低半部）与进程之间：
> 中断可以打断正在执行的进程，处理中断的程序和被打断的进程间也可能发生竞态。

> 竞态的解决办法

> 解决竞态问题的途径是保证对共享资源的互斥访问。访问共享资源的代码区域称为
> 临界区
> ，临界区要互斥机制保护。Linux设备驱动中常见的互斥机制有以下方式：中断屏蔽、原子操作、自旋锁和信号量等。
![](https://img-blog.csdnimg.cn/20190218201757664.png)
> Most readers would agree that this scenario is best avoided.

> Therefore, the core rule that applies to spinlocks is that any code

> must, while holding a spinlock, be
> [atomic.It](http://atomic.It)
> cannot sleep; in fact, it

> cannot relinquish the processor for any reason except toservice

> interrupts (and sometimes not even then).

> 对上面死锁理解不够深入的，可以细细评味这段英文。
![](https://img-blog.csdnimg.cn/20190221143612689.png)
> 3. 讨论下死锁

> 死锁的问题是开发中稍不小心就可能遇到的，在SMP系统里面，如果有一个CPU被死锁了，还有其他CPU可以继续运行，就像一个车子，有一个轮子爆胎了，理论上还是可以跑的，就是开不快，或者开快的话就会容易挂逼。

> 3.1 多进程调度导致死锁

> 之前的文章https://mp.weixin.qq.com/s/au-ZXnTgpOO3mYRyKir01w

> 出现以下四种情况会产生死锁：

> 1，相互排斥。一个线程或进程永远占有共享资源，比如，独占该资源。

> 2，循环等待。例如，进程A在等待进程B，进程B在等待进程C，而进程C又在等待进程A。

> 3，部分分配。资源被部分分配，例如，进程A和B都需要访问一个文件，同时需要用到打印机，进程A得到了这个文件资源，进程B得到了打印机资源，但两个进程都不能获得全部的资源了。

> 4，缺少优先权。一个进程获得了该资源但是一直不释放该资源，即使该进程处于阻塞状态。

> 具体使用的场景会更加复杂，要需要按实际分析，对号入座~

> 3.2 单线程导致死锁

> 单线程导致死锁的情况一般是由于调用了引起阻塞的函数，比如（copy_from_user()、copy_to_ser()、和kmalloc（）），阻塞后进行系统调度，调度的过程中有可能又调用了之前获取锁的函数，这样必然导致死锁。

> 还有一种就是自旋锁函数在没有释放锁马上又进行申请同一个自旋锁，这样的低级问题也是会导致自旋锁。
![](https://img-blog.csdnimg.cn/20190221143612689.png)
> 4. 互斥锁和自旋锁、信号量的区别？

> 互斥锁和互斥量
> 在我的理解里没啥区别，不同叫法。广义上讲可以值所有实现互斥作用的同步机制。狭义上讲指的就是mutex这种特定的二元锁机制。互斥锁的作用就是互斥，mutual exclusive，
> 是用来保护临界区(critical section)的
> 。所谓临界区就是代码的一个区间，如果两个线程同时执行就有可能出问题，所以需要互斥锁来保护。

> 信号量（semaphore）
> 是一种更高级的同步机制，
> mutex（互斥锁）
> 可以说是
> semaphore（信号量）
> 在仅取值0/1时的特例。Semaphore可以有更多的取值空间，用来实现更加复杂的同步，而不单单是线程间互斥。

> 自旋锁
> 是一种
> 互斥锁
> 的实现方式而已，相比一般的互斥锁会在等待期间放弃cpu，
> 自旋锁（spinlock）
> 则是不断循环并测试锁的状态，这样就一直占着cpu。所以相比于自旋锁和信号量，在申请锁失败的话，自旋锁会不断的查询，申请线程不会进入休眠，信号量和互斥锁如果申请锁失败的话线程进入休眠，如果申请锁被释放后会唤醒休眠的线程。

> 同步锁
> 好像没啥特殊说法，你可以理解为能实现同步作用的都可以叫同步锁，比如信号量。最后，不要钻这些名词的牛角尖，更重要的是理解这些东西背后的原理，叫什么名字并没有什么好说的。这些东西在不同的语言和平台上又有可能会有不同的叫法，其实本质上就这么回事。
![](https://img-blog.csdnimg.cn/20190221143612689.png)
> 5. 如何解决竞态引起的问题？

> 上面我们已经分析了竞态产生的原因、发生的情况以及解决办法，下面我们对常见的解决办法一一分析。

> 1. 中断屏蔽

> 1.基本概念
> ：在单CPU中避免竞态的一种简单方法是在进入临界区之前屏蔽系统的中断。由于linux的异步I/O、进程调度等很多内容都依靠中断，所以我们应该尽快的执行完临界区的代码，换句话就是临界区代码应该尽量少。

> 2.具体操作
> ： linux内核提供了下面具体方法

> Local_irq_disable();//屏蔽中断
Local_irq_enable();//打开中断
Local_irq_save(flags);//禁止中断并保存当前cpu的中断位信息
> 2.原子操作

> 1.基本概念：原子操作指在执行过程中不会被别的代码中断的操作。

> 2.具体操作：linux内核提供了一系列的函数来实现内核中的原子操作，这些操作分为两类，一类是整型原子操作，另一类是位原子操作，其都依赖底层CPU的原子操作实现，所以这些函数与CPU架构有密切关系。

> 1) 整型原子操作

> a)设置原子变量的值

> atomic_t v = ATOMIC_INIT(0);//定义原子变量v并初始化为0
void atomic_set(atomic_t *v, int i);//设置原子变量值为i
> b)获取原子变量的值

> atomic_read(atomic_t *v);//返回原子变量v的值
> c)原子变量加、减操作

> void atomic_add(int i, atomic_t *v);//原子变量v增加i
void atomic_sub(int I, atomic_t *v);//原子变量v减少i
> d)原子变量自增、自减

> void atomic_inc(atomic_t *v);//原子变量v自增1
void atomic_dec(atomic_t *v);//原子变量v自减1
> e)操作并测试

> int atomic_inc_and_test(atomic_t *v);
int atomic_dec_and_test(atomic_t *v);  
 int atomic_sub_and_test(int i,atomic_t *v);
/*上述三个函数对原子变量v自增、自减和减操作（没有加）后测试其是否为0，如果为0返回true，否则返回false*/
> f) 操作并返回

> int atomic_add_return(int i,atomic_t *v);
int atomic_sub_return(int i,atomic_t *v);  
 int atomic_inc_return(atomic_t *v);
int atomic_dec_return(atomic_t *v);
/*上述函数对原子变量v进行自增、自减、加、减操作，并返回新的值*/
> 2) 位原子操作

> a)设置位

> void set_bit(nr,void *addr);//设置addr地址的第nr位，即向该位写入1。
> b)清除位

> void clear_bit(nr,void *addr);//清除addr地址的第nr位，即向该位写入0。
> c)改变位

> void change_bit(nr,void *addr);//对addr地址的第nr取反
> d)测试位

> int test_bit(nr,void *addr);//返回addr地址的第nr位
> e) 测试并操作位

> int test_and_set_bit(nr,void *addr);
int test_and_clear_bit(nr,void *addr); 
 int test_and_change_bit(nr,void *addr);
/*上述函数等同于执行test_bit后，再执行xxx_bit函数*/
> 3. 自旋锁

> 1.基本概念：
> 自旋锁是一种对临界资源进行互斥访问的手段。

> 2.工作原理：
> 为获得自旋锁，在某CPU上运行的代码需先执行一个原子操作，该操作测试并设置某个内存变量，由于其为原子操作，所以在该操作完成之前其他执行单元不可能访问这个内存变量，如果测试结果表明已经空闲，则程序获得这个自旋锁并继续执行，如果测试结果表明该锁仍被占用，程序将在一个小的循环内重复这个“测试并设置”操作，即进行所谓的“自旋”，通俗的说就是在“原地打转”。

> 3.具体操作：
> linux内核中与自旋锁相关的操作主要有：

> 1)定义自旋锁

> spinlock_t lock;
> 2)初始自旋锁

> spin_lock_init(lock);
> 3)获得自旋锁

> spin_lock(lock);//获得自旋锁lock
spin_trylock(lock);//尝试获取lock如果不能获得锁，返回假值，不在原地打转。
> 4)释放自旋锁

> spin_unlock(lock);//释放自旋锁
> 为保证我们执行临界区代码的时候不被中断等影响我们的自旋锁又衍生了下面的内容

> 5)自旋锁衍生

> spin_lock_irq() = spin_lock() + local_irq_disable()
spin_unlock_irq() = spin_unlock() + local_irq_enable()
spin_lock_irqsave() = spin_lock() + local_irq_save() 
 spin_unlock_irqrestore() = spin_unlock() + local_irq_restore()
spin_lock_bh() = spin_lock() + local_bh_disable()
spin_unlock_bh() = spin_unlock() + local_bh_disable()
> 4.使用注意事项：

> 1)自旋锁实质是忙等锁，因此在占用锁时间极短的情况下，使用锁才是合理的，反之则会影响系统性能。

> 2)自旋锁可能导致系统死锁。

> 3)自旋锁锁定期间不能调用可能引起进程调度的函数。

> 4.读写自旋锁

> 1.基本概念：
> 为解决自旋锁中不能允许多个单元并发读的操作，衍生出了读写自旋锁，其不允许写操作并发，但允许读操作并发。

> 2.具体操作：
> linux内核中与读写自旋锁相关的操作主要有：

> 1)定义和初始化读写自旋锁

> rwlock_t my_rwlock = RW_LOCK_UNLOCKED;//静态初始化
rwlock_t my_rwlock;
rwlock_init(&my_rwlock);//动态初始化
> 2)读锁定

> read_lock();
read_lock_irqsave();
read_lock_irq();
read_lock_bh();
> 3)读解锁

> read_unlock();
read_unlock_irqrestore();
read_unlock_irq();
read_unlock_bh();
> 4)写锁定

> write_lock();
write_lock_irqsave();
write_lock_irq();
write_lock_bh();
write_trylock();
> 5)写解锁

> write_unlock();
write_unlock_irqrestore();
write_unlock_irq();
write_unlock_bh();
> 5.顺序锁

> 1.基本概念：
> 顺序锁是对读写锁的一种优化，如果使用顺序锁，读执行单元在写执行单元对被顺序锁保护的共享资源进行写操作时仍然可以继续读，不必等待写执行单元的完成，写执行单元也不需等待读执行单元完成在进行写操作。

> 2.注意事项：
> 顺序锁保护的共享资源不含有指针，因为在写执行单元可能使得指针失效，但读执行单元如果此时访问该指针，将导致oops。

> 3.具体操作：
> linux内核中与顺序锁相关的操作主要有：

> 1)写执行单元获得顺序锁

> write_seqlock();
write_tryseqlock();
write_seqlock_irqsave();
write_seqlock_irq();
write_seqlock_bh();
> 2)写执行单元释放顺序锁

> write_sequnlock();
write_sequnlock_irqrestore();
write_sequnlock_irq();
write_sequnlock_bh();
> 3)读执行单元开始

> read_seqbegin();
read_seqbegin_irqsave();//local_irq_save + read_seqbegin
> 4)读执行单元重读

> read_seqretry ();
read_seqretry_irqrestore ();
> 6. RCU（读-拷贝-更新）

> 1.基本概念：
> RCU可以看做是读写锁的高性能版本，相比读写锁，RCU的优点在于即允许多个读执行单元同时访问被保护数据，又允许多个读执行单元和多个写执行单元同时访问被保护的数据。

> 2.注意事项：
> RCU不能代替读写锁。

> 3.具体操作：
> linux内核中与RCU相关的操作主要有：

> 1)读锁定

> rcu_read_lock ();
rcu_read_lock_bh ();
> 2)读解锁

> rcu_read_unlock ();
rcu_read_unlock_bh ();
> 3)同步RCU

> synchronize_rcu ();//由RCU写执行单元调用
synchronize_sched();//可以保证中断处理函数处理完毕，不能保证软中断处理结束
> 4)挂接回调

> call_rcu ();
call_rcu_bh ();
> 有关RCU的操作还有很多，大家可以参考网络。

> 7. 信号量

> 1.基本概念：
> 信号量用于保护临界区的常用方法与自旋锁类似，但不同的是当获取不到信号量时，进程不会原地打转而是进入休眠等待状态。

> 2.具体操作：
> linux内核中与信号量相关的操作主要有：

> 1)定义信号量

> Struct semaphore sem;
> 2)初始化信号量

> void sema_init(struct semaphore *sem, int val);//初始化sem为val，当然还有系统定义的其他宏初始化，这里不列举
> 3)获得信号量

> void down(struct semaphore *sem);//获得信号量sem，其会导致睡眠，并不能被信号打断 
 int down_interruptible(struct semaphore *sem);//进入睡眠可以被信号打断 
 int down_trylock(struct semaphore *sem);//不会睡眠
> 4)释放信号量

> void up(struct semaphore *sem);//释放信号量，唤醒等待进程
> 注：当信号量被初始为0时，其可以用于同步。

> 8.Completion用于同步

> 1.基本概念：
> linux中的同步机制。

> 2.具体操作：
> linux内核中与Completion相关的操作主要有：

> 1)定义Completion

> struct completion *my_completion;
> 2)初始化Completion

> void init_completion(struct completion *x);
> 3)等待Completion

> void wait_for_completion(struct completion *);
> 4)唤醒Completion

> void complete(struct completion *);//唤醒一个 
　　 void complete_all(struct completion *);//唤醒该Completion的所有执行单元
> 9.读写信号量

> 1.基本概念：
> 与自旋锁和读写自旋锁的关系类似

> 2.具体操作：
> linux内核中与读写信号量相关的操作主要有：

> 1)定义和初始化读写自旋锁

> struct rw_semaphore sem; 
 init_rwsem(&sem);
> 2)读信号量获取

> down_read ();
down_read_trylock();
> 3)读信号量释放

> up_read ();
> 4)写信号量获取

> down_write ();
down_write_trylock ();
> 5)写信号量释放

> up_write();
> 10.互斥体

> 1.基本概念：
> 用来实现互斥操作

> 2.具体操作：
> linux内核中与互斥体相关的操作主要有：

> 1)定义和初始化互斥体

> struct mutex lock;
mutex_init(&lock);
> 2)获取互斥体

> void mutex_lock(struct mutex *lock);
int mutex_lock_interruptible(struct mutex *lock); 
 int mutex_lock_killable(struct mutex *lock);
> 3)释放互斥体

> void mutex_unlock(struct mutex *lock);
![](https://img-blog.csdnimg.cn/20190221225232282.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190221143612689.png)
![](https://img-blog.csdnimg.cn/20190221143658736.png)

