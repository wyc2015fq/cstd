# OnLineML一：关于Jubatus 的简介... - wishchinYang的专栏 - CSDN博客
2014年04月30日 17:09:12[wishchin](https://me.csdn.net/wishchin)阅读数：920
个人分类：[MLandPy																[时序/变长分析](https://blog.csdn.net/wishchin/article/category/6034695)](https://blog.csdn.net/wishchin/article/category/1508471)
**一：简介：**原文链接：jubat.us/en/  [xuwenq.iteye.com/blog/1702746](xuwenq.iteye.com/blog/1702746)
Jubatus [http://jubat.us/en/overview.html](http://jubat.us/en/overview.html) 是一个面向大数据数据流的分布式在线机器学习的开源框架，和storm有些类似，但是从介绍上来看，它提供了更多的功能。 
 Jubatus认为未来的数据分析平台应该同时向三个方向展开：处理更大的数据，深层次的分析和实时处理；而当前还没有一种能够处理不断生成的流式大数据的水平可扩展的分布式架构。Hadoop的mapreduce能够处理大数据，但不能做复杂的机器学习算法；Apache Mahout是基于Hadoop的机器学习平台，但不适用于在线处理数据流。 
Jubatus将在线机器学习，分布式计算和随机算法等的优势结合在一起用于机器学习，并支持分类，回归，推荐等基本元素。根据其设计目的，Jubatus有如下的特点： 
- 可扩展:支持可扩展的机器学习处理。在普通硬件集群上处理数据速度高达100000条/秒
- 实时计算:实时分析数据和更新模型
- 深层次的数据分析:支持各种分析计算：分类，回归，统计，推荐等
  Jubatus还是一个很年轻的项目，当前最新的发布版本是0.3.2(c++), 但暂时还没有看到有商业使用的例子；如果有基于流数据的机器学习方面的需求，还是关注一下的。
**二：又一链接**：[blog.csdn.net/jixuan1989/article/details/7880978](blog.csdn.net/jixuan1989/article/details/7880978)
**Abstract:**In
 the coming era of extremely large databases, computer science will face new challenges in real Big Data applications such as nation-wide M2M sensor network analysis, online advertising optimization for millions of consumers, and real-time security monitoring
 on the raw Internet traffic. In such applications, it is impractical or useless to apply ordinary approaches for data analysis on small datasets by storing all data into databases, analyzing the data on the databases as a batch-processing, and only visualizing
 the summarized output. In fact, the future of data analytics platform should expand to three directions at the same time, handling even bigger data, applying deep analytics, and processing in real-time. However, there has been no such analytics platform for
 massive data streams of continuously generated Big Data with a distributed scale-out architecture. For example, Hadoop is not equipped with sophisticated machine learning algorithms since most of the algorithms do not fit its MapReduce paradigm. Though Apache
 Mahout is also a Hadoop-based machine learning platform, online processing of data streams is still out of the scope.
在即将到来的超大规模数据库的时代，计算机科学将在实时大数据应用上面临新的挑战，比如全国M2M传感器网络分析，面向百万级别用户的在线广告优化，和互联网流量的实时安全监控。在这些应用中，使用传统的用来处理小数据集的方式进行数据分析是不切合实际的，这种传统方式往往把所有数据存在数据库中、使用一个批处理在数据库中分析数据、并且仅仅可视化输出概要数据。事实上，未来的数据分析平台应该同时向三个方向展开：处理更大的数据、深层的分析、实时处理。然而，在分布式水平扩展架构上还没有这样的分析平台来处理不断生成大数据的数据流。比如说，由于大多数算法无法适应Hadoop
 的Map/Reduce框架，因此 Hadoop 不能做复杂的机器学习算法。尽管Apache Mahout 也是一个基于Hadoop的机器学习平台，但在线处理数据流仍然超出了他的能力范围。
Jubatus
 is the first open source platform for online distributed machine learning on the data streams of Big Data. We use a loose model sharing architecture for efficient training and sharing of machine learning models, by defining
 three fundamental operations; Update, Mix, and Analyze, in a similar way with the Map and Reduce operations in Hadoop. The point is how to reduce the size of model and the number of the Mix operations while keeping high accuracy, since Mix-ing large models
 for many times causes high networking cost and high latency in the distributed environment. Then our development team includes competent researchers who combine the latest advances in online machine learning, distributed computing, and randomized algorithms
 to provide efficient machine learning features for Jubatus. Currently, Jubatus supports basic tasks including classification, regression, and recommendation. A demo system for tweet categorization on fast Twitter data streams is available.
Jubatus是第一个面向大数据数据流的分布式在线机器学习的开源平台。我们使用一个松散的模型通过定义了三种基本操作来共享有效训练的架构
 并且共享机器学习模型，这三种方式做事：更新、混合、分析，这是一种和Hadoop中的Map 、Reduce操作类似的方式。关键点是如何在保持高精准度的同时来减小模型的大小和混合操作的数量，因为多次混合大模型将导致在分布式环境下的高网络消耗和高潜伏期。我们的开发团队中有这样的研究者：他们结合了在在线机器学习、分布式计算和随机算法中的最新的优点以提供Jubatus高效的机器学习特点。目前，Jubatus支持基本的任务，包括分类、回归和推荐。一个在Twitter的数据上的信息分类演示系统已经可用了。
三：项目主页：[jubat.us/en/](jubat.us/en/)
**Jubatus **is a distributed processing framework and streaming machine learning library. Jubatus includes these functionalities:
- Online Machine Learning Library: Classification, Regression, Recommendation (Nearest Neighbor Search), Graph Mining, Anomaly Detection, Clustering
- Feature Vector Converter (fv_converter): Data Preprocess and Feature Extraction
- Framework for Distributed Online Machine Learning with Fault Tolerance
## Table of Contents
- [Quick Start](http://jubat.us/en/quickstart.html)- [Install Jubatus](http://jubat.us/en/quickstart.html#install-jubatus)- [Red Hat Enterprise Linux 6.2 or later
 (64-bit)](http://jubat.us/en/quickstart.html#red-hat-enterprise-linux-6-2-or-later-64-bit)
- [Ubuntu Server 12.04 LTS (64-bit)](http://jubat.us/en/quickstart.html#ubuntu-server-12-04-lts-64-bit)
- [Other Linux Distributions (including 32-bit)](http://jubat.us/en/quickstart.html#other-linux-distributions-including-32-bit)
- [Mac OS X](http://jubat.us/en/quickstart.html#mac-os-x)
- [Install Jubatus Client Libraries](http://jubat.us/en/quickstart.html#install-jubatus-client-libraries)- [C++](http://jubat.us/en/quickstart.html#c)
- [Python](http://jubat.us/en/quickstart.html#python)
- [Ruby](http://jubat.us/en/quickstart.html#ruby)
- [Java](http://jubat.us/en/quickstart.html#java)
- [Try Tutorial](http://jubat.us/en/quickstart.html#try-tutorial)
- [Write Your Application](http://jubat.us/en/quickstart.html#write-your-application)
- [Overview](http://jubat.us/en/overview.html)- [Scalable](http://jubat.us/en/overview.html#scalable)
- [Real-Time](http://jubat.us/en/overview.html#real-time)
- [Deep-Analysis](http://jubat.us/en/overview.html#deep-analysis)
- [Difference from Hadoop and Mahout](http://jubat.us/en/overview.html#difference-from-hadoop-and-mahout)
- [Tutorial](http://jubat.us/en/tutorial.html)- [Scenario](http://jubat.us/en/tutorial.html#scenario)
- [Run Tutorial](http://jubat.us/en/tutorial.html#run-tutorial)
- [Tutorial in Detail](http://jubat.us/en/tutorial.html#tutorial-in-detail)- [Dataset](http://jubat.us/en/tutorial.html#dataset)
- [Server Configuration](http://jubat.us/en/tutorial.html#server-configuration)
- [Use of Classifier API: Train & Classify](http://jubat.us/en/tutorial.html#use-of-classifier-api-train-classify)
- [Other Tutorials](http://jubat.us/en/tutorial.html#other-tutorials)- [Classifier](http://jubat.us/en/tutorial/classifier.html)
- [Regression](http://jubat.us/en/tutorial/regression.html)
- [Graph](http://jubat.us/en/tutorial/graph.html)
- [Stat](http://jubat.us/en/tutorial/stat.html)
- [Setup in Distributed Mode](http://jubat.us/en/tutorial_distributed.html)- [Distributed Mode](http://jubat.us/en/tutorial_distributed.html#distributed-mode)- [Setup ZooKeeper](http://jubat.us/en/tutorial_distributed.html#setup-zookeeper)
- [Register configuration file to
 ZooKeeper](http://jubat.us/en/tutorial_distributed.html#register-configuration-file-to-zookeeper)
- [Jubatus Proxy](http://jubat.us/en/tutorial_distributed.html#jubatus-proxy)
- [Join Jubatus Servers to Cluster](http://jubat.us/en/tutorial_distributed.html#join-jubatus-servers-to-cluster)
- [Run Tutorial](http://jubat.us/en/tutorial_distributed.html#run-tutorial)
- [Cluster Management in Jubatus](http://jubat.us/en/tutorial_distributed.html#cluster-management-in-jubatus)- [ZooKeepers & Jubatus Proxies](http://jubat.us/en/tutorial_distributed.html#zookeepers-jubatus-proxies)
- [Jubavisor: Process Management Agent](http://jubat.us/en/tutorial_distributed.html#jubavisor-process-management-agent)
- [Documentation](http://jubat.us/en/documentation.html)- [Architecture](http://jubat.us/en/processes.html)
- [Data Conversion](http://jubat.us/en/fv_convert.html)- [Datum](http://jubat.us/en/fv_convert.html#datum)
- [Flow of Data Conversion](http://jubat.us/en/fv_convert.html#flow-of-data-conversion)
- [Filter](http://jubat.us/en/fv_convert.html#filter)
- [Feature Extraction from Strings](http://jubat.us/en/fv_convert.html#feature-extraction-from-strings)
- [Feature Extraction from Numbers](http://jubat.us/en/fv_convert.html#feature-extraction-from-numbers)
- [Feature Extraction from Binary Data](http://jubat.us/en/fv_convert.html#feature-extraction-from-binary-data)
- [Hashing Key of Feature Vector](http://jubat.us/en/fv_convert.html#hashing-key-of-feature-vector)
- [Plugins](http://jubat.us/en/fv_convert.html#plugins)
- [Plugin Development](http://jubat.us/en/plugin.html)- [Plugin for Data Conversion](http://jubat.us/en/plugin.html#plugin-for-data-conversion)
- [Cluster Administration Guide](http://jubat.us/en/admin.html)- [Recommended Process Configuration](http://jubat.us/en/admin.html#recommended-process-configuration)
- [Managing Clusters](http://jubat.us/en/admin.html#managing-clusters)
- [Monitoring](http://jubat.us/en/admin.html#monitoring)
- [Logging](http://jubat.us/en/admin.html#logging)
- [Save and Load](http://jubat.us/en/admin.html#save-and-load)
- [Building Jubatus from Source](http://jubat.us/en/build.html)- [Requirements](http://jubat.us/en/build.html#requirements)
- [Using Framework](http://jubat.us/en/framework.html)- [Using Code Generators](http://jubat.us/en/server.html)
- [How to Get Clients](http://jubat.us/en/howtogetclients.html)
- [RPC Error Handling](http://jubat.us/en/faq_rpc_err_workaround.html)- [Common Issues](http://jubat.us/en/faq_rpc_err_workaround.html#common-issues)
- [Recommendation for each client
 languages](http://jubat.us/en/faq_rpc_err_workaround.html#recommendation-for-each-client-languages)
- [Backup and Recovery](http://jubat.us/en/backup_and_recovery.html)- [Save and Load](http://jubat.us/en/backup_and_recovery.html#save-and-load)
- [Frequently Asked Questions (FAQs)](http://jubat.us/en/faq.html)- [Installation](http://jubat.us/en/faq.html#installation)
- [RPC Errors](http://jubat.us/en/faq.html#rpc-errors)
- [Distributed Environment](http://jubat.us/en/faq.html#distributed-environment)
- [Learning Model](http://jubat.us/en/faq.html#learning-model)
- [Anomaly detection](http://jubat.us/en/faq.html#anomaly-detection)
- [Miscellaneous](http://jubat.us/en/faq.html#miscellaneous)
- [References](http://jubat.us/en/references.html)- [Commands](http://jubat.us/en/commands.html)- [Jubatus Servers](http://jubat.us/en/commands.html#jubatus-servers)
- [Distributed Environment](http://jubat.us/en/commands.html#distributed-environment)
- [Utilities](http://jubat.us/en/commands.html#utilities)
- [Client API](http://jubat.us/en/api.html)- [Common Data Structures and Methods](http://jubat.us/en/common_structs.html)
- [Classifier](http://jubat.us/en/api_classifier.html)
- [Regression](http://jubat.us/en/api_regression.html)
- [Recommender](http://jubat.us/en/api_recommender.html)
- [Nearest Neighbor](http://jubat.us/en/api_nearest_neighbor.html)
- [Anomaly](http://jubat.us/en/api_anomaly.html)
- [Clustering](http://jubat.us/en/api_clustering.html)
- [Stat](http://jubat.us/en/api_stat.html)
- [Graph](http://jubat.us/en/api_graph.html)
- [How to Contribute](http://jubat.us/en/howtocontribute.html)- [We Welcome Your Contribution](http://jubat.us/en/howtocontribute.html#we-welcome-your-contribution)
- [Join the Community](http://jubat.us/en/howtocontribute.html#join-the-community)
- [Issue Openning Policy](http://jubat.us/en/howtocontribute.html#issue-openning-policy)
- [Pull-Request Policy](http://jubat.us/en/howtocontribute.html#pull-request-policy)
- [Tips for Contributors](http://jubat.us/en/howtocontribute.html#tips-for-contributors)
- [Miscellaneous](http://jubat.us/en/misc.html)- [Publications](http://jubat.us/en/misc.html#publications)- [2013](http://jubat.us/en/misc.html#id1)
- [2012](http://jubat.us/en/misc.html#id2)
- [2011](http://jubat.us/en/misc.html#id3)
- [Contributions (Thanks a lot!)](http://jubat.us/en/misc.html#contributions-thanks-a-lot)
- [About Us](http://jubat.us/en/aboutus.html)- [Jubatus Team Members](http://jubat.us/en/aboutus.html#jubatus-team-members)
待翻译........................
