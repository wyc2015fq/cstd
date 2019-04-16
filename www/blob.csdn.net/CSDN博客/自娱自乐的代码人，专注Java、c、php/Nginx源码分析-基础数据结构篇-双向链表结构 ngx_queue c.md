# Nginx源码分析 - 基础数据结构篇 - 双向链表结构 ngx_queue.c - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年01月31日 17:49:19[initphp](https://me.csdn.net/initphp)阅读数：1874
个人分类：[Nginx源码分析](https://blog.csdn.net/initphp/article/category/6081681)

所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









Nginx的链表结构非常小巧和简单。设计的非常精巧。

通过链表的简单和精巧的设计，让Nginx的链表的数据结构和具体业务依赖进行了解耦。一般我们在设计c语言程序的时候，完全可以学习Nginx的这种数据结构的设计方式。




### 数据结构定义

#### 链表数据结构 ngx_queue_t



```cpp
typedef struct ngx_queue_s ngx_queue_t;

/**
 * 链表的数据结构非常简单，ngx_queue_s会挂载到实体
 * 结构上。然后通过ngx_queue_s来做成链表
 */
struct ngx_queue_s {
	ngx_queue_t *prev;
	ngx_queue_t *next;
};
```
1. prev：前一个元素。



2. next：后一个元素。

大家都看到这个链表的数据结构非常简单，甚至没有和任何的业务数据结构关联起来。但是如何用的呢？

一般情况下，会在业务的数据结构中，放置一个**ngx_queue_t**的数据结构。通过这个数据结构进行双向链表的连接。

例如下面的数据结构：



```cpp
/**
 * 该结构体用于描述一个网络连接
 */
struct ngx_connection_s {
	void *data; //连接未使用时，data用于充当连接池中空闲链表中的next指针。连接使用时由模块而定，HTTP中，data指向ngx_http_request_t
	ngx_event_t *read; //连接对应的读事件
	ngx_event_t *write; //连接对应的写事件

	ngx_socket_t fd; //套接字句柄

	ngx_recv_pt recv; //直接接受网络字节流
	ngx_send_pt send; //直接发送网络字节流
	ngx_recv_chain_pt recv_chain; //网络字节流接收链表
	ngx_send_chain_pt send_chain; //网络字节流发送链表

	/*用来将当前连接以双向链表元素的形式添加到ngx_cycle_t核心结构体
	 * 的reuseable_connection_queue双向链表中，表示可以重用的连接*/
	ngx_queue_t queue;

	/* 省去部分 */
};
```


### 数据结构图

![](https://img-blog.csdn.net/20160217095717954?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


1. 整个链表实现的是双向链表。

2. 遍历链表后，获取链表的主体业务数据结构（上图为ngx_connection_s）使用**ngx_queue_data**方法

3. 链表和业务数据结构之间进行了**解耦**，使用更加灵活和方便。


### 具体函数实现

#### 链表常用操作方法

Nginx的链表用宏定义的方式定义了很多常用的双向链表操作方法。



```cpp
/**
 * 初始化一个Q
 */
#define ngx_queue_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->next = q

/**
 * 判断是否是空Q
 */
#define ngx_queue_empty(h)                                                    \
    (h == (h)->prev)

/**
 * 向链表H后面插入一个x的Q，支持中间插入
 */
#define ngx_queue_insert_head(h, x)                                           \
    (x)->next = (h)->next;                                                    \
    (x)->next->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->next = x

#define ngx_queue_insert_after   ngx_queue_insert_head

/**
 * 向链表H前面插入一个x的Q，支持中间插入
 */
#define ngx_queue_insert_tail(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->next = x;                                                      \
    (x)->next = h;                                                            \
    (h)->prev = x

/**
 * h是尾部，链表的第一个元素
 */
#define ngx_queue_head(h)                                                     \
    (h)->next

// h 是头，h 的上一个就是尾
#define ngx_queue_last(h)                                                     \
    (h)->prev

#define ngx_queue_sentinel(h)                                                 \
    (h)

/**
 * 返回节点Q的下一个元素
 */
#define ngx_queue_next(q)                                                     \
    (q)->next

/**
 * 返回节点Q的上一个元素
 */
#define ngx_queue_prev(q)                                                     \
    (q)->prev

#if (NGX_DEBUG)

/**
 * 移除某一个节点
 */
#define ngx_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next;                                              \
    (x)->prev = NULL;                                                         \
    (x)->next = NULL

#else

#define ngx_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next

#endif

//分割一个链表
#define ngx_queue_split(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->next = n;                                                      \
    (n)->next = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->next = h;                                                      \
    (q)->prev = n;


#define ngx_queue_add(h, n)                                                   \
    (h)->prev->next = (n)->next;                                              \
    (n)->next->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->next = h;
```



#### 获取链表的主体结构 ngx_queue_data

前面我们已经看到，Nginx的**ngx_queue_t**数据结构是挂载在具体的业务数据结构上面的。通过ngx_queue_data，我们可以通过业务结构体中**ngx_queue_t**数据结构的偏移量来得到业务主体的数据结构的指针地址。

**此函数，是Nginx 链表设计的关键点。**




```cpp
/**
 * 通过链表可以找到结构体所在的指针
 * typedef struct {
 * 		ngx_queue_s queue;
 * 		char * x;
 * 		....
 * } TYPE
 * 例如：ngx_queue_data(&type->queue, TYPE, queue)
 */
#define ngx_queue_data(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))
```











