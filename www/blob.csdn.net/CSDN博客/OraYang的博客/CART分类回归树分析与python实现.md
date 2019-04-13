
# CART分类回归树分析与python实现 - OraYang的博客 - CSDN博客

2017年11月01日 16:27:16[OraYang](https://me.csdn.net/u010665216)阅读数：2526所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



## 引言
前面我们分享过一篇决策树算法叫ID3:[ID3决策树原理分析及python实现](http://blog.csdn.net/u010665216/article/details/78173064)。首先我们来回顾下ID3算法。ID3每次选取最佳特征来分割数据，这个最佳特征的判断原则是通过信息增益来实现的。这种按某种特征切分完数据集后，当前特征在下次切分数据集时就不再起作用，因此会存在切分方式过于迅速地问题。ID3算法还存在另一个问题就是它不能直接处理连续型特征，因此算法需要改进。于是有人提出了二元切分法很好的解决了连续性变量问题及切分迅速的问题。其中代表性算法就是CART。
## CART分类回归树
CART是Classification And Regression Trees的缩写叫做“分类回归树”。它既能做分类任务又能做回归任务。
> 分类树：目标变量是类别数据，树被用来识别目标变量可能属于哪个类
![cla](https://img-blog.csdn.net/20171101101319055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 回归树：目标变量是连续数据，树被用来预测目标变量的值是多少
![reg](https://img-blog.csdn.net/20171101101751122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> CART树的典型代表就是二叉树，如下图所示：
![cart-bin](https://img-blog.csdn.net/20171101101950879?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> CART树构建算法

> 因为ID3决策树分享时，我们已经分享过树构建算法的流程，这里我们就直接来实现CART树构建的过程。

> 树构建框架

> 在树的构建过程中，与ID3类似采用字典来存储树的数据结构，该字典包含以下4种元素：

> 待切分的特征

> 待切分的特征值

> 右子树。当不再需要切分的时候，也可以是单个值

> 左子树。与右子树类似

> 函数createTree()伪代码如下：
![createTree](https://img-blog.csdn.net/20171101102712206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20171101102712206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 可以很明显的发现这是个典型的递归算法。

> 这个算法与ID3创建分支的算法createBranch()十分类似:
![id3](https://img-blog.csdn.net/20171101103429649?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20171101103429649?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 划分数据集的代码如下：

> [

](https://img-blog.csdn.net/20171101103429649?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> def
> binSplitDataSet
> (dataSet, feature, value)
> :
> mat0 = dataSet[nonzero(dataSet[:,feature] > value)[
> 0
> ],:]
    mat1 = dataSet[nonzero(dataSet[:,feature] <= value)[
> 0
> ],:]
> return
> mat0,mat1
> [
](https://img-blog.csdn.net/20171101103429649?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 通过数组过滤方式将数据集合切分得到两个子集并返回。

> [

](https://img-blog.csdn.net/20171101103429649?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 划分数据点

> [
](https://img-blog.csdn.net/20171101103429649?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 创建二进制决策树本质上就是递归划分输入空间的过程。有一种贪心的算法用来划分这个空间被称为递归二进制划分。所有输入变量和所有可能的分割点都以贪婪的方式进行评估和选择(例如，每次选择最佳的分割点)。

> [
](https://img-blog.csdn.net/20171101103429649?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 对分类问题来说
> ，基尼指数（Gini index）被用来选择划分的属性，数据集的纯度可用基尼值来衡量：

> Gini(D) = 1 - \sum_{k=1}^{|y|}p_k^2

> 直观地来说，Gini(D)反映了从数据集D中随机抽取两个样本，其类别标记不一致的概率。因此Gini(D)越小，则数据集D的纯度越高。对于二分类问题，这就可以写成:

> Gini(D) = 1-p_1^2-p_2^2

> 那么属性a的基尼指数定义为

> Gini\_index(D,a)=\sum_{v=1}^V\frac{|D^v|}{|D|}Gini(D^v)

> 于是，我们在候选属性集合A中，选择那个使得划分后基尼指数最小的属性作为最优划分属性。

> 对回归问题来说
> ，用最小代价函数来划分数据点，度量方法就是平方误差的总值（总方差）

> 构建树

> 在我们构建树的伪代码中有一个“find the best feature to split data”函数，前面我们已经分析了构建回归树时的误差计算方法，根据这个方法就能找到数据集上最佳的二元切分方式。因此这个函数将会完成两部分内容：1.用最佳方式切分数据集   2.生成相应的叶节点

> 我们实现“find the best feature to split data”函数chooserBestSplit()，它的伪代码如下：
![choose](https://img-blog.csdn.net/20171101133310699?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20171101133310699?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 上述伪代码的目标是找到数据集切分的最佳位置。它遍历所有的特征及其可能的取值来确定使误差最小化的切分阈值。

> 代码如下：

> def
> chooseBestSplit
> (dataSet, leafType=regLeaf, errType=regErr, ops=
> (
> 1
> ,
> 4
> )
> )
> :
> tolS = ops[
> 0
> ]; tolN = ops[
> 1
> ]
> \#if all the target variables are the same value: quit and return value
> if
> len(set(dataSet[:,-
> 1
> ].T.tolist()[
> 0
> ])) ==
> 1
> :
> \#exit cond 1
> return
> None
> , leafType(dataSet)
    m,n = shape(dataSet)
> \#the choice of the best feature is driven by Reduction in RSS error from mean
> S = errType(dataSet)
    bestS = inf; bestIndex =
> 0
> ; bestValue =
> 0
> for
> featIndex
> in
> range(n-
> 1
> ):
> for
> splitVal
> in
> set(dataSet[:,featIndex]):
            mat0, mat1 = binSplitDataSet(dataSet, featIndex, splitVal)
> if
> (shape(mat0)[
> 0
> ] < tolN)
> or
> (shape(mat1)[
> 0
> ] < tolN):
> continue
> newS = errType(mat0) + errType(mat1)
> if
> newS < bestS: 
                bestIndex = featIndex
                bestValue = splitVal
                bestS = newS
> \#if the decrease (S-bestS) is less than a threshold don't do the split
> if
> (S - bestS) < tolS:
> return
> None
> , leafType(dataSet)
> \#exit cond 2
> mat0, mat1 = binSplitDataSet(dataSet, bestIndex, bestValue)
> if
> (shape(mat0)[
> 0
> ] < tolN)
> or
> (shape(mat1)[
> 0
> ] < tolN):
> \#exit cond 3
> return
> None
> , leafType(dataSet)
> return
> bestIndex,bestValue
> \#returns the best feature to split on
> \#and the value used for that split
> 上述代码有出返回即函数退出条件：1.剩余特征数目为1;直接返回 2.切分数据集后效果提升不大，不需进行切分操作而直接创建叶节点;3提前终止条件均不满足，返回切分特征和特征值。

> 剪枝

> 在决策树的学习中，有时会造成决策树分支过多，这时就需要去掉一些分支从而降低过拟合的风险。通过降低决策树复杂度来避免过拟合的过程称为剪枝。一种是预剪枝，一种是后剪枝。

> 预剪枝

> 前面我们的chooseBestSplit（）代码里已经有了预减枝的处理就是提前终止条件。但是树构建算法对这些提前终止条件很敏感，通过不断地修改停止条件来得到合理地结果并不是很好的办法。于是就有人提出了利用测试集对树进行减枝就是我们接下来要分析的后剪枝。

> 后剪枝

> 后剪枝需要从训练集生成一棵完整的决策树，然后自底向上对非叶子结点进行考察，利用测试集判断若将该结点对应的子树替换成叶结点，能否带来决策树泛化性能的提升？将上述思路转换成伪代码如下：
![prune](https://img-blog.csdn.net/20171101150604599?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 总结

> 本次博文主要分享了CART树的构建过程，然后稍稍介绍了下剪枝的方法


> [
  ](https://img-blog.csdn.net/20171101133310699?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
