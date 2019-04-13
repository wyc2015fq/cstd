
# Linux 内核自旋锁 - 嵌入式Linux - CSDN博客

2018年10月10日 09:45:57[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：62



# 为什么需要内核自旋锁？
现在很多CPU都是几核几核的了，如果有一个变量A，CPU-X正在访问，突然CPU-Y也过来访问他，这时候就可能出现问题，因为这个A非常重要，可能导致系统崩溃，中断异常等。
我们来看之前说的TP驱动里面的代码
`void gtp_irq_enable(struct goodix_ts_data *ts)
{
    unsigned long irqflags = 0;
    GTP_DEBUG_FUNC();
    
    spin_lock_irqsave(&ts->irq_lock, irqflags);
    if (ts->irq_is_disable) 
    {
        enable_irq(ts->client->irq);
        ts->irq_is_disable = 0; 
    }
    spin_unlock_irqrestore(&ts->irq_lock, irqflags);
}`
在进行中断操作的时候，用到了自旋锁，就是担心正在操作的时候又被调用，听起来有点拗口，但是就是那么一回事。
自旋锁（spinlock）是用在多个CPU系统中的锁机制，当一个CPU正访问自旋锁保护的临界区时，临界区将被锁上，其他需要访问此临界区的CPU只能忙等待，直到前面的CPU已访问完临界区，将临界区开锁。自旋锁上锁后让等待线程进行忙等待而不是睡眠阻塞，而信号量是让等待线程睡眠阻塞。自旋锁的忙等待浪费了处理器的时间，但时间通常很短，在1毫秒以下。
自旋锁用于多个CPU系统中，在单处理器系统中，自旋锁不起锁的作用，只是禁止或启用内核抢占。在自旋锁忙等待期间，内核抢占机制还是有效的，等待自旋锁释放的线程可能被更高优先级的线程抢占CPU。
自旋锁基于共享变量。一个线程通过给共享变量设置一个值来获取锁，其他等待线程查询共享变量是否为0来确定锁现是否可用，然后在忙等待的循环中"自旋"直到锁可用为止。

## 通用自旋锁解析
自旋锁的状态值为1表示解锁状态，说明有1个资源可用；0或负值表示加锁状态，0说明可用资源数为0。Linux内核为通用自旋锁提供了API函数初始化、测试和设置自旋锁。API函数功能说明如下表。
通用自旋锁API函数功能说明
|函数定义|功能说明|
|spin_lock_init(lock)|初始化自旋锁，将自旋锁设置为1，表示有一个资源可用。|
|spin_is_locked(lock)|如果自旋锁被置为1（未锁），返回0，否则返回1。|
|spin_unlock_wait(lock)|等待直到自旋锁解锁（为1），返回0；否则返回1。|
|spin_trylock(lock)|尝试锁上自旋锁（置0），如果原来锁的值为1，返回1，否则返回0。|
|spin_lock(lock)|循环等待直到自旋锁解锁（置为1），然后，将自旋锁锁上（置为0）。|
|spin_unlock(lock)|将自旋锁解锁（置为1）。|
|spin_lock_irqsave(lock, flags)|循环等待直到自旋锁解锁（置为1），然后，将自旋锁锁上（置为0）。关中断，将状态寄存器值存入flags。|
|spin_unlock_irqrestore(lock, flags)|将自旋锁解锁（置为1）。开中断，将状态寄存器值从flags存入状态寄存器。|
|spin_lock_irq(lock)|循环等待直到自旋锁解锁（置为1），然后，将自旋锁锁上（置为0）。关中断。|
|spin_unlock_irq(lock)|将自旋锁解锁（置为1）。开中断。|
|spin_unlock_bh(lock)|将自旋锁解锁（置为1）。开启底半部的执行。|
|spin_lock_bh(lock)|循环等待直到自旋锁解锁（置为1），然后，将自旋锁锁上（置为0）。阻止软中断的底半部的执行。|

## 自旋锁小例子
下面用一个使用自旋锁锁住链表的样例，代码列出如下（在gt9xx.c中）：
`/*初始化变量锁*/
spin_lock_init(&ts->irq_lock);          // 2.6.39 内核版本后的初始化使用方式
// ts->irq_lock = SPIN_LOCK_UNLOCKED;   // 2.6.39 内核版本之前的初始化使用方式
void gtp_irq_enable(struct goodix_ts_data *ts)
{
    unsigned long irqflags = 0;// 中断上下文使用的变量
    GTP_DEBUG_FUNC();
    spin_lock_irqsave(&ts->irq_lock, irqflags);//加锁
    if (ts->irq_is_disable) 
    {
        enable_irq(ts->client->irq);
        ts->irq_is_disable = 0; 
    }
    spin_unlock_irqrestore(&ts->irq_lock, irqflags);//解锁
}`
## 自旋锁相关源码
自旋锁用结构spinlock_t描述，在include/linux/spinlock.h中有类型 spinlock_t定义，列出如下：
`typedef struct {
    raw_spinlock_t raw_lock;
#ifdef CONFIG_GENERIC_LOCKBREAK    /*引入另一个自旋锁*/
    unsigned int break_lock;
#endif
#ifdef CONFIG_DEBUG_SPINLOCK   /*用于调试自旋锁*/
    unsigned int magic, owner_cpu;
    void *owner;
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
    struct lockdep_map dep_map;  /*映射lock实例到lock-class对象
#endif
} spinlock_t;`
**（1）spin_lock_init**
函数spin_lock_init将自旋锁状态值设置为1，表示未锁状态。其列出如下（在include/linux/spinlock.h中）：
`# define spin_lock_init(lock)                    /
    do { *(lock) = SPIN_LOCK_UNLOCKED; } while (0)`宏**__SPIN_LOCK_UNLOCKED**列出如下（在include/linux/spinlock_types.h中）：
`# define __SPIN_LOCK_UNLOCKED(lockname) /
    (spinlock_t)    {   .raw_lock = __RAW_SPIN_LOCK_UNLOCKED,   /
                SPIN_DEP_MAP_INIT(lockname) }
#define __RAW_SPIN_LOCK_UNLOCKED    { 1 }`**（2）函数spin_lock_irqsave**
函数spin_lock_irqsave等待直到自旋锁解锁，即自旋锁值为1，它还关闭本地处理器上的中断。其列出如下（在include/linux/spinlock.h中）：
`#define spin_lock_irqsave(lock, flags)    flags = _spin_lock_irqsave(lock)`函数spin_lock_irqsave分析如下（在kernel/spinlock.c中）：
`unsigned long __lockfunc _spin_lock_irqsave(spinlock_t *lock)
{
    unsigned long flags;
    local_irq_save(flags);  //将状态寄存器的值写入flags保存
    preempt_disable();      //关闭内核抢占，内核抢占锁加1
    spin_acquire(&lock->dep_map, 0, 0, _RET_IP_);
#ifdef CONFIG_LOCKDEP
    LOCK_CONTENDED(lock, _raw_spin_trylock, _raw_spin_lock);
#else
    _raw_spin_lock_flags(lock, &flags);
#endif
    return flags;
}`宏定义**local_irq_save**保存了状态寄存器的内容到x中，同时关中断。这个宏定义列出如下：
`#define local_irq_save(x)    __asm__ __volatile__("pushfl ; popl %0 ; cli":"=g" (x): /* no input */ :"memory")`上述语句中，指令pushfl将当前处理器的状态寄存器的内容压入堆栈保护。指令popl %0 将状态寄存器的内容存入x中，其中%0这里指x。
函数_raw_spin_lock_flags空操作等待直到自旋锁的值为1，表示有资源可用，就跳出循环等待，准备执行本函数后面的操作。其列出如下：
`# define _raw_spin_lock_flags(lock, flags) /
        __raw_spin_lock_flags(&(lock)->raw_lock, *(flags`函数__raw_spin_lock_flags列出如下（在include/asm-x86/spinlock.h中）：
`#define __raw_spin_lock_flags(lock, flags) __raw_spin_lock(lock)
static __always_inline void __raw_spin_lock(raw_spinlock_t *lock)
{
    int inc = 0x00010000;
    int tmp;
    /*指令前缀lock用来锁住内存控制器，不让其他处理器访问，保证指令执行的原子性*/
    asm volatile("lock ; xaddl %0, %1/n"   // lock->slock=lock->slock+inc
             "movzwl %w0, %2/n/t"         //tmp=inc
             "shrl $16, %0/n/t"           //inc >> 16 后，inc=1
             "1:/t"
             "cmpl %0, %2/n/t"           //比较inc与lock->slock
             "je 2f/n/t"                 //如果inc与lock->slock相等，跳转到2
             "rep ; nop/n/t"           //空操作
             "movzwl %1, %2/n/t"       //tmp=lock->slock
             /* 这里不需要读内存屏障指令lfence，因为装载是排序的*/
             "jmp 1b/n"               //跳转到1
             "2:"
             : "+Q" (inc), "+m" (lock->slock), "=r" (tmp)
             :
             : "memory", "cc");
}`**（3）函数spin_unlock_irqrestore**
宏定义spin_unlock_irqrestore是解锁，开中断，并把flags值存入到状态寄存器中，这个宏定义分析如下：
`#define spin_unlock_irqrestore(lock, flags)    _spin_unlock_irqrestore(lock, flags)`函数_spin_unlock_irqrestore列出如下（在kernel/spinlock.c中）：
`void __lockfunc _spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags)
{
    spin_release(&lock->dep_map, 1, _RET_IP_);
    _raw_spin_unlock(lock);    //解锁
    local_irq_restore(flags);  //开中断，将flag的值存入状态寄存器
    preempt_enable();          //开启内核抢占
}
# define _raw_spin_unlock(lock)        __raw_spin_unlock(&(lock)->raw_lock)`函数__raw_spin_unlock将自旋锁状态值加1，表示有1个资源可用，从而释放自旋锁，其列出如下（在include/asm-x86/spinlock.h中）：
`static __always_inline void __raw_spin_unlock(raw_spinlock_t *lock)
{
    asm volatile(UNLOCK_LOCK_PREFIX "incw %0"       // lock->slock= lock->slock +1
             : "+m" (lock->slock)
             :
             : "memory", "cc");
}`
## 自旋锁调试
由于自旋锁的性能严重地影响着操作系统的性能，Linux内核提供了Lock-class和Lockdep跟踪自旋锁的使用对象和锁的状态，并可从/proc文件系统查询自旋锁的状态信息。自旋锁的调试通过配置项CONFIG_DEBUG_*项打开。
对于对称多处理器系统（SMP），slock为一个int数据类型，对于单个处理器系统，slock定义为空。SMP的slock定义列出如下（在include/linux/spinlock_types.h）：
`typedef struct {    
     volatile unsigned int slock;
} raw_spinlock_t;`
---
**欢迎加我微信（weiqifa0）拉大家进微信技术讨论群**
**欢迎关注微信公众号-嵌入式Linux**
​

