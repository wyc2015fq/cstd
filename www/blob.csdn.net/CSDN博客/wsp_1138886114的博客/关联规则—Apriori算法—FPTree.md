# 关联规则—Apriori算法—FPTree - wsp_1138886114的博客 - CSDN博客





2018年07月19日 13:58:56[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：2815










- - - [1.关联规则](#1关联规则)- - [1.1 概念](#11-概念)
- [1.2 示例](#12-示例)


- [2  关联规则挖掘推论（Apriori 算法）](#2-关联规则挖掘推论apriori-算法)- - [2.1  关联规则挖掘方法：](#21-关联规则挖掘方法)
- [2.3 FP-growth](#23-fp-growth)


- [3 FP-growth原理](#3-fp-growth原理)- - [3.1 生成项头表](#31-生成项头表)
- [3.2 生成FP tree以及节点链表](#32-生成fp-tree以及节点链表)
- [3.3 挖掘过程](#33-挖掘过程)








### 1.关联规则

##### 1.1 概念

```
啤酒与尿布的故事
关联规律挖掘：从交易数据中发现：买了X 还会买Y 的规则
关联规律挖掘‘购物篮分析’Market Basket Analysis（MBA）
关联规律->应用于推荐系统

基本概念：
通过数据的关联性进行分析挖掘，适合用于大数据样本中。
项集：包含0个或多个项的集合。包含k个项就是一个k项集

事务：表示事件
（比如每次去商场购买东西是一次事务，而实际购买到的东西就是项集）

关联规则的三个计算:
    支持度 support
    置信度 confidence
    提升度 lift
```
- 支持度（support）:
全部事务中，项集中{X,Y}同时出现的概率： 


$support(X=>Y)=\frac{\sigma(X U Y)}{N}$

该指标作为建立强关联规则的第一个门槛，通过最小阈值(minsup)的设定，来剔除那些 “出镜率” 较低的无意义的规则.而相应地保留下出现较为频繁的项集所隐含的规则。即筛选出满足： 


$support(Z) >= minSup$

项集Z，被称为频繁项集(Frequent Itemset)。- 置信度（confidence）:
在关联规则的先决条件 X 发生的条件下，关联结果Y发生的概率，  

即含有X的项集条件下，同时含有Y的可能性： 


$confidence(X=>Y) = \frac{\sigma(X U Y)}{\sigma(X)}$
在这里，也要对置信度设置 最小阈值(mincon) 来进一步筛选满足需要的强关联规则。因此，继产生频繁项集后，需从中进而选取满足： 


$confidence(X=>Y) \geq  mincon$
- 提升度（lift）:

表示在含有 X 的条件下同时含有 Y 的可能性与无条件下含有Y的可能性之比。  

即在Y的自身出现的可能性P(Y)的基础上，X的出现对于Y的“出镜率” P(Y/X)的提升程度： 


$lift(X=>Y) = \frac{P(Y|X)}{P(Y) }  == \frac{confidence (X=>Y)}{P(Y)}$

该指标与confidence同样用来衡量规则的可靠性，可以看作置信度的一种互补指标。  
lift >1  X-y 正相关 

lift <1  X-y 负相关 

lift =1  X-y 无相关
Support较大的规则lift值会倾向于比较小 

往往lift值特别大的规则，support会非常小   

##### 1.2 示例
|TID|面包|牛奶|尿布|啤酒|鸡蛋|可乐|
|----|----|----|----|----|----|----|
|1|1|1|0|0|0|0|
|2|1|0|1|1|1|0|
|3|0|1|1|1|0|1|
|4|1|1|1|1|0|0|
|5|1|1|1|0|0|1|

```
考虑规则{牛奶，尿布}->{啤酒}。 
由于项集{牛奶，尿布，啤酒}支持度计数2。事务总数5（购买5次）
所以支持度：2/5=0.4 

由于项集{牛奶，尿布，啤酒}支持度计数2。项集{牛奶，尿布}支持度计数为3 
所以该置信度：2/3=0.67
```

![这里写图片描述](https://img-blog.csdn.net/20180728202700338?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 2  关联规则挖掘推论（Apriori 算法）

![这里写图片描述](https://img-blog.csdn.net/20180719102709766?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180719102721484?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### 2.1  关联规则挖掘方法：
- 分两步走：
频繁项集（Frequent Itemset）的生成：生成所有supoort>= minsup的项集合关联规则的生成：对每一个频繁项集（XY）进行二元划分，生成confidence最高的一系列规则（X->Y）- Apriori 算法思想：（购买A、B、C频繁则分别购买AB、BC、AC、ABC肯定频繁）

频繁项集的子集也是频繁项集

```
优点：使用先验性质，大大提高了频繁项集产生的效率
      算法简单易理解，容易实施

缺点：候选频繁K项集数量巨大
      需要对整个数据库进行扫描，非常耗时
      数据集存放内存中，大数据处理有困难
```

![这里写图片描述](https://img-blog.csdn.net/20180602222141387?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 关联规则应用方向:
交叉销售：向买A的用户推荐B捆绑销售：把A和B打包销售- 基于support，confidence和lift 筛选规则
![这里写图片描述](https://img-blog.csdn.net/20180602222258913?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### 2.3 FP-growth

```
每次由（k-1）项集L(K-1)产生K项集L(K)时，自连接产生采用枚举的方式，而且产生的项集每次都得去扫描一下数据库

如何提高Apriori算法的效率，有比较多的方法:基于散列，抽样等方式，比较出名的改进算法有FPGrowth算法。
```
- 经典的关联规则挖掘算法包括Apriori算法和FP-growth算法。
apriori算法多次扫描交易数据库，每次利用候选频繁集产生频繁集；
FP-growth则利用树形结构，无需产生候选频繁集而是直接得到频繁集，大大减少扫描交易数据库的次数，从而提高了算法的效率。  

但是apriori的算法扩展性较好，可以用于并行计算等领域。

使用Apriori算法进行关联分析。FP-growth算法来高效发现频繁项集。 

sklearn 没有关联规则算法，寻求GitHub 
[https://github.com/asaini/Apriori](https://github.com/asaini/Apriori)

Python Implementation of Apriori Algorithm
### 3 FP-growth原理

##### 3.1 生成项头表

原始数据中共有10个事务。 

       首先根据原始数据并对1项集计数，我们发现F，O，I，L，J，P，M, N都只出现一次，支持度为1，低于2的阈值(此处假设我们设定一个阈值为2)。  

       去除后将剩下的A, C, E, G, B, D, F按照出现次数(即支持度)的大小降序排列，组成了项头表。 （一般使用降序排序）  
![这里写图片描述](https://img-blog.csdn.net/20180807154846500?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### 3.2 生成FP tree以及节点链表

原始数据得到初步的整理后，开始生成FP tree以及节点链表。  

首先把根节点设为null，也就是说根节点不包含任何项。这样做的好处是，任何一个事务都可以被视为是从一个空项开始的。  
![这里写图片描述](https://img-blog.csdn.net/20180807155736324?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180807164733310?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### 3.3 挖掘过程

首先挖掘频率低的项，然后逐次挖掘频率高的项。  

**首先挖掘频率最低的项F**，因为其子树恰好较为简单，所以可以很容易地得到其**条件模式基。**

条件模式基（conditional pattern base）：条件模式基是以所查找项为结尾的路径集合。每一条路径其实都是一条前缀路径。  

简而言之，一条前缀路径是介于所查找元素项与树根节点之间的所有内容。 

对于一颗conditional FP-tree(条件FP树)，可以进行递归的挖掘(合并分支等)。  
![这里写图片描述](https://img-blog.csdn.net/20180807165711423?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**F挖掘完了，我们开始挖掘项D。**

       D比F复杂一些，因为其节点链表含有两个叶子节点。因此首先得到一颗conditional FP-tree 。 

       我们接着将所有的祖先节点计数设置为叶子节点的计数，即变成{A:2, C:2, E:1, G:1, D:1, D:1}。此时E和G由于在条件模式基里面的计数低于阈值2，被我们删除，最终得到条件模式基为{A:2, C:2}。（如右上图）  
![这里写图片描述](https://img-blog.csdn.net/20180807170158399?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)









