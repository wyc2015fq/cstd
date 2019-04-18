# OnLineML：时序数据挖掘 - wishchinYang的专栏 - CSDN博客
2014年07月16日 19:04:32[wishchin](https://me.csdn.net/wishchin)阅读数：3014
关于时序分析：
      我们跟随时间的脚步，试图解释现在、理解过去、甚至预测未来........
[原文链接](http://blog.sciencenet.cn/home.php?mod=space&uid=34250&do=blog&id=287173)：[http://blog.sciencenet.cn/home.php?mod=space&uid=34250&do=blog&id=287173](http://blog.sciencenet.cn/home.php?mod=space&uid=34250&do=blog&id=287173)
** 简介：**
        时间序列是一种重要的高维数据类型，它是由客观对象的某个物理量在不同时间点的采样值按照时间先后次序排列而组成的序列，在经济管理以及工程领域具有广 泛 应用。例如证券市场中股票的交易价格与交易量、外汇市场上的汇率、期货和黄金的交易价格以及各种类型的指数等，这些数据都形成一个持续不断的时间序 列。利 用时间序列数据挖掘，可以获得数据中蕴含的与时间相关的有用信息，实现知识的提取[1]。时间序列数据本身所具备的高维性、复杂性、动态性、高噪 声特性以 及容易达到大规模的特性，因此时间序列挖掘是数据挖掘研究中最具有挑战性的十大研究方向之一[2]。
        目前重点的研究内容包括时间序列的模式表 示、时间序列 的相似性度量和查询、时间序列的聚类、时间序列的异常检测、时间序列的分类、时间序列的预测等。 
**特点：**
        由于时间序列数 据本身所具备的高维性、复杂性、动态性、高噪声特性以及容易达到大规模的特性，直接在时间序列上进行数据挖掘不但在储存和计算上要花费高昂 代价而且可能 会影响算法的准确性和可靠性。
        时间**序列的模式表示**是一种对时间序列进行抽象和概括的特征表示方法，是在更高层次上对时间序列的重新描述[3, 4]。  时间序列的模式表示具有压缩数据、保持时间序列基本形态的功能，并且具有一定的除噪能力。
        常用的时间序列模式表示方法主要包含：频域表示法、分段线 性表示 法、符号表示法以及主成分分析表示法等。
频域表示的基本思想是将时间序列从时域通过傅里叶变换或小波变换映射到频域，用很少的低频系数来代表原来的 时间 序列数据，这种方法虽然数据浓缩的效率很高，但是对噪声敏感，而且不直观。
分段线性表示法的基本思想是用K个直线段来近似代替原来的时间序列，这种方 法 能够实现数据压缩的目的，而且允许在时间轴上进行缩放，但实现过程较复杂，且要求事先给出直线段数K。K值的选择是一个关键因素，太小则丢失有用信 息， 太大又会产生过多的冗余信息。
        时间序列的符号化表示就是通过一些离散化方法将时间序列的连续实数值或者一段时间内的时间序列波形映射到有限的符号表 上，将 时间序列转换为有限符号的有序集合。符号化表示的优点在于可以利用许多字符串研究领域的成果，缺点在于如何选择合适的离散化算法，解释符号的意 义，以及定 义符号之间的相似性度量。
        主成分分析是一种常见的降维方法。在时间序列的模式表示中，通过对整个时间序列数据库的整体表示实现对整个时间序列 数据库的特征 提取和压缩。其优点在于计算精度高且对噪声数据的鲁棒性强，但由于在奇异值分解过程中涉及到特征值计算，计算开销较大。
        时间序列的相似性度量是时间序列数据挖掘的基础[5, 6]。时间序列由于其特定的形状特征, 使得目前常用的一些相似性度量和聚类方法失去了原有的优越 性, 而几乎所有的时间序列挖掘算法都涉及到计算序列之间的相似性问题。目前，时间序列的相似性度量主要采用Lp范数（例如欧几里德距离）、动态时间弯曲 距离、 最长公共子序列、编辑距离、串匹配等。前两种相似性度量方法应用较为广泛。但是欧几里德距离不支持时间序列的线性漂移和时间弯曲，动态时间弯曲距 离的计算 量很大，不适合直接应用于海量时间序列的挖掘，从而限制了其在时间序列数据挖掘上的广泛应用。
**特征聚类方法：**
       虽然各种聚类方法已经在数 据挖掘领域中得到了较为深入的研究，但这些方法大多是针对关系数据库中的静态数据对象而提出的。然而在现实世界中越来越多的应用 涉及到流数据和时间序列 数据等随时间变化的复杂动态数据对象的聚类分析。由于时间序列数据与静态数据有着极大的不同，故对其进行聚类分析有着很大的复杂 性。
        近年来，涌现出许多 时间序列聚类方法[7]，这些时间序列数据聚类方法大体上可以分为三种，即基于原始数据的聚类、基于特征的聚类和基于模型的聚类。 其中后两种方法的核心 思想是利用时间序列的模式表示方法把时间序列数据转化为静态的特征数据或者是模型参数，然后再直接应用静态数据的聚类方法来完成聚类 任务。
       在对时间序列进行分析时, 经常希望能够发现这些时间序列在不同时间段的形态有何关联关系。这种关联关系一般表现为时间序列中频繁出现的变化模式和极少出现 的变化模式。这种极少出现 的变化模式称之为**异常模式**。在某些领域, 异常模式的发现对人们来说往往更有价值。例如, 医院可以从病人的心电图序列中发现 异常模式从而进行诊断和治疗。按照异常的表现形式不同, 线性时间和空间上时间序列的异常主要可以分为点异常和模式异常两种, 它们都是用于发现一条时间 序列上的异常情况的。
        模式异常是指在一条时间序列上与其他模式之间具有显著差异的模式。事实上, 点异常也可以认为是长度为1 的模式异常。目前已经提出 多种时间序列异常检测方法，例如基于人工免疫系统的时间序列异常检测[9]、基于支持向量聚类的时间序列异常检测[9]以及后缀 树和马尔可夫模型的时间 序列异常检测[10]。
**时间序列分类：**
       时间序列分类是时间序列数据分析中的重要任务之一. 不同于时间序列分析中常用的算法与问题，时间序列分类 是要把整个时间序列当作输入，其目的是要赋予这个序列某个离散标记。它比一般分类问题困难，主要在于 要分类的时间序列数据不等长，这使得一般的分类算法 不能直接应用。即使是等长的时间序列，由于不同序列在相同位置的数值一般不可直接比较，一般的分类算法 依然还是不适合直接应用。
        为了解决这些难点,通常 有两种方法：第一，定义合适的距离度量（最常用的距离度量是DTW距离），使得在此度量意义下相近的序列 有相同的分类标签，这类方法属于领域无关的方 法；第二，首先对时间序列建模（利用序列中前后数据的依赖关系建立模型），再用模型参数组成等长向量来表示每 条序列，最后用一般的分类算法进行训练和分 类，这类方法属于领域相关的方法。文[11]分析了两类方法，并且分别在不同的合成数据集和实际数据集上比较了 领域无关和领域相关的两类方法。结果发现 在训练数据较少时，使用领域相关的算法比较合适；另一方面，领域无关的算法受噪声的影响相对较少。
**预测：**
        预测是对尚未发生或目前还不明确 的事物进行预先的估计和推测，是在现时对事物将要发生的结果进行探讨和研究，简单地说就是指从已知事件测定未知事件。进行 预测的总原则是：认识事物的发 展变化规律，利用规律的必然性进行科学预测。时间序列预测主要包括三种基本方法：内生时间序列预测技术；外生时间序列预测技 术；主观时间序列预测技术。 时间序列分析与预测在经济[12]、金融[13]、工程[14]等领域有着广泛的应用，研究成果也最为丰富，将另文讨论。
![](http://blog.sciencenet.cn/upload/blog/images/2010/1/201011417016524.jpg)
**参考文献：**
1.       Keogh E, Kasetty S.On the need for time series data mining benchmarks: a survey and empirical demonstration.Data Mining and Knowledge
 Discovery, 2003, 7(4): 349-371.
2.        
Yang Qiang, Wu Xindong. 10 challenging problems in data mining research. International Journal of Information Technology & Decision Making, 2006, 5(4): 597-604.
3.      Lin J, Keogh E, Lonardi S, Chiu B.A
 symbolic representation of time series, with implications for streaming algorithms. Proceedings of the 8th ACM SIGMOD workshop on Research issues in data mining and knowledge discovery, 2003, Pages: 2 – 11.  
4.       Gullo F, Ponti G, Tagarelli A, Greco S. A time series representation model for accurate and fast
 similarity detection, Pattern Recognition, 2009, 42(11): 2998-3014.
5.      Gunopulos D, Das G. Time series similarity measures.KDD’00:
 Tutorial notes of the sixth ACM SIGKDD international conference on Knowledge discovery and data mining, 2000.
6.      Literatures on Similarity-based Time Series Retrieval.[http://www.cs.ust.hk/~leichen/readings/literaturesovertimeseries.htm](http://www.cs.ust.hk/~leichen/readings/literaturesovertimeseries.htm)
7.       Liao T W. Clustering of time series data: a survey. Pattern Recognition, 2005, 38: 1857-1874
8.      Dasgupta D, Forrest S. Novelty detection in time series data using ideas from immunology. In: Proceeding
 of the 5th International Conference on Intelligent Systems. 1996, Pages: 82- 87.
9.      Ma J, Perkins S. Time-series Novelty Detection Using One-class Support Vector Machines. Procedding
 of International Joint Conference on Neural Networks, 2003.
10.    Keogh E, Lonardi S. Finding surprising patterns in a time series database in linear time and space.
 Proceedings of the eighth ACM SIGKDD, 2002.
11.    杨一鸣，潘嵘，潘嘉林，杨强，李磊.**时间序列分类问题的算法比较**.
 计算机学报，2007，30（8）：1259-1265.
12.Clements M P（柯莱蒙兹），Hendry
 D F（韩德瑞），陆懋祖.预测经济时间序列.北京大学出版社，2008
13.    Tsay R S（蔡瑞胸），潘家柱译.金融时间序列分析.机械工业出版社，2006
14.    杨叔子．时间序列分析的工程应用（上下册）．第二版．华中科技大学出版社，2007
