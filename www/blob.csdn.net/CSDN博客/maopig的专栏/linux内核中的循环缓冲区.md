# linux内核中的循环缓冲区 - maopig的专栏 - CSDN博客
2012年03月25日 19:21:36[maopig](https://me.csdn.net/maopig)阅读数：965
      Linux内核中的循环缓冲区（circular buffer）为解决某些特殊情况下的竞争问题提供了一种免锁的方法。这种特殊的情况就是当生产者和消费者都只有一个，而在其它情况下使用它也是必须要加锁的。
**循环缓冲区定义在include/linux/kfifo.h**中，如下：
struct kfifo { 
unsigned char *buffer; // buffer指向存放数据的缓冲区 
unsigned int size;        // size是缓冲区的大小 
unsigned int in;           // in是写指针下标 
unsigned int out;        // out是读指针下标 
spinlock_t *lock;         // lock是加到struct kfifo上的自旋锁 
};  
      （上面说的免锁不是免这里的锁，这个锁是必须的），防止多个进程并发访问此数据结构。当in==out时，说明缓冲区为空；当(in-out)==size时，说明缓冲区已满。
       为kfifo提供的接口可以分为两类: 
       一类是满足上述情况下使用的，以双下划线开头，没有加锁的； 
       另一类是在不满足的条件下，即需要额外加锁的情况下使用的。 
       其实后一类只是在前一类的基础上进行加锁后的包装（也有一处进行了小的改进），实现中所加的锁是spin_lock_irqsave。
**清空缓冲区的函数： **static inline void __kfifo_reset(struct kfifo *fifo); 
static inline void kfifo_reset(struct kfifo *fifo); 
这很简单，直接把读写指针都置为0即可。
**向缓冲区里放入数据的接口是： **static inline unsigned int kfifo_put(struct kfifo *fifo, unsigned char *buffer, unsigned int len);
unsigned int __kfifo_put(struct kfifo *fifo, unsigned char *buffer, unsigned int len);
后者是在kernel/kfifo.c中定义的。这个接口是经过精心构造的，可以小心地避免一些边界情况。我们有必要一起来看一下它的具体实现。
 1: /** 2: * __kfifo_put - puts some data into the FIFO, no locking version 3: * @fifo: the fifo to be used. 4: * @buffer: the data to be added. 5: * @len: the length of the data to be added. 6: * 7: * This function copies at most @len bytes from the @buffer into 8: * the FIFO depending on the free space, and returns the number of 9: * bytes copied. 10: * 11: * Note that with only one concurrent reader and one concurrent 12: * writer, you don't need extra locking to use these functions. 13: */ 14: unsigned int __kfifo_put(struct kfifo *fifo, 15: const unsigned char *buffer, unsigned int len) 16: { 17: unsigned int l; 18:  19: len = min(len, fifo->size - fifo->in + fifo->out); 20:  21: /* 22: * Ensure that we sample the fifo->out index -before- we 23: * start putting bytes into the kfifo. 24: */ 25:  26: smp_mb(); 27:  28: /* first put the data starting from fifo->in to buffer end */ 29: l = min(len, fifo->size - (fifo->in & (fifo->size - 1))); 30: memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l); 31:  32: /* then put the rest (if any) at the beginning of the buffer */ 33: memcpy(fifo->buffer, buffer + l, len - l); 34:  35: /* 36: * Ensure that we add the bytes to the kfifo -before- 37: * we update the fifo->in index. 38: */ 39:  40: smp_wmb(); 41:  42: fifo->in += len; 43:  44: return len; 45: } 46: EXPORT_SYMBOL(__kfifo_put);
 1: /** 2: * kfifo_put - puts some data into the FIFO 3: * @fifo: the fifo to be used. 4: * @buffer: the data to be added. 5: * @len: the length of the data to be added. 6: * 7: * This function copies at most @len bytes from the @buffer into 8: * the FIFO depending on the free space, and returns the number of 9: * bytes copied. 10: */ 11: static inline unsigned int kfifo_put(struct kfifo *fifo, 12: const unsigned char *buffer, unsigned int len) 13: { 14: unsigned long 15: unsigned int ret; 16:  17: spin_lock_irqsave(fifo->lock, flags); 18:  19: ret = __kfifo_put(fifo, buffer, len); 20:  21: spin_unlock_irqrestore(fifo->lock, flags); 22:  23: return ret; 24: }
**len = min(len, fifo->size - fifo->in + fifo->out); **      在 **len **和 **(fifo->size - fifo->in + fifo->out)** 之间取一个较小的值赋给len。注意，当** (fifo->in == fifo->out+fifo->size)** 时，表示缓冲区已满，此时得到的较小值一定是0，后面实际写入的字节数也全为0。
      另一种边界情况是当 **len** 很大时（因为len是无符号的，负数对它来说也是一个很大的正数），这一句也能保证len取到一个较小的值，因为    **fifo->in** 总是大于等于** fifo->out** ，所以后面的那个表达式
**l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));** 的值不会超过**fifo->size**的大小。
**smp_mb();  smp_wmb();** 是加内存屏障，这里不是我们讨论的范围，你可以忽略它。 
**   l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));**    是把上一步决定的要写入的字节数**len** “切开”，这里又使用了一个技巧。注意：实际分配给
**fifo->buffer** 的字节数** fifo->size**，必须是2的幂，否则这里就会出错。既然** fifo->size** 是2的幂，那么** (fifo->size-1)** 也就是一个后面几位全为1的数，也就能保证**(fifo->in & (fifo->size - 1))** 总为不超过** (fifo->size - 1)**的那一部分，和** (fifo->in)% (fifo->size - 1) **的效果一样。 
      这样后面的代码就不难理解了，**它先向  fifo->in  到缓冲区末端这一块写数据，如果还没写完，在从缓冲区头开始写入剩下的，从而实现了循环缓冲。最后，把写指针后移 len 个字节，并返回len。**       从上面可以看出，fifo->in的值可以从0变化到超过fifo->size的数值，fifo->out也如此，但它们的差不会超过fifo->size。
**从kfifo向外读数据的函数是： **static inline unsigned int kfifo_get(struct kfifo *fifo, unsigned char *buffer, unsigned int len);
unsigned int __kfifo_get(struct kfifo *fifo, unsigned char *buffer, unsigned int len);
 1:  2: /** 3: * __kfifo_get - gets some data from the FIFO, no locking version 4: * @fifo: the fifo to be used. 5: * @buffer: where the data must be copied. 6: * @len: the size of the destination buffer. 7: * 8: * This function copies at most @len bytes from the FIFO into the 9: * @buffer and returns the number of copied bytes. 10: * 11: * Note that with only one concurrent reader and one concurrent 12: * writer, you don't need extra locking to use these functions. 13: */ 14: unsigned int __kfifo_get(struct kfifo *fifo, 15: unsigned char *buffer, unsigned int len) 16: { 17: unsigned int l; 18:  19: len = min(len, fifo->in - fifo->out); 20:  21: /* 22: * Ensure that we sample the fifo->in index -before- we 23: * start removing bytes from the kfifo. 24: */ 25:  26: smp_rmb(); 27:  28: /* first get the data from fifo->out until the end of the buffer */ 29: l = min(len, fifo->size - (fifo->out & (fifo->size - 1))); 30: memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l); 31:  32: /* then get the rest (if any) from the beginning of the buffer */ 33: memcpy(buffer + l, fifo->buffer, len - l); 34:  35: /* 36: * Ensure that we remove the bytes from the kfifo -before- 37: * we update the fifo->out index. 38: */ 39:  40: smp_mb(); 41:  42: fifo->out += len; 43:  44: return len; 45: } 46: EXPORT_SYMBOL(__kfifo_get);
 1:  2: /** 3: * kfifo_get - gets some data from the FIFO 4: * @fifo: the fifo to be used. 5: * @buffer: where the data must be copied. 6: * @len: the size of the destination buffer. 7: * 8: * This function copies at most @len bytes from the FIFO into the 9: * @buffer and returns the number of copied bytes. 10: */ 11: static inline unsigned int kfifo_get(struct kfifo *fifo, 12: unsigned char *buffer, unsigned int len) 13: { 14: unsigned long flags; 15: unsigned int ret; 16:  17: spin_lock_irqsave(fifo->lock, flags); 18:  19: ret = __kfifo_get(fifo, buffer, len); 20:  21: /* 22: * optimization: if the FIFO is empty, set the indices to 0 23: * so we don't wrap the next time 24: */ 25: if (fifo->in == fifo->out) 26: fifo->in = fifo->out = 0; 27:  28: spin_unlock_irqrestore(fifo->lock, flags); 29:  30: return ret; 31: }
和上面的__kfifo_put类似，不难分析。
static inline unsigned int __kfifo_len(struct kfifo *fifo); 
static inline unsigned int kfifo_len(struct kfifo *fifo);
 1:  2: /** 3: * __kfifo_len - returns the number of bytes available in the FIFO, no locking version 4: * @fifo: the fifo to be used. 5: */ 6: static inline unsigned int __kfifo_len(struct kfifo *fifo) 7: { 8: return fifo->in - fifo->out; 9: } 10:  11: /** 12: * kfifo_len - returns the number of bytes available in the FIFO 13: * @fifo: the fifo to be used. 14: */ 15: static inline unsigned int kfifo_len(struct kfifo *fifo) 16: { 17: unsigned long flags; 18: unsigned int ret; 19:  20: spin_lock_irqsave(fifo->lock, flags); 21:  22: ret = __kfifo_len(fifo); 23:  24: spin_unlock_irqrestore(fifo->lock, flags); 25:  26: return ret; 27: }
这两个函数返回缓冲区中实际的字节数，只要用fifo->in减去fifo->out即可。
kernel/kfifo.c中还提供了初始化kfifo，分配和释放kfifo的接口：
struct kfifo *kfifo_init(unsigned char *buffer, unsigned int size, gfp_t gfp_mask, spinlock_t *lock);
struct kfifo *kfifo_alloc(unsigned int size, gfp_t gfp_mask, spinlock_t *lock);
void kfifo_free(struct kfifo *fifo);
**再一次强调，调用kfifo_init必须保证size是2的幂**，而kfifo_alloc不必，它内部会把size向上圆到2的幂。kfifo_alloc和kfifo_free搭配使用，因为这两个函数会为fifo->buffer分配/释放内存空间。而kfifo_init只会接受一个已分配好空间的fifo->buffer，不能和kfifo->free搭配，用kfifo_init分配的kfifo只能用kfree释放。
循环缓冲区在驱动程序中使用较多，尤其是网络适配器。但这种免锁的方式在内核互斥中使用较少，取而代之的是另一种高级的互斥机制──RCU。
