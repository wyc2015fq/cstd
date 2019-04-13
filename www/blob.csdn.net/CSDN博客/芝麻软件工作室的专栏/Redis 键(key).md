
# Redis 键(key) -  芝麻软件工作室的专栏 - CSDN博客


2016年07月27日 10:18:48[seven-soft](https://me.csdn.net/softn)阅读数：166个人分类：[Redis																](https://blog.csdn.net/softn/article/category/6332853)



Redis 键命令用于管理 redis 的键。
### 语法
Redis 键命令的基本语法如下：
redis 127.0.0.1:6379> COMMAND KEY_NAME
### 实例
redis 127.0.0.1:6379> SET w3ckey redis
OK
redis 127.0.0.1:6379> DEL w3ckey
(integer) 1在以上实例中**DEL**是一个命令，**w3ckey**是一个键。 如果键被删除成功，命令执行后输出**(integer) 1**，否则将输出**(integer) 0**
---

## Redis keys 命令
下表给出了与 Redis 键相关的基本命令：
|序号|命令及描述|
|1|[DEL key](http://www.runoob.com/redis/keys-del.html)
|该命令用于在 key 存在是删除 key。|
|2|[DUMP key](http://www.runoob.com/redis/keys-dump.html)
|序列化给定 key ，并返回被序列化的值。|
|3|[EXISTS key](http://www.runoob.com/redis/keys-exists.html)
|检查给定 key 是否存在。|
|4|[EXPIRE key](http://www.runoob.com/redis/keys-expire.html)|seconds
|为给定 key 设置过期时间。|
|5|[EXPIREAT key timestamp](http://www.runoob.com/redis/keys-expireat.html)
|EXPIREAT 的作用和 EXPIRE 类似，都用于为 key 设置过期时间。 不同在于 EXPIREAT 命令接受的时间参数是 UNIX 时间戳(unix timestamp)。|
|6|[PEXPIRE key milliseconds](http://www.runoob.com/redis/keys-pexpire.html)
|设置 key 的过期时间亿以毫秒计。|
|7|[PEXPIREAT key milliseconds-timestamp](http://www.runoob.com/redis/keys-pexpireat.html)
|设置 key 过期时间的时间戳(unix timestamp) 以毫秒计|
|8|[KEYS pattern](http://www.runoob.com/redis/keys-keys.html)
|查找所有符合给定模式( pattern)的 key 。|
|9|[MOVE key db](http://www.runoob.com/redis/keys-move.html)
|将当前数据库的 key 移动到给定的数据库 db 当中。|
|10|[PERSIST key](http://www.runoob.com/redis/keys-persist.html)
|移除 key 的过期时间，key 将持久保持。|
|11|[PTTL key](http://www.runoob.com/redis/keys-pttl.html)
|以毫秒为单位返回 key 的剩余的过期时间。|
|12|[TTL key](http://www.runoob.com/redis/keys-ttl.html)
|以秒为单位，返回给定 key 的剩余生存时间(TTL, time to live)。|
|13|[RANDOMKEY](http://www.runoob.com/redis/keys-randomkey.html)
|从当前数据库中随机返回一个 key 。|
|14|[RENAME key newkey](http://www.runoob.com/redis/keys-rename.html)
|修改 key 的名称|
|15|[RENAMENX key newkey](http://www.runoob.com/redis/keys-renamenx.html)
|仅当 newkey 不存在时，将 key 改名为 newkey 。|
|16|[TYPE key](http://www.runoob.com/redis/keys-type.html)
|返回 key 所储存的值的类型。|

