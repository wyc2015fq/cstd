# Memcached源码分析 - Memcached源码分析之HashTable（4） - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年03月22日 14:37:08[initphp](https://me.csdn.net/initphp)阅读数：3112
所属专栏：[Memcache 源码阅读](https://blog.csdn.net/column/details/memcache-read.html)









文章列表：

[](http://blog.csdn.net/initphp/article/details/43915683)

[《Memcached源码分析 - Memcached源码分析之基于Libevent的网络模型（1）》](http://blog.csdn.net/initphp/article/details/43915683)

[《Memcached源码分析 - Memcached源码分析之命令解析（2）》](http://blog.csdn.net/initphp/article/details/44208733)

[《Memcached源码分析 - Memcached源码分析之消息回应（3）  》](http://blog.csdn.net/initphp/article/details/44423505)

[《Memcached源码分析 - Memcached源码分析之HashTable（4） 》](http://blog.csdn.net/initphp/article/details/44537547)

[《Memcached源码分析 - Memcached源码分析之增删改查操作（5） 》](http://blog.csdn.net/initphp/article/details/44626403)

[《Memcached源码分析 - Memcached源码分析之LRU算法（6）》](http://blog.csdn.net/initphp/article/details/44680115)

[《Memcached源码分析 - Memcached源码分析之存储机制Slabs（7）》](http://blog.csdn.net/initphp/article/details/44888555)
[《Memcached源码分析 - Memcached源码分析之总结篇（8）》](http://blog.csdn.net/initphp/article/details/44893869)
[](http://blog.csdn.net/initphp/article/details/44893869)


### 前言

上一章我们讲解了Memcached的消息回应机制[《Memcached源码分析 - Memcached源码分析之消息回应（3）》](http://blog.csdn.net/initphp/article/details/44423505)。从这一章开始我们慢慢讲解Memcached是如何存储数据的。

讲解本章前，我们先看一个Memcached存储数据的item的基本结构。



```cpp
//item的具体结构
typedef struct _stritem {
    //记录下一个item的地址,主要用于LRU链和freelist链
    struct _stritem *next;
    //记录上一个item的地址,主要用于LRU链和freelist链
    struct _stritem *prev;
    //记录HashTable的下一个Item的地址
    struct _stritem *h_next;
    //最近访问的时间，只有set/add/replace等操作才会更新这个字段
    //当执行flush命令的时候，需要用这个时间和执行flush命令的时间相比较，来判断是否失效
    rel_time_t      time;       /* least recent access */
    //缓存的过期时间。设置为0的时候，则永久有效。
    //如果Memcached不能分配新的item的时候，设置为0的item也有可能被LRU淘汰
    rel_time_t      exptime;    /* expire time */
    //value数据大小
    int             nbytes;     /* size of data */
    //引用的次数。通过这个引用的次数，可以判断item是否被其它的线程在操作中。
    //也可以通过refcount来判断当前的item是否可以被删除，只有refcount -1 = 0的时候才能被删除
    unsigned short  refcount;
    uint8_t         nsuffix;    /* length of flags-and-length string */
    uint8_t         it_flags;   /* ITEM_* above */
    //slabs_class的ID。
    uint8_t         slabs_clsid;/* which slab class we're in */
    uint8_t         nkey;       /* key length, w/terminating null and padding */
    /* this odd type prevents type-punning issues when we do
     * the little shuffle to save space when not using CAS. */
    //数据存储结构
    union {
        uint64_t cas;
        char end;
    } data[];
    /* if it_flags & ITEM_CAS we have 8 bytes CAS */
    /* then null-terminated key */
    /* then " flags length\r\n" (no terminating null) */
    /* then data with terminating \r\n (no terminating null; it's binary!) */
} item;
```

说明：

1. Memcached上存储的每一个元素都会有一个Item的结构。

2. Item结构主要记录与HashTable之间的关系，以及存储数据的slabs_class（后面几章节会讲解）的关系以及key的信息，存储的数据和长度等基本信息。

3. Item块会被分配在slabclass上，slabclass会在下下章节中详细讲解。

4. HashTable的主要作用是：**用于通过key快速查询缓存数据**。





### Memcached的HashTable源码分析

#### HashTable结构图

![](https://img-blog.csdn.net/20150402203518795?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


说明：

1. Memcached在启动的时候，会默认初始化一个HashTable，这个table的默认长度为65536。

2. 我们将这个HashTable中的每一个元素称为**桶**，每个桶就是一个item结构的单向链表。

3. Memcached会将key值hash成一个变量名称为hv的**uint32_t**类型的值。

4. 通过hv与桶的个数之间的按位与计算，**hv & hashmask(hashpower)**，就可以得到当前的key会落在哪个桶上面。

5. 然后会将item挂到这个桶的链表上面。链表主要是通过item结构中的**h_next**实现。





#### item_get - 继续从get方法说起

结合第二章，我们继续看Memcached get的操作方法。这个方法中主要作用：

1. 将key 哈希成一个**uint32_t**类型的值。

2. 调用do_item_get方法。




```cpp
//这边的item_*系列的方法，就是Memcached核心存储块的接口
item *item_get(const char *key, const size_t nkey) {
    item *it;
    uint32_t hv;
    hv = hash(key, nkey); //对key进行hash,返回一个uint32_t类型的值
    item_lock(hv); //块锁，当取数据的时候，不允许其他的操作，保证取数据的原子性
    it = do_item_get(key, nkey, hv);
    item_unlock(hv);
    return it;
}
```




继续看do_item_get这个方法，我们可以看到assoc_find这个方法就是查询HashTable上的Item。



```cpp
/** wrapper around assoc_find which does the lazy expiration logic */
item *do_item_get(const char *key, const size_t nkey, const uint32_t hv) {
    //mutex_lock(&cache_lock);
    item *it = assoc_find(key, nkey, hv); //hashTable上存储Item
    //....more code
}
```




#### assoc.c - assoc_init 初始化HashTable

当我们进入assoc_find这个方法后，发现HashTable的操作都在assoc.c这个文件中。

1. assoc_init这个方法为HashTable初始化，assoc_init这个方法是在入口函数main中初始化的。

2. HashTable默认设置为16，1 << 16后得到**65536**个桶。如果用户自定义设置，设置值在**12-64**之间。

3. 一般情况下，memcached的桶的个数足够使用了。




```cpp
//初始化HahsTable表
void assoc_init(const int hashtable_init) {
	//初始化的时候 hashtable_init值需要大于12 小于64
	//如果hashtable_init的值没有设定，则hashpower使用默认值为16
    if (hashtable_init) {
        hashpower = hashtable_init;
    }
    //primary_hashtable主要用来存储这个HashTable
    //hashsize方法是求桶的个数，默认如果hashpower=16的话，桶的个数为：65536
    primary_hashtable = calloc(hashsize(hashpower), sizeof(void *));
    if (! primary_hashtable) {
        fprintf(stderr, "Failed to init hashtable.\n");
        exit(EXIT_FAILURE);
    }
    STATS_LOCK();
    stats.hash_power_level = hashpower;
    stats.hash_bytes = hashsize(hashpower) * sizeof(void *);
    STATS_UNLOCK();
}
```






#### assoc.c - assoc_find 查找一个Item

1. 首先通过key的hash值hv找到对应的桶。

2. 然后遍历桶的单链表，比较key值，找到对应item




```cpp
//寻找一个Item
item *assoc_find(const char *key, const size_t nkey, const uint32_t hv) {
    item *it;
    unsigned int oldbucket;

    //判断是否在扩容中...
    if (expanding &&
        (oldbucket = (hv & hashmask(hashpower - 1))) >= expand_bucket)
    {
        it = old_hashtable[oldbucket];
    } else {
    	//获取得到具体的桶的地址
        it = primary_hashtable[hv & hashmask(hashpower)];
    }

    item *ret = NULL;
    int depth = 0; //循环的深度
    while (it) {
    	//循环查找桶的list中的Item
        if ((nkey == it->nkey) && (memcmp(key, ITEM_key(it), nkey) == 0)) {
            ret = it;
            break;
        }
        it = it->h_next;
        ++depth;
    }
    MEMCACHED_ASSOC_FIND(key, nkey, depth);
    return ret;
}
```




#### assoc.c - assoc_insert 新增一个Item

1. 首先通过key的hash值hv找到对应的桶。

2. 然后将item放到对应桶的单链表的头部

3. 判断是否需要扩容，如果扩容，则会在单独的线程中进行。（桶的个数(默认：65536) * 3） / 2




```cpp
//新增Item操作
int assoc_insert(item *it, const uint32_t hv) {
    unsigned int oldbucket;

    assert(assoc_find(ITEM_key(it), it->nkey) == 0);  /* shouldn't have duplicately named things defined */

    //判断是否在扩容，如果是扩容中，为保证程序继续可用，则需要使用旧的桶
    if (expanding &&
        (oldbucket = (hv & hashmask(hashpower - 1))) >= expand_bucket)
    {
        it->h_next = old_hashtable[oldbucket];
        old_hashtable[oldbucket] = it;
    } else {
    	//hv & hashmask(hashpower) 按位与计算是在哪个桶上面
    	//将当前的item->h_next 指向桶中首个Item的位置
        it->h_next = primary_hashtable[hv & hashmask(hashpower)];
        //然后将hashtable中的首页Item指向新的Item地址值
        primary_hashtable[hv & hashmask(hashpower)] = it;
    }

    hash_items++; //因为是新增操作，则就会增加一个Item
    //如果hash_items的个数大于当前  （桶的个数(默认：65536) * 3） / 2的时候，就需要重新扩容
    //因为初始化的桶本身就比较多了，所以扩容必须在单独的线程中处理，每次扩容估计耗时比较长
    if (! expanding && hash_items > (hashsize(hashpower) * 3) / 2) {
        assoc_start_expand();
    }

    MEMCACHED_ASSOC_INSERT(ITEM_key(it), it->nkey, hash_items);
    return 1;
}
```




#### assoc.c - assoc_delete 删除item操作



1. 首先通过key的hash值hv找到对应的桶。

2. 找到桶对应的链表，遍历单链表，删除对应的Item。




```cpp
//该方法主要用于寻找
static item** _hashitem_before (const char *key, const size_t nkey, const uint32_t hv) {
    item **pos;
    unsigned int oldbucket;

    //判断是否在扩容中
    if (expanding &&
        (oldbucket = (hv & hashmask(hashpower - 1))) >= expand_bucket)
    {
        pos = &old_hashtable[oldbucket];
    } else {
        //返回具体桶的地址
        pos = &primary_hashtable[hv & hashmask(hashpower)];
    }

    //在桶的list中匹配key值是否相同，相同则找到Item
    while (*pos && ((nkey != (*pos)->nkey) || memcmp(key, ITEM_key(*pos), nkey))) {
        pos = &(*pos)->h_next;
    }
    return pos;
}
//删除一个桶上的Item
void assoc_delete(const char *key, const size_t nkey, const uint32_t hv) {
    item **before = _hashitem_before(key, nkey, hv); //查询Item是否存在

    //如果Item存在，则当前的Item值指向下一个Item的指针地址
    if (*before) {
        item *nxt;
        hash_items--; //item个数减去1
        /* The DTrace probe cannot be triggered as the last instruction
         * due to possible tail-optimization by the compiler
         */
        MEMCACHED_ASSOC_DELETE(key, nkey, hash_items);
        nxt = (*before)->h_next;
        (*before)->h_next = 0;   /* probably pointless, but whatever. */
        *before = nxt;
        return;
    }
    /* Note:  we never actually get here.  the callers don't delete things
       they can't find. */
    assert(*before != 0);
}
```




#### assoc.c - 关于扩容

Memcached的扩容都是在单独线程中进行的。

（桶的个数(默认：65536) * 3） / 2的时候，就需要重新扩容。扩容需要的时间比较久，所以必须使用不同的线程进行自动扩容处理。




```cpp
static void assoc_start_expand(void) {
    if (started_expanding)
        return;
    started_expanding = true;
    //唤醒线程
    pthread_cond_signal(&maintenance_cond);
}
/* grows the hashtable to the next power of 2. */
//扩容方法
static void assoc_expand(void) {
    old_hashtable = primary_hashtable;

    primary_hashtable = calloc(hashsize(hashpower + 1), sizeof(void *));
    if (primary_hashtable) {
        if (settings.verbose > 1)
            fprintf(stderr, "Hash table expansion starting\n");
        hashpower++;
        expanding = true;
        expand_bucket = 0;
        STATS_LOCK();
        stats.hash_power_level = hashpower;
        stats.hash_bytes += hashsize(hashpower) * sizeof(void *);
        stats.hash_is_expanding = 1;
        STATS_UNLOCK();
    } else {
        primary_hashtable = old_hashtable;
        /* Bad news, but we can keep running. */
    }
}
static volatile int do_run_maintenance_thread = 1;

#define DEFAULT_HASH_BULK_MOVE 1
int hash_bulk_move = DEFAULT_HASH_BULK_MOVE;

static void *assoc_maintenance_thread(void *arg) {

    while (do_run_maintenance_thread) {
        int ii = 0;

        /* Lock the cache, and bulk move multiple buckets to the new
         * hash table. */
        item_lock_global();
        mutex_lock(&cache_lock);

        for (ii = 0; ii < hash_bulk_move && expanding; ++ii) {
            item *it, *next;
            int bucket;

            for (it = old_hashtable[expand_bucket]; NULL != it; it = next) {
                next = it->h_next;

                bucket = hash(ITEM_key(it), it->nkey) & hashmask(hashpower);
                it->h_next = primary_hashtable[bucket];
                primary_hashtable[bucket] = it;
            }

            old_hashtable[expand_bucket] = NULL;

            expand_bucket++;
            if (expand_bucket == hashsize(hashpower - 1)) {
                expanding = false;
                free(old_hashtable);
                STATS_LOCK();
                stats.hash_bytes -= hashsize(hashpower - 1) * sizeof(void *);
                stats.hash_is_expanding = 0;
                STATS_UNLOCK();
                if (settings.verbose > 1)
                    fprintf(stderr, "Hash table expansion done\n");
            }
        }

        mutex_unlock(&cache_lock);
        item_unlock_global();

        if (!expanding) {
            /* finished expanding. tell all threads to use fine-grained locks */
            switch_item_lock_type(ITEM_LOCK_GRANULAR);
            slabs_rebalancer_resume();
            /* We are done expanding.. just wait for next invocation */
            mutex_lock(&cache_lock);
            started_expanding = false;
            pthread_cond_wait(&maintenance_cond, &cache_lock);
            /* Before doing anything, tell threads to use a global lock */
            mutex_unlock(&cache_lock);
            slabs_rebalancer_pause();
            switch_item_lock_type(ITEM_LOCK_GLOBAL);
            mutex_lock(&cache_lock);
            assoc_expand();
            mutex_unlock(&cache_lock);
        }
    }
    return NULL;
}

static pthread_t maintenance_tid;

int start_assoc_maintenance_thread() {
    int ret;
    char *env = getenv("MEMCACHED_HASH_BULK_MOVE");
    if (env != NULL) {
        hash_bulk_move = atoi(env);
        if (hash_bulk_move == 0) {
            hash_bulk_move = DEFAULT_HASH_BULK_MOVE;
        }
    }
    if ((ret = pthread_create(&maintenance_tid, NULL,
                              assoc_maintenance_thread, NULL)) != 0) {
        fprintf(stderr, "Can't create thread: %s\n", strerror(ret));
        return -1;
    }
    return 0;
}

void stop_assoc_maintenance_thread() {
    mutex_lock(&cache_lock);
    do_run_maintenance_thread = 0;
    pthread_cond_signal(&maintenance_cond);
    mutex_unlock(&cache_lock);

    /* Wait for the maintenance thread to stop */
    pthread_join(maintenance_tid, NULL);
}
```




















