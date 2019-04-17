# python操作redis - kittyzc的博客 - CSDN博客





2018年10月12日 10:01:55[IE06](https://me.csdn.net/kittyzc)阅读数：39
个人分类：[python																[算法](https://blog.csdn.net/kittyzc/article/category/7940151)](https://blog.csdn.net/kittyzc/article/category/7940149)

所属专栏：[python小tips](https://blog.csdn.net/column/details/31935.html)








# 1. redis介绍

redis是一个运行在内存中的key-value存储系统，主要优势在于读写性能非常棒。redis中的数据持久化方法有两种：1. 固定时间点转储；2. 修改命令写入文件

reids的安装非常简单，到[这里](http://www.redis.net.cn/download/)下载最新版本，解压缩后进入文件夹，执行make命令即可。

make完后 redis-2.8.17目录下会出现编译后的redis服务程序redis-server，还有用于测试的客户端程序redis-cli。到src文件夹下执行redis-server即可启动redis服务：
![在这里插入图片描述](https://img-blog.csdn.net/20181012095120681?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tpdHR5emM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 2. 客户端连接redis

在客户端使用redis也非常简单，首先安装redis库：

```
pip install redis
```

然后在python下就可以操作redis了：

```python
import redis

r = redis.Redis(host='localhost', port=6379, decode_responses=True)#这里的host和port按照服务器的配置来修改
r.set('name', 'chen') 
print(r['name'])
```

如果是集群模式，则还需要安装redis-py-cluster：

```
pip install redis-py-cluster
```

然后redis操作使用如下方式：

```python
from rediscluster import StrictRedisCluster
redis_nodes =  [{'host':'10.111.70.52','port':8489},
                {'host':'10.111.70.53','port':8489},
                {'host':'10.111.70.51','port':8489}]
r = StrictRedisCluster(startup_nodes=redis_nodes)
r.hlen("gh:wtte9t")
```

# 3. redis常用命令

redis的五种数据结构：string, list(of string), set(of string) ,hash(无序散列表，key-value set) ,zset(有序集合，member-value，value为浮点数)，支持linked-list

常用命令：set (key : value)，get (key)，incr自增，del删除，expire (key time)设置生命周期，ttl：查看生命

列表：lpush，rpush，lpop，rpop，lrange (key range)列出元素，llen列出长度

集合：sadd，srem，sismember，sunion

有序集合（带有用于排序的key）：zadd，zrange

哈希集（string-string的map，hashmap）：hset，hget，hincrby，hdel，hmset。示例：

HMSET user:1001 name “Mary Jones” password “hidden” email ["mjones@example.com](mailto:%22mjones@example.com)"





