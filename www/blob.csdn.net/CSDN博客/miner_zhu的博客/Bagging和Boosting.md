# Bagging和Boosting - miner_zhu的博客 - CSDN博客





2018年07月23日 19:09:12[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：69








Baggging 和Boosting都是模型融合的方法，可以将弱分类器融合之后形成一个强分类器，而且融合之后的效果会比最好的弱分类器更好。



**Bagging:**

先介绍Bagging方法：

Bagging即套袋法，其算法过程如下：
- 
从原始样本集中抽取训练集。每轮从原始样本集中使用Bootstraping的方法抽取n个训练样本（在训练集中，有些样本可能被多次抽取到，而有些样本可能一次都没有被抽中）。共进行k轮抽取，得到k个训练集。（k个训练集之间是相互独立的）

- 
每次使用一个训练集得到一个模型，k个训练集共得到k个模型。（注：这里并没有具体的分类算法或回归方法，我们可以根据具体问题采用不同的分类或回归方法，如决策树、感知器等）

- 
对分类问题：将上步得到的k个模型采用投票的方式得到分类结果；对回归问题，计算上述模型的均值作为最后的结果。（所有模型的重要性相同）


![](https://img-blog.csdn.net/20170624153051402?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2hlbnl1a3VhaTY2MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Boosting：**

      AdaBoosting方式每次使用的是全部的样本，每轮训练改变样本的权重。下一轮训练的目标是找到一个函数f 来拟合上一轮的残差。当残差足够小或者达到设置的最大迭代次数则停止。Boosting会减小在上一轮训练正确的样本的权重，增大错误样本的权重。（对的残差小，错的残差大）

      梯度提升的Boosting方式是使用代价函数对上一轮训练出的模型函数f的偏导来拟合残差。

![](https://img-blog.csdn.net/20170624154224694?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2hlbnl1a3VhaTY2MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# Bagging，Boosting二者之间的区别

Bagging和Boosting的区别：

1）样本选择上：

Bagging：训练集是在原始集中有放回选取的，从原始集中选出的各轮训练集之间是独立的。

Boosting：每一轮的训练集不变，只是训练集中每个样例在分类器中的权重发生变化。而权值是根据上一轮的分类结果进行调整。

2）样例权重：

Bagging：使用均匀取样，每个样例的权重相等

Boosting：根据错误率不断调整样例的权值，错误率越大则权重越大。

3）预测函数：

Bagging：所有预测函数的权重相等。

Boosting：每个弱分类器都有相应的权重，对于分类误差小的分类器会有更大的权重。

4）并行计算：

Bagging：各个预测函数可以并行生成

Boosting：各个预测函数只能顺序生成，因为后一个模型参数需要前一轮模型的结果。



# 总结：

这两种方法都是把若干个分类器整合为一个分类器的方法，只是整合的方式不一样，最终得到不一样的效果，将不同的分类算法套入到此类算法框架中一定程度上会提高了原单一分类器的分类效果，但是也增大了计算量。

下面是将决策树与这些算法框架进行结合所得到的新的算法：

1）Bagging + 决策树 = 随机森林

2）AdaBoost + 决策树 = 提升树

3）Gradient Boosting + 决策树 = GBDT



# 为什么说bagging是减少variance，而boosting是减少bias？

Bagging对样本重采样，对每一重采样得到的子样本集训练一个模型，最后取平均。由于子样本集的相似性以及使用的是同种模型，因此各模型有近似相等的bias和variance（事实上，各模型的分布也近似相同，但不独立）。由于![E[\frac{\sum X_i}{n}]=E[X_i]](https://www.zhihu.com/equation?tex=E%5B%5Cfrac%7B%5Csum+X_i%7D%7Bn%7D%5D%3DE%5BX_i%5D)，所以bagging后的bias和单个子模型的接近，一般来说不能显著降低bias。另一方面，若各子模型独立，则有![Var(\frac{\sum X_i}{n})=\frac{Var(X_i)}{n}](https://www.zhihu.com/equation?tex=Var%28%5Cfrac%7B%5Csum+X_i%7D%7Bn%7D%29%3D%5Cfrac%7BVar%28X_i%29%7D%7Bn%7D)，此时可以显著降低variance。若各子模型完全相同，则![Var(\frac{\sum X_i}{n})=Var(X_i)](https://www.zhihu.com/equation?tex=Var%28%5Cfrac%7B%5Csum+X_i%7D%7Bn%7D%29%3DVar%28X_i%29)

，此时不会降低variance。bagging方法得到的各子模型是有一定相关性的，属于上面两个极端状况的中间态，因此可以一定程度降低variance。为了进一步降低variance，Random forest通过随机选取变量子集做拟合的方式de-correlated了各子模型（树），使得variance进一步降低。

（用公式可以一目了然：设有i.d.的n个随机变量，方差记为![\sigma^2](https://www.zhihu.com/equation?tex=%5Csigma%5E2)，两两变量之间的相关性为![\rho](https://www.zhihu.com/equation?tex=%5Crho)，则![\frac{\sum X_i}{n}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Csum+X_i%7D%7Bn%7D)的方差为![\rho*\sigma^2+(1-\rho)*\sigma^2/n](https://www.zhihu.com/equation?tex=%5Crho%2A%5Csigma%5E2%2B%281-%5Crho%29%2A%5Csigma%5E2%2Fn)

，bagging降低的是第二项，random forest是同时降低两项。详见ESL p588公式15.1）

boosting从优化角度来看，是用forward-stagewise这种贪心法去最小化损失函数![L(y, \sum_i a_i f_i(x))](https://www.zhihu.com/equation?tex=L%28y%2C+%5Csum_i+a_i+f_i%28x%29%29)。例如，常见的AdaBoost即等价于用这种方法最小化exponential loss：![L(y,f(x))=exp(-yf(x))](https://www.zhihu.com/equation?tex=L%28y%2Cf%28x%29%29%3Dexp%28-yf%28x%29%29)。所谓forward-stagewise，就是在迭代的第n步，求解新的子模型f(x)及步长a（或者叫组合系数），来最小化![L(y,f_{n-1}(x)+af(x))](https://www.zhihu.com/equation?tex=L%28y%2Cf_%7Bn-1%7D%28x%29%2Baf%28x%29%29)，这里![f_{n-1}(x)](https://www.zhihu.com/equation?tex=f_%7Bn-1%7D%28x%29)

是前n-1步得到的子模型的和。因此boosting是在sequential地最小化损失函数，其bias自然逐步下降。但由于是采取这种sequential、adaptive的策略，各子模型之间是强相关的，于是子模型之和并不能显著降低variance。所以说boosting主要还是靠降低bias来提升预测精度。



原文链接：[https://www.cnblogs.com/earendil/p/8872001.html](https://www.cnblogs.com/earendil/p/8872001.html)



