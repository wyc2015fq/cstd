# Redis学习笔记2--Redis数据类型及相关命令 - gauss的专栏 - CSDN博客
2014年10月17日 20:12:58[gauss](https://me.csdn.net/mathlmx)阅读数：254
个人分类：[分布式与缓存设计](https://blog.csdn.net/mathlmx/article/category/3168687)

Redis支持的种数据类型包括string、list 、set 、sorted set 和hash。
Redis相关的命令可以查看：[http://redis.io/commands](http://redis.io/commands) 这是官方的命令使用手册，也有中文翻译的：[http://redis.readthedocs.org/en/2.4/index.html](http://redis.readthedocs.org/en/2.4/index.html)
**1. keys: **
redis本质上一个key-value store，所以首先了解它的key。首先key也是字符串类型，但是key中不能包括边界字符。由于key不是binary safe的字符串，所以像"my key"和"mykey\n"这样包含空格和换行的key是不允许的。在redis内部并不限制使用binary字符，这是redis协议限制的。"\r\n"在协议格式中会作为特殊字符。redis 1.2以后的协议中部分命令已经开始使用新的协议格式了(比如MSET)。总之目前还是把包含边界字符当成非法的key吧，免得被bug纠缠。另外关于key的一个格式约定介绍下，object-type:id:field。比如user:1000:password，blog:xxidxx:title。还有key的长度最好不要太长。道理很明显占内存啊，而且查找时候相对短key也更慢。不过也推荐过短的key，比如u:1000:pwd，这样的。显然没上面的user:1000:password可读性好。
       下面介绍下key相关的命令：
|exits key|测试指定key是否存在，返回1表示存在，0不存在|
|----|----|
|del key1 key2 ....keyN|删除给定key，返回删除key的数目，0表示给定key都不存在|
|type key|返回给定key的value类型。返回 none 表示不存在key，string字符类型，list 链表类型 set 无序集合类型|
|keys pattern|返回匹配指定模式的所有key|
|randomkey|返回从当前数据库中随机选择的一个key，如果当前数据库是空的，返回空串|
|rename oldkey newkey|原子的重命名一个key，如果newkey存在，将会被覆盖，返回1表示成功，0失败。可能是oldkey不存在或者和newkey相同|
|renamenx oldkey newkey|同上，但是如果newkey存在返回失败|
|dbsize|返回当前数据库的key数量|
|expire key seconds|为key指定过期时间，单位是秒。返回1成功。|
|ttl key|返回设置过过期时间的key的剩余过期秒数 -1表示key不存在或者没有设置过过期时间|
|select db-index|将key从当前数据库移动到指定数据库。返回1成功。0 如果key不存在，或者已经在指定数据库中|
|move key db-index|将key从当前数据库移动到指定数据库。返回1成功。0 如果key不存在，或者已经在指定数据库中|
|flushdb|删除当前数据库中所有key，此方法不会失败。慎用|
|flushall|删除所有数据库中的所有key，此方法不会失败。慎用|
key相关命令简单示例：
redis 127.0.0.1:6379> set test dsf
OK
redis 127.0.0.1:6379> set tast dsaf
OK
redis 127.0.0.1:6379> set tist adff
OK
redis 127.0.0.1:6379> keys t*
1. "tist"
2. "tast"
3. "test"
redis 127.0.0.1:6379> keys t[ia]st
1. "tist"
2. "tast"
redis 127.0.0.1:6379> keys t?st
1. "tist"
2. "tast"
3. "test"
**2. string:**
string是redis最基本的类型，而且string类型是二进制安全的。意思是redis的string可以包含任何数据。比如jpg图片或者序列化的对象。从内部实现来看其实string可以看作byte数组，最大上限是1G字节。
下面是string类型的定义。
struct sdshdr {
long len; 
long free; 
char buf[];
}
buf是个char数组用于存贮实际的字符串内容。char是一个字节。len是buf数组的长度，free是数组中剩余可用字节数。由此可以理解为什么string类型是二进制安全的了。因为它本质上就是个byte数组。当然可以包含任何数据了。另外string类型可以被部分命令按int处理。比如incr等命令，下面详细介绍。还有redis的其他类型像list，set，sorted set ，hash它们包含的元素与都只能是string类型。如果只用string类型，redis就可以被看作加上持久化特性的memcached。当然redis对string类型的操作比memcached多很多啊。如下：
|set key value|设置key对应的值为string类型的value,返回1表示成功，0失败|
|----|----|
|setnx key value|同上，如果key已经存在，返回0 。nx 是not exist的意思|
|get key|获取key对应的string值,如果key不存在返回nil|
|getset key value|原子的设置key的值，并返回key的旧值。如果key不存在返回nil|
|mget key1 key2 ... keyN|一次获取多个key的值，如果对应key不存在，则对应返回nil。下面是个实验,首先清空当前数据库，然后设置k1,k2.获取时k3对应返回nil|
|mset key1 value1 ... keyN valueN|一次设置多个key的值，成功返回1表示所有的值都设置了，失败返回0表示没有任何值被设置|
|msetnx key1 value1 ... keyN valueN|同上，但是不会覆盖已经存在的key|
|incr key|对key的值做加加操作,并返回新的值。注意incr一个不是int的value会返回错误，incr一个不存在的key，则设置key为1|
|decr key|同上，但是做的是减减操作，decr一个不存在key，则设置key为-1|
|incrby key integer|同incr，加指定值 ，key不存在时候会设置key，并认为原来的value是 0|
|decrby key integer|同decr，减指定值。decrby完全是为了可读性，我们完全可以通过incrby一个负值来实现同样效果，反之一样。|
|append key value|给指定key的字符串值追加value,返回新字符串值的长度。|
|substr key start end|返回截取过的key的字符串值,注意并不修改key的值。下标是从0开始的|
string相关命令简单示例：
redis 127.0.0.1:6379> set key hello
OK
redis 127.0.0.1:6379> append k ,world
(integer) 11
redis 127.0.0.1:6379> get k
"hello,world"
redis 127.0.0.1:6379> substr k 0 8
"hello,wor"
redis 127.0.0.1:6379> get k
"hello,world"
redis 127.0.0.1:6379> set k1 a
OK
redis 127.0.0.1:6379> set k2 b
OK
redis 127.0.0.1:6379> mget k1 k2 k3
1. "a"
2. "b"
3. (nil)
**3. list:**
redis的list类型是一个每个子元素都是string类型的双向链表。所以[lr]push和[lr]pop命令的算法时间复杂度都是O(1)另外list会记录链表的长度。所以llen操作也是O(1).链表的最大长度是(2的32次方-1)。我们可以通过push,pop操作从链表的头部或者尾部添加删除元素。
这使得list既可以用作栈，也可以用作队列。有意思的是list的pop操作还有阻塞版本的。当我们[lr]pop一个list对象是，如果list是空，或者不存在，会立即返回nil。但是阻塞版本的b[lr]pop可以则可以阻塞，当然可以加超时时间，超时后也会返回nil。
为什么要阻塞版本的pop呢，主要是为了避免轮询。举个简单的例子如果我们用list来实现一个工作队列。执行任务的thread可以调用阻塞版本的pop去获取任务这样就可以避免轮询去检查是否有任务存在。当任务来时候工作线程可以立即返回，也可以避免轮询带来的延迟。
下面介绍list相关命令：
|lpush key string|在key对应list的头部添加字符串元素，返回值为添加新元素后list的元素个数，0表示key存在且不是list类型|
|----|----|
|rpush key string|同上，在尾部添加|
|llen key|返回key对应list的长度，key不存在返回0,如果key对应类型不是list返回错误|
|lrange key start end|返回指定区间内的元素，下标从0开始，负值表示从后面计算，-1表示倒数第一个元素 ，key不存在返回空列表|
|ltrim key start end|截取list，保留指定区间内元素，成功返回1，key不存在返回错误|
|lset key index value|设置list中指定下标的元素值，成功返回1，key或者下标不存在返回错误|
|lrem key count value|从key对应list中删除count个和value相同的元素。count为0时候删除全部|
|lpop key|从list的头部删除元素，并返回删除元素。如果key对应list不存在或者是空返回nil，如果key对应值不是list返回错误|
|rpop key|同上，但是从尾部删除|
|blpop key1...keyN timeout|从左到右扫描返回对第一个非空list进行lpop操作并返回，比如blpop list1 list2 list3 0 ,如果list不存在list2,list3都是非空则对list2做lpop并返回从list2中删除的元素。如果所有的list都是空或不存在，则会阻塞timeout秒，timeout为0表示一直阻塞。当阻塞时，如果有client对key1...keyN中的任意key进行push操作，则第一在这个key上被阻塞的client会立即返回。如果超时发生，则返回nil。有点像unix的select或者poll|
|brpop|同blpop，一个是从头部删除一个是从尾部删除|
|rpoplpush srckey destkey|从srckey对应list的尾部移除元素并添加到destkey对应list的头部,最后返回被移除的元素值，整个操作是原子的.如果srckey是空或者不存在返回nil|
4.**set:**
redis的set是string类型的无序集合。set元素最大可以包含(2的32次方-1)个元素。set的是通过hash table实现的，所以添加，删除，查找的复杂度都是O(1)。hash table会随着添加或者删除自动的调整大小。需要注意的是调整hash table大小时候需要同步（获取写锁）会阻塞其他读写操作。关于set集合类型除了基本的添加删除操作，其他有用的操作还包含集合的取并集(union)，交集(intersection)，差集(difference)。通过这些操作可以很容易的实现sns中的好友推荐和blog的tag功能。
下面详细介绍set相关命令：
|sadd key member|添加一个string元素到,key对应的set集合中，成功返回1,如果元素以及在集合中返回0,key对应的set不存在返回错误|
|----|----|
|srem key member|从key对应set中移除给定元素，成功返回1，如果member在集合中不存在或者key不存在返回0，如果key对应的不是set类型的值返回错误|
|spop key|删除并返回key对应set中随机的一个元素,如果set是空或者key不存在返回nil|
|srandmember key|同spop，随机取set中的一个元素，但是不删除元素|
|smove srckey dstkey member|从srckey对应set中移除member并添加到dstkey对应set中，整个操作是原子的。成功返回1,如果member在srckey中不存在返回0，如果key不是set类型返回错误|
|scard key|返回set的元素个数，如果set是空或者key不存在返回0|
|sismember key member|判断member是否在set中，存在返回1，0表示不存在或者key不存在|
|sinter key1 key2...keyN|返回所有给定key的交集|
|sinterstore dstkey key1...keyN|同sinter，但是会同时将交集存到dstkey下|
|sunion key1 key2...keyN|返回所有给定key的并集|
|sunionstore dstkey key1...keyN|同sunion，并同时保存并集到dstkey下|
|sdiff key1 key2...keyN|返回所有给定key的差集|
|sdiffstore dstkey key1...keyN|同sdiff，并同时保存差集到dstkey下|
|smembers key|返回key对应set的所有元素，结果是无序的|
5.**sorted set:**
sorted set和set一样也是string类型元素的集合，不同的是每个元素都会关联一个double类型的score。sorted set的实现是skip list（跳表）和hash table的混合体当元素被添加到集合中时，一个元素到score的映射被添加到hash table中，所以给定一个元素获取score的开销是O(1)，另一个score到元素的映射被添加到skip list并按照score排序，所以就可以有序的获取集合中的元素。添加，删除操作开销都是O(log(N))和skip list的开销一致，redis的skip
 list实现用的是双向链表，这样就可以逆序从尾部取元素。sorted set最经常的使用方式应该是作为索引来使用。我们可以把要排序的字段作为score存储，对象的id当元素存储。
下面是sorted set相关命令：
|zadd key score member|添加元素到集合，元素在集合中存在则更新对应score|
|----|----|
|zrem key member|删除指定元素，1表示成功，如果元素不存在返回0|
|zincrby key incr member|增加对应member的score值，然后移动元素并保持skip list保持有序。返回更新后的score值|
|zrank key member|返回指定元素在集合中的排名（下标）,集合中元素是按score从小到大排序的|
|zrevrank key member|同上,但是集合中元素是按score从大到小排序|
|zrange key start end|类似lrange操作从集合中去指定区间的元素。返回的是有序结果|
|zrevrange key start end|同上，返回结果是按score逆序的|
|zrangebyscore key min max|返回集合中score在给定区间的元素|
|zcount key min max|返回集合中score在给定区间的数量|
|zcard key|返回集合中元素个数|
|zscore key element|返回给定元素对应的score|
|zremrangebyrank key min max|删除集合中排名在给定区间的元素|
|zremrangebyscore key min max|删除集合中score在给定区间的元素|
6.**hash:**
redis hash是一个string类型的field和value的映射表。它的添加，删除操作都是O(1)（平均）。hash特别适合用于存储对象。相较于将对象的每个字段存成单个string类型。将一个对象存储在hash类型中会占用更少的内存，并且可以更方便的存取整个对象。内存的原因是因为它有存储优化机制，后面介绍。
下面介绍hash相关命令：
|hset key field value|设置hash field为指定值，如果key不存在，则先创建|
|----|----|
|hget key field|获取指定的hash field|
|hmget key filed1....fieldN|获取全部指定的hash filed|
|hmset key filed1 value1 ... filedN valueN|同时设置hash的多个field|
|hincrby key field integer|将指定的hash filed 加上给定值|
|hexists key field|测试指定field是否存在|
|hdel key field|删除指定的hash field|
|hlen key|返回指定hash的field数量|
|hkeys key|返回hash的所有field|
|hvals key|返回hash的所有value|
|hgetall|返回hash的所有filed和value|
**7. 查看状态信息：**
[root@md04 ~]# redis-cli info
redis_version:2.4.15
redis_git_sha1:00000000
redis_git_dirty:0
arch_bits:64
multiplexing_api:epoll
process_id:2204
uptime_in_seconds:25
uptime_in_days:0
lru_clock:2013834
used_cpu_sys:0.00
used_cpu_user:0.00
used_cpu_sys_children:0.00
used_cpu_user_children:0.00
connected_clients:1
connected_slaves:0
client_longest_output_list:0
client_biggest_input_buf:0
blocked_clients:0
used_memory:17505392
used_memory_human:16.69M
used_memory_rss:1617920
used_memory_peak:17496792
used_memory_peak_human:16.69M
mem_fragmentation_ratio:0.09
mem_allocator:jemalloc-2.2.1
loading:0
aof_enabled:1
changes_since_last_save:5
bgsave_in_progress:0
last_save_time:1320372561
bgrewriteaof_in_progress:0
total_connections_received:1
total_commands_processed:0
expired_keys:0
evicted_keys:0
keyspace_hits:2
keyspace_misses:8
pubsub_channels:0
pubsub_patterns:0
latest_fork_usec:0
vm_enabled:1
role:master
aof_current_size:237
aof_base_size:237
aof_pending_rewrite:0
vm_conf_max_memory:0
vm_conf_page_size:32
vm_conf_pages:134217728
vm_stats_used_pages:3
vm_stats_swapped_objects:3
vm_stats_swappin_count:0
vm_stats_swappout_count:3
vm_stats_io_newjobs_len:0
vm_stats_io_processing_len:0
vm_stats_io_processed_len:0
vm_stats_io_active_threads:0
vm_stats_blocked_clients:0
db0:keys=4,expires=0 
