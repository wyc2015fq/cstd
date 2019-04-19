# 简单说说bigtable - Arthur的随笔 - CSDN博客
2012年08月23日 23:19:32[largetalk](https://me.csdn.net/largetalk)阅读数：1003
这个题目很难，所以肯定说不好，但最近看了点这方面得东西，总得唠叨唠叨，留下点什么吧。话说GFS，bigtable这样得东西我也只是看看，工作中根本用不到，懂些名词，免得在别人吹牛B时只能在旁边呵呵傻笑，行，言归正传。
bigtable首先是google提出来得，[论文](http://static.googleusercontent.com/external_content/untrusted_dlcp/research.google.com/zh-CN//archive/bigtable-osdi06.pdf)在此，给的注释就是“一个分布式结构化数据存储系统”，其实就一数据库，当然和我们平时得关系数据库是不一样得。首先看它的结构化，它的结构是什么呢，比较像nosql数据的K，V结构，但又不全像。它的结构学术名是“a
 sparse, distributed, persistent multi-dimensional sorted map”。它是一个大的稀疏，分布式，多维排序map。map很像我们平时说得表了，有row name，column name，加上一个时间戳就是多维了，时间戳用来做版本号。一般cloumn会有很多，成千上万，所以叫bigtable。然后有个概念column family,几个cloumn都属于某个column family,我想是应该有局部化得好处吧。bigtable逻辑结构就这么简单，然后它得实现就复杂了，比如由chubby,
 master server, tablet server 构成，简单来说基于chubby提供得文件锁服务，master管理数据元信息，tablet提供数据服务。这么说当然是太简单了，但目前我就了解到这么深了，上面那篇论文还能讲得再多点，比如tablet怎么怎么切分，存储，压缩等等，有兴趣得自己看论文。
bigtable在google内部应该用得比较多，但都没什么资料。有个产品GAE可以让我们管中窥豹一下。GAE的datastore用来存储用户数据，就是bigtable得一种了。下面是大量引用：来自[DBA notes](http://www.dbanotes.net/arch/google_app_engine-datastore.html)
“在编程方面，Datastore是基于"Entity（实体）"这个概念，而且Entity和"对象"这个概念比较类似，同时Entity可以包括多个Property（属性），Property的类别有整数，浮点和字符串等，比如，可以设计一个名为"Person"的Entity，它包含名为"Name"的字符串Property和名为"Age"的整数Property。由于Datastore是"Schema-less"的，所以数据的Schema都由应用维护，而且能非常方便地对一个Entity所包含的属性进行增删和修改。在存储方面，一个Entity的实例可以被认为是一个普通的"Row（行）"，而包含所有这种Entity的实例的Table被称为Kind，比如，所有通过"Person"这个Entity生成实例，比如小吴，小朱和小华等，它们都会存放在同一个名为"Person"的Kind中。在结构方面，虽然也能通过特定的方式在Datastore中实现关系型结构，但是Datastore在设计上是为层次（Hierarchical）性结构"度身定做"的，有Root
 Entity和Child Entity之分，比如，可以把"Person"作为Root Entity（父实体），"Address"作为"Person"的Child Entity，两者合在一起可以称为一个"Entity Group"。这样做的好处是能将这两个实体集中一个BigTable本地分区中，而且能对这两个实体进行本地事务。“
看到没，所有关系都能对应得上，GAE的datastore用起来还是很简单得，python中累死django得model，用起来很不错。
google之外，典型的bigtable应用我知道得有hbase和cassandra。hbase没看过，不过看别人简介说，基本上是对应着上面那篇论文设计得，改了改名称，和hdfs与GFS关系一样。
cassandra设计上和google bigtable与hbase相比一个优势是去掉了中心节点，每个节点平等，没有单点故障（SPF），其他设计理念差不多。和hbase相比还有一个好处是不受限与hdfs。
目前python都能调用habse和cassandra， python通过thrift访问hbase，访问cassandra有一个包[pycassa](http://pycassa.github.com/pycassa/)
