# 什么是Hadoop最大的挑战？ - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年01月19日 13:06:06[boonya](https://me.csdn.net/boonya)阅读数：572








This Article Is From：[https://dzone.com/articles/what-are-the-biggest-hadoop-challenges](https://dzone.com/articles/what-are-the-biggest-hadoop-challenges)




![](https://dz2cdn1.dzone.com/thumbnail?fid=64092&w=80)by[Sergey Tryuber](https://dzone.com/users/2528265/sergey-tryuber.html)·
Jul. 27, 15 · [Big Data Zone](https://dzone.com/big-data-analytics-tutorials-tools-news)



建议先看英文再看翻译：翻译使用的是Google翻译。


Hadoop已经变得更受欢迎。然而，随着其使用的增加，产生了许多误解。不要只使用Hadoop，应该正确地使用它。查看这些常见的Hadoop挑战。

了解[如何使用Apache
 Hadoop最大化云中的大数据](https://dzone.com/go?i=177153&u=http%3A%2F%2Fhortonworks.com%2Finfo%2Fmaximize-big-data-cloud-aws-ebook%2F%3Futm_medium%3Dsponsored-content%26utm_source%3Ddzone%26utm_campaign%3Daws)，立即下载此电子书。与[Hortonworks](https://dzone.com/go?i=177153&u=http%3A%2F%2Fhortonworks.com%2Finfo%2Fmaximize-big-data-cloud-aws-ebook%2F%3Futm_medium%3Dsponsored-content%26utm_source%3Ddzone%26utm_campaign%3Daws)合作为您带来。




许多公司在其IT基础架构中采用Hadoop。对于具有强大工程团队的旧大数据分发器，设计目标系统，选择技术堆栈并开始实施通常不是一个大问题。那些有很多经验的人有时候可能面临着所有复杂性的障碍，但是Hadoop初学者面临着无数挑战。以下是[Grid Dynamics](http://www.griddynamics.com/)为其客户解决的最常见的Hadoop挑战。

# 供应商的多样性，选择哪个？

通常的第一反应是使用来自[Apache](https://hadoop.apache.org/#Download+Hadoop)网站的原始Hadoop二进制文件，但是这导致实现为什么只有少数公司在生产环境中“原样”使用它们。有很多伟大的论据，不这样做。但是恐慌带来了[Hortonworks](http://hortonworks.com/hdp/)，[Cloudera](http://www.cloudera.com/content/cloudera/en/products-and-services/cdh.html)，[MapR](https://www.mapr.com/products/mapr-distribution-editions)可以免费提供多少Hadoop发行版，并结束于大型商业[IBM
 InfoSphere BigInsights](http://www-01.ibm.com/software/data/infosphere/hadoop/enterprise.html)和[Oracle大数据机](https://www.oracle.com/engineered-systems/big-data-appliance/index.html)。Oracle甚至包括硬件！在与供应商几次介绍性电话之后，事情变得更加混乱。选择正确的分发并不是一件容易的事情，即使是有经验的员工，因为他们每个人都嵌入不同的Hadoop组件（如CDH中的[Cloudera Impala](http://www.cloudera.com/content/cloudera/en/products-and-services/cdh/impala.html)），配置管理器（[Ambari](https://ambari.apache.org/)，[Cloudera
 Manager](http://www.cloudera.com/content/cloudera/en/products-and-services/cloudera-enterprise/cloudera-manager.html)等）以及Hadoop的总体愿景任务。


# Hadoop的SQL很受欢迎，但不清晰...

Hadoop存储了大量的数据。除了根据预定义的管道进行处理之外，企业希望通过给数据科学家和业务分析师提供交互式访问获得更多的价值。互联网上的营销嗡嗡声甚至迫使他们这样做，暗示，但不清楚说，与企业数据仓库的竞争力。这里的情况类似于供应商的多样性，因为有太多的框架提供“在Hadoop之上的交互式SQL”，但挑战不是选择最好的。了解目前他们都不是传统OLAP数据库的平等替代。与许多明显的战略优势同时，在性能，SQL兼容性和支持简单性方面存在争议的缺点。这是一个不同的世界，你应该玩它的规则，或者不认为它取代了传统的方法。


# 大数据工程师，有什么吗？

一个好的工程人员是任何IT组织的重要组成部分，但它在大数据中是至关重要的。在大多数情况下，依靠良好的Java / Python / C ++ /等工程师设计/实现高质量的数据处理流程意味着浪费数百万美元。经过两年的发展，你可能会得到不稳定，不可支持的，过度设计的混乱脚本/ jar，伴随着动物园的框架。如果主要开发商离开公司，情况变得绝望。在任何其他编程领域，经验丰富的大数据开发人员花费大部分时间思考如何保持简单，以及系统将如何评估。但是大数据技术堆栈的经验是一个关键因素。因此，挑战是找到这样的开发人员。

# 安全Hadoop环境，有点头疼




越来越多的公司在Hadoop中存储敏感数据。希望不是信用卡号码，但至少数据符合安全规定和相应的要求。所以这个挑战纯粹是技术性的，但往往导致问题。事情很简单，如果只有HDFS和MapReduce使用。
运动数据和静态加密都可用，文件系统权限足以进行授权，Kerberos用于身份验证。只需添加外围和主机级安全与显式边缘节点，并保持冷静。但是一旦你决定使用其他框架，特别是如果他们在自己的系统用户下执行请求，你就会陷入困境。第一个是，并不是所有的都支持Kerberized环境。第二，他们可能没有自己的授权功能。第三种是频繁缺少运动中数据加密。最后，如果请求被提交到集群外，很多麻烦。




# 总结

我们指出了我们看到的一些主要挑战。当然，上面的项目远不是完整的，一个可以被他们吓跑，导致决定不使用Hadoop或推迟其采用以后的时间。这不会明智。有一个完整的列表，Hadoop带给了具有巧手的组织的优势。与其他大数据框架和技术合作，它可以将面向数据的业务的能力转移到一个全新的性能水平。





Hortonworks DataFlow是一个集成的平台，使数据提取快速，方便，安全。[立即下载白皮书](https://dzone.com/go?i=133024&u=http%3A%2F%2Fhortonworks.com%2Finfo%2Fdata-ingestion%2F%3Futm_medium%3Dsponsored-content%26utm_source%3Ddzone%26utm_campaign%3Ddata-ingestion)。与[Hortonworks](https://dzone.com/go?i=133024&u=http%3A%2F%2Fhortonworks.com%2Finfo%2Fdata-ingestion%2F%3Futm_medium%3Dsponsored-content%26utm_source%3Ddzone%26utm_campaign%3Ddata-ingestion)合作为您带来。




