# 蜕变成蝶：Linux设备驱动中的并发控制 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [李辉](http://www.jobbole.com/members/harrylhmj) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
**并发和竞争发生在两类体系中：**
-     对称多处理器（SMP）的多个CPU
-     内核可抢占的单CPU系统
访问共享资源的代码区域称为临界区**（critical sections）**,临界区需要以某种互斥机制加以保护。在驱动程序中，当多个线程同时访问相同的资源**（critical sections）**时（驱动程序中的全局变量是一种典型的共享资源），可能会引发”竞态”，因此我们必须对共享资源进行并发控制。Linux内核中解决并发控制的方法又**中断屏蔽、原子操作、自旋锁、信号量**。（后面为主要方式）
**中断屏蔽：**
使用方法

C++
```
local_irq_disable()  //屏蔽中断
...
critical section        //临界区
...
local_irq_enable()   //开中断
```
local_irq_disable/enable只能禁止/使能本CPU内的中断，不能解决SMP多CPU引发的竞态，故不推荐使用，其适宜于自旋锁联合使用。
**原子操作：**
原子操作是一系列的不能被打断的操作。linux内核提供了一系列的函数来实现内核中的原子操作，这些函数分为2类，分别针对位和整型变量进行原子操作。
**实现整型原子操作的步骤如下：**
1.定义原子变量并设置变量值

C++
```
void atomic_set(atomic_t *v , int i); //设置原子变量值为i
atomic_t v = ATOMIC_INIT(0); //定义原子变量v，初始化为0
```
2.获取原子变量的值

C++
```
atomic_read(atomic_t *v);
```
3.原子变量加减操作

C++
```
void atomic_add(int i,atomic_t *v);//原子变量加i
void atomic_sub(int i ,atomic_t *v);//原子变量减i
```
4.原子变量自增/自减

C++
```
void atomic_inc(atomic_t *v);//自增1
void atomic_dec(atomic_t *v);//自减1
```
5.操作并测试：对原子变量执行自增、自减后（没有加）测试其是否为0，如果为0返回true，否则返回false。

C++
```
int atomic_inc_and_test(atomic_t *v);
int atomic_dec_and_test(atomic_t *v);
int atomic_sub_and_test(int i ,atomic_t *v);
```
6.操作并返回

C++
```
int atomic_add_return(int i , atomic_t *v);
int atomic_sub_return(int i , atomic_t *v);
int atomic_inc_return(atomic_t * v);
int atomic_dec_return(atomic_t * v)；
```
**实现 位原子操作如下：**

C++
```
// 设置位
void set_bit(nr, void *addr);  // 设置addr地址的第nr位，即将位写1
// 清除位
void clear_bit(nr, void *addr);  // 清除addr地址的第nr位，即将位写0
// 改变位
void change_bit(nr, void *addr);  // 对addr地址的第nr位取反
// 测试位
test_bit(nr, void *addr); // 返回addr地址的第nr位
// 测试并操作：等同于执行test_bit(nr, void *addr)后再执行xxx_bit(nr, void *addr)
int test_and_set_bit(nr, void *addr);
int test_and_clear_bit(nr, void *addr);
int test_and_change_bit(nr, void *addr)
```
**下面来举一个实例，是原子变量使用实例，使设备只能被一个进程打开：**

C++
```
static atomic_t xxx_available = ATOMIC_INIT(1);  // 定义原子变量
static int xxx_open(struct inode *inode, struct file *filp)
{
    ...
    if(!atomic_dec_and_test(&xxx_available))
    {
        atomic_inc(&xxx_availble);
        return - EBUSY;  // 已经打开
    }
    ...
    return 0;  // 成功
}
static int xxx_release(struct inode *inode, struct file *filp)
{
    atomic_inc(&xxx_available);  // 释放设备
    return 0;
}<strong> </strong>
```
我要着重谈一下:
**自旋锁VS信号量**
从严格意义上来说，信号量和自旋锁属于不同层次的互斥手段，前者的实现依赖于后者，在多CPU中需要自旋锁来互斥。信号量是进程级的，用于多个进程之间对资源的互斥，虽然也在内核中，但是该内核执行路径是以进程的身份，代表进程来争夺资源的。如果竞争失败，会切换到下个进程，而当前进程进入睡眠状态，因此，当进程占用资源时间较长时，用信号量是较好的选择。
当所要保护的临界访问时间比较短时，用自旋锁是非常方便的，因为它节省了上下文切换的时间。但是CPU得不到自旋锁是，CPU会原地打转，直到其他执行单元解锁为止，所以要求锁不能在临界区里停留时间过长。
**自旋锁的操作步骤：**

C++
```
1.定义自旋锁
spinlock_t lock;
2.初始化自旋锁
spin_lock_init(lock);这是个宏，它用于动态初始化自旋锁lock；
3.获得自旋锁
spin_lock(lock);该宏用于加锁，如果能够立即获得锁，它就能马上返回，否则，他将自旋在那里，直到该自旋锁的保持者释放。
spin_trylock(lock);能够获得，则返回真，否则返回假，实际上是不在原地打转而已。
4.释放自旋锁
spin_unlock(lock);
```
自旋锁持有期间内核的抢占将被禁止。 自旋锁可以保证临界区不受别的CPU和本CPU内的抢占进程打扰，但是得到锁的代码路径在执行临界区的时候还可能受到中断和底半部(BH)的影响。为防止这种影响，需要用到自旋锁的衍生：

C++
```
spin_lock_irq() = spin_lock() + local_irq_disable()
spin_unlock_irq() = spin_unlock() + local_irq_enable()
spin_lock_irqsave() = spin_lock() + local_irq_save()
spin_unlock_irqrestore() = spin_unlock() + local_irq_restore()
spin_lock_bh() = spin_lock() + local_bh_disable()
spin_unlock_bh() = spin_unlock() + local_bh_enable()
```
**注意：**自旋锁实际上是忙等待，只有在占用锁的时间极短的情况下，使用自旋锁才是合理的自旋锁可能导致死锁：递归使用一个自旋锁或进程获得自旋锁后阻塞。
**例子：**

C++
```
spinlock_t lock;
spin_lock_init(&lock);
spin_lock(&lock);  //获取自旋锁，保护临界区
。。。。临界区
spin_unlock(&lock);//释放自旋锁
```
自旋锁不关心锁定的临界区究竟是如何执行的。不管是读操作还是写操作，实际上，对共享资源进行读取的时候是应该可以允许多个执行单元同时访问的，那么这样的话，自旋锁就有了弊端。于是便衍生出来一个读写锁。它保留了自旋的特性，但在对操作上面可以允许有多个单元进程同时操作。当然，读和写的时候不能同时进行。
现在又有问题了，如果我第一个进程写共享资源，第二个进程读的话，一旦写了，那么就读不到了，可能写的东西比较多，但是第二个进程读很小，那么能不能第一个进程写的同时，我第二个进程读呢？
当然可以，那么引出了顺序锁的概念。都是一样的操作。
**读写自旋锁(rwlock)**允许读的并发。在写操作方面，只能最多有一个写进程，在读操作方面，同时可以有多个读执行单元。当然，读和写也不能同时进行。

C++
```
// 定义和初始化读写自旋锁
rwlock_t my_rwlock = RW_LOCK_UNLOCKED;  // 静态初始化
rwlock_t my_rwlock;
rwlock)init(&my_rwlock);  // 动态初始化
// 读锁定：在对共享资源进行读取之前，应先调用读锁定函数，完成之后调用读解锁函数
void read_lock(rwlock_t *lock);
void read_lock_irqsave(rwlock_t *lock, unsigned long flags);
void read_lock_irq(rwlock_t *lock);
void read_lock_bh(rwlock_t *lock);
// 读解锁
void read_unlock(rwlock_t *lock);
void read_unlock_irqrestore(rwlock_t *lock, unsigned long flags);
void read_unlock_irq(rwlock_t *lock);
void read_unlock_bh(rwlock_t *lock);
// 写锁定：在对共享资源进行写之前，应先调用写锁定函数，完成之后调用写解锁函数
void write_lock(rwlock_t *lock);
void write_lock_irqsave(rwlock_t *lock, unsigned long flags);
void write_lock_irq(rwlock_t *lock);
void write_lock_bh(rwlock_t *lock);
int write_trylock(rwlock_t *lock);
// 写解锁
void write_unlock(rwlock_t *lock);
void write_unlock_irqsave(rwlock_t *lock, unsigned long flags);
void write_unlock_irq(rwlock_t *lock);
void write_unlock_bh(rwlock_t *lock);
```
**读写自旋锁一般用法：**

C++
```
rwlock_t lock;  // 定义rwlock
rwlock_init(&lock);  // 初始化rwlock
// 读时获取锁
read_lock(&lock);
...  // 临界资源
read_unlock(&lock);
// 写时获取锁
write_lock_irqsave(&lock, flags);
...  // 临界资源
write_unlock_irqrestore(&lock, flags);<strong> </strong>
```
**顺序锁（seqlock）:**
顺序锁是对读写锁的一种优化，若使用顺序锁，读与写操作不阻塞，只阻塞同种操作，即读与读/写与写操作。
写执行单元的操作顺序如下：

C++
```
//获得顺序锁
void write_seqlock(seqlock_t *s1);
int write_tryseqlock(seqlock_t *s1);
write_seqlock_irqsave(lock, flags)
write_seqlock_irq(lock)
write_seqlock_bh(lock)
//释放顺序锁
void write_sequnlock(seqlock_t *s1);
write_sequnlock_irqrestore(lock, flags)
write_sequnlock_irq(lock)
write_sequnlock_bh(lock)
```
读执行单元的操作顺序如下：

C++
```
//读开始
unsinged read_seqbegin(const seqlock_t *s1);
read_seqbegin_irqsave(lock, flags)
//重读，读执行单元在访问完被顺序锁s1保护的共享资源后需要调用该函数来检查在读操作器件是否有写操作，如果有，读执行单元需要从新读一次。
int reead_seqretry(const seqlock_t *s1, unsigned iv);
read_seqretry_irqrestore(lock, iv, flags)
```
**RCU(Read-Copy Update 读-拷贝-更新)**可看作读写锁的高性能版本，既允许多个读执行单元同时访问被保护的数据，又允许多个读执行单元和多个写执行单元同时访问被保护的数据。但是RCU不能替代读写锁。因为如果写操作比较多时，对读执行单元的性能提高不能弥补写执行单元导致的损失。因为使用RCU时，写执行单元之间的同步开销会比较大，它需要延迟数据结构的释放，复制被修改的数据结构，它也必须使用某种锁机制同步并行的其他写执行单元的修改操作。
具体操作：略
**信号量的使用**
信号量(semaphore)与自旋锁相同，只有得到信号量才能执行临界区代码，但，当获取不到信号量时，进程不会原地打转而是进入休眠等待状态。
相同点：只有得到信号量的进程才能执行临界区的代码。（linux自旋锁和信号量锁采用的都是“获得锁-访问临界区-释放锁”，可以称为“互斥三部曲”，实际存在于几乎所有多任务操作系统中）
不同点：当获取不到信号量时，进程不会原地打转而是进入休眠等待状态。
信号量的操作：

C++
```
//信号量的结构
struct semaphore sem;
//初始化信号量
void sema_init(struct semaphore *sem, int val)
    //常用下面两种形式
#define init_MUTEX(sem) sema_init(sem, 1)
#define init_MUTEX_LOCKED(sem) sema_init(sem, 0)
    //以下是初始化信号量的快捷方式，最常用的
DECLARE_MUTEX(name)    //初始化name的信号量为1
DECLARE_MUTEX_LOCKED(name) //初始化信号量为0
//常用操作
DECLARE_MUTEX(mount_sem);
down(&mount_sem); //获取信号量
...
critical section    //临界区
...
up(&mount_sem);    //释放信号量
```
信号量用于同步时只能唤醒一个执行单元，而完成量（completion）用于同步时可以唤醒所有等待的执行单元。
**自旋锁与互斥锁的选择**
- 当锁 不能被获取到时，使用信号量的开销是进程上下文切换时间Tsw，使用自旋锁的开始是等待获取自旋锁的时间Tcs，若Tcs比较小，则应使用自旋锁，否则应使用信号量
- 信号量锁保护的临界区可以包含引起阻塞的代码，而自旋锁则却对要避免使用包含阻塞的临界区代码，否则很可能引发锁陷阱
- 信号量存在于进程上下文，因此，如果被保护的共享资源需要在中断或软中断情况下使用，则在信号量和自旋锁之间只能选择自旋锁。当然，如果一定要使用信号量，则只能通过down_trylock()方式进行，不能获取就立即返回以避免阻塞。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/d4181232cde3a79b8c6d11e39fd06ad2.png)![](http://www.jobbole.com/wp-content/uploads/2016/04/0e2c29b501d9020b8ef4615fe1493fc0.jpg)
