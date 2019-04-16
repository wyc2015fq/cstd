# NoSQL 与大数据 - 我相信...... - CSDN博客





2016年05月25日 22:17:59[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：30362
所属专栏：[大数据修炼道场](https://blog.csdn.net/column/details/12856.html)









概览一下大数据项目中可以使用的数据存储技术，聚焦于Couchbase 和 ElasticSearch，展示如何使用以及它们的区别，先理解一下NoSQL领域中各种不同的技术。

# NoSQL

关系型数据库是过去的选择，几乎是许多开发者和DBA对于传统三层架构应用的唯一选择。使用这一场景有很多原因，数据建模方法，查询语言与数据交互，保证数据的一致性部署，并能够为复杂的应用服务。 

然而，这不是解决所有数据存储问题的唯一方案，也是NoSQL 产生的原因。NoSQL 提供了新的方法而不是采用面向标准SQL的范式。 

NoSQL 技术与高伸缩性无缝融合，很多技术同时具备了高分布性和高性能。大多数时间里，它们使 现有RDBMS 技术所实现的架构更加完整，例如 作为缓存服务器，搜索引擎，非结构化存储，易变信息存储等。主要分为4类：- 
Key/value 

- 
列存储

- 
面向文档的存储

- 
图存储


现在深入到各种技术，选择最适用于使用场景的技术。

## Key/Value

第一个也是最早的 NoSQL 数据存储就是key/value. 这些数据就像字典一样根据key来匹配value，通常使用在需要高性能的基本信息存储，例如需要快速读写的session信息，这些存储在这样的情景非常高效，也通常具有高伸缩性。

Key/value也经常被用于上下文的队列化来保证数据不丢失，例如日志架构或搜索引擎的索引架构。Redis 和Riak KV 是非常有名的key/value数据存储； Redis 使用的更加广泛，因为它有着一个内存型 K/V 存储，并且持久化是可选的。Redis 经常用于web应用中来存储session相关的数据，例如node或者- 

PHP的 web应用 ; 每秒钟可以提取成千上万的session信息而没有性能损失。另一个典型场景是后面要讲到的序列化：Redis位于 Logstash 和 ElasticSearch 之间来存储t ElasticSearch 查询中的索引。

## Column

由于要存储超大量的记录信息 到达了key/value存储限制的时候就需要用到列存储。列存储技术对于RDBMS世界的工程师可能不太容易理解，但事实上非常简单。RDBMS 中数据是按行存储的，而列存储中是按列的。使用列数据库的主要好处是能高速访问海量数据。 RDBMS的一行在硬盘上是一个连续的存储，多行可能存储在硬盘不同的位置，使访问稍显复杂，在列数据库中的一列数据是连续存储的。

举个例子，考虑在RDBMS中查询索引博客的标题，尤其是有数百万数据的时候，需要大量的IO操作，而在列数据库中，这样的查询只是一次访问。这样的数据库在从特定簇提取海量数据中非常顺手，但此消彼长的是缺乏灵活性。使用最多的列存储数据库是 

Google Cloud Bigtable, 但开源的列存储数据库是Apache HBase 和Cassandra.  

列存储数据库的另一个好处是容易伸缩，这些列在海量存储时具有高伸缩性。这就是为什么它们主要用于保存非易变且长久保留信息的原因。
## Document

列存储数据库对于含有比较深嵌套结构的结构化数据的存储不是最好的，这种场景需要使用面向文档的数据存储。数据实际上以key/value 存储,但是所有压缩的数据叫做文档。 文档依赖于一个结构或者编码例如XML, 但更多时候是 JSON (JavaScript Object Notation).

尽管文档型数据库对于数据的结构化存储和表达都非常有用，但也有其脆弱的一面，特别是与数据的交互性操作。它们基本上要遍历整个文档，例如当读取某个特定字段的时候，遍历可能会影响性能。  

当需要存储嵌套信息的时候，可以采用文档型数据库。例如，考虑怎样表达应用中的一个账户，大概有以下信息：
- 
基础信息:姓名，生日，照片 ，URL, 创建日期等等

- 
复杂信息: 地址，认证方法(password, Facebook, 等第三方认证),兴趣等等。


这也是NoSQL 文档型数据库经常用到web应用的原因: 表达嵌套对象非常容易，由于都使用JSON，还可以与前端的JavaScript技术无缝集成。

使用最多的文档型数据库是MongoDB, Couchbase, 和 Apache CouchDB，都非常容易安装和启动，有很好的文档说明，而且都是可伸缩的，除此之外，它们也是开放现代web应用的明确选择。  

Couchbase 是这里架构中选择的技术，使用它来存储，查询和组织数据。原因是基于它的性能基准测试，它的高吞吐量操作时延低于MongoDB. 

还有一个值得关注的是，Couchbase 今天是 CouchDB 和Memcached的结合体。从支持的角度上看，使用CouchBase更显得有意义。

## Graph

Graph 数据库与其它数据库有着本质的区别。它使用了不同的范式来表达数据——树结构，节点和边连接起来叫做关系。这些数据库是随着社交网络而诞生的，例如表达用户的好友网络，他们的好友关系等等。对于其它类型的数据存储，可能把一个用户的好友关系存储在一个文档中，但是，存储好友关系还依然非常复杂；使用图数据库就非常简单，为每个好友创建节点，通过关系连接他们，依赖查询的需要和范围浏览图。

最著名的图数据库是Neo4j, 象前面所说的，主要使用场景是处理复杂的关系信息，例如实体间的连接，也可以用于分类的场景。 

Figure 2-1 展示了在图数据库中3个实体是如何连接的。

![F2-1 图数据库示例](https://img-blog.csdn.net/20160520085903201)

Figure 2-1. Graph database example  
图中的两天账户节点Jane 和 John, 它们之间的每一条边定义了他们的关系，在某天相互认识，另一组节点连接的两个账户展示了Jane 和 Joh在某天后都成为了足球组的成员。

# 使用场景中的NoSQL

根据使用场景，首先需要一个文档型的 NoSQL数据库，将存储在关系型数据库中的数据结构化的一个 JSON 文档. 如前所述，传统的RDBMSs 将数据存储到多个有关系的表，当得到一个完整对象时变得比较复杂和低效。在Figure 2-2. 中可以看到一个账户被分割成多个表的例子。

![F2-2 账户表](https://img-blog.csdn.net/20160520090225119)

Figure 2-2. Account tables
如果要获得所有的账户信息，基本上需要join两到三个表。现在考虑这样的情形: 需要处理所有用户在应用中的每一次连接，这些连接有着不同的商业逻辑。 最后,想要账户自身的视图。通过传递一个账户标识通过API从全部用户视图中得到一个怎样的文档呢？

```
{
    "id": "account_identifier",
    "email": "account@email.com",
    "firstname": "account_firstname",
    "lastname": "account_lastname",
    "birthdate": "account_birthdate",
    "authentication": [{
        "token": "authentication_token_1",
        "source": "authenticaton_source_1",
        "created": "12-12-12"

    }, {

        "token": "authentication_token_2",
        "source": "authenticaton_source_2",
        "created": "12-12-12"

    }],
    "address": [{
        "street": "address_street_1",
        "city": "address_city_1"
        "zip": "address_zip_1"
        "country": "address_country_1"
        "created": "12-12-12"
    }]

}
```

好处显而易见: 通过保持一个实体的 JSON 表达,可以更快更好的访问数据。进一步，将这一方法通用化，从NoSQL数据库读取所有的读操作，而让所有的写操作  (create, update,delete) 还在RDBMS上 .但必须实现一个逻辑来维持 RDBMS到NoSQL 的数据同步，如果没在缓存中的话还要创建一个关系型数据库的对象。

在NoSQL高效可伸缩地创建文档时为什么还要保持 RDBMS呢？因为这不是应用的真正目的。我不想产生一个Big Bang 的影响. 假设RDBMS已经准备好了，但因为RDBMS缺乏灵活性而集成了一个NoSQL存储。希望充分利用两个最好的技术 —特别是RDBMS的数据一致性和NoSQL的伸缩性 。  

除此之外，这是一个简单查询的例子，但是希望更进一步，例如文档中任意字段的全文检索。那么，在关系型数据库中如何做的呢？是索引，但是要索引表中所有的列么？实际上，这是不可能的，但可以轻松地使用NoSQL技术做这件事，例如ElasticSearch. 在深入一个NoSQL 缓存系统之前，先看一下如何使用 Couchbase 这一文档型数据库，然后回顾它的局限再切换到ElasticSearch。 

先看一下Couchbase这一可伸缩架构, 但因为Couchbase 的一些严重局限, 还要迁移前看一下拥有ElasticSearch的完整架构。

# Couchbase 介绍

Couchbase是一个开源的文档型数据库，有着灵活的数据模型，性能和伸缩性都适合一般的使用场景，将关系型数据库的数据存储到一个结构化的   JSON 文档.大多数NoSQL技术有着类似的架构—首先看 Couchbase 架构师如何组织的，然后介绍 Couchbase中命名约定, 再深入到存储数据是如何查询的，最后讨论跨数据中心的复制。

## 架构

Couchbase是一个真正的无共享架构，这意味着没有单点故障，因为集群中的每个节点都是自给自足而且独立的。分布式技术工作的方式如何？——各节点不共享任何内存或硬盘存储。文档以JSON或者二进制形式存储在Couchbase中,在集群间复制，所组织的单元叫buckets. 一个bucket 通过设置RAM缓存来根据存储和访问需求完成伸缩，还可以设置弹性副本的数量。 从底层看, bucket 被分割成叫做vBuckets 的小单元，实际上是数据分区。 Couchbase 使用一个集群映射将分区和服务器关联起来。一个Couchbase服务器在一个集群内对一个bucket有3个副本，每个 Couchbase 管理vbucket 激活或副本的子集。 这就是Couchbase的弹性原理; 每个文档的每次索引，都有副本，如果集群中的一个节点挂了，集群将激活副本分区来保证连续的服务。 

 Figure 2-3 解释了集群中只有一个激活的数据拷贝同时有一个或多个副本。 
![F2-3 Couchbase 复制](https://img-blog.csdn.net/20160520091525880)
Figure 2-3. Couchbase active document and replicas

从客户端视角看,如果使用智能客户端(Java, C, C++,Ruby等.), 然后这些客户端连接集群映射; 客户端从应用发送请求到合适的服务器. 就交互而言，有非常重要的一点: 文档操作默认是异步的。这意味着当你更新文档的时候，Couchbase 不能在硬盘上立即更新数据。Figure 2-4. 呈现了真实的处理过程。

![F2-4](https://img-blog.csdn.net/20160520091800102)

Figure 2-4. Couchbase data flow

如 Figure2-4 所示, 智能客户端连接到一个 Couchbase 实例，首先异步将文档写到缓存中，客户端立即得到响应而不是阻塞到数据流处理完，当写操作完成时由客户端改变行为状态。然后，文档被放入集群那边的写队列，所以在集群间复制; 此后, 文档被放入硬盘存储的写队列来持久化到相关节点。如果部署了多集群，Cross Data Center Replication (XDCR) 特性可以在不同集群间传播这种数据改变，集群可以位于不同的数据中心。

Couch base有自己的数据查询方法; 实际上, 简单地使用文档ID就可以查询文档，Couchbase的强大在视图特性的内部. Couchbase 中, 有一个二级索引叫做设计文档，是在bucket内部创建的. bucket可以包含文档的多个类型，例如一个简单的电子商务应用的bucket 包含如下内容:
- Account
- Product
- Cart
- Orders
- Bills

Couchbase 使用设计文档来完成它们的逻辑分割.  一个bucket 可以包含多设计文档 , 也包含了多视图.  一个视图是由用户定义bucket内的索引文档的函数。该函数是用户定义的map/reduce 函数将文档映射到集群中，输出 key/value 对, 存储在索引中用于将来的信息提取。回顾电商网站的例子，尝试从帐户标识的所有订单中建立索引. map/reduce 函数如下所示:   

```java
function(doc, meta) {
    if (doc.order_account_id)
        emit(doc.order_account_id, null);
}
```

if 判断语句允许函数聚焦在一个文档，它包含了order_account_id 字段然后索引这一标识。因此，任何客户端都可以根据这一标识从Couchbase中查询数据。

## Cluster Manager 和管理控制台

Cluster manager 是集群中一个特殊的节点。在任何时候，如果集群内的一个节点挂了,  orchestrator 通过通知集群内所有其它节点来处理故障切换, 定位故障节点的副本分区来使之处于激活状态. Figure 2-5 描述了故障切换处理。

![F2-5 couchbase 故障切换](https://img-blog.csdn.net/20160520092533690)

Figure 2-5. Couchbase failover  

如果 orchestrator 节点挂了,所有节点都可以通过心跳监控检测到，这个 watchdog 运行在集群中的所有节点上。所有集群相关的特性都可以通过API形式来管理Couchbase, 但是有现成的管理控制台。Couchbase 有一个安全控制台来管理和监控集群; 可以选择可用的操作包括服务器搭建，创建buckets, 浏览和更新文档, 实现新的视图,以及监控 vBucket 和硬盘写队列.

Figure 2-6 展示了Couchbase 控制台主页，有现存buckets所使用的内存, 数据使用的硬盘, 以及buckets的活动.

![F2-6 couchbase 控制台主页](https://img-blog.csdn.net/20160520092832851)

Figure 2-6. Couchbase console home

在 Server Nodes 中执行集群管理,让用户配置故障切换和复制以防止数据丢失。 Figure 2-7 展示了单节点安装故障切换的不安全型。

![F2-7 couchbase 服务器节点](https://img-blog.csdn.net/20160520093128199)

Figure 2-7. Couchbase server nodes
任何时候，都可以通过点击Add Server 按钮来增加一个新的Couchbase 服务器，这一操作的时候，数据就开始在节点间复制来保证故障切换。通过点击 

服务器IP, 可用访问一个bucket的监控数据, 如 Figure 2-8.

![F2-8 couchbase 监控](https://img-blog.csdn.net/20160520093339092)

Figure 2-8. Couchbase bucket monitoring

该图展示了一个叫做DevUser的数据 bucket，包含了用户相关的JSON文档.如前所述 ,新文档索引的处理是复杂的底层处理的一部分。当处理高索引吞吐量的海量数据时，可用从监控控制台看到基本的测量信息。例如，当写操作时统计磁盘写队列的瓶颈。

Figure 2-9 中, 可用看的drain rate—从磁盘写队列中写入磁盘的数据数量——在写入副本时活跃节点时平滑的，而且在平滑区间内增长相对平均。一个改变的行为将是看到活动项目的平均年龄保持增长，这意味着，写操作是与数据推入写磁盘队列的数量相比太慢了。 
![F2-9 couchbase 磁盘队列](https://img-blog.csdn.net/20160520093722369)
Figure 2-9. Couchbase bucket disk queue

## 管理文档

通过bucket视图，可以从管理控制台管理所有的文档. 这一视图允许用户浏览buckets， 设计文档以及视图.文档存储在Couchbase的一个bucket中,可以通过管理控制台的 Data Bucket 栏目访问，如Figure 2-10 所示. 
![F2-10 Couchbase bucket view](https://img-blog.csdn.net/20160520094056198)
Figure 2-10. Couchbase console bucket view

从服务器来看 , 控制台给出了bucket的统计数据如 RAM 和存储大小以及每秒操作的数量。但真正的好处是这个视图可以浏览文档并通过ID来提取它们，如 Figure 2-11 所示. 
![F2-11 Couchbase document by ID](https://img-blog.csdn.net/20160520094310293)
Figure 2-11. Couchbase document by ID

同时，可以通过这一视图创建设计文档和索引文档的视图如Figure 2-12 所示.

![F2-12 couchbase 视图实现](https://img-blog.csdn.net/20160520094548825)

Figure 2-12. Couchbase console view implementation
在Figure 2-12 中, 实现了一个视图，它基于公司名称提取文档。管理控制台可用非常方便的管理文档，但在真实世界中，需要在管理控制台开始实现一个设计文档，并且创建一个工业化部署的备份。所有的设计文档都存储在一个JSON 文件中，有着一个简单的结构来描绘上所有的视图，如表 2-1 所示.

Listing 2-1. Designing a Document JSON Example

```
[{
    ...
    "doc": {
        "json": {
            "views": {
                "by_Id": {"map": "function (doc, meta)
                {\n emit(doc.id, doc);\n}"},
                "by_email": {"map": "function (doc, meta)
                {\n emit(doc.email, doc);\n}"},
                "by_name": {"map": "function (doc, meta)
                {\n emit(doc.firstname, null);\n}"},
                "by_company": {"map": "function (doc, meta)
                {\n emit(doc.company, null);\n}"},
                "by_form": {"map": "function (doc, meta)
                {\n emit(meta.id, null);\n}"}
            }
        }
        ...
    }
}]
```

已经看到，可以通过管理控制台执行文档的管理，但是在商用架构中，大量这样的操作是通过使用Couchbase API 的脚本完成的。

# 介绍ElasticSearch

既然了解了Couchbase这样的一个NoSQL，那么ElasticSearch 也是一个NoSQL技术，但与 Couchbase完全不同. 这是由Elastic公司命名的分布式数据库。

## 架构

它是一个建立于Apache Lucene（一个Java写的全文搜素引擎）之上的索引/搜索引擎。从一开始, ElasticSearch 就是分布式可伸缩的，可以通过增加节点资源来垂直扩展，也可以通过增加节点来水平扩展增加高可用性同时保持弹性。如果一个节点挂了，由于在集群间的复制，由其它节点提供服务。

ElasticSearch 是一个无缝引擎；数据以JSON存储，分区叫做shards. 一个 shard 实际上是一个Lucene的索引，是 ElasticSearch中最小的可扩展单元. Shards 组织成ElasticSearch中的索引，使应用完成读写交互。最后, 索引是一个ElasticSearch中逻辑上的命名空间，是shards 集合的一个重新分组，当请求到达时, ElasticSearch 将它路由到合适的shard上，如 Figure 2-13 所示.

![F2-13](https://img-blog.csdn.net/20160520095354010)

Figure 2-13. Elasticsearch index and shards

ElasticSearch中有两种shard类型:基本 shards 和复制shards. 当启动一ElasticSearch 节点, 开始只增加一个基本shard, 这就足够了,当时当读／检索请求的吞吐量迅猛增加的时候会怎样呢？这种情况下, 一个基本 shard 可能不够用了，就需要另一个shard. 不能实时增加shard来期望ElasticSearch 来扩展; 这将在两个新的基本shard中重新索引所有的数据。所以，在启动一个 ElasticSearch项目的开始, 适当评估集群中有多少个基本shard 非常重要。在一个节点增加多个 shards 不能增加集群的容量，因为有着硬件的现在。为了增加集群容量，也需要增加更多的节点来承载基本 shards，见Figure 2-14.

![F2-14 es primary shard](https://img-blog.csdn.net/20160520095523996)

Figure 2-14. ElasticSearch primary shard allocation
一个好事情时ElasticSearch在新节点上可以通过网络自动拷贝shard，如 Figure 2-14.但是如何确保数据不丢失呢？这就是replica shards的角色。

Replica shards 开始源于故障切换;当primary shard 挂了, 一个副本被提升为primary来保证集群的连续性。Replica shards 在primary shards 做索引的时候拥有同样的负载; 一旦一个文档在 primary shard中建立了索引, 同样也在replica shards中做了索引. 这就是为什么在集群中增加副本却不会增加索引的性能，但是如果增加了额外的硬件，就会提升搜索的性能。在一个三节点集群中,有一个primary shards 和两个副本  Figure 2-15 展示了如何重新分区.

![F2-15 ES shards](https://img-blog.csdn.net/20160520100017893)

Figure 2-15. ElasticSearch primary and replica shards
另外, 通过在集群中帮助请求的负载均衡，从而得到更好的性能。最后要讨论的是纯 ElasticSearch 架构的indices以及多节点. Indices 被重新分组到 ElasticSearch 的节点,Figure 2-16 展示了3种类型的节点。

![F2-16 ES 集群拓扑结构](https://img-blog.csdn.net/20160520120445790)

Figure 2-16. ElasticSearch cluster topology

这是三种节点的描述:
- 
Master nodes: 这些节点是轻量级的，负责集群管理。它们不承载任何数据, server indices,或搜素请求. 它们专门来保证集群的稳定性，负载较低。推荐使用三个master nodes通过冗余保证故障切换。

- 
Data nodes: 这些节点持有数据，索引和搜索请求。

- 
Client nodes: 这保证部分处理步骤的负载均衡，且部分工作负载在数据节点执行，例如搜索请求发散在不同节点上。


理解了ElasticSearch架构, 就可以使用API运行一些查询了。 

## ElasticSearch 监控

Elastic 提供了一个叫 Marvel 的插件，目标是监控 ElasticSearch 集群. 这个插件是个商用产品，但你可以在开发模式免费使用它。

下载和安装都非常简单:[https://www.elastic.co/downloads/marvel](https://www.elastic.co/downloads/marvel)

Marvel 是依赖 Kibana的, Elastic的可视化控制台,带来大量的可视化技术，使操作者准确地知道在集群上发生了什么。 Figure 2-17展示了 Marvel的控制面板. 
![F2-17 ES Marvel](https://img-blog.csdn.net/20160520120817861)
Figure 2-17. ElasticSearch Marvel console

Marvel 提供了节点indices和shards的信息; CPU 利用率;JVM所使用的内存;索引的速度,以及搜索的速度. Marvel 甚至可以进入到 Lucene的底层查看 

flushes 和 merges的信息.例如集群中shard的分布列表 见 Figure 2-18.

![F2-18 Marvel shard](https://img-blog.csdn.net/20160520121055652)

Figure 2-18. Marvel’s shard allocation view

Marvel 可以提供大量有关集群的信息。Figure 2-19展示了节点统计的子集.

![F2-19 Marvel 节点统计面板](https://img-blog.csdn.net/20160520121606216)

Figure 2-19. Marvel Node Statistics dashboard
dashboard 被组织成几行; 实际上，在截屏中是看不到多于20行的内容的.每行包含一个或行标题的可视化。 在 Figure 2-19中，没看到发送到 

indices的GET 请求; 这就是为什么线图是平的. 在开发模式中，这些统计可以帮助判定服务器是否伸缩，例如，启动一个单节点集群，查看特殊需求的行为。在生产环境，能够真实地看到集群的信息，而不损失什么。

## 通过 ElasticSearch搜索

Marvel又一个叫 Sense的特性，是ElasticSearch的一个查询编辑器. Sense 的强大是又一个自动补全查询的能力，当不熟悉 

ElasticSearch API时非常有用, 见Figure 2-20.

![F2-20 Marvel Sense](https://img-blog.csdn.net/20160520122357554)

Figure 2-20. Marvel Sense completion feature
也可以将查询导出到cURLs, 例如, 使用的脚本参见Figure 2-21.

![F2-21 copy as cURL](https://img-blog.csdn.net/20160520122625830)

Figure 2-21. Marvel Sense copy as cURL feature

在这种情况下,查询被视为cURL 命令,见  Listing 2-3.

Listing 2-3. Example of cURL Command Generated by Sense

```
curl -XGET "http://192.168.59.103:9200/yahoo/_search" -d'
{
    "query": {
        "match_all": {}
    }
}'
```

这一查询基本上搜索了Yahoo索引下的全部文档. 未来展示 search API的优势, 使用来自 Yahoo的数据集，它包含了若干年Yahoo股票的信息。一个 ElasticSearch’s search API中关键的特性是，这是一个聚合框架。可以用不同的方法聚合数据; 更通用的方式是事业日期直方图，相当于时间线。Figure 2-22 解释了一个查询的例子; 通过日期聚合股票数据，间隔可以是月，也可以计算给定月份的股价最大值。

![F2-22 聚合示例](https://img-blog.csdn.net/20160520123016967)

Figure 2-22. Marvel Sense Aggregation example
作为结果, 得到一个文档的数组，每个条目包含了日期，每月的文档数量，最高值，见Listing 2-4.

Listing 2-4. Aggregation Bucket

```
{
    "key_as_string": "2015-01-01T00:00:00.000Z",
    "key": 1420070400000,
    "doc_count": 20,
    "by_high_value": {
        "value": 120
    }
}
```

可以看到如 Figure 2-22的查询, 有两个层次的聚合: 一个是日期直方图，第二个是最大值。实际上 , ElasticSearch 支持多层次聚合。 

搜素API 非常丰富，不可能逐个浏览，详情参见:

[https://www.elastic.co/guide/en/elasticsearch/reference/1.4/search-search.html](https://www.elastic.co/guide/en/elasticsearch/reference/1.4/search-search.html)

现在熟悉了两种 NoSQL技术 ,  看一下把它们集成到一个电子商务应用的不同方法。

# 在基于SQL的架构中 使用 NoSQL 缓存

尽管理解了NoSQL技术相对于 SQL 数据库的优势，但不想打破依赖于SQL数据库的现有架构。下面的方法可以完善这里的架构，基于NoSQL来增加访问数据的灵活性。

## 文档缓存

第一个要讨论的是如何复制数据到NoSQL的后端。希望每次数据写到SQL数据库中的时候，一个文档同时在NoSQL中创建或更新。文档的创建、更新字段，或者丰富子文档对应于 RDBMS的表关系. 在访问文档的时候，只要API的GET 请求产生了，都先看NoSQL后端，如果有则返回此文档。

如果文档没找到怎么办？缓存没有命中的事件将被触发，NoSQL manager 从 RDBMS中重建此文档. 那在SQL层插入的transaction失败了怎么办? 框架应该是交易型的，只有当SQL 的交易完成时才触发文档的重建。Figure 2-23 总结了这一机制. 
![F2-23 文档缓存](https://img-blog.csdn.net/20160520123956652)

Figure 2-23. Caching a document in NoSQL
Figure 2-23 描述的内容:
- 
第一框图展示了一个账户如何在 RDBMS 中创建的，以及如何在NoSQL中创建一个完整账户的文档表达

- 
第二框图展示了API如何从NoSQL存储中获得一个账户信息.

- 
第三个框图展示了一个缓存丢失的例子,当文档不在NoSQL中的时候必须从RDBMS中重建


实际上，基于NoSQL为web应用建立了一个缓存。这种方法依赖于NoSQL访问整个文档的灵活性而不增加SQL数据库的负担，也充分利用了NoSQL的分布式灵活性。 通过这种方法，随着请求速度的大量增长可以扩展集群，减少SQL数据库的压力。

## ElasticSearch的 Couchbase 插件

为了获得这样一个缓存机制，需要选择NoSQL技术。第一个方法独立使用 

Couchbase, 但是 Couchbase的搜索特性不太好使。用map/reduce 函数索引数据较烦琐，尤其是只做简单聚合的时候。 Couch base 是个很棒的 NoSQL 数据库, 但实现搜索确实痛苦。作为替代方法，可以使用Couchbase/ElasticSearch 集成插件, 基本上使用Cross Data Center Replication (XDCR)  传输所有数据到 ElasticSearch 集群(www.couchbase.com/jp/couchbase-server/connectors/elasticsearch). 

就操作任务而言，要维护三种不同的技术:RDBMS, Couchbase, 和 ElasticSearch.
## 最有效的ElasticSearch

保持使用 Couchbase 原因：
- 
索引全部对象的能力， 例如将SQL中的账户信息签到NoSQL

- 
通过灵活的搜索API适应从简单到复杂聚合查询的能力


从选择 Couchbase 开始，作为最佳实践，文档存储在数据库中。当体验架构的时候，要知道什么是访问和请求数据的最有效方法。在大多数使用场景中， ElasticSearch 就是最有效的方法。





















