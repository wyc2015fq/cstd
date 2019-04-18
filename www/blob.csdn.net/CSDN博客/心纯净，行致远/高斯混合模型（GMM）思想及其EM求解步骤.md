# 高斯混合模型（GMM）思想及其EM求解步骤 - 心纯净，行致远 - CSDN博客





2017年10月13日 20:32:41[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：536








高斯混合模型是典型的聚类算法之一，最近一段时间一直在研究GMM。相比于GMM，K-means显得要容易理解一些，虽然GMM中最难的部分，也就是EM算法对建立的GMM进行求解，实际上就用到了K-means的算法。有人说，K-means求解是EM算法的精髓。确实如此！下面将今天思考的GMM整理如下，K-means部分有时间继续更新，今晚看了看K-means，还是很值得研究的。虽然算法理解起来简单，不过要是没有数个结合例子的实践，也谈不上对算法的深刻理解。

GMM是用来对大量数据进行聚类的算法。我们知道自然界中海量同类型数据的分布遵循高斯分布，即满足

N(x│u,σ^2
)=1/(σ*√2π) e^((-〖(x-u)〗^2)/(2σ^2 ))

带入特定的x，就可以求得在该分布下对应的概率。

好了，下面展示要解决的问题：有一大堆没有聚类的数据需要进行聚类处理（注：聚类的目的是为了搞清“类”的特征，便于对数据和模型进行辨识，以便于后续对数据进行分类，这要牵扯到分类算法），由于不知道这些数据满足何种分布，暂时我们用高斯分布来处理这些数据。当然，如果数据可以用一个Guess Model来描述，那最好不过！但实际上，遇到的问题往往是用一个GM不能描述的，所以很自然的想法是使用多个GM来进行描述，即GMM。

下面就开始对GMM的原理和使用到的EM进行描述。

我们知道

N(x│u,σ^2 )=1/(σ*√2π) e^((-〖(x-u)〗^2)/(2σ^2 ))


其中u代表总体均值，σ^2
代表总体方差。当用多个GM来描述数据时，显然对不同的GM要分配给不同的权值，也就是不用的GM在该点数据值上占据不同的比例。从下图容易理解这一想法：

![](https://img-blog.csdn.net/20171013205923807?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


最终的分布可能是多个GM根据不同权重叠加的结果。


![](https://img-blog.csdn.net/20171013205643648?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


好，我们假设有N个数据，利用K个GM来进行描述，每个GM在同一个数据点Xi上所占的比例为πk(k=1,2,3……K)。显然对于一个数据点Xi，有

![](https://img-blog.csdn.net/20171013211124641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


我们下面的任务就是确定权重πk（也就确定了GMM中GM的个数，也就是“类”的数目），各类均值Uk和各类协方差σ(k)^2 ，这三个参数。

要解决这样一个问题，想法是这样的：找到一组πk，Uk和σ(k)^2，让它所确定的概率分布生成这些给定数据点的概率最大。我们可以想象，如果这样一组πk，Uk，σ(k)^2确定的数据分布与已给数据点完全重合，那么效果最好，也就是我们找到的目标GMM。但是多数情况下，GMM只能达到局部最优，即在局部可以得到收敛解。然而局部最优不一定是全局最优，很有可能得到一个相当差的结果。

典型的例子是人人只为己对整个社会是有害的，会造成双输的局面。我们在这里写博客分享想法就不是出于只为自己考虑的动机~，人只为自己着想，看起来是局部最优，但实际上不是局部最优，长远来看是全局最劣策略。

那既然GMM不是全局最优，为什么还要采用它呢？实际上，即使GMM不是全局最优，通过合理地选定K值和初值Uk，σ(k)^2，也可以得到较好地分类效果。另外，绝对完美的算法很少见，而且改进的GMM也有很多，后面需要用到可以再学习。

所以，没必要纠结算法的完美性，而是要关注不完美算法的应用是否能准确刻画或解决问题的某一方面。

下面，回到之前的想法：找到一组πk，Uk和σ(k)^2，让它所确定的概率分布生成这些给定数据点的概率最大。根据之前的分析，GMM的概率密度等于各类GM在某点出的概率值与其对应权值乘积的结果，即πk*P(x|k)，也就是πk*N(x│u,σ^2
)=1/(σ*√2π) e^((-〖(x-u)〗^2)/(2σ^2 ))，故设在x处GMM的概率密度为P(x)，显然有

![](https://img-blog.csdn.net/20171013213549109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


式中，πk是第k个GM对数据集的贡献，也就是权重；N(x│u,σ^2
 )是在第k个GM时x对应的概率。所以，找到一组πk，Uk和σ(k)^2，让它所确定的概率分布生成这些给定数据点的概率最大，也就是让![](https://img-blog.csdn.net/20171013214124472?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)最大。

下面的处理显然要按照参数估计里面的极大似然估计。这里有一点要注意的是，通常单个Xi点对应的概率P(Xi)很小，相乘会在计算机里容易造成浮点数下溢的情况，故通常会取对数，即，将乘积写成![](https://img-blog.csdn.net/20171013214637469?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。

下面的问题就是数学问题了，只要求得函数最大值（通常的做法是求导并令导数等于0，然后解方程），亦即找到这样一组参数值。它可以让极大似然函数取得最大值，我们就认为这是合适的参数。因为存在局部最优的情况，如下图。

![](https://img-blog.csdn.net/20171013220220540?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


故求导为0的点应该不知一个，应该讲满足导数为0的点全部解出来，找到对应的参数，其结果应该是一个导数为0的点对应一个参数组。代入参数组比较，取最大值对应的参数组应该才是正确的。由于GMM在这里的求解多采用EM算法，故很多资料在这里只讲会找出一组参数值，应该是不正确的。既然承认存在局部最优，那应该至少存在2个局部最优的情况（只存在一个局部最优就是全局最优）。

在求解过程中由于对数函数里又有加和，我们没法直接用求导方程的办法直接解方程。多数采用EM（Expection Maximization Algorithm 期望值最大化算法），EM算法类似于K-means求解的步骤。K-means算法的求解就是EM算法的精髓之处。

EM求解分为两步：

第一步假设我们知道K个GM的参数（可以初始化一个，或者基于上一步迭代结果），也就是知道这K个Uk和σ(k)^2,k=1,2,3……K，显然还要确定权值πk。故下面要做的就是估计每个GM的权值。

对于第i个样本，它由第k个GM生成的概率为：

![](https://img-blog.csdn.net/20171013222206075?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


公式太繁琐，直接借用其他博主的。




[点击打开链接](http://www.cnblogs.com/zhangchaoyang/articles/2624882.html)










