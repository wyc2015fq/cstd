# Pivotal开源基于PostgreSQL的数据库Greenplum - =朝晖= - 博客园
# [Pivotal开源基于PostgreSQL的数据库Greenplum](https://www.cnblogs.com/dhcn/p/7412484.html)
http://www.infoq.com/cn/news/2015/11/PostgreSQL-Pivotal
近日，Pivotal[宣布](http://blog.pivotal.io/big-data-pivotal/news/introducing-greenplum-worlds-first-open-source-mpp-data-warehouse)开源大规模并行处理（MPP）数据库Greenplum，其架构是针对大型分析型数据仓库和商业智能工作负载专门设计的。借助MPP这种高性能的系统架构，Greenplum可以将TB级的数据仓库负载分解，并使用所有的系统资源并行处理单个查询。
Greenplum数据库基于PostgreSQL开源技术。本质上讲，它是多个PostgreSQL实例一起充当一个数据库管理系统。Greenplum以PostgreSQL 8.2.15为基础构建，在SQL支持、特性、配置选项和终端用户功能方面非常像PostgreSQL，用户操作Greenplum就跟平常操作PostgreSQL一样。不过，为了支持Greenplum数据库的并发结构，PostgreSQL的内部构件经过了修补。例如，为了在所有并行的PostgreSQL数据实例上并发执行查询，系统目录、优化器、查询执行器以及事务管理器组件都经过了修改和增强。此外，Greenplum还引入了针对商业智能工作负载优化PostgreSQL的特性。例如，增加了并行数据加载、资源管理、查询优化、存储增强。这些功能是标准PostgreSQL所不具备的。
Greenplum数据库的架构如下：
![](http://cdn1.infoqstatic.com/statics_s1_20170822-0429/resource/news/2015/11/PostgreSQL-Pivotal/zh/resources/1.png)
Greenplum master是Greenplum数据库系统的入口，接受客户端连接及提交的SQL语句，将工作负载分发给其它数据库实例（segment实例），由它们存储和处理数据。Greenplum interconnect负责不同PostgreSQL实例之间的通信。Greenplum segment是独立的PostgreSQL数据库，每个segment存储一部分数据。大部分查询处理都由segment完成。
根据Pivotal的开源公告，他们希望Greenplum会成为一个重大的里程碑，永久改变数据仓库这个行业。Greenplum数据库与其它开源数据处理系统（如Apache Hadoop、MySQL甚或PostgreSQL）的差别在架构和功能上都有体现。借助MPP，Greenplum在大型数据集上执行复杂SQL分析的速度比他们测试过的任何一个方案都要快。而借助[下一代查询优化技术](http://blog.pivotal.io/big-data-pivotal/products/new-benchmark-results-pivotal-query-optimizer-speeds-up-big-data-queries-up-to-1000x)，Greenplum带来了其它开源方案中没有的数据管理质量特性、升级和扩展能力。他们相信，这样一款经过证明的、广泛采用的数据仓库开源将会在整个业界引发巨大的连锁反应。最重要的是，这降低了大规模实时数据分析的门槛，更多的公司可以参与到大数据所带来的挑战中来。
另据[InfoWorld报道](http://www.infoworld.com/article/2999396/big-data/greenplum-goes-open-source-and-a-new-cloud-analytics-star-is-born.html)，数据库行业分析师Curt Monash[将Greenplum视为分析型RDBMS的真正竞争者](http://www.dbms2.com/2015/02/18/greenplum-is-being-open-sourced/)。而且，相比现有的产品（如[Teradata](http://www.teradata.com/)、[HP Vertica](http://www8.hp.com/us/en/software-solutions/advanced-sql-big-data-analytics/)、[IBM Netezza](http://www.ibm.com/software/data/netezza/)和[Oracle Exadata](https://www.oracle.com/engineered-systems/exadata/index.html)），其引入成本更低。Greenplum作为一项服务似乎是个再简单不过的选择。它有一个为人熟知的名字和广泛的用户基础。MySQL或PostgreSQL也通过类似的技术提供云端服务。但是，Greenplum真要展现出其优势，需要做好两个方面的工作：一是从现有的Greenplum部署移植要简单；二是有一个可行的发展路线，要么可以通过其它云托管产品富集数据，要么集成新兴的分析技术，如Spark。
在[Hacker News](https://news.ycombinator.com/item?id=10462339)上，Pivotal Labs成员jacques_chester回答了多名网友的问题。网友tlrobinson提出：
> 
为什么Greenplum以PostgreSQL 8.2为基础，而不是更新的版本？
对此，jacques_chester解释说，“那是因为Greenplum最初从该版本派生。”网友djokkataja的问题也是围绕这一点：
> 
现在有计划吗？Greenplum最终是否会与现行的PostgreSQL开发有同等的特性，或者Greenplum主要还是遵循自己的发展路线？
jacques_chester并没有明确回答这个问题，只是说，这取决于许多因素。同时，他还指出：
> 
Greenplum采用PostgreSQL [Wire Protocol](https://en.wikipedia.org/wiki/Wire_protocol)。所有可以同PostgreSQL交互的工具都可以顺畅地同Greenplum交互。
还有网友担心Greenplum的单master会成为写入瓶颈，jacques_chester答复说，这是[gpfdist](http://gpdb.docs.pivotal.io/4330/utility_guide/admin_utilities/gpfdist.html)要解决的问题，只要正确使用，就可以实现批量并行加载，而且master不会成为瓶颈。
网友们还讨论了Greenplum与ElasticSearch的差别，感兴趣的读者可以进一步阅读。

