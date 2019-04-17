# Ubuntu server 15.0.4 Redis相关笔记及配置参数说明 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年10月27日 18:10:45[boonya](https://me.csdn.net/boonya)阅读数：716标签：[ubuntu																[redis																[cluster](https://so.csdn.net/so/search/s.do?q=cluster&t=blog)
个人分类：[Memcache/Redis																[Linux](https://blog.csdn.net/boonya/article/category/1740759)](https://blog.csdn.net/boonya/article/category/5904837)





#### 操作命令及资料

```
# Redis集群教程
http://www.redis.cn/topics/cluster-tutorial.html
http://redis.io/topics/cluster-tutorial

# Redis客户端下载
http://www.redis.cn/clients.html

# Redis在线教程
http://www.yiibai.com/redis/

# 定位集群节点
cd ./redis-3.0.3/cluster/7000

# 启动redis节点
redis-server redis.conf

# 定位到redis src目录
cd ./redis-3.0.3/src

# 设置集群节点(本地测试，这种方式局域网也可以正常访问)
./redis-trib.rb create --replicas 1 127.0.0.1:7000 127.0.0.1:7001 127.0.0.1:7002 127.0.0.1:7003 127.0.0.1:7004 127.0.0.1:7005
# 设置集群节点（局域网测试）
./redis-trib.rb create --replicas 1 192.168.28.194:7000 192.168.28.194:7001 192.168.28.194:7002 192.168.28.194:7003 192.168.28.194:7004 192.168.28.194:7005

sudo gem install redis

# 查询集群节点
check   ./redis-trib.rb check 127.0.0.1:7000

# 新增节点Adding node 127.0.0.1:7006 to cluster 127.0.0.1:7000
add  ./redis-trib.rb add-node 127.0.0.1:7006 127.0.0.1:7000
   

# 根据节点(Master或Slave)ID删除节点
del  ./redis-trib.rb del-node 127.0.0.1:7007 ‘a7c4da616ce671e3da6f4447af7f89e189af6ffe’

# 删除redis存在的数据和节点配置
rm  dump.rdb  nodes.conf

# 绑定Host
bind  192.168.28.194

# 修改Redis密码，Redis默认没有密码(redis.conf文件中修改)
requirepass mypassword

# Redis在线API
http://www.boyunjian.com/javadoc/org.apache.servicemix.bundles/org.apache.servicemix.bundles.jedis/2.4.2_1/_/redis/clients/jedis/exceptions/JedisMovedDataException.html
http://grepcode.com/project/repo1.maven.org/maven2/redis.clients/jedis/

# 淘宝Redis代码（TaoCode）示例
http://code.taobao.org/p/only/src/trunk/redis-demo/

# Redis研究文摘
http://blog.csdn.net/moxiaomomo/article/category/1433230

# Redis windows版本安装
http://www.cnblogs.com/edisonfeng/p/3571870.html
```

#### 集群API常用方法说明

##### 1）连接操作命令

quit：关闭连接（connection）

auth：简单密码认证

help cmd： 查看cmd帮助，例如：help quit


##### 2）持久化

save：将数据同步保存到磁盘

bgsave：将数据异步保存到磁盘

lastsave：返回上次成功将数据保存到磁盘的Unix时戳

shundown：将数据同步保存到磁盘，然后关闭服务


##### 3）远程服务控制

info：提供服务器的信息和统计

monitor：实时转储收到的请求

slaveof：改变复制策略设置

config：在运行时配置Redis服务器


##### 4）对value操作的命令

exists(key)：确认一个key是否存在

del(key)：删除一个key

type(key)：返回值的类型

keys(pattern)：返回满足给定pattern的所有key

randomkey：随机返回key空间的一个

keyrename(oldname, newname)：重命名key

dbsize：返回当前数据库中key的数目

expire：设定一个key的活动时间（s）

ttl：获得一个key的活动时间

select(index)：按索引查询

move(key, dbindex)：移动当前数据库中的key到dbindex数据库

flushdb：删除当前选择数据库中的所有key

flushall：删除所有数据库中的所有key


##### 5）String

set(key, value)：给数据库中名称为key的string赋予值value

get(key)：返回数据库中名称为key的string的value

getset(key, value)：给名称为key的string赋予上一次的value

mget(key1, key2,…, key N)：返回库中多个string的value

setnx(key, value)：添加string，名称为key，值为value

setex(key, time, value)：向库中添加string，设定过期时间time

mset(key N, value N)：批量设置多个string的值

msetnx(key N, value N)：如果所有名称为key i的string都不存在

incr(key)：名称为key的string增1操作

incrby(key, integer)：名称为key的string增加integer

decr(key)：名称为key的string减1操作

decrby(key, integer)：名称为key的string减少integer

append(key, value)：名称为key的string的值附加value

substr(key, start, end)：返回名称为key的string的value的子串


##### 6）List

rpush(key, value)：在名称为key的list尾添加一个值为value的元素

lpush(key, value)：在名称为key的list头添加一个值为value的 元素

llen(key)：返回名称为key的list的长度

lrange(key, start, end)：返回名称为key的list中start至end之间的元素

ltrim(key, start, end)：截取名称为key的list

lindex(key, index)：返回名称为key的list中index位置的元素

lset(key, index, value)：给名称为key的list中index位置的元素赋值

lrem(key, count, value)：删除count个key的list中值为value的元素

lpop(key)：返回并删除名称为key的list中的首元素

rpop(key)：返回并删除名称为key的list中的尾元素

blpop(key1, key2,… key N, timeout)：lpop命令的block版本。

brpop(key1, key2,… key N, timeout)：rpop的block版本。

rpoplpush(srckey, dstkey)：返回并删除名称为srckey的list的尾元素，并将该元素添加到名称为dstkey的list的头部


##### 7）Set

sadd(key, member)：向名称为key的set中添加元素member

srem(key, member) ：删除名称为key的set中的元素member

spop(key) ：随机返回并删除名称为key的set中一个元素

smove(srckey, dstkey, member) ：移到集合元素

scard(key) ：返回名称为key的set的基数

sismember(key, member) ：member是否是名称为key的set的元素

sinter(key1, key2,…key N) ：求交集

sinterstore(dstkey, (keys)) ：求交集并将交集保存到dstkey的集合

sunion(key1, (keys)) ：求并集

sunionstore(dstkey, (keys)) ：求并集并将并集保存到dstkey的集合

sdiff(key1, (keys)) ：求差集

sdiffstore(dstkey, (keys)) ：求差集并将差集保存到dstkey的集合

smembers(key) ：返回名称为key的set的所有元素

srandmember(key) ：随机返回名称为key的set的一个元素


##### 8）Hash

hset(key, field, value)：向名称为key的hash中添加元素field

hget(key, field)：返回名称为key的hash中field对应的value

hmget(key, (fields))：返回名称为key的hash中field i对应的value

hmset(key, (fields))：向名称为key的hash中添加元素field

hincrby(key, field, integer)：将名称为key的hash中field的value增加integer

hexists(key, field)：名称为key的hash中是否存在键为field的域

hdel(key, field)：删除名称为key的hash中键为field的域

hlen(key)：返回名称为key的hash中元素个数

hkeys(key)：返回名称为key的hash中所有键

hvals(key)：返回名称为key的hash中所有键对应的value

hgetall(key)：返回名称为key的hash中所有的键（field）及其对应的value

Redis编译(make)完成后,会产生六个文件:

redis-server：这个是redis的服务器

redis-cli：这个是redis的客户端

redis-check-aof：这个是检查AOF文件的工具

redis-check-dump：这个是本地数据检查工具

redis-benchmark：性能基准测试工具，安装完后可以测试一下当前Redis的性能

redis-sentinel：Redis监控工具，集群管理工具



#### Redis配置文件参数说明



Redis的配置文件是：redis.conf

常用配置项为:

daemonize: 是否以后台进程运行，默认为no

pidfile /var/run/redis.pid: pid文件路径

port 6379: 监听端口

bind 127.0.0.1:绑定主机ip

unixsocket /tmp/redis.sock：sock文件路径

timeout 300：超时时间，默认是300s

loglevel verbose：日志等级，可选项有debug:大量的信息，开发和测试有用；verbose：很多极其有用的信息，但是不像debug那么乱；notice：在生产环境中你想用的信息；warning：最关键、最重要的信息才打印。 默认是erbose

logfile stdout：日志记录方式，默认是stdout

syslog-enabled no：日志记录到系统日志中，默认是no

syslog-ident redis：指定系统日志标识

syslog-facility local0：指定系统日志设备，必须是USER或者 LOCAL0~LOCAL7。 默认是local0

databases 16：数据库的数量，默认的数据库是DB 0，你可以使用 SELECT 来选择不同的数据库。dbid的范围是0~(你设置的值-1)


save <seconds> <changes>：RDB在多长时间内，有多少次更新操作，就将数据同步到数据文件。

save 900 1：15min内至少1个key被改变

save 300 10：5min内至少有300个key被改变

save 60 10000：60s内至少有10000个key被改变

rdbcompression yes：存储至本地数据库时是否压缩数据，默认是yes

dbfilename dump.rdb：本地数据库文件名，默认是dump.rdb

dir ./：本地数据库存放路径，默认是./

slaveof <masterip> <masterport>：当本机为从服务时，设置主服务的ip以及端口

masterauth <master-password>：主服务的连接密码


从结点与主结点失去连接、或者正在复制时，从结点对客户端请求的处理方式：

slave-serve-stale-data yes：yes：从结点继续响应客户端的请求，但是数据有可能不准确或者为空 no：除了INFO和SLAVEOF以外，其它的命令都返回“SYNC with master in progress”


requirepass foobared：连接密码foobared

maxclients 128：最大连接数，默认不限制

maxmemory <bytes>：设置最大内存，达到最大内存设置后，redis会先尝试清除已到期或即将到期的key,当此方法处理后，任然到达最大内存设置，将无法再进行写入操作


下面是maxmemory的策略

maxmemory-policy volatile-lru：maxmemory设置策略，默认是volatile-lru

volatile-lru：使用LRU算法，从过期集中移除

allkeys-lru：根据LRU算法移除key

volatile-random：从过期集中随机移动一个

allkeys-random：随机移除一个

volatile-ttl： 根据最近过期时间移除key

noeviction：不移除数据,客户端写操作时返回错误 don’t expire at all, just return an error on write operations


maxmemory-samples 3


appendonly no：是否 在每次更新操作后进行日志记录，如果不开启，可能会在断电时导致一段时间内的数据丢失。因为redis本身同步数据文件是按照上面save条件来进行同步的，所以有的数据会在一段时间内只存在于内存中。默认是no

appendfilename appendonly.aof：更新日志文件名，默认是appendonly.aof

redis支持的三种不同的同步方式:

no: don’t fsync, just let the OS flush the data when it wants. Faster. //等待OS进行数据缓存同步到硬盘

always: fsync after every write to the append only log . Slow, Safest. //每次更新操作后调用fsync()将数据写到磁盘

everysec: fsync only if one second passed since the last fsync. Compromise. //每秒同步一次

appendfsync everysec //更新日志条件，默认是everysec

no-appendfsync-on-rewrite no


slowlog-log-slower-than 10000：设置redis slow log时间，只包括命令执行时间，不包括IO操作时间，比如客户端连接，应答相应时间等等。单位是microseconds(一百万分之一秒)，默认是10000.负值表示禁用slow log,0表示记录所有命令。

slowlog-max-len 1024：slowlog最大长度1024.这会消耗内存，使用SLOWLOG RESET来回收slowlog内存。


vm-enabled no //是否使用虚拟内存，默认是no。在redis2.4版本，强烈不建议使用virtual memory。

vm-swap-file /tmp/redis.swap //虚拟内存文件路径，默认是/tmp/redis.swap，不可多个redis实例共享虚拟内存文件。

vm-max-memory 0 //设置最大vm，默认为0，所有的value存在于磁盘中。

vm-page-size 32 //设置vm的page大小，默认是32

vm-pages 134217728 //设置swap文件中最大memory pages，默认是134217728。swap大小=vm-page-size * vm-pages

vm-max-threads 4 //vm同时运行的最大io线程


指定在超过一定的数量或者最大的元素超过某一临界值时，采用一种特殊的哈希算法：

hash-max-zipmap-entries 512

hash-max-zipmap-value 64

list-max-ziplist-entries 512

list-max-ziplist-value 64

set-max-intset-entries 512

activerehashing yes //是否重置hash表](https://so.csdn.net/so/search/s.do?q=redis&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)




