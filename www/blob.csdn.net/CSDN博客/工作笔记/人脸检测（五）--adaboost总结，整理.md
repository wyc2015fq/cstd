# 人脸检测（五）--adaboost总结，整理 - 工作笔记 - CSDN博客





2016年02月03日 09:17:46[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：14738
个人分类：[人脸检测](https://blog.csdn.net/App_12062011/article/category/7574422)

所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









20170706新增:

各种Adaboost variants的比较以及不均衡问题 

[http://blog.csdn.net/scarecrow_wiscom/article/details/49803125](http://blog.csdn.net/scarecrow_wiscom/article/details/49803125)

1.这边有个浅显的解释。

[http://blog.csdn.net/haidao2009/article/details/7514787](http://blog.csdn.net/haidao2009/article/details/7514787)



2.简单解释到代码实现。

[http://blog.csdn.net/dark_scope/article/details/14103983](http://blog.csdn.net/dark_scope/article/details/14103983)



3.相对来说，这个看起来不是个人总结。



原文：[http://www.kuqin.com/shuoit/20141104/343009.html](http://www.kuqin.com/shuoit/20141104/343009.html)



4.手动C代码Adaboost + stump弱分类器

[http://blog.csdn.net/lampqiu/article/details/41520821](http://blog.csdn.net/lampqiu/article/details/41520821)

[http://blog.sina.com.cn/s/blog_7fac59eb0100tfyr.html](http://blog.sina.com.cn/s/blog_7fac59eb0100tfyr.html)

<iframe name="google_esf" id="google_esf" src="https://googleads.g.doubleclick.net/pagead/html/r20160128/r20151006/zrt_lookup.html" style="display: none;" data-ad-client="ca-pub-9317413389774415"></iframe>

### 0 引言

1 Adaboost的原理

1.1 Adaboost是什么

AdaBoost，是英文"Adaptive Boosting"（自适应增强）的缩写，由Yoav Freund和Robert Schapire在1995年提出。它的自适应在于：前一个基本分类器分错的样本会得到加强，加权后的全体样本再次被用来训练下一个基本分类器。

AdaBoost是一种迭代算法，在每一轮中加入一个新的弱分类器，直到达到某个预定的足够小的错误率。每一个训练样本都被赋予一个权重，表明它被某个分类器选入训练集的概率。如果某个样本点已经被准确地分类，那么在构造下一个训练集中，它被选中的概率就被降低；相反，如果某个样本点没有被准确地分类，那么它的权重就得到提高。

在具体实现上，最初令每个样本的权重都相等，对于第k次迭代操作，我们就根据这些权重来选取样本点，进而训练分类器。然后就根据这个分类器，来提高被它分错的的样本的权重，并降低被正确分类的样本权重。然后，权重更新过的样本集被用于训练下一个分类器。整个训练过程如此迭代地进行下去。

1.2 Adaboost算法流程

给定一个训练数据集T={(x1,y1), (x2,y2)…(xN,yN)}，其中实例![x in mathcal{X}](http://img.kuqin.com/upimg/allimg/141104/2039334591-0.png)，而实例空间![mathcal{X} subset mathbb{R}^n](http://img.kuqin.com/upimg/allimg/141104/20393352W-1.png)，yi属于标记集合{-1,+1}，Adaboost的目的就是从训练数据中学习一系列弱分类器或基本分类器，然后将这些弱分类器组合成一个强分类器。

Adaboost的算法流程如下：
- ***1.***首先，初始化训练数据的权值分布。每一个训练样本最开始时都被赋予相同的权重：1/N。

> 
> 
![](https://img-blog.csdn.net/20141102234630160)



接下来，如果某个样本点已经被准确地分类，那么在构造下一个训练集中，它被选中的概率就被降低；相反，如果某个样本点没有被准确地分类，那么它的权重就得到提高。具体说来，则是：
- ***2.***对于m = 1,2, ..., M

***a***.使用具有权值分布Dm的训练数据集学习，得到基本二元分类器：

> 
> 
![](https://img-blog.csdn.net/20141102234909561)



***b***.计算Gm(x)在训练数据集上的分类误差率

> 
> ![](https://img-blog.csdn.net/20141102235141318)


***c***. 计算Gm(x)的系数，am表示Gm(x)在最终分类器中的重要程度：

> 
> ![](https://img-blog.csdn.net/20141102235307399)

由上述式子可知，em <= 1/2时，am >= 0，且am随着em的减小而增大，意味着分类误差率越小的基本分类器在最终分类器中的作用越大。

***d***. 更新训练数据集的权值分布

> 
![](https://img-blog.csdn.net/20141103000618960)


> 使得被基本分类器Gm(x)误分类样本的权值增大，而被正确分类样本的权值减小。就这样，通过这样的方式，AdaBoost方法能“聚焦于”那些较难分的样本上。

其中，Zm是规范化因子，使得Dm+1成为一个概率分布：

> 
> 
![](https://img-blog.csdn.net/20141103000759596)


- ***3.***构建基本分类器的线性组合

> 
![](https://img-blog.csdn.net/20141103001101875)


从而得到最终分类器，如下：

> 
> 
![](https://img-blog.csdn.net/20141103001155359)



1.3 Adaboost的一个例子

> 
> 


下面，给定下列训练样本，请用AdaBoost算法学习一个强分类器。

> 
![](https://img-blog.csdn.net/20141103002143995)


求解过程：初始化训练数据的权值分布，令每个权值W1i = 1/N = 0.1，其中，N = 10，i = 1,2, ..., 10，然后分别对于m = 1,2,3, ...等值进行迭代。

**迭代过程1**：对于m=1，在权值分布为D1的训练数据上，阈值v取2.5时误差率最低，故基本分类器为：

> 
> 
![](https://img-blog.csdn.net/20141103002826164)



从而可得G1(x)在训练数据集上的误差率e1=P(G1(xi)≠yi) = 0.3

然后计算G1的系数：

> 
> 
![](https://img-blog.csdn.net/20141103003013274)



接着更新训练数据的权值分布：

> 
> ![](https://img-blog.csdn.net/20141103003345106)


最后得到各个数据的权值分布D2=(0.0715, 0.0715, 0.0715, 0.0715, 0.0715, 0.0715, 0.1666, 0.1666, 0.1666, 0.0715)，分类函数f1(x)=0.4236G1(x)，故最终得到的分类器sign(f1(x))在训练数据集上有3个误分类点。

**迭代过程2**：对于m=2，在权值分布为D2的训练数据上，阈值v取8.5时误差率最低，故基本分类器为：

> 
> 
![](https://img-blog.csdn.net/20141103004412446)



G2(x)在训练数据集上的误差率e2=P(G2(xi)≠yi) = 0.2143

计算G2的系数：

> 
> 
![](https://img-blog.csdn.net/20141103004526967)



更新训练数据的权值分布：

> 
> ![](https://img-blog.csdn.net/20141103004728086)


D3=(0.0455, 0.0455, 0.0455, 0.1667, 0.1667, 0.01667, 0.1060, 0.1060, 0.1060, 0.0455)

f2(x)=0.4236G1(x) + 0.6496G2(x)

分类器sign(f2(x))在训练数据集上有3个误分类点。

**迭代过程3**：对于m=3，在权值分布为D3的训练数据上，阈值v取5.5时误差率最低，故基本分类器为：

> 
> 
![](https://img-blog.csdn.net/20141103004932109)



G3(x)在训练数据集上的误差率e3=P(G3(xi)≠yi) = 0.1820

计算G3的系数：

> 
> 
![](https://img-blog.csdn.net/20141103005116515)



更新训练数据的权值分布：

> 
> 
![](https://img-blog.csdn.net/20141103005213566)



D4=(0.125, 0.125, 0.125, 0.102, 0.102, 0.102, 0.065, 0.065, 0.065, 0.125)，f3(x)=0.4236G1(x) + 0.6496G2(x)+0.7514G3(x)，分类器sign(f3(x))在训练数据集上有0个误分类点。

### 2 Adaboost的误差界

通过上面的例子可知，Adaboost在学习的过程中不断减少训练误差e，那这个误差界到底是多少呢？

事实上，adaboost 的训练误差的上界为：

> 
> 
![](https://img-blog.csdn.net/20141103095154746)



下面，咱们来通过推导来证明下上述式子。

当G(xi)≠yi时，yi*f(xi)<0，因而exp(-yi*f(xi))≥1，因此前半部分得证。

关于后半部分，别忘了：

> 
> 
![](https://img-blog.csdn.net/20141103095555766)



整个的推导过程如下：

> 
> 
![](https://img-blog.csdn.net/20141103095712971)



这个结果说明，可以在每一轮选取适当的Gm使得Zm最小，从而使训练误差下降最快。接着，咱们来继续求上述结果的上界。

对于二分类而言，有如下结果：

> 
> 
![](https://img-blog.csdn.net/20141103100722152)



其中，![](https://img-blog.csdn.net/20141103100816846)。

继续证明下这个结论。

由之前Zm的定义式跟本节最开始得到的结论可知：

> 
> 
![](https://img-blog.csdn.net/20141103101042316)



而这个不等式![](https://img-blog.csdn.net/20141103101343953)可先由e^x和1-x的开根号，在点x的泰勒展开式推出。

值得一提的是，如果取γ1, γ2… 的最大值，记做γ（显然，γ≥γi>0，i=1,2,...m），则对于所有m，有：

> 
> 
![](https://img-blog.csdn.net/20141103101631265)



这个结论表明，AdaBoost的训练误差是以指数速率下降的。另外，AdaBoost算法不需要事先知道下界γ，AdaBoost具有自适应性，它能适应弱分类器各自的训练误差率 。

最后，Adaboost 还有另外一种理解，即可以认为其模型是加法模型、损失函数为指数函数、学习算法为前向分步算法的二类分类学习方法，有机会再推导下，然后更新此文。而在此之前，有兴趣的可以参看《统计学习方法》第8.3节或其它相关资料。

### 3 参考文献与推荐阅读
- wikipedia上关于Adaboost的介绍：[http://zh.wikipedia.org/zh-cn/AdaBoost](http://zh.wikipedia.org/zh-cn/AdaBoost)；
- 邹博之决策树与Adaboost PPT：[http://pan.baidu.com/s/1hqePkdY](http://pan.baidu.com/s/1hqePkdY)；
- 《统计学习方法 李航著》第8章；
- 关于adaboost的一些浅见：[http://blog.sina.com.cn/s/blog_6ae183910101chcg.html](http://blog.sina.com.cn/s/blog_6ae183910101chcg.html)；
- A Short Introduction to Boosting：[http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.93.5148&rep=rep1&type=pdf](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.93.5148&rep=rep1&type=pdf)；
- 南大周志华教授做的关于boosting 25年的报告PPT：[http://vdisk.weibo.com/s/FcILTUAi9m111](http://vdisk.weibo.com/s/FcILTUAi9m111)；
- 《数据挖掘十大算法》第7章 Adaboost。

补充：

创建Gm（x）

**4. Decision Trees 决策树模型 **

**(1) Dicision Treedecision tree** 就是一个简单的 *流程图* ，它从 **root** node开始，自顶向下，根据每个decision node对当前输入的决策选择不同的分枝，重复迭代，一直到**leaf** node.

总体来说，决策树学习着眼于从一组无次序、无规则的事例中推理出决策树形式的分类规则 — 从根结点到叶结点的一条路径就对应一条合取规则；整棵决策树就对应一组析取表达式规则.

看我们的gender classifier的决策树：
![](http://img0.tuicool.com/IfEZv2.png!web)

***decision stump***是一棵单点的decision tree, 它基于单个feature对输入进行分类，对每个可能的feature, 它都包含一个leaf，用来指明该feature的class label.

为了建立一个decision stump, 我们必须首先决定用哪一个feature:

最简单的方法是为每个可能的feature都建一个decision stump, 看哪个feature在training set上的accuracy分数高就用哪个（当然还有很多其他方法）;一旦我们选定了feature, 就可以将每个leaf都标为频率最高的label.

Decision Stump的选择算法如下：

>     首先为分类任务选择全局最好的decision stump

    然后在training set上检测每个leaf node的accuracy

         对于accuracy不好的leaf nodes，在该叶结点路径的训练语料库子集上重新训练获得新的decision stump替换原有decision stump

**(2) Entropy and Information Gain 熵与信息获取**
- **information gain** : measures how much more organized the input values become when we divide them up using a given feature
- **entopy** : to measure how disorganized the original set of input values are

熵的定义如下：

    H = Σ l |in| labelsP(l) ×log2P(l) .

熵是信息不确定性的度量，information gain是原熵与重新组织decision stump后的熵的差，其越大，表明我们获得的信息越多

decision tree构建效率也是一个很重要的问题，上面讲的算法可以用动态规划的技术来提升效率

decision tree的优点：易于理解与表达，非常适合多层级的分类结构（如族谱、进化谱）

decision tree的缺点：

    (1) 每个分枝都要分一部分training data出去，最后可能导致到叶结点的时候训练集已经很小了 — 这就非常容易导致overfit问题 — 一种解决方案是当训练集小到一定程度时，就停止分裂；还有一种解决方案是依旧生成整棵决策树，但是对dev-test set上不能提高性能的部分进行剪枝（prune）

    (2) decision tree认为feature的检测之间是有自顶向下的层级关系，而实际情况中不一定是这样，下面的也可能影响上面的，这种情况用decision tree就不好处理

    (3) 另外，某分枝的概率非常低的时候，需要非常大量的数据才能把这个分枝训练好 ﻿﻿

﻿﻿



﻿完整的Adaboost M1,M2, MH等综述

转自：[http://blog.csdn.net/chenbang110/article/details/11557657](http://blog.csdn.net/chenbang110/article/details/11557657)

转自：[http://blog.csdn.net/chenbang110/article/details/11557585](http://blog.csdn.net/chenbang110/article/details/11557585)

[http://blog.csdn.net/chenbang110/article/details/11557527](http://blog.csdn.net/chenbang110/article/details/11557527)







一、Boosting算法的发展历史

　　Boosting算法是一种把若干个分类器整合为一个分类器的方法，在boosting算法产生之前，还出现过两种比较重要的将多个分类器整合为一个分类器的方法，即boostrapping方法和bagging方法。我们先简要介绍一下bootstrapping方法和bagging方法。

　　1）bootstrapping方法的主要过程

　　主要步骤：

　　i)重复地从一个样本集合D中采样n个样本

　　ii)针对每次采样的子样本集，进行统计学习，获得假设Hi

　　iii)将若干个假设进行组合，形成最终的假设Hfinal

　　iv)将最终的假设用于具体的分类任务

　　2）bagging方法的主要过程

　　主要思路：

　　i)训练分类器

　　从整体样本集合中，抽样n* <<em> N个样本 针对抽样的集合训练分类器Ci

　　ii)分类器进行投票，最终的结果是分类器投票的优胜结果

　　但是，上述这两种方法，都只是将分类器进行简单的组合，实际上，并没有发挥出分类器组合的威力来。直到1989年，Yoav Freund与 Robert Schapire提出了一种可行的将弱分类器组合为强分类器的方法。并由此而获得了2003年的哥德尔奖（Godel price）。

　　Schapire还提出了一种早期的boosting算法，其主要过程如下：

　　i)从样本整体集合D中，不放回的随机抽样n1 < n个样本，得到集合 D1

　　训练弱分类器C1

　　ii)从样本整体集合D中，抽取 n2 < n 个样本，其中合并进一半被 C1 分类错误的样本。得到样本集合 D2

　　训练弱分类器C2

　　iii)抽取D样本集合中，C1 和 C2 分类不一致样本，组成D3

　　训练弱分类器C3

　　iv)用三个分类器做投票，得到最后分类结果

　　到了1995年，Freund and schapire提出了现在的adaboost算法，其主要框架可以描述为：

　　i)循环迭代多次

　　更新样本分布

　　寻找当前分布下的最优弱分类器

　　计算弱分类器误差率

　　ii)聚合多次训练的弱分类器

　　在下图中可以看到完整的adaboost算法：

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=image00112953418311302596098.png&type=image/png&width=672&height=493)

图1.1 adaboost算法过程

　　现在，boost算法有了很大的发展，出现了很多的其他boost算法，例如：logitboost算法，gentleboost算法等等。在这次报告中，我们将着重介绍adaboost算法的过程和特性。

### 二、Adaboost算法及分析

　　从图1.1中，我们可以看到adaboost的一个详细的算法过程。Adaboost是一种比较有特点的算法，可以总结如下：

　　1）每次迭代改变的是样本的分布，而不是重复采样（re weight)

　　2）样本分布的改变取决于样本是否被正确分类

　　总是分类正确的样本权值低

　　总是分类错误的样本权值高（通常是边界附近的样本）

　　3）最终的结果是弱分类器的加权组合

　　权值表示该弱分类器的性能

　　简单来说，Adaboost有很多优点:

　　1)adaboost是一种有很高精度的分类器

　　2)可以使用各种方法构建子分类器，adaboost算法提供的是框架

　　3)当使用简单分类器时，计算出的结果是可以理解的。而且弱分类器构造极其简单

　　4)简单，不用做特征筛选

　　5)不用担心overfitting！

　　总之：adaboost是简单，有效。

　　下面我们举一个简单的例子来看看adaboost的实现过程：

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=image00212953420561302596151.png&type=image/png&width=368&height=320)

　　图中，“+”和“-”分别表示两种类别，在这个过程中，我们使用水平或者垂直的直线作为分类器，来进行分类。

　　第一步：

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=image003129534212413025961911302602682.png&type=image/png&width=835&height=310)

　　根据分类的正确率，得到一个新的样本分布D2­，一个子分类器h1

　　其中划圈的样本表示被分错的。在右边的途中，比较大的“+”表示对该样本做了加权。

　　第二步：

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=image00412953421681302596228.png&type=image/png&width=837&height=310)

　　根据分类的正确率，得到一个新的样本分布D3，一个子分类器h2

　　第三步：

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=image005129534221113025962651302602796.png&type=image/png&width=483&height=326)

　　得到一个子分类器h3

　　整合所有子分类器：

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=image00612953422551302596303.png&type=image/png&width=626&height=382)

　　因此可以得到整合的结果，从结果中看，及时简单的分类器，组合起来也能获得很好的分类效果，在例子中所有的。

　　Adaboost算法的某些特性是非常好的，在我们的报告中，主要介绍adaboost的两个特性。一是训练的错误率上界，随着迭代次数的增加，会逐渐下降；二是adaboost算法即使训练次数很多，也不会出现过拟合的问题。

　　下面主要通过证明过程和图表来描述这两个特性：

　　1）错误率上界下降的特性

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=image00712953423301302596371.png&type=image/png&width=581&height=305)

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=image008129534237813025964061302602890.png&type=image/png&width=616&height=444)

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=image009129534242013025964401302602984.png&type=image/png&width=618&height=329)

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=image010129534245513025964741302603045.png&type=image/png&width=486&height=159)

　　从而可以看出，随着迭代次数的增加，实际上错误率上界在下降。

　　2）不会出现过拟合现象

　　通常，过拟合现象指的是下图描述的这种现象，即随着模型训练误差的下降，实际上，模型的泛化误差（测试误差）在上升。横轴表示迭代的次数，纵轴表示训练误差的值。

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=image01112953425481302596508.png&type=image/png&width=411&height=272)

而实际上，并没有观察到adaboost算法出现这样的情况，即当训练误差小到一定程度以后，继续训练，返回误差仍然不会增加。

　　对这种现象的解释，要借助margin的概念，其中margin表示如下：

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=image01212953426081302596556.png&type=image/png&width=534&height=131)

　　通过引入margin的概念，我们可以观察到下图所出现的现象：

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=image01312953426741302596619.png&type=image/png&width=556&height=184)

　　从图上左边的子图可以看到，随着训练次数的增加，test的误差率并没有升高，同时对应着右边的子图可以看到，随着训练次数的增加，margin一直在增加。这就是说，在训练误差下降到一定程度以后，更多的训练，会增加分类器的分类margin，这个过程也能够防止测试误差的上升。

### 三、多分类adaboost

　　在日常任务中，我们通常需要去解决多分类的问题。而前面的介绍中，adaboost算法只能适用于二分类的情况。因此，在这一小节中，我们着重介绍如何将adaboost算法调整到适合处理多分类任务的方法。

　　目前有三种比较常用的将二分类adaboost方法。

　　1、adaboost M1方法

　　主要思路： adaboost组合的若干个弱分类器本身就是多分类的分类器。

　　在训练的时候，样本权重空间的计算方法，仍然为：

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=image01412953427431302596661.png&type=image/png&width=475&height=95)

　　在解码的时候，选择一个最有可能的分类

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=image01512953428081302596698.png&type=image/png&width=337&height=73)

　　2、adaboost MH方法

　　主要思路： 组合的弱分类器仍然是二分类的分类器，将分类label和分类样例组合，生成N个样本，在这个新的样本空间上训练分类器。

　　可以用下图来表示其原理：

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=image01612953428661302596735.png&type=image/png&width=339&height=160)

　　3、对多分类输出进行二进制编码

　　主要思路：对N个label进行二进制编码，例如用m位二进制数表示一个label。然后训练m个二分类分类器，在解码时生成m位的二进制数。从而对应到一个label上。

### 四、总结

　　最后，我们可以总结下adaboost算法的一些实际可以使用的场景：

　　1）用于二分类或多分类的应用场景

　　2）用于做分类任务的baseline

　　无脑化，简单，不会overfitting，不用调分类器

　　3）用于特征选择（feature selection)

　　4）Boosting框架用于对badcase的修正

　　只需要增加新的分类器，不需要变动原有分类器

　　由于adaboost算法是一种实现简单，应用也很简单的算法。Adaboost算法通过组合弱分类器而得到强分类器，同时具有分类错误率上界随着训练增加而稳定下降，不会过拟合等的性质，应该说是一种很适合于在各种分类场景下应用的算法。



众所周知，模式识别的方法可以按照参与识别特征的属性来区分，可以分为两大类：一、使用定量特征（可度量）如物体长度、宽度等，来描述的各种模式，这一类主要是指决策理论，有匹配、统计、神经网络等方法；二、使用定性特征如特征结构、排列顺序等，来描绘的各种模式，这一类主要是指结构判别，主要有串和树的匹配等方法。

模式识别的完整的流程顺序是：传感器——分割组织——特征提取——分类器——后处理。其中分类器的设计部分，可以使用的理论有很多，目前主要包括：基于统计理论的方法（贝叶斯理论）、线性判别函数、神经网络的方法、随机方法（对于复杂的问题）、非度量方法（定性结构特征）

分类器得到的模型不仅要很好拟合输入数据，还要能够正确地预测未知样本的类标号。因此，训练算法的主要目标就是要建立具有很好的泛化能力模型，即建立能够准确地预测未知样本类标号的模型。

通常我们用“方差”和“偏差”来测量学习算法与给定分类问题的“匹配”和“校准”程度。“偏差”度量的是匹配的“准确性”和“质量”：一个高的偏差意味着一个坏的匹配，“方差”度量的是匹配的“精确性”和“特定性”：一个高的方差意味着一个弱的匹配。

研究表明，使用重采样技术可以提高分类器的准确率，而boosting算法就是涉及分类器设计中的重采样技术。其思想内涵在于：从给定的数据集中抽取多个数据子集，使得有可能计算任意统计量的值及其范围。

说道boosting，不得不说Arcing（adaptive reweighting and combining）自适应的权值重置和组合：重新使用和选择数据，以期达到改善分类器性能的目的。最简单的arcing版本就是bagging算法。

**Bagging一个多分类器系统**

bagging算法的基本思想:

给定一个弱学习算法，和一个训练集；单个弱学习算法准确率不高；将该学习算法使用多次，得出预测函数序列，进行投票，最后结果准确率将得到提高。

步骤1：从大小为n的原始数据集D中，分别独立随机的抽取n’个数据（n’），形成自助数据集，将这个过程独立重复多次，直到产生很多独立的自助数据集。

步骤2：每一个自助数据集都被独立的用于训练一个“分量分类器”。

步骤3、最终的分类判决由这些“分量分类器”各自的判决结果投票决定。

Bagging算法是第一个多分类器系统，后面还有（组合分类器系统）。

算法:

For t = 1, 2, …, T Do

从数据集S中取样（放回选样）

训练得到模型Ht

对未知样本X分类时,每个模型Ht都得出一个分类，得票最高的即为未知样本X的分类，也

可通过得票的平均值用于连续值的预测 。



**Bagging和boosting的区别**
||训练集:|预测函数|准确性|使用要求|
|----|----|----|----|----|
|Bagging|随机选择,各轮训练集相互独立|没有权重;可以并行生成|在有些数据集中，boosting会引起退化|要求“不稳定”的分类方法|
|Boosting|各轮训练集并不独立,它的选择与前轮的学习结果有关|有权重;只能顺序生成|在大多数数据集中，boosting的准确性比bagging高|要求“不稳定”的分类方法训练集的小变动能够使得分类模型显著变动|

Bagging是一个纯粹的降低相关度的方法，如果树的节点具有很高的相关性，bagging就会有好的结果。早期的AdaBoost在第二步的时候采用重采样方法，即使某些样本权重增加。这种方法与bagging存在某种关联。它也是Boost的成功之处中降低相关度方面的重要部分。

AdaBoost在第二步中如果使用加权的tree-growing算法，而不是重采样算法,效果会更好。可以使用stumps作为弱分类器

**最初的boosting算法**

1989年Kearns and Valiant研究了PAC学习模型中弱学习算法和强学习算法两者间的等价问题，即任意给定仅仅比随机猜测稍好(准确率大于0.5)的弱学习算法，是否可以被提升为强学习算法？若两者等价，则我们只需寻找一个比随机猜测稍好的弱学习算法，然后将其提升为强学习算法，从而不必费很大力气去直接寻找强学习算法。就此问题，Schapire于1990年首次给出了肯定的答案。他主持这样一个观点：任一弱学习算法可以通过加强提升到一个任意正确率的强学习算法，并通过构造一种多项式级的算法来实现这一加强过程，这就是最初的Boosting算法的原型。

主要思想是，根据已有的训练样本集设计一个分类器，要求其准确率要比平均性能好，然后依次顺序加入多个分量分类器系统，最后形成一个总体分类器。

以一个二类问题举例。

步骤1：从大小为n的原始样本集D中随机选取n1个样本点（不放回），组成样本集D1。根据D1训练出第一个分类器C1。

步骤2：构造第二个样本集D2，它是根据C1最富信息的那些样本点组成的。在最后产生的D2集合中将有一半的样本被C1正确分类，而另一半的样本被C1错误分类。

步骤3：继续构造第三个样本集D3，方法：在D中剩余的样本中选取样本点，并且用C1和C2进行分类，如果C1和C2判决结果不同，那么就把样本加入D3，否则就忽略这个样本。然后用D3训练新分类器C3。

步骤4：用这3个分类器对新样本x进行分类，如果C1和C2的判决结果相同，则表为一类，不同则表为另一类。

Boosting方法有许多不同的变形，其中最流行的一种就是adaboost。这个名词是“adaptive boosting”的缩写。这个方法允许设计者不断的加入新的“弱分类器”，直到达到某个预定的最小错误率。

1995年Freund and Schapire提出AdaBoost算法。

1996年Yoav Freund在Experiments with a New Boosting Algorithm中提出了AdaBoost.M1和AdaBoost.M2两种算法。其中，AdaBoost.M1是我们通常所说的Discrete AdaBoost：而AdaBoost.M2是M1的泛化形式。该文的一个结论是:当弱分类器算法使用简单的分类方法时，boosting的效果明显地统一地比bagging要好。当弱分类器算法使用C4.5时，boosting比bagging较好，但是没有前者的比较来得明显。

**AdaBoost.M1 Discrete AdaBoost：**

初始版本

1.获得一组样本(X)和它的分类(Y)和一个分类器(weaklearn).

2.赋予平均的权值分布D(i)

进入循环:T次

1. 赋予弱分类器权值D(i),使用弱分类器获得样本(X)到分类(Y)上的一个映射.(就是把某个X归到某个Y类中去)

2. 计算这个映射的误差e，e=各个归类错误的样本权值之和.如果e>1/2那么弱分类器训练失败,跳出循环,

训练结束(这在二值检测中是不会发生的,而多值的情况就要看分类器够不够强健了)

3.设beta B = e / ( 1 - e ).用于调整权值.因为e<1/2.因此0
**4. 如果某样本分类正确,该样本的权值就乘以B让权值变小;如果分类错误,就让该样本的权值乘以B^-1或者不变,这样就让分类正确的样本权值降低,分类错误的样本权值升高,加强了对较难分类样本的分类能力5. 权值均衡化循环结束1. 最终的分类器是,当一个X进入时,遍历所有Y,寻找使(h(x)=y的情况下,log(1/B)之和)最大者即是输出分类y**



书上版本

具体算法：

每个样本都赋予一个权重，T次迭代，每次迭代后，对分类错误的样本加大权重，使得下一次的迭代更加关注这些样本。

输入:(X1,Y1), (X2,Y2),…(Xn,Yn)

Xi∈X, Yi∈Y={+1,-1}

初始化权值:D1(i)=1/n

For t=1,…,T

在Dt下训练,

得到弱的假设ht: X->{-1,+1},

错误率:Εt=ΣDt(i) [ht(Xi)≠Yi]

选择αt=1/2 ln ( (1- Εt)/ Εt ),

更改权值:

if ht(Xi)≠Yi , Dt+1(i)=Dt(i)* eαt /Zt

if ht(Xi)=Yi , Dt+1(i)=Dt(i)* e -αt /Zt

输出:H(X)=sign( ∑ αtht(X) )



带图版本（程序版本）：

初始赋予每个样本相等的权重1/N ；

For t = 1, 2, …, T Do

学习得到分类法Ct；

计算该分类法的错误率Et

Et=所有被错误分类的样本的权重和；

βt= Et/（1 - Et）

根据错误率更新样本的权重；

正确分类的样本： Wnew= Wold* βt

错误分类的样本： Wnew= Wold

调整使得权重和为1；

每个分类法Ct的投票价值为log [ 1 / βt ]﻿﻿

**最大错误率问题：**

将γt=1/2-Et ;

Freund and Schapire证明:

最大错误率为:



即训练错误率随γt的增大呈指数级的减小.

最大总误差:



m : 样本个数

d : VC维

T :训练轮数

Pr:对训练集的经验概率

如果T值太大,Boosting会导致过适应（overfit）

《模式分类》386页



**AdaBoost.M2是M1的泛化形式**

.M2的流程是

1.获得一组样本(X)和它的分类(Y)和一个分类器(weaklearn).

2.对于某个样本Xi将它的分类归为一个正确分类Yi和其他不正确分类Yb

3.样本权值进行如下分布首先每个样本分到1/m的权值,然后每个不正确分类分到(1/m)/Yb的个数。也就是说样本权值是分到了每个不正确的分类上

进入循环

1. 求每个样本的权值,即每个样本所有不正确的分类的权值和,再求每个样本错误分类的权值,即不正确分类的权值除以该样本的权值.最后将每个样本的权值归一化

2. 将样本权值和某样本的不正确分类的权值输入到weaklearn,获得弱分类器的输出为各个分类的可能值

3. 计算伪错误率:

4. 更新权值

退出循环

1999年，ROBERT E. SCHAPIRE和YORAM SINGER，于Machine Learning发表论文：

Improved Boosting Algorithms Using Confidence-rated Predictions。提出了更具一般性的AdaBoost形式。提出了自信率以改善AdaBoost的性能。并提出了解决多标签问题的AdaBoost.MH和AdaBoost.MR算法，其中AdaBoost.MH算法的一种形式又被称为Real Boost算法。事实上：Discrete AdaBoost是指，弱分类器的输出值限定在{-1,+1}，和与之相应的权值调整，强分类器生成的AdaBoost算法；Real AdaBoost是指，弱分类器输出一个可能度，该值的范围是整个R，和与之相应的权值调整，强分类器生成的AdaBoost算法。

事实上，Discrete到Real的转变体现了古典集合到模糊集合转变的思想。

至于Gentle AdaBoost。考虑到(AdaBoost对”不像”的正样本权值调整很高,而导致了分类器的效率下降)，而产生的变种算法。它较少地强调难以分类的样本。Rainer Lienhart，Alexander Kuranov，Vadim Pisarevsky在论文Empirical Analysis of Detection Cascades of Boosted Classifiers for Rapid Object Detection中提出在stump弱分类器(即每个弱分类器使用一个特征进行分类)上进行的对比试验中，Gentle的结果明显好于Real和Discrete。

***AdaBoost.MH（real）***

算法的运算流程:

1. 得到一组样本(m个)和样本相应的分类,这个分类是由K个是和否的标签组成.某一个样本可以有多个是标签.

2. 均分权值:1/mk

进入循环:

1. 由弱分类器获得各样本针对各标签的是或否结果(给出离散值或连续值)

2. 获得alpha（t）

3. 调整权值.大概是,弱分类器判断l标签的是或否,若判断正确乘以1,错误乘以-1,再乘以 ,然后…

4. 权值归一化

跳出循环

输出强分类器

Logit和Gentle算法的提出过程大致是这样的

1. 验证Boosting algorithms是一种拟合一个additive logistic regression model(加性的逻辑回归模型)的阶段式估计过程。它有最优一个指数判据,这个判据由第二定理与二项式对数似然判据是等价的。

2. 作者证明Discrete是使用adaptive Newton updates拟合一个additive logistic regression model来最小化Ee^(-yF(x))的过程，其中F(x)=求和fm(x),而fm(x)就是各层分类器的结果。

3. 作者证明Real是使用层级最优的方法来拟合一个additive logistic regression model.

4. 作者说明了为什么要选择Ee^(-yF(x))作为目标:因为大家都用这个

5. 作者证明了当（blabla一个很复杂的公式，贴不出来）时Ee^(-yF(x))最小

6. 作者证明了每次权值更新以后,最近一次训练出的弱分类器错误率为50%.

7. 作者证明了对于最优的F(x),样本的分类乘以权值的和应该为0.

于是作者用80年代兴起的逻辑回归的寻优方法中提炼出了LogitBoost(我终于找到logitBoost的logic了)

**logitBoost**

自适应的牛顿法,拟合加性logistic回归模型

1. 获得样本,(x,y)序列.将分类y*=(y+1)/2

2. 设置初值,F(x)=0,p(xi)=1/2

进入循环

1. 依式计算zi,wi.

2. 通过加权最小二乘的方式拟合函数fm(x).由zi拟合xi,权重为wi

3. 更新F(x),p(x)

退出循环

输出分类器sign[F(x)].

作者提出,logitAdaBoost在每一轮中都使Ee^(-y(F(x)+f(x)))最优,会使训练样本的代表性下降,于是提出了Gentle AdaBoost(牛顿步长法)

**Gentle AdaBoost（matlab版）**

for It = 1 : Max_Iter

nodes = train(WeakLrn, Data, Labels, distr);

for i = 1:length(nodes)

curr_tr = nodes{i};

step_out = calc_output(curr_tr, Data);

s1 = sum( (Labels == 1) .* (step_out) .* distr);

s2 = sum( (Labels == -1) .* (step_out) .* distr);

if(s1 == 0 && s2 == 0)

continue;

end

Alpha = (s1 - s2) / (s1 + s2);%注意alpha的不同 Alpha = 0.5*log((s1 + eps) / (s2+eps));real版

Weights(end+1) = Alpha;

Learners{end+1} = curr_tr;

final_hyp = final_hyp + step_out .* Alpha;

end

Z = sum(abs(Labels .* final_hyp));

if(Z == 0)

Z = 1;

end

distr = exp(- 1 * (Labels .* final_hyp) / Z);

Z = sum(distr);

distr = distr / Z;

end

**ModestAdaBoost**



for i = 1:length(nodes)

curr_tr = nodes{i};

step_out = calc_output(curr_tr, Data);

s1 = sum( (Labels == 1) .* (step_out) .* distr);

s2 = sum( (Labels == -1) .* (step_out) .* distr);

s1_rev = sum( (Labels == 1) .* (step_out) .* rev_distr);

s2_rev = sum( (Labels == -1) .* (step_out) .* rev_distr);

**Alpha = s1 * (1 - s1_rev) - s2 * (1 - s2_rev);**

其中的fm即为alpha

**AdaBoost**算法针对不同的训练集训练同一个基本分类器（弱分类器），然后把这些在不同训练集上得到的分类器集合起来，构成一个更强的最终的分类器（强分类器）。理论证明，只要每个弱分类器分类能力比随机猜测要好，当其个数趋向于无穷个数时，强分类器的错误率将趋向于零。

**AdaBoost**算法中不同的训练集是通过调整每个样本对应的权重实现的。最开始的时候，每个样本对应的权重是相同的，在此样本分布下训练出一个基本分类器h1(x)。对于h1(x)错分的样本，则增加其对应样本的权重；而对于正确分类的样本，则降低其权重。这样可以使得错分的样本突出出来，并得到一个新的样本分布。同时，根据错分的情况赋予h1(x)一个权重，表示该基本分类器的重要程度，错分得越少权重越大。在新的样本分布下，再次对基本分类器进行训练，得到基本分类器h2(x)及其权重。依次类推，经过T次这样的循环，就得到了T个基本分类器，以及T个对应的权重。最后把这T个基本分类器按一定权重累加起来，就得到了最终所期望的强分类器。



**一些改进的算法**

1、级联ad

一种改进的AdaBoost算法——AD_AdaBoost。Viola提出的级联结构的分类器是指一组串行的分类器。在对待识样本进行分类时，只有被前面一级的分类器判决为正的样本才被送入后面的分类器继续处理，反之则被认为是负样本直接输出。最后，只有那些被每一级的分类器都判决为正的样本才作为正样本输出。在级联结构的分类器中，Viola和jones采用Ad aBoost算法来对每一级的分类器进行训练。

2、一种用于不平衡数据分类的改进Ad a B o o s t算法



3、基于双阈值的增强型AdaBoost_快速算法﻿﻿



## 原文：[http://www.cnblogs.com/jcchen1987/p/4581651.html](http://www.cnblogs.com/jcchen1987/p/4581651.html)

## 关于boost算法

　　boost[算法](http://lib.csdn.net/base/31)是基于[PAC学习理论](http://blog.pluskid.org/?p=821)（probably approximately correct）而建立的一套集成学习算法(ensemble learning)。其根本思想在于通过多个简单的弱分类器，构建出准确率很高的强分类器，PAC学习理论证实了这一方法的可行性。下面关于几种Boost算法的比较，是基于文章《Additive Logistic Regression a Statistical View of Boosting》整理的。

## 几种boost算法步骤

　　通常使用最多的应该是离散的Adaboost算法(Discrete AdaBoost),主要因为它的简单却不俗的表现，Discrete Adaboost算法的步骤如下：

![](http://images0.cnblogs.com/blog2015/719951/201506/161957161852886.jpg)

　　可以看出，Discrete AdaBoost的每一个弱分类的输出结果是1或-1，并没有属于某个类的概率，略显粗糙。

如果让每个弱分类器输出样本属于某个类的概率，则可以得到Real AdaBoost算法，其步骤如下：

![](http://images0.cnblogs.com/blog2015/719951/201506/161957486236776.jpg)

　　Real Adaboost每个弱分类器输出样本属于某类的概率后，通过一个对数函数将0-1的概率值映射到实数域，最后的分类器是所有映射函数的和。

将Real Adaboost算法每次迭代的两部合并，直接产生一个映射到实数域的函数，则就成了Gentle AdaBoost， 其算法步骤如下：

![](http://images0.cnblogs.com/blog2015/719951/201506/161957579666954.jpg)

　　Gentle AdaBoost则在每次迭代时，基于最小二乘去做一个加权回归，最后所有回归函数的和作为最终的分类器。

　　LogitBoost算法则和Gentle AdaBoost算法有点相像，不过其每次进行回归拟合的变量z是在不断更新的，Gentle AdaBoost使用的是y。LogitBoost算法步骤如下：

![](http://images0.cnblogs.com/blog2015/719951/201506/161958047012764.jpg)

## 4种boost算法的原理差异

　　上面4中boost算法，其大体结构都是比较相似的，那么是如何推导出每种算法的具体形式的呢？

　　首先是关于损失函数（或代价函数），通常见到比较多的是均方误差和似然函数，而上面的算法中，Discrete AdaBoost、Real AdaBoost和Gentle AdaBoost算法都是采用对数损失函数，具体形式如下：

*J*(*F*)=*E**e* ( −*y**F*(*x*))  J(F)=Ee(−yF(x))



　　其表达的意义实质上与分类错误个数是相同的。

　　而Logit Boost算法则采用最大化对数似然函数来推导的。

　　第二点是具体优化方法，Discrete AdaBoost与Real AdaBoost主要通过类似梯度下降的方法来优化，而Gentle AdaBoost与Logit Boost都是采用类似牛顿迭代的方式优化的。





## 算法的效果差异

　　在前面提到的参考文章中，对几种算法的效果进行了大量比较，大致如下;
- 整体效果而言，效果由好到差的顺序为Logit Boost，Gentle AdaBoost， Real AdaBoost， Discrete AdaBoost
- 若弱分类器采用树桩模型（也就是只要2个叶子节点的决策树），Discrete AdaBoost的结果比其他3种算法结果差了很多，大概是由于系统偏差过大导致的泛化误差较大
- 若弱分类器采用多层的决策树（4或8个叶子节点），Discrete AdaBoost的结果能有较大提升，而其他3种算法则差异不大。

　　平时我们所用的AdaBoost算法大多是Discrete AdaBoost，从这里可以看出Discrete AdaBoost算法模型相对比较简单，需要弱分类器的精确度稍高，因此在具体应用时最好将每个弱分类器的叶子节点控制在4个或8个。

　　关于Boost算法还有很多比较有趣的结论，这里不多讲，可以参考上面的那篇Paper﻿﻿

﻿﻿

﻿﻿

﻿﻿



