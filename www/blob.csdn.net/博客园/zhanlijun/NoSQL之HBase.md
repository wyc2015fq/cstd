# NoSQL之HBase - zhanlijun - 博客园
# [NoSQL之HBase](https://www.cnblogs.com/LBSer/p/3330383.html)
#### [http://www.cnblogs.com/LBSer/p/3330383.html](http://www.cnblogs.com/LBSer/p/3330383.html)
         9月初淘宝飞芃做了一个关于HBase的分享，讲的激情飞扬，让听众收益匪浅，现做下简单总结。      
         HBase是一个NoSQL数据库，在国内外被广泛使用，是bitTable的开源实现，主要由FaceBook、Cloudera、Yahoo!和ebay等大公司贡献，国内贡献主要是华为和阿里系，其中淘宝还有一个代码commiter。飞芃首先对HBase做了简要介绍，着重强调它的一些特点。之后又讲了Hbase不好的地方，以及使用的注意点，最后介绍了HBase在淘宝的一些应用。
**1. HBase的特点**
     a）强一致性：同一行数据读写在同一个regionserver上
     b）水平伸缩：region的自动分裂以及master的balance
     c）行事务：同一行的写入是原子的，这是由于同一行数据读写在同一个regionserver上决定的
     d）支持范围查询
     e）高性能随即写
    上面的特点让我觉得HBase是Hash和树的这两种数据结构的结合体。首先HBase使用了Hash，比如根据rowkey获得value，还使用了树，正因为是棵树，所以具有分裂、支持范围查询等特性。
**2.HBase不好的地方**
    a）没有二级索引，使用时需要将自己写索引表，这个索引表也存在HBase里
    b）没有在线备份与还原策略
    c）不稳定，使用有风险，比如NameNode单点，一旦挂了，整个HBase集群挂了，RegionServer挂掉后需要一些时间切换，Region分裂时服务不可用
**3.HBase在淘宝的应用**
          个人感觉是最精华的部分，HBase在淘宝里用在三个地方：
    a）实时推荐、实时报表、实时计费
          这类应用的特点是大量数据的实时写入以及读取
    b）大数据量类型项目
          比如历史类或需要长期保存的数据
    c）二次分析类型项目
          Hadoop集群做粗粒度分析，在线做二次分析，比如数据魔方。
**4.讨论**
         会后讨论的时候，飞芃着重强调了HBase的不稳定，需要专业团队维护，比如淘宝有一百多人的技术团队专门维护Hadoop和HBase集群，而且使用淘宝内部的HBase版本。针对一些同学提出的业务需求是否适合HBase的问题，飞芃建议要综合考虑应用规模等因素，但是如果选择了HBase，必须要有团队专门维护。

