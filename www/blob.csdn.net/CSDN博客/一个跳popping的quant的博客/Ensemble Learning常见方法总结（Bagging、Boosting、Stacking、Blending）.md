# Ensemble Learning常见方法总结（Bagging、Boosting、Stacking、Blending） - 一个跳popping的quant的博客 - CSDN博客





2018年08月14日 17:24:07[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：2334








看到过一个问题：**Random Forest、Adaboost、GBDT、XGBoost的区别是什么？**这个问题基本上覆盖了关于Ensemble Learning中常见的几种方法，以及Ensemble Learning中各个方法的使用的流行程度，最后再来回答这个问题。



南京大学周志华教授在09年发布的论文中[《Ensemble Learning》](https://cs.nju.edu.cn/zhouzh/zhouzh.files/publication/springerEBR09.pdf)对集成学习中常见的三种方法（Boosting、Bagging和Stacking）进行了介绍，除此之外还有一些其他的方法。下面我们从简单到复杂来介绍下面几种方法：
- Voting
- Averaging
- Boosting
- Bagging
- Stacking
- Blending



### Voting

Voting是一种很思想很简单的集成策略。主要应用与**分类**问题。

几种常见的Voting方法有：

1、绝对多数投票法（majority voting）

也就是若某个标记的得票超过了半数，则预测为该标记，否则拒绝预测。

2、相对多数投票法（plurality voting）

即预测为得票最多的标记，若同时有多个标记，那么从中随机选取一个。

3、加权投票法（weighted voting）

即为不同的标记赋予不同的权重，最后再按照相对多数投票法来预测。

### Averaging

Averagin即是一种取平均的思想，从整体的角度上来提高预测的鲁棒性，通常用于**回归**问题。

几种常见的Averaging方法：

1、简单平均法（simple averaging）

2、加权平均法（weighted averaging）

### Boosting

Boosting是一种将弱学习器转换为强学习器的算法，它的机制是：**先从初始训练集训练出一个基学习器，然后根据基学习器的表现对训练样本进行调整，使得先前基学习器做错的训练样本在后续的训练中得到更多的关注，然后基于调整后的样本分布来训练下一个基学习器。**Boosting的代表是Adam Boosting。

![](https://img-blog.csdn.net/2018081416205336?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### Bagging

Bagging是Bootstrap Aggregating的缩写。Bagging是为了得到泛化能力强的集成，因而就需要让各个子学习器之间尽可能独立，但是如果将样本分为了不同的不重合子集，那么每个基学习器学习的样本就会不足。所以它采用一种自助采样的方法（boostrap sampling）**每次从数据集中随机选择一个subset，然后放回初始数据集，下次取时，该样本仍然有一定概率取到。然后根据对每个subset训练出一个基学习器，然后将这些基学习器进行结合。**对于分类任务可以通过vote来输出结果，回归任务可以求平均值。Bagging的代表是Random Forest，RF是在决策树作为基学习器通过Bagging思想建立的。

![](https://img-blog.csdn.net/20180814162118694?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### Stacking

stacking是一种将弱学习器集成进行输出的策略，其中，在stacking中，所有的弱学习器被称作0级（**0 level**）学习器，他们的输出结果被一个1级（**1 level**）学习器接受，然后再输出最后的结果。这是实际上是一种分层的结构，前面提到的就是一种最基本的二级Stacking。另外，在bagging或者boosting中，所有的弱学习器一般都要求是相同的模型，如决策树，而stacking中可以是不同的模型，如KNN、SVM、LR、RF等。

![](https://img-blog.csdn.net/20180814162316604?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### Blending

blending和stacking类似，主要是对已学好的基学习器的结果的融合不同，Blending是**线性**融合，而Stacking是**非线性**融合。

### **Random Forest、Adaboost、GBDT、XGBoost的区别是什么？**

现在来回答这个问题。

Random Forest是一种基于Bagging思想的Ensemble learning方法，它实际上就是Bagging + 决策树。Random Forest可以用来做分类也可以做回归，做分类时最后多棵树的分类器通过voting来决定分类结果；做回归时，由多棵树预测值的averaging来决定预测结果。

Adaboost是Boosting算法中的代表，它的思想也便是基于Boosting思想的。在adaboost的运算过程中，一开始在训练样本时，为每个子样本赋予一个权重，一开始这些权重都是相等的，然后在训练数据集上训练出一个弱分类器，并计算这个弱分类器在每个子样本上的错误率，在第二次对这同一数据集进行训练时，将会根据分类器的错误率对子数据集中各个权重进行调整，分类正确的权重降低，分类错误的权重上升，这些权重的总和不变。最终得到的分类器会基于这些训练的弱分类器的分类错误率来分配不同的决定系数，从而使权重更新时，错误样本具有更高的权重。最后以此来更新各个样本的权重，直至达到迭代次数或者错误率为0。**所以Adaboost会对那些影响准确率的数据额外关注，从而会降低bias，而导致overfit。**

GBDT也即是Gradient Boosting Decisioin Tree，中文是梯度提升决策树，它的基学习器是分类回归树CART。

XGBoost的基学习器除了GBDT中用到的CART还支持线性分类器。传统GBDT在优化时只用到一阶导数信息，xgboost则对代价函数进行了二阶泰勒展开，同时用到了一阶和二阶导数。区别可以参考[机器学习算法中 GBDT 和 XGBOOST 的区别有哪些？]()总之XGBoost要比GBDT更快、效果更好。



借鉴：

[https://www.cnblogs.com/ljygoodgoodstudydaydayup/p/7132902.html](https://www.cnblogs.com/ljygoodgoodstudydaydayup/p/7132902.html)

[https://blog.csdn.net/edogawachia/article/details/79441138](https://blog.csdn.net/edogawachia/article/details/79441138)

[随机森林](https://blog.csdn.net/qq547276542/article/details/78304454)

[Ada Boost](https://www.cnblogs.com/zy230530/p/6909288.html)

[GBDT](https://blog.csdn.net/google19890102/article/details/51746402/)

[从GB、GBDT到XGBoost](https://www.cnblogs.com/wxquare/p/5541414.html)

[xgboost](https://www.cnblogs.com/zhouxiaohui888/p/6008368.html)



