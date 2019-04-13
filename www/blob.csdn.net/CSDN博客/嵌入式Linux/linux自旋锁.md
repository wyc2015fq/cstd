
# linux自旋锁 - 嵌入式Linux - CSDN博客

2015年11月17日 17:03:33[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：578个人分类：[Linux																](https://blog.csdn.net/weiqifa0/article/category/1388863)



## １、为什么要有自旋锁
我们写驱动代码的时候，往往忽略这一点，然后版本发布后会经常遇到一些异常的问题，资源的竞争相当重要，很容易出现空指针
引用：
if (!dptr->data[s_pos]) {
dptr->data[s_pos] = kmalloc(quantum, GFP_KERNEL);
if (!dptr->data[s_pos])
goto out;
}
假设有 2 个进程( 我们会称它们为”A”和”B” ) 独立地试图写入同一个 schull 设备的相同偏移. 每个进程同时到达上面片段的第一行的 if 测试. 如果被测试的指针是 NULL,每个进程都会决定分配内存, 并且每个都会复制结果指针给 dptr->datat[s_pos]. 因为2 个进程都在赋值给同一个位置, 显然只有一个赋值可以成功.当然, 发生的是第 2 个完成赋值的进程将”胜出”. 如果进程 A 先赋值, 它的赋值将被进程 B 覆盖. 在此, scull 将完全忘记 A 分配的内存; 它只有指向 B 的内存的指针. A所分配的指针, 因此, 将被丢掉并且不再返回给系统.
事情的这个顺序是一个竞争情况的演示. 竞争情况是对共享数据的无控制存取的结果. 当错误的存取模式发生了, 产生了不希望的东西. 对于这里讨论的竞争情况, 结果是内存泄漏. 这已经足够坏了, 但是竞争情况常常导致系统崩溃和数据损坏. 程序员可能被诱惑而忽视竞争情况为相当低可能性的事件. 但是, 在计算机世界, 百万分之一的事件会每隔几秒发生, 并且后果会是严重的.
## ２、使用自旋锁注意事项
１、尽可能短时间的拥有锁
２、获取锁之前，要禁止终端，避免无限自旋让系统崩溃
## ３、自旋锁的一些函数和方法
```python
#include <linux/spinlock.h>
```
```python
spinlock_t
```
```python
lock
```
```python
= SPIN_LOCK_UNLOCKED;
spin_lock_init(spinlock_t *
```
```python
lock
```
```python
);
定义自旋锁接口的包含文件, 以及初始化锁的
```
```python
2
```
```python
个方法.
```
```python
void
```
```python
spin_lock(spinlock_t *
```
```python
lock
```
```python
);
```
```python
void
```
```python
spin_lock_irqsave(spinlock_t *
```
```python
lock
```
```python
, unsigned
```
```python
long
```
```python
flags);
```
```python
void
```
```python
spin_lock_irq(spinlock_t *
```
```python
lock
```
```python
);
```
```python
void
```
```python
spin_lock_bh(spinlock_t *
```
```python
lock
```
```python
);
加锁一个自旋锁的各种方法, 并且, 可能地, 禁止中断.
```
```python
int
```
```python
spin_trylock(spinlock_t *
```
```python
lock
```
```python
);
```
```python
int
```
```python
spin_trylock_bh(spinlock_t *
```
```python
lock
```
```python
);
上面函数的非自旋版本; 在获取锁失败时返回
```
```python
0
```
```python
, 否则非零.
```
```python
void
```
```python
spin_unlock(spinlock_t *
```
```python
lock
```
```python
);
```
```python
void
```
```python
spin_unlock_irqrestore(spinlock_t *
```
```python
lock
```
```python
, unsigned
```
```python
long
```
```python
flags);
```
```python
void
```
```python
spin_unlock_irq(spinlock_t *
```
```python
lock
```
```python
);
```
```python
void
```
```python
spin_unlock_bh(spinlock_t *
```
```python
lock
```
```python
);
释放一个自旋锁的相应方法.
rwlock_t
```
```python
lock
```
```python
= RW_LOCK_UNLOCKED
rwlock_init(rwlock_t *
```
```python
lock
```
```python
);
初始化读者/写者锁的
```
```python
2
```
```python
个方法.
```
```python
void
```
```python
read_lock(rwlock_t *
```
```python
lock
```
```python
);
```
```python
void
```
```python
read_lock_irqsave(rwlock_t *
```
```python
lock
```
```python
, unsigned
```
```python
long
```
```python
flags);
```
```python
void
```
```python
read_lock_irq(rwlock_t *
```
```python
lock
```
```python
);
```
```python
void
```
```python
read_lock_bh(rwlock_t *
```
```python
lock
```
```python
);
获得一个读者/写者锁的读存取的函数.
```
```python
void
```
```python
read_unlock(rwlock_t *
```
```python
lock
```
```python
);
```
```python
void
```
```python
read_unlock_irqrestore(rwlock_t *
```
```python
lock
```
```python
, unsigned
```
```python
long
```
```python
flags);
```
```python
void
```
```python
read_unlock_irq(rwlock_t *
```
```python
lock
```
```python
);
```
```python
void
```
```python
read_unlock_bh(rwlock_t *
```
```python
lock
```
```python
);
释放一个读者/写者自旋锁的读存取.
```
```python
void
```
```python
write_lock(rwlock_t *
```
```python
lock
```
```python
);
```
```python
void
```
```python
write_lock_irqsave(rwlock_t *
```
```python
lock
```
```python
, unsigned
```
```python
long
```
```python
flags);
```
```python
void
```
```python
write_lock_irq(rwlock_t *
```
```python
lock
```
```python
);
```
```python
void
```
```python
write_lock_bh(rwlock_t *
```
```python
lock
```
```python
);
获得一个读者/写者锁的写存取的函数.
```
```python
void
```
```python
write_unlock(rwlock_t *
```
```python
lock
```
```python
);
```
```python
void
```
```python
write_unlock_irqrestore(rwlock_t *
```
```python
lock
```
```python
, unsigned
```
```python
long
```
```python
flags);
```
```python
void
```
```python
write_unlock_irq(rwlock_t *
```
```python
lock
```
```python
);
```
```python
void
```
```python
write_unlock_bh(rwlock_t *
```
```python
lock
```
```python
);
释放一个读者/写者自旋锁的写存取的函数.
```
```python
#include <asm/atomic.h>
```
```python
atomic_t v = ATOMIC_INIT(
```
```python
value
```
```python
);
```
```python
void
```
```python
atomic_set(atomic_t *v,
```
```python
int
```
```python
i);
```
```python
int
```
```python
atomic_read(atomic_t *v);
```
```python
void
```
```python
atomic_add(
```
```python
int
```
```python
i, atomic_t *v);
```
```python
void
```
```python
atomic_sub(
```
```python
int
```
```python
i, atomic_t *v);
```
```python
void
```
```python
atomic_inc(atomic_t *v);
```
```python
void
```
```python
atomic_dec(atomic_t *v);
```
```python
int
```
```python
atomic_inc_and_test(atomic_t *v);
```
```python
int
```
```python
atomic_dec_and_test(atomic_t *v);
```
```python
int
```
```python
atomic_sub_and_test(
```
```python
int
```
```python
i, atomic_t *v);
```
```python
int
```
```python
atomic_add_negative(
```
```python
int
```
```python
i, atomic_t *v);
```
```python
int
```
```python
atomic_add_return(
```
```python
int
```
```python
i, atomic_t *v);
```
```python
int
```
```python
atomic_sub_return(
```
```python
int
```
```python
i, atomic_t *v);
```
```python
int
```
```python
atomic_inc_return(atomic_t *v);
```
```python
int
```
```python
atomic_dec_return(atomic_t *v);
原子地存取整数变量. atomic_t 变量必须只通过这些函数存取.
```
```python
#include <asm/bitops.h>
```
```python
void
```
```python
set_bit(nr,
```
```python
void
```
```python
*addr);
```
```python
void
```
```python
clear_bit(nr,
```
```python
void
```
```python
*addr);
```
```python
void
```
```python
change_bit(nr,
```
```python
void
```
```python
*addr);
test_bit(nr,
```
```python
void
```
```python
*addr);
```
```python
int
```
```python
test_and_set_bit(nr,
```
```python
void
```
```python
*addr);
```
```python
int
```
```python
test_and_clear_bit(nr,
```
```python
void
```
```python
*addr);
```
```python
int
```
```python
test_and_change_bit(nr,
```
```python
void
```
```python
*addr);
原子地存取位值; 它们可用做标志或者锁变量. 使用这些函数阻止任何与并发存取
这个位相关的竞争情况.
```
```python
#include <linux/seqlock.h>
```
```python
seqlock_t
```
```python
lock
```
```python
= SEQLOCK_UNLOCKED;
seqlock_init(seqlock_t *
```
```python
lock
```
```python
);
定义 seqlock 的包含文件, 已经初始化它们的
```
```python
2
```
```python
个方法.
unsigned
```
```python
int
```
```python
read_seqbegin(seqlock_t *
```
```python
lock
```
```python
);
unsigned
```
```python
int
```
```python
read_seqbegin_irqsave(seqlock_t *
```
```python
lock
```
```python
, unsigned
```
```python
long
```
```python
flags);
```
```python
int
```
```python
read_seqretry(seqlock_t *
```
```python
lock
```
```python
, unsigned
```
```python
int
```
```python
seq);
```
```python
int
```
```python
read_seqretry_irqrestore(seqlock_t *
```
```python
lock
```
```python
, unsigned
```
```python
int
```
```python
seq, unsigned
```
```python
long
```
```python
flags);
获得一个 seqlock-保护 的资源的读权限的函数.
```
```python
void
```
```python
write_seqlock(seqlock_t *
```
```python
lock
```
```python
);
```
```python
void
```
```python
write_seqlock_irqsave(seqlock_t *
```
```python
lock
```
```python
, unsigned
```
```python
long
```
```python
flags);
```
```python
void
```
```python
write_seqlock_irq(seqlock_t *
```
```python
lock
```
```python
);
```
```python
void
```
```python
write_seqlock_bh(seqlock_t *
```
```python
lock
```
```python
);
获取一个 seqlock-保护的资源的写权限的函数.
```
```python
void
```
```python
write_sequnlock(seqlock_t *
```
```python
lock
```
```python
);
```
```python
void
```
```python
write_sequnlock_irqrestore(seqlock_t *
```
```python
lock
```
```python
, unsigned
```
```python
long
```
```python
flags);
```
```python
void
```
```python
write_sequnlock_irq(seqlock_t *
```
```python
lock
```
```python
);
```
```python
void
```
```python
write_sequnlock_bh(seqlock_t *
```
```python
lock
```
```python
);
释放一个 seqlock-保护的资源的写权限的函数.
```
```python
#include <linux/rcupdate.h>
```
```python
需要使用读取-拷贝-更新(RCU)机制的包含文件.
```
```python
void
```
```python
rcu_read_lock;
```
```python
void
```
```python
rcu_read_unlock;
获取对由 RCU 保护的资源的原子读权限的宏定义.
```
```python
void
```
```python
call_rcu(
```
```python
struct
```
```python
rcu_head *head,
```
```python
void
```
```python
(*func)(
```
```python
void
```
```python
*arg),
```
```python
void
```
```python
*arg);
安排一个回调在所有处理器已经被调度以及一个 RCU-保护的资源可用被安全的释
放之后运行.
```

