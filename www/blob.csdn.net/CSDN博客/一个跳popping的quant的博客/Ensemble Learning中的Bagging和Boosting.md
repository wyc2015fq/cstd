# Ensemble Learning中的Bagging和Boosting - 一个跳popping的quant的博客 - CSDN博客





2018年05月29日 13:50:09[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1907标签：[Ensemble Learning																[Bagging																[Boosting](https://so.csdn.net/so/search/s.do?q=Boosting&t=blog)
个人分类：[ensemble learning](https://blog.csdn.net/FrankieHello/article/category/7698129)





Ensemble Learning大致可以分为两大类：

1、个体学习器之间存在强依赖关系、串行生成的序列化方法（**Sequential Ensemble**），代表是Boosting。

2、个体学习器之间不存在强依赖关系、可同时生成的并行方法（**Independent Ensemble**），代表是Bagging。



Boosting是一种将弱学习器转换为强学习器的算法，周志华的西瓜书描述它的机制：**先从初始训练集训练出一个基学习器，然后根据基学习器的表现对训练样本进行调整，使得先前基学习器做错的训练样本在后续受到更多的关注，然后基于调整后的样本分布来训练下一个基学习器。 **Boosting的代表是Adam Boosting。

Bagging是Bootstrap Aggregating的缩写。**它通过自助采样的方法（boostrap sampling），每次从数据集中随机选择一个subset，然后放回初始数据集，下次取时，该样本仍然有一定概率取到。然后根据对每个subset训练出一个基学习器，然后将这些基学习器进行结合。**对于分类任务可以通过vote来输出结果，回归任务可以求平均值。



从Bias-Variance Trade-off的角度来看，Boosting主要关注降低Bias，因而容易造成Overfit。而Bagging则侧重于降低Variance，所以容易陷入Underfit。原因是Boosting通过一次次的迭代，最小化loss function，所以降低Bias很好理解；由于受数据中的noise和anamoly的影响，Boosting会对受这些数据的影响，而Bagging则通过选取subset可以在一定程度上减少异常数据的影响，使得数据的分布更加均匀，从而降低Variance。

![](https://img-blog.csdn.net/20180529134654673)](https://so.csdn.net/so/search/s.do?q=Bagging&t=blog)](https://so.csdn.net/so/search/s.do?q=Ensemble Learning&t=blog)




