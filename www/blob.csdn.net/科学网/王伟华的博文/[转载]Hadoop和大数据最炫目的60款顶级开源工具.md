# 科学网—[转载]Hadoop和大数据最炫目的60款顶级开源工具 - 王伟华的博文




# [转载]Hadoop和大数据最炫目的60款顶级开源工具                           

已有 1787 次阅读2016-1-24 01:04|个人分类:[计算机&大数据](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=11842&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=1)|文章来源:转载



说到处理大数据的工具，普通的开源解决方案（尤其是Apache Hadoop）堪称中流砥柱。弗雷斯特调研公司的分析师Mike Gualtieri最近预测，在接下来几年，“100%的大公司”会采用Hadoop。Market Research的一份报告预测，到2011年，Hadoop市场会以58%的年复合增长率（CAGR）高速增长；到2020年，市场产值会超过10亿美元。IBM更是非常看好开源大数据工具，派出了3500名研究人员开发Apache Spark，这个工具是Hadoop生态系统的一部分。




这回我们推出了最新的顶级开源大数据工具排行榜。这个领域最近方兴未艾，许多新项目纷纷启动。许多最知名的项目由Apache基金会管理，与Hadoop密切相关。

请注意：本文不是要搞什么排名；相反，项目按类别加以介绍。与往常一样，要是你知道另外的开源大数据及/或Hadoop工具应该榜上有名，欢迎留言交流。




**一、Hadoop相关工具**

1. Hadoop

Apache的Hadoop项目已几乎与大数据划上了等号。它不断壮大起来，已成为一个完整的生态系统，众多开源工具面向高度扩展的分布式计算。

支持的操作系统：Windows、Linux和OS X。

相关链接：[http://hadoop.apache.org](http://hadoop.apache.org)




2. Ambari

作为Hadoop生态系统的一部分，这个Apache项目提供了基于Web的直观界面，可用于配置、管理和监控Hadoop集群。有些开发人员想把Ambari的功能整合到自己的应用程序当中，Ambari也为他们提供了充分利用REST（代表性状态传输协议）的API。

支持的操作系统：Windows、Linux和OS X。

相关链接：[http://ambari.apache.org](http://ambari.apache.org)




3. Avro

这个Apache项目提供了数据序列化系统，拥有丰富的数据结构和紧凑格式。模式用JSON来定义，它很容易与动态语言整合起来。

支持的操作系统：与操作系统无关。

相关链接：[http://avro.apache.org](http://avro.apache.org)




4. Cascading

Cascading是一款基于Hadoop的应用程序开发平台。提供商业支持和培训服务。

支持的操作系统：与操作系统无关。

相关链接：[http://www.cascading.org/projects/cascading/](http://www.cascading.org/projects/cascading/)




5. Chukwa

Chukwa基于Hadoop，可以收集来自大型分布式系统的数据，用于监控。它还含有用于分析和显示数据的工具。

支持的操作系统：Linux和OS X。

相关链接：[http://chukwa.apache.org](http://chukwa.apache.org)




6. Flume

Flume可以从其他应用程序收集日志数据，然后将这些数据送入到Hadoop。官方网站声称：“它功能强大、具有容错性，还拥有可以调整优化的可靠性机制和许多故障切换及恢复机制。”

支持的操作系统：Linux和OS X。

相关链接：[https://cwiki.apache.org/confluence/display/FLUME/Home](https://cwiki.apache.org/confluence/display/FLUME/Home)




7. HBase

HBase是为有数十亿行和数百万列的超大表设计的，这是一种分布式数据库，可以对大数据进行随机性的实时读取/写入访问。它有点类似谷歌的Bigtable，不过基于Hadoop和Hadoop分布式文件系统（HDFS）而建。

支持的操作系统：与操作系统无关。

相关链接：[http://hbase.apache.org](http://hbase.apache.org)




8. Hadoop分布式文件系统（HDFS）

HDFS是面向Hadoop的文件系统，不过它也可以用作一种独立的分布式文件系统。它基于Java，具有容错性、高度扩展性和高度配置性。

支持的操作系统：Windows、Linux和OS X。

相关链接：[https://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-hdfs/HdfsUserGuide.html](https://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-hdfs/HdfsUserGuide.html)




9. Hive

Apache Hive是面向Hadoop生态系统的数据仓库。它让用户可以使用HiveQL查询和管理大数据，这是一种类似SQL的语言。

支持的操作系统：与操作系统无关。

相关链接：[http://hive.apache.org](http://hive.apache.org)




10. Hivemall

Hivemall结合了面向Hive的多种机器学习算法。它包括诸多高度扩展性算法，可用于数据分类、递归、推荐、k最近邻、异常检测和特征哈希。

支持的操作系统：与操作系统无关。

相关链接：[https://github.com/myui/hivemall](https://github.com/myui/hivemall)




11. Mahout

据官方网站声称，Mahout项目的目的是“为迅速构建可扩展、高性能的机器学习应用程序打造一个环境。”它包括用于在Hadoop MapReduce上进行数据挖掘的众多算法，还包括一些面向Scala和Spark环境的新颖算法。

支持的操作系统：与操作系统无关。

相关链接：[http://mahout.apache.org](http://mahout.apache.org)




12. MapReduce

作为Hadoop一个不可或缺的部分，MapReduce这种编程模型为处理大型分布式数据集提供了一种方法。它最初是由谷歌开发的，但现在也被本文介绍的另外几个大数据工具所使用，包括CouchDB、MongoDB和Riak。

支持的操作系统：与操作系统无关。

相关链接：[http://hadoop.apache.org/docs/current/hadoop-mapreduce-client/hadoop-mapreduce-client-core/MapReduceTutorial.html](http://hadoop.apache.org/docs/current/hadoop-mapreduce-client/hadoop-mapreduce-client-core/MapReduceTutorial.html)




13. Oozie

这种工作流程调度工具是为了管理Hadoop任务而专门设计的。它能够按照时间或按照数据可用情况触发任务，并与MapReduce、Pig、Hive、Sqoop及其他许多相关工具整合起来。

支持的操作系统：Linux和OS X。

相关链接：[http://oozie.apache.org](http://oozie.apache.org)




14. Pig

Apache Pig是一种面向分布式大数据分析的平台。它依赖一种名为Pig Latin的编程语言，拥有简化的并行编程、优化和可扩展性等优点。

支持的操作系统：与操作系统无关。

相关链接：[http://pig.apache.org](http://pig.apache.org)




15. Sqoop

企业经常需要在关系数据库与Hadoop之间传输数据，而Sqoop就是能完成这项任务的一款工具。它可以将数据导入到Hive或HBase，并从Hadoop导出到关系数据库管理系统（RDBMS）。

支持的操作系统：与操作系统无关。

相关链接：[http://sqoop.apache.org](http://sqoop.apache.org)




16. Spark

作为MapReduce之外的一种选择，Spark是一种数据处理引擎。它声称，用在内存中时，其速度比MapReduce最多快100倍；用在磁盘上时，其速度比MapReduce最多快10倍。它可以与Hadoop和Apache Mesos一起使用，也可以独立使用。

支持的操作系统：Windows、Linux和OS X。

相关链接：[http://spark.apache.org](http://spark.apache.org)




17. Tez

Tez建立在Apache Hadoop YARN的基础上，这是“一种应用程序框架，允许为任务构建一种复杂的有向无环图，以便处理数据。”它让Hive和Pig可以简化复杂的任务，而这些任务原本需要多个步骤才能完成。

支持的操作系统：Windows、Linux和OS X。

相关链接：[http://tez.apache.org](http://tez.apache.org)




18. Zookeeper

这种大数据管理工具自称是“一项集中式服务，可用于维护配置信息、命名、提供分布式同步以及提供群组服务。”它让Hadoop集群里面的节点可以彼此协调。

支持的操作系统：Linux、Windows（只适合开发环境）和OS X（只适合开发环境）。

相关链接：[http://zookeeper.apache.org](http://zookeeper.apache.org)




**二、大数据分析平台和工具**

19. Disco

Disco最初由诺基亚开发，这是一种分布式计算框架，与Hadoop一样，它也基于MapReduce。它包括一种分布式文件系统以及支持数十亿个键和值的数据库。

支持的操作系统：Linux和OS X。

相关链接：[http://discoproject.org](http://discoproject.org)




20. HPCC

作为Hadoop之外的一种选择，HPCC这种大数据平台承诺速度非常快，扩展性超强。除了免费社区版外，HPCC Systems还提供收费的企业版、收费模块、培训、咨询及其他服务。

支持的操作系统：Linux。

相关链接：[http://hpccsystems.com](http://hpccsystems.com)




21. Lumify

Lumify归Altamira科技公司（以国家安全技术而闻名）所有，这是一种开源大数据整合、分析和可视化平台。你只要在Try.Lumify.io试一下演示版，就能看看它的实际效果。

支持的操作系统：Linux。

相关链接：[http://www.jboss.org/infinispan.html](http://www.jboss.org/infinispan.html)




22. Pandas

Pandas项目包括基于Python编程语言的数据结构和数据分析工具。它让企业组织可以将Python用作R之外的一种选择，用于大数据分析项目。

支持的操作系统：Windows、Linux和OS X。

相关链接：[http://pandas.pydata.org](http://pandas.pydata.org)




23. Storm

Storm现在是一个Apache项目，它提供了实时处理大数据的功能（不像Hadoop只提供批任务处理）。其用户包括推特、美国天气频道、WebMD、阿里巴巴、Yelp、雅虎日本、Spotify、Group、Flipboard及其他许多公司。

支持的操作系统：Linux。

相关链接：[https://storm.apache.org](https://storm.apache.org)




**三、数据库/数据仓库**

24. Blazegraph

Blazegraph之前名为“Bigdata”，这是一种高度扩展、高性能的数据库。它既有使用开源许可证的版本，也有使用商业许可证的版本。

支持的操作系统：与操作系统无关。

相关链接：[http://www.systap.com/bigdata](http://www.systap.com/bigdata)




25. Cassandra

这种NoSQL数据库最初由Facebook开发，现已被1500多家企业组织使用，包括苹果、欧洲原子核研究组织（CERN）、康卡斯特、电子港湾、GitHub、GoDaddy、Hulu、Instagram、Intuit、Netfilx、Reddit及其他机构。它能支持超大规模集群；比如说，苹果部署的Cassandra系统就包括75000多个节点，拥有的数据量超过10 PB。

支持的操作系统：与操作系统无关。

相关链接：[http://cassandra.apache.org](http://cassandra.apache.org)




26. CouchDB

CouchDB号称是“一款完全拥抱互联网的数据库”，它将数据存储在JSON文档中，这种文档可以通过Web浏览器来查询，并且用JavaScript来处理。它易于使用，在分布式上网络上具有高可用性和高扩展性。

支持的操作系统：Windows、Linux、OS X和安卓。

相关链接：[http://couchdb.apache.org](http://couchdb.apache.org)




27. FlockDB

由推特开发的FlockDB是一种非常快、扩展性非常好的图形数据库，擅长存储社交网络数据。虽然它仍可用于下载，但是这个项目的开源版已有一段时间没有更新了。

支持的操作系统：与操作系统无关。

相关链接：[https://github.com/twitter/flockdb](https://github.com/twitter/flockdb)




28. Hibari

这个基于Erlang的项目自称是“一种分布式有序键值存储系统，保证拥有很强的一致性”。它最初是由Gemini Mobile Technologies开发的，现在已被欧洲和亚洲的几家电信运营商所使用。

支持的操作系统：与操作系统无关。

相关链接：[http://hibari.github.io/hibari-doc/](http://hibari.github.io/hibari-doc/)




29. Hypertable

Hypertable是一种与Hadoop兼容的大数据数据库，承诺性能超高，其用户包括电子港湾、百度、高朋、Yelp及另外许多互联网公司。提供商业支持服务。

支持的操作系统：Linux和OS X。

相关链接：[http://hypertable.org](http://hypertable.org)




30. Impala

Cloudera声称，基于SQL的Impala数据库是“面向Apache Hadoop的领先的开源分析数据库”。它可以作为一款独立产品来下载，又是Cloudera的商业大数据产品的一部分。

支持的操作系统：Linux和OS X。

相关链接：[http://www.cloudera.com/content/cloudera/en/products-and-services/cdh/impala.html](http://www.cloudera.com/content/cloudera/en/products-and-services/cdh/impala.html)




31. InfoBright社区版

InfoBright为数据分析而设计，这是一种面向列的数据库，具有很高的压缩比。InfoBright.com提供基于同一代码的收费产品，提供支持服务。

支持的操作系统：Windows和Linux。

相关链接：[http://www.infobright.org](http://www.infobright.org)




32. MongoDB

mongoDB的下载量已超过1000万人次，这是一种极其受欢迎的NoSQL数据库。MongoDB.com上提供了企业版、支持、培训及相关产品和服务。

支持的操作系统：Windows、Linux、OS X和Solaris。

相关链接：[http://www.mongodb.org](http://www.mongodb.org)




33. Neo4j

Neo4j自称是“速度最快、扩展性最佳的原生图形数据库”，它承诺具有大规模扩展性、快速的密码查询性能和经过改进的开发效率。用户包括电子港湾、必能宝（Pitney Bowes）、沃尔玛、德国汉莎航空公司和CrunchBase。

支持的操作系统：Windows和Linux。

相关链接：[http://neo4j.org](http://neo4j.org)




34. OrientDB

这款多模型数据库结合了图形数据库的一些功能和文档数据库的一些功能。提供收费支持、培训和咨询等服务。

支持的操作系统：与操作系统无关。

相关链接：[http://www.orientdb.org/index.htm](http://www.orientdb.org/index.htm)




35. Pivotal Greenplum Database

Pivotal声称，Greenplum是“同类中最佳的企业级分析数据库”，能够非常快速地对庞大的海量数据进行功能强大的分析。它是Pivotal大数据库套件的一部分。

支持的操作系统：Windows、Linux和OS X。

相关链接：[http://pivotal.io/big-data/pivotal-greenplum-database](http://pivotal.io/big-data/pivotal-greenplum-database)




36. Riak

Riak“功能完备”，有两个版本：KV是分布式NoSQL数据库，S2提供了面向云环境的对象存储。它既有开源版，也有商业版，还有支持Spark、Redis和Solr的附件。

支持的操作系统：Linux和OS X。

相关链接：[http://basho.com/riak-0-10-is-full-of-great-stuff/](http://basho.com/riak-0-10-is-full-of-great-stuff/)




37. Redis

Redis现在由Pivotal赞助，这是一种键值缓存和存储系统。提供收费支持。要注意：虽然该项目并不正式支持Windows，不过微软在GitHub上有一个Windows派生版。

支持的操作系统：Linux。

相关链接：[http://redis.io](http://redis.io)




**四、商业智能**

38. Talend Open Studio

Talend的下载量已超过200万人次，其开源软件提供了数据整合功能。该公司还开发收费的大数据、云、数据整合、应用程序整合和主数据管理等工具。其用户包括美国国际集团（AIG）、康卡斯特、电子港湾、通用电气、三星、Ticketmaster和韦里逊等企业组织。

支持的操作系统：Windows、Linux和OS X。

相关链接：[http://www.talend.com/index.php](http://www.talend.com/index.php)




39. Jaspersoft

Jaspersoft提供了灵活、可嵌入的商业智能工具，用户包括众多企业组织：高朋、冠群科技、美国农业部、爱立信、时代华纳有线电视、奥林匹克钢铁、内斯拉斯加大学和通用动力公司。除了开源社区版外，它还提供收费的报表版、亚马逊网络服务（AWS）版、专业版和企业版。

支持的操作系统：与操作系统无关。

相关链接：[http://www.jaspersoft.com](http://www.jaspersoft.com)




40. Pentaho

Pentaho归日立数据系统公司所有，它提供了一系列数据整合和业务分析工具。官方网站上提供了三个社区版；访问Pentaho.com，即可了解收费支持版方面的信息。

支持的操作系统：Windows、Linux和OS X。

相关链接：[http://community.pentaho.com](http://community.pentaho.com)




41. SpagoBI

Spago被市场分析师们称为“开源领袖”，它提供商业智能、中间件和质量保证软件，另外还提供Java EE应用程序开发框架。该软件百分之分免费、开源，不过也提供收费的支持、咨询、培训及其他服务。

支持的操作系统：与操作系统无关。

相关链接：[http://www.spagoworld.org/xwiki/bin/view/SpagoWorld/](http://www.spagoworld.org/xwiki/bin/view/SpagoWorld/)




42. KNIME

KNIME的全称是“康斯坦茨信息挖掘工具”（Konstanz Information Miner），这是一种开源分析和报表平台。提供了几个商业和开源扩展件，以增强其功能。

支持的操作系统：Windows、Linux和OS X。

相关链接：[http://www.knime.org](http://www.knime.org)




43. BIRT

BIRT的全称是“商业智能和报表工具”。它提供的一种平台可用于制作可以嵌入到应用程序和网站中的可视化元素及报表。它是Eclipse社区的一部分，得到了Actuate、IBM和Innovent Solutions的支持。

支持的操作系统：与操作系统无关。

相关链接：[http://www.eclipse.org/birt/](http://www.eclipse.org/birt/)




**五、数据挖掘**

44.DataMelt

作为jHepWork的后续者，DataMelt可以处理数学运算、数据挖掘、统计分析和数据可视化等任务。它支持Java及相关的编程语言，包括Jython、Groovy、JRuby和Beanshell。

支持的操作系统：与操作系统无关。

相关链接：[http://jwork.org/dmelt/](http://jwork.org/dmelt/)




45. KEEL

KEEL的全称是“基于进化学习的知识提取”，这是一种基于Java的机器学习工具，为一系列大数据任务提供了算法。它还有助于评估算法在处理递归、分类、集群、模式挖掘及类似任务时的效果。

支持的操作系统：与操作系统无关。

相关链接：[http://keel.es](http://keel.es)




46. Orange

Orange认为数据挖掘应该是“硕果累累、妙趣横生”，无论你是有多年的丰富经验，还是刚开始接触这个领域。它提供了可视化编程和Python脚本工具，可用于数据可视化和分析。

支持的操作系统：Windows、Linux和OS X。

相关链接：[http://orange.biolab.si](http://orange.biolab.si)




47. RapidMiner

RapidMiner声称拥有250000多个用户，包括贝宝、德勤、电子港湾、思科和大众。它提供一系列广泛的开源版和收费版，不过要注意：免费的开源版只支持CSV格式或Excel格式的数据。

支持的操作系统：与操作系统无关。

相关链接：[https://rapidminer.com](https://rapidminer.com)




48. Rattle

Rattle的全称是“易学易用的R分析工具”。它为R编程语言提供了一种图形化界面，简化了这些过程：构建数据的统计或可视化摘要、构建模型以及执行数据转换。

支持的操作系统：Windows、Linux和OS X。

相关链接：[http://rattle.togaware.com](http://rattle.togaware.com)




49. SPMF

SPMF现在包括93种算法，可用于顺序模式挖掘、关联规则挖掘、项集挖掘、顺序规则挖掘和集群。它可以独立使用，也可以整合到其他基于Java的程序中。

支持的操作系统：与操作系统无关。

相关链接：[http://www.philippe-fournier-viger.com/spmf/](http://www.philippe-fournier-viger.com/spmf/)




50. Weka

怀卡托知识分析环境（Weka）是一组基于Java的机器学习算法，面向数据挖掘。它可以执行数据预处理、分类、递归、集群、关联规则和可视化。

支持的操作系统：Windows、Linux和OS X。

相关链接：[http://www.cs.waikato.ac.nz/~ml/weka/](http://www.cs.waikato.ac.nz/~ml/weka/)




**六、查询引擎**

51. Drill

这个Apache项目让用户可以使用基于SQL的查询，查询Hadoop、NoSQL数据库和云存储服务。它可用于数据挖掘和即席查询，它支持一系列广泛的数据库，包括HBase、MongoDB、MapR-DB、HDFS、MapR-FS、亚马逊S3、Azure Blob Storage、谷歌云存储和Swift。

支持的操作系统：Windows、Linux和OS X。

相关链接：[http://drill.apache.org](http://drill.apache.org)




**七、编程语言**

52. R

R类似S语言和环境，旨在处理统计计算和图形。它包括一套整合的大数据工具，可用于数据处理、计算和可视化。

支持的操作系统：Windows、Linux和OS X。

相关链接：[http://www.r-project.org](http://www.r-project.org)




53. ECL

企业控制语言（ECL）是开发人员用来在HPCC平台上构建大数据应用程序的语言。HPCC Systems官方网站上有集成开发环境（IDE）、教程以及处理该语言的众多相关工具。

支持的操作系统：Linux。

相关链接：[http://hpccsystems.com/download/docs/ecl-language-reference](http://hpccsystems.com/download/docs/ecl-language-reference)




**八、大数据搜索**

54. Lucene

基于Java的Lucene可以非常迅速地执行全文搜索。据官方网站声称，它在现代硬件上每小时能够检索超过150GB的数据，它含有强大而高效的搜索算法。开发工作得到了Apache软件基金会的赞助。

支持的操作系统：与操作系统无关。

相关链接：[http://lucene.apache.org/core/](http://lucene.apache.org/core/)




55. Solr

Solr基于Apache Lucene，是一种高度可靠、高度扩展的企业搜索平台。知名用户包括eHarmony、西尔斯、StubHub、Zappos、百思买、AT&T、Instagram、Netflix、彭博社和Travelocity。

支持的操作系统：与操作系统无关。

相关链接：[http://lucene.apache.org/solr/](http://lucene.apache.org/solr/)




**九、内存中技术**

56. Ignite

这个Apache项目自称是“一种高性能、整合式、分布式的内存中平台，可用于对大规模数据集执行实时计算和处理，速度比传统的基于磁盘的技术或闪存技术高出好几个数量级。”该平台包括数据网格、计算网格、服务网格、流媒体、Hadoop加速、高级集群、文件系统、消息传递、事件和数据结构等功能。

支持的操作系统：与操作系统无关。

相关链接：[https://ignite.incubator.apache.org](https://ignite.incubator.apache.org)




57. Terracotta

Terracotta声称其BigMemory技术是“世界上数一数二的内存中数据管理平台”，声称拥有210万开发人员，250家企业组织部署了其软件。该公司还提供商业版软件，另外提供支持、咨询和培训等服务。

支持的操作系统：与操作系统无关。

相关链接：[http://www.terracotta.org](http://www.terracotta.org)




58. Pivotal GemFire/Geode

今年早些时候，Pivotal宣布它将开放其大数据套件关键组件的源代码，其中包括GemFire内存中NoSQL数据库。它已向Apache软件基金会递交了一项提案，以便在“Geode”的名下管理GemFire数据库的核心引擎。还提供该软件的商业版。

支持的操作系统：Windows和Linux。

相关链接：[http://pivotal.io/big-data/pivotal-gemfire](http://pivotal.io/big-data/pivotal-gemfire)




59. GridGain

由Apache Ignite驱动的GridGrain提供内存中数据结构，用于迅速处理大数据，还提供基于同一技术的Hadoop加速器。它既有收费的企业版，也有免费的社区版，后者包括免费的基本支持。

支持的操作系统：Windows、Linux和OS X。

相关链接：[http://www.gridgain.com](http://www.gridgain.com)




60. Infinispan

作为一个红帽JBoss项目，基于Java的Infinispan是一种分布式内存中数据网格。它可以用作缓存、用作高性能NoSQL数据库，或者为诸多框架添加集群功能。

支持的操作系统：与操作系统无关。





转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-952138.html](http://blog.sciencenet.cn/blog-81613-952138.html)

上一篇：[​  在线协同利器$\pi$Lab简介](blog-81613-951967.html)
下一篇：[数学工作者的日常（音乐）](blog-81613-952210.html)


