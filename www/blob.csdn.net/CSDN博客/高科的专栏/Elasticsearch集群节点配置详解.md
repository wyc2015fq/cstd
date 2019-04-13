
# Elasticsearch集群节点配置详解 - 高科的专栏 - CSDN博客

2018年11月14日 15:29:32[高科](https://me.csdn.net/pbymw8iwm)阅读数：66标签：[elasticsearch																](https://so.csdn.net/so/search/s.do?q=elasticsearch&t=blog)个人分类：[elasticsearch																](https://blog.csdn.net/pbymw8iwm/article/category/8347998)


注意：如果是在局域网中运行elasticsearch集群也是很简单的，只要cluster.name设置一致，并且机器在同一网段下，启动的es会自动发现对方，组成集群。
三、配置浅涉
elasticsearch的config文件夹里面有两个配置文件：elasticsearch.yml和logging.yml，第一个是es的基本配置文件，第二个是日志配置文件，es也是使用log4j来记录日志的，所以logging.yml里的设置按普通log4j配置文件来设置就行了。下面主要讲解下elasticsearch.yml这个文件中可配置的东西。
cluster.name:elasticsearch
配置es的集群名称，默认是elasticsearch，es会自动发现在同一网段下的es，如果在同一网段下有多个集群，就可以用这个属性来区分不同的集群。
node.name:"FranzKafka"
节点名，默认随机指定一个name列表中名字，该列表在es的jar包中config文件夹里name.txt文件中，其中有很多作者添加的有趣名字。
node.master:true
指定该节点是否有资格被选举成为node，默认是true，es是默认集群中的第一台机器为master，如果这台机挂了就会重新选举master。
node.data:true
指定该节点是否存储索引数据，默认为true。
index.number_of_shards:5
设置默认索引分片个数，默认为5片。
index.number_of_replicas:1
设置默认索引副本个数，默认为1个副本。
path.conf:/path/to/conf
设置配置文件的存储路径，默认是es根目录下的config文件夹。
path.data:/path/to/data
设置索引数据的存储路径，默认是es根目录下的data文件夹，可以设置多个存储路径，用逗号隔开，例：
path.data:/path/to/data1,/path/to/data2
path.work:/path/to/work
设置临时文件的存储路径，默认是es根目录下的work文件夹。
path.logs:/path/to/logs
设置日志文件的存储路径，默认是es根目录下的logs文件夹
path.plugins:/path/to/plugins
设置插件的存放路径，默认是es根目录下的plugins文件夹
bootstrap.mlockall:true
设置为true来锁住内存。因为当jvm开始swapping时es的效率会降低，所以要保证它不swap，可以把ES_MIN_MEM和ES_MAX_MEM两个环境变量设置成同一个值，并且保证机器有足够的内存分配给es。同时也要允许elasticsearch的进程可以锁住内存，[Linux](http://lib.csdn.net/base/linux)下可以通过`ulimit-l unlimited`命令。
network.bind_host:192.168.0.1
设置绑定的ip地址，可以是ipv4或ipv6的，默认为0.0.0.0。
network.publish_host:192.168.0.1
设置其它节点和该节点交互的ip地址，如果不设置它会自动判断，值必须是个真实的ip地址。
network.host:192.168.0.1
这个参数是用来同时设置bind_host和publish_host上面两个参数。
transport.tcp.port:9300
设置节点间交互的tcp端口，默认是9300。
transport.tcp.compress:true
设置是否压缩tcp传输时的数据，默认为false，不压缩。
http.port:9200
设置对外服务的http端口，默认为9200。
http.max_content_length:100mb
设置内容的最大容量，默认100mb
http.enabled:false
是否使用http协议对外提供服务，默认为true，开启。
gateway.type:local
gateway的类型，默认为local即为本地文件系统，可以设置为本地文件系统，分布式文件系统，[Hadoop](http://lib.csdn.net/base/hadoop)的HDFS，和amazon的s3服务器，其它文件系统的设置方法下次再详细说。
gateway.recover_after_nodes:1
设置集群中N个节点启动时进行数据恢复，默认为1。
gateway.recover_after_time:5m
设置初始化数据恢复进程的超时时间，默认是5分钟。
gateway.expected_nodes:2
设置这个集群中节点的数量，默认为2，一旦这N个节点启动，就会立即进行数据恢复。
cluster.routing.allocation.node_initial_primaries_recoveries:4
初始化数据恢复时，并发恢复线程的个数，默认为4。
cluster.routing.allocation.node_concurrent_recoveries:2
添加删除节点或负载均衡时并发恢复线程的个数，默认为4。
indices.recovery.max_size_per_sec:0
设置数据恢复时限制的带宽，如入100mb，默认为0，即无限制。
indices.recovery.concurrent_streams:5
设置这个参数来限制从其它分片恢复数据时最大同时打开并发流的个数，默认为5。
discovery.zen.minimum_master_nodes:1
设置这个参数来保证集群中的节点可以知道其它N个有master资格的节点。默认为1，对于大的集群来说，可以设置大一点的值（2-4）
discovery.zen.ping.timeout:3s
设置集群中自动发现其它节点时ping连接超时时间，默认为3秒，对于比较差的网络环境可以高点的值来防止自动发现时出错。
discovery.zen.ping.multicast.enabled:false
设置是否打开多播发现节点，默认是true。
discovery.zen.ping.unicast.hosts:["host1", "host2:port","host3[portX-portY]"]
设置集群中master节点的初始列表，可以通过这些节点来自动发现新加入集群的节点
四、集群配置多个节点
1.概述
本例子的集群将部署4个节点：
10.0.0.11 es1
10.0.0.209 es2
10.0.0.206 es3
10.0.0.208 es4
2.集群配置
上文提到过，只要集群名相同，且机器处于同一局域网同一网段，es会自动去发现其他的节点。
2.1es2的配置
vimES_HOME/config/elasticsearch.yml
在文件尾部添加一下内容：
cluster.name:elasticsearch \#集群的名称，同一个集群该值必须设置成相同的
node.name:"es2" \#该节点的名字
node.master:true \#该节点有机会成为master节点
node.data:true \#该节点可以存储数据
node.rack:rack2 \#该节点所属的机架
index.number_of_shards:5 \#shard的数目
index.number_of_replicas:3 \#数据副本的数目
network.bind_host:0.0.0.0 \#设置绑定的IP地址，可以是IPV4或者IPV6
network.publish_host:10.0.0.209 \#设置其他节点与该节点交互的IP地址
network.host:10.0.0.209 \#该参数用于同时设置bind_host和publish_host
transport.tcp.port:9300 \#设置节点之间交互的端口号
transport.tcp.compress:true \#设置是否压缩tcp上交互传输的数据
http.port:9200 \#设置对外服务的http端口号
http.max_content_length:100mb \#设置http内容的最大大小
http.enabled:true \#是否开启http服务对外提供服务
discovery.zen.minimum_master_nodes:2 \#设置这个参数来保证集群中的节点可以知道其它N个有master资格的节点。默认为1，对于大的集群来说，可以设置大一点的值（2-4）
discovery.zen.ping.timeout:120s \#设置集群中自动发现其他节点时ping连接的超时时间
discovery.zen.ping.multicast.enabled:true \#设置是否打开多播发现节点
discovery.zen.ping.unicast.hosts:["10.0.0.209:9300","10.0.0.206:9300","10.0.0.208:9300"] \#设置集群中的Master节点的初始列表，可以通过这些节点来自动发现其他新加入集群的节点
2.2es3的配置
相似地，在206的机子上
vimES_HOME/config/elasticsearch.yml
在文件尾部添加一下内容：
cluster.name:elasticsearch \#集群的名称，同一个集群该值必须设置成相同的
node.name:"es3" \#该节点的名字
node.master:true \#该节点有机会成为master节点
node.data:true \#该节点可以存储数据
node.rack:rack3 \#该节点所属的机架
index.number_of_shards:5 \#shard的数目
index.number_of_replicas:3 \#数据副本的数目
network.bind_host:0.0.0.0 \#设置绑定的IP地址，可以是IPV4或者IPV6
network.publish_host:10.0.0.206 \#设置其他节点与该节点交互的IP地址
network.host:10.0.0.206 \#该参数用于同时设置bind_host和publish_host
transport.tcp.port:9300 \#设置节点之间交互的端口号
transport.tcp.compress:true \#设置是否压缩tcp上交互传输的数据
http.port:9200 \#设置对外服务的http端口号
http.max_content_length:100mb \#设置http内容的最大大小
http.enabled:true \#是否开启http服务对外提供服务
discovery.zen.minimum_master_nodes:2 \#设置这个参数来保证集群中的节点可以知道其它N个有master资格的节点。默认为1，对于大的集群来说，可以设置大一点的值（2-4）
discovery.zen.ping.timeout:120s \#设置集群中自动发现其他节点时ping连接的超时时间
discovery.zen.ping.multicast.enabled:true \#设置是否打开多播发现节点
discovery.zen.ping.unicast.hosts:["10.0.0.209:9300","10.0.0.206:9300","10.0.0.208:9300"] \#设置集群中的Master节点的初始列表，可以通过这些节点来自动发现其他新加入集群的节点
2.3 208的机子上的es配置就参考上面两个节点的
2.4验证结果
启动节点：
ES_HOME/bin/service/elasticsearchstart
成功启动各个节点后，浏览器打开http://10.0.0.209:9200/_plugin/head/，界面会列出各个节点的信息。
3.节点添加和删除
3.1添加节点非常简单，几乎与上面设置节点的步骤一致。
在10.0.0.11机子上，vimES_HOME/config/elasticsearch.yml
cluster.name:elasticsearch \#集群的名称，同一个集群该值必须设置成相同的
node.name:"es5" \#该节点的名字
node.master:false \#该节点有机会成为master节点
node.data:true \#该节点可以存储数据
node.rack:rack5 \#该节点所属的机架
index.number_of_shards:5 \#shard的数目
index.number_of_replicas:3 \#数据副本的数目
network.bind_host:0.0.0.0 \#设置绑定的IP地址，可以是IPV4或者IPV6
network.publish_host:10.0.0.11 \#设置其他节点与该节点交互的IP地址
network.host:10.0.0.11 \#该参数用于同时设置bind_host和publish_host
transport.tcp.port:9300 \#设置节点之间交互的端口号
transport.tcp.compress:true \#设置是否压缩tcp上交互传输的数据
http.port:9200 \#设置对外服务的http端口号
http.max_content_length:100mb \#设置http内容的最大大小
http.enabled:true \#是否开启http服务对外提供服务
discovery.zen.minimum_master_nodes:2 \#设置这个参数来保证集群中的节点可以知道其它N个有master资格的节点。默认为1，对于大的集群来说，可以设置大一点的值（2-4）
discovery.zen.ping.timeout:120s \#设置集群中自动发现其他节点时ping连接的超时时间
discovery.zen.ping.multicast.enabled:true \#设置是否打开多播发现节点
discovery.zen.ping.unicast.hosts:["10.0.0.209:9300","10.0.0.206:9300","10.0.0.208:9300"] \#设置集群中的Master节点的初始列表，可以通过这些节点来自动发现其他新加入集群的节点
写好配置，启动此es节点。
查看集群的状态：
[http://1](http://0.0.0.1/)0.0.0.11:9200/_nodes
elasticsearch采用广播的方式自动发现节点，需要等待一段时间才能发现新的节点：
耐心等待...最后可以在此界面看到各个节点的信息。
3.2节点删除
在想要删除节点的机子上，运行ES_HOME/bin/service/elasticsearchstop，等待一会后，查看集群状态，会发现该节点没有了


