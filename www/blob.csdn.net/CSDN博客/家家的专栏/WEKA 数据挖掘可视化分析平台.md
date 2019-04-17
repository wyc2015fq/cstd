# WEKA 数据挖掘可视化分析平台 - 家家的专栏 - CSDN博客





2010年11月24日 20:48:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：6504








利用WEKA编写数据挖掘算法 　

　WEKA是由新西兰怀卡托大学开发的开源项目。WEKA是由JAVA编写的，并且限制在GNU通用公众证书的条件下发布，可以运行在所有的操作系统中。WEKA工作平台包含能处理所有标准数据挖掘问题的方法：回归、分类、聚类、关联规则挖掘以及属性选择。作为数据挖掘爱好者自然要对WEKA的源代码进行分析并以及改进，努力写出自己的数据挖掘算法。下面着重介绍一下如何利用WEKA编写新的数据挖掘算法： 注意：WEKA的版本有两个版本：稳定版(STABLE)和开发版(DEVELOP)，不同WEKA版本与不同JDK的版本匹配，稳定版WEKA3-4的与JDK1.4.2匹配，而开发版WEKA3-5与JDK1.5匹配，WEKA3-5新加入了对数据库的数据连接。稳定版直接下载weka-src.jar文件就行了，而开发版需使用CVS连接到sourceForge下载，:pserver:cvs_anon@cvs.scms.waikato.ac.nz:/usr/local/global-cvs/ml_cvs。本文以稳定版为例。 一、首先从WEKA官方网站（[http://www.cs.waikato.ac.nz/ml/weka](http://www.cs.waikato.ac.nz/ml/weka)）下载WEKA程序包。将程序包解压获得weka-src.jar源文件，再将源代码解压缩导入某个JAVA开发工具中（图1），如：JBuilder，Eclipse，Netbeans等。我现在以Netbeans为例。



  Weka--最著名的数据挖掘开源项目 收藏 
WEKA（ developed at the University of Waikato in New Zealand 新西兰怀卡托大学）[http://www.cs.waikato.ac.nz/~ml/weka/index.html](http://www.cs.waikato.ac.nz/~ml/weka/index.html)

与此开源项目对应的书为《Data Mining Practical Machine Learning Tools and Techniques Second Edition 》，该书下载地址[http://www.itpub.net/showthread.php?s=&threadid=731436](http://www.itpub.net/showthread.php?s=&threadid=731436)，另外本书已有中文版《数据挖掘，实用机器学习技术》。

如果想研究里面算法的具体实现，可以用Eclipse调试。见[http://weka.sourceforge.net/wiki/index.php/Eclipse_3.0.x](http://weka.sourceforge.net/wiki/index.php/Eclipse_3.0.x)

Weka 3: Data Mining Software in Java 

Weka is a collection of machine learning algorithms for data mining tasks. The algorithms can either be applied directly to a dataset or called from your own Java code. Weka contains tools for data pre-processing, classification, regression, clustering, association rules, and visualization. It is also well-suited for developing new machine learning schemes. 

Weka 3：开源的数据挖掘软件（Java）

Weka 是实施数据挖掘任务所需的各种机器学习算法的合集。这些算法既可以直接应用到某数据集上，也可以在你自己设计的Java程序调用它们。Weka 包含了下列工具：数据预处理，分类，回归，聚类，关联规则，以及可视化。另外也可以在Weka 的基础上开发新的机器学习。

Weka主要致力于classifier and filter algorithms。

以下转载自[http://blog.donews.com/carouter/archive/2005/07/11/462134.aspx](http://blog.donews.com/carouter/archive/2005/07/11/462134.aspx)

数据挖掘终于可以和烦人的代码们说再见了! Weka，一个不足两岁的新生儿，让数据挖掘轻松易行，无需编程也能轻松搞定。

   Weka是基于java，用于数据挖掘和知识分析一个平台。来自世界各地的java爱好者们都可以把自己的算法放在这个平台上，然后从海量数据中发掘其背后隐藏的种种关系；也许你只是出于对数据的狂热爱好，但也许你的发现会蕴含着无限的商机。

  打开Weka，首先出现一个命令行窗口。原以为要在这个命令行下写java语句呢，不过稍等一秒，Weka GUIChooser的出现了。这是一个很简单的窗体，提供四个按钮：SimpleCLI、Explorer、Experimenter、KnowledgeFlow。SimpleCLI应该是一个使用命令行的界面，有点像SAS的编辑器；Explorer是则是视窗模式下的数据挖掘工具；Experimenter和KnowledgeFlow的使用有待进一步摸索....

   先打开WekaExlporer感受一下它的强大吧。它有六个标签页，分别是Preprocess、Classify、Cluster、Associate、Selectattributes、Visualize。在Preprocess中Open一个数据文件(Weka使用的数据文件是.arff，其实是一个文本数据集，格式并不复杂，用notepad打开一看就明白了)。当然也可以Open URL或Open DB，不过我没有check一下支持哪些DB。
  打开数据文件后，可以使用Filter进行一下过滤，相当于“预处理的预处理”。Filter提供了许多算法来过滤数据，比如filters/unsupervised/instance/normalize应该是一个标准化的算法。当然，也可以编写你自己的算法! 
   这时窗体上已经给出这个数据集的一些基本特征了，比如有多少属性，各属性的一些简单统计量，右下方还给出一些可视化效果比如柱状图。通过这些可以初步了解这个数据集了。但这些都是很直观的可以看出来，好戏在后头，隐藏的关系即将登场。

   接下来的两个标签页是classify(分类)和cluster(聚类)，接触数据挖掘的人对它们一定不会陌生。同样Weka有许多分类和聚类算法可供选择，在这里面称为clasifier和clusterer。不过Weka提供的classify功能似乎还不够灵活，只能定长度和定频率地分类。但这个关系不大，现在很多数据处理软件都可以做到这个，比如excel。Cluster功能强大，提供了许多巧妙的聚类算法，选定一个算法，给出你所需要生成的聚类数目，就可以自动完成。当然如果能不给出聚类数目也能自动聚类的话就更佳了，不过我还没发现怎么做。

    Next，终于到伟大的Associate了! 这是一个用于发掘AssociateRules(关联规则)的模块。对商学略有涉猎的人一定熟知沃尔玛发现了啤酒和尿布销售的关系这一佳话。有了WekaAssociate，任何一家超市都可以做到这一点了。将前面导入的数据使用Associator进行发掘，就可以发现其中无数隐藏的关系。Weka-3-4提供了Apriori、PredictiveApriori、Tertius三种关联规则发掘算法，不过我感觉这已经够用了。选定一个算法，进行一些必要的设置，包括支持度上界、下界，每次运算的支持度递减值，等等。另外一个重要的参数：所需要生成的关联规则个数。太不可思议了，以前我们能从海量数据中发现一个关联规则就已经沾沾自喜，现在Weka居然问你想生成多少关联规则! 
    参数设置完成，点Start，就可以去喝茶了。不一会，10条关联规则已经生成，可以提交给老板了。当然，你还可以分析一下哪些规则比较有用，哪一条有潜在收益，这就需要business sense了。

    另外两个标签页还没怎么看。Selectattributes大概是针对单属性的分析？Visualize则提供了许多可视化效果，需要拿出去演示时很方便。不过今天使用感觉这个模块的功能有点问题，没太搞懂。也有可能是我用错了。

   Weka实在是一个伟大的工具。基于java，却没有运行其它java程序那种慢吞吞的感觉。前天我还在说Data Mining isexcruciating but interesting，有了Weka，Data Mining也可以轻轻松松了!



出处：[http://blog.csdn.net/wwweducn/archive/2006/12/17/1447215.aspx](http://blog.csdn.net/wwweducn/archive/2006/12/17/1447215.aspx)

[http://blog.chinaunix.net/u1/34560/showart_269880.html](http://blog.chinaunix.net/u1/34560/showart_269880.html)



