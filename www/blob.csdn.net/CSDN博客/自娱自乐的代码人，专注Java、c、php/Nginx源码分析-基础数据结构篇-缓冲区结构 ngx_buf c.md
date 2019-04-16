# Nginx源码分析 - 基础数据结构篇 - 缓冲区结构 ngx_buf.c - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年01月30日 13:32:53[initphp](https://me.csdn.net/initphp)阅读数：3854
所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









Nginx的buf缓冲区数据结构，主要用来存储非常大块的内存。ngx_buf_t数据结构也贯穿了整个Nginx。

Nginx的缓冲区设计是比较灵活的。

1. 可以自定义管理业务层面的缓冲区链表；

2. 也可以将空闲的缓冲区链表交还给内存池pool->chain结构。

缓冲区ngx_buf_t是nginx处理大数据的关键数据结构，它既应用于内存数据也应用于磁盘数据。




### 数据结构定义

#### 缓冲区内存块的数据结构 ngx_buf_t



```cpp
typedef struct ngx_buf_s  ngx_buf_t;
/**
 * Nginx缓冲区
 */
struct ngx_buf_s {
    u_char          *pos;           /* 待处理数据的开始标记  */
    u_char          *last;          /* 待处理数据的结尾标记 */
    off_t            file_pos;		/* 处理文件时，待处理的文件开始标记  */
    off_t            file_last;		/* 处理文件时，待处理的文件结尾标记  */

    u_char          *start;         /* 缓冲区开始的指针地址 */
    u_char          *end;           /* 缓冲区结尾的指针地址 */
    ngx_buf_tag_t    tag;			/* 缓冲区标记地址，是一个void类型的指针。 */
    ngx_file_t      *file;			/* 引用的文件 */
    ngx_buf_t       *shadow;


    /* the buf's content could be changed */

    unsigned         temporary:1;	 /* 标志位，为1时，内存可修改 */

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    unsigned         memory:1;   	/* 标志位，为1时，内存只读 */

    /* the buf's content is mmap()ed and must not be changed */
    unsigned         mmap:1;		/* 标志位，为1时，mmap映射过来的内存，不可修改 */

    unsigned         recycled:1;	/* 标志位，为1时，可回收 */
    unsigned         in_file:1;		/* 标志位，为1时，表示处理的是文件 */
    unsigned         flush:1;		/* 标志位，为1时，表示需要进行flush操作 */
    unsigned         sync:1;		/* 标志位，为1时，表示可以进行同步操作，容易引起堵塞 */
    unsigned         last_buf:1;	/* 标志位，为1时，表示为缓冲区链表ngx_chain_t上的最后一块待处理缓冲区 */
    unsigned         last_in_chain:1;/* 标志位，为1时，表示为缓冲区链表ngx_chain_t上的最后一块缓冲区 */

    unsigned         last_shadow:1;	/* 标志位，为1时，表示是否是最后一个影子缓冲区 */
    unsigned         temp_file:1;	/* 标志位，为1时，表示当前缓冲区是否属于临时文件 */

    /* STUB */ int   num;
};
```

1. 从上面这个数据结构中，可以看到ngx_buf_t结构，即可以处理内存，也可以处理文件。

2. Nginx使用了位域的方法，节省存储空间。

3. 每个buf都记录了开始和结束点以及未处理的开始和结束点，因为缓冲区的内存申请了之后，是可以被复用的。

4. 所有缓冲区需要的数据结构以及缓冲区的buf内存块都会被分配到内存池上面。


#### 缓冲区链表结构 ngx_chain_t



```cpp
typedef struct ngx_chain_s       ngx_chain_t;
/**
 * 缓冲区链表结构，放在pool内存池上面
 */
struct ngx_chain_s {
    ngx_buf_t    *buf;
    ngx_chain_t  *next;
};
```


1. 是否还记得内存池结构中，有一个数据结构**pool->chain**就是保存空闲的缓冲区链表的。

2. Nginx的缓冲区ngx_buf_t，通过ngx_chain_t链表结构进行关联和管理。

3. 通过链表的方式实现buf有一个非常大的好处：如果一次需要缓冲区的内存很大，那么并不需要分配一块完整的内存，只需要将缓冲区串起来就可以了。




### 数据结构图

![](https://img-blog.csdn.net/20160217102952810?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


1. Nginx的缓冲区数据结构主要包含链表数据结构ngx_chain_t和buf数据结构ngx_buf_t

2. Nginx可以在自定义的业务层面管理繁忙busy和空闲free的缓冲区链表结构。通过后边的函数，可以对缓冲区的链表结构和buf结构进行管理。

3. 如果缓冲区链表需要被回收，则会放到Nginx内存池的pool->chain链表上。

4. 缓冲区是Nginx用的非常多的一种数据结构，主要用于接收和输出HTTP的数据信息。所以对Nginx的缓冲区的数据结构深入理解非常有必要。





### 具体函数实现

#### 创建一个缓冲区buf ngx_create_temp_buf

ngx_create_temp_buf直接从pool上创建一个缓冲区的buf，buf大小可以自定义。buf的数据结构和buf内存块都会被创建到pool内存池上。



```cpp
/**
 * 创建一个缓冲区。需要传入pool和buf的大小
 */
ngx_buf_t *
ngx_create_temp_buf(ngx_pool_t *pool, size_t size)
{
	ngx_buf_t *b;

	/* 最终调用的是内存池pool，开辟一段内存用作缓冲区，主要放置ngx_buf_t结构体 */
	b = ngx_calloc_buf(pool);
	if (b == NULL) {
		return NULL;
	}

	/* 分配缓冲区内存;  pool为内存池，size为buf的大小*/
	b->start = ngx_palloc(pool, size);
	if (b->start == NULL) {
		return NULL;
	}

	/*
	 * set by ngx_calloc_buf():
	 *
	 *     b->file_pos = 0;
	 *     b->file_last = 0;
	 *     b->file = NULL;
	 *     b->shadow = NULL;
	 *     b->tag = 0;
	 *     and flags
	 */

	b->pos = b->start;  //待处理数据的标记指针
	b->last = b->start; //待处理数据的结尾标记指针
	b->end = b->last + size; //缓冲区结尾地址
	b->temporary = 1;

	return b;
}
```
****


#### **创建一个缓冲区的链表结构 ngx_alloc_chain_link**

单独创建缓冲区ngx_buf_t是没法形成回收和管理机制的。所以需要创建ngx_chain_t缓冲区链表结构，用来管理整个缓冲区。



```cpp
/**
 * 创建一个缓冲区的链表结构
 */
ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
	ngx_chain_t *cl;
	/*
	 * 首先从内存池中去取ngx_chain_t，
	 * 被清空的ngx_chain_t结构都会放在pool->chain 缓冲链上
	 */
	cl = pool->chain;

	if (cl) {
		pool->chain = cl->next;
		return cl;
	}
	/* 如果取不到，则从内存池pool上分配一个数据结构  */
	cl = ngx_palloc(pool, sizeof(ngx_chain_t));
	if (cl == NULL) {
		return NULL;
	}

	return cl;
}
```



#### 批量创建多个缓冲区buf ngx_create_chain_of_bufs

批量创建多个buf，并且用链表串起来。当我们需要的缓冲区非常大的时候，可以通过此方法，分配一个缓冲区链表，用于缓冲区的数据管理。



```cpp
/**
 * 批量创建多个buf，并且用链表串起来
 */
ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
	u_char *p;
	ngx_int_t i;
	ngx_buf_t *b;
	ngx_chain_t *chain, *cl, **ll;

	/* 在内存池pool上分配bufs->num个 buf缓冲区 ，每个大小为bufs->size */
	p = ngx_palloc(pool, bufs->num * bufs->size);
	if (p == NULL) {
		return NULL;
	}

	ll = &chain;

	/* 循环创建BUF，并且将ngx_buf_t挂载到ngx_chain_t链表上，并且返回链表*/
	for (i = 0; i < bufs->num; i++) {

		/* 最终调用的是内存池pool，开辟一段内存用作缓冲区，主要放置ngx_buf_t结构体 */
		b = ngx_calloc_buf(pool);
		if (b == NULL) {
			return NULL;
		}

		/*
		 * set by ngx_calloc_buf():
		 *
		 *     b->file_pos = 0;
		 *     b->file_last = 0;
		 *     b->file = NULL;
		 *     b->shadow = NULL;
		 *     b->tag = 0;
		 *     and flags
		 *
		 */

		b->pos = p;
		b->last = p;
		b->temporary = 1;

		b->start = p;
		p += bufs->size; //p往前增
		b->end = p;

		/* 分配一个ngx_chain_t */
		cl = ngx_alloc_chain_link(pool);
		if (cl == NULL) {
			return NULL;
		}

		/* 将buf，都挂载到ngx_chain_t链表上，最终返回ngx_chain_t链表 */
		cl->buf = b;
		*ll = cl;
		ll = &cl->next;
	}

	*ll = NULL;

	/* 最终得到一个分配了bufs->num的缓冲区链表  */
	return chain;
}
```

#### 拷贝缓冲区链表 ngx_chain_add_copy

将其它缓冲区链表放到已有缓冲区链表结构的尾部。



```cpp
/**
 * 将其它缓冲区链表放到已有缓冲区链表结构的尾部
 */
ngx_int_t ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
		ngx_chain_t *in)
{
	ngx_chain_t *cl, **ll;

	ll = chain; //chain 指向指针的指针，很绕

	/* 找到缓冲区链表结尾部分，cl->next== NULL；cl = *chain既为指针链表地址*/
	for (cl = *chain; cl; cl = cl->next) {
		ll = &cl->next;
	}

	/* 遍历in */
	while (in) {
		cl = ngx_alloc_chain_link(pool);
		if (cl == NULL) {
			return NGX_ERROR;
		}

		cl->buf = in->buf; //in上的buf拷贝到cl上面
		*ll = cl; //并且放到chain链表上
		ll = &cl->next; //链表往下走
		in = in->next; //遍历，直到NULL
	}

	*ll = NULL;

	return NGX_OK;
}
```



#### 获取一个空闲的buf链表结构 ngx_chain_get_free_buf



```cpp
/**
 * 冲空闲的buf链表上，获取一个未使用的buf链表
 */
ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **free)
{
	ngx_chain_t *cl;

	/* 空闲列表中有数据，则直接返回 */
	if (*free) {
		cl = *free;
		*free = cl->next;
		cl->next = NULL;
		return cl;
	}

	/* 否则分配一个新的buf */
	cl = ngx_alloc_chain_link(p);
	if (cl == NULL) {
		return NULL;
	}

	cl->buf = ngx_calloc_buf(p);
	if (cl->buf == NULL) {
		return NULL;
	}

	cl->next = NULL;

	return cl;
}
```


释放缓冲区链表 ngx_free_chain和ngx_chain_update_chains 



ngx_free_chain：直接交还给Nginx内存池的pool->chain空闲buf链表

ngx_chain_update_chains：可以交还给自定义的空闲链表上。



```cpp
//直接交还给缓存池
#define ngx_free_chain(pool, cl)                                             \
    cl->next = pool->chain;                                                  \
    pool->chain = cl


/**
 * 释放BUF
 * 1. 如果buf不为空，则不释放
 * 2. 如果cl->buf->tag标记不一样，则直接还给Nginx的pool->chain链表
 * 3. 如果buf为空，并且需要释放，则直接释放buf，并且放到free的空闲列表上
 */
void ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **free,
		ngx_chain_t **busy, ngx_chain_t **out, ngx_buf_tag_t tag)
{
	ngx_chain_t *cl;

	/* *busy 指向OUT，将已经输出的out放到busy链表上 */
	if (*busy == NULL) {
		*busy = *out;

	} else {
		for (cl = *busy; cl->next; cl = cl->next) { /* void */
		}

		cl->next = *out;
	}

	*out = NULL;

	/* 遍历 busy链表 */
	while (*busy) {
		cl = *busy;

		/* 如果buf不为空，则继续遍历 */
		if (ngx_buf_size(cl->buf) != 0) {
			break;
		}

		/* 如果标识一样，则释放这个BUF */
		if (cl->buf->tag != tag) {
			*busy = cl->next;
			ngx_free_chain(p, cl); //还给Nginx pool->chain
			continue;
		}

		/* 直接将buf使用的部分回归到 起点指针地址 */
		cl->buf->pos = cl->buf->start;
		cl->buf->last = cl->buf->start;

		*busy = cl->next; //继续往后遍历

		/* 并且将cl放到free列表上 */
		cl->next = *free;
		*free = cl;
	}
}
```


















