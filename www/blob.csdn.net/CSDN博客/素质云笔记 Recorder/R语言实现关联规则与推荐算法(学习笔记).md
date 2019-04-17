# R语言实现关联规则与推荐算法(学习笔记) - 素质云笔记/Recorder... - CSDN博客





2016年02月14日 13:43:39[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：12188标签：[关联规则																[R语言																[R																[代码																[Apriori](https://so.csdn.net/so/search/s.do?q=Apriori&t=blog)
个人分类：[R︱精准营销](https://blog.csdn.net/sinat_26917383/article/category/6093521)













R语言实现关联规则





笔者前言：以前在网上遇到很多很好的关联规则的案例，最近看到一个更好的，于是便学习一下，写个学习笔记。






**推荐算法中**



物品-物品用关联规则；

人物-物品用协同过滤；

人-人用社会网络分析；

特征-物品用预测建模，分类模型。（本总结来自CDA DSC相关课程）










**关联规则和协同过滤算法**




关联规则，将所有用户的高频产品进行推荐，但是如果要清仓，清除一些低频的产品，关联规则不太适用；而协同过滤可以顾及长尾。





————————————————————————————————————————————————————————————




# 一、关联规则数据规则



## 1、数据格式



关联规则需要把源数据的格式转换为稀疏矩阵。

把上表转化为稀疏矩阵，1表示访问，0表示未访问。


|Session ID|News|Finance|Entertainment|Sports|
|----|----|----|----|----|
|1|1|1|0|0|
|2|1|1|0|0|
|3|1|1|0|1|
|4|0|0|0|0|
|5|1|1|0|1|
|6|1|0|1|0|




## 2、关联规则专业术语项集 ItemSet

这是一条关联规则：![](https://img-blog.csdn.net/20150420154100822?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2p3YW5nMTk4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



括号内的Item集合称为项集。如上例，{News, Finance}是一个项集，{Sports}也是一个项集。

这个例子就是一条关联规则：基于历史记录，同时看过News和Finance版块的人很有可能会看Sports版块。



{News,Finance} 是这条规则的Left-hand-side (LHS or Antecedent)

{Sports}是这条规则的Right-hand-side (RHS or Consequent)



**LHS**（Left Hand Side)的项集和**RHS**（Right Hand Side）的项集不能有交集。






# 二、关联规则强度指标

## 1、支持度——商品出现频次



**项集的支持度**就是该项集出现的次数除以总的记录数（交易数）。

Support({News}) = 5/6 = 0.83

Support({News, Finance}) = 4/6 =0.67

Support({Sports}) = 2/6 = 0.33

支持度的意义在于度量项集在整个事务集中出现的频次。我们在发现规则的时候，希望关注频次高的项集。


## 2、置信度——两商品同时发生概率



关联规则 X -> Y 的置信度 计算公式 ![](https://img-blog.csdn.net/20150420154115423?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2p3YW5nMTk4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

规则的置信度的意义在于项集{X，Y}同时出现的次数占项集{X}出现次数的比例。发生X的条件下，又发生Y的概率。

![](https://img-blog.csdn.net/20150420154249529?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2p3YW5nMTk4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





表示50%的人  访问过{News, Finance}，同时也会访问{Sports}


## 3、提升度——两商品独立性



**当右手边的项集（consequent）的支持度已经很显著时，即时规则的Confidence较高，这条规则也是无效的。**

**举个例子：**

> 
*在所分析的10000个事务中,6000个事务包含计算机游戏,7500个包含游戏机游戏,4000个事务同时包含两者。*

*关联规则（计算机游戏，游戏机游戏） 支持度为0.4，看似很高，但其实这个关联规则是一个误导。*

*在用户购买了计算机游戏后有 （4000÷6000）0.667 的概率的去购买游戏机游戏，而在没有任何前提条件时，用户反而有（7500÷10000）0.75的概率去购买游戏机游戏，也就是说设置了购买计算机游戏这样的条件反而会降低用户去购买游戏机游戏的概率，所以计算机游戏和游戏机游戏是相斥的。*




![](https://img-blog.csdn.net/20150420154204408?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2p3YW5nMTk4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

所以要引进Lift这个概念，Lift(X->Y)=Confidence(X->Y)/Support(Y)

规则的提升度的意义在于度量项集{X}和项集{Y}的独立性。即，Lift(X->Y)= 1 表面 {X}，{Y}相互独立。[注：P(XY)=P(X)*P(Y),if X is independent of Y]





如果该值=1,说明两个条件没有任何关联,如果<1,说明A条件(或者说A事件的发生)与B事件是相斥的,一般在数据挖掘中当提升度大于3时,我们才承认挖掘出的关联规则是有价值的。

### ![](https://img-blog.csdn.net/20150420154339278?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2p3YW5nMTk4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 最后，lift(X->Y) = lift(Y->X)


## 4、出错率——规则预测精度






Conviction的意义在于度量规则预测错误的概率。



表示X出现而Y不出现的概率。

![](https://img-blog.csdn.net/20150420154520350?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2p3YW5nMTk4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


例子：

![](https://img-blog.csdn.net/20150420154417944?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2p3YW5nMTk4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




表面这条规则的出错率是32%。





三、关联规则核心算法——Apriori算法






如果项集A是频繁的，那么它的子集都是频繁的。如果项集A是不频繁的，那么所有包括它的父集都是不频繁的。

例子：{X, Y}是频繁的，那么{X}，{Y}也是频繁的。如果{Z}是不频繁的，那么{X,Z}, {Y, Z}, {X, Y, Z}都是不频繁的。




**生成频繁项集**

给定最小支持度Sup，计算出所有大于等于Sup的项集。

第一步，计算出单个item的项集，过滤掉那些不满足最小支持度的项集。

第二步，基于第一步，生成两个item的项集，过滤掉那些不满足最小支持度的项集。

第三步，基于第二步，生成三个item的项集，过滤掉那些不满足最小支持度的项集。




如下例子（频繁项集）：
|**One-Item Sets**|**Support Count**|**Support**|
|----|----|----|
|{News}|5|0.83|
|{Finance}|4|0.67|
|{Entertainment}|1|0.17|
|{Sports}|2|0.33|
|**Two-Item Sets**|**Support Count**|**Support**|
|{News, Finance}|4|0.67|
|{News, Sports}|2|0.33|
|{Finance, Sports}|2|0.33|
|**Three-Item Sets**|**Support Count**|**Support**|
|{News, Finance, Sports}|2|0.33|





# 四、R语言实现关联规则




可参考该博客：http://blog.csdn.net/gjwang1983/article/details/45015203




贴一些实现的图：

![](https://img-blog.csdn.net/20160214133958507)


![](https://img-blog.csdn.net/20160214134002162)


![](https://img-blog.csdn.net/20160214134006100)


________________________________________________________________________________________




# 五、关联规则的推荐案例解读




## 1、支持度、置信度、提升度用法




本总结来自CDA DSC相关课程三个指标，支持度、置信度、提升度的用法。

![](https://img-blog.csdn.net/20160606194929176)


本图的解读，

第一幅上图，买了基金的人还买了黄金的占0.35，所有的人中买了黄金的有18%（基准），此时提升度为0.35/0.18=1.94>1，可以作为推荐；

作为客户要冲销量，则选择面向基数大的部分，则选择支持度、置信度大的，比如第一张图的第三方存管，第二图的结构性理财产品。

**所以三个指标的基本用法：冲销量、KPI会重点关注置信度大的；随机推荐用提升度。**







## 2、网商时代关联规则背弃长尾效应




在实际案例运用过程中关联规则与协同过滤的区别在于，

关联规则推荐的是本来就很热门的产品，因为代表同时发生频率越高，关联性越强。在网商时代会背弃长尾效应，让差异扩大，2/8定律会一定程度上扩充至1/9，助长马太效应。

一般要推荐冷门产品会使用协同过滤。下图就是京东上使用关联规则的例子。

![](https://img-blog.csdn.net/20160606195815820)](https://so.csdn.net/so/search/s.do?q=代码&t=blog)](https://so.csdn.net/so/search/s.do?q=R&t=blog)](https://so.csdn.net/so/search/s.do?q=R语言&t=blog)](https://so.csdn.net/so/search/s.do?q=关联规则&t=blog)




