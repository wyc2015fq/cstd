# Xgboost算法推导及分析 - 快来学习鸭～～～ - CSDN博客





2017年10月21日 14:51:03[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：4307








**Author:** DivinerShi

Xgboost其实就是gbdt的一个改进版本，但是因为效果好，工程建设完善，所以经常和传统的gbdt分开讲。

Boost思想的话是每一步产生一个弱预测模型（如决策树），并加权累加到总模型中，经典的算法有如Adaboost。提升的理论意义：如果一个问题存在弱分类器，则可以通过提升的办法得到强分类器。这里，如果每一步的弱预测模型生成都是依赖损失函数的梯度方向，则称之为梯度提升（Gradient boosting）。

梯度提升算法首先给定一个目标损失函数，它的定义域是所有可行的弱函数集合（基函数）；提升算法通过迭代的选择一个负梯度方向上的基函数来逐渐逼近局部极小值。
## Boost算法

首先讲一下什么是boost

给定输入向量X和输出变量y组成的若干训练样本![这里写图片描述](https://img-blog.csdn.net/20171021122725657?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，目标是找到近似函数![这里写图片描述](https://img-blog.csdn.net/20171021122829860?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，使得损失函数![这里写图片描述](https://img-blog.csdn.net/20171021122912578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 的损失值最小。

最常见的损失就是平方误差损失
![这里写图片描述](https://img-blog.csdn.net/20171021123017577?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

也有一些其他的，如绝对误差损失函数等
![这里写图片描述](https://img-blog.csdn.net/20171021123129597?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

假设最优函数为![这里写图片描述](https://img-blog.csdn.net/20171021123248927?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，即：![这里写图片描述](https://img-blog.csdn.net/20171021123322201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

假定![这里写图片描述](https://img-blog.csdn.net/20171021123503065?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 是一族基函数![这里写图片描述](https://img-blog.csdn.net/20171021123538420?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 的加权和![这里写图片描述](https://img-blog.csdn.net/20171021123617155?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，那么这个函数就是我们要找的函数。
## 开始推导

从最常见的平方误差入手，可以定义为
![这里写图片描述](https://img-blog.csdn.net/20171021124517804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

而这里的函数f就是我们要求的函数，可以想一下一个问题，那就是对于决策树，其实决策树也可以理解为一个函数，树的输出就是输入数据的预测值。而求函数的过程其实就是求函数的参数的一个过程，所以我们上面的目标函数可以改成
![这里写图片描述](https://img-blog.csdn.net/20171021124801156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，这里的![这里写图片描述](https://img-blog.csdn.net/20171021124903360?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 就是该函数的参数。

那么把这里的函数改成树也是一样的。

需要注意一点，其实损失函数不同，所假设的误差的所服从的分布也是不同的，平方误差的话，应该假设误差服从高斯分布，且最优值应该是均值；如果是绝对误差的话，应该假设误差服从拉普拉斯分布，且最优值应该是中位值。

前面我们说了，xgboost是要沿着梯度的方向去最小化整个模型的误差，那么目标函数可以定义为如下
![这里写图片描述](https://img-blog.csdn.net/20171021125301952?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中i指的是第i个样本，![这里写图片描述](https://img-blog.csdn.net/20171021130343363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 是第t-1个模型对样本i的预测值，![这里写图片描述](https://img-blog.csdn.net/20171021125459243?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 表示的是新加入的第t个模型，![这里写图片描述](https://img-blog.csdn.net/20171021125545934?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 表示正则项， C是计算过程中析出来的一些常数项。最外面的函数L()表示的就是整个模型的误差。那么这个函数的意思就是加上新的模型的预测值后来降低整个模型的误差。而我们的目标就是想办法去找出这个能使得误差最小的函数![这里写图片描述](https://img-blog.csdn.net/20171021125843143?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 。

首先我们知道二阶泰勒展开如下所示：
![这里写图片描述](https://img-blog.csdn.net/20171021130014050?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

，那么我们可以把之前的目标函数和该二阶泰勒展开函数对应起来，即去对目标函数做二阶泰勒展开
![这里写图片描述](https://img-blog.csdn.net/20171021131329455?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如上图所示，我们可以把loss函数当做要展开的函数f，然后把前t-1个模型预测的结果作为x，把新加入的基函数当做增量，并进行泰勒展开。那么我们对该泰勒函数一项一项的展开，首先
![这里写图片描述](https://img-blog.csdn.net/20171021130014050?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 中的f(x)就可以直接表示为![这里写图片描述](https://img-blog.csdn.net/20171021131815524?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因为后面分别要计算一阶和二阶导数，所以我们定义样本i的一阶和二阶导数为
![这里写图片描述](https://img-blog.csdn.net/20171021132123852?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 和 ![这里写图片描述](https://img-blog.csdn.net/20171021132145425?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，![这里写图片描述](https://img-blog.csdn.net/20171021132244046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 的话其实就是新的基函数![这里写图片描述](https://img-blog.csdn.net/20171021132358230?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 那么我们就可以得到展开后的目标函数：
![这里写图片描述](https://img-blog.csdn.net/20171021132501507?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

而且这里的函数g()和函数h()都是关于之前所有树的函数，都是可计算的**数值**。

我们知道给定一颗树，正真起到分类作用的其实是叶子节点，输入一个样本，叶子的输出值就是预测的结果，即一颗决策树，预测的结果是由决策树的叶子节点所决定的。对于分类问题，决策树的叶子就是指类别，对于回归问题，叶子的值就是数值。

这里，我们先做一个假设，假设样本x的输出落在第q个叶子上，那么样本x的输出值为![这里写图片描述](https://img-blog.csdn.net/20171021135801885?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，那么我们就可以重新定义函数f为：![这里写图片描述](https://img-blog.csdn.net/20171021135900286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)。

正则项：决策树的正则一般考虑的是叶子节点数和叶子权值，比如常见的是使用叶子节点总数和叶子权值平方和的加权作为正则项：
![这里写图片描述](https://img-blog.csdn.net/20171021140055055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)。

现在我们重新来看我们之前的目标函数：
![这里写图片描述](https://img-blog.csdn.net/20171021140143080?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

前面我们说了![这里写图片描述](https://img-blog.csdn.net/20171021140352107?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 计算出来的是一个常数，所以我们可以直接把这项放到常数项C中，那么我们就可以得到
![这里写图片描述](https://img-blog.csdn.net/20171021140654196?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

接着，继续去替换函数![这里写图片描述](https://img-blog.csdn.net/20171021135900286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，以及写入具体的正则项，可以得到如下
![这里写图片描述](https://img-blog.csdn.net/20171021141137965?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

进一步，n个样本落在T个不同的叶子上，那么可以等价的按不同的叶子将样本进行区分，即按T,将落在同一个叶子上的样本进行求和，有可继续得到
![这里写图片描述](https://img-blog.csdn.net/20171021141440940?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

再把正则项中相似的项合并一下就可以得到
![这里写图片描述](https://img-blog.csdn.net/20171021141542734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

到这，基本上我们要求的目标函数和基函数f就没什么关系了，完全就是一个关于样本一阶导数和二阶导数的函数了。

当然我们还可以再简化下这个目标函数，我们可以把位于同一号叶子上一届偏导的和记为G,位于同一号叶子上的二阶偏导的和记为H,如：
![这里写图片描述](https://img-blog.csdn.net/20171021142209127?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从而优化函数变成：
![这里写图片描述](https://img-blog.csdn.net/20171021142255765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

到这里为止，目标函数本质已经变成w的函数，变成了一个求w的游戏，求函数极值，那就求导呗，所以这里对w求偏导，令其为0：
![这里写图片描述](https://img-blog.csdn.net/20171021142523771?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后再带回原目标函数：
![这里写图片描述](https://img-blog.csdn.net/20171021142603774?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这就是我们最终得到的目标损失函数，所以xgboost中的gini指数不是正常的gini指数了，而是这个改进的损失函数了。在进行对决策树划分的时候需使用这个函数作为划分的依据。
## 改进

上面的是推导，在具体的工程实现中，xgboost还有很多优化：

1.比如xgboost不止支持CART，还支持线性分类器，可以当做带L1或者L2的逻辑回归（分类用）或者线性回归（回归用）。

2.还做了Shrinkage（缩减）![这里写图片描述](https://img-blog.csdn.net/20171021143438484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

类似于学习率，xgboost进行一次迭代后，会将叶子节点的权值乘上系数，目的是为了减少每棵树对整体模型的影响，让后面又更大的学习空间，可以简单理解为，模型在学习的过程中并不完全相信每棵树，只是从每棵树中学习一个大概的方向。
3.列采样。xgboost还做了和RF一样的列采样，用于减低过拟合，还能减少计算量。

4.xgboost还能自动分裂出缺失值的分裂方向。

5.xgboost在特征粒度上实现了并行，因为决策树在寻找最优切分点的时候是需要对特征进行排序的，这个步骤是相当费时的，所以xgboost对数据进行了预排序，然后保存为block结构，并在后面的迭代中重复使用这个结构，大大减少了计算量。因为有了block所以对特征进行计算增益的时候就可以多线程并行了。

6.还有一个就是并行化的近似直方图算法。我们在计算每个特征的每个切分点对应的增益的时候是用贪心的方法去枚举每个可能的分割点，但是如果数据无法一次载入内存或者在分布式的情况下，贪心算法的效率很低，所以xgboost提出了一种近似直方图算法，用于高效的生成候选的分割点。

参考

1.《机器学习》-	邹博

2. [https://www.zhihu.com/question/41354392/answer/98658997](https://www.zhihu.com/question/41354392/answer/98658997)







