# 【redis】内存回收机制 - Big Smile - CSDN博客
置顶2018年07月29日 10:10:35[王啸tr1912](https://me.csdn.net/tr1912)阅读数：986
在redis4.0的配置文件中有这么一段被注释了的解释：
![](https://img-blog.csdn.net/20180729092259245?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这个里面讲的是redis的内存回收配置，其中有这么几种：
volatile-lru -> 根据LRU算法删除设置了超时属性（expire）的键，直到腾出足够空间为止。如果没有可删除的键对象，回退到noeviction策略。
allkeys-lru -> 根据LRU算法删除键，不管数据有没有设置超时属性，直到腾出足够空间为止。
volatile-lfu -> 根据LFU算法删除设置了超时属性（expire）的键，直到腾出足够空间为止。如果没有可删除的键对象，回退到noeviction策略。
allkeys-lfu -> 根据LFU算法删除键，不管数据有没有设置超时属性，直到腾出足够空间为止。
volatile-random -> 随机删除过期键，直到腾出足够空间为止。
allkeys-random -> 随机删除所有键，直到腾出足够空间为止。
volatile-ttl -> 根据键值对象的ttl属性，删除最近将要过期数据。如果没有，回退到noeviction策略。
noeviction -> 不会删除任何数据，拒绝所有写入操作并返 回客户端错误信息，此 时Redis只响应读操作。
redis默认的策略就是noeviction策略，如果想要配置的话，需要在配置文件中写这个配置：
`maxmemory-policy volatile-lru`
# 一、什么是LRU
        LRU是Least Recently Used 近期最少使用算法，很多缓存策略都使用了这种策略进行空间的释放，在学习操作系统的内存回收的时候也用到了这种机制进行内存的回收，类似的还有LFU（Least Frequently Used）最不经常使用算法，这种算法。
        我们在上面的描述中也可以了解到，redis使用的是一种类似LRU的算法进行内存溢出回收的，其算法的代码：
```
/* volatile-lru and allkeys-lru policy */
else if (server.maxmemory_policy == REDIS_MAXMEMORY_ALLKEYS_LRU ||
	server.maxmemory_policy == REDIS_MAXMEMORY_VOLATILE_LRU)
{
	struct evictionPoolEntry *pool = db->eviction_pool;
	while(bestkey == NULL) {
		evictionPoolPopulate(dict, db->dict, db->eviction_pool);
		/* Go backward from best to worst element to evict. */
		for (k = REDIS_EVICTION_POOL_SIZE-1; k >= 0; k--) {
			if (pool[k].key == NULL) continue;
			de = dictFind(dict,pool[k].key);
			/* Remove the entry from the pool. */
			sdsfree(pool[k].key);
			/* Shift all elements on its right to left. */
			memmove(pool+k,pool+k+1,
				sizeof(pool[0])*(REDIS_EVICTION_POOL_SIZE-k-1));
			/* Clear the element on the right which is empty
			 * since we shifted one position to the left.  */
			pool[REDIS_EVICTION_POOL_SIZE-1].key = NULL;
			pool[REDIS_EVICTION_POOL_SIZE-1].idle = 0;
			/* If the key exists, is our pick. Otherwise it is
			 * a ghost and we need to try the next element. */
			if (de) {
				bestkey = dictGetKey(de);
				break;
			} else {
				/* Ghost... */
				continue;
			}
		}
	}
}
```
Redis会基于server.maxmemory_samples配置选取固定数目的key，然后比较它们的lru访问时间，然后淘汰最近最久没有访问的key，maxmemory_samples的值越大，Redis的近似LRU算法就越接近于严格LRU算法，但是相应消耗也变高。所以，频繁的进行这种内存回收是会降低redis性能的，主要是查找回收节点和删除需要回收节点的开销。
       所以一般我们在配置redis的时候，尽量不要让它进行这种内存溢出的回收操作，redis是可以配置maxmemory，used_memory指的是redis真实占用的内存，但是由于操作系统还有其他软件以及内存碎片还有swap区的存在，所以我们实际的内存应该比redis里面设置的maxmemory要大，具体大多少视系统环境和软件环境来定。maxmemory也要比used_memory大，一般由于碎片的存在需要做1~2个G的富裕。
# 二、内存回收策略
        内存回收触发有两种情况，上面说的是一种，也就是**内存使用达到maxmemory上限时候触发的溢出回收**，还有一种是我们设置了过期的对象到期的时候触发的到期释放的内存回收。
**删除过期对象**
        Redis所有的键都可以设置过期属性，内部保存在过期字典中。由于进程内保存大量的键，维护每个键精准的过期删除机制会导致消耗大量的 CPU，对于单线程的Redis来说成本过高，因此Redis采用惰性删除和定时任务删除机制实现过期键的内存回收。
      1、惰性删除：顾名思义，指的是不主动删除，当用户访问已经过期的对象的时候才删除，最大的优点是节省cpu的开销，不用另外的内存和TTL链表来维护删除信息，缺点就是如果数据到期了但是一直没有被访问的话就不会被删除，会占用内存空间。
      2、定时任务删除：为了弥补第一种方式的缺点，redis内部还维护了一个定时任务，默认每秒运行10次。定时任务中删除过期逻辑采用了自适应算法，使用快、慢两种速率模式回收键。
![](https://img-blog.csdn.net/2018072910094364?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
流程说明：
- 定时任务在每个数据库空间随机检查20个键，当发现过期时删除对应的键。
- 如果超过检查数25%的键过期，循环执行回收逻辑直到不足25%或 运行超时为止，慢模式下超时时间为25毫秒。
- 如果之前回收键逻辑超时，则在Redis触发内部事件之前再次以快模 式运行回收过期键任务，快模式下超时时间为1毫秒且2秒内只能运行1次。
- 快慢两种模式内部删除逻辑相同，只是执行的超时时间不同。
