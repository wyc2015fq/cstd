# 《机器学习》周志华-CH4 决策树 - PeterBishop - CSDN博客





2019年01月10日 15:15:57[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：19
个人分类：[机器学习](https://blog.csdn.net/qq_40061421/article/category/8552662)









### 第 4 章 决策树4.1 基本流程



决策树（decision tree）是一类常见的机器学习方法。一般的，一棵决策树包含一个根结点、若干个内部结点和若干个叶结点；叶结点对应于决策树结果，其他每个结点则对应于一个属性测试；每个结点包含的样本集合根据属性测试的结果被划分到子结点中；根结点包含样本全集。从根结点到每个叶结点的路径对应了一个判定测试序列。

决策树的学习目的是为了产生一棵泛化能力强，即处理未见示例能力强的决策树，其基本流程遵循简单且直观的“分而治之”（divide-and-conquer）策略。决策树的生成是一个递归过程。





![](https://img-blog.csdnimg.cn/20190110152149316.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

### 4.2 划分选择



决策树学习的关键是如何选择最优划分属性。一般而言，随着划分过程不断进行，我们希望决策树的分支结点所包含的样本尽可能属于同一类别，即结点的“纯度”（purity）越来越高。

### 4.2.1 信息增益



“信息熵”（information entropy）是度量样本集合最常用的一种指标。 

假定当前样本集合 D 中第 k 类样本所占比例为![这里写图片描述](https://img-blog.csdn.net/20170506160319800)，则 D 的信息熵定义为 

![这里写图片描述](https://img-blog.csdn.net/20170506160405270)


Ent(D)的值越小，则 D 的纯度越高。

考虑到不同的分支结点所包含的样本数不同，样本数越多的分支结点的影响越大，于是可计算出用属性a对样本D进行划分所获得的“信息增益”（information gain）： 

![这里写图片描述](https://img-blog.csdn.net/20170506160525250)



![](https://img-blog.csdnimg.cn/20190110192512306.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190110192557156.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190110192639398.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2019011019271327.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



### 4.2.2 增益率



实际上，信息增益准则对可取数目较多的属性有所偏好，为减少这种偏好可能带来的不利影响，著名的 C4.5 决策树不直接使用信息增益，而是使用“增益率”（gain ratio）来选择最优划分属性。增益率定义为： 

![这里写图片描述](https://img-blog.csdn.net/20170506160637070)


其中![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20170506160656891)称为属性a的“固有值”（intrinsic value）。

增益率准则对可取数目较少的属性有所偏好，因此，C4.5算法并不是直接选择增益率最大的候选划分属性，而是使用了一个启发式：先从候选划分属性中找出信息增益高于平均水平的属性，再从中选择增益率最高的。

![](https://img-blog.csdnimg.cn/20190110194019210.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

### 4.3 剪枝处理



剪枝（pruning）是决策树学习算法对付“过拟合”的主要手段。 决策树剪枝的基本策略有“预剪枝”（prepruning）和“后剪枝”（postpruning）。

### 4.3.1 预剪枝



预剪枝是指在决策树生成过程中，对每个结点在划分前先进行估计，若当前结点的划分不能带来决策树泛化性能提升，则停止划分并将当前结点标记为叶结点。

预剪枝优缺点

预剪枝使得决策树的很多分支都没有“展开”，这不仅降低了过拟合的风险，还显著减少了决策树的训练时间开销和测试时间开销，但另一方面，有些分支的当前划分虽不能提升泛化性能、甚至可能导致泛化性能暂时下降，但在其基础上进行的后续划分却有可能导致性能显著提高；预剪枝基于“贪心”本质禁止这些分支展开，给预剪枝决策树带来了欠拟合的风险。

![](https://img-blog.csdnimg.cn/20190110194935832.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

### 4.3.2 后剪枝



后剪枝则是先从训练集生成一棵完整的决策树，然后自底向上地对非叶结点进行考察，若将该结点对应的子树替换为叶结点能带来决策树泛化性能提升，则将该子树替换为叶结点。

后剪枝优缺点

后剪枝决策树通常比预剪枝决策树保留了更多的分支。一般情形下，后剪枝决策树的欠拟合风险很小，泛化性能往往优于预剪枝决策树。但后剪枝过程是在生成完全决策树之后进行的，并且要自底向上地对数中的所有非叶结点进行逐一考察，因此其训练时间开销比未剪枝决策树和预剪枝决策树要大得多。

### 4.4 连续与缺失值4.4.1 连续值处理



对于连续属性可以使用连续属性离散化技术，最简单的策略是采用二分法（bi-partition）对连续属性进行处理，这是 C4.5 决策树中采用的机制。

![](https://img-blog.csdnimg.cn/20190110200654559.png)

对连续属性a，我们可考察包含 n-1 个元素的候选划分点集合 

![这里写图片描述](https://img-blog.csdn.net/20170506160926542)


即把区间![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20170506161426157)的中位点![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20170506161205190)作为候选划分点。

### 4.4.2 缺失值处理



现实任务中常会遇到某些属性值缺失的不完整样本，此时需要解决两个问题：（1）如何在属性值缺失的情况下进行划分属性选择？（2）给定划分属性，若样本在该属性上的值缺失，如何对样本进行划分？

对于问题（1）可以根据训练集中在属性a上没有缺失值的样本子集来判断属性a的优劣，从而划分属性选择；对于问题（2）若样本 x 在划分属性 a 上的取值已知，则将 x 划入与其取值对应的子结点。若样本 x 在划分属性 a 上的取值未知，就让同一个样本以不同的概率划入到不同的子结点中去。

### 4.5 多变量决策树



决策树所形成的分类边界有一个明显的特点：轴平行（axis-parallel），即它的分类边界由若干个与坐标轴平行的分段组成。“多变量决策树”（multivariate decision tree）能实现“斜划分”甚至更复杂划分的决策树。

与传统的“单变量决策树”（univariate decision tree）不同，在多变量决策树的学习过程中，不是为每个非叶结点寻找一个最优划分属性，而是试图建立一个合适的线性分类器。




