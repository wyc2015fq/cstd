# PCA和LDA - 快来学习鸭～～～ - CSDN博客





2016年12月28日 13:58:42[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：2742








[欢迎访问我的blog](http://blog.csdn.net/sxf1061926959)
[本文是PCA的简单推导和理解，详细了解各个演化版本的PCA可以参考WIKI](https://en.wikipedia.org/wiki/Principal_component_analysis)

第二块数学基础内容很简单，都是高中知识，但是还是看一下，这样理解PCA就很快了
# 1、为什么要PCA

在真实的数据中总是会存在许多的冗余信息，比如： 

1、一支笔的长度是15cm，也可以说是0.15m。但是把这两者当作两个特征，那么他们所表达的意义其实是完全一样的。这里我要表达的就是两个特征之间的相关性，如果两个特征相关性很强，那么完全可以合并成一个特征。 

2、如果给你很多人的特征：身高、体重、头发长度、腿长、单眼皮/双眼皮、酒窝、大眼睛/小眼睛、黑头发/黄头发、是否有鼻炎……然后根据这些内容去区分是男人还是女人，但是呢，现在给你的人的样本数量又不多，里面刚好有鼻炎的全是女生，没有鼻炎的全是男生，但是其实鼻炎对于区分男女其实根本没有啥作用。（这就叫过拟合），所以很有必要对数据进行降维，去掉那些没有用的噪声。
# 2、数学基础

## 2.1、标准差、方差

标准差和方差可以表达出数据的抖动情况。 

假设现在有十个人的数学成绩形成的数据集 
![这里写图片描述](https://img-blog.csdn.net/20161228133021672?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

那么均值就是 
![这里写图片描述](https://img-blog.csdn.net/20161228133152674?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

即，对全部的十个分数求和，再除以十。 

标准差计算方法: 
![这里写图片描述](https://img-blog.csdn.net/20161228133220159?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

即数据到其均值的平均距离。s大说明数据离散程度大，s小说明数据离散程度小，数据比较集中。 

方差的定义和标准差类似，是标准差的平方 
![这里写图片描述](https://img-blog.csdn.net/20161228133300050?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 2.2、协方差（Covariance）

前面的标准差，方差都是针对一维数据的，如果数据集的维度比较高，那么不能用了，比如对每个同学提取的不只是数学成绩，还有英语成绩。而标准差这种在一维数据上操作，那么对于高维数据，标准差也只能计算单独每个维度的标准差。**但是其实去找出不同维度之间的均值的差异是多少，这个也是很重要的一个属性。**

所以引入了协方差。 

将方差展开 
![这里写图片描述](https://img-blog.csdn.net/20161228133354911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

而协方差的形式为 
![这里写图片描述](https://img-blog.csdn.net/20161228133429833?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

协方差所表达的意义：假设上面的X代码的是数学成绩，Y代表的是英语成绩， 

那么如果算出来得协方差是正数，那么说明当X增加的时候，大部分的Y也是会增加的，因为只有这样，对他们求和后才可能是正数。如果算出来的协方差是负数，那么说明当X增加的时候，绝大部分情况下Y会减小，他们之间是负相关的。如果是0，或者说接近于0的时候，说明X和Y的相关度不是很大，有的同学X增加Y增加，有的同学X增加Y减少，而且出现这两种情况的概率五五开。 

对于二维数据，比较他们之间的相关性，完全可以通过画图，用可视化的方式看出来，但是也只限维度为2的情况下，当维度大于2的时候就无法通过可视化的方式来分析了，但是协方差可以，因为他可以计算任意两个维度之间的协方差值。所以要引入协方差矩阵了。 
**上面的X和Y是等价的，可以互相交换位置，互相代替。**
## 2.3、协方差矩阵

如果协方差是计算二维数据，如果一个数据集的维度大于2，那么就要计算多个协方差值了。比如一个三维的数据集（维度为x,y,z），那么我们能计算cov(x,y),cov(x,z),cov(y,z)。对于一个n维的数据，我们能计算![这里写图片描述](https://img-blog.csdn.net/20161228133539724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)个不同的协方差值。 

一个可行的方法是去计算出全部的协方差，然后把协方差值组成一个矩阵： 
![这里写图片描述](https://img-blog.csdn.net/20161228133610648?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其实![这里写图片描述](https://img-blog.csdn.net/20161228133631618?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)是一个有n行n列的矩阵，![这里写图片描述](https://img-blog.csdn.net/20161228133653899?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)表示第x维度。即如果有一个n维的数据集，那么这个矩阵就有n行n列，这个矩阵中的每一个值都是不同两个维度之间的协方差值，即矩阵中第2行第3列的元素是数据集中第二维度和第三维度数据的协方差值。可以想象，这个矩阵是一个对称矩阵。如下有x,y,z三维的数据集的协方差矩阵。 
![这里写图片描述](https://img-blog.csdn.net/20161228133731072?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 2.4、矩阵代数

## 2.4.1、特征向量

先给两个例子： 

例1一个非特征向量和一个特征向量 
![这里写图片描述](https://img-blog.csdn.net/20161228133758882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

例2特征向量尺度变换后还是特征向量 
![这里写图片描述](https://img-blog.csdn.net/20161228133858778?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在第一个例子里，第一个公式计算出的向量和乘法运算中的向量不存在整数倍的关系。而第二个公式中的结果是乘法运算中向量的4倍。 

其实特征向量表达的意义是这样的，上例中![这里写图片描述](https://img-blog.csdn.net/20161228133933649?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)表达的是一个点（0，0）到（3，2） 

的有向向量。而前面的![这里写图片描述](https://img-blog.csdn.net/20161228133957857?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)表达的是一个用于转换向量的转换矩阵。再经过这个矩阵转换后的向量和原始向量还是保持原来的方向，即只是发生了尺度变换，那么这个向量就是特征向量。 

一个有特征向量的nxn的矩阵，一定有n个特征向量。而且这n个向量正交。因为特征向量可以尺度变换，所以把同方向的特性向量看作一个向量，我们基本会对特征向量进行归一化，即如下 
![这里写图片描述](https://img-blog.csdn.net/20161228134026118?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)是一个特征向量，它的长度是![这里写图片描述](https://img-blog.csdn.net/20161228134046525?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，所以我们都会把他归一化为![这里写图片描述](https://img-blog.csdn.net/20161228134114078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 2.4.2特征值

特征值就是计算特征向量时产生的那个数，就是上例中的4，对于同方向的特征向量，不管它的尺度怎么变，特征值是不会变的，如例2中，特征值一直是4。

# 3、PCA

PCA能找到数据中的相似性和不同性。如果能找到数据中的一些相似性，PCA就能对数据进行压缩，即降维，但是不会损失数据的信息。

## 3.1具体步骤

**Setp1:得到数据**

本例用一个两维的数据来表示。 
![这里写图片描述](https://img-blog.csdn.net/20161228134155157?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如图所示中的Data为我们使用的原始数据。将其画出后如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20161228134220385?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**Setp2：原始数据减去均值**

为了能使得PCA正确运行，对每一个维度减去其均值是很有必要的，所以全部的x都要减去x的均值，全部的y值都要减去y的均值。得到如下图所示的DataAdjust。 
![这里写图片描述](https://img-blog.csdn.net/20161228134248330?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**Step3：计算协方差矩阵**

和我们前面介绍的计算协方差矩阵的方式一样，我们计算数据的协方差矩阵，因为数据是二维的，所以得到一个二维的协方差矩阵： 
![这里写图片描述](https://img-blog.csdn.net/20161228134319972?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**可以看到非对角线元素都是正的，说明x和y是成正相关的。**
**Step4：计算协方差矩阵的特征值和特征函数。**

前面已经介绍了特征值和特征函数的形式和意义，具体的求解过程这里不做讨论。 

我们直接得出特征值和特征向量： 
![这里写图片描述](https://img-blog.csdn.net/20161228134356754?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
*注意：我们用到的特征向量都是单位向量，这对PCA很重要。*
***那么为什么要提取特征向量和特征值呢？***

下面我们将特征向量和经过均值处理的数据一起画在一张图上。 
![这里写图片描述](https://img-blog.csdn.net/20161228134508714?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以看出，经过均值处理的数据确实是呈现出正相关的，能看出来它们都分布在y=x附近。然后我们也画出了两条特征向量。其中一条很接近与y=x，我们能够发现，所以的数据其实是围绕在这条特征向量附近的，感觉这条特征向量就是这些数据的最佳拟合线段。这条特征向量告诉了我们，这两维数据是怎么相关的。而第二条特征向量是垂直与第一条的，前面我们已经说过了，所以的特征向量都是相互垂直的。但是第二条特征向量没有告诉我们有用的信息。 

所以，把协方差均值的特征向量提取出来后，他能告诉我们数据的一些**线性关系。**
**Step5：选择主成份（主要的特征向量），形成特征向量**

前面我们说了，两条特征向量中，一条很好的拟合了数据，另外一条却没有显示什么重要性。所以这步，我们就要把重要的提取出来，删去不重要的。从而来实现数据的压缩。 

可以发现前面的两个特征向量的特征值是完全不同，一个值大，一个值小。***已经得到证明，拥有高的特征值的特征向量是数据集的主要组成成分（principle component）。***前面分析了，在我们的例子中，拥有高特征值的特征向量是数据的最佳拟合直线，是两个特征向量中最有意义的一个。 

所以，通过对于一个给定的数据集，我们会计算出所以的特征值和特征函数，然后按特征值从大到小进行排序，特征值越大说明越重要。然后排序好后，可自行根据你要降维多少来确定删除多少个组成成分（特征向量）。当然删除的特征值越大的组成成分，损失的信息也会越多，所以尽量删除特征值很小的组成成分。 
**通俗点说就是把特征值全部按从大到小排序，留下前p个最大的。**
**怎么形成特征向量？**

把所以的特征向量提取出来，形成一个矩阵： 
![这里写图片描述](https://img-blog.csdn.net/20161228134638668?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们的例子中就是 
![这里写图片描述](https://img-blog.csdn.net/20161228134713137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后根据我们前面选择的**step4**选择的特征值来保留相应的特征向量，删除没有保留特征值的特征向量。我们例子中选择一个大的就是 
![这里写图片描述](https://img-blog.csdn.net/20161228134751229?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**Step6：生成新的降维数据集**

这步是PCA中的最后一步，也是最简单的一步。一旦我们选择了主成份（主要的特征向量），那么我们只要把它左乘原始均值化的数据集就行了。 
![这里写图片描述](https://img-blog.csdn.net/20161228134833934?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在这里特征向量和数据集都经过转置，对于我们的例子来说，RowFeatureVector是1x2的矩阵，RowDataAdjust是2x10的矩阵。到这里为止，我们得到的FInalData就是最终的降维后的矩阵，变成了1x10的数据集。原来的数据集有x,y两维，现在只有一维了。
## 3.2、如何返回得到降维前的数据

根据前面的公式 
![这里写图片描述](https://img-blog.csdn.net/20161228134932091?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们只需要记住这个特征向量![这里写图片描述](https://img-blog.csdn.net/20161228135024419?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，然后根据矩阵变换，就能得到原始数据： 
![这里写图片描述](https://img-blog.csdn.net/20161228135057450?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后再加上原来的均值： 
![这里写图片描述](https://img-blog.csdn.net/20161228135136375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因为中间删去了一些特征向量，所以会损失一些信息，得到的原始数据集会存在一定误差，如下图： 
![这里写图片描述](https://img-blog.csdn.net/20161228135159670?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**到此位置PCA的整个过程都介绍完了，其实很简单，就几步，只要搞懂协方差和特征函数就相当于搞懂了PCA。**
# 参考资料：

A tutorial on Principal Components Analysis.Lindsay I Smith,February 26,2002.

## 1、LDA介绍

LDA(LInear discriminant analysis)也叫做FIsher linear discriminant。目的是为了找到一种特征的**线性组合**，用于表达或者区分多类标签。它们产生的结果可用于线性分类，或者用于分类前的降维。

## 2.计算过程

这里先简单介绍一下只有两个类的计算方法，再进一步推广到多个类别，以便于理解。 
![这里写图片描述](https://img-blog.csdn.net/20161229203344707?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

LDA的目的就是要找到上图中可以将红和蓝两类很好区分出来的那个直线（向量）。 

假设，我们的原始数据为X,我们要找的投影向量是W，经过投影的数据为Y。 

现在我们要做的是找到一个投影使得类间距离最大，类内距离最短。我们分别用类间方差和类内方差来表示这两个距离。 
**先假定我们的数据：**
**假定x是3维的样本集，类别为2，y是x经过w投影后的数据。**
**类别i的均值：**
![这里写图片描述](https://img-blog.csdn.net/20161229203439098?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，![这里写图片描述](https://img-blog.csdn.net/20161229203529380?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)是属于类![这里写图片描述](https://img-blog.csdn.net/20161229203551208?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)的数量，这里x是3维的属于Di的一个样本。** 
**类别i经过w投影后的均值：**
![这里写图片描述](https://img-blog.csdn.net/20161229203629397?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**经过投影后的均值平方差（类间方差）：**
![这里写图片描述](https://img-blog.csdn.net/20161229203700741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)** 
**投影后的类内方差：**
![这里写图片描述](https://img-blog.csdn.net/20161229203733302?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)** 
**而我们要优化的函数就是：**
![这里写图片描述](https://img-blog.csdn.net/20161229203755617?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们需要从这个优化函数中找出那个使得J(w)最小的向量w，所以我们需要对其进行展开， 
**从分母开始:**
![这里写图片描述](https://img-blog.csdn.net/20161229203830849?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20161229203923275?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里我们把同类的**方差和**定义为Sw。 
**分子展开：**
![这里写图片描述](https://img-blog.csdn.net/20161229204009037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，这里我们把整体均值和类均值的平方差定义为SB. 

那么我们的**优化目标**就可以写成： 
![这里写图片描述](https://img-blog.csdn.net/20161229204040491?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)， 

然后我们要求的就是w，只有求出w使得J（w）最小，我们的目的就达到了。具体的推导过程，我这就不写了，我也没看，都是些数学推导。 
**求出的w是：**
![这里写图片描述](https://img-blog.csdn.net/20161229204119819?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我前面这里写的是针对两个类的， 
**对于多类别：**
![这里写图片描述](https://img-blog.csdn.net/20161229204145647?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对于Sb，之前两个类的是均值的差，现在有多个类了，LDA**计算每个类别的均值与整个数据集均值的差异**，并且对每个类别的数据量**进行加权处理**。如下所示：u为所有类别的均值，ui为类别i的均值，ni为类别i的数量。 
![这里写图片描述](https://img-blog.csdn.net/20161229204234593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
参考资料： 
[http://blog.csdn.net/porly/article/details/8020696](http://blog.csdn.net/porly/article/details/8020696)

Linear Discriminant analysis-a brief tutorial.S.Balakrishnama,A.Ganapathiraju. 
[https://en.wikipedia.org/wiki/Linear_discriminant_analysis](https://en.wikipedia.org/wiki/Linear_discriminant_analysis)















