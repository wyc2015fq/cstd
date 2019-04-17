# 学习Hadoop的前提条件 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年01月19日 12:31:34[boonya](https://me.csdn.net/boonya)阅读数：3038








This Article Is From :[https://examples.javacodegeeks.com/enterprise-java/apache-hadoop/prerequisites-learning-hadoop/](https://examples.javacodegeeks.com/enterprise-java/apache-hadoop/prerequisites-learning-hadoop/)

建议先看英文再看翻译：翻译使用的是Google翻译。


关于作者：Ramaninder毕业于德国Georg-August大学计算机科学与数学系，目前与奥地利的大数据研究中心合作。他拥有应用计算机科学硕士学位，专业应用系统工程和商业信息学。他也是一名微软认证过程，在Java，C＃，Web开发和相关技术方面有超过5年的经验。目前，他的主要兴趣是大数据生态系统，包括批处理和流处理系统，机器学习和Web应用程序。

在本文中，我们将深入了解什么是学习和使用Hadoop的先决条件。在开始学习Hadoop之前，我们将看到什么是需要的东西，什么是行业标准建议的事情。

# 1.介绍

Apache Hadoop是入门点，或者我们可以说是进入整个大数据生态系统的基础。它是大数据生态系统中大多数高级工具，应用程序和框架的基础，但是在学习Apache Hadoop时，还需要事先知道一些事情。

开始学习Apache Hadoop没有严格的先决条件。但是，它使事情更容易，如果你想成为和Apache Hadoop的专家，这些是很好的知道的东西。

因此，Apache Hadoop的两个非常基本的先决条件是：


- Java
- Linux
- SQL


我们将在下面的章节中介绍这些。


# 2.Java

知道Java不是使用Hadoop的严格先决条件，但是知道它显然是深入了解Apache
 Hadoop工作的额外优势。

这可能听起来很奇怪，我提到的第一个先决条件是Java和我保存这也不是严格的先决条件，但添加。好吧，让我们看看为什么这样。

有一些工具和应用程序，例如Pig，Hive等，它们构建在Hadoop之上。这些工具提供自己的高级交互语言，用于处理在Apache
 Hadoop集群上存储和处理的数据。例如，Pig Latin用于Pig，HiveQL用于Hive。因此，对于不想深入编写复杂的MapReduce应用程序但希望使用Hive或Pig与集群中的数据进行交互的人，可以跳过Java。

对于编写Hadoop MapReduce应用程序，Java也不是唯一的选择。Hadoop提供了一个选项，可以使用任何可以从标准输入读取的语言，并且可以使用称为Hadoop
 Streaming的组件写入标准输出以编写MapReduce程序。例如，Python，Ruby，C等。但是由于Apache Hadoop是用Java编写的，为了使组件尽可能接近，Java是一种语言。Pig
 Latin和HiveQL命令也在内部转换为Java MapReduce程序并执行。

所以，如果你想知道Apache Hadoop的坚果和螺栓，如果需求变得越来越复杂，Java是Apache Hadoop的先决条件。

注意：要了解有关Hadoop
 MapReduce，Hadoop Streaming和Hive的基础知识的更多信息，请按照以下文章：



- [Apache Hadoop WordCount Example](http://examples.javacodegeeks.com/enterprise-java/apache-hadoop/apache-hadoop-wordcount-example/)
- [Hadoop Streaming Example](http://examples.javacodegeeks.com/enterprise-java/apache-hadoop/hadoop-streaming-example/)
- [Apache Hadoop Hive Tutorial](http://examples.javacodegeeks.com/enterprise-java/apache-hadoop/apache-hadoop-hive-tutorial/)



**但是，当我们有Hadoop Steaming时，为什么要使用Java**
确实Hadoop Steaming提供了使用许多语言编写MapReduce程序的选项，但是目前只有Java API在Apache Hadoop中有一些优点和优点。

因此，Java不是学习Hadoop的严格先决条件，但是被行业用例强烈推荐。


# 3.Linux

虽然Apache Hadoop可以在Windows上运行，但它最初是在Linux上构建的。Linux是安装和管理Hadoop集群的首选方法。因此，了解Linux的工作和使用也有很多帮助。

当从命令行管理Hadoop分布式文件系统（HDFS）时，许多命令与Linus shell命令类似或完全相同。要了解HDFS和HDFS shell命令，请参阅文章：


- [Apache Hadoop Distributed File System Explained](http://examples.javacodegeeks.com/enterprise-java/apache-hadoop/apache-hadoop-distributed-file-system-explained/)
- [Apache Hadoop FS Commands Example](http://examples.javacodegeeks.com/enterprise-java/apache-hadoop/apache-hadoop-fs-commands-example/)



除此之外，我们还需要知道linux，如果我们想要部署和配置Hadoop集群甚至单节点机器。


# 4.SQL

对于已经熟悉SQL的人，他们可以利用他们现有的知识。
他们可以在Hive之上学习和使用SQL语法。Apache Hive查询语言几乎类似于ANSI SQL。除了Hive，Apache Pig还有很多类似于SQL命令的命令。例如，连接，分组，顺序等。不仅Apache Hadoop，而且其他大数据生态系统工具也提供类似SQL的接口，以便用户更容易地学习已经熟悉SQL的工具。Cassandra和HBase是一些工具，它们提供类似于查询接口的SQL，用于与数据交互。


# 5.总结

正如所讨论的，开始学习Apache Hadoop没有严格的先决条件，但在挖掘Apache Hadoop深入之前，我们应该熟悉一些事情。然后，我们一次讨论了这些先决条件，以了解它们在哪里和如何使用以及我们将需要它。在我们深入Apache Hadoop之前，了解一些或所有这些先决条件将是很好的。










