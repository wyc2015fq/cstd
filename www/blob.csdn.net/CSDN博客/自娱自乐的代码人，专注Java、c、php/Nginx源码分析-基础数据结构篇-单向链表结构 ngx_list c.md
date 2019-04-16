# Nginx源码分析 - 基础数据结构篇 - 单向链表结构 ngx_list.c - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年02月05日 10:11:45[initphp](https://me.csdn.net/initphp)阅读数：1673
所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









Nginx的list单向链表的结构和Nginx的数组结构Array有点类似，总体来说，数据结构也是非常简单清晰的。

Nginx的单向链表也是固定了每个元素的大小，并且用单向链表的方式连接。




### 数据结构定义

#### ngx_list_part_t 链表节点定义



```cpp
typedef struct ngx_list_part_s  ngx_list_part_t;

/**
 * 链表节点  每个节点大小 = size * nelts
 * 节点元素用完后，每次就会分配一个新的节点
 */
struct ngx_list_part_s {
    void             *elts;  	/* 节点的内存起始位置 */
    ngx_uint_t        nelts; 	/* 已经使用的元素*/
    ngx_list_part_t  *next;  	/* 指向下一个链表节点*/
};
```



#### ngx_list_t 链表结构



```cpp
/**
 * 链表结构
 */
typedef struct {
    ngx_list_part_t  *last;		/* 指向最新的链表节点*/
    ngx_list_part_t   part;		/* 第一个链表节点*/
    size_t            size;		/* 这个链表默认的每个元素大小 */
    ngx_uint_t        nalloc;	        /* 每个节点part 可以支持多少个元素*/
    ngx_pool_t       *pool;		/* 线程池*/
} ngx_list_t;
```




### 数据结构图

![](https://img-blog.csdn.net/20160217094910558?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 具体函数实现

#### 创建一个list ngx_list_create

通过调用ngx_list_create创建一个list，并且可以指定每个元素的大小以及每个节点元素个数。



```cpp
/**
 * 创建链表
 */
ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    ngx_list_t  *list;

    /* 从内存池上面分配一块内存，存储ngx_list_t数据结构 */
    list = ngx_palloc(pool, sizeof(ngx_list_t));
    if (list == NULL) {
        return NULL;
    }

    /* 分配一个链表节点的内存块。内存大小  n * size*/
    list->part.elts = ngx_palloc(pool, n * size);
    if (list->part.elts == NULL) {
        return NULL;
    }

    list->part.nelts = 0; 		/* 使用的元素个数	*/
    list->part.next = NULL; 	/* 下一个节点		*/
    list->last = &list->part; 	/* 最后一个节点地址 	*/
    list->size = size;  		/* 每个元素的大小	*/
    list->nalloc = n;			/* 分配多少个 		*/
    list->pool = pool;			/* 线程池			*/

    return list;
}
```




#### 使用一个list元素 ngx_list_push

使用ngx_list_push方法可以使用一个元素，并且返回元素的指针地址。如果节点元素已经用完，则会创建一个新的链表



```cpp
void *
ngx_list_push(ngx_list_t *l)
{
    void             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    /* 如果最后一个链表节点的元素已经用完，则需要创建一个新的链表*/
    if (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

    	/* 分配一块内存，存储ngx_list_part_t数据结构 */
        last = ngx_palloc(l->pool, sizeof(ngx_list_part_t));
        if (last == NULL) {
            return NULL;
        }

        /* 分配一个链表节点的内存块。内存大小  n * size*/
        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        if (last->elts == NULL) {
            return NULL;
        }

        last->nelts = 0;
        last->next = NULL;

        l->last->next = last;
        l->last = last;
    }


    /* 返回元素指针 */
    elt = (char *) last->elts + l->size * last->nelts;
    last->nelts++;

    return elt;
}
```













