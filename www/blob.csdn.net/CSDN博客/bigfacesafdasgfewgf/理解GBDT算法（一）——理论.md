# 理解GBDT算法（一）——理论 - bigfacesafdasgfewgf - CSDN博客





2014年12月16日 10:32:18[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：24978标签：[GBDT																[决策树																[集成学习																[残差																[预测和分类](https://so.csdn.net/so/search/s.do?q=预测和分类&t=blog)
个人分类：[Machine Learning&&Data Mining](https://blog.csdn.net/puqutogether/article/category/2254295)





**理解GBDT算法（一）——理论**



    关于决策树decision tree的组合模型有两种：random forest 和 GBDT （gradient boosting decision tree）。上一篇我们说了《理解随机森林》，这次我们来说下以下GBDT的理论模型，后面还有连续的两片分别将GBDT之应用、GBDT之源码。




# 1. GBDT的基本思想——积跬步以至千里




    我们前面讲到，一棵决策树很容易出现过拟合现象。但是，我们把训练集通过反复学习（或者采样，或者不采样），得到多颗决策树，这样就可以一定程度上避免过拟合。前面的random forest是又放回地随机采样数据集，再随机采样特征，学习出来多课决策树，然后我们再投票决策分类。

    而这篇讲到的GBDT的基本思想是“**积跬步以至千里**”！也就是说我每次都只学习一点，然后一步步的接近最终要预测的值（完全是gradient的思想~）。换句话来说，我们先用一个初始值来学习一棵决策树，叶子处可以得到预测的值，以及预测之后的残差，然后后面的决策树就要基于前面决策树的残差来学习，直到预测值和真实值的残差为零。最后对于测试样本的预测值，就是前面许多棵决策树预测值的累加。整个过程都是每次学习一点（真实值的一部分），最后累加，所以叫做“**积跬步以至千里**”！




# 2. 举例说明




    网上流传了一个预测年龄的例子，例子举的很好，当时解释的过程没有结合GBDT的工作过程，这里我们改进一下：




训练集：（A, 14岁）、（B，16岁）、（C, 24岁）、（D, 26岁）；

训练数据的均值：20岁；   （这个很重要，因为GBDT与i开始需要设置预测的均值，这样后面才会有残差！）

决策树的个数：2棵；

每个样本的特征有两个：购物金额是否小于1K；经常去百度提问还是回答；




    开始GBDT学习了~




首先，输入初值20岁，根据第一个特征（具体选择哪些特征可以根据信息增益来计算选择），可以把4个样本分成两类，一类是购物金额<=1K，一类是>=1K的。假如这个时候我们就停止了第一棵树的学习，这时我们就可以统计一下每个叶子中包含哪些样本，这些样本的均值是多少，因为这个时候的均值就要作为所有被分到这个叶子的样本的预测值了。比如AB被分到左叶子，CD被分到右叶子，那么预测的结果就是：AB都是15岁，CD都是25岁。和他们的实际值一看，结果发现出现的残差，ABCD的残差分别是-1,
 1, -1, 1。这个残差，我们要作为后面第二棵决策树的学习样本。




![](https://img-blog.csdn.net/20141216113705604?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





然后学习第二棵决策树，我们把第一棵的残差样本（A, -1岁）、（B，1岁）、（C, -1岁）、（D, 1岁）输入。此时我们选择的特征是经常去百度提问还是回答。这个时候我们又可以得到两部分，一部分是AC组成了左叶子，另一部分是BD组成的右叶子。那么，经过计算可知左叶子均值为-1，右叶子均值为1.
 那么第二棵数的预测结果就是AC都是-1，BD都是1. 我们再来计算一下此时的残差，发现ABCD的残差都是0！停止学习~




![](https://img-blog.csdn.net/20141216114115859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





这样，我们的两棵决策树就都学习好了。进入测试环节：




测试样本：请预测一个购物金额为3k，经常去百度问淘宝相关问题的女生的年龄~




我们提取2个特征：购物金额3k，经常去百度上面问问题；




第一棵树 —> 购物金额大于1k —> 右叶子，初步说明这个女生25岁

第二棵树 —> 经常去百度提问 —> 左叶子，说明这个女生的残差为-1；




叠加前面每棵树得到的结果：25-1=24岁，最终预测结果为24岁~




以上就是GBDT的全部过程，更多的细节我们在介绍GBDT的应用，以及GBDT的源码的时候，再去详细说明！](https://so.csdn.net/so/search/s.do?q=残差&t=blog)](https://so.csdn.net/so/search/s.do?q=集成学习&t=blog)](https://so.csdn.net/so/search/s.do?q=决策树&t=blog)](https://so.csdn.net/so/search/s.do?q=GBDT&t=blog)




