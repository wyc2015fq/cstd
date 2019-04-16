# 服务器数据库系列 - Nginx源码分析-内存池 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年10月16日 12:52:05[initphp](https://me.csdn.net/initphp)阅读数：719








# Nginx源码分析-内存池


Nginx的内存池实现得很精巧，代码也很简洁。总的来说，所有的内存池基本都一个宗旨：申请大块内存，避免“细水长流”。

# 一、创建一个内存池

nginx内存池主要有下面两个结构来维护，他们分别维护了内存池的头部和数据部。此处数据部就是供用户分配小块内存的地方。
//该结构用来维护内存池的数据块，供用户分配之用。
typedef struct {
u_char *last; //当前内存分配结束位置，即下一段可分配内存的起始位置
u_char *end; //内存池结束位置
ngx_pool_t *next; //链接到下一个内存池
ngx_uint_t failed; //统计该内存池不能满足分配请求的次数
} ngx_pool_data_t;
//该结构维护整个内存池的头部信息。
struct ngx_pool_s {
ngx_pool_data_t d; //数据块
size_t max; //数据块的大小，即小块内存的最大值
ngx_pool_t *current; //保存当前内存池
ngx_chain_t *chain; //可以挂一个chain结构
ngx_pool_large_t *large; //分配大块内存用，即超过max的内存请求
ngx_pool_cleanup_t *cleanup; //挂载一些内存池释放的时候，同时释放的资源。
ngx_log_t *log;
};



有了上面的两个结构，就可以创建一个内存池了，nginx用来创建一个内存池的接口是：ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log)（位于src/core/ngx_palloc.c中）;调用这个函数就可以创建一个大小为size的内存池了。这里，我用内存池的结构图来展示，就不做具体的代码分析了。
[![](http://www.tbdata.org/wp-content/uploads/2011/01/m1.jpg)](http://www.tbdata.org/wp-content/uploads/2011/01/m1.jpg)

ngx_create_pool接口函数就是分配上图这样的一大块内存，然后初始化好各个头部字段（上图中的彩色部分）。红色表示的四个字段就是来自于上述的第一个结构，维护数据部分，由图可知：last是用户从内存池分配新内存的开始位置，end是这块内存池的结束位置，所有分配的内存都不能超过end。蓝色表示的max字段的值等于整个数据部分的长度，用户请求的内存大于max时，就认为用户请求的是一个大内存，此时需要在紫色表示的large字段下面单独分配；用户请求的内存不大于max的话，就是小内存申请，直接在数据部分分配，此时将会移动last指针。

# 二、分配小块内存(size <= max)

上面创建好了一个可用的内存池了，也提到了小块内存的分配问题。nginx提供给用户使用的内存分配接口有：

void *ngx_palloc(ngx_pool_t *pool, size_t size);

void *ngx_pnalloc(ngx_pool_t *pool, size_t size);

void *ngx_pcalloc(ngx_pool_t *pool, size_t size);

void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);

ngx_palloc和ngx_pnalloc都是从内存池里分配size大小内存，至于分得的是小块内存还是大块内存，将取决于size的大小；他们的不同之处在于，palloc取得的内存是对齐的，pnalloc则否。ngx_pcalloc是直接调用palloc分配好内存，然后进行一次0初始化操作。ngx_pmemalign将在分配size大小的内存并按alignment对齐，然后挂到large字段下，当做大块内存处理。下面用图形展示一下分配小块内存的模型：
[![](http://www.tbdata.org/wp-content/uploads/2011/01/m2.jpg)](http://www.tbdata.org/wp-content/uploads/2011/01/m2.jpg)

上图这个内存池模型是由上3个小内存池构成的，由于第一个内存池上剩余的内存不够分配了，于是就创建了第二个新的内存池，第三个内存池是由于前面两个内存池的剩余部分都不够分配，所以创建了第三个内存池来满足用户的需求。由图可见：所有的小内存池是由一个单向链表维护在一起的。这里还有两个字段需要关注，failed和current字段。failed表示的是当前这个内存池的剩余可用内存不能满足用户分配请求的次数，即是说：一个分配请求到来后，在这个内存池上分配不到想要的内存，那么就failed就会增加1；这个分配请求将会递交给下一个内存池去处理，如果下一个内存池也不能满足，那么它的failed也会加1，然后将请求继续往下传递，直到满足请求为止（如果没有现成的内存池来满足，会再创建一个新的内存池）。current字段会随着failed的增加而发生改变，如果current指向的内存池的failed达到了4的话，current就指向下一个内存池了。猜测：4这个值应该是作者的经验值，或者是一个统计值。

# 三、大块内存的分配(size > max)

大块内存的分配请求不会直接在内存池上分配内存来满足，而是直接向操作系统申请这么一块内存（就像直接使用malloc分配内存一样），然后将这块内存挂到内存池头部的large字段下。内存池的作用在于解决小块内存池的频繁申请问题，对于这种大块内存，是可以忍受直接申请的。同样，用图形展示大块内存申请模型：
[![](http://www.tbdata.org/wp-content/uploads/2011/01/m3.jpg)](http://www.tbdata.org/wp-content/uploads/2011/01/m3.jpg)

注意每块大内存都对应有一个头部结构（next&alloc），这个头部结构是用来将所有大内存串成一个链表用的。这个头部结构不是直接向操作系统申请的，而是当做小块内存（头部结构没几个字节）直接在内存池里申请的。这样的大块内存在使用完后，可能需要第一时间释放，节省内存空间，因此nginx提供了接口函数：ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p)；此函数专门用来释放某个内存池上的某个大块内存，p就是大内存的地址。ngx_pfree只会释放大内存，不会释放其对应的头部结构，毕竟头部结构是当做小内存在内存池里申请的；遗留下来的头部结构会作下一次申请大内存之用。

# 四、cleanup资源

[![](http://www.tbdata.org/wp-content/uploads/2011/01/m4.jpg)](http://www.tbdata.org/wp-content/uploads/2011/01/m4.jpg)

可以看到所有挂载在内存池上的资源将形成一个循环链表，一路走来，发现链表这种看似简单的数据结构却被频繁使用。由图可知，每个需要清理的资源都对应有一个头部结构，这个结构中有一个关键的字段handler，handler是一个函数指针，在挂载一个资源到内存池上的时候，同时也会注册一个清理资源的函数到这个handler上。即是说，内存池在清理cleanup的时候，就是调用这个handler来清理对应的资源。比如：我们可以将一个开打的文件描述符作为资源挂载到内存池上，同时提供一个关闭文件描述的函数注册到handler上，那么内存池在释放的时候，就会调用我们提供的关闭文件函数来处理文件描述符资源了。

# 五、内存的释放

nginx只提供给了用户申请内存的接口，却没有释放内存的接口，那么nginx是如何完成内存释放的呢？总不能一直申请，用不释放啊。针对这个问题，nginx利用了web server应用的特殊场景来完成；一个web server总是不停的接受connection和request，所以nginx就将内存池分了不同的等级，有进程级的内存池、connection级的内存池、request级的内存池。也就是说，创建好一个worker进程的时候，同时为这个worker进程创建一个内存池，待有新的连接到来后，就在worker进程的内存池上为该连接创建起一个内存池；连接上到来一个request后，又在连接的内存池上为request创建起一个内存池。这样，在request被处理完后，就会释放request的整个内存池，连接断开后，就会释放连接的内存池。因而，就保证了内存有分配也有释放。

总结：通过内存的分配和释放可以看出，nginx只是将小块内存的申请聚集到一起申请，然后一起释放。避免了频繁申请小内存，降低内存碎片的产生等问题




