# Redis集群搭建1 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年12月21日 17:25:31[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：379
wget [http://download.redis.io/releases/redis-3.2.0.tar.gz](http://download.redis.io/releases/redis-3.2.0.tar.gz)
cd redis-3.2.0
make && make installshell> ln -s /usr/local/bin/redis* /usr/bin/shell> cp src/redis-trib.rb /usr/bin/
执行./utils/install_server.sh脚本分别各安装redis实例(如果对安装路径没有特殊要求的话，在安装时只需改变端口号，其它都保持默认即可。)
测试时用6个：6379，6380，6381，16379，16380，16381
Redis服务安装完成之后，服务会同时启动，且会自动加入到系统服务中，并设为开机启动。
Redis默认绑定的是127.0.0.0地址，需要将其修改为本机IP或0.0.0.0，集群中的各个节点才能互相通信。
sed -i 's/127.0.0.1/0.0.0.0/g' /etc/redis/6379.conf
sed -i 's/127.0.0.1/0.0.0.0/g' /etc/redis/6380.conf
。。。
**修改端口号（**有5处修改端口的地方**）**
sed -i 's/6379/6380/g' /etc/redis/6380.conf
sed -i 's/6379/6381/g' /etc/redis/6381.conf
sed -i 's/6379/16379/g' /etc/redis/16379.conf
sed -i 's/6379/16380/g' /etc/redis/16380.confsed -i 's/6379/16381/g' /etc/redis/16381.conf
修改集群配置
cluster-enabled yes cluster-config-file nodes-6379.conf cluster-node-timeout 5000 appendonly yes
其中：
cluster-enabled：开启集群模式 
cluster-config-file：保存节点的配置信息，如集群中所有节点的IP、端口、状态、节点类型（master/slave)、节点ID、slots等 
cluster-node-timeout：节点心跳超时时长 
appendonly：开启aof文件存储
依次将每个实例配置文件中的以上注释打开，并修改成对应的值。
此时通过PS命令查看redis进程，和普通进程不同的是在进程名后边加了一个[cluster]标识。
现在都还是独立的实例，还没有分配到一个集群当中。没有master和slave关系。
创建Redis集群：
创建集群要用到ruby的一个脚本，在创建集群前，先安装ruby的运行环境和ruby的redis客户端。
shell> yum -y install ruby rubygems
shell> gem sources --add https://ruby.taobao.org/ --remove https://rubygems.org/ # 配置taobao的源
shell> gem install redis --version 3.2.0
创建集群
shell>
redis-trib.rb
create--replicas 1
192.168.0.201:6379
 192.168.0.201:6380 192.168.0.201:6381
192.168.0.202:16379
 192.168.0.202:16380 192.168.0.202:16381
参数说明： 
1> redis-trib.rb脚本默认安装在$REDIS_SRC_HOME/src目录下，在安装redis服务时，已将其拷贝到/usr/bin目录下 
2> create：创建集群参数 
3> –replicas 1：每个master有1个slave节点 
根据节点顺序确定master与slave节点，前边的优先选择做为master节点，后边优先选择做为slave节点。由脚本自动分配，但master和slave绝对不会分配在同一台机器上。
检查Redis集群状态
redis-trib.rb check 192.168.0.201:6379
从上图可以看出，192.168.0.201:6379、192.168.0.201:6380、192.168.0.201:6381被选为了master节点。192.168.0.202:16379被选为192.168.0.201:6381的slave节点，192.168.0.202:16380被选为192.168.0.201:6379的slave节点，192.168.0.202:16381被选为192.168.0.201:6380的slave节点（看repliates
 后面的字符串）。
验证Redis集群
# 连接到201的6379节点shell> redis-cli -c -h 192.168.0.201 -p 6379192.168.0.201:6379> set foo abc-> Redirected to slot [12182] located at 192.168.0.201:6380OK192.168.0.201:6380>
set foo abc本想在201的6379节点保存foo这个key，但201的6379节点并没有存，而是将请求转发给了201的6380节点存储。而且客户端自动切换到了6380节点。这是神马情况？这其实是redis集群数据存储的机制，通过crc16[**算法**](http://lib.csdn.net/base/datastructure)计算key的hash值，然后对16384取模，得到一个0~16384以内的slot值来决定由哪个节点存储，而每一个节点在创建集群的时候，都会均匀的分配相应的slot数量。
其中 -c Enable cluster mode
2，192.168.0.202:16381被选为192.168.0.201:6380的slave节点，现在把6380 master节点kill掉，看redis会不会将16381选主为master节点，并将刚刚保存的foo这个key有没有复制到16381节点中。
结果：当master节点挂掉之后，slave节点自动选举成了master节点，并且之前保存在master节点的数据foo也成功复制给了slave节点。


管理：添加节点、删除节点、slot分配
添加节点
添加一对（一主一从）新的节点加入到集群当中：192.168.0.201:6382（Master）和192.168.0.201:6383（Slave）
**添加master节点**
redis-trib.rb add-node 192.168.0.201:6382
192.168.0.201:6379
参数说明： 
**192.168.0.201:6382：新节点**
192.168.0.201:6379：可以是集群中的任意一个节点（这里以201:6379为例）
注意： 
1> 新的节点必须开启了集群配置 
2> 新的节点必须已启动 
如果不同时满足以上两点，将无法加入到集群中。
**为master节点分配slot**
新加入的master节点默认是不会分配slot的。所以我们需要手动为新加入的节点分配一定范围的slot，以便集群在存取数据的时候，能够找到这个新加入的节点。
redis-cli cluster nodes901cade1bc2703ef2bb4ff49466b38762346755d
192.168.0.201:6382
 master - 014683244221100 connected06023f8283389ad5580c782797f49a5996c108b3
192.168.0.201:6379
 myself,master - 001 connected
0-5460...
- 
sloat范围显示在节点状态（connected）后边，从上可以看出，新加入的节点201:6382是没有分配slot的，所以为空。而为201:6379节点分配了0-5460范围的slot。 
为新节点分配slot的原理是从其它主节点中负责的slot中，各移出一部份 
slot给新的节点。下面为201:6382分配slot：
shell>
redis-trib.rbreshard 192.168.0.201:6382
注意上图中**红框**部份： 
1> 1000 ： 为新的master节点分配1000个slot 
2> 901cade1bc2703ef2bb4ff49466b38762346755d：接收1000个slot的master节点ID 
3> 选择从哪些主节点移动slot到新加入的主节点中： 
* all：表示从集群中所有master节点中各移动一部份slot，凑够1000个slot为止 
* 选择输入一个或多个master的node_id来从中移动，最后输入done完成选择 
4> yes：执行slot移动操作，输入no表示终止操作。 
**添加slave节点**
shell> redis-trib.rb
add-node
--slave --master-id 901cade1bc2703ef2bb4ff49466b38762346755d 192.168.0.201:6383 192.168.0.201:6379
参数说明： 
1> –slave：表示添加从节点 
2> –master-id：master节点ID，这里用上边新添加的主节点ID（201:6382） 
3> 192.168.0.201:6383：新slave节点的IP和端口 
4> 192.168.0.201:6379：集群中任意旧节点，只要能连上就可以。这里以201:6379为例
cluster nodes命令说明
459dbcccc1e5be021f19efde794b9e84474c0c02
 192.168.0.202:16379slaveb6814c717080de33907e6f769e1fd8dc4fe53c38 0 1468333841661 7
connected
1> 459dbcccc1e5be021f19efde794b9e84474c0c02 ：节点ID 
2> 192.168.0.202:16379：节点IP和端口 
3> slave：节点类型，master、slave或myself(master/slave) 
4> b6814c717080de33907e6f769e1fd8dc4fe53c38 ：master节点ID，如果自己是master节点，这一列的值为0 
5> 0：集群最近一次向节点发送 PING 命令之后， 过去了多长时间还没接到回复。 
6> 1468333841661：节点最近一次返回 PONG 回复的时间 
7> 7：节点的配置纪元 
8> connected：节点网络连接状态 
9> 如果是主节点，节点状态后边显示slot分配的范围
[Redis集群技术架构](http://blog.csdn.net/xyang81/article/details/51898465)
- Redis Cluster采用无中心架构，每个节点都保存数据和整个集群的节点状态
- 每个节点都和集群中的其它节点保持连接
- 每个Master Node都负责分管一定范围的slot，当这些slot对应的Master和Slave都挂掉时，这部份slot对应的数据将不可用
- 16384个slot必须被全部分配，否则集群将不可用。通过cluster info查看集群状态可以得知。
参考：
[http://blog.csdn.net/xyang81/article/details/51881900](http://blog.csdn.net/xyang81/article/details/51881900)
[http://blog.csdn.net/xyang81/article/details/51895011](http://blog.csdn.net/xyang81/article/details/51895011)
