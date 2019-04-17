# nginx内存池实现原理 - 囧囧有神的专栏 - CSDN博客





2014年01月03日 17:29:44[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：7326








                
    Nginx以高效，节省内存著称。到底如何高效，如何节省内存，这个得真正了解其设计原理才能知道，分析源码是了解其原理最直接的方法。Nginx对非常多的基础设施（红黑树 内存池 连接池 hash表）都重复造了轮子，我们来看看为什么要这么做。

    对于c系统，最难的常常是内存管理，随着系统复杂度的提高，各种内存问题都出来了，很难管理，对于系统的长期稳定运行构成影响。我们生产线上的nginx常年稳定运行，内存池设计非常精巧，值得学习。


# **工作原理**

    预先分配一大块内存，作为内存池，小块内存申请和释放时，从内存池中分配。大块内存另行分配

内存对齐：分配的内存块地址会进行内存对齐，提高IO效率


优点：

将大量小内存的申请聚集到一块，能够比malloc 更快

减少内存碎片，防止内存泄漏

减少内存管理复杂度

缺点：

造成内存空间浪费，以空间换时间


分配时怎么判断是小块内存还是大块内存呢？

p->max=(size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

#define NGX_MAX_ALLOC_FROM_POOL (ngx_pagesize - 1)

Pagesize为内存一页的大小，x86结构通常为4k

Max为内存池可分配大小和pagesize中较小的一个

如果需要分配的内存大于max，则认为是较大内存，否则为较小内存


内存对齐

#define ngx_align_ptr(p, a) \

　　(u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

m = ngx_align_ptr(m, NGX_ALIGNMENT);

这个宏使得到的内存地址为NGX_ALIGNMENT的倍数。数据对齐，可以避免cpu取值时，要进行两次IO 。


# **数据结构和基本设置**

```cpp
Src/core/ngx_palloc.h
struct ngx_pool_s {
ngx_pool_data_t d; //内存池数据块
size_t max;//内存池数据块最大值
ngx_pool_t *current;//当前内存池的指针
ngx_chain_t *chain;//
ngx_pool_large_t *large;//大块内存链表，分配空间超过max时使用
ngx_pool_cleanup_t *cleanup;//释放内存的callback
ngx_log_t *log;//日志信息
};
Src/core/ngx_palloc.h
typedef struct {
u_char *last;//已分配内存的末尾，下一次分配，从这里开始
u_char *end;//内存池结束位置
ngx_pool_t *next;//链表，指向下一块内存池
ngx_uint_t failed;//内存池分配失败次数
} ngx_pool_data_t;
struct ngx_pool_large_s {
ngx_pool_large_t *next; //用链表组织，指向下一块较大内存
void *alloc;//实际内存地址
};
```




# **基本操作**

Src/core/ngx_palloc.c

基本操作 函数头

创建内存池 ngx_pool_t * ngx_create_pool(size_t size, ngx_log_t *log);

销毁内存池 void ngx_destroy_pool(ngx_pool_t *pool);

重置内存池 void ngx_reset_pool(ngx_pool_t *pool);

内存申请（对齐） void * ngx_palloc(ngx_pool_t *pool, size_t size);

void * ngx_palloc(ngx_pool_t *pool, size_t size); （清零）

内存申请（不对齐） void * ngx_pnalloc(ngx_pool_t *pool, size_t size);

内存释放 ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p);


## 创建内存池后

![](https://img-blog.csdn.net/20140103172206281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



## 内存申请

内存申请ngx_palloc 如果分配较大内存，那么会调用ngx_palloc_large，否则在内存池中分配

分配较小内存后的内存池

![](https://img-blog.csdn.net/20140103172254718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



或者

![](https://img-blog.csdn.net/20140103172321531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



分配较大内存后的内存池

![](https://img-blog.csdn.net/20140103172357531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





## 重置内存池


重置内存池ngx_reset_pool

```cpp
void
ngx_reset_pool(ngx_pool_t *pool)
{
ngx_pool_t *p;
　　ngx_pool_large_t *l;
　　//释放掉所有较大内存
for (l = pool->large; l; l = l->next) {
if (l->alloc) {
ngx_free(l->alloc);
}
}
　　pool->large = NULL;
　　//重置所有较小内存块
for (p = pool; p; p = p->d.next) {
p->d.last = (u_char *) p + sizeof(ngx_pool_t);
}

}
```






## 释放内存池

```cpp
ngx_int_t
ngx_pfree(ngx_pool_t *pool, void *p)
{
ngx_pool_large_t *l;

　　for (l = pool->large; l; l = l->next) {
　　//只有内存块是较大内存块时，才释放掉。较小内存只在摧毁整个内存池时统一销毁
if (p == l->alloc) {
ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
"free: %p", l->alloc);
ngx_free(l->alloc);
l->alloc = NULL;

return NGX_OK;
}
}
return NGX_DECLINED;

}
```







## 销毁内存池

销毁内存池步骤

调用所有cleanup函数，清理数据

释放所有大块内存

释放所有内存池中的内存块

值得关注的是cleanup函数

为什么要有cleanup回调函数？ 因为我们在释放内存的时候，常常伴随需要其他的释放操作，比如释放文件句柄，关闭网络连接等。这些需要在释放内存之前完成。




```cpp
struct ngx_pool_cleanup_s {
ngx_pool_cleanup_pt handler; //回调函数指针
void *data;//执行回调函数时，传入的数据
ngx_pool_cleanup_t *next;//下一个回调函数结构体
};
注册cleanup
ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
ngx_pool_cleanup_t *c;
c = ngx_palloc(p, sizeof(ngx_pool_cleanup_t));
if (c == NULL) {
return NULL;
}
if (size) {
c->data = ngx_palloc(p, size);
if (c->data == NULL) {
return NULL;
}
} else {
c->data = NULL;
}
c->handler = NULL;
c->next = p->cleanup;
p->cleanup = c;
ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);
return c;

}
```




# 使用场景

整个nginx在神马时候建立内存池呢？ 总共会有多少个内存池？ 这些内存池神马时候销毁？

事实上，nginx会不止建立一个内存池，nginx给内存池分了不同的等级，进程级的内存池 connection级的内存池，request级别的内存池 模块也可以有自己的内存池


当worker进程创建时，worker进程也创建了一个内存池，当新的连接建立时，为这个连接创建一个内存池，当得到一个request时，为这个request创建一个连接池

这样，request处理完后，可以释放掉request的整个内存池，连接断开后，释放掉连接的整个内存池


参考了网上大量相关资料，感谢各位前辈





