# redis 命令行操作记录 - gauss的专栏 - CSDN博客
2014年09月09日 16:55:32[gauss](https://me.csdn.net/mathlmx)阅读数：357
redis命令学习
Redis 的vaule
redis 提供五种数据类型：string,hash,list,set 及sorted set。
hash 类型
hash 是一个string 类型的field 和value 的映射表。添加，删除操作都是O(1)（平均）。
hash 特别适合用于存储对象。相对于将对象的每个字段存成单个string 类型。将一个对象
存储在hash 类型中会占用更少的内存，并且可以更方便的存取整个对象。省内存的原因是
新建一个hash 对象时开始是用zipmap（又称为small hash）来存储的。这个zipmap 其实并
不是hash table，但是zipmap 相比正常的hash 实现可以节省不少hash 本身需要的一些元
数据存储开销。尽管zipmap 的添加，删除，查找都是O(n)，但是由于一般对象的field
数量都不太多。所以使用zipmap 也是很快的,也就是说添加删除平均还是O(1)。如果field
或者value 的大小超出一定限制后，redis 会在内部自动将zipmap 替换成正常的hash 实现.
这个限制可以在配置文件中指定。
hash-max-zipmap-entries 64 #配置字段最多64 个
hash-max-zipmap-value 512 #配置value 最大为512 字节
hash 类型数据操作指令简介
hset key field value 设置hash field为指定值，如果key不存在，则创建
hget key field 获取指定的hash field。
hmget key filed1....fieldN 获取全部指定的hash filed。
hmset key filed1 value1 ...... filedN valueN 同时设置hash的多个field。
hincrby key field integer 将指定的hash filed 加上指定值。成功返回hash filed 变更后的
值。
hexists key field 检测指定field是否存在。
hdel key field 删除指定的hash field。
hlen key 返回指定hash的field 数量。
5
hkeys key 返回hash 的所有field。
hvals key 返回hash的所有value。
hgetall 返回hash的所有filed和value
1 hset命令
对hash中的字段赋值，如果不存在则创建否则覆盖
返回值:被创建返回1，被更新返回0
例子:
**[php]**[view plain](http://blog.csdn.net/xinguan1267/article/details/7844304#)[copy](http://blog.csdn.net/xinguan1267/article/details/7844304#)
- redis 127.0.0.1:6379> hset debughash field1 "hello"
- (integer) 1  
- redis 127.0.0.1:6379> hget debughash field1  
- "hello"
- redis 127.0.0.1:6379> hset debughash field1 "world"
- (integer) 0  
- redis 127.0.0.1:6379> hget debughash field1  
- "world"
- redis 127.0.0.1:6379>   
2 hget命令
获取field对于的value
返回值:存在返回value值，否则返回 nil
例子:
**[php]**[view plain](http://blog.csdn.net/xinguan1267/article/details/7844304#)[copy](http://blog.csdn.net/xinguan1267/article/details/7844304#)
- redis 127.0.0.1:6379> hset debughash field2 "hello"
- (integer) 1  
- redis 127.0.0.1:6379> hget debughash field2  
- "hello"
- redis 127.0.0.1:6379> hget debughash field3  
- (nil)  
3 hexists
判断field是否存在
返回值:存在返回1，否则返回0
例子:
**[php]**[view plain](http://blog.csdn.net/xinguan1267/article/details/7844304#)[copy](http://blog.csdn.net/xinguan1267/article/details/7844304#)
- redis 127.0.0.1:6379> hexists debughash field2  
- (integer) 1  
- redis 127.0.0.1:6379> hexists debughash field3  
- (integer) 0  
4 hdel
删除field
返回值:返回删除的数量
例子:
**[php]**[view plain](http://blog.csdn.net/xinguan1267/article/details/7844304#)[copy](http://blog.csdn.net/xinguan1267/article/details/7844304#)
- redis 127.0.0.1:6379> hdel debughash field2  
- (integer) 1  
- redis 127.0.0.1:6379> hdel debughash field3  
- (integer) 0  
- redis 127.0.0.1:6379> hget debughash field3  
- (nil)  
5 hkeys
返回hash下包含的field
例子:
**[php]**[view plain](http://blog.csdn.net/xinguan1267/article/details/7844304#)[copy](http://blog.csdn.net/xinguan1267/article/details/7844304#)
- redis 127.0.0.1:6379> hkeys debughash  
- 1) "field1"
- redis 127.0.0.1:6379> hset debughash field2 "word"
- (integer) 1  
- redis 127.0.0.1:6379> HKEYS debughash  
- 1) "field1"
- 2) "field2"
- redis 127.0.0.1:6379> HKEYS debughash field2  
- (error) ERR wrong number of arguments for'hkeys' command  
- redis 127.0.0.1:6379> HKEYS debughash  
- 1) "field1"
- 2) "field2"
- redis 127.0.0.1:6379>   
6 hlen
返回hash下含有field的数量
例子:
**[php]**[view plain](http://blog.csdn.net/xinguan1267/article/details/7844304#)[copy](http://blog.csdn.net/xinguan1267/article/details/7844304#)
- redis 127.0.0.1:6379> hlen debughash  
- (integer) 2  
7 hmset
设置多个field值，存在覆盖，否则创建
例子:
**[php]**[view plain](http://blog.csdn.net/xinguan1267/article/details/7844304#)[copy](http://blog.csdn.net/xinguan1267/article/details/7844304#)
- redis 127.0.0.1:6379> hmset testhash field1 "hello" field2 "world"
- OK  
- redis 127.0.0.1:6379> hget testhash field1  
- "hello"
- redis 127.0.0.1:6379> hget testhash field2  
- "world"
8 hmget 
获取多个field的值
例子:
**[php]**[view plain](http://blog.csdn.net/xinguan1267/article/details/7844304#)[copy](http://blog.csdn.net/xinguan1267/article/details/7844304#)
- redis 127.0.0.1:6379> hmget testhash field1 field2 field3  
- 1) "hello"
- 2) "world"
- 3) (nil)  
9 hgetall
获取hash下所有field与value
例子:
**[php]**[view plain](http://blog.csdn.net/xinguan1267/article/details/7844304#)[copy](http://blog.csdn.net/xinguan1267/article/details/7844304#)
- redis 127.0.0.1:6379> hgetall testhash  
- 1) "field1"
- 2) "hello"
- 3) "field2"
- 4) "world"
10 hvals
获取value值
例子:
**[php]**[view plain](http://blog.csdn.net/xinguan1267/article/details/7844304#)[copy](http://blog.csdn.net/xinguan1267/article/details/7844304#)
- redis 127.0.0.1:6379> hvals testhash  
- 1) "hello"
- 2) "world"
11 hsetnx
只有field不存在的时候，设置value
返回值：如果创建成功返回1，若存在返回0
例子:
**[php]**[view plain](http://blog.csdn.net/xinguan1267/article/details/7844304#)[copy](http://blog.csdn.net/xinguan1267/article/details/7844304#)
- redis 127.0.0.1:6379> hgetall testhash  
- 1) "field1"
- 2) "hello"
- 3) "field2"
- 4) "world"
- redis 127.0.0.1:6379> hsetnx testhash field3 "00000"
- (integer) 1  
- redis 127.0.0.1:6379> hgetall testhash  
- 1) "field1"
- 2) "hello"
- 3) "field2"
- 4) "world"
- 5) "field3"
- 6) "00000"
- redis 127.0.0.1:6379>   
12 hincrby 
按整数自增，如果不存在则field赋值为value，如果不是数值型的字符串，返回error
返回值:返回自增后的值
例子:
**[php]**[view plain](http://blog.csdn.net/xinguan1267/article/details/7844304#)[copy](http://blog.csdn.net/xinguan1267/article/details/7844304#)
- redis 127.0.0.1:6379> hgetall testhash  
- 1) "field1"
- 2) "hello"
- 3) "field2"
- 4) "world"
- 5) "field3"
- 6) "00000"
- redis 127.0.0.1:6379> hincrby testhash field4 5  
- (integer) 5  
- redis 127.0.0.1:6379> hicrby testhash field4 5  
- (error) ERR unknown command 'hicrby'
- redis 127.0.0.1:6379> hincrby testhash field4 5  
- (integer) 10  
- redis 127.0.0.1:6379> hincrby testhash field4 -11  
- (integer) -1  
- redis 127.0.0.1:6379> hset testhash field5 "yy"
- (integer) 1  
- redis 127.0.0.1:6379> hincrby testhash field5 -11  
- (error) ERR hash value is not an integer  
- redis 127.0.0.1:6379>   
13 hincrbyfloat
按浮点数自增

