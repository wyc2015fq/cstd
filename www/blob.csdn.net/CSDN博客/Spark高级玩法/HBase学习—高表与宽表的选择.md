# HBase学习—高表与宽表的选择 - Spark高级玩法 - CSDN博客
2018年01月20日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1329
**转自：https://yq.aliyun.com/articles/213705?utm_content=m_31236**
hbase中的宽表是指很多列较少行，即列多行少的表，一行中的数据量较大，行数少；高表是指很多行较少列，即行多列少，一行中的数据量较少，行数大。
hbase的row key是分布式的索引，也是分片的依据。
hbase的row key + column family + column qualifier + timestamp + value 是HFile中数据排列依据。HFile据此，对数据的索引到data block级别，而不是行级别。所以这种key是HFile内部的粗粒度（data block粒度）本地索引的主键。
据此，在HBase中使用宽表、高表的优劣总结如下：
- 
查询性能：高表更好，因为查询条件都在row key中, 是全局分布式索引的一部分。高表一行中的数据较少。所以查询缓存BlockCache能缓存更多的行，以行数为单位的吞吐量会更高。
- 
分片能力：高表分片粒度更细，各个分片的大小更均衡。因为高表一行的数据较少，宽表一行的数据较多。HBase按行来分片。
- 
元数据开销：高表元数据开销更大。高表行多，row key多，可能造成region数量也多，- root -、 .meta表数据量更大。过大的元数据开销，可能引起HBase集群的不稳定、master更大的负担（这方面后续再好好总结）。
- 
事务能力：宽表事务性更好。HBase对一行的写入（Put）是有事务原子性的，一行的所有列要么全部写入成功，要么全部没有写入。但是多行的更新之间没有事务性保证。
- 
数据压缩比：如果我们对一行内的数据进行压缩，宽表能获得更高的压缩比。因为宽表中，一行的数据量较大，往往存在更多相似的二进制字节，有利于提高压缩比。通过压缩，缓解了宽表一行数据量太大，并导致分片大小不均匀的问题。查询时，我们根据row key找到压缩后的数据，进行解压缩。而且解压缩可以通过协处理器（coproesssor）在HBase服务器上做，而不是在业务应用的服务器上做，以充分应用HBase集群的CPU能力。
设计表时，可以不绝对追求高表、宽表，而是在两者之间做好**平衡**。根据查询模式，需要分布式索引、分片、**有很高选择度**（即能据此查询条件迅速锁定很小范围的一些行）的查询用字段，应该放入row key；能够均匀地划分数据字节数的字段，也应该放入row key，作为分片的依据。选择度较低，并且不需要作为分片依据的查询用字段，放入column family和column qualifier，不放入row key。
![640.png?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png?)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg?)
**更多文章，敬请期待**
