# 细说MySQL 之MEM_ROOT - 文章 - 伯乐在线
原文出处： [zxszcaijin](http://blog.chinaunix.net/uid-20708886-id-5581118.html)
这篇文章会详细解说MySQL中使用非常广泛的MEM_ROOT的结构体，同时省去debug部分的信息，仅分析正常情况下，mysql中使用MEM_ROOT来做内存分配的部分。
在具体分析之前我们先例举在该结构体使用过程中用到的一些宏:


```
#define MALLOC_OVERHEAD 8 //分配过程中，需要保留一部分额外的空间
#define ALLOC_MAX_BLOCK_TO_DROP 4096 //后续会继续分析该宏的用途
#define ALLOC_MAX_BLOCK_USAGE_BEFORE_DROP 10 //后续会继续分析该宏的用途
#define ALIGN_SIZE(A) MY_ALIGN((A),sizeof(double))
#define MY_ALIGN(A,L) (((A) + (L) - 1) & ~((L) - 1))
#define ALLOC_ROOT_MIN_BLOCK_SIZE (MALLOC_OVERHEAD + sizeof(USED_MEM) + 8)
/* Define some useful general macros (should be done after all headers). */
#define MY_MAX(a, b) ((a) > (b) ? (a) : (b)) //求两个数值之间的最大值
#define MY_MIN(a, b) ((a) < (b) ? (a) : (b)) //求两个数值之间的最小值
```
下面再来看看MEM_ROOT结构体相关的信息:


```
typedef struct st_mem_root
{
	USED_MEM	*free;                  /* free block link list的链表头指针 */
	USED_MEM	*used;                  /* used block link list的链表头指针 */
	USED_MEM	*pre_alloc;             /* 预先分配的block */
	size_t		min_malloc;             /* 如果block剩下的可用空间小于该值，将会从free list移动到used list */
	size_t		block_size;             /* 每次初始化的空间大小 */
	unsigned int	block_num;              /* 记录实际的block数量,初始化为4 */
	unsigned int	first_block_usage;      /* free list中的第一个block 测试不满足分配空间大小的次数 */
	void (*error_handler)( void );          /* 分配失败的错误处理函数 */
} MEM_ROOT;
```
以下是分配具体的block信息.


```
typedef struct st_used_mem
{ 
    struct st_used_mem *next; //指向下一个分配的block
    unsigned int left; //该block剩余的空间大小
    unsigned int size; //该block的总大小
} USED_MEM;
```
其实MEM_ROOT在分配过程中，是通过双向链表来管理used和free的block:
![20708886_1450839605g7di](http://jbcdn2.b0.upaiyun.com/2016/10/0b9891b2c5dfac37ca7138668efb6191.jpg)
MEM_ROOT的初始化过程如下:


```
void init_alloc_root( MEM_ROOT *mem_root, size_t block_size, size_t pre_alloc_size __attribute__( (unused) ) )
{
	mem_root->free			= mem_root->used = mem_root->pre_alloc = 0;
	mem_root->min_malloc		= 32;
	mem_root->block_size		= block_size - ALLOC_ROOT_MIN_BLOCK_SIZE;
	mem_root->error_handler		= 0;
	mem_root->block_num		= 4; /* We shift this with >>2 */
	mem_root->first_block_usage	= 0;
}
```
初始化过程中，block_size空间为block_size-ALLOC_ROOT_MIN_BLOCK_SIZE。因为在内存不够，需要
扩容时，是通过mem_root->block_num >>2 * block_size 来扩容的，所以mem_root->block_num >>2 至少为1
，因此在初始化的过程中mem_root->block_num=4(注:4>>2=1)。
![20708886_1450840232iki4](http://jbcdn2.b0.upaiyun.com/2016/10/6b8932914b89cc18c49bda0fa66f221b.jpg)
下面来看看具体分配内存的步骤:


```
void *alloc_root( MEM_ROOT *mem_root, size_t length )
{
	size_t		get_size, block_size;
	uchar		* point;
	reg1 USED_MEM	*next = 0;
	reg2 USED_MEM	**prev;
	length = ALIGN_SIZE( length );
	if ( (*(prev = &mem_root->free) ) != NULL )
	{
		if ( (*prev)->left < length &&
		     mem_root->first_block_usage++ >= ALLOC_MAX_BLOCK_USAGE_BEFORE_DROP &&
		     (*prev)->left < ALLOC_MAX_BLOCK_TO_DROP )
		{
			next				= *prev;
			*prev				= next->next; /* Remove block from list */
			next->next			= mem_root->used;
			mem_root->used			= next;
			mem_root->first_block_usage	= 0;
		}
		for ( next = *prev; next && next->left < length; next = next->next )
			prev = &next->next;
	}
	if ( !next )
	{       /* Time to alloc new block */
		block_size	= mem_root->block_size * (mem_root->block_num >> 2);
		get_size	= length + ALIGN_SIZE( sizeof(USED_MEM) );
		get_size	= MY_MAX( get_size, block_size );
		if ( !(next = (USED_MEM *) my_malloc( get_size, MYF( MY_WME | ME_FATALERROR ) ) ) )
		{
			if ( mem_root->error_handler )
				(*mem_root->error_handler)();
			DBUG_RETURN( (void *) 0 );                              /* purecov: inspected */
		}
		mem_root->block_num++;
		next->next	= *prev;
		next->size	= get_size;
		next->left	= get_size - ALIGN_SIZE( sizeof(USED_MEM) );    /* bug:如果该block是通过mem_root->block_size * (mem_root->block_num >> 2)计算出来的，则已经去掉了ALIGN_SIZE(sizeof(USED_MEM)，这里重复了。 */
		*prev		= next;
	}
	point = (uchar *) ( (char *) next + (next->size - next->left) );
/*TODO: next part may be unneded due to mem_root->first_block_usage counter*/
	if ( (next->left -= length) < mem_root->min_malloc )
	{                                                                       /* Full block */
		*prev				= next->next;                   /* Remove block from list */
		next->next			= mem_root->used;
		mem_root->used			= next;
		mem_root->first_block_usage	= 0;
	}
}
```
上述代码的具体逻辑如下:
1.查看free链表，寻找满足空间的block。如果找到了合适的block,则：
   1.1 直接返回该block从size-left处的初始地址即可。当然，在free list遍历的过程中，会去判断free list
         中第一个block中left的空间不满足需要分配的空间，且该block中已经查找过了10次
         (ALLOC_MAX_BLOCK_USAGE_BEFORE_DROP)都不满足分配长度，且该block剩余空间小于
         4k(ALLOC_MAX_BLOCK_TO_DROP),则将该block 移动到used链表中。
2.如果free链表中，没有合适的block，则：
   2.1 分配 mem_root->block_size * (mem_root->block_num >> 2)和length+ALIGN_SIZE(sizeof(USED_MEM))
         中比较大的作为新的block内存空间。
   2.2 根据该block的使用情况，将该block挂在used或者free链表上。
这里需要注意的是二级指针的使用:


```
for (next= *prev ; next && next->left < length ; next= next->next)
prev= &next->next;
}
```
prev指向的是最后一个block的next指向的地址的地址：
![20708886_1450840345dubb](http://jbcdn2.b0.upaiyun.com/2016/10/de5a8e29e603313bc7a56c0d15130f9f.jpg)
所以将prev的地址替换为new block的地址，即将该new block加到了free list的结尾:
*prev=next;
![20708886_1450840487epep](http://jbcdn2.b0.upaiyun.com/2016/10/66cd47a9bcf99de49ac19354ad4de423.jpg)
总结：
MEM_ROOT的内存分配采用的是启发式分配算法，随着后续block的数量越多，单个block的内存也会越大:block_size= mem_root->block_size * (mem_root->block_num >> 2) .
