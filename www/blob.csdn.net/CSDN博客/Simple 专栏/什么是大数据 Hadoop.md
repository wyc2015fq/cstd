# 什么是大数据? Hadoop? - Simple 专栏 - CSDN博客
2018年09月28日 20:04:51[Simple_Zz](https://me.csdn.net/love284969214)阅读数：348
## 首先学大数据必会[Java](https://baike.baidu.com/item/java/85979)
- [Java](https://baike.baidu.com/item/Java/85979)是一门[面向对象](https://baike.baidu.com/item/%E9%9D%A2%E5%90%91%E5%AF%B9%E8%B1%A1)编程语言，不仅吸收了[C++](https://baike.baidu.com/item/C%2B%2B)语言的各种优点，还摒弃了C++里难以理解的[多继承](https://baike.baidu.com/item/%E5%A4%9A%E7%BB%A7%E6%89%BF)、[指针](https://baike.baidu.com/item/%E6%8C%87%E9%92%88/2878304)等概念，因此Java语言具有功能强大和简单易用两个特征。Java语言作为静态面向对象编程语言的代表，极好地实现了面向对象理论，允许程序员以优雅的思维方式进行复杂的编程 。
- Java具有简单性、面向对象、[分布式](https://baike.baidu.com/item/%E5%88%86%E5%B8%83%E5%BC%8F/19276232)、[健壮性](https://baike.baidu.com/item/%E5%81%A5%E5%A3%AE%E6%80%A7/4430133)、[安全性](https://baike.baidu.com/item/%E5%AE%89%E5%85%A8%E6%80%A7/7664678)、平台独立与可移植性、[多线程](https://baike.baidu.com/item/%E5%A4%9A%E7%BA%BF%E7%A8%8B/1190404)、动态性等特点 。Java可以编写[桌面应用程序](https://baike.baidu.com/item/%E6%A1%8C%E9%9D%A2%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F/2331979)、[Web应用程序](https://baike.baidu.com/item/Web%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F)、[分布式系统](https://baike.baidu.com/item/%E5%88%86%E5%B8%83%E5%BC%8F%E7%B3%BB%E7%BB%9F/4905336)和[嵌入式系统](https://baike.baidu.com/item/%E5%B5%8C%E5%85%A5%E5%BC%8F%E7%B3%BB%E7%BB%9F/186978)应用程序等 。
- 最重要的是，Hadoop以及其他大数据处理技术很多都是用Java，例如Apache的基于Java的HBase和Accumulo以及 ElasticSearchas，因此学习Hadoop的一个首要条件，就是掌握Java语言编程。
## 什么是[大数据](https://baike.baidu.com/item/%E5%A4%A7%E6%95%B0%E6%8D%AE/1356941?fr=aladdin)？
- 大数据（big data），指无法在一定时间范围内用常规软件工具进行捕捉、管理和处理的数据集合，是需要新处理模式才能具有更强的决策力、洞察发现力和流程优化能力的海量、高增长率和多样化的信息资产。
- 在[维克托·迈尔-舍恩伯格](https://baike.baidu.com/item/%E7%BB%B4%E5%85%8B%E6%89%98%C2%B7%E8%BF%88%E5%B0%94-%E8%88%8D%E6%81%A9%E4%BC%AF%E6%A0%BC)及肯尼斯·库克耶编写的《[大数据时代](https://baike.baidu.com/item/%E5%A4%A7%E6%95%B0%E6%8D%AE%E6%97%B6%E4%BB%A3/15434499)》  中大数据指不用随机分析法（[抽样调查](https://baike.baidu.com/item/%E6%8A%BD%E6%A0%B7%E8%B0%83%E6%9F%A5)）这样捷径，而采用所有数据进行分析处理。大数据的5V特点（IBM提出）：[Volume](https://baike.baidu.com/item/Volume/17610592)（大量）、[Velocity](https://baike.baidu.com/item/Velocity/1398152)（高速）、[Variety](https://baike.baidu.com/item/Variety/191328)（多样）、[Value](https://baike.baidu.com/item/Value/2285610)（低价值密度）、[Veracity](https://baike.baidu.com/item/Veracity/19362178)（真实性）。
## 什么是[Hadoop](https://baike.baidu.com/item/Hadoop)？
- 狭义的Hadoop：是一个适合大数据分布式存储(HDFS)、分布式计算(MapReduce)和资源调度(YARN)的平台 
- 广义的Hadoop：指的是Hadoop生态系统,Hadoop生态系统是一个很庞大的概念，Hadoop是其中最重要最基础的一个部分，生态系统中的每一个子系统只解决某一个特定的问题域(甚至可能很窄)，不搞统一型的一个全能系统，而是小而精的多个小系统。
- 生态圈特点：开源、社区活跃、囊括了大数据处理的方方面面、成熟的生态圈。
![Hadoop生态圈](https://img-blog.csdn.net/20180830202400936?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)Hadoop生态圈
        当下的 Hadoop 已经成长为一个庞大的体系，随着生态系统的成长，新出现的项目越来越多，其中不乏一些非 Apache 主管的项目，这些项目对 HADOOP 是很好的补充或者更高层的抽象。比如：
- [Hadoop](https://baike.baidu.com/item/Hadoop)：是一个由Apache基金会所开发的分布式系统基础架构。
	- [HDFS](https://baike.baidu.com/item/hdfs)：分布式文件系统
- [MapReduce](https://baike.baidu.com/item/MapReduce)：分布式运算程序开发框架
- [YARN](https://baike.baidu.com/item/yarn)：在Hadoop2.x中加入的，新的 Hadoop 通用资源管理系统
- [hive](https://baike.baidu.com/item/hive/67986)：基于 Hadoop 的分布式数据仓库，提供基于 SQL 的查询数据操作
- [HBase](https://baike.baidu.com/item/HBase)：基于 Hadoop 的分布式海量数据库
- [zookeeper](https://baike.baidu.com/item/zookeeper)：分布式协调服务基础组件
- [sqoop](https://baike.baidu.com/item/sqoop)：数据导入导出工具（比如用于 mysql 和 HDFS 之间）
- [flume](https://baike.baidu.com/item/flume/6250098)：日志数据采集框架
- [Impala](https://baike.baidu.com/item/Impala)：基于 Hadoop 的实时分析
## Hadoop能做什么？
- **搜索引擎**（Doug Cutting  设计Hadoop的初衷，为了针对大规模的网页快速建立索引）。
- **大数据存储**，利用Hadoop的分布式存储能力，例如数据备份、数据仓库等。
- **大数据处理**，利用Hadoop的分布式处理能力，例如数据挖掘、数据分析等。
- **科学研究**，Hadoop是一种分布式的开源框架，对于分布式计算有很大程度地参考价值。
- **总结**，Hadoop可以搭建大型数据仓库，可以做PB级数据的存储、处理、分析、统计等业务，还可以做搜索引擎、日志分析、商业智能、数据挖掘等等。
### Hadoop发展？
- Hadoop 是 Apache Lucene 创始人 Doug Cutting 创建的。最早起源于 Nutch，它是 Lucene 的子项目。Nutch 的设计目标是构建一个大型的全网搜索引擎，包括网页抓取、索引、查询等功能，但随着抓取网页数量的增加，遇到了严重的可扩展性问题：如何解决数十亿网页的存储和索引问题。
- 2003 年 Google 发表了一篇论文为该问题提供了可行的解决方案。论文中描述的是谷歌的产品架构，该架构称为：谷歌分布式文件系统（GFS）,可以解决他们在网页爬取和索引过程中产生的超大文件的存储需求。
- 2004 年 Google 发表论文向全世界介绍了谷歌版的 MapReduce 系统。同时期，Nutch 的开发人员完成了相应的开源实现 HDFS 和 MAPREDUCE，并从Nutch 中剥离成为独立项目 HADOOP，到 2008 年 1 月，HADOOP 成为 Apache 顶级项目，迎来了它的快速发展期。
- 2006 年 Google 发表了论文是关于 BigTable 的，这促使了后来的 Hbase 的发展。因此，Hadoop 及其生态圈的发展离不开 Google 的贡献。
**Hadoop的发展离不开Google的几篇论文**
- 源自Google的GFS论文，发表于2003年10月，后为Hadoop的HDFS。
- 源自Google的MapReduce论文，发表于2004年12月 ，后为Hadoop的MapReduce。
- 源自Google的BigTable论文，发表于2006年，后为Hadoop生态圈中的HBase。
## Hadoop  特性优点
- **扩容能力(**Scalable)：Hadoop 可在计算机集群间分配数据并完成计算任务，这些集群可方便的扩展到数以千计的节点中。
- **成本低**(Economical)：Hadoop 通过普通廉价的机器组成服务器集群来分发以及处理数据，以至于成本很低。
- **高效率**(Efficient)：通过并发数据，Hadoop 可在节点之间动态并行的移动数据，使得速度非常快。
- **可靠性(**Rellable)：能自动维护数据多份复制，并且在任务失败后能自动地重新部署计算任务。所以Hadoop 的按位存储和处理数据的能力值得人们信赖。
## Hadoop  国内外应用
- 不管是国内还是国外，Hadoop 最受青睐的行业是互联网领域，可以说互联网公司是 hadoop 的主要使用力量。
- 国外来说，Yahoo、Facebook、IBM 等公司都大量使用 hadoop 集群来支撑业务。比如：
	- Yahoo 的 Hadoop 应用在支持广告系统、用户行为分析、支持 Web 搜索等。
- Facebook 主要使用 Hadoop 存储内部日志与多维数据，并以此作为报告、分析和机器学习的数据源。
- 国内来说，BAT 领头的互联网公司是当仁不让的 Hadoop 使用者、维护者。比如Ali 云梯（14 年国内最大 Hadoop 集群）、百度的日志分析平台、推荐引擎系统等。
- 国内其他非互联网领域也有不少 hadoop 的应用，比如：
	- 金融行业： 个人征信分析
- 证券行业： 投资模型分析
- 交通行业： 车辆、路况监控分析
- 电信行业： 用户上网行为分析
- 总之：hadoop 并不会跟某种具体的行业或者某个具体的业务挂钩，它只是一种用来做海量数据分析处理的工具。
## Hadoop发行版的选择
- Apache Hadoop [官方网站](http://hadoop.apache.org/)
- CDH:Cloudera Distributed Hadoop [资源地址](http://archive.cloudera.com/cdh5/cdh/5/)
- HDP:Hortonworks Data Platform
