# Nginx源码剖析之内存池，与内存管理 - 结构之法 算法之道 - CSDN博客





2011年12月04日 23:43:59[v_JULY_v](https://me.csdn.net/v_JULY_v)阅读数：65760标签：[nginx																[alignment																[null																[数据结构																[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[28.Source analysis](https://blog.csdn.net/v_JULY_v/article/category/865324)





> 
> 
> 
> 
> 
### Nginx源码剖析之内存池，与内存管理







作者：dreamice、July、阿波、yixiao。
出处：[http://blog.csdn.net/v_JULY_v/](http://blog.csdn.net/v_JULY_v/)。
### 引言    

    Nginx（发音同 engine x）是一款轻量级的Web 服务器/反向代理服务器及电子邮件（IMAP/POP3）代理服务器，并在一个BSD-like 协议下发行。由俄罗斯的程序设计师Igor Sysoev所开发，最初供俄国大型的入口网站及搜寻引擎Rambler（俄文：Рамблер）使用。  
    其特点是占有内存少，并发能力强，事实上nginx的并发能力确实在同类型的网页服务器中表现较好，目前中国大陆使用nginx网站用户有：新浪、网易、 腾讯，另外知名的微网志Plurk也使用nginx，以及诸多暂不曾得知的玩意儿。

    读者可以到此处下载Nginx最新版本的源码：[http://nginx.org/en/download.html](http://nginx.org/en/download.html)。同时，本文本不想给源码太多注释，因为这不像讲解算法，算法讲解的越通俗易懂越好，而源码剖析则不同，缘由在于不同的读者对同一份源码有着不同的理解，或深或浅，所以，更多的是靠读者自己去思考与领悟。

    ok，本文之中有任何疏漏或不正之处，恳请批评指正。谢谢。

### Nginx源码剖析之内存池

### 1、内存池结构
    内存相关的操作主要在文件 os/unix/ngx_alloc.{h,c} 和 core/ngx_palloc.{h,c} 中实现，ok，咱们先来看内存管理中几个主要的数据结构：

```cpp
typedef struct {	//内存池的数据结构模块
    u_char               *last;    //当前内存分配结束位置，即下一段可分配内存的起始位置
    u_char               *end;     //内存池的结束位置
    ngx_pool_t           *next;    //链接到下一个内存池，内存池的很多块内存就是通过该指针连成链表的
    ngx_uint_t            failed;  //记录内存分配不能满足需求的失败次数
} ngx_pool_data_t;   //结构用来维护内存池的数据块，供用户分配之用。
```

```cpp
struct ngx_pool_t {  //内存池的管理分配模块
    ngx_pool_data_t       d;	     //内存池的数据块（上面已有描述），设为d
    size_t                max;       //数据块大小，小块内存的最大值
    ngx_pool_t           *current;   //指向当前或本内存池
    ngx_chain_t          *chain;     //该指针挂接一个ngx_chain_t结构
    ngx_pool_large_t     *large;     //指向大块内存分配，nginx中，大块内存分配直接采用标准系统接口malloc
    ngx_pool_cleanup_t   *cleanup;   //析构函数，挂载内存释放时需要清理资源的一些必要操作
    ngx_log_t            *log;       //内存分配相关的日志记录
};
```
再来看看大块数据分配的结构体：

```cpp
struct ngx_pool_large_t {
    ngx_pool_large_t     *next;
    void                 *alloc;
};
```
其它的数据结构与相关定义：

```cpp
typedef struct {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *log;
} ngx_pool_cleanup_file_t;
```



```cpp
#define NGX_MAX_ALLOC_FROM_POOL (ngx_pagesize - 1)  //在x86体系结构下，该值一般为4096B，即4K
```



上述这些数据结构的逻辑结构图如下(下图最左上角部分没有与上文的第一个数据结构内的ngx_uint_t对应起来，特此说明)：

![](http://hi.csdn.net/attachment/201112/4/0_1323015018FKrd.gif)


**1.1、ngx_pool_t的逻辑结构**

    再看一下用UML绘制的ngx_pool_t的逻辑结构图：

> 
![](http://hi.csdn.net/attachment/201107/5/0_1309882017mTT4.gif)

    在下一节，我们将会深入分析内存管理的主要函数。

### Nginx源码剖析之内存管理

### **2、内存池操作**

**2.1、创建内存池**



```cpp
ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;
	
	p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
	//ngx_memalign()函数执行内存分配，该函数的实现在src/os/unix/ngx_alloc.c文件中（假定NGX_HAVE_POSIX_MEMALIGN被定义）：
	
    if (p == NULL) {
        return NULL;
    }
	
    p->d.last = (u_char *) p + sizeof(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = NULL;
    p->d.failed = 0;
	
    size = size - sizeof(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL; 
	//最大不超过4095B，别忘了上面NGX_MAX_ALLOC_FROM_POOL的定义
	
    p->current = p;
    p->chain = NULL;
    p->large = NULL;
    p->cleanup = NULL;
    p->log = log;
	
    return p;
}
```



    例如，调用ngx_create_pool(1024, 0x80d1c4c)后，创建的内存池物理结构如下图：

> 
> 
![](http://hi.csdn.net/attachment/201107/5/0_1309882022ZpNM.gif)


    紧接着，咱们就来分析下上面代码中所提到的：ngx_memalign()函数。

```cpp
void *
ngx_memalign(size_t alignment, size_t size, ngx_log_t *log)
{
	void  *p;
	int    err;
	
	err = posix_memalign(&p, alignment, size);
	//该函数分配以alignment为对齐的size字节的内存大小，其中p指向分配的内存块。
	
	if (err) {
		ngx_log_error(NGX_LOG_EMERG, log, err,
			"posix_memalign(%uz, %uz) failed", alignment, size);
		p = NULL;
	}
	
	ngx_log_debug3(NGX_LOG_DEBUG_ALLOC, log, 0,
		"posix_memalign: %p:%uz @%uz", p, size, alignment);
	
	return p;
}
//从这个函数的实现体，我们可以看到p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
//函数分配以NGX_POOL_ALIGNMENT字节对齐的size字节的内存，在src/core/ngx_palloc.h文件中：
```

```cpp
#define NGX_POOL_ALIGNMENT       16
```

    因此，nginx的内存池分配，是以16字节为边界对齐的。
**2.1、销毁内存池**
    接下来，咱们来看内存池的销毁函数，pool指向需要销毁的内存池

```cpp
void
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;
	
    for (c = pool->cleanup; c; c = c->next) {
        if (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
				"run cleanup: %p", c);
            c->handler(c->data);
        }
    }
	//前面讲到，cleanup指向析构函数，用于执行相关的内存池销毁之前的清理工作，如文件的关闭等，
	//清理函数是一个handler的函数指针挂载。因此，在这部分，对内存池中的析构函数遍历调用。
	
    for (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
		
        if (l->alloc) {
            ngx_free(l->alloc);
        }
    }
	//这一部分用于清理大块内存，ngx_free实际上就是标准的free函数，
	//即大内存块就是通过malloc和free操作进行管理的。
	
#if (NGX_DEBUG)
	
    /**
	* we could allocate the pool->log from this pool
	* so we can not use this log while the free()ing the pool
	*/
	
    for (p = pool, n = pool->d.next; /** void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
			"free: %p, unused: %uz", p, p->d.end - p->d.last);
		
        if (n == NULL) {
            break;
        }
    }
	//只有debug模式才会执行这个片段的代码，主要是log记录，用以跟踪函数销毁时日志记录。
#endif
	
    for (p = pool, n = pool->d.next; /** void */; p = n, n = n->d.next) {
        ngx_free(p);
		
        if (n == NULL) {
            break;
        }
    }
}
//该片段彻底销毁内存池本身。
```
    该函数将遍历内存池链表，所有释放内存，如果注册了clenup(也是一个链表结构)，亦将遍历该cleanup链表结构依次调用clenup的handler清理。同时，还将遍历large链表，释放大块内存。 

**2.3、重置内存池**
void ngx_reset_pool(ngx_pool_t *pool)
    重置内存池，将内存池恢复到刚分配时的初始化状态，注意内存池分配的初始状态时，是不包含大块内存的，因此初始状态需要将使用的大块内存释放掉，并把内存池数据结构的各项指针恢复到初始状态值。代码片段如下：

```cpp
void
ngx_reset_pool(ngx_pool_t *pool)
{
	ngx_pool_t        *p;
	ngx_pool_large_t  *l;
	
    for (l = pool->large; l; l = l->next) {
		if (l->alloc) {
			ngx_free(l->alloc);
		}
	}
	//上述片段主要用于清理使用到的大块内存。
	
	pool->large = NULL;
	
    for (p = pool; p; p = p->d.next) {
		p->d.last = (u_char *) p + sizeof(ngx_pool_t);
    }
}
```
    这里虽然重置了内存池，但可以看到并没有释放内存池中被使用的小块内存，而只是将其last指针指向可共分配的内存的初始位置。这样，就省去了内存池的释放和重新分配操作，而达到重置内存池的目的。
    上面我们主要阐述了内存池管理的几个函数，接下来我们深入到如何从内存池中去申请使用内存。


### **2.4、分配内存（重点）**
2.4.1、ngx_palloc 与ngx_pnalloc函数
    这两个函数的参数都为(ngx_pool_t *pool, size_t size)，且返回类型为void*，唯一的区别是ngx_palloc从pool内存池分配以NGX_ALIGNMENT对齐的内存，而ngx_pnalloc分配适合size大小的内存，不考虑内存对齐。
    我们在这里只分析ngx_palloc，对于ngx_pnalloc其实现方式基本类似，便不再赘述。
文件：src/core/ngx_palloc.c

```cpp
void *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    ngx_pool_t  *p;
	
	//判断待分配内存与max值
	//1、小于max值，则从current结点开始遍历pool链表
    if (size <= pool->max) {
		
        p = pool->current; 
		
        do {
			//执行对齐操作，
			//即以last开始，计算以NGX_ALIGNMENT对齐的偏移位置指针，
            m = ngx_align_ptr(p->d.last, NGX_ALIGNMENT);
			
			//然后计算end值减去这个偏移指针位置的大小是否满足索要分配的size大小，
			//如果满足，则移动last指针位置，并返回所分配到的内存地址的起始地址；
            if ((size_t) (p->d.end - m) >= size) {
                p->d.last = m + size;  
				//在该结点指向的内存块中分配size大小的内存
				
                return m;
            }
			
			//如果不满足，则查找下一个链。
            p = p->d.next;
			
        } while (p);
		
		//如果遍历完整个内存池链表均未找到合适大小的内存块供分配，则执行ngx_palloc_block()来分配。
		
		//ngx_palloc_block()函数为该内存池再分配一个block，该block的大小为链表中前面每一个block大小的值。
		//一个内存池是由多个block链接起来的。分配成功后，将该block链入该poll链的最后，
		//同时，为所要分配的size大小的内存进行分配，并返回分配内存的起始地址。
        return ngx_palloc_block(pool, size);    //下文a节分析
		
    }
	//2、如果大于max值，则执行大块内存分配的函数ngx_palloc_large，在large链表里分配内存
    return ngx_palloc_large(pool, size);        //下文b节分析
}
```
    例如，在2.1节中创建的内存池中分配200B的内存，调用ngx_palloc(pool, 200)后，该内存池物理结构如下图：
> 
> 
![](http://hi.csdn.net/attachment/201107/5/0_13098820269Eii.gif)


**a、待分配内存小于max值的情况**
    同样，紧接着，咱们就来分析上述代码中的ngx_palloc_block()函数：

```cpp
static void *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new, *current;
	
    psize = (size_t) (pool->d.end - (u_char *) pool);
	//计算pool的大小，即需要分配的block的大小
	
	m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
	if (m == NULL) {
		return NULL;
	}
	//执行按NGX_POOL_ALIGNMENT对齐方式的内存分配，假设能够分配成功，则继续执行后续代码片段。
	
	//这里计算需要分配的block的大小
    new = (ngx_pool_t *) m;
	
    new->d.end = m + psize;
    new->d.next = NULL;
    new->d.failed = 0;
	//执行该block相关的初始化。
	
    m += sizeof(ngx_pool_data_t);
	//让m指向该块内存ngx_pool_data_t结构体之后数据区起始位置
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;
	//在数据区分配size大小的内存并设置last指针
	
    current = pool->current;
	
    for (p = current; p->d.next; p = p->d.next) {
        if (p->d.failed++ > 4) {
            current = p->d.next;
			//失败4次以上移动current指针
        }
    }
	
    p->d.next = new;
	//将分配的block链入内存池
	
    pool->current = current ? current : new;
	//如果是第一次为内存池分配block，这current将指向新分配的block。
	
    return m;
}
```
    注意：该函数分配一块内存后，last指针指向的是ngx_pool_data_t结构体(大小16B)之后数据区的起始位置，而创建内存池时时，last指针指向的是ngx_pool_t结构体(大小40B)之后数据区的起始位置。 结合2.8节的内存池的物理结构，更容易理解。
**b、待分配内存大于max值的情况**
    如2.4.1节所述，如果分配的内存大小大于max值，代码将跳到ngx_palloc_large(pool, size)位置，
ok，下面进入ngx_palloc_large(pool, size)函数的分析：

```cpp
//这是一个static的函数，说明外部函数不会随便调用，而是提供给内部分配调用的，
//即nginx在进行内存分配需求时，不会自行去判断是否是大块内存还是小块内存，
//而是交由内存分配函数去判断，对于用户需求来说是完全透明的。
static void *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    void              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;
	
    p = ngx_alloc(size, pool->log);  //下文紧接着将分析此ngx_alloc函数
    if (p == NULL) {
        return NULL;
    }
	
    n = 0;
	
	//以下几行，将分配的内存链入pool的large链中，
	//这里指原始pool在之前已经分配过large内存的情况。
    for (large = pool->large; large; large = large->next) {
        if (large->alloc == NULL) {
            large->alloc = p;
            return p;
        }
		
        if (n++ > 3) {
            break;
        }
    }
	
	//如果该pool之前并未分配large内存，则就没有ngx_pool_large_t来管理大块内存
	//执行ngx_pool_large_t结构体的分配，用于来管理large内存块。
    large = ngx_palloc(pool, sizeof(ngx_pool_large_t));
    if (large == NULL) {
        ngx_free(p);
        return NULL;
    }
	
    large->alloc = p;
    large->next = pool->large;
    pool->large = large;
	
    return p;
}
```
    上述代码中，调用ngx_alloc执行内存分配：

```cpp
void *
ngx_alloc(size_t size, ngx_log_t *log)
{
    void  *p;
	
    p = malloc(size);  
	//从这里可以看到，ngx_alloc实际上就是调用malloc函数分配内存的。
	
    if (p == NULL) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
			"malloc() %uz bytes failed", size);
    }
	
    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, log, 0, "malloc: %p:%uz", p, size);
	
    return p;
}
```

2.4.2、ngx_pcalloc与ngx_pmemalign函数

    ngx_pcalloc是直接调用palloc分配好内存，然后进行一次0初始化操作。ngx_pcalloc的源码如下：



```cpp
void *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    void *p;

    p = ngx_palloc(pool, size);
    if (p) {
        ngx_memzero(p, size);
    }

    return p;
}
```
    ngx_pmemalign将在分配size大小的内存并按alignment对齐，然后挂到large字段下，当做大块内存处理。ngx_pmemalign的源码如下：



```cpp
void *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    void              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    if (p == NULL) {
        return NULL;
    }

    large = ngx_palloc(pool, sizeof(ngx_pool_large_t));
    if (large == NULL) {
        ngx_free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}
```



    其余的不再详述。nginx提供给我们使用的内存分配接口，即上述本2.4节中这4种函数，至此，都已分析完毕。



**2.5、释放内存**

```cpp
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
    需要注意的是该函数只释放large链表中注册的内存，普通内存在ngx_destroy_pool中统一释放。
**2.6、注册cleanup**

```cpp
ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;
	
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
**2.7、文件相关**
    一些文件相关的操作函数如下，此处就不在详述了。

```cpp
void
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
	//....
}

void
ngx_pool_cleanup_file(void *data)
{
	//....
}

void
ngx_pool_delete_file(void *data)
{
	//...
}
```

**2.8、内存池的物理结构**
    针对本文前几节的例子，画出的内存池的物理结构如下图。

> ![](http://hi.csdn.net/attachment/201107/5/0_130988203286UE.gif)


    从该图也能看出2.4节的结论，即内存池第一块内存前40字节为ngx_pool_t结构，后续加入的内存块前16个字节为ngx_pool_data_t结构，这两个结构之后便是真正可以分配内存区域。 

### 全文总结

    来自淘宝数据共享平台blog内的一篇文章对上述Nginx源码剖析之内存池，与内存管理总结得很好，特此引用之，作为对上文全文的一个总结：

    Nginx的内存池实现得很精巧，代码也很简洁。总的来说，所有的内存池基本都一个宗旨：申请大块内存，避免“细水长流”。
**3.1、创建一个内存池**
    nginx内存池主要有下面两个结构来维护，他们分别维护了内存池的头部和数据部。此处数据部就是供用户分配小块内存的地方。


```cpp
//该结构用来维护内存池的数据块，供用户分配之用。 
typedef struct { 
	u_char *last; //当前内存分配结束位置，即下一段可分配内存的起始位置 
	u_char *end; //内存池结束位置 
	ngx_pool_t *next; //链接到下一个内存池 
	ngx_uint_t failed; //统计该内存池不能满足分配请求的次数 
} ngx_pool_data_t; 
//该结构维护整个内存池的头部信息。
```

```cpp
struct ngx_pool_s {
	ngx_pool_data_t d; //数据块 
	size_t max;	 //数据块的大小，即小块内存的最大值 
	ngx_pool_t *current;	//保存当前内存池 
	ngx_chain_t *chain;	//可以挂一个chain结构 
	ngx_pool_large_t *large;	//分配大块内存用，即超过max的内存请求 
	ngx_pool_cleanup_t *cleanup;	//挂载一些内存池释放的时候，同时释放的资源。
	ngx_log_t *log; 
};
```
有了上面的两个结构，就可以创建一个内存池了，nginx用来创建一个内存池的接口是：



> 
ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log)（位于src/core/ngx_palloc.c中）;


调用这个函数就可以创建一个大小为size的内存池了。
![](http://hi.csdn.net/attachment/201112/4/0_1323022665nuH9.gif)
    ngx_create_pool接口函数就是分配上图这样的一大块内存，然后初始化好各个头部字段（上图中的彩色部分）。红色表示的四个字段就是来自于上述的第一个结构，维护数据部分，
    由图可知：last是用户从内存池分配新内存的开始位置，end是这块内存池的结束位置，所有分配的内存都不能超过end。蓝色表示的max字段的值等于整个数据部分的长度。**用户请求的内存大于max时，就认为用户请求的是一个大内存，此时需要在紫色表示的large字段下面单独分配；用户请求的内存不大于max的话，就是小内存申请，直接在数据部分分配，此时将会移动last指针**（具体见上文2.4.1节）。

**3.2、分配小块内存(size <= max)**
    上面创建好了一个可用的内存池了，也提到了小块内存的分配问题。nginx提供给用户使用的内存分配接口有：

void *ngx_palloc(ngx_pool_t *pool, size_t size);
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);

ngx_palloc和ngx_pnalloc都是从内存池里分配size大小内存，至于分得的是小块内存还是大块内存，将取决于size的大小；
他们的不同之处在于，palloc取得的内存是对齐的，pnalloc则否。
ngx_pcalloc是直接调用palloc分配好内存，然后进行一次0初始化操作。
ngx_pmemalign将在分配size大小的内存并按alignment对齐，然后挂到large字段下，当做大块内存处理。下面用图形展示一下分配小块内存的模型：
![](http://hi.csdn.net/attachment/201112/4/0_13230227867YcO.gif)

    上图这个内存池模型是由上3个小内存池构成的，由于第一个内存池上剩余的内存不够分配了，于是就创建了第二个新的内存池，第三个内存池是由于前面两个内存池的剩余部分都不够分配，所以创建了第三个内存池来满足用户的需求。
    由图可见：所有的小内存池是由一个单向链表维护在一起的。这里还有两个字段需要关注，failed和current字段。failed表示的是当前这个内存池的剩余可用内存不能满足用户分配请求的次数，即是说：一个分配请求到来后，在这个内存池上分配不到想要的内存，那么就failed就会增加1；这个分配请求将会递交给下一个内存池去处理，如果下一个内存池也不能满足，那么它的failed也会加1，然后将请求继续往下传递，直到满足请求为止（如果没有现成的内存池来满足，会再创建一个新的内存池）。
    current字段会随着failed的增加而发生改变，如果current指向的内存池的failed达到了4的话，current就指向下一个内存池了。猜测：4这个值应该是Nginx作者的经验值，或者是一个统计值（详见上文2.4.1节a部分）。

**3.3、大块内存的分配(size > max)**
    大块内存的分配请求不会直接在内存池上分配内存来满足，而是直接向操作系统申请这么一块内存（就像直接使用malloc分配内存一样），
然后将这块内存挂到内存池头部的large字段下。内存池的作用在于解决小块内存池的频繁申请问题，对于这种大块内存，是可以忍受直接申请的。
    同样，用图形展示大块内存申请模型：
![](http://hi.csdn.net/attachment/201112/4/0_1323022828Quou.gif)
    注意每块大内存都对应有一个头部结构（next&alloc），这个头部结构是用来将所有大内存串成一个链表用的。
这个头部结构不是直接向操作系统申请的，而是当做小块内存（头部结构没几个字节）直接在内存池里申请的。
这样的大块内存在使用完后，可能需要第一时间释放，节省内存空间，因此nginx提供了接口函数：
ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p)；
此函数专门用来释放某个内存池上的某个大块内存，p就是大内存的地址。
ngx_pfree只会释放大内存，不会释放其对应的头部结构，毕竟头部结构是当做小内存在内存池里申请的；遗留下来的头部结构会作下一次申请大内存之用。

**3.4、cleanup资源**
![](http://hi.csdn.net/attachment/201112/4/0_1323022871C1w5.gif)


    可以看到所有挂载在内存池上的资源将形成一个循环链表，一路走来，发现链表这种看似简单的数据结构却被频繁使用。
    由图可知，每个需要清理的资源都对应有一个头部结构，这个结构中有一个关键的字段handler，handler是一个函数指针，在挂载一个资源到内存池上的时候，同时也会注册一个清理资源的函数到这个handler上。即是说，内存池在清理cleanup的时候，就是调用这个handler来清理对应的资源。
    比如：我们可以将一个开打的文件描述符作为资源挂载到内存池上，同时提供一个关闭文件描述的函数注册到handler上，那么内存池在释放的时候，就会调用我们提供的关闭文件函数来处理文件描述符资源了。

**3.5、内存的释放**
    nginx只提供给了用户申请内存的接口，却没有释放内存的接口，那么nginx是如何完成内存释放的呢？总不能一直申请，用不释放啊。针对这个问题，nginx利用了web server应用的特殊场景来完成；
    一个web server总是不停的接受connection和request，所以nginx就将内存池分了不同的等级，有进程级的内存池、connection级的内存池、request级的内存池。
也就是说，创建好一个worker进程的时候，同时为这个worker进程创建一个内存池，待有新的连接到来后，就在worker进程的内存池上为该连接创建起一个内存池；连接上到来一个request后，又在连接的内存池上为request创建起一个内存池。
    这样，在request被处理完后，就会释放request的整个内存池，连接断开后，就会释放连接的内存池。因而，就保证了内存有分配也有释放。

小结：通过内存的分配和释放可以看出，nginx只是将小块内存的申请聚集到一起申请，然后一起释放。避免了频繁申请小内存，降低内存碎片的产生等问题。


### **参考文献**


- 朋友dreamice：http://bbs.chinaunix.net/thread-3626006-1-1.html；
- 友人阿波：http://blog.csdn.net/livelylittlefish/article/details/6586946；
- 朋友dreamice’blog：http://blog.chinaunix.net/space.php?uid=7201775；
- 淘宝数据共享平台博客：http://www.tbdata.org/archives/1390。


### 后记    

    今闲来无事，拿着个nginx源码在编译器上做源码剖析，鼓捣了一下午，至晚上不料中途停电，诸多部分未能保存。然不想白忙活，又花费了一个晚上，终至补全，方成上文，并修订至五日凌晨三点。同时，也参考和借鉴了dreamice、阿波等朋友们及yixiao等大牛的作品，异常感谢。读者若有兴趣，还可以看看sgi stl 的内存池及其管理（或者，日后自个也写下）。

    OK，最后，本文若有任何疏漏之处，望不吝赐教与批评指正。谢谢，完。July、二零一一年十二月五日凌晨。](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=alignment&t=blog)](https://so.csdn.net/so/search/s.do?q=nginx&t=blog)




