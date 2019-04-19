# 为什么 SQL 正在击败 NoSQL，数据的未来是什么？ - 文章 - 伯乐在线
原文出处： [Ajay Kulkarni](https://blog.timescale.com/why-sql-beating-nosql-what-this-means-for-future-of-data-time-series-database-348b777b847a)   译文出处：[Vincent](http://geek.csdn.net/news/detail/238939)
![](http://jbcdn2.b0.upaiyun.com/2017/10/22f63f78e9c9f35b288f0634be3a89f5.gif)
自从可以利用计算机做事以来，我们一直在收集的数据以指数级的速度在增长，因此对于数据存储、处理和分析技术的要求也越来越高。在过去的十年里，由于SQL无法满足这些要求，软件开发人员就抛弃了它，NoSQL也就因此而渐渐发展起来：MapReduce，Bigtable，Cassandra，MongoDB等等。
然而，如今SQL正在重新复出。云端的主要供应商们现在都提供了广受大众欢迎的托管关系型数据库服务：例如[Amazon RDS](https://aws.amazon.com/rds/)，[谷歌Cloud SQL](https://cloud.google.com/sql/docs/)，[Azure的PostgreSQL数据库](https://azure.microsoft.com/en-us/services/postgresql/)(Azure将于今年发布)。用亚马逊自己的话来说就是Aurora数据库结合了PostgreSQL和mysql数据库，因此该产品一直是“[AWS历史上增长最快的服务](http://www.businesswire.com/news/home/20161130006131/en/AWS-Extends-Amazon-Aurora-PostgreSQL-Compatibility)”。在Hadoop和Spark之上的SQL接口继续蓬勃发展。就在上个月，[Kafka推出了SQL支持](https://www.confluent.io/blog/ksql-open-source-streaming-sql-for-apache-kafka/)。
在这篇文章中，我们将研究SQL现在为什么会复出的原因，以及这对未来的数据社区工程和分析意味着什么。
# 第一章：新希望
为了理解为什么SQL会卷土重来，让我们先了解一下最初设计它的原因。
![](http://jbcdn2.b0.upaiyun.com/2017/10/72951783ccb91647b0b1ebee30d50620.gif)
好的故事都是起源于20世纪70年代
我们的故事始于20世纪70年代早期的IBM研究，那时关系型数据库就诞生了。当时的查询语言依赖于复杂的数学逻辑和符号。Donald Chamberlin和Raymond Boyce两个人刚刚完成哲学博士学位，对关系型数据模型印象深刻，但是发现查询语言将成为其发展的一个主要瓶颈。于是他们便开始设计一种新的查询语言(用他们自己的话说)：“[让那些没有接受过数学和计算机编程方面正规训练的用户更容易使用](http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=6359709)”。
![](http://jbcdn2.b0.upaiyun.com/2017/10/05572bb1723acf75c70333b932d338c1.png)
两个查询语言的比较 © ([source](http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=6359709))
仔细想想这件事。在互联网出现之前，在个人电脑出现之前，当编程语言C首次被引入世界时，两位年轻的计算机科学家意识到，“[计算机行业的成功很大程度上依赖于培养一种除了训练有素的计算机专家以外的用户](http://www.almaden.ibm.com/cs/people/chamberlin/sequel-1974.pdf)。”他们想要的是一种像英语一样易于阅读的查询语言，这也将包括数据库管理和操作。
其结果就是在1974年首次将SQL引入世界。在接下来的几十年里，SQL将被证明是非常受欢迎的。随着诸如System R、Ingres、DB2、Oracle、SQL Server、PostgreSQL、MySQL(等等)关系型数据库接管了软件行业，SQL也成为了与数据库交互的卓越语言，成为了一个日益拥挤、竞争激烈的生态系统的通用语言。
（遗憾的是，Raymond Boyce从来没有机会见证SQL的成功。1个月后[他便死于脑动脉瘤](https://en.wikipedia.org/wiki/Raymond_F._Boyce)，只做了一个最早的SQL演讲，当时他只有26岁，留下了一个妻子和一个年轻的女儿。）
有一段时间，似乎SQL成功地完成了它的任务。但后来互联网发生了。
# 第二章：NoSQL的反击
在Chamberlin和Boyce都在开发SQL的同时，令他们没有想到的是在加州的第二组工程师正在研究另一个正在萌芽的项目，该项目后来会广泛扩散并威胁到SQL的存在。这个项目就是[阿帕网](https://en.wikipedia.org/wiki/ARPANET)，1969年10月29日，[它诞生了](http://all-that-is-interesting.com/internet-history)。
![](http://jbcdn2.b0.upaiyun.com/2017/10/b027676ce3d4e5a5566c55927e1a77fc.png)
ARPANET的一些创造者，最终演变成今天的互联网([来源](http://all-that-is-interesting.com/internet-history))
SQL一直发展的都很好，但是直到1989年，另一个工程师出现并发明了[万维网](https://en.wikipedia.org/wiki/World_Wide_Web)。
![](http://jbcdn2.b0.upaiyun.com/2017/10/e0df1de91fb9ad4f9bbcf503b2a2aa30.png)
发明网络的物理学家([来源](https://webfoundation.org/about/vision/history-of-the-web/))
像那些茂密的野草一样，互联网和网络蓬勃发展，极大地扰乱了我们的世界，但对于数据社区来说，它还造成了一个特别的麻烦:跟以前相比，新的数据源以更高的数量和速度生成数据。
随着互联网的不断发展，软件社区发现，当时的关系型数据库无法处理这一新的负载。*因此出现了一阵骚动的力量，就好像一百万个数据库突然过载了。*
然后，两个新的互联网巨人取得了突破，并开发了他们自己的非关系型分布式系统来帮助解决这一新的数据冲击：由谷歌发布的**MapReduce**([2004年出版](https://static.googleusercontent.com/media/research.google.com/en//archive/mapreduce-osdi04.pdf))和**Bigtable**([2006年出版](https://static.googleusercontent.com/media/research.google.com/en//archive/bigtable-osdi06.pdf))，以及亚马逊(Amazon)发布的 **Dynamo** ([2007出版](http://www.allthingsdistributed.com/files/amazon-dynamo-sosp2007.pdf))。这些开创性的论文导致出现了更多的非关系数据库，包括**Hadoop**(基于MapReduce文件，[2006](https://en.wikipedia.org/wiki/Apache_Hadoop))，**Cassandra**(受Bigtable和Dynamo文件的启发，[2008年](https://en.wikipedia.org/wiki/Apache_Cassandra))和**MongoDB**([2009](https://en.wikipedia.org/wiki/MongoDB))。因为这些新系统基本上都是从零开始编写的，所以它们也没有使用SQL，导致了NoSQL运动的兴起。
开发者社区的软件工程师们也接受了NoSQL，而且跟SQL当时的出现相比，接受的群众范围更广了。这个原因很容易理解：NoSQL是现在流行的；它承诺了规模和权力；这似乎是项目通往成功的捷径。但后来出现了问题。
![](http://jbcdn2.b0.upaiyun.com/2017/10/40e1f5a569b1cb3e45d9c9d56bf25653.gif)
典型的被NoSQL诱惑的软件开发人员。不要学这家伙。
开发人员很快发现，没有SQL实际上是非常有限的。。每个NoSQL数据库都提供了自己独特的查询语言，这意味着：学习更多的语言(并在同事之间传播知识);增加了将数据库连接到应用程序的难度，导致代码之间有很强的耦合性;缺乏第三方生态系统，需要公司开发自己的操作和可视化工具。
这些NoSQL语言是新的，但也没有完全开发出来。例如，关系型数据库已经运行很多年了，像为SQL添加必要的特性(例如JOIN)这些工作早都已经完成了;NoSQL语言的不成熟意味着在应用程序级别就会有更多的复杂性。缺乏JOIN也导致了反规格化，从而又导致数据膨胀和僵化。
一些NoSQL数据库添加了自己的“类sql”查询语言，比如Cassandra的CQL。但这常常会使问题变得更糟。如果使用跟别的东西完全一样的界面，如果越常见，实际上会导致心理产生更多的疑问：工程师压根就不知道支持什么，不支持什么。
![](http://jbcdn2.b0.upaiyun.com/2017/10/788e6f314d2276358ead9cef3281be6f.gif)
类sql的查询语言就像[《星球大战》假日特别节目](https://www.youtube.com/watch?v=ZX0x-I06Fpc)。接受不模仿。
([而且总是避免《星球大战》的特别节目](https://xkcd.com/653/))
社区中的一些人在早期就看到了NoSQL的问题（[例如德维特和斯通布雷克在2008年就发现了](https://homes.cs.washington.edu/~billhowe/mapreduce_a_major_step_backwards.html)）。随着时间的推移，通过使用过程中个人经验的辛苦积累，越来越多的软件开发人员也同意了这一点。
# 第三章：SQL的回归
最初被黑暗势力所诱惑的软件社区开始看到了光明，SQL也上演了英雄回归的一幕。
首先是Hadoop上的SQL接口(Spark之后也是)，导致该行业兴起了NoSQL，NoSQL表示“不只是SQL”(Not Only SQL)。
紧接着NewSQL兴起了：完全接纳了SQL的新的可扩展数据库。来自于麻省理工学院(MIT)和布朗大学(Brown)研究人员的**H-Store**([2008年出版](http://hstore.cs.brown.edu/papers/hstore-demo.pdf))是最早的扩展OLTP数据库之一。谷歌再次引领了风向标，根据他们的**Spanner** 论文（[出版于2012年](https://static.googleusercontent.com/media/research.google.com/en//archive/spanner-osdi2012.pdf)）（其作者包括原始的MapReduce作者）开创了地缘重复的SQL界面的数据库，其次再是**CockroachDB**（[2014](https://en.wikipedia.org/wiki/Cockroach_Labs)）这样的其他先驱者。
与此同时，PostgreSQL社区开始复苏，添加了一些关键的改进，比如JSON数据类型(2012)，以及[PostgreSQL 10](https://wiki.postgresql.org/wiki/New_in_postgres_10)中的新特性的potpourri：对分区和复制更好的本地支持，支持对JSON的全文搜索，以及其它更多的特性(定于今年晚些时候发布的版本)。其他如**CitusDB**([2016](https://www.citusdata.com/blog/2016/03/24/citus-unforks-goes-open-source/))以及其他的公司([今年发布](https://www.citusdata.com/blog/2016/03/24/citus-unforks-goes-open-source/)的**[TimescaleDB](https://www.citusdata.com/blog/2016/03/24/citus-unforks-goes-open-source/)**)找到了新方法从而针对特定数据工作负载的扩展PostgreSQL。
![](http://jbcdn2.b0.upaiyun.com/2017/10/e1263fac9bef0ad66e743bcedef06752.png)
事实上，我们开发**[TimescaleDB](https://github.com/timescale/timescaledb)**的过程与这个行业的发展轨迹是密切相关。早期的TimescaleDB内部版本使用了我们自己的类sql查询语言“ioQL”。是的，我们也没能抵挡住黑暗一面的诱惑:我们感觉能够构建自己的查询语言应该会非常强大。然而，尽管这似乎是一条简单的道路，但我们很快意识到其实需要做更多的工作。我们还发现自己需要不断地去查找合适的语法，去查询那些已经可以用SQL进行查询的内容。
有一天，我们意识到构建自己的查询语言毫无意义。最关键的还是要接受SQL。这是我们做出的最好的设计决定之一。顿时，一个全新的世界出现了。现在尽管我们的数据库才问世5个月,但是用户却可以在生产环境上使用我们的数据库，还有很多其他的美好事物:可视化工具(Tableau),与常见的ORM的连接器,各种工具和备份选项,丰富的在线教程和语法解释等等。
# 信谷歌，得永生
![](http://jbcdn2.b0.upaiyun.com/2017/10/b27ea29357f08bd86515851503e79123.png)
谷歌已经在数据工程和基础架构领域领先了十多年了。我们应该密切关注他们正在做的事情。
看看谷歌的第二大**Spanner**论文，就在四个月前发布的([Spanner:成为一个SQL系统](https://static.googleusercontent.com/media/research.google.com/en//pubs/archive/46103.pdf)，2017年5月)，你会发现它支持我们的发现成果。
例如，谷歌开始的时候是在Bigtable上面构建，但后来发现不用SQL会造成很多问题(强调了我们下面的所有引用):
> 
虽然这些系统提供了数据库系统的某些优点，但它们缺少许多应用程序开发人员经常依赖的传统数据库特性。**举一个关键的例子就是一个健壮的查询语言**，这意味着开发人员必须编写复杂的代码来处理和聚合应用程序中的数据。**因此，我们决定将Spanner变成一个完整的SQL系统**，查询执行与Spanner的其他架构特性紧密集成(例如强一致性和全局复制)。
在论文的后面，他们进一步抓住了从NoSQL过渡到SQL的基本原理:
> 
Spanner的原始API提供了对单个和交叉表的点查找和范围扫描的NoSQL方法。虽然NoSQL方法提供了一个简单的启动扳手的方法，并且在简单的检索场景中继续有用，但是SQL在表达更复杂的数据访问模式和将计算推到数据上提供了重要的附加价值。
本文还描述了SQL的采用是如何在扳手上不停止的，但实际上扩展到了谷歌的其余部分，这里的多个系统现在共享一个通用的SQL方言:
> 
扳手的SQL引擎共享一个共同的SQL方言,称为“标准SQL”,与其他几个系统在谷歌上钻包括内部系统如F1和小孔(等)和外部系统如BigQuery…
对于谷歌的用户来说，这降低了跨系统工作的障碍。一个开发人员或数据分析人员编写了针对Spanner数据库的SQL，可以将他们对该语言的理解转移到Dremel，而不必担心语法、空处理等细微的差异。
这种方法的成功不言自明。Spanner已经成为主要谷歌系统的“真相之源”，包括AdWords和谷歌游戏，而“潜在的云客户对使用SQL非常感兴趣”。
考虑到谷歌首先帮助发起了NoSQL运动，很值得注意的是，它现在正在接受SQL。(导致一些人最近想:“谷歌在10年的假时间里发送了大数据产业吗?”)
# 这对数据的未来意味着什么:SQL将变成细腰
在计算机网络中，有一个概念叫做“细腰结构”。
这个想法的出现解决了一个关键问题:在任何给定的网络设备上，想象一个堆栈，底层的硬件层和顶部的软件层。中间可能会存在各种网络硬件;同样，也存在存在各种各样的软件和应用程序。需要某种可以确保无论硬件发生了什么情况，软件仍然可以连接到网络的方法;同样的也能确保无论软件发生什么，网络硬件都知道如何处理网络请求。
![](http://jbcdn2.b0.upaiyun.com/2017/10/c10ce6a68e20e57c787edd1f6341e7b2.png)
在网络中，细腰的角色由[互联网协议(IP)](https://en.wikipedia.org/wiki/Internet_Protocol)扮演，它是为局域网设计的底层联网协议和更高级别的应用程序和传输协议的公共接口。([这是一个很好的解释](https://www.youtube.com/watch?v=uXumm52oBMo)。)而且(在一个广泛的简化中)，这个公共接口成为了计算机的通用语言，使网络能够相互连接，设备可以通信，而这种“网络网络”可以发展成为今天丰富多样的互联网。
**我们认为SQL已经成为数据分析的细腰。**
我们生活的时代，数据正在成为“世界上最有价值的资源”([《经济学人》，2017年5月](https://www.economist.com/news/leaders/21721656-data-economy-demands-new-approach-antitrust-rules-worlds-most-valuable-resource))。因此,我们看到了专业数据库(OLAP、时间序列、文档、图表等)，数据处理工具(Hadoop,Spark,Flink),数据总线(Kafka,RabbitMQ)等呈现出了寒武纪大爆发式的情形。我们也有了更多需要依靠这些数据基础设施的应用程序,无论是第三方数据可视化工具(Tableau,Grafana PowerBI,Superset),web框架(Rails,Django)或定制的数据驱动的应用程序。
![](http://jbcdn2.b0.upaiyun.com/2017/10/ffffebb9ec5cbd41d7c66b3f39209ffc.png)
像网络一样，我们也有一个复杂的堆栈，底层的基础设施和顶部的应用程序。通常，我们最终会编写大量的胶水代码来完成这个堆栈工作。但是胶水代码可能很脆弱:需要精心的运维。
我们需要的是一个公共接口，允许堆栈的各个部分彼此通信。理想情况下，这个行业已经标准化了。它能让不同层之间的通信阻碍能够降到最小。
这就是SQL的力量。和IP一样，SQL也是一个公共接口。
但SQL实际上比IP复杂得多。因为数据还需要支持人类分析。而且，SQL创建者最初给它设定的目标之一就是可读性要高。
SQL是完美的吗?不，但社区中的大多数人都已经了解了这门语言。虽然已经有工程师在开发更自然的语言界面，但是这些系统最终会连接到哪里?还是SQL。
所以在堆栈的顶部还有一层。那一层就是我们人类。
# SQL回归
SQL回来了。不只是因为在组装NoSQL工具时编写胶水代码的做法十分令人反感。不仅仅是因为学习各种各样的新语言是困难的。也不只是因为标准会带来各种优点。
也因为这个世界充满了数据。它包围着我们，束缚着我们。起初，我们依靠人类的感觉神经系统来处理它。现在，软件和硬件系统也变得足够智能，可以帮助我们。随着收集的数据越来越多，我们也可以更好地认识这个世界，系统的复杂性、存储、处理、分析以及对这些数据可视化的需求只会继续增长。
![](http://jbcdn2.b0.upaiyun.com/2017/10/067cfbb7c99c2c08bee927e5e293beea.gif)
*数据科学家尤达大师*
我们可以生活在满大街的系统都是如纸一般脆弱，接口量达到数百万个的世界里。 或者我们可以再次选择SQL，这样我们生活的世界也可能会变得越来越强大。
