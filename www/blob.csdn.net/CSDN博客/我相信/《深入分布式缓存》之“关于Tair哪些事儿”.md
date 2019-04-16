# 《深入分布式缓存》之“关于Tair哪些事儿” - 我相信...... - CSDN博客





2018年01月25日 10:45:21[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：2336
所属专栏：[深入分布式缓存](https://blog.csdn.net/column/details/19098.html)









Tair（TaoBao Pair的意思，Pair即Key-Value数据对）是淘宝开发的一个优秀的分布式高可用的 key/value 存储引擎。采用服务端自动负载均衡的方式，使客户端逻辑简单。

Tair 分为持久化和非持久化两种使用方式。非持久化的 Tair 可以看成是一个分布式缓存。持久化的 Tair 将数据存放于磁盘中。在最新版本的Tair项目中实现了以下4种存储引擎。

q  非持久化：mdb

q  持久化：fdb、kdb和 ldb

这4种存储引擎分别基于四种开源的key/value数据库：Memcached、Firebird、Kyoto Cabinet和LevelDB。其中Firebird是关系型存储数据库，Memcached、Kyoto Cabinet和LevelDB是Nosql数据库。

## Tair总体架构

Tair的总体架构图如图9-1所示。

![](https://img-blog.csdn.net/20180125104135452?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图9-1 Tair的总体架构图



一个Tair集群主要包括client、Config server和Dataserver 三个不同的应用。Configserver通过和Data server的心跳（HeartBeat）维护集群中可用的节点，并根据可用的节点，构建数据的在集群中的分布信息（见下文的对照表）。Client在初始化时，从Config server处获取数据的分布信息，根据分布信息和相应的Data server交互完成用户的请求。Data server负责数据的存储，并按照Config server的指示完成数据的复制和迁移工作。

## Config server简介

Tair的Configserver维护了集群内可用Data server的信息，Data server的信息，以及用户配置的桶数量、副本数、机房信息等，构建数据分布的对照表，以达到负载均衡和高可用的目标。Tair Config server和client相互配合根据对照表决定数据的具体分布。如果Data server宕机或扩容，Config server负责协调数据迁移、管理进度，将数据迁移到负载较小的节点上。

Tair 客户端和Configserver的交互主要是为了获取数据分布的对照表，客户端从Config server拿到对照表后，会在本地缓存对照表，在需要存储/获取数据时根据对照表查找数据在哪个Data server上。由此也可以看出，数据访问请求不需要和Config server交互，所以Config server本身的性能高低并不会形成集群的瓶颈。

Config server维护的对照表有版本概念，由于集群变动或管理触发，构建新的对照表后，对照表的版本号递增，并通过Data server的心跳，将新表同步给数据节点。

客户端和Data server交互时，Dataserver每次都把自己缓存的对照表版本号放入response结构中，返回给客户端，客户端将Data server的对照表版本号和自己缓存的对照表版本号比较，如果不相同，会主动和Config server通信，请求新的对照表。

Tair的Configserver使客户端使用时候，不需要配置数据节点列表，也不需要处理节点的的状态变化，这使得Tair对最终用户来说使用和配置都很简单。

Config server源代码目录下主要有下面几个cpp文件：

l  **tair_cfg_svr.cpp、server_conf_thread.cpp**

Config server的主文件，tair_cfg_svr被执行后，会检查参数和配置，然后启动几个主要的线程：

q  task_queue_thread，处理请求的具体线程。

q  packet_transport，发送和接收命令数据包的线程，其中引用了tbnet公共包处理epoll。

q  server_conf_thread，Config server的主要业务逻辑实现线程。包括Config server之间的心跳保持，根据心跳维持Data server存活列表，维护对照表，数据迁移过程管理等逻辑。

q  heartbeat_transport，发送和接收心跳数据包的线程，其中引用了tbnet公共包处理epoll。



l  **conf_server_table_manager.cpp**

管理对照表的辅助类，提供对照表持久化、取得一些元信息等功能，还提供打印对照表的功能，方便调试。



l  **table_builder.cpp、table_builder1.cpp、table_builder2.cpp**

构造对照表的实际逻辑，其中：

q  table_builder，是基类，定义了构造对照表的主体逻辑，其中有几个虚函数：rebuild_table、set_available_server、is_this_node_OK、caculate_capable、get_tokens_per_node用于不同的构造实现扩展不同的逻辑。

q  table_builder1，构建负载均衡策略对照表的实现类，继承了table_builder类，对几个虚函数进行了基于负载均衡优先的逻辑实现。

q  table_builder1，构建多数据中心策略对照表的实现类，继承了table_builder类，对几个虚函数进行了基于位置和负载均衡双因子的逻辑实现。

实际的对照表构建过程由server_conf_thread::table_builder_thread::build_table触发的。



l  **group_info.cpp**

group_info负责处理group.conf和持久化文件$TAIR_DATA_DIR/data/group_1_server_table，通过读取配置和持久化的信息，构建Data server位置信息，供Config server主逻辑使用。



l  **server_info**

记录Data server存活信息的主要数据结构，server_info由下面几个部分组成：

q  serverid：Data server在集群里的唯一标识，由ip和port构成。

q  last_time：记录最后一次接收到该Data server心跳时间。

q  status：表示该Data server的状态，有三种状态，参考下面的代码段：


enum

      {

        ALIVE = 0,

        DOWN,

        FORCE_DOWN,

      };


server_info会被持久化到$TAIR_DATA_DIR/data/server_info.0中。



l  **server_info_file_mapper.cpp、server_info_allocator.cpp**

实现了server_info持久化逻辑。

持久化该文件存放在$TAIR_DATA_DIR/data目录下，参考图9-2

![](https://img-blog.csdn.net/20180125104223633?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图 9-2** data目录

server_info_allocator维护了server_info持久化化文件集合和其中包含的server_info数量。如果当前文件没有空间来存储新的server_info，那么就新建一个序列化文件。

l  **stat_info_detail.cpp**

存储统计信息。主要的数据结构vector<u64>data_holder，包含GETCOUNT，PUTCOUNT，EVICTCOUNT，REMOVECOUNT，HITCOUNT，DATASIZE，USESIZE，ITEMCOUNT。





## Data server简介

Data server负责数据的物理存储，并根据Configserver构建的对照表完成数据的复制和迁移工作。Data server具备抽象的存储引擎层，可以很方便地添加新存储引擎。Data server还有一个插件容器，可以动态地加载/卸载插件。

![](https://img-blog.csdn.net/20180125104258411?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**图 9-3** Data server逻辑架构

##### 1）抽象的存储引擎层

Tair的存储引擎有一个抽象层，只要满足存储引擎需要的接口，便可以很方便地替换Tair底层的存储引擎。比如可以将bdb、tc甚至MySQL作为Tair的存储引擎，而同时使用Tair的分布方式、同步等特性。

Tair默认包含两个存储引擎：mdb和fdb。

mdb是一个高效的缓存存储引擎，它有着和memcached类似的内存管理方式。mdb支持使用share memory，这使得我们在重启Tair数据节点的进程时不会导致数据的丢失，从而使升级对应用来说更平滑，不会导致命中率的较大波动。

fdb是一个简单高效的持久化存储引擎，使用树的方式根据数据key的hash值索引数据，加快查找速度。索引文件和数据文件分离，尽量保持索引文件在内存中，以便减小IO开销。使用空闲空间池管理被删除的空间。

##### 2）自动的复制和迁移

为了增强数据的安全性，Tair支持配置数据的备份数。比如你可以配置备份数为3，则每个数据都会写在不同的3台机器上。得益于抽象的存储引擎层，无论是作为cache的mdb，还是持久化的fdb，都支持可配的备份数。

当数据写入一个Data server（主节点）后，主节点会根据对照表自动将数据写入到其他备份节点，整个过程对用户是透明的。

当有新Data server加入或者有Dataserver不可用时，Config server会根据当前可用的Data server列表，重新构建对照表。Data server获取到新的对照表时，会自动将在新表中不由自己负责的数据迁移到对照表中相应的Data server。迁移完成后，客户端可以从Config server同步到新的对照表，完成扩容或者容灾过程。整个过程对用户是透明的，服务不中断。

##### 3）插件容器

Tair还内置了一个插件容器，可以支持热插拔插件。

插件由Config server配置，Configserver会将插件配置同步给各个数据节点，数据节点会负责加载/卸载相应的插件。

插件分为request和response两类，可以分别在request和response时执行相应的操作，比如在put前检查用户的quota信息等。

插件容器也让Tair在功能方便具有更好的灵活性。



Tair的Data server最主要的组成模块有tair_server、request_processor、tair_manager、storage_manager和各种存储的具体实现实现。

Data server主要介绍下面几个cpp文件：



##### 1）tair_server.cpp

Data server的主要应用逻辑在这个模块里实现。

在tair_server::start函数中，启动了几个线程：

task_queue_thread.start();

duplicate_task_queue_thread.start();

heartbeat.start();



tbnet::transport提供了epoll处理支持，包括监听端口、连接复用等。

task_queue_thread，用于处理客户端的请求包，例如，put、get、range等命令的请求包。，duplicate_task_queue_thread，用于处理其他Data server发送来的数据复制相关的请求包。request_processor，处理由task_queue和duplicate_task_queue两个线程解包出来的命令，实现put、get、range等命令的逻辑，最终的处理由重载的request_processor:: process函数，根据不同的类进行处理。

heartbeat用于处理心跳数据，数据包的处理由heartbeat_thread:: handlePacket进行处理。



##### 2）request_processor.cpp

通过重载的process函数，处理put、get、range等请求。request_processor定义了每种请求的最高层执行流程，每个请求的大体流程都相似，如图9-4：

![](https://img-blog.csdn.net/20180125104334782?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**图 9-4 **request_processor处理流程

如上图，tair接收到请求后，会循环处理每一个key，如果key在迁移，会发送数据迁移的响应给客户端，客户端重新获取数据分布后，到新的data_server操作相应的数据。

处理过程中会调用性能监控工具，统计相应的性能数据。



##### 3）tair_manager.cpp

业务流程的第二层定义，主要定义了各个命令的主要处理流程。例如：

get的流程，确定bucket ->调用storage_mgr获取数据 -> 返回数据。

put的流程，将area合并到key里 -> 调用storage_mgr-> 通过hash_table确定怎样复制数据。



##### 4）storage_manager.cpp

存储引擎的接口，用于接入不同的底层存储。不同的存储引擎支持的主要功能（还有一些辅助功能没有列出）如表9-1。

**表9-1 四种引擎对比**
|**storage_mgr接口**|**是否必选**|**mdb**|**ldb**|**fdb**|**kdb**|
|----|----|----|----|----|----|
|put|是|√|√|√|√|
|batch_put|否|×|√|×|×|
|get|是|√|√|√|√|
|remove|是|√|√|√|√|
|add_count|否|√|×|×|×|
|get_range|否|×|√|×|×|
|del_range|否|×|√|×|×|
|clear|是|√|√|√|√|

说明：

q  第一列是storage_manager定义的常用操作接口。

q  第二列是虚函数是否有默认实现，一般情况下默认的实现函数是返回一个操作不支持的消息。

q  其余列是各个存储引擎对主要功能的支持，没有支持的，会调用默认的实现函数返回该操作不支持。


本文摘自**《深入分布式缓存：从原理到实践》**


新书推荐：**《深入分布式缓存》**

![](https://img-blog.csdn.net/20180125104400165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







京东购书，扫描二维码：

![](https://img-blog.csdn.net/20180125104443319?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



