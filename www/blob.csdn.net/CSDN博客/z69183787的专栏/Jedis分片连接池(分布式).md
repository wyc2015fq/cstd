# Jedis分片连接池(分布式) - z69183787的专栏 - CSDN博客
2015年03月25日 15:34:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：852
[http://flyingsnail.blog.51cto.com/5341669/1371650](http://flyingsnail.blog.51cto.com/5341669/1371650)
一下内容来自网络，但是很多细节没有写出来，所以我经过自己琢磨，终于找到原因了。
Redis-2.4.15目前没有提供集群的功能，Redis作者在博客中说将在3.0中实现集群机制。目前Redis实现集群的方法主要是采用一致性哈稀分片（Shard），将不同的key分配到不同的redis
 server上，达到横向扩展的目的。下面来介绍一种比较常用的分布式场景：
在读写操作比较均匀且实时性要求较高，可以用下图的分布式模式：
在读操作远远多于写操作时，可以用下图的分布式模式：
对于一致性哈稀分片的算法，Jedis-2.0.0已经提供了，下面是使用示例代码（以ShardedJedisPool为例）：
**package** com.jd.redis.client;
**import** java.util.ArrayList;
**import** java.util.List;
**import** redis.clients.jedis.JedisPoolConfig;
**import** redis.clients.jedis.JedisShardInfo;
**import** redis.clients.jedis.ShardedJedis;
**import** redis.clients.jedis.ShardedJedisPool;
**import** redis.clients.util.Hashing;
**import** redis.clients.util.Sharded;
**publicclass** RedisShardPoolTest {
**static** ShardedJedisPool*pool*;
**static**{
        JedisPoolConfig config =**new** JedisPoolConfig();//Jedis池配置
        config.setMaxActive(500);//最大活动的对象个数
          config.setMaxIdle(1000 * 60);//对象最大空闲时间
          config.setMaxWait(1000 * 10);//获取对象时最大等待时间
          config.setTestOnBorrow(**true**);
        String hostA = "10.10.224.44";
**int** portA = 6379;
          String hostB = "10.10.224.48";
**int** portB = 6379;
        List<JedisShardInfo> jdsInfoList =**new** ArrayList<JedisShardInfo>(2);
        JedisShardInfo infoA = **new** JedisShardInfo(hostA, portA);
        infoA.setPassword("redis.360buy");
        JedisShardInfo infoB = **new** JedisShardInfo(hostB, portB);
        infoB.setPassword("redis.360buy");
        jdsInfoList.add(infoA);
        jdsInfoList.add(infoB);
*pool* =**new** ShardedJedisPool(config, jdsInfoList, Hashing.*MURMUR_HASH*,
Sharded.*DEFAULT_KEY_TAG_PATTERN*);
    }
/**
 * **@param** args
 */
**publicstaticvoid** main(String[] args) {
**for**(**int** i=0; i<100; i++){
           String key =*generateKey*();
//key += "{aaa}";
           ShardedJedis jds =**null**;
**try** {
               jds =*pool*.getResource();
               System.*out*.println(key+":"+jds.getShard(key).getClient().getHost());
               System.*out*.println(jds.set(key,"1111111111111111111111111111111"));
           }**catch** (Exception e) {
               e.printStackTrace();
           }
**finally**{
*pool*.returnResource(jds);
           }
        }
    }
**privatestaticint***index* = 1;
**publicstatic** String generateKey(){
**return** String.*valueOf*(Thread.*currentThread*().getId())+"_"+(*index*++);
    }
}
从运行结果中可以看到，不同的key被分配到不同的Redis-Server上去了。
 总结： 客户端jedis的一致性哈稀进行分片原理：初始化ShardedJedisPool的时候，会将上面程序中的jdsInfoList数据进行一个算法技术，主要计算依据为list中的index位置来计算，我大概看了一下其源码如下：
# [jedis源码中ShardedJedis实现sharding](http://blog.csdn.net/mydreamongo/article/details/8951905)
（如果亲还是不信的话，可以将上面程序中的 jdsInfoList在add的时候，先add第二个，在add第一个，绝对取不出数据，原因很简单，第一次set值的时候，是按list下标来hash计算出一个服务器的，所以取值的时候，list顺序不能变动）
实际上，上面的集群模式还存在两个问题：
1.       扩容问题：
因为使用了一致性哈稀进行分片，那么不同的key分布到不同的Redis-Server上，当我们需要扩容时，需要增加机器到分片列表中，这时候会使得同样的key算出来落到跟原来不同的机器上，这样如果要取某一个值，会出现取不到的情况，对于这种情况，Redis的作者提出了一种名为Pre-Sharding的方式：
Pre-Sharding方法是将每一个台物理机上，运行多个不同断口的Redis实例，假如有三个物理机，每个物理机运行三个Redis实际，那么我们的分片列表中实际有9个Redis实例，当我们需要扩容时，增加一台物理机，步骤如下：
A.     在新的物理机上运行Redis-Server；
B.      该Redis-Server从属于(slaveof)分片列表中的某一Redis-Server（假设叫RedisA）；
C.      等主从复制(Replication)完成后，将客户端分片列表中RedisA的IP和端口改为新物理机上Redis-Server的IP和端口；
D.     停止RedisA。
这样相当于将某一Redis-Server转移到了一台新机器上。Prd-Sharding实际上是一种在线扩容的办法，但还是很依赖Redis本身的复制功能的，如果主库快照数据文件过大，这个复制的过程也会很久，同时会给主库带来压力。所以做这个拆分的过程最好选择为业务访问低峰时段进行。
再总结一下这里的扩容：其实这里的扩容很简单的思想：就是前期我们可能只用到两三个服务器，但是但是担心后期要扩容，所以前期就现在每一个机器上面再装两个redis，这样就有9个redis嘛，后面如果确实服务器不够，需要扩容，就重新找一台新机来代替9个中的一个redis，有人说，这样不还是9个么，是的，但是以前服务器上面有三个redis，压力很大的，这样做，相当于单独分离出来并且将数据一起copy给新的服务器。值得注意的是，还需要修改客户端被代替的redis的IP和端口为现在新的服务器，只要顺序不变，不会影响一致性哈希分片（刚才上面刚说了哈）。
2.       单点故障问题：
还是用到Redis主从复制的功能，两台物理主机上分别都运行有Redis-Server，其中一个Redis-Server是另一个的从库，采用双机热备技术，客户端通过虚拟IP访问主库的物理IP，当主库宕机时，切换到从库的物理IP。只是事后修复主库时，应该将之前的从库改为主库（使用命令slaveof
 no one），主库变为其从库（使命令slaveof IP PORT），这样才能保证修复期间新增数据的一致性。
