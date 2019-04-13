
# DPDK-同步互斥机制 - 3-Number - CSDN博客


2017年11月15日 15:02:51[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：864


## 0x01 缘由
继续学习DPDK设计思想。

## 0x02 概念
**原子操作：**多个线程执行一个操作时，其中任何一个线程要么完全执行完此操作，要么没有执行此操作的任何步骤，那么这个操作就是原子的。原子操作操作系统中：“不能被进一步分割的最小粒子”。
**原子保证操作：**一些基础的内存事务操作，如对一个字节的读和写，它们总是原子的。对于边界对齐的字节、字、双字和四字节都可以原子操作。反之。
**decl指令：**递减指令，“读->改->写”，涉及两次内存操作，如果多个CPU运行的多个进程或线程在同时对同一块内存执行这个指令，那情况是无法预测的。
**加LOCK指令前缀**：对于LOCK指令前缀的总线锁。
**缓存一致性：**缓存一致性保证了一致性。
**CMPXCHG单指令操作：**比较并交换操作数。
**内存屏障：**内存屏障，也称内存栅栏，内存栅障，屏障指令等， 是一类同步屏障指令，是CPU或编译器在对内存随机访问的操作中的一个同步点，使得此点之前的所有读写操作都执行后才可以开始执行此点之后的操作。大多数现代计算机为了提高性能而采取乱序执行，这使得内存屏障成为必须。语义上，内存屏障之前的所有写操作都要写入内存；内存屏障之后的读操作都可以获得同步屏障之前的写操作的结果。因此，对于敏感的程序块，写操作之后、读操作之前可以插入内存屏障。
**读写锁：**它把对共享资源的访问操作划分成读操作和写操作，读操作只对共享资源进行读访问，写操作则需要对共享资源进行写操作。
**自旋锁：**自旋锁是专为防止多处理器并发而引入的一种锁，它在内核中大量应用于中断处理等部分（对于单处理器来说，防止中断处理中的并发可简单采用关闭中断的方式，即在标志寄存器中关闭/打开中断标志位，不需要自旋锁）。何谓自旋锁？它是为实现保护共享资源而提出一种锁机制。其实，自旋锁与互斥锁比较类似，它们都是为了解决对某项资源的互斥使用。无论是互斥锁，还是自旋锁，在任何时刻，最多只能有一个保持者，也就说，在任何时刻最多只能有一个执行单元获得锁。但是两者在调度机制上略有不同。对于互斥锁，如果资源已经被占用，资源申请者只能进入睡眠状态。但是自旋锁不会引起调用者睡眠，如果自旋锁已经被别的执行单元保持，调用者就一直循环在那里看是否该自旋锁的保持者已经释放了锁，"自旋"一词就是因此而得名。
**无锁机制：**不加锁的数据结构。

## 0x03 原子操作
Linux 内核原子操作：软件级的原子操作实现依赖于硬件原子操作的支持。
### 1.原子整数操作
```python
typedef struct {
        volatile int counter;
     } atomic_t;
    static __inline__ int atomic_read(const atomic_t *v)
    {
        return v->counter;
    }
```

### 2.原子性与顺序性
**原子性：**确保指令执行期间不被打断，要么全部执行，要么根本不执行。
**顺序性：**即使两条或多条指令出现在独立的执行线程中，甚至独立的处理器上，他们本该执行的顺序依然要保持。

### 3.原子位操作
位操作函数是对普通的内存地址进行操作的。
glibc原子操作：
```python
type __sync_fetch_and_add (type *ptr, type value, ...)
    type __sync_fetch_and_sub (type *ptr, type value, ...)
    type __sync_fetch_and_or (type *ptr, type value, ...)
    type __sync_fetch_and_and (type *ptr, type value, ...)
    type __sync_fetch_and_xor (type *ptr, type value, ...)
    type __sync_fetch_and_nand (type *ptr, type value, ...)
    type __sync_add_and_fetch (type *ptr, type value, ...)
    type __sync_sub_and_fetch (type *ptr, type value, ...)
    type __sync_or_and_fetch (type *ptr, type value, ...)
    type __sync_and_and_fetch (type *ptr, type value, ...)
    type __sync_xor_and_fetch (type *ptr, type value, ...)
    type __sync_nand_and_fetch (type *ptr, type value, ...)
```

### 4.DPDK中原子操作
内存屏障：
```python
#ifdef __cplusplus
```
```python
extern "C" {
```
```python
#endif
    #define dsb(opt)  { asm volatile("dsb " #opt : : : "memory"); }
    #define dmb(opt)  { asm volatile("dmb " #opt : : : "memory"); }
    #define rte_mb() dsb(sy)
    #define rte_wmb() dsb(st)
    #define rte_rmb() dsb(ld)
    .....
    #ifdef __cplusplus
    }
    #endif
```

原子操作：
```python
static inline int
    rte_atomic64_cmpset(volatile uint64_t *dst, uint64_t exp, uint64_t src)
    {
        uint8_t res;
        asm volatile(
                MPLOCKED
                "cmpxchgq %[src], %[dst];"
                "sete %[res];"
                : [res] "=a" (res),     /* output */
                  [dst] "=m" (*dst)
                : [src] "r" (src),      /* input */
                  "a" (exp),
                  "m" (*dst)
                : "memory");            /* no-clobber list */
        return res；
    }
```

## 0x04 读写锁
读写锁相对于自旋锁而言，能提高并发性，因为在多处理器系统中，它允许同事有多个读操作来访问共享资源，最大可能的读操作为实际的逻辑CPU数。
1.Linux 读写锁
\#define read_lock(lock)            _read_lock(lock)
\#define _read_lock(lock)            __LOCK(lock)
\#define __LOCK(lock) \
do { preempt_disable(); __acquire(lock); (void)(lock); } while (0)
2.glibc 读写锁
int pthread_rwlock_rdlock(pthread_rwlock_t *rwptr);
int pthread_rwlock_wrlock(pthread_rwlock_t *rwptr);
int pthread_rwlock_unlock(pthread_rwlock_t *rwptr);
3.DPDK中读写锁
![](https://img-blog.csdn.net/20171115150115596?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 0x05 自旋锁
1.DPDK自旋锁
![](https://img-blog.csdn.net/20171115150132579?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 0x06 无锁机制
高性能的服务器软件在大部分情况下是运行在多核服务器上的，当前硬件可以提供32/64或者更多的CPU，在这种高并发的环境下，锁竞争机制有时会比数据拷贝、上下文切换等更伤害系统性能。

### 1.Linux 内核无锁环形缓冲
分析：、
/* is x a power of 2? */
\#define is_power_of_2(x)    ((x) != 0 && (((x) & ((x) - 1)) == 0))
下面对kfifo的分析比较完整：[http://blog.csdn.net/yang_chen_shi_wo/article/details/46284903](http://blog.csdn.net/yang_chen_shi_wo/article/details/46284903)

### 2.DPDK无锁环形缓冲
DPDK中rte_ring 支持：单生产者-单消费者、多生产者-多消费者。
多个生产者入列：

## 0x06 总结
锁是性能杀手，这点在实践中很有体会。学习DPDK的设计方法。




