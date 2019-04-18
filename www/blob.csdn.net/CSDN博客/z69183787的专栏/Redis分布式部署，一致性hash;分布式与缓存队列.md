# Redis分布式部署，一致性hash;分布式与缓存队列 - z69183787的专栏 - CSDN博客
2017年08月04日 11:27:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：807
[http://blog.csdn.net/yfkiss/article/details/39996129](http://blog.csdn.net/yfkiss/article/details/39996129)
Redis 3.0.0 RC1版本10.9号发布，[Release Note](https://raw.githubusercontent.com/antirez/redis/3.0/00-RELEASENOTES)
这个版本支持Redis Cluster，相信很多同学期待已久，不过这个版本只是RC版本，要应用到生产环境，还得等等
Redis Cluster设计要点：
## 架构：无中心
Redis Cluster采用无中心结构，每个节点都保存数据和整个集群的状态
每个节点都和其他所有节点连接，这些连接保持活跃
使用gossip协议传播信息以及发现新节点
node不作为client请求的代理，client根据node返回的错误信息重定向请求
## 数据分布：预分桶
预分好16384个桶，根据 CRC16(key) mod 16384的值，决定将一个key放到哪个桶中
每个Redis物理结点负责一部分桶的管理，当发生Redis节点的增减时，调整桶的分布即可
例如，假设Redis Cluster三个节点A/B/C，则
Node A 包含桶的编号可以为: 0 到 5500.
Node B 包含桶的编号可以为: 5500 到 11000.
Node C包含桶的编号可以为: 11001 到 16384.
当发生Redis节点的增减时，调整桶的分布即可。
预分桶的方案介于“硬Hash”和“一致性Hash”之间，牺牲了一定的灵活性，但相比“一致性Hash“，数据的管理成本大大降低
## 可用性：Master-Slave
为了保证服务的可用性，Redis Cluster采取的方案是的Master-Slave
每个Redis Node可以有一个或者多个Slave。当Master挂掉时，选举一个Slave形成新的Master
一个Redis  Node包含一定量的桶，当这些桶对应的Master和Slave都挂掉时，这部分桶对应的数据不可用
## 写
Redis Cluster使用异步复制
一个完整的写操作步骤：
1.client写数据到master
2.master告诉client "ok"
3.master传播更新到slave
存在数据丢失的风险：
1. 上述写步骤1）和2）成功后，master crash，而此时数据还没有传播到slave
2. 由于分区导致同时存在两个master，client向旧的master写入了数据。
当然，由于Redis Cluster存在超时及故障恢复机制，第2个风险基本上不可能发生
## 数据迁移
Redis Cluster支持在线增/减节点。
基于桶的数据分布方式大大降低了迁移成本，只需将数据桶从一个Redis Node迁移到另一个Redis Node即可完成迁移。
当桶从一个Node A向另一个Node B迁移时，Node A和Node B都会有这个桶，Node A上桶的状态设置为MIGRATING，Node B上桶的状态被设置为IMPORTING
当客户端请求时：
所有在Node A上的请求都将由A来处理，所有不在A上的key都由Node B来处理。同时，Node A上将不会创建新的key
## 多key操作
当系统从单节点向多节点扩展时，多key的操作总是一个非常难解决的问题，Redis Cluster方案如下：
1. 不支持多key操作
2. 如果一定要使用多key操作，请确保所有的key都在一个node上，具体方法是使用“hash tag”方案
hash tag方案是一种数据分布的例外情况
Reference：
[Redis cluster tutorial](http://redis.io/topics/cluster-tutorial)
[Redis cluster Specification](http://redis.io/topics/cluster-spec)
http://zhoushouby.blog.51cto.com/9150272/1560346
redis 3.0 cluster 集群 学习之路篇 [1]
2014-10-02 11:35:15
标签：[集群](http://blog.51cto.com/tag-%E9%9B%86%E7%BE%A4.html)[redis
 3.0](http://blog.51cto.com/tag-redis%203.0%20.html)
版权声明：原创作品，谢绝转载！否则将追究法律责任。
周氏一族，整理技术文档，给下一代留点教程......
目前，项目用的redis主备，感觉超不爽，很多局限性，特别是在 redis master 宕机之后，维护非常麻烦，寻思着弄一个redis集群，可不，总算到了今年10.1，redis发布了cluster版本。开启摸索之路...
很多人，一看到官网有最新的cluster版本，满怀热血，第一件事，就是搭建cluster环境，其实，鄙人却不，还是要从基层走起，先来了解一下官方信息，整理成下面的，大部分都是根据网络上整理出来的，又存在相同，纯属借鉴，往见谅！！！
redis主备面临的问题需求
1、主备模式，过于单一，特别是主服务器宕机的情况下，数据没法写入。
2、业务发展，数据庞大，主备必须同步相同数据，对存储需求较大，有点多余
3、虽然说有sentinel，但是效率还是非常低
Redis集群是一个可以在多个 Redis 节点之间进行数据共享的设施。 这一点毫无疑问的！
Redis集群通过分区来提供一定程度的可用性：即使集群中有一部分节点失效或者无法进行通讯， 集群也可以继续处理命令请求。
Redis 集群提供了以下两个好处：
（1）将数据自动切分（split）到多个节点的能力。
           面临大数据时代，集群，就是得要能实现一台宕机，不影响整个集群运作，换句话说，数据得每台机器都必须拥有对方的data，但是这样由于业务的慢慢扩展，数据会原来越大，很明显对存储比较吃力。redis在这一点上面，做得比较完美，那就是"切片"。通过内部 “私有算法技术”，把data分布式存储到集群机器，当要读取数据时，再通过 “私有内部算法” 映射到对应机器读取。
（2）当集群中的一部分节点失效或者无法进行通讯时， 仍然可以继续处理命令请求的能力。
          因为在3.0版本当中，redis采用master n-1 salver的模式，也就是说，一台master主，就若干台备，甚至没有备也行，但是本人强烈建议要1:1的模式，条件允许的情况下，建议1:n的模式
2、集群的数据分片功能：
集群要实现的目的是要将不同的 key 分散放置到不同的 redis 节点，这里我们需要一个规则或者算法，通常的做法是获取 key 的哈希值，然后根据节点数来求模，但这种做法有其明显的弊端，当我们需要增加或减少一个节点时，会造成大量的 key 无法命中，这种比例是相当高的，所以就有人提出了一致性哈希的概念。
一致性哈希有四个重要特征：
均衡性：也有人把它定义为平衡性，是指哈希的结果能够尽可能分布到所有的节点中去，这样可以有效的利用每个节点上的资源。
单调性：对于单调性有很多翻译让我非常的不解，而我想要的是当节点数量变化时哈希的结果应尽可能的保护已分配的内容不会被重新分派到新的节点。
分散性和负载：这两个其实是差不多的意思，就是要求一致性哈希算法对 key 哈希应尽可能的避免重复。
但是：
Redis 集群没有使用一致性hash, 而是引入了哈希槽的概念。
Redis 集群有16384个哈希槽,每个key通过CRC16校验后对16384取模来决定放置哪个槽.集群的每个节点负责一部分hash槽。这种结构很容易添加或者删除节点，并且无论是添加删除或者修改某一个节点，都不会造成集群不可用的状态。
使用哈希槽的好处就在于可以方便的添加或移除节点。
当需要增加节点时，只需要把其他节点的某些哈希槽挪到新节点就可以了；
当需要移除节点时，只需要把移除节点上的哈希槽挪到其他节点就行了；
在这一点上，我们以后新增或移除节点的时候不用先停掉所有的 redis 服务，very good的
3、Redis集群的主从架构：
为了使在部分节点失败或者大部分节点无法通信的情况下集群仍然可用，所以集群使用了主从复制模型,每个节点都会有N-1个复制品。
例如有A，B，C三个节点的集群,在没有复制模型的情况下,如果节点B失败了，那么整个集群就会以为缺少B节点所承担的哈希槽这个范围的槽而不可用。
然而如果在集群创建的时候（或者过一段时间）我们为每个节点添加一个从节点A1，B1，C1,那么整个集群便有三个master节点和三个slave节点组成，这样在节点B失败后，集群便会选举B1为新的主节点继续服务，整个集群便不会因为槽找不到而不可用了。当然如果B和B1都down了，那集群还是不可用的，不过这种情况微乎其妙，基本不用考虑，出发你交换机挂了吧，或者机房断电。
4、redis架构图
[](http://s3.51cto.com/wyfs02/M00/4B/72/wKiom1Qsx06wtVlXAAGEwXnBz08131.jpg)
架构细节:
(1)所有的redis节点彼此互联(PING-PONG机制),内部使用二进制协议优化传输速度和带宽.
(2)节点的fail是通过集群中超过半数的节点检测失效时才生效.
(3)客户端与redis节点直连,不需要中间proxy层.客户端不需要连接集群所有节点,连接集群中任何一个可用节点即可
(4)redis-cluster把所有的物理节点映射到[0-16383]slot上,cluster 负责维护node<->slot<->value
5、redis-cluster选举:容错
[](http://s3.51cto.com/wyfs02/M01/4B/74/wKioL1Qsx8uwkQU2AAEcQpIDk1E403.jpg)
(1)领着选举过程是集群中所有master参与,如果半数以上master节点与master节点通信超过(cluster-node-timeout),认为当前master节点挂掉.
(2):什么时候整个集群不可用(cluster_state:fail),当集群不可用时,所有对集群的操作做都不可用，收到((error) CLUSTERDOWN The cluster is down)错误
    a:如果集群任意master挂掉,且当前master没有slave.集群进入fail状态,也可以理解成进群的slot映射[0-16383]不完成时进入fail状态.
    b:如果进群超过半数以上master挂掉，无论是否有slave集群进入fail状态
本文出自 “[周氏一族](http://zhoushouby.blog.51cto.com/)” 博客，谢绝转载！
http://hot66hot.iteye.com/blog/2050676
最近研究redis-cluster,正好搭建了一个环境,遇到了很多坑,系统的总结下,等到redis3 release出来后，换掉memCache 集群. 转载请注明出处哈:http://hot66hot.iteye.com/admin/blogs/2050676
# 一:关于redis cluster
## 1:redis cluster的现状
reids-cluster计划在redis3.0中推出，可以看作者antirez的声明:[http://antirez.com/news/49](http://antirez.com/news/49) (ps:跳票了好久，今年貌似加快速度了),目前的最新版本见:[https://raw.githubusercontent.com/antirez/redis/3.0/00-RELEASENOTES](https://raw.githubusercontent.com/antirez/redis/3.0/00-RELEASENOTES)
作者的目标:Redis Cluster will support up to ~1000 nodes. 赞...
目前redis支持的cluster特性(已测试):
1):节点自动发现
2):slave->master 选举,集群容错
3):Hot resharding:在线分片
4):集群管理:cluster xxx
5):基于配置(nodes-port.conf)的集群管理
6):ASK 转向/MOVED 转向机制.
## 2:redis cluster 架构
### 1)redis-cluster架构图
架构细节:
(1)所有的redis节点彼此互联(PING-PONG机制),内部使用二进制协议优化传输速度和带宽.
(2)节点的fail是通过集群中超过半数的节点检测失效时才生效.
(3)客户端与redis节点直连,不需要中间proxy层.客户端不需要连接集群所有节点,连接集群中任何一个可用节点即可
(4)redis-cluster把所有的物理节点映射到[0-16383]slot上,cluster 负责维护node<->slot<->value
### 2) redis-cluster选举:容错
(1)领着选举过程是集群中所有master参与,如果半数以上master节点与master节点通信超过(cluster-node-timeout),认为当前master节点挂掉.
(2):什么时候整个集群不可用(cluster_state:fail)? 
    a:如果集群任意master挂掉,且当前master没有slave.集群进入fail状态,也可以理解成集群的slot映射[0-16383]不完成时进入fail状态. ps : redis-3.0.0.rc1加入cluster-require-full-coverage参数,默认关闭,打开集群兼容部分失败.
    b:如果集群超过半数以上master挂掉，无论是否有slave集群进入fail状态.
  ps:当集群不可用时,所有对集群的操作做都不可用，收到((error) CLUSTERDOWN The cluster is down)错误
# 二:redis cluster的使用
## 1:安装redis cluster
1):安装redis-cluster依赖:redis-cluster的依赖库在使用时有兼容问题,在reshard时会遇到各种错误,请按指定版本安装.
##### (1)确保系统安装zlib,否则gem install会报(no such file to load -- zlib)
- 
#download:zlib-1.2.6.tar  
- 
./configure  
- 
make  
- 
make install  
#####  (2)安装ruby:version(1.9.2)
- 
# ruby1.9.2   
- 
cd /path/ruby  
- 
./configure -prefix=/usr/local/ruby  
- 
make  
- 
make install  
- 
sudo cp ruby /usr/local/bin  
##### (3)安装rubygem:version(1.8.16)
- 
# rubygems-1.8.16.tgz  
- 
cd /path/gem  
- 
sudo ruby setup.rb  
- 
sudo cp bin/gem /usr/local/bin  
##### (4)安装gem-redis:version(3.0.0)
- 
gem install redis --version 3.0.0  
- 
#由于源的原因，可能下载失败，就手动下载下来安装  
- 
#download地址:http://rubygems.org/gems/redis/versions/3.0.0  
- 
gem install -l /data/soft/redis-3.0.0.gem  
##### (5)安装redis-cluster
- 
cd /path/redis  
- 
make  
- 
sudo cp /opt/redis/src/redis-server /usr/local/bin  
- 
sudo cp /opt/redis/src/redis-cli /usr/local/bin  
- 
sudo cp /opt/redis/src/redis-trib.rb /usr/local/bin  
### 2:配置redis cluster
#### 1)redis配置文件结构:
 使用包含(include)把通用配置和特殊配置分离,方便维护.
#### 2)redis通用配置.
- 
#GENERAL  
- 
daemonize no  
- 
tcp-backlog 511  
- 
timeout 0  
- 
tcp-keepalive 0  
- 
loglevel notice  
- 
databases 16  
- 
dir /opt/redis/data  
- 
slave-serve-stale-data yes  
- 
#slave只读  
- 
slave-read-only yes  
- 
#not use default  
- 
repl-disable-tcp-nodelay yes  
- 
slave-priority 100  
- 
#打开aof持久化  
- 
appendonly yes  
- 
#每秒一次aof写  
- 
appendfsync everysec  
- 
#关闭在aof rewrite的时候对新的写操作进行fsync  
- 
no-appendfsync-on-rewrite yes  
- 
auto-aof-rewrite-min-size 64mb  
- 
lua-time-limit 5000  
- 
#打开redis集群  
- 
cluster-enabled yes  
- 
#节点互连超时的阀值  
- 
cluster-node-timeout 15000  
- 
cluster-migration-barrier 1  
- 
slowlog-log-slower-than 10000  
- 
slowlog-max-len 128  
- 
notify-keyspace-events ""  
- 
hash-max-ziplist-entries 512  
- 
hash-max-ziplist-value 64  
- 
list-max-ziplist-entries 512  
- 
list-max-ziplist-value 64  
- 
set-max-intset-entries 512  
- 
zset-max-ziplist-entries 128  
- 
zset-max-ziplist-value 64  
- 
activerehashing yes  
- 
client-output-buffer-limit normal 0 0 0  
- 
client-output-buffer-limit slave 256mb 64mb 60  
- 
client-output-buffer-limit pubsub 32mb 8mb 60  
- 
hz 10  
- 
aof-rewrite-incremental-fsync yes  
#### 3)redis特殊配置.
- 
#包含通用配置  
- 
include /opt/redis/redis-common.conf  
- 
#监听tcp端口  
- 
port 6379  
- 
#最大可用内存  
- 
maxmemory 100m  
- 
#内存耗尽时采用的淘汰策略:  
- 
# volatile-lru -> remove the key with an expire set using an LRU algorithm  
- 
# allkeys-lru -> remove any key accordingly to the LRU algorithm  
- 
# volatile-random -> remove a random key with an expire set  
- 
# allkeys-random -> remove a random key, any key  
- 
# volatile-ttl -> remove the key with the nearest expire time (minor TTL)  
- 
# noeviction -> don't expire at all, just return an error on write operations  
- 
maxmemory-policy allkeys-lru  
- 
#aof存储文件  
- 
appendfilename "appendonly-6379.aof"  
- 
#不开启rdb存储,只用于添加slave过程  
- 
dbfilename dump-6379.rdb  
- 
#cluster配置文件(启动自动生成)  
- 
cluster-config-file nodes-6379.conf  
- 
#部署在同一机器的redis实例，把auto-aof-rewrite搓开，因为cluster环境下内存占用基本一致.  
- 
#防止同意机器下瞬间fork所有redis进程做aof rewrite,占用大量内存(ps:cluster必须开启aof)  
- 
auto-aof-rewrite-percentage 80-100  
### 3:cluster 操作
cluster集群相关命令,更多redis相关命令见文档:[http://redis.readthedocs.org/en/latest/](http://redis.readthedocs.org/en/latest/)
- 
集群  
- 
CLUSTER INFO 打印集群的信息  
- 
CLUSTER NODES 列出集群当前已知的所有节点（node），以及这些节点的相关信息。  
- 
节点  
- 
CLUSTER MEET <ip> <port> 将 ip 和 port 所指定的节点添加到集群当中，让它成为集群的一份子。  
- 
CLUSTER FORGET <node_id> 从集群中移除 node_id 指定的节点。  
- 
CLUSTER REPLICATE <node_id> 将当前节点设置为 node_id 指定的节点的从节点。  
- 
CLUSTER SAVECONFIG 将节点的配置文件保存到硬盘里面。  
- 
槽(slot)  
- 
CLUSTER ADDSLOTS <slot> [slot ...] 将一个或多个槽（slot）指派（assign）给当前节点。  
- 
CLUSTER DELSLOTS <slot> [slot ...] 移除一个或多个槽对当前节点的指派。  
- 
CLUSTER FLUSHSLOTS 移除指派给当前节点的所有槽，让当前节点变成一个没有指派任何槽的节点。  
- 
CLUSTER SETSLOT <slot> NODE <node_id> 将槽 slot 指派给 node_id 指定的节点，如果槽已经指派给另一个节点，那么先让另一个节点删除该槽>，然后再进行指派。  
- 
CLUSTER SETSLOT <slot> MIGRATING <node_id> 将本节点的槽 slot 迁移到 node_id 指定的节点中。  
- 
CLUSTER SETSLOT <slot> IMPORTING <node_id> 从 node_id 指定的节点中导入槽 slot 到本节点。  
- 
CLUSTER SETSLOT <slot> STABLE 取消对槽 slot 的导入（import）或者迁移（migrate）。  
- 
键  
- 
CLUSTER KEYSLOT <key> 计算键 key 应该被放置在哪个槽上。  
- 
CLUSTER COUNTKEYSINSLOT <slot> 返回槽 slot 目前包含的键值对数量。  
- 
CLUSTER GETKEYSINSLOT <slot> <count> 返回 count 个 slot 槽中的键。  
### 4:redis cluster 运维操作
### 1)初始化并构建集群
(1)启动集群相关节点（必须是空节点,beta3后可以是有数据的节点）,指定配置文件和输出日志
- 
redis-server /opt/redis/conf/redis-6380.conf > /opt/redis/logs/redis-6380.log 2>&1 &  
- 
redis-server /opt/redis/conf/redis-6381.conf > /opt/redis/logs/redis-6381.log 2>&1 &  
- 
redis-server /opt/redis/conf/redis-6382.conf > /opt/redis/logs/redis-6382.log 2>&1 &  
- 
redis-server /opt/redis/conf/redis-7380.conf > /opt/redis/logs/redis-7380.log 2>&1 &  
- 
redis-server /opt/redis/conf/redis-7381.conf > /opt/redis/logs/redis-7381.log 2>&1 &  
- 
redis-server /opt/redis/conf/redis-7382.conf > /opt/redis/logs/redis-7382.log 2>&1 &  
(2):使用自带的ruby工具(redis-trib.rb)构建集群
- 
#redis-trib.rb的create子命令构建  
- 
#--replicas 则指定了为Redis Cluster中的每个Master节点配备几个Slave节点  
- 
#节点角色由顺序决定,先master之后是slave(为方便辨认,slave的端口比master大1000)  
- 
redis-trib.rb create --replicas 1 10.10.34.14:6380 10.10.34.14:6381 10.10.34.14:6382 10.10.34.14:7380 10.10.34.14:7381 10.10.34.14:7382  
(3):检查集群状态
- 
#redis-trib.rb的check子命令构建  
- 
#ip:port可以是集群的任意节点  
- 
redis-trib.rb check 10.10.34.14:6380  
 最后输出如下信息,没有任何警告或错误，表示集群启动成功并处于ok状态
- 
[OK] All nodes agree about slots configuration.  
- 
>>> Check for open slots...  
- 
>>> Check slots coverage...  
- 
[OK] All 16384 slots covered.  
#### 2):添加新master节点
(1)添加一个master节点:创建一个空节点（empty node），然后将某些slot移动到这个空节点上,这个过程目前需要人工干预
a):根据端口生成配置文件(ps:establish_config.sh是我自己写的输出配置脚本)
- 
sh establish_config.sh 6386 > conf/redis-6386.conf  
b):启动节点
- 
redis-server /opt/redis/conf/redis-6386.conf > /opt/redis/logs/redis-6386.log 2>&1 &  
c):加入空节点到集群
add-node  将一个节点添加到集群里面， 第一个是新节点ip:port, 第二个是任意一个已存在节点ip:port
- 
redis-trib.rb add-node 10.10.34.14:6386 10.10.34.14:6381  
node:新节点没有包含任何数据， 因为它没有包含任何slot。新加入的加点是一个主节点， 当集群需要将某个从节点升级为新的主节点时， 这个新节点不会被选中
d):为新节点分配slot
- 
redis-trib.rb reshard 10.10.34.14:6386  
- 
#根据提示选择要迁移的slot数量(ps:这里选择500)  
- 
How many slots do you want to move (from 1 to 16384)? 500  
- 
#选择要接受这些slot的node-id  
- 
What is the receiving node ID? f51e26b5d5ff74f85341f06f28f125b7254e61bf  
- 
#选择slot来源:  
- 
#all表示从所有的master重新分配，  
- 
#或者数据要提取slot的master节点id,最后用done结束  
- 
Please enter all the source node IDs.  
- 
  Type 'all' to use all the nodes as source nodes for the hash slots.  
- 
  Type 'done' once you entered all the source nodes IDs.  
- 
Source node #1:all  
- 
#打印被移动的slot后，输入yes开始移动slot以及对应的数据.  
- 
#Do you want to proceed with the proposed reshard plan (yes/no)? yes  
- 
#结束  
#### 3):添加新的slave节点
a):前三步操作同添加master一样
b)第四步:redis-cli连接上新节点shell,输入命令:cluster replicate 对应master的node-id
- 
cluster replicate 2b9ebcbd627ff0fd7a7bbcc5332fb09e72788835  
note:在线添加slave 时，需要dump整个master进程，并传递到slave，再由 slave加载rdb文件到内存，rdb传输过程中Master可能无法提供服务,整个过程消耗大量io,小心操作.
例如本次添加slave操作产生的rdb文件
- 
-rw-r--r-- 1 root root  34946 Apr 17 18:23 dump-6386.rdb  
- 
-rw-r--r-- 1 root root  34946 Apr 17 18:23 dump-7386.rdb  
### 4):在线reshard 数据:
对于负载/数据不均匀的情况，可以在线reshard slot来解决,方法与添加新master的reshard一样，只是需要reshard的master节点是老节点.
### 5):删除一个slave节点
- 
#redis-trib del-node ip:port '<node-id>'  
- 
redis-trib.rb del-node 10.10.34.14:7386 'c7ee2fca17cb79fe3c9822ced1d4f6c5e169e378'  
###  6):删除一个master节点
  a):删除master节点之前首先要使用reshard移除master的全部slot,然后再删除当前节点(目前只能把被删除
master的slot迁移到一个节点上)
- 
#把10.10.34.14:6386当前master迁移到10.10.34.14:6380上  
- 
redis-trib.rb reshard 10.10.34.14:6380  
- 
#根据提示选择要迁移的slot数量(ps:这里选择500)  
- 
How many slots do you want to move (from 1 to 16384)? 500(被删除master的所有slot数量)  
- 
#选择要接受这些slot的node-id(10.10.34.14:6380)  
- 
What is the receiving node ID? c4a31c852f81686f6ed8bcd6d1b13accdc947fd2 (ps:10.10.34.14:6380的node-id)  
- 
Please enter all the source node IDs.  
- 
  Type 'all' to use all the nodes as source nodes for the hash slots.  
- 
  Type 'done' once you entered all the source nodes IDs.  
- 
Source node #1:f51e26b5d5ff74f85341f06f28f125b7254e61bf(被删除master的node-id)  
- 
Source node #2:done  
- 
#打印被移动的slot后，输入yes开始移动slot以及对应的数据.  
- 
#Do you want to proceed with the proposed reshard plan (yes/no)? yes  
b):删除空master节点
- 
redis-trib.rb del-node 10.10.34.14:6386 'f51e26b5d5ff74f85341f06f28f125b7254e61bf'  
三:redis cluster 客户端(Jedis)
## 1:客户端基本操作使用
- 
<span style="color: #333333; font-family: Arial, sans-serif;"> private static BinaryJedisCluster jc;  
- static {  
- //只给集群里一个实例就可以  
- 
        Set<HostAndPort> jedisClusterNodes = new HashSet<HostAndPort>();  
- 
        jedisClusterNodes.add(new HostAndPort("10.10.34.14", 6380));  
- 
        jedisClusterNodes.add(new HostAndPort("10.10.34.14", 6381));  
- 
        jedisClusterNodes.add(new HostAndPort("10.10.34.14", 6382));  
- 
        jedisClusterNodes.add(new HostAndPort("10.10.34.14", 6383));  
- 
        jedisClusterNodes.add(new HostAndPort("10.10.34.14", 6384));  
- 
        jedisClusterNodes.add(new HostAndPort("10.10.34.14", 7380));  
- 
        jedisClusterNodes.add(new HostAndPort("10.10.34.14", 7381));  
- 
        jedisClusterNodes.add(new HostAndPort("10.10.34.14", 7382));  
- 
        jedisClusterNodes.add(new HostAndPort("10.10.34.14", 7383));  
- 
        jedisClusterNodes.add(new HostAndPort("10.10.34.14", 7384));  
- 
        jc = new BinaryJedisCluster(jedisClusterNodes);  
- 
    }  
- @Test  
- public void testBenchRedisSet() throws Exception {  
- final Stopwatch stopwatch = new Stopwatch();  
- 
        List list = buildBlogVideos();  
- for (int i = 0; i < 1000; i++) {  
- 
            String key = "key:" + i;  
- 
            stopwatch.start();  
- byte[] bytes1 = protostuffSerializer.serialize(list);  
- 
            jc.setex(key, 60 * 60, bytes1);  
- 
            stopwatch.stop();  
- 
        }  
- 
        System.out.println("time=" + stopwatch.toString());  
- 
    }</span>  
## 2:jedis客户端的坑.
1)cluster环境下redis的slave不接受任何读写操作，
2)client端不支持keys批量操作,不支持select dbNum操作，只有一个db:select 0
3)JedisCluster 的info()等单机函数无法调用,返回(No way to dispatch this command to Redis Cluster)错误，.
4)JedisCluster 没有针对byte[]的API，需要自己扩展(附件是我加的基于byte[]的BinaryJedisCluster  api)
参考文档:
[http://redis.io/topics/cluster-spec](http://redis.io/topics/cluster-spec)
[http://redis.io/topics/cluster-tutorial](http://redis.io/topics/cluster-tutorial)
 http://blog.sina.com.cn/s/blog_48c95a190101dhe9.html
 Redis Cluster即Redis的分布式版本，将是Redis继支持Lua脚本之后的又一重磅功能，官方声明将会在今年第三季度发布Redis Cluster的beta版并在年底发布第一个稳定版本。当前，虽然Redis的稳定版本里还没有集成分布式功能，但实际上在开发版中Redis Cluster的开发已经取得了长足的进展，我们已经可以搭建Redis集群并使用其部分功能了。今天，本博主基于最新的开发版代码尝试着搭建了一个三节点的Redis集群，这里不妨把过程简单总结一下，希望能对各位看客们有所帮助！（注：本篇博文中的搭建方法是彻彻底底的野路子，Redis官方已正式发布Redis
 Cluster搭建方法，链接如下：http://redis.io/topics/cluster-tutorial。）
    首先，下载最新的Redis开发版源码包。这个再简单不过了，大家既可以去Redis在github上的主页下载，也可以直接运行“git clone git://github.com/antirez/redis.git”克隆整个Redis代码库，当然前提是你已经安装了git。
    其次，安装Redis。因为本博主要搭建一个三节点的Redis集群，所以在这三个节点上都要安装好Redis，我们这里姑且将这三个节点命名为Redis Cluster Node1/Node2/Node3吧，安装目录为/usr/local/redis/。
    再次，修改Redis配置文件。因为即使是在支持分布式功能的开发版中，Redis配置文件也是默认不打开Redis Cluster的，所以在启动Redis服务器之前，我们需要修改Redis配置文件，打开Redis Cluster。为了简单起见，我们这里只修改与Redis Cluster相关的配置，其他无关的配置均采用默认值，具体修改内容如下：
################################ REDIS CLUSTER  ###############################
#
# Normal Redis instances can't be part of a Redis Cluster; only nodes that are
# started as cluster nodes can. In order to start a Redis instance as a
# cluster node enable the cluster support uncommenting the following:
#
cluster-enabled yes
# Every cluster node has a cluster configuration file. This file is not
# intended to be edited by hand. It is created and updated by Redis nodes.
# Every Redis Cluster node requires a different cluster configuration file.
# Make sure that instances running in the same system does not have
# overlapping cluster configuration file names.
#
cluster-config-file nodes-6379.conf
# Cluster node timeout is the amount of seconds a node must be unreachable
# for it to be considered in failure state.
# Most other internal time limits are multiplicators of the node timeout.
#
cluster-node-timeout 15
# In order to setup your cluster make sure to read the documentation
# available at http://redis.io web site.  
    再次，启动三个节点上的Redis服务器。此时，三个Redis服务器节点均会以Redis Cluster的方式开始运行，但并没有自动构建集群，因为三者还处于“我不认识你，你不属于我”的状态，它们每个都是孤零零的Redis节点，或者是只包含了一个节点的集群。我们可以通过Redis客户端连接到服务器查看它们的状态，图一给出了状态查询方法和查询结果，其中cluster nodes命令用于查看当前Redis节点所属的Redis集群中的所有节点，而cluster
 info则用于查看当前Redis节点所属的Redis集群的整体状态。由图中我们可以看到，Redis集群中仅包含一个Redis节点，也就是当前节点，整个集群的状态是fail。
[](http://photo.blog.sina.com.cn/showpic.html#blogid=48c95a190101dhe9&url=http://s6.sinaimg.cn/orignal/48c95a19gddd7ec6fef25)
图一 未形成集群时Redis节点状态
    再次，搭建Redis集群。这里所谓的搭建集群，说白了就是让之前启动的三个Redis节点彼此连通，意识到彼此的存在，那么如何做到这一点呢？答案就是cluster meet命令。该命令的作用就是将当前节点主动介绍给另外一个节点认识，图二给出了cluster meet命令的执行方法和执行效果，由图中可知我们使用cluster meet命令分别将Redis Cluster Node1介绍给了Redis
 Cluster Node2（节点IP地址为192.168.32.3，运行端口为6379）和Redis Cluster Node3（节点IP地址为192.168.32.4，运行端口为6379），之后我们再次查看集群节点和集群状态就可以知道，三个节点已经成功合并到了同一个集群中。
[](http://photo.blog.sina.com.cn/showpic.html#blogid=48c95a190101dhe9&url=http://s1.sinaimg.cn/orignal/48c95a19gddd81d4658b0)
图二 搭建Redis集群
    再次，为集群中的Redis节点分配hash slot。通过上面的操作，我们已经将三个各自为政的Redis节点规划到一个相同的集群中，那么我们现在是否就已经完成了集群搭建的所有工作了呢？非也！通过图二中对集群状态的查看我们可以知道，当前集群的状态还是fail，此时的Redis集群是不工作的，无法处理任何Redis命令。那么集群的状态为什么还是fail呢？本博主通过查看官方文档说明找到了原因所在，现摘录原文如下：
The FAIL state for the cluster happens in two cases.
1) If at least one hash slot is not served as the node serving it currently is in FAIL state.
2) If we are not able to reach the majority of masters (that is, if the majorify of masters are simply in PFAIL state, it is enough for the node to enter FAIL mode).
    很明显，导致我们的集群处于fail状态的原因不是第二个条，也就是说至少有一个hash slot没有被服务！稍微考虑一下，可不是！何止有一个hash slot没有被服务，压根儿就没有Redis节点为任何hash slot服务！众所周知，Redis Cluster通过hash slot将数据根据主键来分区，所以一条key-value数据会根据算法自动映射到一个hash slot，但是一个hash
 slot存储在哪个Redis节点上并不是自动映射的，是需要集群管理者自行分配的。那么我们需要为多少个hash slot分配Redis节点呢？根据源码可知是16384个，即我们要将16384个hash slot分配到集群内的三个节点上。Redis中用于分配hash slot的命令有很多，其中包括cluster addslots、cluster delslots和cluster setslot。鉴于我们现在是集群的初始化阶段，所以我们可以选择cluster addslots来分配hash slot，该命令的语法为cluster addslots slot1
 [slot2] ... [slotN]。
[](http://photo.blog.sina.com.cn/showpic.html#blogid=48c95a190101dhe9&url=http://s4.sinaimg.cn/orignal/48c95a19gddd922b292f3)
图三 Redis Cluster Node1上nodes-6379.conf的内容
    但是另一个问题又出现了，对于16384个hash slot来说，我们总不能用cluster addslots一个个去分配吧？幸运的是，我们还有另外一种方法来分配hash slot，那就是通过集群配置文件，也就是我们在第三步中配置的cluster-config-file参数，来完成hash slot的分段配置。此时，我们在执行Redis服务器启动的目录下找到名字为nodes-6379.conf的配置文件。图三给出了Redis
 Cluster Node1上nodes-6379.conf的内容，由图可知该文件中的内容与我们执行cluster nodes命令得到的结果一样，就是对集群中所有Redis节点的描述，而我们需要修改的就是为每个节点添加上hash slot的分配信息。针对Redis Cluster Node1上nodes-6379.conf的内容，修改内容如下：cda76a0a094d2ce624e33bed7f3c75689a4128fd :0 myself,master - 0 0 connected 0-5000（注意是在自身节点的描述，也就是包含了myself那一行的后面追加hash
 slot的范围）。类似的，Redis Cluster Node2上nodes-6379.conf文件中追加5001-10000，Redis Cluster Node3上nodes-6379.conf文件中追加10001-16383。经过这样的配置后，Redis
 Cluster Node1负责存储0至5000之间的所有hash slots，Redis Cluster Node2负责存储5001至10000之间的所有hash slots，Redis Cluster Node3负责存储10001至16383的所有hash slots。
    保存以上修改，通过Redis客户端将三个Redis服务器重启，再次执行命令cluster nodes和cluster info，我们就可以看到集群的状态已经为ok了（如图四所示），这说明之前对于集群配置文件的修改确实起到了作用。如果经过以上步骤之后集群状态依然是fail的话，可以重新修改一下三个节点的nodes-6379.conf，将其中所有除myself那一行以外的所有配置行都删除，然后再保存重启服务器即可。当然，这里需要说明的一点是，本博主并没有看到任何关于Redis
 Cluster集群配置文件的官方文档，以上配置方法尽管有效但并不一定是官方推荐的做法！
[](http://photo.blog.sina.com.cn/showpic.html#blogid=48c95a190101dhe9&url=http://s14.sinaimg.cn/orignal/48c95a19gddd9bbc3e8fd)
图四 修改集群配置文件后重启的集群状态
    最后，执行Redis命令。在集群状态显示为ok之后，我们就可以像在Redis单机版上一样执行Redis命令了。图五显示了Redis客户端连接到Redis Cluster Node1上执行“set foo bar”命令的执行结果，由图可知主键foo所属的hash slot存储在Redis Cluster Node3上。我们再连接到Redis Cluster Node3，执行相同的命令，正确执行！
[](http://photo.blog.sina.com.cn/showpic.html#blogid=48c95a190101dhe9&url=http://s12.sinaimg.cn/orignal/48c95a19gddd9f096dddb)
图五 执行Redis命令
参考链接：[http://redis.io/topics/cluster-spec](http://redis.io/topics/cluster-spec)
http://blog.sina.com.cn/s/blog_48c95a190101ig6a.html
去年本博主发表了一篇关于Redis Cluster搭建的博文，因为当时没有找到任何官方的说明文档，所以只能根据Redis主页上的Redis cluster Specification一步步地摸索出了一种搭建方法，可以说是彻彻底底的野路子。后来自己怕误人子弟，还很自觉地在原文中对这一点作了说明。后来某位网友发来一篇博文的链接：http://no-fucking-idea.com/blog/2012/04/16/setting-up-redis-cluster/，其中就是介绍如何使用redis源码包中的redis-trib.rb工具来实现Redis
 Cluster的搭建，不仅实现方法异常简单而且发表时间也早了一年有余，由此可知自己的那篇博文确实是闭门造车了。最近Redis官网正式推出了支持Redis Cluster的3.0 Beta版，我在官网上也找到了关于Redis Cluster搭建的tutorial，今天按照上面的步骤尝试了一把，果真是极好的！于是乎赶紧发一篇博文，希望已经被俺前一篇博文毒害的弟兄们早日悬崖勒马，回归正道！
    首先，下载支持Redis Cluster的源码包，最方便的当然就是直接下载3.0 Beta版，其链接地址为：https://github.com/antirez/redis/archive/3.0.0-beta1.tar.gz。解压安装后，对Redis进行配置，主要就是将cluster功能打开，与Cluster相关的具体配置如下所示。
cluster-enabled yes
cluster-config-file /home/wqd/work/conf/nodes-6379.conf
cluster-node-timeout 5000
    此外，这次通过运行在同一台机器上的三个不同Redis实例来搭建Redis Cluster，所以需要准备三个配置文件，每个配置文件中设置不同的端口。这里姑且将这三个配置文件分别命名为redis-6379.conf、redis-6380.conf和redis-6381.conf，其中的配置也是一样，像pid文件、日志文件、rdb文件、aof文件和集群配置文件等都是通过端口号来区分。在完成对配置文件的修改后，分别启动三个Redis实例，结果如图一所示。
[](http://photo.blog.sina.com.cn/showpic.html#blogid=48c95a190101ig6a&url=http://album.sina.com.cn/pic/001kDQi5gy6H16GnEhCcc)
图一 启动三个Redis实例
    其次，通过redis-trib.rb工具构建Redis Cluster。通过以上操作我们已经有了三个独立运行的Redis实例，彼此之间各自为政，接下来就是redis-trib.rb工具发挥作用的时候了。在执行该工具之前，一些准备工作是必不可少的，其一就是安装ruby和rubygem，为了方便起见这里推荐安装ruby 1.9.2及之后的版本，这些版本已经包含了rubygem，无需单独安装，安装方法这里就不赘述了。在完成ruby和rubygem的安装之后，我们还需要为ruby安装redis库，安装命令为：gem
 install redis。在做完了以上这些准备工作后，我们就可以执行redis-trib.rb这个工具了，图二给出了该工具在没有任何参数下的执行结果，其中我们可以了解该工具支持的所有功能，这里就不一一介绍了，官方的tutorial中有详细的介绍。
[](http://photo.blog.sina.com.cn/showpic.html#blogid=48c95a190101ig6a&url=http://album.sina.com.cn/pic/001kDQi5gy6H16JJ57j41)
图二 redis-trib.rb工具使用方法
    再次，基于三个实例构建Redis Cluster。上面我们已经看到了redis-trib.rb支持的所有子命令了，而其中用于构建Redis Cluster的子命令就是create。create子命令的参数有两种，host1:port1 ... hostN:portN指定了用于构建Redis Cluster的所有redis实例，而--replicas 则指定了为Redis Cluster中的每个Master节点配备几个Slave节点。关于后一个参数这里需要简单说明一下，那就是Redis
 Cluster中的节点分为两种：Master节点和Slave节点，一个Master节点可以拥有若干个Slave节点，Master节点上的数据通过异步方式与Slave节点实现数据同步，当Master节点因为某种原因退出集群后，Redis Cluster会自动从该Master节点的Slave节点中选择出一个作为新的Master节点。因此，redis-trib.rb工具的create子命令提供了--args参数来指定集群中的Master节点拥有几个Slave节点，譬如使用6个redis实例构建集群且--args参数值为1，那么整个集群就包含三个Master节点和三个Slave节点，每个Master节点都有一个Slave节点。这里我们只有三个redis实例，所以选择不为Master节点配备Slave节点，创建集群的方法和结果如图三所示，从中可知Redis集群已经构建成功，其中监听6379的实例负责存储0-5460哈希槽，监听6380的实例负责存储5461-10921哈希槽，监听6381的实例负责存储10922-16383哈希槽。
[](http://photo.blog.sina.com.cn/showpic.html#blogid=48c95a190101ig6a&url=http://album.sina.com.cn/pic/001kDQi5gy6H16KTSbNdb)
图三 通过redis-trib.rb创建Redis Cluster
    如果忘记了Redis Cluster的搭建环境，如集群中有几个节点，这些节点分别负责存储哪些哈希槽，我们可以通过redis-trib.rb工具的check子命令来查看集群信息，该子命令只需要提供集群中任意redis实例的IP地址和端口号即可，图四给出了check子命令的执行方法和结果。
[](http://photo.blog.sina.com.cn/showpic.html#blogid=48c95a190101ig6a&url=http://album.sina.com.cn/pic/001kDQi5gy6H16N2Nzt5b)
图四 通过redis-trib.rb查看Redis Cluster
    最后，通过redis客户端实现对Redis Cluster的读写。当前，redis客户端同样实现了对集群的支持，但使用方法略有不同，即在启动的时候需要添加一个-c参数。图五给出了redis客户端的使用方法和执行结果，从中可以看到当读写Key-Value数据所属的哈希槽存储在别的节点上时，redis客户端会将数据自动重定向到目标节点上，而不是像之前那样（即不带-c参数）返回错误并给出目标节点了事。
[](http://photo.blog.sina.com.cn/showpic.html#blogid=48c95a190101ig6a&url=http://album.sina.com.cn/pic/001kDQi5gy6H18grNzs42)
图五 通过redis客户端实现对Redis Cluster读写
    以上我们简单介绍了通过redis-trib.rb工具构建Redis Cluster的方法与步骤，关于redis-trib.rb工具实际上还有很多实用的功能没有介绍，像reshard、add-node、del-node等等，感兴趣地话推荐大家还是去阅读官方的tutorial，其中有详细的说明和示例。
参考链接：http://no-fucking-idea.com/blog/2012/04/16/setting-up-redis-cluster/
http://redis.io/topics/cluster-tutorial
http://blog.nosqlfan.com/html/3302.html
## Redis集群功能说明
作者：[nosqlfan](http://blog.nosqlfan.com/html/author/nosqlfan/) on 星期一, 十月 24, 2011 · [2条评论](http://blog.nosqlfan.com/html/3302.html#comments) 【阅读：14,306
 次】
虽然目前可以通过在客户端做hash的方法来构建[Redis](http://blog.nosqlfan.com/tags/redis)[集群](http://blog.nosqlfan.com/tags/%e9%9b%86%e7%be%a4)，但Redis原生的集群支持还是颇受期待。本文是对Redis集群功能官方描述[文档](http://redis.io/topics/cluster-spec)的一个翻译，译者是@[PPS萝卜](http://weibo.com/tsredis)同学，也感谢他的投稿分享。
> 
### 介绍
这篇文档主要是为了说明正在进展中的Redis集群功能。文档主要分为两个部分，前一部分主要介绍我在非稳定分支已完成的代码，后一部分主要介绍还有哪些功能待实现。
本文档所有的说明都有可能在将来由于设计原因而进行更改，而未实现的计划比已实现的功能更有可能会被更改。
本文档包含了所有client library所需要的细节，但是client library的作者们需要提前意识到真正实现的细节在将来很有可能会有变化。
### 什么是Redis集群？
Redis集群是一个实现分布式并且允许单点故障的Redis高级版本。
Redis集群没有最重要或者说中心节点，这个版本最主要的一个目标是设计一个线性可伸缩（可随意增删节点？）的功能。
Redis集群为了数据的一致性可能牺牲部分允许单点故障的功能，所以当网络故障和节点发生故障时这个系统会尽力去保证数据的一致性和有效性。（这里我们认为节点故障是网络故障的一种特殊情况）
为了解决单点故障的问题，我们同时需要masters 和 slaves。 即使主节点(master)和从节点(slave)在功能上是一致的，甚至说他们部署在同一台服务器上，从节点也仅用以替代故障的主节点。 实际上应该说 如果对从节点没有read-after-write（写并立即读取数据 以免在数据同步过程中无法获取数据）的需求，那么从节点仅接受只读操作。
### 已实现子集
Redis集群会把所有的单一key存储在非分布式版本的Redis中。对于复合操作比如求并集求交集之类则未实现。
在将来，有可能会增加一种为“Computation Node”的新类型节点。这种节点主要用来处理在集群中multi-key的只读操作，但是对于multi-key的只读操作不会以集群传输到Computation Node节点再进行计算的方式实现。
Redis集群版本将不再像独立版本一样支持多数据库，在集群版本中只有database 0，并且SELECT命令是不可用的。
### 客户端与服务端在Redis集群版中的约定
在Redis集群版本中，节点有责任/义务保存数据和自身状态，这其中包括把数据（key）映射到正确的节点。所有节点都应该自动探测集群中的其他节点，并且在发现故障节点之后把故障节点的从节点更改为主节点（原文这里有“如果有需要” 可能是指需要设置或者说存在从节点）。
集群节点使用TCP bus和二进制协议进行互联并对任务进行分派。各节点使用gossip 协议发送ping packets给集群其他节点以确定其他节点是否正常工作。[cluster](http://blog.nosqlfan.com/tags/cluster) bus也可以用来在节点间执行PUB/SUB命令。
当发现集群节点无应答的时候则会使用redirections errors -MOVED and -ASK命令并且会重定向至可用节点。理论上客户端可随意向集群中任意节点发送请求并获得重定向，也就是说客户端实际上并不用关心集群的状态。然而，客户端也可以缓存数据对应的节点这样可以免去服务端进行重定向的工作，这在一定程度上可以提高效率。
### Keys分配模式
一个集群可以包含最多4096个节点（但是我们建议最多设置几百个节点）。
所有的主节点会控制4096个key空间的百分比。当集群稳定之后，也就是说不会再更改集群配置（更改配置指的增删节点），那么一个节点将只为一个hash slot服务。（但是服务节点（主节点）可以拥有多个从节点用来防止单点故障）
用来计算key属于哪个hash slot的算法如下：
HASH_SLOT = CRC16(key) mod 4096
Name: XMODEM (also known as ZMODEM or CRC-16/ACORN)
Width: 16 bit
Poly: 1021 (That is actually x^16 + x^12 + x^5 + 1)
Initialization: 0000
Reflect Input byte: False
Reflect Output CRC: False
Xor constant to output CRC: 0000
Output for "123456789": 31C3
这里我们会取CRC16后的12个字节。在我们的测试中，对于4096个slots， CRC16算法最合适。
### 集群节点特性
在集群中每个节点都拥有唯一的名字。节点名为16进制的160 bit随机数，当节点获取到名字后将被立即启用。节点名将被永久保存到节点设置文件中，除非系统管理员手动删除节点配置文件。
节点名是集群中每个节点的身份证明。在不更改节点ID的情况下是允许修改节点IP和地址的。cluster bus会自动通过gossip协议获取更改后的节点设置。
每个节点可获知其他节点的信息包括：
- IP 端口
- 状态
- 管理的hash slots
- cluster bus最后发送PING的时间
- 最后接收到PONG的时间
- 从节点数量
- 节点ID
无论是主节点还是从节点都可以通过CLUSTER NODES命令来获取以上信息
示例如下：
$ redis-cli cluster nodes
d1861060fe6a534d42d8a19aeb36600e18785e04 :0 myself - 0 1318428930 connected 0-1364
3886e65cc906bfd9b1f7e7bde468726a052d1dae 127.0.0.1:6380 master - 1318428930 1318428931 connected 1365-2729
d289c575dcbc4bdd2931585fd4339089e461a27d 127.0.0.1:6381 master - 1318428931 1318428931 connected 2730-4095
### 节点交互
所有节点总是允许接受来自cluster bus的连接请求，并且即使请求PING的节点是不可信的也会进行应答。然而，所有来自非集群节点的packets都会被忽略。
只有以下两种情况节点才会把其他节点认为是集群的一部分：
如果一个节点使用 MEET message 介绍自己。MEET message 命令是强制其他节点把自己当成是集群的一部分。只有系统管理员使用 CLUSTER MEET ip port 命令节点才会发送MEET message给其他节点。
另外一种方式就是通过集群节点间的推荐机制。例如 如果A节点知道B节点属于集群，而B知道C节点属于集群，那么B将会发送gossip信息告知A：C是属于集群的。当A获得gossip信息之后就会尝试去连接C。
这意味着，当我们以任意连接方式为集群加入一个节点，集群中所有节点都会自动对新节点建立信任连接。也就是说，集群具备自动识别所有节点的功能，但是这仅发生在当系统管理强制为新节点与集群中任意节点建立信任连接的前提下。
这个机制使得集群系统更加健壮。
当一个节点故障时，其余节点会尝试连接其他所有已知的节点已确定其他节点的健壮性。
### 被移动数据的重定向
Redis客户端被允许向集群中的任意节点发送命令，其中包括从节点。被访问的节点将会分析命令中所需要的数据（这里仅指请求单个key），并自己通过判断hash slot确定数据存储于哪个节点。
如果被请求节点拥有hash slot数据（这里指请求数据未被迁移过 或者 hash slot在数据迁移后被重新计算过），则会直接返回结果，否则将会返回一个 MOVED 错误。
MOVED 错误如下：
GET x
-MOVED 3999 127.0.0.1:6381
这个错误包括请求的数据所处的 hash slot（3999） 和 数据目前所属的IP:端口。客户端需要通过访问返回的IP:端口获取数据。即使在客户端请求并等待数据返回的过程中，集群配置已被更改（也就是说请求的key在配置文件中所属的节点ID已被重定向至新的IP:端口），目标节点依然会返回一个MOVED错误。
所以虽然在集群中的节点使用节点ID来确定身份，但是map依然是靠hash slot和Redis节点的IP:端口来进行配对。
客户端虽然不被要求但是应该尝试去记住hash slot 3999现在已被转移至127.0.0.1:6381。这样的话，当一个新的命令需要从hash slot 3999获取数据时就可以有更高的几率从hash slot获取到正确的目标节点。
假设集群已经足够的稳定（不增删节点），那么所有的客户端将会拥有一份hash slots对应节点的数据，这可以使整个集群更高效，因为这样每个命令都会直接定向到正确的节点，不需要通过节点寻找节点或者重新计算hash slot对应的节点。
### 集群不下线更新配置
Redis集群支持线上增删节点。实际上对于系统来说，增加和删除节点在本质上是一样的，因为他们都是把hash slot从一个节点迁移至另外一个节点而已。
增加节点：集群中加入一个空节点并且把hash slot从已存在的节点们移至新节点。
删除节点：集群删除一个已存在节点并且把hash slot分散到已存在的其他节点中。
所以实现这个功能的核心就是迁移slots。实际上从某种观点上来说，hash slot只不过是一堆key的合集，所以Redis集群要做的事情只是在重分片的时候把一堆key从一个实例移动到另外一个实例。
为了清楚的了解这是如何实现的，我们需要先了解一下CLUSTER用来控制slots传输的底层命令。
这些底层命令包括：
CLUSTER ADDSLOTS slot1 [slot2] ... [slotN]
CLUSTER DELSLOTS slot1 [slot2] ... [slotN]
CLUSTER SETSLOT slot NODE node
CLUSTER SETSLOT slot MIGRATING node
CLUSTER SETSLOT slot IMPORTING node
前两个命令 ADDSLOTS 和 DELSLOTS 是用来在Redis节点上增加/删除slots。当hash slots被赋值之后他们会通过gossip协议在整个集群进行广播（例如：大喊一声 兄弟们 我现在住在X节点 有需要我的以后请到X节点来找我）。当slots被添加，ADDSLOTS 命令是用来通知集群其余所有节点最高效的方法。
SETSLOT 命令是用来给把slot注册给一个特殊的node ID（也就是说ADDSLOTS 和 DELSLOTS 对slots进行操作是不指定节点的 而SETSLOT 是会指定节点的）。另外 SETSLOT 还包含两个特殊的状态 MIGRATING 和 IMPORTING：
当一个slot是以 MIGRATING 状态进行设置，那么目标节点将在确认key存在的前提下接受这个hash slot的所有请求，否则查询会被使用 -ASK 重定向至源节点。
当一个slot是以 IMPORTING 状态进行设置，那么目标节点只接受被设置过ASKING命令的所有请求，否则查询将会通过 -MOVED错误重定向至真正的hash slot所有者。
（MIGRATING 和 IMPORTING 我自己也没太看懂 所以这里不敢保证翻译的没有问题）
当你第一次看到以上内容的时候或许会感到困惑，不过没关系，现在我们来把思路理清楚。假设我们有2个Redis节点，一个叫A，另一个叫B。现在我们希望把hash slot8 从A移动到B，那么我们执行的命令应该如下：
We send B: CLUSTER SETSLOT 8 IMPORTING A
We send A: CLUSTER SETSLOT 8 MIGRATING B
所有来自客户端对hash slot8的查询每次都会被导向至节点A，实际过程如下：
所有对A节点存在的数据查询会由A节点来处理
所有对A节点不存在的数据查询会由B节点来处理
我们会发现我们将会无法在A节点创建任何新的数据，因为会被导向B节点。为了解决这个问题，我们设计了一个叫redis-trib的特殊客户端来保证把A节点所有存在的key迁移至B节点。
我们用以下命令来处理：
CLUSTER GETKEYSINSLOT slot count
上面的命令将会返回hash slot中 count keys。对每一个key，redis-trib都会给A节点发送一个 MIGRATE 命令，这个命令会以一种原子的方式把key从A迁移到B（两个节点在迁移过程中都会被锁定）。
以下展示 MIGRATE 如何工作：
MIGRATE target_host target_port key target_database id timeout
MIGRATE 命令会先连接目标节点，并把目标key序列化后进行传输，当源节点收到OK返回值后会删除源节点上的key删除。所以从这个观点上来看，一个key只能存在A或者B而不会同时存在与A和B。
### ASK 重定向
在之前的章节我们说了一下ASK重定向，为什么我们不能只是简单的使用 MOVED 重定向？因为如果使用MOVED命令则有可能会为一个key轮询集群中所有的节点，而ASK命令只询问下一个节点。
ASK是必要的因为在对于hash slot8的下一次查询命令依然是发送给A节点，我们希望客户端先尝试在A节点找数据然后在获取不到的情况下再向B节点请求数据。
然后我们真正的需求是客户端在向A节点请求数据失败后仅尝试向B节点请求数据而不再轮询。节点B将只接受带ASKING命令的IMPORTING 数据查询。
简单说，ASKING 命令给IMPORTING slot添加了一个只轮询一次的标记。
### Clients implementations hints
TODO Pipelining: use MULTI/EXEC for pipelining.
TODO Persistent connections to nodes.
TODO hash slot guessing algorithm.
## 单点故障
### 节点故障侦测
故障侦测使用以下方法实现：
- 如果一个节点没有在给定时间内回复PING请求，则该一个节点会被其他节点设置 PFAIL 标志(possible failure 有可能故障)
- 如果一个节点被设置 PFAIL 标志，那么对目标节点设置 PFAIL 标志的节点会在节点之间互相进行广播通知并通知其他节点发送PING请求
- 如果有一个节点被设置 PFAIL 标志，并且其他节点也认同其为 PFAIL 状态，那么该节点会被设置为 FAIL 状态（故障）
- 一旦一个节点被设置 FAIL 标志，那么对故障节点设置 FAIL 标志的节点会通知其余所有节点
所以实际只有大多数节点认同的情况下，一个节点才会被设置为故障状态
（还在努力实现）一旦一个节点被设置为故障，那么其他任何节点收到来自故障节点的PING或者连接请求则会返回“MARK AS FAIL”从而强制故障节点把自己设置为故障
集群状态侦测（目前仅实现了一部分）：每当集群配置文件发生变更，所有集群节点都会重新扫描节点列表（这可以是由更改一个hash slot 或者只是一个节点故障造成的）
每个被扫描的节点会返回以下状态中的一个：
- FAIL：节点故障
- OK：节点正常
这意味着Redis集群被设计为有能力拒绝对故障节点的查询。然而这里有一个特例，就是一个节点从被设置为 PFAIL 到被设置为 FAIL 是有时间差的，如果仅仅是被设置为 PFAIL 还是有可能对该节点尝试连接
另外，Redis集群将不再支持MULTI/EXEC批量方法
### 从节点推举制度（未实现）
每个主节点可以拥有0个或者多个从节点。当主节点发生故障的时候，从节点有责任/义务推举自己成为主节点。假设我们有A1，A2，A3三个节点，A1是主节点并且A2和A3为A1的从节点
如果A1发生故障并且长时间未回复PING请求，那么其他节点将会将A1标记为故障节点。当这种情况发生的时候，第一个从节点将会尝试推举自己为主节点。
定义第一个从节点非常简单。取所有从节点中节点ID最小的那个。如果第一个从节点也被标记为故障，那么就由第二个从节点推举自己，以此类推。
实际流程是：集群配置被变更（节点故障导致的），故障节点所有的从节点检测自己是否是第一个从节点。从节点在升级为主节点后会通知其他节点更改配置
### 保护模式（未实现）
如果部分节点由于网络原因被隔离（比如断网），则这些节点会停止判断其他节点是否正常，而会开始从节点推举或者其他操作去更改集群配置。为了防止这种情况发生，节点间一旦发现大部分节点在一段时间内被标记为 PFAIL 或者 FAIL 状态则会立即让集群启动保护模式以阻止集群采取任何行动（更改配置）。
一旦集群状态恢复正常则保护模式会被取消
### 主节点多数原则（未完成）
对于发生网络故障的情况，2个或者更多的分片有能力处理所有的hash slots。而这会影响集群数据的一致性，所以网络故障应该导致0或者只有1个分区可用。
为了强制此规则生效，所有符合主节点多数原则的节点应该强制不处理任何命令。
### Publish/Subscribe（已实现，但是会重定义）
在一个Redis集群中，所有节点都被允许订阅其他节点或者对其他节点进行广播。集群系统会保证所有的广播通知给所有节点。
目前的实现仅仅是简单的一一进行广播，但是在某种程度上广播应该使用bloom filters或者其他算法进行优化。
另外，关于Redis Cluster的一些信息，你还可以参考本站较早的文章：《[听 antirez 讲 Redis Cluster](http://blog.nosqlfan.com/html/1007.html)》（视频+PPT）
http://quietmadman.blog.51cto.com/3269500/1553232
这里使用的 redis 版本是 redis-3.0.0-beta8（2.9.57）来安装和配置的。
整个安装和配置过程都是参考的 redis 官方的 cluster 手册: http://redis.io/topics/cluster-tutorial
其他的一些 API 和 redis cluster 相关的开源项目有:
https://github.com/xetorthio/jedis
https://github.com/antirez/redis-rb-cluster
https://github.com/Grokzen/redis-py-cluster
在安装配置之前对 redis cluster 有了一点初步的了解:
+ 每一个 redis cluster 节点都至少需要两个 tcp 连接，一个是用于为 client 服务的监听端口(如: 6379)，另一个则用于为 cluster 节点通信提供的通道(cluster bus，如：6379+10000 = 16379)；
+ cluster 节点之间传输的协议为 binary protocol，主要用于 故障检测(failure detection)，配置更新(configuration update)，故障转移授权(failover authorization)等等；
+ Redis Cluster 并没有采用一致性 hash 算法来对 data 进行 sharding，而是采用了简单的 hash slot 机制来实现 -- 计算给定 key 的 hash slot 槽位 -- CRC16(key) % 16384 ；
+ Redis Cluster 最多能够支持 16384 个节点；
+ 为了把所有 slot 占满，当节点少的时候，需要每一个节点分配一定范围的 slots，如：
  - Node A contains hash slots from 0 to 5500
  - Node B contains hash slots from 5501 to 11000
  - Node C contains hash slots from 11001 to 16384
  当有新的 node 进来，那需要从 A，B，C 中移出一些 slots 分配给 D。
  当有节点要退出(如：A)，那就需要将 A 上的 slots 再分配给 B 和 C。
+ Redis Cluster 支持 master-slave 模型来实现高可用性，一个节点退出，可以再次选举出该节点的某一个 slave 节点作为新的 master 节点来服务；
下面开始安装配置：
1，创建本地测试目录：
```
```bash
$
```
```bash
mkdir
```
```bash
-p
```
```bash
/root/test/redis-cluster
```
```
2，本次测试以在同台主机上测试集群，端口从 7000~7005 的 6 个节点
```
```bash
$
```
```bash
cd
```
```bash
/root/test/redis-cluster
```
```bash
$
```
```bash
mkdir
```
```bash
7000 7001 7002 7003 7004 7005
```
```
3，在每个节点目录下分别创建 redis.conf 配置文件，这里的配置文件内容都如下(端口除外)：
```
```bash
port 7000
```
```bash
cluster-enabled
```
```bash
yes
```
```bash
cluster-config-
```
```bash
file
```
```bash
nodes.conf
```
```bash
cluster-node-timeout 5000
```
```bash
appendonly
```
```bash
yes
```
```
创建好后，结构如下：
```
```bash
[redis-cluster]
```
```bash
# tree
```
```bash
.
```
```bash
├── 7000
```
```bash
│   └── redis.conf
```
```bash
├── 7001
```
```bash
│   └── redis.conf
```
```bash
├── 7002
```
```bash
│   └── redis.conf
```
```bash
├── 7003
```
```bash
│   └── redis.conf
```
```bash
├── 7004
```
```bash
│   └── redis.conf
```
```bash
└── 7005
```
```bash
```
```bash
└── redis.conf
```
```bash
```
```bash
6 directories, 6 files
```
```
4，启动这 6 个节点，如下示例：
```
```bash
$
```
```bash
cd
```
```bash
7000/
```
```bash
$ redis-server redis.conf
```
```
  运行后在每个节点的目录下可以看到新增了 nodes.conf 配置，如 7000 节点的配置如下：
```
```bash
123ed65d59ff22370f2f09546f410d31207789f6 :0 myself,master - 0 0 0 connected
```
```bash
vars currentEpoch 0 lastVoteEpoch 0
```
```
  其中 123ed65d59ff22370f2f09546f410d31207789f6 为每个节点给自己分配的 NodeID，该 ID 在之后的 cluster 环境中唯一的标识该节点实例，每一个节点也都是通过这个 ID 来表示其他的节点的。
5，查看网络监听，可以看到同时监听 700* + 10000
```
```bash
tcp        0      0 :::17003                    :::*                        LISTEN      10614
```
```bash
/redis-server
```
```bash
tcp        0      0 :::17004                    :::*                        LISTEN      10655
```
```bash
/redis-server
```
```bash
tcp        0      0 :::17005                    :::*                        LISTEN      10696
```
```bash
/redis-server
```
```bash
tcp        0      0 :::7000                     :::*                        LISTEN      10483
```
```bash
/redis-server
```
```bash
tcp        0      0 :::7001                     :::*                        LISTEN      10549
```
```bash
/redis-server
```
```bash
tcp        0      0 :::7002                     :::*                        LISTEN      10573
```
```bash
/redis-server
```
```bash
tcp        0      0 :::7003                     :::*                        LISTEN      10614
```
```bash
/redis-server
```
```bash
tcp        0      0 :::7004                     :::*                        LISTEN      10655
```
```bash
/redis-server
```
```bash
tcp        0      0 :::7005                     :::*                        LISTEN      10696
```
```bash
/redis-server
```
```bash
tcp        0      0 :::17000                    :::*                        LISTEN      10483
```
```bash
/redis-server
```
```bash
tcp        0      0 :::17001                    :::*                        LISTEN      10549
```
```bash
/redis-server
```
```bash
tcp        0      0 :::17002                    :::*                        LISTEN      10573
```
```bash
/redis-server
```
```
6，下面开始搭建集群，这里使用源码中自带的 ruby 脚本 redis-trib.rb 来创建集群，因此首先确保系统已经安装了最新版本的 ruby 环境
  redis-trib.rb 除了创建集群，还可以 check，reshard 一个存在的 cluster。
```
```bash
$ .
```
```bash
/redis-trib
```
```bash
.rb create --replicas 1 127.0.0.1:7000 127.0.0.1:7001 127.0.0.1:7002 127.0.0.1:7003 127.0.0.1:7004 127.0.0.1:7005
```
```bash
>>> Creating cluster
```
```bash
Connecting to node 127.0.0.1:7000: OK
```
```bash
Connecting to node 127.0.0.1:7001: OK
```
```bash
... ...
```
```bash
>>> Performing
```
```bash
hash
```
```bash
slots allocation on 6 nodes...
```
```bash
Using 3 masters:
```
```bash
127.0.0.1:7000
```
```bash
127.0.0.1:7001
```
```bash
127.0.0.1:7002
```
```bash
Adding replica 127.0.0.1:7003 to 127.0.0.1:7000
```
```bash
Adding replica 127.0.0.1:7004 to 127.0.0.1:7001
```
```bash
Adding replica 127.0.0.1:7005 to 127.0.0.1:7002
```
```bash
```
```bash
//
```
```bash
M 开头的为 master 节点
```
```bash
//
```
```bash
S 开头的为 slave 节点，同时 replicates 后面的 NodeID 为 master 节点ID
```
```bash
M: 123ed65d59ff22370f2f09546f410d31207789f6 127.0.0.1:7000
```
```bash
```
```bash
slots:0-5460 (5461 slots) master
```
```bash
M: 82578e8ec9747e46cbb4b8cc2484c71b9b2c91f4 127.0.0.1:7001
```
```bash
```
```bash
slots:5461-10922 (5462 slots) master
```
```bash
M: f5bdda1518cd3826100a30f5953ed82a5861ed48 127.0.0.1:7002
```
```bash
```
```bash
slots:10923-16383 (5461 slots) master
```
```bash
S: 35e0f6fdadbf81a00a1d6d1843698613e653867b 127.0.0.1:7003
```
```bash
```
```bash
replicates 123ed65d59ff22370f2f09546f410d31207789f6
```
```bash
S: 61dfb1055760d5dcf6519e35435d60dc5b207940 127.0.0.1:7004
```
```bash
```
```bash
replicates 82578e8ec9747e46cbb4b8cc2484c71b9b2c91f4
```
```bash
S: bfc910f924d772fe03d9fe6a19aabd73d5730d26 127.0.0.1:7005
```
```bash
```
```bash
replicates f5bdda1518cd3826100a30f5953ed82a5861ed48
```
```bash
```
```bash
//
```
```bash
回复
```
```bash
yes
```
```bash
表示接受 redis-trib 的 master-slave 的配置
```
```bash
Can I
```
```bash
set
```
```bash
the above configuration? (
```
```bash
type
```
```bash
'yes'
```
```bash
to accept):
```
```bash
yes
```
```bash
>>> Nodes configuration updated
```
```bash
>>> Assign a different config epoch to each node
```
```bash
>>> Sending CLUSTER MEET messages to
```
```bash
join
```
```bash
the cluster
```
```bash
```
```bash
//
```
```bash
等待 cluster 的 node 加入
```
```bash
Waiting
```
```bash
for
```
```bash
the cluster to
```
```bash
join
```
```bash
...
```
```bash
```
```bash
//
```
```bash
加入后开始给各个 node 分配 slots 槽位
```
```bash
//
```
```bash
如：127.0.0.1:7000 的 master 节点分配了 0-5460 的槽位
```
```bash
>>> Performing Cluster Check (using node 127.0.0.1:7000)
```
```bash
M: 123ed65d59ff22370f2f09546f410d31207789f6 127.0.0.1:7000
```
```bash
```
```bash
slots:0-5460 (5461 slots) master
```
```bash
M: 82578e8ec9747e46cbb4b8cc2484c71b9b2c91f4 127.0.0.1:7001
```
```bash
```
```bash
slots:5461-10922 (5462 slots) master
```
```bash
M: f5bdda1518cd3826100a30f5953ed82a5861ed48 127.0.0.1:7002
```
```bash
```
```bash
slots:10923-16383 (5461 slots) master
```
```bash
M: 35e0f6fdadbf81a00a1d6d1843698613e653867b 127.0.0.1:7003
```
```bash
```
```bash
slots: (0 slots) master
```
```bash
```
```bash
replicates 123ed65d59ff22370f2f09546f410d31207789f6
```
```bash
M: 61dfb1055760d5dcf6519e35435d60dc5b207940 127.0.0.1:7004
```
```bash
```
```bash
slots: (0 slots) master
```
```bash
```
```bash
replicates 82578e8ec9747e46cbb4b8cc2484c71b9b2c91f4
```
```bash
M: bfc910f924d772fe03d9fe6a19aabd73d5730d26 127.0.0.1:7005
```
```bash
```
```bash
slots: (0 slots) master
```
```bash
```
```bash
replicates f5bdda1518cd3826100a30f5953ed82a5861ed48
```
```bash
[OK] All nodes agree about slots configuration.
```
```bash
>>> Check
```
```bash
for
```
```bash
open
```
```bash
slots...
```
```bash
>>> Check slots coverage...
```
```bash
[OK] All 16384 slots covered.
```
```bash
[root@sasd redis-cluster]
```
```bash
#
```
```
下面为使用 cluster nodes 和 slots 命令查看节点以及 slot 的信息：
```
```bash
$ redis-cli -c -p 7000
```
```bash
//
```
```bash
查看节点分布信息
```
```bash
127.0.0.1:7000> cluster nodes
```
```bash
35e0f6fdadbf81a00a1d6d1843698613e653867b 127.0.0.1:7003 slave 123ed65d59ff22370f2f09546f410d31207789f6 0 1410835785216 4 connected
```
```bash
61dfb1055760d5dcf6519e35435d60dc5b207940 127.0.0.1:7004 slave 82578e8ec9747e46cbb4b8cc2484c71b9b2c91f4 0 1410835784715 5 connected
```
```bash
82578e8ec9747e46cbb4b8cc2484c71b9b2c91f4 127.0.0.1:7001 master - 0 1410835786217 2 connected 5461-10922
```
```bash
123ed65d59ff22370f2f09546f410d31207789f6 127.0.0.1:7000 myself,master - 0 0 1 connected 0-5460
```
```bash
bfc910f924d772fe03d9fe6a19aabd73d5730d26 127.0.0.1:7005 slave f5bdda1518cd3826100a30f5953ed82a5861ed48 0 1410835786717 6 connected
```
```bash
f5bdda1518cd3826100a30f5953ed82a5861ed48 127.0.0.1:7002 master - 0 1410835785715 3 connected 10923-16383
```
```bash
127.0.0.1:7000>
```
```bash
//
```
```bash
查看 slots 分布信息
```
```bash
127.0.0.1:7000> cluster slots
```
```bash
1) 1) (integer) 5461
```
```bash
//
```
```bash
slots 的其实槽位索引
```
```bash
```
```bash
2) (integer) 10922
```
```bash
//
```
```bash
slots 的结束槽位索引
```
```bash
```
```bash
//
```
```bash
即 [5461, 10922]
```
```bash
```
```bash
3) 1)
```
```bash
"127.0.0.1"
```
```bash
```
```bash
2) (integer) 7001
```
```bash
//
```
```bash
master
```
```bash
```
```bash
4) 1)
```
```bash
"127.0.0.1"
```
```bash
```
```bash
2) (integer) 7004
```
```bash
//
```
```bash
对应的 slave 节点
```
```bash
2) 1) (integer) 0
```
```bash
```
```bash
2) (integer) 5460
```
```bash
```
```bash
3) 1)
```
```bash
"127.0.0.1"
```
```bash
```
```bash
2) (integer) 7000
```
```bash
```
```bash
4) 1)
```
```bash
"127.0.0.1"
```
```bash
```
```bash
2) (integer) 7003
```
```bash
3) 1) (integer) 10923
```
```bash
```
```bash
2) (integer) 16383
```
```bash
```
```bash
3) 1)
```
```bash
"127.0.0.1"
```
```bash
```
```bash
2) (integer) 7002
```
```bash
```
```bash
4) 1)
```
```bash
"127.0.0.1"
```
```bash
```
```bash
2) (integer) 7005
```
```bash
127.0.0.1:7000>
```
```
本文出自 “[安静的疯子](http://quietmadman.blog.51cto.com/)” 博客，请务必保留此出处[http://quietmadman.blog.51cto.com/3269500/1553232](http://quietmadman.blog.51cto.com/3269500/1553232)
http://blog.csdn.net/freebird_lb/article/details/7778999
Redis-2.4.15目前没有提供集群的功能，Redis作者在博客中说将在3.0中实现集群机制。目前Redis实现集群的方法主要是采用一致性哈稀分片（Shard），将不同的key分配到不同的redis server上，达到横向扩展的目的。下面来介绍一种比较常用的分布式场景：
在读写操作比较均匀且实时性要求较高，可以用下图的分布式模式：
在读操作远远多于写操作时，可以用下图的分布式模式：
       对于一致性哈稀分片的算法，Jedis-2.0.0已经提供了，下面是使用示例代码（以ShardedJedisPool为例）：
package com.jd.redis.client;
import java.util.ArrayList;
import java.util.List;
import redis.clients.jedis.JedisPoolConfig;
import redis.clients.jedis.JedisShardInfo;
import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPool;
import redis.clients.util.Hashing;
import redis.clients.util.Sharded;
publicclass RedisShardPoolTest {
static ShardedJedisPoolpool;
static{
        JedisPoolConfig config =new JedisPoolConfig();//Jedis池配置
        config.setMaxActive(500);//最大活动的对象个数
          config.setMaxIdle(1000 * 60);//对象最大空闲时间
          config.setMaxWait(1000 * 10);//获取对象时最大等待时间
          config.setTestOnBorrow(true);
        String hostA = "10.10.224.44";
int portA = 6379;
          String hostB = "10.10.224.48";
int portB = 6379;
        List<JedisShardInfo> jdsInfoList =new ArrayList<JedisShardInfo>(2);
        JedisShardInfo infoA = new JedisShardInfo(hostA, portA);
        infoA.setPassword("redis.360buy");
        JedisShardInfo infoB = new JedisShardInfo(hostB, portB);
        infoB.setPassword("redis.360buy");
        jdsInfoList.add(infoA);
        jdsInfoList.add(infoB);
pool =new ShardedJedisPool(config, jdsInfoList, Hashing.MURMUR_HASH,
Sharded.DEFAULT_KEY_TAG_PATTERN);
    }
    /**
     * @param args
     */
publicstaticvoid main(String[] args) {
for(int i=0; i<100; i++){
            String key = generateKey();
            //key += "{aaa}";
            ShardedJedis jds = null;
try {
                jds = pool.getResource();
                System.out.println(key+":"+jds.getShard(key).getClient().getHost());
                System.out.println(jds.set(key,"1111111111111111111111111111111"));
            } catch (Exception e) {
                e.printStackTrace();
            }
finally{
pool.returnResource(jds);
            }
        }
    }
privatestaticintindex = 1;
publicstatic String generateKey(){
return String.valueOf(Thread.currentThread().getId())+"_"+(index++);
    }
}
从运行结果中可以看到，不同的key被分配到不同的Redis-Server上去了。
       实际上，上面的集群模式还存在两个问题：
1.       扩容问题：
因为使用了一致性哈稀进行分片，那么不同的key分布到不同的Redis-Server上，当我们需要扩容时，需要增加机器到分片列表中，这时候会使得同样的key算出来落到跟原来不同的机器上，这样如果要取某一个值，会出现取不到的情况，对于这种情况，Redis的作者提出了一种名为Pre-Sharding的方式：
Pre-Sharding方法是将每一个台物理机上，运行多个不同断口的Redis实例，假如有三个物理机，每个物理机运行三个Redis实际，那么我们的分片列表中实际有9个Redis实例，当我们需要扩容时，增加一台物理机，步骤如下：
A.     在新的物理机上运行Redis-Server；
B.      该Redis-Server从属于(slaveof)分片列表中的某一Redis-Server（假设叫RedisA）；
C.      等主从复制(Replication)完成后，将客户端分片列表中RedisA的IP和端口改为新物理机上Redis-Server的IP和端口；
D.     停止RedisA。
这样相当于将某一Redis-Server转移到了一台新机器上。Prd-Sharding实际上是一种在线扩容的办法，但还是很依赖Redis本身的复制功能的，如果主库快照数据文件过大，这个复制的过程也会很久，同时会给主库带来压力。所以做这个拆分的过程最好选择为业务访问低峰时段进行。
[http://blog.nosqlfan.com/html/3153.html](http://blog.nosqlfan.com/html/3153.html)
2.       单点故障问题：
还是用到Redis主从复制的功能，两台物理主机上分别都运行有Redis-Server，其中一个Redis-Server是另一个的从库，采用双机热备技术，客户端通过虚拟IP访问主库的物理IP，当主库宕机时，切换到从库的物理IP。只是事后修复主库时，应该将之前的从库改为主库（使用命令slaveof no one），主库变为其从库（使命令slaveof IP PORT），这样才能保证修复期间新增数据的一致性。
http://www.searchdatabase.com.cn/showcontent_78941.htm
豁达是正确乐观的面对失败的系统。不需要过多的担心，需要一种去说那又怎样的能力。因此架构的设计是如此的重要。许多优秀的系统没有进一步成长的能力，我们应该做的是使用其他的系统去共同分担工作。
[Redis](http://www.searchdatabase.com.cn/showcontent_73774.htm)是其中一个吸引我的系统，一个持久性的，键值对存储内存操作高性能的平台。它是一个优秀的键值对数据库。我已经在使用了。即使AWS最近宣布开始支持ElasticCache的下级缓存。但是一个无主的redis集群仍然起着重要的作用。我们需要多系统去完成工作。同时，我们能够集合多种组件在一个容错和无主的集群里共同工作么？在这片文章中我将介绍梦幻般的redis。
一致哈希
构建一个存储数据集群的关键是有一个有效的数据存储和复制机制。我希望通过一个行之有效的方法来说明建造一个数据集群，在这个过程中你可以随意添加或移除一个Redis节点，同时保证你的数据仍然存在，而不会消失。这个方法称为一致哈希。
由于它不是一个很明显的概念，我将用一点时间来解释一下。为了理解一致哈希，你可以想像有一个函数f（x），对于给定的x总是返回一个1到60（为什么是60？你会知道的，但现在请等等）之间的结果，同样对于一个唯一的x，f（x）总是返回相同的结果。这些1到60的值按顺时针排成一个环。
现在[集群](http://www.searchdatabase.com.cn/showcontent_72562.htm)中的每个节点都需要一个唯一的名字。所以如果你将这个名字传递给f（''），它将返回一个1到60之间的数字（包括1和60），这个数字就是节点在环上的位置。当然它只是节点的逻辑（记录的）位置。这样，你获得一个节点，将它传给哈希函数，获得结果并将它放到环上。是不是很简单？这样每个节点都在环上有了它自己的位置。假设这里有5个Redis节点，名字分别为'a'，'b'，'c'，'d'，'e'。每个节点都传给哈希函数f（x）并且放到了环上。在这里f（'a'）
 = 21, f（'b'） = 49, f（'c'） = 11, f（'d'） = 40, f（'e'） = 57。一定记得这里位置是逻辑位置。
那么，我们为什么要将节点放在一个环上呢？将节点放到环上的目的是确定拥有哪些哈希空间。图中的节点'd'拥有的哈希空间就是f（'a'）到f（'d'）（其值为40）之间的部分，包括f（'d'），即（21, 40]。也就是说节点'd'将拥有键x，如果f（x）的属于区间（21, 40】。比如键‘apple’，其值f（'apple'） = 35，那么键'apple'将被存在'd'节点。类似的，每个存储在集群上的键都会通过哈希函数，在环上按顺时针方向被恰当地存到最近的节点。
虽然一致哈希讲完了，但应知道，在多数情况下，这种类型的系统是伴随着高可用性而构建。为了满足数据的高可用性，需要根据一些因子进行复制， 这些因子称为复制因子。假设我们集群的复制因子为2，那么属于'd'节点的数据将会被复制到按顺时针方向与之相隔最近的'b'和'e'节点上。这就保证了如果从'd'节点获取数据失败，这些数据能够从'b'或'e'节点获取。
不仅仅是键使用一致哈希来存储，也很容易覆盖失败了的节点，并且复制因子依然完好有效。比如'd'节点失败了，'b'节点将获取'd'节点哈希空间的所有权，同时'd'节点的哈希空间能够很容易地复制到'c'节点。
坏事和好事
坏事就是目前这些讨论过的所有概念，复制（冗余），失效处理以及集群规模等，在Redis之外是不可行的。一致哈希仅仅描述了节点在哈希环上的映射以及那些哈希数据的所有权，尽管这样，它仍然是构建一个弹性可扩展系统的极好的开端。
好事就是，也有一些分立的其他工具在Redis集群上实现一致哈希，它们能提醒节点失效和新节点的加入。虽然这个功能不是一个工具的一部分，我们将看到如何用多个系统来使一个理想化的Redis集群运转起来。
Twemproxy aka Nutcracker
Twemproxy是一个开源工具，它是一个基于memcached和Redis协议的快速、轻量的代理。其本质就是，如果你有一些[Redis服务器](http://www.searchdatabase.com.cn/showcontent_71222.htm)在运行，同时希望用这些服务器构建集群，你只需要将Twemproxy部署在这些服务器前端，并且让所有Redis流量都通过它。
Twemproxy除了能够代理Redis流量外，在它存储数据在Redis服务器时还能进行一致哈希。这就保证了数据被分布在基于一致哈希的多个不同Redis节点上。
但是Twemproxy并没有为Redis集群建立高可用性支持。最简单的办法是为集群中的每个节点都建立一个从（冗余）服务器，当主服务器失效时将从（冗余）服务器提升为主服务器。为Redis配置一个从服务器是非常简单的。
这种模型的缺点是非常明显的，它需要为Redis集群中的每个节点同时运行两个服务器。但是节点失效也是非常明显，并且更加危险，所以我们怎么知道这些问题并解决。
Gossip on Serf
Gossip是一个标准的机制，通过这个机制集群上的节点可以很清楚的了解成员的最新情况。这样子集群中的每个节点就很清楚集群中节点的变化，如节点的新增和节目的删除。
Serf通过实现Gossip机制提供这样的帮助。Serf是一个基于代理的机制，这个机制实现了Gossip的协议达到节点成员信息交换的目的。Serf是不断运行，除此之外，它还可以生成自定义的事件
现在拿我们的节点集群为例，如果每个节点上也有一个serf代理正在运行,那么节点与节点之间可以进行细节交换.因此，群集中的每个节点都能清楚知道其他节点的存在，也能清楚知道他们的状态。
这还并不够，为了高可靠性我们还需要让twemproxy知道何时一个节点已经失效，这样的话它就可以据此修改它的配置。像前面提到的Serf，就可以做到这一点，它是基于一些gossip触发的事件，使用自定义动作实现的。因此只要集群中的一个Redis节点因为一些原因宕掉了，另一个节点就可以发送有成员意外掉线的消息给任何给定的端点，这个端点在我们的案例中也就是twemproxy服务器。
这还不是全部
现在我们有了Redis集群，基于一致性哈希环，相应的是用twemproxy存储数据（一致性哈希），还有Serf，它用gossip协议来检测Redis集群成员失效，并且向twemproxy发送失效消息；但是我们还没有建立起理想化的Redis集群。
消息侦听器
虽然Serf可以给任何端点发送成员离线或者成员上线消息。然而twemproxy却没有侦听此类事件的机制。因此我们需要自定义一个侦听器，就像Redis-Twenproxy代理，它需要做以下这些事情。
- 侦听Serf消息
- 更新nutcraker.yml 以反映新的拓扑
- 重启twemproxy
这个消息侦听器可以是一个小型的http服务器；它在收到一批POST数据的时候，为twemproxy做以上列表中的动作。需要记住的是，这种消息应该是一个原子操作；因为当一个节点失效（或者意外离线）的时候，所有能发消息的活动节点都将发送这个失效事件消息给侦听器；但是侦听器应该只响应一次。
数据复制
在上面的“一致哈希”中，我提到了Redis集群中的复制因素。同样它也不是Twemproxy的固有特性；Twemproxy只关心使用一致哈希存储一个拷贝。所以在我们追求理想化Redis集群的过程中，我们还需要给twemproxy或者redis自己创建这种复制的能力。
为了给Twenproxy创建复制能力，需要将复制因子作为一个配置项目，并且将数据保存在集群中相邻的redis节点（根据复制因子）。由于twemproxy知道节点的位置，所以这将给twemproxy增加一个很棒的功能。
由于twemproxy只不过是代理服务器，它的简单功能就是它强大的地方，为它创建复制管理功能将使它臃肿膨胀。
Redis 主从环
在思考这其中的工作机制的时候，我忽然想到，为什么不将每个节点设置成另一个节点的副本，或者说从节点，并由此而形成一个主从环呢？
这样的话如果一个节点失效了，失效节点的数据在这个环上相邻的节点上仍然可以获得。而那个具有该数据副本的节点，将作为该节点的从节点，并提供保存数据副本的服务。这是一个既是主节点也是从节点的环。Serf仍像通常一样作为散布节点失效消息的代理。但是这一回，我们twenproxy上的的客户端，即侦听器，将不仅仅只更新twenproxy上的失效信息，还要调整redis服务器群集来适应这个变化。
在这个环中有一个明显的，同样也是技术方面的缺陷。这个明显的缺陷是，这个环会坏掉，因为从节点的从节点无法判别哪一个是它的主节点的数据，哪一个是它的主节点的主节点的数据。这样的话就会循环的传送所有的数据。
同样技术性的问题是，一旦redis将主节点的数据同步给从节点，它就会将从节点的数据擦除干净；这样就将曾经写到从节点的所有数据删除了。这种主从环显然不能实际应用，除非修改主从环的复制机制以适应我们的需求。这样的话每个从节点就不会将它的主节点的数据同步给它的从节点。要想实现这一点，必须的条件是每个节点都可以区分出自己的密钥空间，以及它的主节点的密钥空间；这样的话它就不会将主节点的数据传送给它的从节点。
那么这样一来，当一个节点失效时，就需要执行四个动作。一，将失效节点的从节点作为它的密钥空间的所有者。二，将这些密钥散布给失效节点从节点的从节点，以便进行复制。三，将失效节点的从节点作为失效节点的主节点的从节点。最后，在新的拓扑上复位twemproxy。
如何理想化？
事实上并没有这样的Redis集群，它可以具有一致性的哈希，高可靠性以及分区容错性。因此最后一幅图片描绘了一种理想化的Redis集群；但这并不是不可能的。接下来将罗列一下需要哪些条件才能使之成为一个实实在在的产品。
透明的Twenproxy
有必要部署一个Twenproxy，这会使得Hash散列中Redis各节点的位置都是透明的。每个Redis节点都可以知道自己以及其相邻节点的位置，这些信息对于节点的主从复制以及失败节点的修复是有必要的。自从Twenproxy开源之后，节点的位置信息可以被修改、以及扩展。
Redis的数据拥有权
这是比较困难的部分的，每个Redis节点都应该记录自身拥有的数据，以及哪些是主节点的数据。当前这样的隔离是不可能的。这也需要修改Redis的基础代码，这样节点才知道何时与从节点同步，什么时候不需要。
综上所述，我们的理想化的Redis集群变成现实需要修改这样的两个组件。一直以来，它们都是非常大的工业级别，使用在生产环境中。这已经值得任何人去实现这个集群了。
原文出处：http://megam.in/post/66659169136/utopian-redis-cluster
http://www.cnblogs.com/lulu/archive/2013/06/10/3130906.html
使用zookeeper 实现一致性hash。
redis服务启动时，将自己的路由信息通过临时节点方式写入zk，客户端通过zk client读取可用的路由信息。
![image_thumb[12]](http://images.cnitblog.com/blog/28886/201306/10171026-9c459434bb9e45f59d63d934110fd4b5.png)
# 服务端
> 
使用python 脚本写的守护进程：[https://github.com/LittlePeng/redis-manager](https://github.com/LittlePeng/redis-manager)
脚本部署在redis-server本机，定时ping redis-server
节点失效的情况：
1.服务器与ZK服务器失去连接 Session Expired ，环境网络波动造成，需要根据网络情况设置适当zookeeper的Timeout时间，避免此情况发生
2. 服务器宕机，Zookeeper server 发现zkclient ping超时，就会通知节点下线
3. redis-server 挂了，redis-manager ping 超时主动断开与zookeeper server的连接
# 客户端
> 
> 
需要zkclient监控 节点变化，及时更新路由策略
下面是C# 版本一致性hash算法：
   1:  class KetamaNodeLocator   2:      {   3:          private Dictionary<long, RedisCluster> ketamaNodes;   4:          private HashAlgorithm hashAlg;   5:          private int numReps = 160;   6:          private long[] keys;   7:      8:          public KetamaNodeLocator(List<RedisCluster> nodes)   9:          {  10:              ketamaNodes = new Dictionary<long, RedisCluster>();  11:     12:              //对所有节点，生成nCopies个虚拟结点  13:              for (int j = 0; j < nodes.Count; j++) {  14:                  RedisCluster node = nodes[j];  15:                  int numReps = node.Weight;  16:     17:                  //每四个虚拟结点为一组  18:                  for (int i = 0; i < numReps / 4; i++) {  19:                      byte[] digest = ComputeMd5(  20:                          String.Format("{0}_{1}_{2}", node.RoleName, node.RouteValue, i));  21:     22:                      /** Md5是一个16字节长度的数组，将16字节的数组每四个字节一组，  23:                       * 分别对应一个虚拟结点，这就是为什么上面把虚拟结点四个划分一组的原因*/  24:                      for (int h = 0; h < 4; h++) {  25:     26:                          long rv = ((long)(digest[3 + h * 4] & 0xFF) << 24)  27:                                     | ((long)(digest[2 + h * 4] & 0xFF) << 16)  28:                                     | ((long)(digest[1 + h * 4] & 0xFF) << 8)  29:                                     | ((long)digest[0 + h * 4] & 0xFF);  30:     31:                          rv = rv & 0xffffffffL; /* Truncate to 32-bits */  32:                          ketamaNodes[rv] = node;  33:                      }  34:                  }  35:              }  36:     37:              keys = ketamaNodes.Keys.OrderBy(p => p).ToArray();  38:          }  41:          public RedisCluster GetWorkerNode(string k)  42:          {  43:              byte[] digest = ComputeMd5(k);  44:              return GetNodeInner(Hash(digest, 0));  45:          }  46:     47:          RedisCluster GetNodeInner(long hash)  48:          {  49:              if (ketamaNodes.Count == 0)  50:                  return null;  51:              long key = hash;  52:              int near = 0;  53:              int index = Array.BinarySearch(keys, hash);  54:              if (index < 0) {  55:                  near = (~index);  56:                  if (near == keys.Length)  57:                      near = 0;  58:              }  59:              else {  60:                  near = index;  61:              }  62:     63:              return ketamaNodes[keys[near]];  64:          }  65:     66:          public static long Hash(byte[] digest, int nTime)  67:          {  68:              long rv = ((long)(digest[3 + nTime * 4] & 0xFF) << 24)  69:                      | ((long)(digest[2 + nTime * 4] & 0xFF) << 16)  70:                      | ((long)(digest[1 + nTime * 4] & 0xFF) << 8)  71:                      | ((long)digest[0 + nTime * 4] & 0xFF);  72:     73:              return rv & 0xffffffffL; /* Truncate to 32-bits */  74:          }  79:          public static byte[] ComputeMd5(string k)  80:          {  81:              MD5 md5 = new MD5CryptoServiceProvider();  82:     83:              byte[] keyBytes = md5.ComputeHash(Encoding.UTF8.GetBytes(k));  84:              md5.Clear();  85:              return keyBytes;  86:          }  87:      }

