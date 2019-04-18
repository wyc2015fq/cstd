# 无锁的数据结构（Lock-Free）及CAS（Compare-and-Swap）机制 - 文章 - 伯乐在线
原文出处： [lifesider](http://blog.csdn.net/lifesider/article/details/6582338)
当同时存在读写线程时，默认情况下是不保证线程安全的，因而需要利用信号量来进行线程同步（Synchronization），如关键代码段、互斥体等，同时操作系统也提供了相应的API。然而同步并不总是满足条件的且有效率的，比如陷入内核时会有性能损失、死锁、活锁以及资源浪费等。
于是Lock-Free和Wait-Free的思想出现了，由于此时不存在读写线程的同步，因而在写线程运行时，读线程也在运行（多核中两个线程在不同的核上被调度运行），而且代码量减少，程序运行更快。而这一思想是通过CAS机制来实现，如下


```
template<typename T>
bool CAS(T* ptr, T expected, T fresh)
{
    if(*ptr != expected)
         return false;
    *ptr = fresh;
    return true;
}
```
CAS的原理是，将旧值与一个期望值进行比较，如果相等，则更新旧值，类型T = {char, short, int, __int64, …}等，以及指针（pointer to any type）。
注意CAS这里只是说明了原理，并不是真实的源代码实现，具体实现请参考操作系统。
在Windows API中，提供了很多原子操作（Atomic Operatoration），如InterlockedCompareExchange等一系列InterLocked函数，从汇编的角度来讲，intel的XCHG指令即可以一个时钟周期内完成数据的交换（寄存器和内存的数据交换），使用方法可参考InterlockedCompareExchange的反汇编代码。
考虑这样一种情况：存在多个读线程和一个写线程，在使用同步方法时，很可能写线程并不能立即获得锁，最坏的情况下是写线程永远得不到锁，即进入活锁状态。但是使用CAS的方法时，便可以让读写线程并行运行，当写线程一旦更新为新的共享数据时，读线程便能即时读出更新后的数据。
如下示例


```
class Widget
{
    Data* p_;
    ...
    void Use() { ... use p_ ... }
    void Update() {
    Data * pOld, * pNew = new Data;
    do
    { 
    pOld = p_;
    ...
    }while (!CAS(&p_, pOld, pNew));
    }
};
```
但随之而来会有一个疑问，Update函数中该何时删除旧数据呢，由于很有可能有别的读线程在使用旧数据。对于JAVA等有自动内存回收（GC）机制的语言环境而言，这不是问题，但对于C/C++等无GC机制的环境而言，旧数据的回收就比较棘手的问题了。
当然也存在很多的解决方法，这也成为CAS机制中最有趣最受讨论的问题，而且在不同条件下方法也不同，这里便不一一赘述了，具体可以查看“参考文献”部分的论文。
参考文献：
（1）[Andrei Alexandrescu论文集](http://erdani.com/index.php/articles/)
（2）[maged michael论文集](http://www.research.ibm.com/people/m/michael/pubs.htm)
