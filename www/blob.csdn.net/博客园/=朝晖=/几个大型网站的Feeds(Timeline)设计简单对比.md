# 几个大型网站的Feeds(Timeline)设计简单对比 - =朝晖= - 博客园
# [几个大型网站的Feeds(Timeline)设计简单对比](https://www.cnblogs.com/dhcn/p/7120965.html)
https://mp.weixin.qq.com/s?__biz=MjM5NzQ3ODAwMQ==&mid=404465806&idx=1&sn=3a68a786138538ffc452bca06a4892c8&scene=0#rd
Facebook起源的NewsFeed，以及Twitter起源的Timeline，核心问题都是如何处理巨大的消息(活动,activity)分发。“推Push”和“拉Pull”或者“推拉结合”，是主要的处理方式。
 以前各大网站陆续透露的文档，以及这次QCon2012 London和深圳的[架构](http://lib.csdn.net/base/architecture)师会议，较大程度的公开了各自的实现方式。本文从 消息分发模式；内部通信工具、协议；存储方式 3方面总结。 
各大网站都大量使用的Nginx, memcached, [MySQL](http://lib.csdn.net/base/mysql)等开源产品，都标配了，文中不再提。实现技术上，异步消息队列的引入，来模块解耦和尖峰削平；Cache的精良设计等，也都是各家大量使用的技能，可看参看文档，不再详述。
|推|Push, fan-out, Write-fanout|写时消息推送给粉丝。空间换时间|
|----|----|----|
|拉|Pull, fan-in, Read-fanout|读时拉取所有好友的消息，再聚合。时间换空间|
|混合|Hybrid|基于推，混入拉；基于拉，加速推。时空平衡|
## 1Facebook
参考《Facebook news-feed QCon12.pdf》。典型的Pull方式，读时fanout，获得所有好友的活动，再进行聚合，rank，排序等操作（这几步后续动作，是feed和timeline的最大不同特点）。Facebook把这种模式叫做“Multifeed – Multi-fetch and aggregate stories at read time”。 
![](http://mmbiz.qpic.cn/mmbiz/Bf4u9qKuXWuHr9TJvhoFByHBuhY7OYIpJ1ibwcUtj7EluQrJHRx8hTpTvicyqibfEbcu8Kljud6Q0wgiaO7X0IibMVg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)FB的众多产品、模块，通讯协议自然用自家的Thrift，还用到SMC和其他的底层平台。 
存储模块，有自家的“排序”存储文件（feed要按时间倒排，还有rank影响排序…内存的B树排序结构，可以预测性的合并到文件。可能开源）。还大量使用了 [Redis](http://lib.csdn.net/base/redis) 和Google开发的开源持久化KV存储： LevelDB。 
Feeds相对于Timeline，最大特点是有rank影响排序，需要按类型合并，有推荐[算法](http://lib.csdn.net/base/datastructure)的插入，有更复杂的[数据结构](http://lib.csdn.net/base/datastructure)…这些都是影响架构设计的重要因素，但这些都没有文档详细描述。拉模式下，最重要的是高效稳定、分布式的Aggregator的设计，也没有详细文档说明。 
(Facebook可以说是技术文档最不透明的网站了，特别是相较于他拥有最大的UGC而言。)
## 2Twitter
参考《TimelinesTwitter-QCon12.pdf》等众多文档。主要是推模式。Twitter的Timeline这种应用，和FB的Feed最大的区别，就是要解决fan-out的效率和全文搜索的效率。整体模块划分图： 
![](http://mmbiz.qpic.cn/mmbiz/Bf4u9qKuXWuHr9TJvhoFByHBuhY7OYIp3FrMuBsecQqibc59SPib1lDUKAsDBe5Jz6JwL3toHWc3BCNPQVrqnPGw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
主要特点是对fanout的处理：队列化(有自己用[Scala](http://lib.csdn.net/base/scala)语言实现的Kestrel队列)，并发处理推送等大消耗业务，各级缓存(包括In-Proc)… 
通讯协议上， Kestrel 复用了MemCached协议；而Timeline API模块使用了FB的Thrift。通信框架是大量使用的自己开发的（已开源）RPC框架 Finagle （A fault tolerant, protocol-agnostic RPC system）。 
[搜索引擎](http://lib.csdn.net/base/searchengine)使用了Lucene。存储也大量使用了[redis](http://lib.csdn.net/base/redis)。
## 3人人网
参考《人人网Feed系统结构浅析.pdf》和《人人网网站架构–服务化的演进》。作为中国的大型SNS网站，设计上也有很多自己的特色。 
从查询的效率考虑, 人人网采用了推模式(近似twitter模式)。但是，人人网的Feeds，又比twitter类的timeline，有更复杂的结构和功能需求，所以在设计上，会有FB和Twitter双方融合的特点。 
![](http://blog.csdn.net/dipolar/article/details/50156311)
在Cache上，人人有自己实现的Server来支持。特别是在IndexCache上，基本数据结构和FB一样，使用了C++ Boost multi-index [Container](http://lib.csdn.net/base/docker)；序列化和压缩采用Protobuf和QuickLZ。特别是有专门实现的解决feed索引持久化难题的Feed Index DB。 
最后用模板渲染引擎(也是C++实现)来显示复杂的Feed。 
Renren在网络通信上大量使用 ICE框架 ，协议上多用Protobuf，实现缓存等中间层、新鲜事儿等系统。大量自己开发的server集群，通过他们高效通信。 
在高性能计算上，Renren网倾向用C/C++编写定制性Server，保证数据中心存储，大规模数据尽量在进程内访问。像IndexCache Server（海量的Feed数据装载在单一Server内，实现“数据尽可能靠近CPU”的原则），实现高速排序等计算需求；此外还有文档里提及的渲染Server…都是用C写的专用Server。好处自然是本地内存的纳秒级访问速度，远远高于网络IO，可实现极高的性能。 
现在，人人网的架构也在向Service化方向发展，并封装成了XOA，基础总线使用了Thrift，消息队列用了ZeroMQ …
## 4新浪微博
参考TimYang的《 构建可扩展的微博架构 》和《新浪微博cache设计谈.pdf》 
虽然来源于Twitter，但不得不说，就数据量、复杂性而言，已经不弱于Twitter；稳定性更是高出了Twitter很多。新浪微博基础是拉模式，但是增加了“在线推”，对于在线用户有“Inbox Cache”加速对timeline的获取，减少aggregator的性能和时间消耗。结构如下图： 
![](http://blog.csdn.net/dipolar/article/details/50156311)
首页timeline获取步骤是：1.检查inbox cache是否可用; 2.获取关注列表; 3.聚合内容, 从 following 关系; 4.根据id list返回最终feed聚合内容。Sina的这种结合模式，符合中国的特点：明星海量粉丝(纯推送代价巨大)，个人用户关注多(纯拉取代价大)，且在线用户能得到极快的响应。 
存储大量使用了Redis。并且有专门的讲演，详细介绍了Sina在Redis的大规模应该场景。《 Redis介绍》 《 新浪微博开放平台Redis实践 》 
但是基于拉模式的aggragator没有对外介绍。此外，sina微博的消息机制、RPC框架，也未介绍。
## 5腾讯微博
参考《 张松国-腾讯微博架构介绍 08.pdf》。腾讯作为最有技术底子的公司，其架构有很多独特之处，参考和直接利用国外的网站的模式最少。腾讯微博采用“拉”模式，聚合计算aggregator采用了多级模式： 
![](http://mmbiz.qpic.cn/mmbiz/Bf4u9qKuXWuHr9TJvhoFByHBuhY7OYIpWWFfk70M1GgiaiaF1MIibzqrEBEZTCzpqUibh52BuAnKicmXGJRpZhSDGhA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
同大多的timeline系统一样，使用队列来异步化和解耦，不过qq的解耦包括了系统解耦和业务解耦(和Renren网的“中转单向RPC调用的消息队列”类似)，不但解耦模块，还使得各模块开发得以并行，提升开发效率。其主要架构图： 
![](http://mmbiz.qpic.cn/mmbiz/Bf4u9qKuXWuHr9TJvhoFByHBuhY7OYIpb8Rib3LL6DMTXkg2iblSz19uJRb4COFj7J2YkQJ3oA6uL4Nch9NCiclzQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
腾讯的积累，使得腾讯微博在平台化做的扎实。整个产品的“接口-服务”感觉清晰。在容灾容错方面更是比其它家（至少从文档上）高出了很多。集群建设，系统维护都沿袭了腾讯的积累，光海量日志的查询就用了Sphinx全文搜索。数据挖掘和分析（比如关系链分析、圈子挖掘、用户价值评估）也一直是腾讯的重点能力。 

