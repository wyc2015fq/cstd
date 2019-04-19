# python结合redis的部分操作 - LC900730的博客 - CSDN博客
2017年05月16日 16:52:28[lc900730](https://me.csdn.net/LC900730)阅读数：239
## 列表
conn=redis.Redis();
#### LTRIM
- conn.ltrim(‘list-key’,2,-1);
- conn.lrange(‘list-key’,0,-1)
组合使用ltrim和lrange可以构建出来一个在功能上类似于LPOP或RPOP，但是能够一次返回并弹出多个元素的操作；
#### BLOP
- BLPOP key-name [key-name …] timeout—从第一个非空列表中弹出位于最左端的元素，或者在timeout秒之内堵塞并等待可弹出元素的出现
- BRPOP key-name [key-name …] timeout—从第一个非空列表中弹出位于最右端的元素，或者在timeout秒之内堵塞并等待可弹出元素的出现
- RPOPLPUSH source-key dest-key—从source-key列表中弹出位于最右端的元素，然后将这个元素推入dest-key列表的最左端，然后向用户返回这个元素
- BRPOPLPUSH source-key dest-key   timeout—从source-key列表中弹出位于最右端的元素，然后将这个元素推入dest-key列表的最左端，然后向用户返回这个元素;如果source-key为空，那么在timeout秒之内堵塞并等待可弹出的元素出现
在redis中，多个命令原子地执行指的是，在这些命令正在读取或者修改数据的时候，其他客户端不能读取或者修改相同的数据
conn.blpop([‘list’,’list2’],1);//blpop命令会从左到右检查传入的列表，并对最先遇到的非空列表执行弹出操作
#### 列表的一个主要优点是它可以包含多个字符串值，这使得用户可以将数据集中在同一个地方，列表允许有重复元素
## 集合
redis集合以无序的方式来存储多个不相同的元素，用户可以快速的对集合执行添加元素操作、移除元素操作以及检查一个元素是否在集合中 
- SADD
SADD key-name item [item …]:将一个或者多个元素添加到集合里面，并返回被添加元素当中原本并不存在于集合里面的元素数量； 
- SREM
SREM key-name item [item …]:将集合中移除一个或者多个元素，并返回被移除元素的个数； 
- SREM
SREM key-name item [item …]:将集合中移除一个或者多个元素，并返回被移除元素的个数； 
- SISMEMBER
SISMEMBER key-name item:检查元素item是否存在于集合key-name里； 
- SMEMBERS
SISMEMBER key-name :返回集合中包含的所有元素； 
- SRANDMEMBER
SRANDMEMBER key-name [count] :从集合中随机返回一个或者多个元素；当count为正数时候，返回的随机元素不会重复；当count为负数，命令返回的随机元素可能会出现重复 
- SPOP
SPOP key-name:随机返回集合中一个元素，并返回被移除的元素 
- SMOVE
SMOVE source-key dest-key item :如果集合source-key包含元素item，那么从集合source-key里面移除元素item，并将元素item添加到集合dest-key中；如果item被成功移除，那么命令返回1，否则返回0；
### 处理集合相关
- SDIFF
SDIFF key-name [key-name …]:返回那些存在于第一个集合中，但是不存在其他集合中的元素(数学上的差集) 
- SDIFFSTORE
SDIFFSTORE dest-key key-name [key-name …]:将那些存在于第一个集合中，但是不存在其他集合中的元素(数学上的差集运算)存储到dest-key键里面 
- SINTER
SINTER key-name [key-name …]:返回那些同时存在于所有集合中的元素(数学上的交集) 
- SINTERSTORE
SINTERSTORE dest-key-name [key-name …]:返回那些同时存在于所有集合中的元素(数学上的交集) 
- SUNION
SUNION key-name [key-name …]:返回那些至少存在于一个集合中的元素(数学上的并集) 
- SINTERSTORE
SUNIONSTORE dest-key key-name [key-name …]:将那些至少存在于一个集合中的元素存储到dest-key键里面(数学上的交集)
## hash散列
- HMGET
HMGET key-name key [key …]：从散列里面获取一个或者多个键的值； 
conn.hmget(‘hash-key’,[‘k2’,’k3’])  //[‘v2’,’v3’] 
- HMSET
HMSET key-name key value [key value…]：为散列里面的一个或者多个键设置值； 
conn.hmset(‘hash-key’,{‘k1’:’v1’,’k2’:’v2’,’k3’:’v3’})  //TRUE 
- HDEL
HDEL key-name key [key …]:删除散列里面的一个或者多个键值对，返回成功找到删除的键值对的数量 
- HLEN
HLEN key-name :返回散列包含的键值对的数量 
conn.hlen(‘hash-key’)  //3 
- HEXISTS
HEXISTS key-name key:检查给定键是否存在于散列中 
- HKEYS
HKEYS key-name :获取散列包含的所有键 
- HVALS
HVALS key-name :获取散列包含的所有值 
- HGETALL
HGETALL key-name :获取散列包含的所有键值对 
- HINCRBY
HINCRBY key-name key increment :将键key存储的值加上整数increment 
//和字符串一样，对散列中一个尚未存在的键执行自增操作时候，redis会将键的值当做0来处理 
- HINCRBYFLOAT
HINCRBYFLOAT key-name key increment :将键key存储的值加上浮点数increment
## 有序集合
有序集合中存储着成员与分值之间的映射 
- ZADD
ZADD key-name score member [score member…]:将带有给定分值的成员添加到有序集合中 
conn.zadd(‘zset-key’,’a’,3,’b’,2,’c’,1);  //3 
- ZREM
ZREM key-name  member [ member…]:从有序集合中移除给定的成员，并返回被移除成员的数量 
- ZCARD
ZCARD key-name :返回有序集合包含成员的数量 
conn.zcard(‘zset-key’);  //3 
- ZINCRBY
ZINCRBY key-name increment member :将member成员的分值加上increment 
conn.zincrby(‘zset-key’,’c’,3);  //4.0 
- ZCOUNT
ZCOUNT key-name min max :返回分值介于min和max之间的成员数量 
- ZRANK
ZRANK key-name member :返回成员member在有序集合中的排名 
- ZSCORE/li> 
ZSCORE key-name member :返回成员member的分值 
conn.zscore(‘zset-key’,’b’);  //2.0 
- ZRANGE
ZRANGE key-name start stop [WITHSCORES] :返回有序集合中排名介于start和stop之间的成员，如果给定了可选的WITHSCORE选项，那么命令会将成员分值也一并返回。 
conn.zrange(‘zset-key’,0,-1,withscores=True);  //[(‘a’,3.0),(‘c’,4.0)]
