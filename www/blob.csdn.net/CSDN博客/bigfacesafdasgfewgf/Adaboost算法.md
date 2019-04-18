# Adaboost算法 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 17:24:06[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1115
个人分类：[Machine Learning&&Data Mining](https://blog.csdn.net/puqutogether/article/category/2254295)









基于多分类器Adaboost的问题相似度匹配算法

**Introduction**
Adaboost是一种分类器，核心思想就是针对同一个训练集，训练不同的弱分类器，然后把这些弱分类器集合起来，构成一个强分类器。
Adaboost算法本身是通过改变数据分布来实现的，它根据每次训练集之中，每个样本的分类是否正确，以及上次的总体分类的准确率，来确定每个样本的权重。将修改过的权重的新数据集送给下层分类器进行训练，最后将每次训练得到的分类器最后融合起来，作为最后的决策分类器。
使用Adaboost分类器可以排除一些不必要的训练数据特征，并放在关键的训练数据上面。


Algorithm

经典的Adaboost算法的过程如下：
Step 1：先通过对N个训练样本的学习得到第一个弱分类器；、
Step 2：将分错的样本和其他的新数据一起构成一个新的N个的训练样本，通过对这个样本的学习得到第二个弱分类器；
Step 3：将1和2都分错了的样本加上其他的新样本构成另一个新的N个训练样本，通过对这个新样本集的学习得到第三个分类器；
Step 4：最终经过提升的强分类器，测试数据被分为哪一类要通过诸多弱分类器的Majority vote Strategy；


对于上面的算法，还有两个问题：如何调整训练集，使得在训练集上训练的弱分类器得以进行；如何将训练得到的各个弱分类器联合起来形成强分类器。所以，Adaboost算法有如下的改进：

- 使用加权后选取的训练数据代替随机选取的训练样本，这样将训练的重点集中在比较难分的训练数据样本上；
- 将弱分类器联合起来，使用加权的投票机制代替平均投票机制。让分类效果好的弱分类器具有较大的权重，而分类效果差的分类器具有较小的权重。



然后，新改进的Adaboost算法如下：
Adaboost算法中不同的训练集是通过调整每个样本对应的权重来实现的。开始时，每个样本对应的权重是相同的，weight=1/n，其中n为样本个数。在此样本分布下训练出一个弱分类器。对于分类错误的样本，加大其对应的权重；而对应分类正确的样本，降低其权重，这样分错的样本就被凸显出来，从而得到一个新的样本分布。在新的样本分布下，在此对样本进行训练，得到弱分类器。依此类推，经过T次循环，得到T个弱分类器，把它们按照一定的权重叠加起来（boost），得到最终想要的强分类器。

上面提到过两个权重，一个是分类器的权重，一个是样本的权重。

1. 分类器的权重： 

AdaBoost为每个分类器都分配了一个权重值，alpha,这些alpha值是基于每个弱分类器的错误率来计算的。如下：

![](https://img-blog.csdn.net/20141008164931359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


2. 样本的权重：

每个样本的权重构成了一个权重向量D，在每得到一个弱分类器之后，我们都需要根据分类结果，对权重向量进行更新。使得那些正确分类的样本的权重降低，而被错误分类的样本的权重升高。权重向量D的计算方法如下：

![](https://img-blog.csdn.net/20141008165119468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


在计算出D之后，AdaBoost又可以进入下一轮的迭代。该算法重复着训练和调整权重的过程，直到训练错误率为0，或者弱分类器的数目达到用户的指定值为止。




机器学习实战中对Adaboost算法绘制的一个示意图，我觉得很不错，可以展示出该算法的精髓：

![](https://img-blog.csdn.net/20141008165355265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)









还有一些详细的理论推导：
[http://blog.csdn.net/dark_scope/article/details/14103983](http://blog.csdn.net/dark_scope/article/details/14103983)













