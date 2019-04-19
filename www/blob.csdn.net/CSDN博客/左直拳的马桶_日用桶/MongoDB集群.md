# MongoDB集群 - 左直拳的马桶_日用桶 - CSDN博客
2014年10月03日 22:10:13[左直拳](https://me.csdn.net/leftfist)阅读数：1788
个人分类：[mongoDB](https://blog.csdn.net/leftfist/article/category/2557427)
                MongoDB集群技术包括副本集、分片两部分；另外还有一种主从模式，但mongoDB目前已不推荐使用，而代之以副本集。
副本集指将数据复制，多份保存，不同服务器保存同一份数据，在出现故障时自动切换。对应的是数据冗余、备份、镜像、读写分离、高可用性等关键词；
而分片则指为处理大量数据，将数据分开存储，不同服务器保存不同的数据，它们的数据总和即为整个数据集。追求的是高性能。
在生产环境中，通常是这两种技术结合使用，分片 + 副本集。
详见拙作：
[MongoDB集群——副本](http://blog.csdn.net/leftfist/article/details/39583585)
[MongoDB集群——分片](http://blog.csdn.net/leftfist/article/details/39583773)
