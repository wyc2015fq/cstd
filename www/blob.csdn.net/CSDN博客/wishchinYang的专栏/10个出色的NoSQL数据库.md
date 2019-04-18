# 10个出色的NoSQL数据库 - wishchinYang的专栏 - CSDN博客
2016年05月18日 19:28:07[wishchin](https://me.csdn.net/wishchin)阅读数：3781
# [10个出色的NoSQL数据库](http://www.csdn.net/article/2012-10-12/2810660-NoSQL-MongoDB-HBase)
随着大数据的不断发展，非关系型的数据库现在成了一个极其热门的新领域，非关系数据库产品的发展非常迅速。现今的计算机体系结构在数据存储方面要有庞大的水平扩展性，而NoSQL也正是致力于改变这一现状。目前Google的 BigTable和Amazon 的Dynamo使用的就是NoSQL型数据库，本文介绍了10种出色的NoSQL数据库。
虽然NoSQL流行语火起来才短短一年的时间，但是不可否认，现在已经开始了第二代运动。尽管早期的堆栈代码只能算是一种实验，然而现在的系统已经更加的成熟、稳定。不过现在也面临着一个严酷的事实：技术越来越成熟——以至于原来很好的NoSQL数据存储不得不进行重写，也有少数人认为这就是所谓的2.0版本。这里列出一些比较知名的工具，可以为大数据建立快速、可扩展的存储库。
**1. Casssandra**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/121011/145_121011141945_1.png)
[Cassandra](http://cassandra.apache.org/)最初由Facebook开发，后来成了Apache开源项目，它是一个网络社交云计算方面理想的数据库。它集成了其他的流行工具如Solr，现在已经成为一个完全成熟的大型数据存储工具。Cassandra是一个混合型的非关系的数据库，类似于Google的BigTable。其主要功能比Dynomite（分布式的Key-Value存储系统）更丰富，但支持度却不如文档存储MongoDB。Cassandra的主要特点就是它不是一个数据库，而是由一堆数据库节点共同构成的一个分布式网络服务，对Cassandra的一个写操作，会被复制到其他节点上去，而对Cassandra的读操作，也会被路由到某个节点上面去读取。在最近的一次测试中，[Netflix建立了一个288个节点的集群](http://techblog.netflix.com/2011/11/benchmarking-cassandra-scalability-on.html)。
**2. Lucene/Solr**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/121011/145_121011142021_1.png)
[Lucene](http://lucene.apache.org/core/)是Apache软件基金会4 jakarta项目组的一个子项目，这是一个开放源代码的全文检索引擎工具包，就是说它不是一个完整的全文检索引擎，而是一个全文检索引擎的架构。不过大多数人并不认同Lucene是一个数据库，因为大多数人只是用它来检索大量的文本块，不过它的确采用了与其他NoSQL数据存储相似的模型。如果说查询并不是仅仅局限于精确的匹配，而是寻找出那些出现在块中的字或者字段的话，毫无疑问，Lucene/Solr是最好的查询方式。
**3. Riak**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/121011/145_121011142101_1.png)
[Riak](http://basho.com/products/riak-overview/)是由技术公司basho开发的一个类似Dynamo的分布式Key-Value系统。其以分布式，水平扩展性，高容错性等特点著称。从事Riak工作最有趣的部分是可以使用JavaScript或者Erlang来做Map/Reduce查询，它们会查询每个节点，收集结果，而且可以重复，如果需要使用的结果进行重新进行搜寻的话。该系统还为类似于Solr的搜索提供全文索引，同时还提供一个控制面板，可以查看集群的信息。
**4. CouchDB**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/121011/145_121011142343_1.png)
[CouchDB](http://couchdb.apache.org/)是用Erlang开发的面向文档的数据库系统，不过它不是一个传统的关系数据库，而是面向文档的数据库，其数据存储方式有点类似lucene的index文件格式，CouchDB最大的意义在于它是一个面向web应用的新一代存储系统。作为一个分布式的数据库，CouchDB可以把存储系统分布到n台物理的节点上面，并且很好的协调和同步节点之间的数据读写一致性。CouchDB支持REST
 API，可以让用户使用JavaScript来操作CouchDB数据库，也可以用JavaScript编写查询语句，可以想像一下，用AJAX技术结合CouchDB开发出来的CMS系统会是多么的简单和方便。
CouchDB还有一个更加商业化的“表亲”——Couchbase，不过它提供缓存功能，更好的分片，增量查询，更好的索引和一些其他的功能。其实Couchbase与CouchDB也是紧密相关的，Couchbase产品包含了CouchDB的一个副本。
**5. Neo4J**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/121011/145_121011142413_1.png)
大多数的NoSQL数据库只是存储键和值的一个灵活的捆绑。不过[Neo4J](http://www.csdn.net/article/2012-10-12/neo4j.org)的存储的是对象之间的关系，或者说这种结构就是数学中的“图”。Neo4J是一个面向网络（“图”）的数据库，也就是说，它是一个嵌入式的、基于磁盘的、具备完全的事务特性的Java持久化引擎，但是它将结构化数据存储在网络上而不是表中，当然也可以把Neo4J看作是一个高性能的图引擎，该引擎具有成熟和健壮的数据库的所有特性。该工具包括很多有关搜索和分析的关系的算法，它能够帮助寻找谁是我的朋友，或者寻找朋友的朋友。这些“图的遍历”算法，可以节省很多指针查询的麻烦。
**6. Oracle的NoSQL**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/121011/145_121011142455_1.png)
也许是NoSQL运动太红火的原因，Oracle决定开发一款产品，将键/值对拆分在整个节点集上，这样的优势在于提供了一个灵活的事务保护措施，进而可以确保从数据在节点上等待存储开始到通过网络被成功备份结束，都尽在掌握之中。
Oracle的[NoSQL Database](http://www.oracle.com/us/NoSQLDatabase)，是在10月4号的甲骨文全球大全上发布的Big Data Appliance的其中一个组件，Big Data Appliance是一个集成了Hadoop、NoSQL Database、Oracle数据库Hadoop适配器、Oracle数据库Hadoop装载器及R语言的系统。
**7. MongoDB**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/121011/145_121011142537_1.png)
[MongoDB](http://www.mongodb.org/)是一个基于分布式文件存储的数据库，介于关系数据库和非关系数据库之间，是非关系数据库当中功能最丰富，最像关系数据库的。MongoDB最大的特点是他支持的查询语言非常强大，其语法有点类似于面向对象的查询语言，几乎可以实现类似关系数据库单表查询的绝大部分功能，而且还支持对数据建立索引。MongoDB支持RUBY，PYTHON，JAVA，C++，PHP,C#等多种语言。
MongoDB是高性能开源文档数据库，也是目前最受关注的NoSQL技术之一，以敏捷、可扩展和对企业应用友好（支持事务，一致性和数据完整性保证，有大企业应用案例）而著称。有人甚至认为LAMP中的M应该用MongoDB取代MySQL，其火热程度可见一斑。使用MongoDB的公司包括Foursquare, Craiglist, 迪士尼，SAP，Intuit，EA等，国内淘宝、大众点评、视觉中国等公司有应用。(最新版[MongoDB
 2.2下载](http://www.mongodb.org/downloads))
**8. Hadoop的HBase**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/121011/145_121011142710_1.png)
[HBase](http://hbase.apache.org/)(Hadoop Database)，是一个高可靠性、高性能、面向列、可伸缩的分布式存储系统，利用HBase技术可在廉价PC Server上搭建起大规模结构化存储集群。HBase是Google Bigtable的开源实现，类似Google Bigtable利用GFS作为其文件存储系统，HBase利用Hadoop HDFS作为其文件存储系统；Google运行MapReduce来处理Bigtable中的海量数据，HBase同样利用Hadoop
 MapReduce来处理HBase中的海量数据。
虽然大多数人都认为Hadoop及其所有的工具都是作为管理大规模集群的一种机制，其实不然，Hadoop也包括数据库，在HBase中也是通过节点来传播数据。Hadoop的Map /Reduce的架构是非常适合于复杂的计算任务或查询工作。领土在不断的扩张，新的数据库像Accumulo就是Hadoop平台的一个延伸。(Apache Accumulo是一个可靠的、可伸缩的、高性能的排序分布式的Key-Value存储解决方案，基于单元访问控制以及可定制的服务器端处理。使用Google BigTable设计思路，基于Apache
 Hadoop、Zookeeper和Thrift构建)
**9. BigTable/ Accumulo/ Hypertable**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/121011/145_121011142757_1.png)
BigTable是非关系的数据库,是一个稀疏的、分布式的、持久化存储的多维度排序Map。Bigtable的设计目的是可靠的处理PB级别的数据，并且能够部署到上千台机器上。Bigtable已经实现了下面的几个目标：适用性广泛、可扩展、高性能和高可用性。Bigtable已经在超过60个Google的产品和项目上得到了应用，包括Google Analytics、GoogleFinance、Orkut、Personalized Search、Writely和GoogleEarth。
谷歌的BigTable开启了NoSQL的热潮，现在很多公司都模仿谷歌的架构搭建了自己的平台。[谷歌的AppEngine用户可以把键/值对存储在数据库中](https://developers.google.com/appengine/docs/python/datastore/overview)，而Hadoop的用户可以把它们放在Accumulo上，其他的可以使用Hypertable。所有的这些基本上都属于键/值存储，只不过添加了一些额外的功能，增加了搜索的速度而已。
**10. DynamoDB**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/121011/145_121011142903_1.png)
[DynamoDB](http://aws.amazon.com/dynamodb/)是亚马逊的key-value模式的存储平台，可用性和扩展性都很好，性能也不错：读写访问中99.9%的响应时间都在300ms内。DynamoDB的NoSQL解决方案，也是使用键/值对存储的模式，平且通过服务器把所有的数据存储在SSD上的三个不同的区域。如果有更高的传输需求，DynamoDB也可以在后台添加更多的服务器。（编译/[@CSDN王鹏](http://weibo.com/1615203572/profile?topnav=1&wvr=3.6)，审校/包研）

