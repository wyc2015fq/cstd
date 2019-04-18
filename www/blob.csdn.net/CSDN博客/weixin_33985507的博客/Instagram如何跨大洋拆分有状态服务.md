# Instagram如何跨大洋拆分有状态服务 - weixin_33985507的博客 - CSDN博客
2018年12月01日 13:57:36[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
在最近举行的Large Installation System Administration（LISA）2018大会上，Instagram产品工程师Sherry Xiao解释了他们的团队如何将Instagram服务分散在美国和欧洲的数据中心。他们使用了Facebook工程团队的工具，在有状态服务（Cassandra和TAO）中解决了数据本地性（locality）问题。
Facebook在2012年收购了Instagram，后者迁移到了Facebook的基础设施上。Instagram的基础设施只部署在美国，而Facebook的数据中心位于美国和欧洲。Instagram的技术栈主要包括Django、Cassandra、TAO分布式数据存储、Memcached和Celery异步作业。他们不得不在美国和欧盟数据中心之间拆分服务，以解决数据存储空间限制问题。Cassandra quorum调用的高延迟，为数据本地性划分数据集，在欧盟区域内进行故障转移以及为TAO进行主副本同步，这些都是团队在进行服务拆分时必须克服的挑战。
![\"image\"](https://static.geekbang.org/infoq/5c0221dd73c20.png?imageView2/0/w/800)*图片来源——[https://www.youtube.com/watch?v=2GInt9E3vrU](%5C)*
Instagram使用Cassandra作为通用的键值存储服务。他们将Cassandra与其他组件从AWS转移到Facebook自己的数据中心。Cassandra在数据中心使用法定数量的副本来实现读写一致性。维护欧洲数据中心的数据副本会导致存储浪费，而quorum请求需要跨洋，延迟太高，不可取。Instagram团队改为使用名为Akkio的工具对数据进行分区，这样美国用户的数据位于美国数据中心，欧盟用户的数据位于欧盟的三个数据中心。Akkio是一款由Facebook开发的数据布局工具，可以对数据检索进行优化。它通过将数据分组为逻辑集来实现布局，然后将逻辑集存储在最靠近经常访问它们的最终用户的数据中心。Xiao说，Akkio“跟踪最终用户的访问模式并触发数据迁移”。
这种架构消除了在每个数据中心存储所有数据副本的必要。美国和欧盟的数据中心可以独立运作，quorum请求可以留在同一个大陆。Instagram还使用Social Hash分区器将请求路由到正确的存储桶，尤其是对于拥有大量关注者的帐户。
Instagram也使用了Facebook的TAO来存储社交图数据。在分片模式下，TAO的每个分片都有一个主节点。写入操作只会被转发给运行在美国数据中心的主节点，副本是只读的。该团队修改了TAO，让它可以将写入操作传给欧盟地区的本地主节点，避免跨洋调用。为什么不在这里使用Akkio？Xiao解释说，“与Cassandra相比，TAO拥有不同的数据模型。大多数用例都是使用用户ID作为键，数据属于用户”。相比之下，TAO处理的对象可以被全球各地的用户访问，因此Akkio无法实现最佳的数据本地性。
在最终的架构中，在前端有一个无状态的Django层，后面是分区的Cassandra和TAO，数据被写入本地主节点。迁移需要改变灾难恢复（DR）计划，因为延迟以及不同的数据集会导致无法实现跨洋的灾难恢复。Xiao说，每个地区都能够通过在每个数据中心保持20％的空闲容量来处理来自故障数据中心的负载。
查看英文原文：[https://www.infoq.com/news/2018/11/instagram-across-continents](%5C)
#### 相关推荐
![\"\"](https://static001.infoq.cn/resource/image/9f/98/9f19d48cfbbd02dcda09ca2ee752f498.jpg)
12 月 7 日北京 ArchSummit 全球架构师峰会上，来自 Google、Netflix、BAT、滴滴、美团 等公司技术讲师齐聚一堂，共同分享“微服务、金融技术、前端黑科技、智能运维等相关经验与实践。详情点击 [https://bj2018.archsummit.com/schedule](%5C)
