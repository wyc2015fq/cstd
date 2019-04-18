# 离线轻量级大数据平台Spark之MLib机器学习库概念学习 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年10月28日 17:47:33[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5678
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









## Mlib机器学习库

### 1.1机器学习概念

机器学习有很多定义，倾向于下面这个定义。机器学习是对能通过经验自动改进的计算机算法的研究。机器学习依赖数据经验并评估和优化算法所运行出的模型。机器学习算法尝试根据训练数据使得表示算法行为的数学目标最大化，并以此来进行预测或作出决定。机器学习问题分类为几种，包括分类、回归、聚类。所有的机器学习算法都经过一条流水线：提取训练数据的特征->基于特征向量训练模型->评估模型选择最佳。特征提取主要是提取训练数据中的数值特征，用于数学建模。机器学习一般有如下分类：

1）监督学习

监督是从给定的训练数据集中学习一个函数（模型），输入新的数据时，可以根据这个函数预测结果。训练集中的目标是人工标注的。监督学习就是人参与到模型评估中，常见算法包括回归分析和统计分类。监督学习场用于分类，目的是让计算机学习人工标注的分类。监督学习是训练神经网络和决策树的常见技术。神经网络和决策树技术高度依赖于实现确定的分类信息。

2）无监督学习

无监督学习就是在训练集构建模型过程中没有人的参与，全部有计算机自动标识。通俗的理解是，人也不知道怎么标识，就让计算机按照某种算法去标识。常见应用场景包括关联规则的学习和聚类，常见算法包括Apriori算法和K-Means算法。

3）半监督学习

半监督学习介于监督学习与无监督学习之间的一种机器学习方式，是模式识别和机器学习领域研究的重点问题。面向少量的标注样本和大量的未标注样本进行训练和分类。主要算法包括：基于概率的算法、在现有监督算法基础上进行修改的方法、直接依赖于聚类假设的方法等。半监督学习就是人部分参与标识，常见算法包括分类和回归，包括对常用监督式学习算法的延伸，算法试图对未标识数据进行建模，在此基础上再对标识的数据进行预测，如图论推理算法或者拉普拉斯支持向量机。

4）强化学习

强化学习通过观察来学习动作的完成，每个动作都会对环境有所影响，学习对象根据观察到的周围环境的反馈来做出判断。输入数据作为对模型的反馈，模型对此加以调整，常见场景包括动态系统以及机器人控制等，常见算法包括Q-Learning及时间差学习。

常见的机器学习算法有：

l   构造条件概率：回归分析和统计分类；

l   人工神经网络；

l   决策树；

l   高斯过程回归；

l   线性判别分析；

l   最近邻居法；

l   感知器；

l   径向基函数核；

l   支持向量机；

l   通过再生模型构造概率密度函数；

l   最大期望算法；

l   graphicalmodel ：包括贝叶斯网和 Markov 随机场；

l   GenerativeTopographic Mapping ；

l   近似推断技术；

l   马尔可夫链蒙特卡罗方法；

l   变分法；

l   最优化：大多数以上方法，直接或者间接使用最优化算法。

主要的机器学习算法介绍：

1）回归算法

回归算法是试图采用对误差的衡量来探索变量之间的关系的一类算法。回归算法是统计机器学习的利器。常见的回归算法包括：最小二乘法（ Ordinary Least Square ），逻辑回归（ LogisticRegression ），逐步式回归（ Stepwise Regression ），多元自适应回归样条（ Multivariate Adaptive Regression Splines ）以及本地散点平滑估计（ Locally Estimated Scatterplot Smoothing ）。

2）基于实例的算法

基于实例的算法常常用来对决策问题建立模型，这样的模型常常先选取一批样本数据，然后根据某些近似性把新数据与样本数据进行比较。通过这种方式来寻找最佳的匹配。因此，基于实例的算法常常也被称为“赢家通吃”学习或者“基于记忆的学习”。常见的算法包括 k-Nearest Neighbor (KNN) ，、学习矢量量化（ LearningVector Quantization ， LVQ ）以及自组织映射算法（ Self-Organizing Map ， SOM ）

3）正则化方法

正则化方法是其他算法（通常是回归算法）的延伸，根据算法的复杂度对算法进行调整。正则化方法通常对简单模型予以奖励而对复杂算法予以惩罚。常见的算法包括： Ridge Regression 、 Least Absolute Shrinkageand Selection Operator （ LASSO ）以及弹性网络（ Elastic Net ）。

4）决策树算法

决策树算法根据数据的属性采用树状结构建立决策模型，决策树模型常常用来解决分类和回归问题。常见的算法包括：分类及回归树（ Classification And Regression Tree ， CART ）、 ID3 (Iterative Dichotomiser 3) 、 C4.5 、 Chi-squared Automatic Interaction Detection (CHAID) 、 Decision Stump 、机森林（ Random Forest ）、多元自适应回归样条（ MARS ）以及梯度推进机（
 Gradient Boosting Machine ， GBM ）。

5）贝叶斯算法

贝叶斯算法是基于贝叶斯定理的一类算法，主要用来解决分类和回归问题。常见算法包括：朴素贝叶斯算法、平均单依赖估计（ Averaged One-Dependence Estimators ， AODE ）以及 Bayesian Belief Network （ BBN ）。

6）基于核的算法

基于核的算法中最著名的莫过于支持向量机（ SVM ）了。基于核的算法把输入数据映射到一个高阶的向量空间， 在这些高阶向量空间里， 有些分类或者回归问题能够更容易解决。常见的基于核的算法包括：支持向量机（ Support Vector Machine ， SVM ）、径向基函数（ Radial Basis Function ， RBF) 以及线性判别分析（ Linear Discriminate Analysis ， LDA) 等。

7）聚类算法

聚类就像回归一样，有时候人们描述的是一类问题，有时候描述的是一类算法。聚类算法通常按照中心点或者分层的方式对输入数据进行归并。所有的聚类算法都试图找到数据的内在结构，以便按照最大的共同点将数据进行归类。常见的聚类算法包括 k-Means 算法以及期望最大化算法（ ExpectationMaximization ， EM ）。

8）关联规则学习

关联规则学习通过寻找最能够解释数据变量之间关系的规则，来找出大量多元数据集中有用的关联规则。常见算法包括 Apriori 算法和 Eclat 算法等。

9）人工神经网络算法

人工神经网络算法模拟生物神经网络，是一类模式匹配算法。通常用于解决分类和回归问题。人工神经网络是机器学习的一个庞大的分支，有几百种不同的算法（其中深度学习就是其中的一类算法，我们会单独讨论）。重要的人工神经网络算法包括：感知器神经网络（ Perceptron Neural Network ）、反向传递（ BackPropagation ）、 Hopfield 网络、自组织映射（ Self-Organizing Map, SOM ）、学习矢量量化（ LearningVector Quantization
 ， LVQ ）。

10）深度学习算法

深度学习算法是对人工神经网络的发展，在近期赢得了很多关注，特别是百度也开始发力深度学习后，更是在国内引起了很多关注。在计算能力变得日益廉价的今天，深度学习试图建立大得多也复杂得多的神经网络。很多深度学习的算法是半监督式学习算法，用来处理存在少量未标识数据的大数据集。常见的深度学习算法包括：受限波尔兹曼机（ Restricted Boltzmann Machine ， RBN ）、 Deep Belief Networks （ DBN ）、卷积网络（ Convolutional Network ）、堆栈式自动编码器（
 StackedAuto-encoders ）。

11）降低维度算法

像聚类算法一样，降低维度算法试图分析数据的内在结构，不过降低维度算法是以非监督学习的方式，试图利用较少的信息来归纳或者解释数据。这类算法可以用于高维数据的可视化或者用来简化数据以便监督式学习使用。常见的算法包括：主成份分析（ Principle Component Analysis ， PCA ）、偏最小二乘回归（ Partial Least Square Regression ， PLS ）、 Sammon 映射、多维尺度（ Multi-Dimensional Scaling,MDS ）、投影追踪（
 Projection Pursuit ）等。

12）集成算法

集成算法用一些相对较弱的学习模型独立地对同样的样本进行训练，然后把结果整合起来进行整体预测。集成算法的主要难点在于究竟集成哪些独立的较弱的学习模型以及如何把学习结果整合起来。这是一类非常强大的算法，同时也非常流行。常见的算法包括： Boosting 、 Bootstrapped Aggregation （ Bagging ）、 AdaBoost 、堆叠泛化（ Stacked Generalization ， Blending ）、梯度推进机（ Gradient Boosting Machine, GBM
 ）、随机森林（Random Forest ）。

### 1.2Spark MLib介绍

Spark 之所以在机器学习方面具有得天独厚的优势，有以下原因：

1）机器学习算法一般都有很多个步骤迭代计算的过程，机器学习的计算需要在多次迭代后获得足够小的误差或者足够收敛才会停止，迭代时如果使用 Hadoop 的 MapReduce 计算框架，每次计算都要读 / 写磁盘以及任务的启动等工作，这回导致非常大的 I/O 和 CPU 消耗。而 Spark 基于内存的计算模型天生就擅长迭代计算，多个步骤计算直接在内存中完成，只有在必要时才会操作磁盘和网络，所以说 Spark 正是机器学习的理想的平台。

2）从通信的角度讲，如果使用 Hadoop 的 MapReduce 计算框架， JobTracker 和 TaskTracker 之间由于是通过 heartbeat 的方式来进行的通信和传递数据，会导致非常慢的执行速度，而 Spark具有出色而高效的 Akka 和 Netty 通信系统，通信效率极高。

MLlib(MachineLearnig lib) 是 Spark 对常用的机器学习算法的实现库，同时包括相关的测试和数据生成器。 Spark的设计初衷就是为了支持一些迭代的 Job, 这正好符合很多机器学习算法的特点。

MLlib 目前支持 4种常见的机器学习问题 : 分类、回归、聚类和协同过滤，MLlib 在 Spark 整个生态系统中的位置如下图所示。

![](https://img-blog.csdn.net/20161028174633150?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




MLlib 基于 RDD，可以与 Spark SQL 、 GraphX 、 Spark Streaming 无缝集成，以 RDD 为基石， 4 个子框架可联手构建大数据计算中心。

MLlib 是MLBase 一部分，其中 MLBase 分为四部分：MLlib 、 MLI 、 ML Optimizer 和 MLRuntime 。

l   ML Optimizer 会选择它认为最适合的已经在内部实现好了的机器学习算法和相关参数，来处理用户输入的数据，并返回模型或别的帮助分析的结果；

l   MLI 是一个进行特征抽取和高级 ML 编程抽象的算法实现的 API 或平台；

l   MLlib 是 Spark 实现一些常见的机器学习算法和实用程序，包括分类、回归、聚类、协同过滤、降维以及底层优化，该算法可以进行可扩充；

l   MLRuntime 基于 Spark 计算框架，将 Spark 的分布式计算应用到机器学习领域。

Mlib中主要包括：1）特征提取：TF-IDF；2）统计；3）分类和回归：属于监督式机器学习，分类预测的是离散变量，而回归预测的是连续变量，有线性回归、逻辑回归、支持向量机、朴素贝叶斯、决策树和随机森林；4）聚类：无监督式机器学习，将对象分到具有高度相似性的聚类中，监督式任务中的数据带标签，而聚类可用于无标签数据，主要用于数据探索以及异常检测，有Kmeans；5）协同过滤和推荐：协同过滤是一种根据用户对各种产品的交互与评分来推荐新产品的推荐系统技术，有交替最小二乘；6）降维：有主成分分析、奇异值分解。

参阅：https://spark.apache.org/docs/latest/mllib-guide.html

### 1.3Spark MLib架构解析

从架构图可以看出 MLlib 主要包含三个部分：

l   底层基础 ：包括 Spark 的运行库、矩阵库和向量库；

l   算法库 ：包含广义线性模型、推荐系统、聚类、决策树和评估的算法；

l   实用程序 ：包括测试数据的生成、外部数据的读入等功能。

![](https://img-blog.csdn.net/20161028174700963?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




1）MLlib的底层基础解析

底层基础部分主要包括向量接口和矩阵接口，这两种接口都会使用 Scala 语言基于 Netlib 和BLAS/LAPACK 开发的线性代数库 Breeze 。

MLlib 支持本地的密集向量和稀疏向量，并且支持标量向量。

MLlib 同时支持本地矩阵和分布式矩阵，支持的分布式矩阵分为 RowMatrix 、 IndexedRowMatrix 、 CoordinateMatrix 等。

2）MLlib的算法库分析

下图是MLlib 算法库的核心内容。

![](https://img-blog.csdn.net/20161028174719495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




Spark 中常用的算法：

Ø  分类算法

分类算法属于监督式学习，使用类标签已知的样本建立一个分类函数或分类模型，应用分类模型，能把数据库中的类标签未知的数据进行归类。分类在数据挖掘中是一项重要的任务，目前在商业上应用最多，常见的典型应用场景有流失预测、精确营销、客户获取、个性偏好等。 MLlib 目前支持分类算法有：逻辑回归、支持向量机、朴素贝叶斯和决策树。

Ø  回归算法

回归算法属于监督式学习，每个个体都有一个与之相关联的实数标签，并且我们希望在给出用于表示这些实体的数值特征后，所预测出的标签值可以尽可能接近实际值。MLlib 目前支持回归算法有：线性回归、岭回归、Lasso和决策树。

案例：导入训练数据集，将其解析为带标签点的RDD，使用 LinearRegressionWithSGD 算法建立一个简单的线性模型来预测标签的值，最后计算均方差来评估预测值与实际值的吻合度。

Ø  聚类算法

聚类算法属于非监督式学习，通常被用于探索性的分析，是根据“物以类聚”的原理，将本身没有类别的样本聚集成不同的组，这样的一组数据对象的集合叫做簇，并且对每一个这样的簇进行描述的过程。它的目的是使得属于同一簇的样本之间应该彼此相似，而不同簇的样本应该足够不相似，常见的典型应用场景有客户细分、客户研究、市场细分、价值评估。MLlib 目前支持广泛使用的KMmeans聚类算法。

案例：导入训练数据集，使用 KMeans 对象来将数据聚类到两个类簇当中，所需的类簇个数会被传递到算法中，然后计算集内均方差总和 (WSSSE)，可以通过增加类簇的个数 k 来减小误差。 实际上，最优的类簇数通常是 1，因为这一点通常是WSSSE图中的 “低谷点”。

Ø  协同过滤

协同过滤常被应用于推荐系统，这些技术旨在补充用户-商品关联矩阵中所缺失的部分。MLlib当前支持基于模型的协同过滤，其中用户和商品通过一小组隐语义因子进行表达，并且这些因子也用于预测缺失的元素。

案例：导入训练数据集，数据每一行由一个用户、一个商品和相应的评分组成。假设评分是显性的，使用默认的ALS.train()方法，通过计算预测出的评分的均方差来评估这个推荐模型。

3）MLlib的实用程序分析

实用程序部分包括数据的验证器、Label的二元和多元的分析器、多种数据生成器、数据加载器。



