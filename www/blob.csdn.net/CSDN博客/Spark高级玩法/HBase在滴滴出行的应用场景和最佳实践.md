# HBase在滴滴出行的应用场景和最佳实践 - Spark高级玩法 - CSDN博客
2017年12月16日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1349
### 本文主要介绍HBase在滴滴内部的一些典型使用场景，如何设计整个业务数据流，让平台开发者与用户建立清晰、明确、良好的合作关系
**背景**
对接业务类型
HBase是建立在Hadoop生态之上的Database，源生对离线任务支持友好，又因为LSM树是一个优秀的高吞吐数据库结构，所以同时也对接了很多线上业务。在线业务对访问延迟敏感，并且访问趋向于随机，如订单、客服轨迹查询。离线业务通常是数仓的定时大批量处理任务，对一段时间内的数据进行处理并产出结果，对任务完成的时间要求不是非常敏感，并且处理逻辑复杂，如天级别报表、安全和用户行为分析、模型训练等。
多语言支持
HBase提供了多语言解决方案，并且由于滴滴各业务线RD所使用的开发语言各有偏好，所以多语言支持对于HBase在滴滴内部的发展是至关重要的一部分。我们对用户提供了多种语言的访问方式：HBase Java native API、Thrift Server（主要应用于C++、PHP、Python）、JAVA JDBC（Phoenix JDBC）、Phoenix QueryServer（Phoenix对外提供的多语言解决方案）、MapReduce Job（Htable/Hfile Input）、Spark Job、Streaming等。
数据类型
HBase在滴滴主要存放了以下四种数据类型：
- 
统计结果、报表类数据：主要是运营、运力情况、收入等结果，通常需要配合Phoenix进行SQL查询。数据量较小，对查询的灵活性要求高，延迟要求一般。
- 
原始事实类数据：如订单、司机乘客的GPS轨迹、日志等，主要用作在线和离线的数据供给。数据量大，对一致性和可用性要求高，延迟敏感，实时写入，单点或批量查询。
- 
中间结果数据：指模型训练所需要的数据等。数据量大，可用性和一致性要求一般，对批量查询时的吞吐量要求高。
- 
线上系统的备份数据：用户把原始数据存在了其他关系数据库或文件服务，把HBase作为一个异地容灾的方案。
**使用场景介绍**
场景一：订单事件
这份数据使用过滴滴产品的用户应该都接触过，就是App上的历史订单。近期订单的查询会落在Redis，超过一定时间范围，或者当Redis不可用时，查询会落在HBase上。业务方的需求如下：
- 
在线查询订单生命周期的各个状态，包括status、event_type、order_detail等信息。主要的查询来自于客服系统。
- 
在线历史订单详情查询。上层会有Redis来存储近期的订单，当Redis不可用或者查询范围超出Redis，查询会直接落到HBase。
- 
离线对订单的状态进行分析。
- 
写入满足每秒10K的事件，读取满足每秒1K的事件，数据要求在5s内可用。
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/zxoLaeCI28RRmOxRibZOKKh3VS09ZqoIG2zA4HzFQure3U3cibWictAzmicbFam71NrYhAJmFlBvdTFfePibt0CY2Sw/?wx_fmt=png)
图1 订单流数据流程
按照这些要求，我们对Rowkey做出了下面的设计，都是很典型的scan场景。
*订单状态表*
Rowkey：reverse(order_id) + (MAX_LONG - TS) 
Columns：该订单各种状态
*订单历史表*
Rowkey：reverse(passenger_id | driver_id) + (MAX_LONG - TS) 
Columns：用户在时间范围内的订单及其他信息
场景二：司机乘客轨迹
这也是一份滴滴用户关系密切的数据，线上用户、滴滴的各个业务线和分析人员都会使用。举几个使用场景上的例子：用户查看历史订单时，地图上显示所经过的路线；发生司乘纠纷，客服调用订单轨迹复现场景；地图部门用户分析道路拥堵情况。
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/zxoLaeCI28RRmOxRibZOKKh3VS09ZqoIGpS6ib2vbIj9S1dpYFBA5DlSEDRnzYEsuJluXDAiaXhf0E556E4uUz7gw/?wx_fmt=png)
图2 司乘轨迹数据流程
用户们提出的需求：
- 
满足App用户或者后端分析人员的实时或准实时轨迹坐标查询；
- 
满足离线大规模的轨迹分析；
- 
满足给出一个指定的地理范围，取出范围内所有用户的轨迹或范围内出现过的用户。
其中，关于第三个需求，地理位置查询，我们知道MongoDB对于这种地理索引有源生的支持，但是在滴滴这种量级的情况下可能会发生存储瓶颈，HBase存储和扩展性上没有压力但是没有内置类似MongoDB地理位置索引的功能，没有就需要我们自己实现。通过调研，了解到关于地理索引有一套比较通用的GeohHash算法 。
GeoHash是将二维的经纬度转换成字符串，每一个字符串代表了某一矩形区域。也就是说，这个矩形区域内所有的点（经纬度坐标）都共享相同的GeoHash字符串，比如说我在悠唐酒店，我的一个朋友在旁边的悠唐购物广场，我们的经纬度点会得到相同的GeoHash串。这样既可以保护隐私（只表示大概区域位置而不是具体的点），又比较容易做缓存。
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/zxoLaeCI28RRmOxRibZOKKh3VS09ZqoIG21D0gGDSGrEfCOxAVhNCkEAmjvLAHhylKpWI3RahxjpPbTgfgiaC24A/?wx_fmt=png)
图3 GeoHash示意图
但是我们要查询的范围和GeohHash块可能不会完全重合。以圆形为例，查询时会出现如图4所示的一半在GeoHash块内，一半在外面的情况（如A、B、C、D、E、F、G等点）。这种情况就需要对GeoHash块内每个真实的GPS点进行第二次的过滤，通过原始的GPS点和圆心之间的距离，过滤掉不符合查询条件的数据。
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/zxoLaeCI28RRmOxRibZOKKh3VS09ZqoIGpXic8cj4pF8RZdwqZia19eibIwwtY1djXqm5gKGO1YPkWY9kysWPwYlYA/?wx_fmt=png)
图4 范围查询时，边界GeoHash块示意图
最后依据这个原理，把GeoHash和其他一些需要被索引的维度拼装成Rowkey，真实的GPS点为Value，在这个基础上封装成客户端，并且在客户端内部对查询逻辑和查询策略做出速度上的大幅优化，这样就把HBase变成了一个MongoDB一样支持地理位置索引的数据库。如果查询范围非常大（比如进行省级别的分析），还额外提供了MR的获取数据的入口。
两种查询场景的Rowkey设计如下：
- 
单个用户按订单或时间段查询： reverse(user_id) + (Integer.MAX_LONG-TS/1000)
- 
给定范围内的轨迹查询：reverse(geohash) + ts/1000 + user_id
场景三：ETA
ETA是指每次选好起始和目的地后，提示出的预估时间和价格。提示的预估到达时间和价格，最初版本是离线方式运行，后来改版通过HBase实现实时效果，把HBase当成一个KeyValue缓存，带来了减少训练时间、可多城市并行、减少人工干预的好处。 
整个ETA的过程如下：
- 
模型训练通过Spark Job，每30分钟对各个城市训练一次；
- 
模型训练第一阶段，在5分钟内，按照设定条件从HBase读取所有城市数据；
- 
模型训练第二阶段在25分钟内完成ETA的计算；
- 
HBase中的数据每隔一段时间会持久化至HDFS中，供新模型测试和新的特征提取。
Rowkey：salting+cited+type0+type1+type2+TS 
Column：order, feature
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/zxoLaeCI28RRmOxRibZOKKh3VS09ZqoIGmiaY0kTDoapJ0hKrGoRgQLsWL3nQdvaiago85WX7boLwf3K59NkUACIg/?wx_fmt=png)
图5 ETA数据流程
场景四：监控工具DCM
用于监控Hadoop集群的资源使用（Namenode，Yarn container使用等），关系数据库在时间维度过程以后会产生各种性能问题，同时我们又希望可以通过SQL做一些分析查询，所以使用Phoenix，使用采集程序定时录入数据，生产成报表，存入HBase，可以在秒级别返回查询结果，最后在前端做展示。
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/zxoLaeCI28RRmOxRibZOKKh3VS09ZqoIGlSz39pY0BsKxMlZLR14uxU53kyhfCCstpzSe5Tr1wQibyrrLhfhVlqA/?wx_fmt=png)
图6 DCM数据流程
图7、图8、图9是几张监控工具的用户UI，数字相关的部分做了模糊处理。
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/zxoLaeCI28RRmOxRibZOKKh3VS09ZqoIGvcQLIRzqZwGibnwGeFOJ4aH3zfrDeMmD0zu5JOr0SYEickKHEc3PSOmA/?wx_fmt=png)
图7 DCM HDFS按时间统计使用全量和增量
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/zxoLaeCI28RRmOxRibZOKKh3VS09ZqoIG8Z4cu53l33d3tDHAG0ibSH7VgoU2bxXkJQ0hDTKEgQtdSaGpKasy7ibg/?wx_fmt=png)
图8 DCM HDFS按用户统计文件数
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/zxoLaeCI28RRmOxRibZOKKh3VS09ZqoIGfjbJMOeoQWrAx8qribNibp0e7hpuJud2YEwQpYHNGQMkCeGiaQ22mVrXQ/?wx_fmt=png)
图9 DCM，MR Job运行结果统计
滴滴在HBase对多租户的管理
我们认为单集群多租户是最高效和节省精力的方案，但是由于HBase对多租户基本没有管理，使用上会遇到很多问题：在用户方面比如对资源使用情况不做分析、存储总量发生变化后不做调整和通知、项目上线下线没有计划、想要最多的资源和权限等；我们平台管理者也会遇到比如线上沟通难以理解用户的业务、对每个接入HBase的项目状态不清楚、不能判断出用户的需求是否合理、多租户在集群上发生资源竞争、问题定位和排查时间长等。
针对这些问题，我们开发了DHS系统（Didi HBase Service）进行项目管理，并且在HBase上通过Namespace、RS Group等技术来分割用户的资源、数据和权限。通过计算开销并计费的方法来管控资源分配。
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/zxoLaeCI28RRmOxRibZOKKh3VS09ZqoIGR2csqmAPylL3DReciaks3aQVnKH9zlYZXib8jAJdIlcACE2EfWiaNqZFA/?wx_fmt=png)
图10 DHS项目表监控
DHS主要有下面几个模块和功能：
- 
项目生命周期管理：包括立项、资源预估和申请、项目需求调整、需求讨论；
- 
用户管理：权限管理，项目审批；
- 
集群资源管理；
- 
表级别的使用情况监控：主要是读写监控、memstore、blockcache、locality。
当用户有使用HBase存储的需求，我们会让用户在DHS上注册项目。介绍业务的场景和产品相关的细节，以及是否有高SLA要求。
之后是新建表以及对表性能需求预估，我们要求用户对自己要使用的资源有一个准确的预估。如果用户难以估计，我们会以线上或者线下讨论的方式与用户讨论帮助确定这些信息。 
然后会生成项目概览页面，方便管理员和用户进行项目进展的跟踪。
HBase自带的jxm信息会汇总到Region和RegionServer级别的数据，管理员会经常用到，但是用户却很少关注这个级别。根据这种情况我们开发了HBase表级别的监控，并且会有权限控制，让业务RD只能看到和自己相关的表，清楚自己项目表的吞吐及存储占用情况。
通过DHS让用户明确自己使用资源情况的基础之上，我们使用了RS Group技术，把一个集群分成多个逻辑子集群，可以让用户选择独占或者共享资源。共享和独占各有自己的优缺点，如表1。
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/zxoLaeCI28RRmOxRibZOKKh3VS09ZqoIGodwLdOV5icibu4wQRAFTh2JXEufG5XDzNhukviciazBCjttnAzb0K3hb8Q/?wx_fmt=png)
表1 多租户共享和独占资源的优缺点
根据以上的情况，我们在资源分配上会根据业务的特性来选择不同方案：
- 
对于访问延迟要求低、访问量小、可用性要求低、备份或者测试阶段的数据：使用共享资源池；
- 
对于延迟敏感、吞吐要求高、高峰时段访问量大、可用性要求高、在线业务：让其独占一定机器数量构成的RegionServer Group资源，并且按用户预估的资源量，额外给出20%~30%的余量。
最后我们会根据用户对资源的使用，定期计算开销并向用户发出账单。
**RS Group**
RegionServer Group，实现细节可以参照HBase HBASE-6721这个Patch。滴滴在这个基础上作了一些分配策略上的优化，以便适合滴滴业务场景的修改。RS Group简单概括是指通过分配一批指定的RegionServer列表，成为一个RS Group，每个Group可以按需挂载不同的表，并且当Group内的表发生异常后，Region不会迁移到其他的Group。这样，每个Group就相当于一个逻辑上的子集群，通过这种方式达到资源隔离的效果，降低管理成本，不必为每个高SLA的业务线单独搭集群。
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/zxoLaeCI28RRmOxRibZOKKh3VS09ZqoIG5ejWvtLVPo7z7BeoXQBiaoKstkRygiaxU7zE8JYZyd388eh41J04uVzw/?wx_fmt=png)
图11 RS Group示意图
**总结**
在滴滴推广和实践HBase的工作中，我们认为至关重要的两点是帮助用户做出良好的表结构设计和资源的控制。有了这两个前提之后，后续出现问题的概率会大大降低。良好的表结构设计需要用户对HBase的实现有一个清晰的认识，大多数业务用户把更多精力放在了业务逻辑上，对架构实现知之甚少，这就需要平台管理者去不断帮助和引导，有了好的开端和成功案例后，通过这些用户再去向其他的业务方推广。资源隔离控制则帮助我们有效减少集群的数量，降低运维成本，让平台管理者从多集群无止尽的管理工作中解放出来，将更多精力投入到组件社区跟进和平台管理系统的研发工作中，使业务和平台都进入一个良性循环，提升用户的使用体验，更好地支持公司业务的发展。
推荐阅读：
1，[案例：Spark基于用户的协同过滤算法](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484291&idx=1&sn=4599b4e31c2190e363aa379a92794ace&chksm=9f38e0aba84f69bd5b78b48e31b3f5b3792ec40e2d25fdbe6bc735f9c98ceb4584462b08e439&scene=21#wechat_redirect)
2，[HBase原理和设计](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484241&idx=1&sn=2f4d59fe83d74a1bc764230a574972dc&chksm=9f38e079a84f696f32c2b823024164ea2c83d4775d24f02ca1d3b3ac6731a1172688001e9666&scene=21#wechat_redirect)
3，[大数据查询——HBase读写设计与实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484239&idx=1&sn=93c55d3a91f6ff9646389755844cabe7&chksm=9f38e067a84f697182d978ba8c3d89a4b3b7349beccbf070a9dc958b5738a83e6cd9bf24b49d&scene=21#wechat_redirect)
4，[如何整合复杂技术打造数据分析平台？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484129&idx=1&sn=15d5bd26315171fe92d3b749d4eeb30a&chksm=9f38e1c9a84f68df435762827b0667ebdfff55f338070acc7ab42cf13e06b6dbe43c79e48ef7&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**

