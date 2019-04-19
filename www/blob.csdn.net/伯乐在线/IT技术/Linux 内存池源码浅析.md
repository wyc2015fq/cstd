# Linux 内存池源码浅析 - 文章 - 伯乐在线
原文出处： [cengku](http://blog.chinaunix.net/uid-27714502-id-3335217.html)
内存池(Memery Pool)技术是在真正使用内存之前，先申请分配一定数量的、大小相等(一般情况下)的内存块留作备用。当有新的内存需求时，就从内存池中分出一部分内存块，若内存块不够再继续申请新的内存。这样做的一个显著优点是尽量避免了内存碎片，使得内存分配效率得到提升。
不仅在用户态应用程序中被广泛使用，同时在Linux内核也被广泛使用，在内核中有不少地方内存分配不允许失败。作为一个在这些情况下确保分配的方式，内核开发者创建了一个已知为内存池(或者是 “mempool” )的抽象，内核中内存池真实地只是相当于后备缓存，它尽力一直保持一个空闲内存列表给紧急时使用，而在通常情况下有内存需求时还是从公共的内存中直接分配，这样的做法虽然有点霸占内存的嫌疑，但是可以从根本上保证关键应用在内存紧张时申请内存仍然能够成功。
下面看下内核内存池的源码，内核内存池的源码在中，实现上非常简洁，描述内存池的结构mempool_t在头文件中定义，结构描述如下：


```
typedef struct mempool_s {
    spinlock_t lock; /*保护内存池的自旋锁*/
    int min_nr; /*内存池中最少可分配的元素数目*/
    int curr_nr; /*尚余可分配的元素数目*/
    void **elements; /*指向元素池的指针*/
    void *pool_data; /*内存源，即池中元素真实的分配处*/
    mempool_alloc_t *alloc; /*分配元素的方法*/
    mempool_free_t *free; /*回收元素的方法*/
    wait_queue_head_t wait; /*被阻塞的等待队列*/
} mempool_t;
```
内存池的创建函数mempool_create的函数原型如下：


```
mempool_t *mempool_create(int min_nr, mempool_alloc_t *alloc_fn,
                mempool_free_t *free_fn, void *pool_data)
{
    return mempool_create_node(min_nr,alloc_fn,free_fn, pool_data,-1);
}
```
函数原型指定内存池可以容纳元素的个数、申请元素的方法、释放元素的方法，以及一个可选的内存源(通常是一个cache)，内存池对象创建完成后会自动调用alloc方法从pool_data上分配min_nr个元素用来填充内存池。
内存池的释放函数mempool_destory函数的原型很简单，应该也能猜到是依次将元素对象从池中移除，再释放给pool_data，最后释放池对象，如下：


```
void mempool_destroy(mempool_t *pool)
{
    while (pool->curr_nr) {
        void *element = remove_element(pool);
        pool->free(element, pool->pool_data);
    }
    kfree(pool->elements);
    kfree(pool);
}
```
值得注意的是内存池分配和回收对象的函数：mempool_alloc和mempool_free。mempool_alloc的作用是从指定的内存池中申请/获取一个对象，函数原型如下：


```
void * mempool_alloc(mempool_t *pool, gfp_t gfp_mask){
......
    element = pool->alloc(gfp_temp, pool->pool_data);
    if (likely(element != NULL))
        return element;
    spin_lock_irqsave(&pool->lock, flags);
    if (likely(pool->curr_nr)) {
        element = remove_element(pool);/*从内存池中提取一个对象*/
        spin_unlock_irqrestore(&pool->lock, flags);
        /* paired with rmb in mempool_free(), read comment there */
        smp_wmb();
        return element;
    }
......
    
}
```
函数先是从pool_data中申请元素对象，当从pool_data无法成功申请到时，才会从池中提取对象使用，因此可以发现内核内存池mempool其实是一种后备池，在内存紧张的情况下才会真正从池中获取，这样也就能保证在极端情况下申请对象的成功率，单也不一定总是会成功，因为内存池的大小毕竟是有限的，如果内存池中的对象也用完了，那么进程就只能进入睡眠，也就是被加入到pool->wait的等待队列，等待内存池中有可用的对象时被唤醒，重新尝试从池中申请元素：


```
init_wait(&wait);
prepare_to_wait(&pool->wait, &wait, TASK_UNINTERRUPTIBLE);
spin_unlock_irqrestore(&pool->lock, flags);
io_schedule_timeout(5*HZ);
finish_wait(&pool->wait, &wait);
```
池回收对象的函数mempool_free的原型如下：


```
void mempool_free(void *element, mempool_t *pool)
{
	if (pool->curr_nr < pool->min_nr) {
		spin_lock_irqsave(&pool->lock, flags);
		if (pool->curr_nr < pool->min_nr) {
			add_element(pool, element);
			spin_unlock_irqrestore(&pool->lock, flags);
			wake_up(&pool->wait);
			return;
		}
		spin_unlock_irqrestore(&pool->lock, flags);
		}
	pool->free(element, pool->pool_data);
}
```
其实原则跟mempool_alloc是对应的，释放对象时先看池中的可用元素是否充足(pool->curr_nr == pool->min_nr)，如果不是则将元素对象释放回池中，否则将元素对象还给pool->pool_data。
此外mempool也提供或者说指定了几对alloc/free函数，及在mempool_create创建池时必须指定的alloc和free函数，分别适用于不同大小或者类型的元素的内存池，具体如下：


```
void *mempool_alloc_slab(gfp_t gfp_mask, void *pool_data)
{
    struct kmem_cache *mem = pool_data;
    return kmem_cache_alloc(mem, gfp_mask);
}
void mempool_free_slab(void *element, void *pool_data)
{
    struct kmem_cache *mem = pool_data;
    kmem_cache_free(mem, element);
}
void *mempool_kmalloc(gfp_t gfp_mask, void *pool_data)
{
    size_t size = (size_t)pool_data;
    return kmalloc(size, gfp_mask);
}
void mempool_kfree(void *element, void *pool_data)
{
    kfree(element);
}
void *mempool_alloc_pages(gfp_t gfp_mask, void *pool_data)
{
    int order = (int)(long)pool_data;
    return alloc_pages(gfp_mask, order);
}
void mempool_free_pages(void *element, void *pool_data)
{
    int order = (int)(long)pool_data;
    __free_pages(element, order);
}
```
总体上来讲mempool的实现很简约，但是不简单，而且非常轻便易用，这也是内核奥妙之所在。
