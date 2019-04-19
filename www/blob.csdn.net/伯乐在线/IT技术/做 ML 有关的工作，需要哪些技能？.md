# 做 ML 有关的工作，需要哪些技能？ - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [Agatha](http://www.jobbole.com/members/kk3039) 翻译，[Panblack](http://www.jobbole.com/members/upanblack) 校稿。未经许可，禁止转载！
英文出处：[Quora](https://www.quora.com/What-skills-are-needed-for-machine-learning-jobs/answer/Jonathan-A-Cox)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
【导读】：有位网友在 Quora 上提问：做与机器学习有关的工作，需要些什么技能呢？
> 
我是一个自学者，现在家中自学线性代数。希望有天能在机器学习领域工作，但是我不太确定：
- a) 这种工作/面试需要什么技术上的技能？
- b) 有没有什么（做这种工作）必须的相关工作经历
比起空想，我至少开始行动了。任何的建议/指导对我来说都很有用。十分感谢！
**本文摘编了 2 位 Quora 网友的回答。**
### Jonathan A. Cox（机器学习和应用物理研究人员）的回答：
这里有很多回复都提到了一些创业公司常用的工具。值得注意的是，在你 35 岁以上的职业生涯中，这些工具会变得完全不同。所以最好的办法是熟悉基本概念和数学。
其次，机器学习领域太过繁杂，根本不可能真正深入理解几十种不同的算法。我觉得你可以先深究几个最有前景的算法。说是这么说，你也要很了解一些基础的东西才行，比如说朴素贝叶斯、支持向量机、感知器、决策树。
最后，很多创业公司，或者非研究领域的人会告诉你要专注于那些快要老掉牙的算法（比如支持向量机 (SVM)）和大数据的数据库工具（比如说Hadoop）。考虑到你刚刚起步，我觉得你可以专注于那些很快会发展起来的架构：神经网络，单指令流多数据流（SIMD）向量计算（比如说GPU)。神经网络最近 10 年内开始革命性地颠覆自然语言处理、机器视觉、语音识别和其他领域。鉴于它的强大，这个影响还会继续下去。
综上，我会选择诸如 GPU 加速这类算法来自学和实现，再加一些分布式数据库的知识。以下是值得考虑的算法：
1. 深度置信网或者层叠降噪自动编码机 / Deep Belief Nets or Stacked Denoising Autoencoders
2. 卷积神经网络（CNN）
3. 长短记忆（LSTM）时间递归神经网络
4. 结构递归神经网络（通常用于自然语言处理）
5. word2vec 神经网络以及相关通过上下文学习词语的相关算法
另外一个很有前景的领域是强化学习，尤其是 Deep Mind 的 Q-learning。当你用到延迟奖励功能（比如玩 Atari 游戏时的得分），并且不能在你的网络中直接计算参数梯度时，你会用到强度学习。当你熟悉了上述的算法之后，你大约会想探索一下这个领域。这个领域跟机器人以及，更现实一点，关于人和动物是怎么学习的模拟，很有关联。
补充：我现在有这样的想法：过去的一年中，结合不同的神经算法，得到一个“深度“系统的方法逐渐显露出重大进步；因为这个方法的整体大于它的组成部分。鉴于奠基石已经铺下，我预计最显著的进步很可能会落在这个方向。一定程度上，细致整顿整个系统之前训练好个体模块，以及更快的计算群让这种进步成为了可能。（然而我们仍会在单个的算法中看到进展。）
换句话说，现在结合了卷积神经网络和多层感知器（MLP）的 LSTM，可以做到用句子描述图像之类的事情（来源：Goolge/Stanford）。类似地，已经有了具备强化学习能力的卷积神经网络和 MLP 组成的 Atari 游戏竞技网络。
往这个清单加上新东西同样很容易：把通过 word2vec/GloVe/skip-gram 训练出来的词向量输送给这样的复合网络，或者甚至是用递归神经网络训练的情感分析层。
有了神经网络和反向传播，在任意几何体上结合/叠加层，以及把组合作为整体系统来训练都变得容易了，它们可以扩展出强大的能力。
由许多不同种类的神经网络组成，用各种技术连接在一起，单独训练，协同微调，这些混合系统将继续向我们展示充满戏剧性的新能力。
### 二、[Joseph Misiti](https://www.quora.com/profile/Joseph-Misiti) 的回答分享，4400+ 顶
个人认为，以下是部分必需技能：
更新：我在 Github 创建了一个 repo，里面有上百的资源链接能够帮你起步。
https://github.com/josephmisiti/awesome-machine-learning
**1. Python/C++/R/Java**
假如你想在机器学习领域里找一份工作，你大概迟早要学会这些语言。Python 的 Numpy and Scipy 库 [2] 非常有用，因为这些库不仅有跟 MATLAB 相似的功能，而且将其整合进Web Service，或者在 Hadoop（见下）里使用的时候也十分简单。在加快代码运行速度的时候会用到C++。R [3] 在统计和画图的时候非常好用。Hadoop [4] 是用 Java写的，所以当你实现 mappers 和 reducers 的时候可能需要用到 Java（虽然你也可以通过 Hadoop streaming  [5] 使用脚本语言）。
**2. 概率和统计**
学习算法里的很大一部分都基于概率和统计理论，比如说朴素贝叶斯 [6]、高斯混合模型 [7]、隐Markov模型 [8]。 想要理解这些模型，就需要扎实的概率和统计功底。测度论 [9] 怎么拼命学都不过分。把统计作为模型的评价指标，比方说混淆矩阵、ROC 曲线、p值等等。
**3. 应用数学和算法**
对于 SVM [10] 这些判别模型，就需要算法理论的扎实基础了。即便你不需要从头开始实现SVM，学习算法理论也可以帮助你理解算法。需要学习的东西有凸优化 [11]，梯度下降 [12]，二次规划 [13]，拉格朗日 [14]，偏微分方程等等。要习惯看求和符号 [16]。
**4. 分布式计算**
如今大部分的机器学习工作都要求使用大型数据集（见《数据科学》）[17]。处理这些数据并不能凭借一台机器，而是需要把工作分派给整一个集群。像 Apache Hadoop [4] 这样的项目和亚马逊的 EC2 [18] 这样的云服务就能以合理的成本方便地处理这些数据。虽然 Hadoop 把很多硬核的分布式计算问题隐去了，对 map-reduce [22] 和分布式文件系统 [19] 等方面有透彻的理解还是十分必要的。Apache Mahout [20] 和 Apache Whirr [21] 也很值得一看。
**5.精通 Unix/Linux 工具**
除非你运气好，不然你总是需要修改你数据集的格式来载入到 R、Hadoop、HBase 等。Python这些脚本语言（使用 python 里的 re）也能用来完成这个事情，但是最好的方法大约还是掌握专门为此设计的 unix 工具：cat [24]、grep [25]、find [26]、awk [27]、sed [28]、sort [29]、cut [30]、tr [31]等。因为最有可能在基于 Linux 的机器上处理这些数据（我记得 Hadoop 并不能在 Windows 上运作），你一定可以用到这些工具。要学会热爱并尽可能地使用这些工具。这些工具让事情简单多了，比如这个例子 [1]。
**6.熟悉Hadoop的子项目**
HBase、Zookeeper [32]、Hive [33]、Mahout 等。这些项目可以储存或读取数据，而且他们可扩展。
**7. 了解高级信号处理的技术**
特征提取是机器学习最重要的部分之一。如果你的特性提取得不好，那不论你使用什么算法，性能都不会好。根据具体情况，你可能可以应用一些很酷炫的高级信号处理算法，比方说 wavelets [42]、shearlets [43]、curvelets [44]、contourlets [45]、bandlets [46]。了解并尝试应用时间-频率分析方法 [47]。如果你还没有了解傅里叶分析和卷积的话，这些东西也值得学习。后面提到的这俩是信号处理的基础知识。
最后，尽量多练习、多阅读。有空的时候可以读读像 Google Map-Reduce [34]、Google File System [35]、Google Big Table [36]、The Unreasonable Effectiveness of Data [37] 上的论文。网上也有很多关于机器学习的好书的免费资源，这些也应该多读。[38][39][40] 我找到了一个很好的课程，并且在转发到Github了 [41]。与其直接使用开源的扩展包，不如自己写一份并且对比结果。如果你能从头写一个支持向量机，你就会明白像支持向量、gamma、cost、hyperplanes 等的概念。载入数据并开始训练并不难，难的是理解这所有的概念。
祝好运！
[1] [http://radar.oreilly.com/2011/04…](http://radar.oreilly.com/2011/04/data-hand-tools.html)
[2] [http://numpy.scipy.org/](http://numpy.scipy.org/)
[3] [http://www.r-project.org/](http://www.r-project.org/)
[4] [http://hadoop.apache.org/](http://hadoop.apache.org/)
[5] [http://hadoop.apache.org/common/…](http://hadoop.apache.org/common/docs/r0.15.2/streaming.html)
[6] [http://en.wikipedia.org/wiki/Nai…](http://en.wikipedia.org/wiki/Naive_Bayes_classifier)
[7] [http://en.wikipedia.org/wiki/Mix…](http://en.wikipedia.org/wiki/Mixture_model)
[8] [http://en.wikipedia.org/wiki/Hid…](http://en.wikipedia.org/wiki/Hidden_Markov_model)
[9] [http://en.wikipedia.org/wiki/Mea…](http://en.wikipedia.org/wiki/Measure_(mathematics))
[10] [http://en.wikipedia.org/wiki/Sup…](http://en.wikipedia.org/wiki/Support_vector_machine)
[11] [http://en.wikipedia.org/wiki/Con…](http://en.wikipedia.org/wiki/Convex_optimization)
[12] [http://en.wikipedia.org/wiki/Gra…](http://en.wikipedia.org/wiki/Gradient_descent)
[13] [http://en.wikipedia.org/wiki/Qua…](http://en.wikipedia.org/wiki/Quadratic_programming)
[14] [http://en.wikipedia.org/wiki/Lag…](http://en.wikipedia.org/wiki/Lagrange_multiplier)
[15] [http://en.wikipedia.org/wiki/Par…](http://en.wikipedia.org/wiki/Partial_differential_equation)
[16] [http://en.wikipedia.org/wiki/Sum…](http://en.wikipedia.org/wiki/Summation)
[17] [http://radar.oreilly.com/2010/06…](http://radar.oreilly.com/2010/06/what-is-data-science.html)
[18] [http://aws.amazon.com/ec2/](http://aws.amazon.com/ec2/)
[19] [http://en.wikipedia.org/wiki/Goo…](http://en.wikipedia.org/wiki/Google_File_System)
[20] [http://mahout.apache.org/](http://mahout.apache.org/)
[21] [http://incubator.apache.org/whirr/](http://incubator.apache.org/whirr/)
[22] [http://en.wikipedia.org/wiki/Map…](http://en.wikipedia.org/wiki/MapReduce)
[23] [http://hbase.apache.org/](http://hbase.apache.org/)
[24] [http://en.wikipedia.org/wiki/Cat…](http://en.wikipedia.org/wiki/Cat_(Unix))
[25] [http://en.wikipedia.org/wiki/Grep](http://en.wikipedia.org/wiki/Grep)
[26] [http://en.wikipedia.org/wiki/Find](http://en.wikipedia.org/wiki/Find)
[27] [http://en.wikipedia.org/wiki/AWK](http://en.wikipedia.org/wiki/AWK)
[28] [http://en.wikipedia.org/wiki/Sed](http://en.wikipedia.org/wiki/Sed)
[29] [http://en.wikipedia.org/wiki/Sor…](http://en.wikipedia.org/wiki/Sort_(Unix))
[30] [http://en.wikipedia.org/wiki/Cut…](http://en.wikipedia.org/wiki/Cut_(Unix))
[31] [http://en.wikipedia.org/wiki/Tr_…](http://en.wikipedia.org/wiki/Tr_(Unix))
[32] [http://zookeeper.apache.org/](http://zookeeper.apache.org/)
[33] [http://hive.apache.org/](http://hive.apache.org/)
[34] [http://static.googleusercontent….](http://static.googleusercontent.com/external_content/untrusted_dlcp/labs.google.com/en/us/papers/mapreduce-osdi04.pdf)
[35][http://static.googleusercontent….](http://static.googleusercontent.com/external_content/untrusted_dlcp/labs.google.com/en/us/papers/gfs-sosp2003.pdf)
[36][http://static.googleusercontent….](http://static.googleusercontent.com/external_content/untrusted_dlcp/labs.google.com/en/us/papers/bigtable-osdi06.pdf)
[37][http://static.googleusercontent….](http://static.googleusercontent.com/external_content/untrusted_dlcp/research.google.com/en/us/pubs/archive/35179.pdf)
[38] [http://www.ics.uci.edu/~welling/…](http://www.ics.uci.edu/~welling/teaching/273ASpring10/IntroMLBook.pdf)
[39] [http://www.stanford.edu/~hastie/…](http://www.stanford.edu/~hastie/local.ftp/Springer/OLD//ESLII_print4.pdf)
[40] [http://infolab.stanford.edu/~ull…](http://infolab.stanford.edu/~ullman/mmds.html)
[41] [https://github.com/josephmisiti/…](https://github.com/josephmisiti/machine-learning-module)
[42] [http://en.wikipedia.org/wiki/Wav…](http://en.wikipedia.org/wiki/Wavelet)
[43] [http://www.shearlet.uni-osnabrue…](http://www.shearlet.uni-osnabrueck.de/papers/Smrus.pdf)
[44] [http://math.mit.edu/icg/papers/F…](http://math.mit.edu/icg/papers/FDCT.pdf)
[45] [http://www.ifp.illinois.edu/~min…](http://www.ifp.illinois.edu/~minhdo/publications/contourlet.pdf)
[46] [http://www.cmap.polytechnique.fr…](http://www.cmap.polytechnique.fr/~mallat/papiers/07-NumerAlgo-MallatPeyre-BandletsReview.pdf)
[47 ][http://en.wikipedia.org/wiki/Tim…](http://en.wikipedia.org/wiki/Time%E2%80%93frequency_analysis)
[48] [http://en.wikipedia.org/wiki/Fou…](http://en.wikipedia.org/wiki/Fourier_analysis)
[49 ][http://en.wikipedia.org/wiki/Con…](http://en.wikipedia.org/wiki/Convolution)
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
![](http://www.jobbole.com/wp-content/uploads/2016/04/b947d0acc8cb4e3b0649a27c606466363.jpg)
