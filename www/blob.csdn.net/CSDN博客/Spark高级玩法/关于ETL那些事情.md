# 关于ETL那些事情 - Spark高级玩法 - CSDN博客
2018年01月06日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1429
![?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFXyURjyD7QswNXxmu7r9t7C2kq1MJ8OaWCjh1AqbiczM4MQ2xmLPdl09AWUhNRjMVzuYBjxES4HEOg/?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
ETL这个概念也很久了，但是真正的进入大家的视野估计还是由于大数据。由于从15年至今，其实整个大数据领域都处于做数据仓库，然后简单生成报表这个层面，ETL（ETL - Extract, Transform, Load），这个概念就必不可少了。由于，经常有人在群里问浪尖：什么是ETL？ETL做了什么事情？也经常有招聘信息里面包含ETL，那么今天在这里我就简单给大家聊聊ETL。
**数据仓库中的ETL概述**
企业中是需要定期的加载数据仓库，以达到促进业务分析的目的。为此，需要提取来自一个或多个操系统的数据并将其复制到数据仓库中。数据仓库环境面临的挑战是在许多系统上集成，重新排列和整合大量数据，从而为商业智能提供新的统一信息库。
从源系统中提取数据并将其引入数据仓库的过程通常称为ETL，即提取，转换和加载。请注意，ETL是指一个广泛的过程，而不是三个明确的步骤。首字母缩略词ETL可能太简单了，因为它省略了运输阶段，意味着该过程的每个其他阶段都是不同的。不过，整个过程被称为ETL。
ETL的方法和任务已为人所熟知多年，并不一定是数据仓库环境所独有的：各种各样的专有应用程序和数据库系统是任何企业的IT骨干。数据必须在应用程序或系统之间共享，试图整合它们，至少给两个应用程序有相同的数据视图。这种数据共享主要是通过类似于我们现在称之为ETL的机制来解决的。
**数据仓库中的ETL基础知识**
在ETL过程中会发生什么？以下任务是该流程中的主要操作。
**数据提取(Extract)**
在提取过程中，从许多不同的来源识别及提取所需的数据，数据源包括数据库系统和应用程序。很多时候，不可能直接识别感兴趣的特定数据子集，因此需要提取更多的数据，而相关数据的识别将在后面的步骤中完成。在此提取过程中可能会根据源数据的一下功能特性，进行一些转换操作。提取数据的大小从几百千字节到千兆字节，取决于源系统和业务情况。两次提取数据操作之间的时间跨度可以在几天/几小时到几分钟之间变化甚至接近实时。例如Web服务器日志文件在短时间内就可以有上百兆，而浪尖收集处理的数据集，则基本上是每秒几个GB。
**数据传输(Transportation)**
数据提取后，必须将其物理运输到目标系统或中间系统进行进一步处理。根据所选的交通方式，也可以在这个过程中进行一些转换。例如，可以夸网络在目标机器上执行SQL语句实现连接两列作为SELECT语句的一部分。
**数据转换(Transformation)**
该步骤主要是将提取的数据转换为需要的格式，以便于存入另一数据仓库。转换的过程中将使用一定的规则(业务强相关)或者lookup表或者和其它数据整合，浪尖在这里列出来几个常见的转换操作供大家参考：
• 应用业务规则（所谓的派生，例如计算新的度量和维度），
• 清理（例如，将NULL映射到0或“男”到“M”和“女”到“F”等）
• 过滤（例如，选择仅加载某些列），
• 将一列分成多列，反之亦然，
• 将来自多个来源的数据（例如，查找，合并）结合在一起，
• 转置行和列，
• 应用任何种类的简单或复杂的数据验证（例如，如果一行中的前3列是空的，则拒绝来自处理的行）
**数据加载（load）**
将数据存储于目标数据仓库(hbase，hive等)，以供数据分析使用。这就是ETL的终点。
**ETL的工具**
最常见的ETL工具就是kettle。但是这个资料比较少。
浪尖在企业中并没有用到这个工具，原因是数据抽取我们有自己的agent(常用的也有flume，sqoop应该也算一个)，然后传输用forward+kafka，转换操作我们可以spark代码，然后数据仓库hive或者hbase均可（看需求）。
商业的版的ETL工具很多，但是收费，即使不收费，什么东西都被工具实现了，我们作为精英程序员的价值就不存在了。
**推荐阅读：**
1，[初识PB级数据分析利器Prestodb](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484365&idx=1&sn=c5f698f5ec745a7ef7ff268fed8cb119&chksm=9f38e0e5a84f69f3fa796d9f28de4e90c12c0afcb63d5fe60e3e359471bb48ef9a0e9e679b7b&scene=21#wechat_redirect)
2，[实战phoenix](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484253&idx=1&sn=2d1a9100dae4abae0efc2a4fea6fded5&chksm=9f38e075a84f69639b03aaa5491c9d9016a3e9057a37f3bf3dc892159fcde39dbb361350d7ac&scene=21#wechat_redirect)
3，[HBase原理和设计](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484241&idx=1&sn=2f4d59fe83d74a1bc764230a574972dc&chksm=9f38e079a84f696f32c2b823024164ea2c83d4775d24f02ca1d3b3ac6731a1172688001e9666&scene=21#wechat_redirect)
4，[Kylin及数据仓库的技术概念详解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484196&idx=1&sn=d95310ff52b68d05ad512ab0179beb5c&chksm=9f38e00ca84f691a82514f795c6e36ecfda06ea200e1d7035043c777e12aaeebc6b4994aa3e3&scene=21#wechat_redirect)
![640.png?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png?)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg?)
**更多文章，敬请期待**
