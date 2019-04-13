
# CART与随机森林 - 郭云飞的专栏 - CSDN博客


2017年10月12日 14:40:55[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：1427


**一、前言**
分类与回归树*（Classification and Regression Trees, CART）*是由Leo Breiman, Jerome Friedman, Richard Olshen与Charles Stone于*1984*年提出，既可用于分类也可用于回归。*CART*被称为数据挖掘领域内里程碑式的算法。
题外话，十大经典数据挖掘算法：
C4.5
K-Means
SVM
Apriori
EM
PageRank
AdaBoost
kNN
Naïve Bayes
CART
不同于C4.5，CART本质是对特征空间进行**二元划分**（即CART生成的决策树是**一棵二叉树**），并能够对标量属性（nominal attribute）与连续属性（continuous attribute）进行分裂。
**二、CART算法总体流程**
T代表当前样本集，当前候选属性集用T_attributelist表示。
（1）创建根节点N
（2）为N分配类别
（3）if T都属于同一类别or T中只剩下一个样本则返回N为叶节点，否则为其分配属性
（4）for each T_attributelist中，执行该属性上的一个划分，计算此划分的GINI系数
（5）N的测试属性test_attribute=T_attributelist中最小GINI系数的属性
（6）划分T得到T1、T2子集
（7）对于T1重复（1）-（6）
（8）对于T2重复（1）-（6）
**三、CART生成**
2.1、分支方法
对于连续特征：比较阈值，高于某个阈值就属于某一类，低于某个阈值属于另一类。
![](https://img-blog.csdn.net/20171012121147466?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对于离散特征：抽取子特征，比如颜值这个特征，有帅、丑、中等三个水平，可以先分为帅和不帅的，不帅的里面再分成丑和中等的。
![](https://img-blog.csdn.net/20171012120951385?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2.2、采用*Gini*指数来度量分裂时的不纯度
![](https://img-blog.csdn.net/20171012141448008?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**2.3、 剪枝**
下面这六种情况都会停止分裂：

如果节点中所有观测属于一类（此种情形属于数的完全成长，会出现过拟合问题，需要剪枝）
如果节点中所有观测的属性取值一致
如果树的深度达到设定阈值
如果节点所含观测值小于设定的父节点应含观测数的阈值
如果节点的子节点所含观测数小于设定的阈值
如果没有属性能满足设定的分裂准则的阈值
剪枝分两种：
预剪枝：及早的停止树增长控制树的规模，方法可以参考上边6点停止分类的条件。
后剪枝：在已生成过拟合决策树上进行剪枝，删除没有意义的组，可以得到简化版的剪枝决策树。有REP、PEP、CCP三种规则。
![](https://img-blog.csdn.net/20171012145512970?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
CCP，即给分裂准则—基尼系数加上惩罚项，此时树的层数越深，基尼系数的惩罚项会越大。

**四、CART与逻辑回归的比较**
![](https://img-blog.csdn.net/20171012143632438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20171012143722557?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
主要优缺点如下图：
![](https://img-blog.csdn.net/20171012144054288?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
缺点补充几点：
不是很稳定，数据变化一点，你的树就会发生变化；
没有考虑变量之间相关性，每次筛选都只考虑一个变量（因此不需要归一化）；
只能线性分割数据；
贪婪算法（可能找不到最好的树）。
优点也补充几点：
同时可以处理分类变量和数值变量（但是可能决策树对连续变量的划分并不合理，所以可以提前先离散化）；
可以处理多输出问题；
另外决策树不需要做变量筛选，它会自动筛选；
适合处理高维度数据。
**五、随机森林**

尽管有剪枝等等方法，一棵树的生成肯定还是不如多棵树，因此就有了随机森林，解决决策树泛化能力弱的缺点。（可以理解成三个臭皮匠顶过诸葛亮）
同一批数据，用同样的算法只能产生一棵树，这时*Bagging*策略可以帮助我们产生不同的数据集。*Bagging*策略来源于**bootstrap aggregation**：从样本集（假设样本集N个数据点）中重采样选出Nb个样本（有放回的采样，样本数据点个数仍然不变为N），在所有样本上，对这n个样本建立分类器（ID3\C4.5\CART\SVM\LOGISTIC），重复以上两步m次，获得m个分类器，最后根据这m个分类器的投票结果，决定数据属于哪一类。
随机森林在bagging的基础上更进一步：
样本的随机：从样本集中用Bootstrap随机选取n个样本
特征的随机：从所有属性中随机选取K个属性，选择最佳分割属性作为节点建立CART决策树（泛化的理解，这里面也可以是其他类型的分类器，比如SVM、Logistics）
重复以上两步m次，即建立了m棵CART决策树
这m个CART形成随机森林，通过投票表决结果，决定数据属于哪一类（投票机制有一票否决制、少数服从多数、加权多数）







