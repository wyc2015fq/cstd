# ElasticSearch的Gateway及存储原理 - BlueSky - CSDN博客
2016年05月19日 13:12:58[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：2786
转自：http://log.medcl.net/item/2010/10/elasticsearch-the-gateway-and-storage/
ES里有一个叫做gateway的东西，今天抽空理一下，前面翻译ES的一篇博文叫做“[搜索引擎与时间机器](http://log.medcl.net/item/2010/09/translation-search-engine-and-the-time-machine/)”，既然里面谈到了时间机器，就免不了需要穿越时空的的门咯，I
 guess，也许gateway这名字就是这么来的，其实在ES里面，gateway的主要职责是用来对数据进行长持久化(持久化了之后加上版本信息，不就可以自由的往还于过去未来之间了吗？)，另外，整个集群重启之后可以通过gateway重新恢复数据。
由于ElasticSearch是专为分布式环境而设计的，所以怎么去对所有节点的索引信息进行持久化是个问题，除了索引信息之外，还有ClusterState(集群信息)，mapping，索引碎片信息，以及transaction log等信息也是需要进行持久化的，0.11之后的版本新加了LocalGateway（并且是默认配置），加上之前的fs（使用共享文件系统）、hdfs（hadoop 分布式文件系统）、cloud（ec2之类的云存储），0.11之前默认的是None，也就是不进行持久化，这样的话如果服务器都挂了，就会造成数据丢失，local
 gateway这种模式顾名思义就是节点各自保存其状态，节点直接从本地存储来恢复节点状态和索引信息，localgateway有如下几个选项：
gateway.recover_after_nodes: 1 (or gateway.recovery_after_master_nodes)
 
gateway.recover_after_time: 5m (m分钟、s秒)
 
gateway.expected_nodes: 2
当你的节点出现故障或者集群重启的时候，这些配置很重要，你应该尽量保证有了足够多的节点来执行故障恢复，确保得到最终的集群状态，可能存放最新版本的状态信息的节点还未起来，否则可能会丢失数据。
至于索引碎片的恢复则只需用达到足够恢复的数目即可，如果你没有设置备份，或者刚好某个碎片偏偏都丢了，那估计只能是没戏咯，
另外，在我的测试中，2台机器，其中一台网络不稳定，经常断开，（ip冲突造成），造成集群中的节点各种为战，这里有一些参数可配：
discovery.zen.fd.connect_on_network_disconnect : true
 
discovery.zen.initial_ping_timeout : 10s
 
discovery.zen.fd.ping_interval : 2s
 
discovery.zen.fd.ping_retries  : 10
如果使用是共享文件系统的gateway，默认是每隔10s将数据异步的复制到共享的文件系统中（这是ES里一个叫gateway snapshot的概念），想想如果集群很大，共享文件系统就需要保存很大的数据量，并且维护以及设备本身成本都很高，而使用local方式则可以做到各个节点自治，确实很方便啊。
有关存储
说了这么半天，还有一个问题就是es工作的存储目录，具体来说就是es服务的索引工作目录，gateway只是做持久化和恢复，不会直接使用，所以还有一个工作目录是用来查询操作的，具体来说就是通过[Store](http://www.elasticsearch.com/docs/elasticsearch/index_modules/store/)模块来完成的（es的每个功能都是一个模块，结构非常好），es的存储设置是per
 index级别的，所以你可以为每个index指定它存储的位置，既然持久化的工作已经完毕了，本地操作的索引一般只放在两个地方：内存和本地文件系统，主要是性能上考虑的，如果内存很大，那就全部放内存咯，值得一提的是，在ES里，所有存放work目录下（或内存）的东西假定能被丢失的和临时性的，另外你有多个副本存放在不同的节点上，每个索引又拆成了多个碎片位于不同的节点上，假设一台节点上的数据丢了，那也只是整个索引少了其中的一个碎片的一个副本而已，并且es会马上进行碎片的重新分配（allocate），确保数据的完整。
还有一个重要特点就是es可以直接使用jvmheap之外的内存。
下面是一个配置inex使用内存来进行存储的例子（[更多配置细节](http://www.elasticsearch.com/docs/elasticsearch/index_modules/store/)）：
index :
    store:
        fs:
            memory:
                enabled**: **true
还有一个就是关于索引碎片数和副本数的配置问题，你不能够对已经存在的索引修改碎片数，也就是有了数据之后就不能修改了,因为目前es只是简单的哈希取模[hash(_id) MOD number_of_shards]，如果以后支持一致性哈希，那就很好了，不过副本数目是可以随时动态更改的，如果你要对全局进行设置，参照如下：
gateway:
    type**: **local
index:
    gateawy:
        snapshot_interval**: **30s
    number_of_shards**: **3
    number_of_replicas**: **2
path:
    logs**: **/path/to/logs
新创建一个索引并设置：
PUT http://localhost:9200/myindex/
{
 "index" : {
 "number_of_replicas" :3,
"number_of_shards" :7
 }
 }
动态设置索引副本数量:
PUT: http://localhost:9200/myindex/_settings
{
 "index" : {
 "numberOfReplicas" :2
 }
 }
