# Alluxio 2.0：在云上实现超大规模数据工作负载 - Spark高级玩法 - CSDN博客
2019年04月14日 21:38:10[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：21
我们非常高兴和激动地宣布推出Alluxio 2.0预览版——这是本项目自创立以来包含最多新功能和改进的一次开源发布版本。Alluxio 2.0预览版现在可供下载（见文末链接1），这里（见文末链接2）有详细的发行说明。
## **一、构思和设计阶段**
当核心项目团队在若干个月之前开始考虑下一个Alluxio大型版本发布时，我们开始力求实现一些重要的总体目标。虽然之前版本的Alluxio已经为云环境中的许多大数据工作负载提供了数据本地性和数据可访问性，但在关键领域仍然需要进一步创新。
- 
**在规模方面取得阶跃式变化**——作为计算和存储之间的数据编排层，Alluxio使得数据能够移动，并且可以跨多个不同的存储系统（HDFS、对象存储、网络附加存储）访问。随着时间的推移，Alluxio需要支持管理的元数据规模会很轻易地超过最大规模的Hadoop部署。元数据管理尤其被认为是Hadoop的一个弱项，然而元数据的管理应该成为Alluxio的强项。
- 
**支持更多数据驱动的工作负载**——Alluxio在创立之初主要关注基于Hadoop的计算工作负载。但是多年来，数据密集型计算工作负载的数量和类型已经呈爆炸式增长，并且在现有数据存储系统或新数据存储系统上实现这些工作负载的数据编排和工程设计都非常重要。特别地，在机器学习和深度学习的训练过程之前，通常需要开展许多数据工程的工作，例如手动进行数据移动。Alluxio应该大大简化这一过程，为数据科学家提供已知原生API，减少所需的数据工程工作量。
- 
**使存储和计算更容易分离——**随着跨多个Hadoop集群的数据的增加，以及越来越多的数据存储在许多不同的对象存储中，或在某些情况下存储在内部或公有云中，企业中的数据仓库会不断增加。这使得从数据中分离计算变得更加困难，因为当数据在处理过程中移动到与其存储不同的位置时，数据本地性和可访问性会受到严重影响。Alluxio应当继续通过抽象存储来实现计算和存储的分离，同时使得数据更容易访问。
考虑到这些远大的目标，工程和产品团队在设计、实现、测试和压测中付出了不懈的努力，最终将Alluxio 2.0变为现实。
## **二、进步和功能**
     Alluxio 2.0包含许多增强功能，用以支持本项目的设计目标，这些功能全部是开源的，并都将包含在社区版(Community Edition)中！
### **2.1 支持超大规模数据工作负载**
- 
**支持超过****10****亿个文件——**2.0引入了分层元数据管理(tiered metadata management)这一新选项，以支持包含超过10亿个文件的单群集部署。我们现在默认使用RocksDB（见文末链接3）进行堆外存储。热数据的元数据继续存储在堆内的进程内存中，而其余元数据由Alluxio在进程内存外进行管理。alluxio.master.metastore(见文末链接4)可以配置为仅使用堆内存储。
- 
**高度分布式数据服务**——2.0引入了Alluxio作业服务(Job Service)，这是一种分布式集群服务，可以实现复制、持久化、跨存储移动和分布式加载等数据操作，从而实现高性能和大规模扩展。用户可以在这里查看Alluxio支持的所有文件系统API（见文末链接5）。
- 
**自适应副本以增强数据本地性**——该功能为Alluxio配置一定数量范围的自动管理的存储数据副本数。alluxio.user.file.replication.max和alluxio.user.file.replication.min可用于指定该范围。用户可在此处（见文末链接6）找到所有用户配置的完整列表。
- 
**内嵌式日志以达到高可用性——**2.0设计了一种称为内嵌式日志(embedded journal)的面向文件/对象元数据的新容错和高可用模式。内嵌式日志使用RAFT共识算法，并且实现方面独立于任何其他外部存储系统。这对于抽象对象存储特别有用。用户可以在这里(见文末链接7)了解如何配置内嵌式日志。
### **2.2 支持在任意存储上运行机器学习和深度学习工作负载**
机器学习和深度学习框架往往需要从Hadoop或对象存储中提取大规模数据，这通常是手动且非常耗时的过程。
- 
**Alluxio POSIX API**：Alluxio的FUSE功能支持POSIX兼容的API，因此通过Alluxio，TensorFlow、Caffe等框架以及其他基于Python的模型可以使用传统文件系统的访问方式直接访问任何存储系统中的数据。用户可以在POSIX API(见文末链接8)了解有关的更多信息。
### **2.3 更好的存储抽象，实现完全独立和弹性的计算**
- 
**支持跨不同版本的****HDFS****集群——**数据的爆炸式增长导致企业通常会拥有许多数据仓库，包括采用跨不同版本的多个Hadoop集群。目前，跨这些集群的统一访问非常困难。使用Alluxio 2.0，用户可以使用Alluxio连接到多个多种版本的HDFS集群，并实现统一的数据访问。用户可以在此处查找支持的HDFS版本列表。
- 
**与****Hadoop****主动同步——**该新功能是与HDFS iNotify进行对接集成，可对存储在Hadoop中的文件所发生的任何数据和元数据更改进行更新，允许通过Alluxio访问数据的应用程序能够主动接收最新更新。
## **三、反 馈**
我们非常期待您的反馈。现在Alluxio 2.0预览版已经可供下载使用，我们真诚地希望您试试Alluxio 2.0，并与我们分享你的体验——我们能够希望听到：您感到兴奋的内容，以及您认为哪些地方可以做得更好，或者您认为我们下一步应该关注什么。我们非常期待听到你的故事。请通过slack（见文末链接9）或电子邮件联系我们：info@alluxio.com，谢谢！
**更多精彩内容，请****点击文末“****阅读原文****”****关注Alluxio官方网站！**
**附录链接：**
链接1：
https://www.alluxio.org/download
链接2：
https://www.alluxio.org/download/releases/alluxio-200-preview-release
链接3:
https://rocksdb.org/
链接4：
https://www.alluxio.org/docs/2.0-preview/en/reference/Properties-List.html?q=alluxio.master. metastore#master-configuration
链接5:
https://www.alluxio.org/docs/2.0-preview/en/basic/Command-Line-Interface.html?q=File%20System%20Operations#file-system-operations
链接6:
https://www.alluxio.org/docs/2.0-preview/en/reference/Properties-List.html?q=replication#worker-configuration
链接7：
https://www.alluxio.org/docs/2.0-preview/en/operation/Journal.html?q=
job%20service#embedded-journal-configuration
链接8：
https://www.alluxio.org/docs/2.0-preview/en/api/POSIX-API.html
链接9：
https://www.alluxio.org/slack
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
