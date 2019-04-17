# LDD3源码分析之slab高速缓存 - 一世豁然的专栏 - CSDN博客





2014年12月18日 16:32:47[一世豁然](https://me.csdn.net/Explorer_day)阅读数：668








本博客转载于：[http://blog.csdn.net/liuhaoyutz/article/details/7415466](http://blog.csdn.net/liuhaoyutz/article/details/7415466) [作者：刘昊昱]





本文分析LDD3第8章中关于使用slab高速缓存的代码，对应的源码在scullc目录下。另外，在较新的内核下编译scullc时会遇到一些错误，本文最后给出了解决这些错误的方法。一、scullc源码分析首先介绍一下slab相关的概念和函数。我们编写程序时，如果经常为某种数据结构进行分配和释放内存空间的操作，常常会用到空闲链表，其中包含多个已经分配好的可供使用的数据结构内存块，当需要使用数据结构时，直接去链表中去取，然后把数据放进去；使用完后，再把数据结构放回空闲链表，以供以后使用。Linux内核中也有类似的需求，但是空闲链表的问题是不能全局控制，当内存紧缺时，内核无法通知每个空闲链表，让其释放出一些内存空间来。为此，Linux内核提供了slab分配器。简单理解，slab就是内核维护的一组大小不同的空闲链表，Linux把每个空闲链表称为后备高速缓存(lookaside cache)，当需要使用时，可以从中取出需要的内存块，不用时，再把内存块归还给slab。slab维护的后备高速缓存是kmem_cache_t类型，可以由kmem_cache_create函数创建：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)

- kmem_cache_t *kmem_cache_create(constchar *name, size_t size,  
- size_t offset,   
-                                 unsigned long flags,  
- void (*constructor)(void *, kmem_cache_t *,  
-                                                     unsigned long flags),  
- void (*destructor)(void *, kmem_cache_t *,  
-                                                    unsigned long flags));  

kmem_cache_create函数创建一个新的后备高速缓存，其中可以容纳任意数目的内存块，这些内存块的大小都相同，由size参数指定。使用kmem_cache_create创建了某个对象的后备高速缓存后，就可以调用kmem_cache_alloc从中分配内存对象：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)

- void *kmem_cache_alloc(kmem_cache_t *cache, int flags);  

释放一个内存对象使用kmem_cache_free函数：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)

- void kmem_cache_free(kmem_cache_t *cache, constvoid *obj);  

如果驱动程序不会再使用后备高速缓存了，例如模块被卸载时，应该调用kmem_cache_destroy函数释放后备高速缓存：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)

- int kmem_cache_destroy(kmem_cache_t *cache);  

我们知道了slab的相关概念和操作函数，下面可以看scullc的代码了。scullc和前面分析过的scull绝大部分代码都是相同的，他们的区别在内存分配上，scullc使用slab分配内存，而scull使用kmalloc。下面我们只分析scullc与scull不同的代码，其他代码如果有问题，大家可以参考前面分析scull的相关文章。首先看scullc的模块初始化函数scullc_init，其中需要分析的是如下语句：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)

- 560    scullc_cache = kmem_cache_create("scullc", scullc_quantum,  
- 561            0, SLAB_HWCACHE_ALIGN, NULL, NULL); /* no ctor/dtor */
- 562    if (!scullc_cache) {  
- 563        scullc_cleanup();  
- 564        return -ENOMEM;  
- 565    }  

scullc_cache定义在52行：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)

- 51/* declare one cache pointer: use it for all devices */
- 52kmem_cache_t *scullc_cache;  

可以看出，在scullc的模块初始化函数中，创建了一个slab后备高速缓存，其关联的名称叫scullc，其中包含的内存块的大小是scullc_quantum(默认值4000)，每个内存块即是驱动程序中的一个量子。现在有了后备高速缓存，下面scullc可以从中为量子分配内存块了，相关代码在scullc_write函数中，其中有必要分析的是如下语句：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)

- 245    /* Allocate a quantum using the memory cache */
- 246    if (!dptr->data[s_pos]) {  
- 247        dptr->data[s_pos] = kmem_cache_alloc(scullc_cache, GFP_KERNEL);  
- 248        if (!dptr->data[s_pos])  
- 249            goto nomem;  
- 250        memset(dptr->data[s_pos], 0, scullc_quantum);  
- 251    }  

247行，从scullc_cache指向的后备高速缓存中分配了一个量子内存块。scullc不再使用量子内存块时，应该返回给后备高速缓存，完成这项工作的函数是scullc_trim，其中关键的代码如下：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)

- 488            for (i = 0; i < qset; i++)  
- 489                if (dptr->data[i])  
- 490                    kmem_cache_free(scullc_cache, dptr->data[i]);  

最后，scullc模块卸载时，必须把后备高速缓存返回给系统，在scullc_cleanup函数中，有如下语句：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)

- 593    if (scullc_cache)  
- 594        kmem_cache_destroy(scullc_cache);  

与scull相比，scullc的最大区别是运行速度略有提高，对内存利用率更高。由于后备调整缓存中的内存块都是同样的大小，所以其在内存中的排列位置达到了最大密集程度，相反，scull的数据对象则会引起不可预测的内存碎片。在我的机器上，测试scullc模块过程如下图所示：二、编译scullc时遇到的问题在2.6.32-38-generic-pae上编译LDD3提供的scullc模块时，会出现如下错误：把Makefile中第12行和第42行的CFLAGS替换为EXTRA_CFLAGS即可解决上面的错误。再次make，会出现如下错误：因为linux/config.h现在已经不存在了，所以直接把main.c的第18行去掉，即可解决这个错误，再次编译，出现如下错误信息：这是因为新内核中不再使用kmem_cache_t为个类型定义，而是使用struct kmem_cache结构体代表一个后备高速缓存。所以把main.c的第51行改为


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)

- 51struct kmem_cache *scullc_cache;  

即可解决这个问题，再次make，又出现如下错误：这是因为在新内核中INIT_WORK宏发生了变化，现在INIT_WORK只能接受两个参数，所以将439行改为：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)

- 439    INIT_WORK(&stuff->work, scullc_do_deferred_op);  

注意，新的INIT_WORK的第二个参数scullc_do_deferred_op函数以INIT_WORK的第一个参数做为参数。连锁反应，还需要把scullc_do_deferred_op函数的实现修改如下：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)

- 409static void scullc_do_deferred_op(struct work_struct *p)  
- 410{  
- 411    struct async_work *stuff = container_of(p, struct async_work, work);  
- 412    aio_complete(stuff->iocb, stuff->result, 0);  
- 413    kfree(stuff);  
- 414}  

409行和411行发生了变化。再次编译，出现如下错误：这是因为，在新内核中，kmem_cache_create函数发生了变化，最后一个参数destructor被删除掉了。所以，把560行最后一个参数NULL删除即可解决这个问题。再次编译，编译通过，但是还有几个警告信息，如下图所示：
这是因为，在2.6.32-38-generic-pae内核中，schedule_delayed_work函数的定义发生了变化，现在其函数原型是



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)

- int schedule_delayed_work(struct delayed_work *dwork, unsigned long delay)  


而在LDD3使用的2.6.10版本的内核中，其函数原型是：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)

- int fastcall schedule_delayed_work(struct work_struct *work, unsigned long delay)  


所以要在新内核上执行schedule_delayed_work，原来的work_struct必须改为delayed_work。

delayed_work结构体定义如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)

- struct delayed_work {  
- struct work_struct work;  
- struct timer_list timer;  
- };  


为了修正这个警告，需要修改如下三个地方：
403行改为：   struct delayed_work work;411行改为：   struct async_work *stuff = container_of(p, struct async_work, work.work);439行改为：   INIT_DELAYED_WORK(&stuff->work, scullc_do_deferred_op);再次编译，还有如下警告信息：这是因为在新内核中，file_operations结构体的aio_read和aio_write成员函数原型发生了变化。所以做如下修改：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7415466#)

- 445static ssize_t scullc_aio_read(struct kiocb *iocb, conststruct iovec *buf, unsigned long count,  
- 446        loff_t pos)  
- 447{  
- 448    return scullc_defer_op(0, iocb, (char __user *) buf, count, pos);  
- 449}  
- 450  
- 451static ssize_t scullc_aio_write(struct kiocb *iocb, conststruct iovec *buf,  
- 452        unsigned long count, loff_t pos)  
- 453{  
- 454    return scullc_defer_op(1, iocb, (char __user *) buf, count, pos);  
- 455}  

改动的地方是445行和451行两个函数的函数原型，使参数类型符合新的定义。另外还修改了448行，把第三个参数强制转换为char __user*类型。修改后，编译成功，如下图所示：





