# Nginx源码分析 - 基础数据结构篇 - 内存池 ngx_palloc.c - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年01月26日 17:38:28[initphp](https://me.csdn.net/initphp)阅读数：4610
个人分类：[Nginx源码分析](https://blog.csdn.net/initphp/article/category/6081681)

所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









Nginx的内存管理是通过内存池来实现的。Nginx的内存池的设计非常的精巧，很多场景下，我们可以将Nginx的内存池实现抽象出来改造成我们开发中的内存池。




### 内存池

一般我们使用malloc/alloc/free等函数来分配和释放内存。但是直接使用这些函数会有一些弊端：

1. 虽然系统自带的ptmalloc内存分配管理器，也有自己的内存优化管理方案（申请内存块以及将内存交还给系统都有自己的优化方案，具体可以研究一下ptmalloc的源码），但是直接使用malloc/alloc/free，仍然会导致内存分配的性能比较低。

2. 频繁使用这些函数分配和释放内存，会导致内存碎片，不容易让系统直接回收内存。典型的例子就是大并发频繁分配和回收内存，会导致进程的内存产生碎片，并且不会立马被系统回收。

3. 容易产生内存泄露。





使用内存池分配内存有几点好处：

1. 提升内存分配效率。不需要每次分配内存都执行malloc/alloc等函数。

2. 让内存的管理变得更加简单。内存的分配都会在一块大的内存上，回收的时候只需要回收大块内存就能将所有的内存回收，防止了内存管理混乱和内存泄露问题。




### 数据结构定义

#### ngx_pool_t 内存池主结构



```cpp
/**
 * Nginx 内存池数据结构
 */
struct ngx_pool_s {
    ngx_pool_data_t       d; 		/* 内存池的数据区域*/
    size_t                max; 		/* 最大每次可分配内存 */
    ngx_pool_t           *current;  /* 指向当前的内存池指针地址。ngx_pool_t链表上最后一个缓存池结构*/
    ngx_chain_t          *chain;	/* 缓冲区链表 */
    ngx_pool_large_t     *large;    /* 存储大数据的链表 */
    ngx_pool_cleanup_t   *cleanup;  /* 可自定义回调函数，清除内存块分配的内存 */
    ngx_log_t            *log;      /* 日志 */
};
```

#### ngx_pool_data_t 数据区域结构



```cpp
typedef struct {
    u_char               *last;  /* 内存池中未使用内存的开始节点地址 */
    u_char               *end;   /* 内存池的结束地址 */
    ngx_pool_t           *next;  /* 指向下一个内存池 */
    ngx_uint_t            failed;/* 失败次数 */
} ngx_pool_data_t;
```

#### ngx_pool_large_t 大数据块结构



```cpp
struct ngx_pool_large_s {
    ngx_pool_large_t     *next;   /* 指向下一个存储地址 通过这个地址可以知道当前块长度 */
    void                 *alloc;  /* 数据块指针地址 */
};
```



#### ngx_pool_cleanup_t 自定义清理回调的数据结构



```cpp
struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;  /* 清理的回调函数 */
    void                 *data; 	/* 指向存储的数据地址 */
    ngx_pool_cleanup_t   *next; 	/* 下一个ngx_pool_cleanup_t */
};
```




### 数据结构图

说明：

1. Nginx的内存池会放在ngx_pool_t的数据结构上（ngx_pool_data_t用于记录内存块block的可用地址空间和内存块尾部）。当初始化分配的内存块大小不能满足需求的时候，Nginx就会调用ngx_palloc_block函数来分配一个新的内存块，通过链表的形式连接起来。

2. 当申请的内存大于pool->max的值的时候，Nginx就会单独分配一块large的内存块，会放置在**pool->large**的链表结构上。

3. **pool->cleanup**的链表结构主要存放需要通过回调函数清理的内存数据。（例如文件描述符）


![](https://img-blog.csdn.net/20160217111839751?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 具体函数实现

#### 内存分配 ngx_alloc和ngx_calloc

ngx_alloc和ngx_calloc 主要封装了Nginx的内存分配函数malloc。



```cpp
/**
 * 封装了malloc函数，并且添加了日志
 */
void *
ngx_alloc(size_t size, ngx_log_t *log)
{
    void  *p;
    //分配一块内存
    p = malloc(size);
    if (p == NULL) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      "malloc(%uz) failed", size);
    }

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, log, 0, "malloc: %p:%uz", p, size);

    return p;
}

/**
 * 调用ngx_alloc方法，如果分配成，则调用ngx_memzero方法，将内存块设置为0
 * #define ngx_memzero(buf, n)  (void) memset(buf, 0, n)
 */
void *
ngx_calloc(size_t size, ngx_log_t *log)
{
    void  *p;

    //调用内存分配函数
    p = ngx_alloc(size, log);

    if (p) {
    	//将内存块全部设置为0
        ngx_memzero(p, size);
    }

    return p;
}
```

#### 创建内存池ngx_create_pool

调用ngx_create_pool这个方法就可以创建一个内存池。



```cpp
/**
 * 创建一个内存池
 */
ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log) {
	ngx_pool_t *p;

	/**
	 * 相当于分配一块内存 ngx_alloc(size, log)
	 */
	p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
	if (p == NULL) {
		return NULL;
	}

	/**
	 * Nginx会分配一块大内存，其中内存头部存放ngx_pool_t本身内存池的数据结构
	 * ngx_pool_data_t	p->d 存放内存池的数据部分（适合小于p->max的内存块存储）
	 * p->large 存放大内存块列表
	 * p->cleanup 存放可以被回调函数清理的内存块（该内存块不一定会在内存池上面分配）
	 */
	p->d.last = (u_char *) p + sizeof(ngx_pool_t); //内存开始地址，指向ngx_pool_t结构体之后数据取起始位置
	p->d.end = (u_char *) p + size; //内存结束地址
	p->d.next = NULL; //下一个ngx_pool_t 内存池地址
	p->d.failed = 0; //失败次数

	size = size - sizeof(ngx_pool_t);
	p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

	/* 只有缓存池的父节点，才会用到下面的这些  ，子节点只挂载在p->d.next,并且只负责p->d的数据内容*/
	p->current = p;
	p->chain = NULL;
	p->large = NULL;
	p->cleanup = NULL;
	p->log = log;

	return p;
}
```



#### 销毁内存池ngx_destroy_pool



```cpp
/**
 * 销毁内存池。
 */
void ngx_destroy_pool(ngx_pool_t *pool) {
	ngx_pool_t *p, *n;
	ngx_pool_large_t *l;
	ngx_pool_cleanup_t *c;

	/* 首先清理pool->cleanup链表 */
	for (c = pool->cleanup; c; c = c->next) {
		/* handler 为一个清理的回调函数 */
		if (c->handler) {
			ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
					"run cleanup: %p", c);
			c->handler(c->data);
		}
	}

	/* 清理pool->large链表（pool->large为单独的大数据内存块）  */
	for (l = pool->large; l; l = l->next) {

		ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);

		if (l->alloc) {
			ngx_free(l->alloc);
		}
	}

#if (NGX_DEBUG)

	/*
	 * we could allocate the pool->log from this pool
	 * so we cannot use this log while free()ing the pool
	 */

	for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
		ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
				"free: %p, unused: %uz", p, p->d.end - p->d.last);

		if (n == NULL) {
			break;
		}
	}

#endif

	/* 对内存池的data数据区域进行释放 */
	for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
		ngx_free(p);

		if (n == NULL) {
			break;
		}
	}
}
```



#### 重设内存池ngx_reset_pool



```cpp
/**
 * 重设内存池
 */
void ngx_reset_pool(ngx_pool_t *pool) {
	ngx_pool_t *p;
	ngx_pool_large_t *l;

	/* 清理pool->large链表（pool->large为单独的大数据内存块）  */
	for (l = pool->large; l; l = l->next) {
		if (l->alloc) {
			ngx_free(l->alloc);
		}
	}

	pool->large = NULL;

	/* 循环重新设置内存池data区域的 p->d.last；data区域数据并不擦除*/
	for (p = pool; p; p = p->d.next) {
		p->d.last = (u_char *) p + sizeof(ngx_pool_t);
	}
}
```



#### 使用内存池分配一块内存ngx_palloc和ngx_pnalloc



```cpp
/**
 * 内存池分配一块内存，返回void类型指针
 */
void *
ngx_palloc(ngx_pool_t *pool, size_t size) {
	u_char *m;
	ngx_pool_t *p;

	/* 判断每次分配的内存大小，如果超出pool->max的限制，则需要走大数据内存分配策略 */
	if (size <= pool->max) {

		p = pool->current;

		/*
		 * 循环读取缓存池链p->d.next的各个的ngx_pool_t节点，
		 * 如果剩余的空间可以容纳size，则返回指针地址
		 *
		 * 这边的循环，实际上最多只有4次，具体可以看ngx_palloc_block函数
		 * */
		do {
			/* 对齐操作,会损失内存，但是提高内存使用速度 */
			m = ngx_align_ptr(p->d.last, NGX_ALIGNMENT);

			if ((size_t)(p->d.end - m) >= size) {
				p->d.last = m + size;

				return m;
			}

			p = p->d.next;

		} while (p);

		/* 如果没有缓存池空间没有可以容纳大小为size的内存块，则需要重新申请一个缓存池pool节点 */
		return ngx_palloc_block(pool, size);
	}

	/* 走大数据分配策略 ，在pool->large链表上分配 */
	return ngx_palloc_large(pool, size);
}

/**
 * 内存池分配一块内存，返回void类型指针
 * 不考虑对齐情况
 */
void *
ngx_pnalloc(ngx_pool_t *pool, size_t size) {
	u_char *m;
	ngx_pool_t *p;

	/* 判断每次分配的内存大小，如果超出pool->max的限制，则需要走大数据内存分配策略 */
	if (size <= pool->max) {

		p = pool->current;

		/* 循环读取数据区域的各个ngx_pool_t缓存池链，如果剩余的空间可以容纳size，则返回指针地址*/
		do {
			m = p->d.last; //分配的内存块的地址

			if ((size_t)(p->d.end - m) >= size) {
				p->d.last = m + size;

				return m;
			}

			p = p->d.next;

		} while (p);

		/* 如果没有缓存池空间没有可以容纳大小为size的内存块，则需要重新申请一个缓存池*/
		return ngx_palloc_block(pool, size);
	}

	/* 走大数据分配策略 */
	return ngx_palloc_large(pool, size);
}
```

**内存分配逻辑:**


1. 分配一块内存，如果分配的内存size小于内存池的pool->max的限制，则属于小内存块分配，走小内存块分配逻辑；否则走大内存分配逻辑。

2. 小内存分配逻辑：循环读取pool->d上的内存块，是否有足够的空间容纳需要分配的size，如果可以容纳，则直接分配内存；否则内存池需要申请新的内存块，调用ngx_palloc_block。

3. 大内存分配逻辑：当分配的内存size大于内存池的pool->max的限制，则会直接调用ngx_palloc_large方法申请一块独立的内存块，并且将内存块挂载到pool->large的链表上进行统一管理。




ngx_palloc_block，内存池扩容：



```cpp
/**
 * 申请一个新的缓存池 ngx_pool_t
 * 新的缓存池会挂载在主缓存池的 数据区域 （pool->d->next）
 */
static void *
ngx_palloc_block(ngx_pool_t *pool, size_t size) {
	u_char *m;
	size_t psize;
	ngx_pool_t *p, *new, *current;

	psize = (size_t)(pool->d.end - (u_char *) pool);

	/* 申请新的块 */
	m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
	if (m == NULL) {
		return NULL;
	}

	new = (ngx_pool_t *) m;

	new->d.end = m + psize;
	new->d.next = NULL;
	new->d.failed = 0;

	/* 分配size大小的内存块，返回m指针地址 */
	m += sizeof(ngx_pool_data_t);
	m = ngx_align_ptr(m, NGX_ALIGNMENT);
	new->d.last = m + size;

	current = pool->current;

	/**
	 * 缓存池的pool数据结构会挂载子节点的ngx_pool_t数据结构
	 * 子节点的ngx_pool_t数据结构中只用到pool->d的结构，只保存数据
	 * 每添加一个子节点，p->d.failed就会+1，当添加超过4个子节点的时候，
	 * pool->current会指向到最新的子节点地址
	 *
	 * 这个逻辑主要是为了防止pool上的子节点过多，导致每次ngx_palloc循环pool->d.next链表
	 * 将pool->current设置成最新的子节点之后，每次最大循环4次，不会去遍历整个缓存池链表
	 */
	for (p = current; p->d.next; p = p->d.next) {
		if (p->d.failed++ > 4) {
			current = p->d.next;
		}
	}

	p->d.next = new;

	/* 最终这个还是没变 */
	pool->current = current ? current : new;

	return m;
}
```


分配一块大内存，挂载到pool->large链表上ngx_palloc_large：





```cpp
/**
 * 当分配的内存块大小超出pool->max限制的时候,需要分配在pool->large上
 */
static void *
ngx_palloc_large(ngx_pool_t *pool, size_t size) {
	void *p;
	ngx_uint_t n;
	ngx_pool_large_t *large;

	/* 分配一块新的大内存块 */
	p = ngx_alloc(size, pool->log);
	if (p == NULL) {
		return NULL;
	}

	n = 0;

	/* 去pool->large链表上查询是否有NULL的，只在链表上往下查询3次，主要判断大数据块是否有被释放的，如果没有则只能跳出*/
	for (large = pool->large; large; large = large->next) {
		if (large->alloc == NULL) {
			large->alloc = p;
			return p;
		}

		if (n++ > 3) {
			break;
		}
	}

	/* 分配一个ngx_pool_large_t 数据结构 */
	large = ngx_palloc(pool, sizeof(ngx_pool_large_t));
	if (large == NULL) {
		ngx_free(p); //如果分配失败，删除内存块
		return NULL;
	}

	large->alloc = p;
	large->next = pool->large;
	pool->large = large;

	return p;
}
```



#### 大内存块的释放ngx_pfree

内存池释放需要走ngx_destroy_pool，独立大内存块的单独释放，可以走ngx_pfree方法。



```cpp
/**
 * 大内存块释放  pool->large
 */
ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p) {
	ngx_pool_large_t *l;

	/* 在pool->large链上循环搜索，并且只释放内容区域，不释放ngx_pool_large_t数据结构*/
	for (l = pool->large; l; l = l->next) {
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



#### cleanup机制 可以回调函数清理数据

Nginx的内存池cleanup机制，设计的非常巧妙。pool->cleanup本身是一个链表，每个ngx_pool_cleanup_t的数据结构上，保存着内存数据的本身cleanup->data和回调清理函数cleanup->handler。

通过cleanup的机制，我们就可以在内存池上保存例如文件句柄fd的资源。当我们调用ngx_destroy_pool方法销毁内存池的时候，首先会来清理pool->cleanup，并且都会执行**c->handler(c->data)**回调函数，用于清理资源。

Nginx的这个机制，最显著的就是让文件描述符和需要自定义清理的数据的管理变得更加简单。




分配一个cleanup结构：



```cpp
/**
 * 分配一个可以用于回调函数清理内存块的内存
 * 内存块仍旧在p->d或p->large上
 *
 * ngx_pool_t中的cleanup字段管理着一个特殊的链表，该链表的每一项都记录着一个特殊的需要释放的资源。
 * 对于这个链表中每个节点所包含的资源如何去释放，是自说明的。这也就提供了非常大的灵活性。
 * 意味着，ngx_pool_t不仅仅可以管理内存，通过这个机制，也可以管理任何需要释放的资源，
 * 例如，关闭文件，或者删除文件等等的。下面我们看一下这个链表每个节点的类型
 *
 * 一般分两种情况：
 * 1. 文件描述符
 * 2. 外部自定义回调函数可以来清理内存
 */
ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size) {
	ngx_pool_cleanup_t *c;

	/* 分配一个ngx_pool_cleanup_t */
	c = ngx_palloc(p, sizeof(ngx_pool_cleanup_t));
	if (c == NULL) {
		return NULL;
	}

	/* 如果size !=0 从pool->d或pool->large分配一个内存块 */
	if (size) {
		/*  */
		c->data = ngx_palloc(p, size);
		if (c->data == NULL) {
			return NULL;
		}

	} else {
		c->data = NULL;
	}

	/* handler为回调函数 */
	c->handler = NULL;
	c->next = p->cleanup;

	p->cleanup = c;

	ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

	return c;
}
```


手动清理 p->cleanup链表上的数据：（内存池销毁函数ngx_destroy_pool也会清理p->cleanup）





```cpp
/**
 * 清除 p->cleanup链表上的内存块（主要是文件描述符）
 * 回调函数：ngx_pool_cleanup_file
 */
void ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd) {
	ngx_pool_cleanup_t *c;
	ngx_pool_cleanup_file_t *cf;

	for (c = p->cleanup; c; c = c->next) {
		if (c->handler == ngx_pool_cleanup_file) {

			cf = c->data;

			if (cf->fd == fd) {
				c->handler(cf); /* 调用回调函数 */
				c->handler = NULL;
				return;
			}
		}
	}
}
```


关闭文件的回调函数和删除文件的回调函数。这个是文件句柄通用的回调函数，可以放置在p->cleanup->handler上。





```cpp
/**
 * 关闭文件回调函数
 * ngx_pool_run_cleanup_file方法执行的时候，用了此函数作为回调函数的，都会被清理
 */
void ngx_pool_cleanup_file(void *data) {
	ngx_pool_cleanup_file_t *c = data;

	ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
			c->fd);

	if (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
		ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
				ngx_close_file_n " \"%s\" failed", c->name);
	}
}
```





```cpp
/**
 * 删除文件回调函数
 */
void ngx_pool_delete_file(void *data) {
	ngx_pool_cleanup_file_t *c = data;

	ngx_err_t err;

	ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
			c->fd, c->name);

	if (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
		err = ngx_errno;

		if (err != NGX_ENOENT) {
			ngx_log_error(NGX_LOG_CRIT, c->log, err,
					ngx_delete_file_n " \"%s\" failed", c->name);
		}
	}

	if (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
		ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
				ngx_close_file_n " \"%s\" failed", c->name);
	}
}
```































