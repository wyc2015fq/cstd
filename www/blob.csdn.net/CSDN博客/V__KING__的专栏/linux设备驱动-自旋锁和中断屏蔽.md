# linux设备驱动-自旋锁和中断屏蔽 - V__KING__的专栏 - CSDN博客





2018年07月20日 09:51:59[v__king__](https://me.csdn.net/V__KING__)阅读数：269








## 自旋锁

自旋锁：为防止多处理器并发引入的一种锁 

在内核中，广泛应用于**中断处理**部分

4中自旋锁：
- 自旋锁
- 读写自旋锁
- 顺序锁

> 
顺序锁（seqlock）是对读写锁的一种优化，若使用顺序锁，读执行单元绝不会被写执行单元阻塞，也就是说，读执行单元可以在写执行单元对被顺序锁保护的共享资源进行写操作时仍然可以继续读，而不必等待写执行单元完成写操作，写执行单元也不需要等待所有读执行单元完成读操作才去进行写操作。 

  但是，写执行单元与写执行单元之间仍然是互斥的，即如果有写执行单元在进行写操作，其他写执行单元必须自旋在那里，直到写执行单元释放了顺序锁。

如果读执行单元在读操作期间，写执行单元已经发生了写操作，那么，读执行单元必须重新读取数据，以便确保得到的数据是完整的。这种锁在读写同时进行的概率比较小时，性能是非常好的，而且它允许读写同时进行，因而更大地提高了并发性。

顺序锁有一个限制，它必须要求被保护的共享资源不含有指针，因为写执行单元可能使得指针失效，但读执行单元如果正要访问该指针，将导致Oops

- RCU (Read-Copy Update，读－拷贝－更新)

> 
RCU（Read-Copy Update，读－拷贝－更新），它是基于其原理命名的。RCU 并不是新的锁机制，它 对于Linux 

  内核而言是新的。早在20 世纪80 年代就有了这种机制，而在Linux 系统中，开发2.5.43 内核时引入该技术，并正式包含在2.6 

  内核中。
对于被 RCU 保护的共享数据结构，读执行单元不需要获得任何锁就可以访问它，不使用原子指令， 而且在除Alpha 

  的所有架构上也不需要内存栅（Memory 

  Barrier），因此不会导致锁竞争、内存延迟以及流水线停滞。不需要锁也使得使用更容易，因为死锁问题就不需要考虑了。
使用 RCU 的写执行单元在访问它前需首先复制一个副本，然后对副本进行修改，最后使用一个回调 

  机制在适当的时机把指向原来数据的指针重新指向新的被修改的数据，这个时机就是所有引用该数据的 CPU 

  都退出对共享数据的操作的时候。读执行单元没有任何同步开销，而写执行单元的同步开销则取决于 使用的写执行单元间的同步机制。
RCU 可以看做读写锁的高性能版本，相比读写锁，RCU 的优点在于既允许多个读执行单元同时访问 

  被保护的数据，又允许多个读执行单元和多个写执行单元同时访问被保护的数据。

但是，RCU 不能替代读写锁，因为如果写比较多时，对读执行单元的性能提高不能弥补写执行单元导 致的损失。因为使用RCU 

  时，写执行单元之间的同步开销会比较大，它需要延迟数据结构的释放，复制 

  被修改的数据结构，它也必须使用某种锁机制同步并行的其他写执行单元的修改操作。
使用场景1： 

自旋锁主要针对 SMP 或单 CPU 单内核支持可抢占的情况，使临界区不受别的 CPU 和本 CPU 内的抢占进程打扰*，但是得到锁的代码路径在执行临界区的时候*，还可能受到中断和底半部的影响。解决办法是：

```
spin_lock_irq() = spin_lock() + local_irq_disable() /* 关中断加锁 */
    spin_unlock_irq() = spin_unlock() + local_irq_enable()

    spin_lock_irqsave() = spin_lock() + local_irq_save()/*关中断并保存状态字*/
    spin_lock_irqrestore() = spin_unlock() + local_irq_restore()/*开中断并恢复状态*/

    spin_lock_bh() = spin_lock() + local_bh_disable() /* 关底半部加锁 */
    spin_unlock_bh() = spin_unlock() + local_bh_enable()
```

1.3 例子：

使用自旋锁实现一个设备只能被一个进程打开：

```
int flag = 0;

struct hello_device
{
    char data[128];
    spinlock_t lock;
    struct cdev cdev;
} hello_device;


static int hello_open (struct inode *inode, struct file *file)
{
    spin_lock(&hello_device.lock);
    if ( flag )
    {
        spin_unlock(&hello_device.lock);
        return -EBUSY;
    }
    flag++;
    spin_unlock(&hello_device.lock);

    printk (KERN_INFO "Hey! device opened\n");

    return 0;
}

static int hello_release (struct inode *inode, struct file *file)
{
    spin_lock(&hello_device.lock);
    flag--;
    spin_unlock(&hello_device.lock);
    printk (KERN_INFO "Hmmm... device closed\n");

    return 0;
}
```

## 中断屏蔽

local_irq_disable()和local_irq_enable()都只能禁止和使能本CPU 内的中断，因此，并不能解决**SMP 多CPU 引发的竞态**。因此**，单独使用中断屏蔽通常不是一种值得推荐的避免竞态的方法，它适宜与自旋锁联合使用**。 

中断屏蔽的使用方法为：

```
local_irq_disable() //屏蔽中断
...
critical section //临界区
...
local_irq_enable() //开中断
```

与 local_irq_disable()不同的是，local_irq_save（flags）除了进行禁止中断的操作以外，还保存目前CPU的中断位信息，local_irq_restore（flags）进行的是与local_irq_save（flags）相反的操作。如果只是想禁止中断的底半部，应使用local_bh_disable()，使能被local_bh_disable()禁止的底半部应该调用local_bh_enable()。

# #








