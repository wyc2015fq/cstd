
# Memcached Java Client API详解 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月10日 11:50:36[seven-soft](https://me.csdn.net/softn)阅读数：284个人分类：[memcache																](https://blog.csdn.net/softn/article/category/6304643)



## Memcached Java Client API详解
针对[Memcached官方网站](http://danga.com/memcached/)![](http://b2b-doc.alibaba-inc.com/images/icons/linkext7.gif)提供的java_memcached-release_2.0.1版本进行阅读分析，Memcached
 Java客户端lib库主要提供的调用类是SockIOPool和MemCachedClient?，关键类及方法整理说明如下。
### SockIOPool
这个类用来创建管理客户端和服务器通讯连接池，客户端主要的工作包括数据通讯、服务器定位、hash码生成等都是由这个类完成的。
public static SockIOPool getInstance()获得连接池的单态方法。这个方法有一个重载方法getInstance( String poolName )，每个poolName只构造一个SockIOPool实例。缺省构造的poolName是default。
如果在客户端配置多个memcached服务，一定要显式声明poolName。
public void setServers( String[] servers )设置连接池可用的cache服务器列表，server的构成形式是IP:PORT（如：127.0.0.1:11211）
public void setWeights( Integer[] weights )设置连接池可用cache服务器的权重，和server数组的位置一一对应
其实现方法是通过根据每个权重在连接池的bucket中放置同样数目的server（如下代码所示），因此所有权重的最大公约数应该是1，不然会引起bucket资源的浪费。
for(inti = 0; i < servers.length; i+/+ ) {if(this.weights /!=null&&this.weights.length > i ) {for(intk = 0; k <this.weights[i].intValue(); k+/+ ) {this.buckets.add( servers[i] );if( log.isDebugEnabled() ) log.debug("++++ added "+ servers[i] +" to server bucket"); } }
public void setInitConn( int initConn )设置开始时每个cache服务器的可用连接数
public void setMinConn( int minConn )设置每个服务器最少可用连接数
public void setMaxConn( int maxConn )设置每个服务器最大可用连接数
public void setMaxIdle( long maxIdle )设置可用连接池的最长等待时间
public void setMaintSleep( long maintSleep )设置连接池维护线程的睡眠时间
设置为0，维护线程不启动
维护线程主要通过log输出socket的运行状况，监测连接数目及空闲等待时间等参数以控制连接创建和关闭。
public void setNagle( boolean nagle )设置是否使用Nagle算法，因为我们的通讯数据量通常都比较大（相对TCP控制数据）而且要求响应及时，因此该值需要设置为false（默认是true）
ublic void setSocketTO( int socketTO )设置socket的读取等待超时值
public void setSocketConnectTO( int socketConnectTO )设置socket的连接等待超时值
public void setAliveCheck( boolean aliveCheck )设置连接心跳监测开关。
设为true则每次通信都要进行连接是否有效的监测，造成通信次数倍增，加大网络负载，因此该参数应该在对HA要求比较高的场合设为TRUE，默认状态是false。
public void setFailback( boolean failback )设置连接失败恢复开关
设置为TRUE，当宕机的服务器启动或中断的网络连接后，这个socket连接还可继续使用，否则将不再使用，默认状态是true，建议保持默认。
public void setFailover( boolean failover )设置容错开关
设置为TRUE，当当前socket不可用时，程序会自动查找可用连接并返回，否则返回NULL，默认状态是true，建议保持默认。
public void setHashingAlg( int alg )设置hash算法alg=0 使用String.hashCode()获得hash code,该方法依赖JDK，可能和其他客户端不兼容，建议不使用
alg=1 使用original 兼容hash算法，兼容其他客户端
alg=2 使用CRC32兼容hash算法，兼容其他客户端，性能优于original算法
alg=3 使用MD5 hash算法
采用前三种hash算法的时候，查找cache服务器使用余数方法。采用最后一种hash算法查找cache服务时使用consistent方法。
public void initialize()设置完pool参数后最后调用该方法，启动pool。

### MemCachedClient?
public void setCompressEnable( boolean compressEnable )设定是否压缩放入cache中的数据
默认值是ture
如果设定该值为true，需要设定CompressThreshold?
public void setCompressThreshold( long compressThreshold )设定需要压缩的cache数据的阈值
默认值是30k
public void setPrimitiveAsString( boolean primitiveAsString )设置cache数据的原始类型是String
默认值是false
只有在确定cache的数据类型是string的情况下才设为true，这样可以加快处理速度。
public void setDefaultEncoding( String defaultEncoding )当primitiveAsString为true时使用的编码转化格式
默认值是utf-8
如果确认主要写入数据是中文等非ASCII编码字符，建议采用GBK等更短的编码格式
cache数据写入操作方法set方法将数据保存到cache服务器，如果保存成功则返回true
如果cache服务器存在同样的key，则替换之
set有5个重载方法，key和value是必须的参数，还有过期时间，hash码，value是否字符串三个可选参数
add方法将数据添加到cache服务器,如果保存成功则返回true
如果cache服务器存在同样key，则返回false
add有4个重载方法，key和value是必须的参数，还有过期时间，hash码两个可选参数
replace方法将数据替换cache服务器中相同的key,如果保存成功则返回true
如果cache服务器不存在同样key，则返回false
replace有4个重载方法，key和value是必须的参数，还有过期时间，hash码两个可选参数
建议分析key的规律，如果呈现某种规律有序，则自己构造hash码，提高存储效率
cache数据读取操作方法使用get方法从cache服务器获取一个数据如果写入时是压缩的或序列化的，则get的返回会自动解压缩及反序列化
get方法有3个重载方法，key是必须的参数，hash码和value是否字符串是可选参数
使用getMulti方法从cache服务器获取一组数据get方法的数组实现，输入参数keys是一个key数组
返回是一个map

通过cache使用计数器使用storeCounter方法初始化一个计数器
使用incr方法对计数器增量操作
使用decr对计数器减量操作

## Memcached Client API 优化(草)
### 实现memcached的遍历操作
有些应用情况下，需要遍历memcached服务器中所有被cache的数据，目前memcached client API不支持遍历操作，需要进行扩展。
### 实现get时刷新数据过期时间（应用于session，可能需要修改服务器端程序）
当memcached被用作session服务器的时候，需要支持session的access方法，根据最近访问时间刷新过期时间，目前memcached也不支持该操作，需要进行扩展。

