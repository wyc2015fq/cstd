# 机器学习方法总结（九）：HMM - qq_38593211的博客 - CSDN博客





2018年08月02日 17:53:15[小屁猴多看书](https://me.csdn.net/qq_38593211)阅读数：86








# HMM(Hidden Markov Model)

> 
   这篇文章会讲解HMM的理论知识，在自然语言分类中会有它用于中文文本分类的实例代码


## 1.基本概念

    隐马尔科夫模型是关于时序的概率模型，描述由一个隐藏的马尔可夫随机生成不可观测的状态随机序列，再由各个状态生成一个观测状态从而产生观测随机序列的过程。举一个小例子，假设有四个盒子，每个盒子（状态序列）中都装有红白两种颜色的球，但它们的数量不一样（即观测概率），如果随机有放回从四个盒子中拿出五个球，但有一个规则：第一次随机一个盒子（初始概率），第二次拿第二个盒子的概率为a（即转移概率）等等，最后得到五个球的为{红红红白白}（观测序列）。求它被广泛用于语音识别、自然语言处理、模式识别中，之后会讲它具体的三种用法。

## 2.算法流程

    设Q是所有可能的状态的集合，V是所有可能的观测的集合：

![Q={q_{1}......q_{N}};V={v_{1}......v_{M}}](https://private.codecogs.com/gif.latex?Q%3D%7Bq_%7B1%7D......q_%7BN%7D%7D%3BV%3D%7Bv_%7B1%7D......v_%7BM%7D%7D)

    I是长度为T的状态序列(对应每次的盒子)，O是对应的观测序列（取出球的情况）：

![I=(i_{1}......i_{T}),O=(o_{1}......o_{T})](https://private.codecogs.com/gif.latex?I%3D%28i_%7B1%7D......i_%7BT%7D%29%2CO%3D%28o_%7B1%7D......o_%7BT%7D%29)

    A是状态转移概率矩阵：![A=[a_{ij}]_{N*N},a_{ij}=P(i_{t+1}=q_{j}|i_{t}=q_{i}),i,j=1......N](https://private.codecogs.com/gif.latex?A%3D%5Ba_%7Bij%7D%5D_%7BN*N%7D%2Ca_%7Bij%7D%3DP%28i_%7Bt&plus;1%7D%3Dq_%7Bj%7D%7Ci_%7Bt%7D%3Dq_%7Bi%7D%29%2Ci%2Cj%3D1......N)

    B是观测概率矩阵：![B=[b_{j}k]_{N*N},b_{j}k=P(o_{t}=v_{k}|i_{t}=q_{j}),j=1......N,k=1......M](https://private.codecogs.com/gif.latex?B%3D%5Bb_%7Bj%7Dk%5D_%7BN*N%7D%2Cb_%7Bj%7Dk%3DP%28o_%7Bt%7D%3Dv_%7Bk%7D%7Ci_%7Bt%7D%3Dq_%7Bj%7D%29%2Cj%3D1......N%2Ck%3D1......M)

    Π是初始状态概率向量：![\pi=[\pi_{i}=P(i_{i}=q_{i})]](https://private.codecogs.com/gif.latex?%5Cpi%3D%5B%5Cpi_%7Bi%7D%3DP%28i_%7Bi%7D%3Dq_%7Bi%7D%29%5D)，是t=1处于状态qi的概率。

    隐马尔科夫模型由初始状态概率向量、状态转移概率矩阵A和观测矩阵B决定，因此模型可由：![\lambda =(A,B,\pi)](https://private.codecogs.com/gif.latex?%5Clambda%20%3D%28A%2CB%2C%5Cpi%29)表示。

  知道了上述基本概念还需要知道隐马尔科夫模型的两个假设：（1）.齐次马尔可夫假设即隐藏的马尔可夫链在任意时刻t的状态只依赖于前一时刻的状态，与其他时刻的状态无关；（2）.观测对立性假设即假设任意时刻的观测只依赖于该时刻的马尔可夫链的状态，与其他观测状态无关。

    算法（观测序列的生成）：

    输入：![\lambda =(A,B,\pi)](https://private.codecogs.com/gif.latex?%5Clambda%20%3D%28A%2CB%2C%5Cpi%29)，观测序列长度T

    输出：O

    (1).按照初始状态分布Π产生状态i1；

    (2).令t=1；

    (3).按状态i1和观测概率产生观测序列

    (4).按状态转移矩阵更新状态it+1；

    (5).t=t+1，直到达到停止条件，否则返回(3)。

## 3.三个问题
- 概率计算问题：给定模型![\lambda =(A,B,\pi)](https://private.codecogs.com/gif.latex?%5Clambda%20%3D%28A%2CB%2C%5Cpi%29)和观测序列O，计算在模型![\lambda =(A,B,\pi)](https://private.codecogs.com/gif.latex?%5Clambda%20%3D%28A%2CB%2C%5Cpi%29)下观测序列O出现的概率。
- 学习问题：已知观测序列O，估计模型![\lambda =(A,B,\pi)](https://private.codecogs.com/gif.latex?%5Clambda%20%3D%28A%2CB%2C%5Cpi%29)的参数使得出现O的概率最大，明显应最大似然估计。
- 预测问题：也就是我们提到的求隐变量的解码问题，已知![\lambda =(A,B,\pi)](https://private.codecogs.com/gif.latex?%5Clambda%20%3D%28A%2CB%2C%5Cpi%29)和O，求最大概率的状态概率I。

## 4.维特比算法

    维特比算法实际是用动态规划解隐马尔科夫模型预测问题（概率最大路径），首先导入两个变量![\delta](https://private.codecogs.com/gif.latex?%5Cdelta).用于定义时刻t状态为i的所有单个路径![(i_{1}......i_{t})](https://private.codecogs.com/gif.latex?%28i_%7B1%7D......i_%7Bt%7D%29)中概率最大值：

![\delta _{t}(i)=max_{i_{1}...i_{t-1}}P(i_{t}=i,i_{1}...i_{t-1},o_{1}...o_{t}|\lambda )](https://private.codecogs.com/gif.latex?%5Cdelta%20_%7Bt%7D%28i%29%3Dmax_%7Bi_%7B1%7D...i_%7Bt-1%7D%7DP%28i_%7Bt%7D%3Di%2Ci_%7B1%7D...i_%7Bt-1%7D%2Co_%7B1%7D...o_%7Bt%7D%7C%5Clambda%20%29)

![\varphi_{t}(i)](https://private.codecogs.com/gif.latex?%5Cvarphi_%7Bt%7D%28i%29)用于定义这个路径的前一个节点。

    算法流程：

                   输入：模型![\lambda =(A,B,\pi)](https://private.codecogs.com/gif.latex?%5Clambda%20%3D%28A%2CB%2C%5Cpi%29)和![O=(o_{1}......o_{T})](https://private.codecogs.com/gif.latex?O%3D%28o_%7B1%7D......o_%7BT%7D%29)；

                   输出：最优路径![I=(i_{1}......i_{T})](https://private.codecogs.com/gif.latex?I%3D%28i_%7B1%7D......i_%7BT%7D%29)

                   (1).初始化：

![\delta _{1}(i)=\pi_{i}b_{i}(o_{1})](https://private.codecogs.com/gif.latex?%5Cdelta%20_%7B1%7D%28i%29%3D%5Cpi_%7Bi%7Db_%7Bi%7D%28o_%7B1%7D%29)

![\varphi_{1}(i)=0](https://private.codecogs.com/gif.latex?%5Cvarphi_%7B1%7D%28i%29%3D0)

                   (2).递推：

![\delta _{t}(i)=max_{1\leq j\leq N}[\delta _{t-1}(j)a_{ji}]b_{i}(o_{t})](https://private.codecogs.com/gif.latex?%5Cdelta%20_%7Bt%7D%28i%29%3Dmax_%7B1%5Cleq%20j%5Cleq%20N%7D%5B%5Cdelta%20_%7Bt-1%7D%28j%29a_%7Bji%7D%5Db_%7Bi%7D%28o_%7Bt%7D%29)

![\psi _{t}(i)=argmax_{1\leq j\leq N}[\delta _{t-1}(j)a_{ji}]](https://private.codecogs.com/gif.latex?%5Cpsi%20_%7Bt%7D%28i%29%3Dargmax_%7B1%5Cleq%20j%5Cleq%20N%7D%5B%5Cdelta%20_%7Bt-1%7D%28j%29a_%7Bji%7D%5D)

                   (3).当t=T时就可以进行回溯找到序列I了。

## 5.缺点

    它的两个缺点就是它需要满足两个假设条件。

## 6.延申MEMM:最大熵模型

    HMM是一个生成模型而最大熵模型是判别模型，HMM需要准确地计算出观测序列X和隐藏状态序列Y的联合概率，要求各时刻独立，如果对于某一个观测值，它可能并非由一个隐藏状态决定的，而是两个以上的隐藏状态综合作用而产生的，那么这时HMM就无能为力了。MEMM对于给定的观测序列X，计算出各隐藏状态序列Y的条件概率分布，这种情况下，就不需要假设观测序列中各时刻的取值相互独立也能算出概率值。

![](https://images2015.cnblogs.com/blog/399159/201612/399159-20161220111525011-885271482.png)

    最大熵模型假设分类模型是一个条件概率分布P(Y|X),X为特征，Y为输出，给定一个训练集![(x^{(1)},y^{(1)})......(x^{(m)},y^{(m)})](https://private.codecogs.com/gif.latex?%28x%5E%7B%281%29%7D%2Cy%5E%7B%281%29%7D%29......%28x%5E%7B%28m%29%7D%2Cy%5E%7B%28m%29%7D%29)其中x为n维特征向量，y为类别输出,我们的目标就是用最大熵模型选择一个最好的分类类型。在给定训练集的情况下，我们可以得到总体联合分布P(X,Y)的经验分布![\widehat{P}(X,Y)](https://private.codecogs.com/gif.latex?%5Cwidehat%7BP%7D%28X%2CY%29),和边缘分布P(X)的经验分布![\widehat{P}(X)](https://private.codecogs.com/gif.latex?%5Cwidehat%7BP%7D%28X%29)。![\widehat{P}(X,Y)](https://private.codecogs.com/gif.latex?%5Cwidehat%7BP%7D%28X%2CY%29)即为训练集中X,Y同时出现的次数除以样本总数m，![\widehat{P}(X)](https://private.codecogs.com/gif.latex?%5Cwidehat%7BP%7D%28X%29)即为训练集中X出现的次数除以样本总数m。用特征函数f(x,y)f(x,y)描述输入x和输出y之间的关系。定义为：

![f(x,y)=1](https://private.codecogs.com/gif.latex?f%28x%2Cy%29%3D1)x和y满足某种关系否则![f(x,y)=0](https://private.codecogs.com/gif.latex?f%28x%2Cy%29%3D0)

    可以认为只要训练集中存在![(x,y)](https://private.codecogs.com/gif.latex?%28x%2Cy%29)则![f(x,y)=1](https://private.codecogs.com/gif.latex?f%28x%2Cy%29%3D1)，特征函数关于经验分布的期望值，用![E_{\widehat{P}(f)}](https://private.codecogs.com/gif.latex?E_%7B%5Cwidehat%7BP%7D%28f%29%7D)表示为:　

![E_{\widehat{P}(f)}=\sum](https://private.codecogs.com/gif.latex?E_%7B%5Cwidehat%7BP%7D%28f%29%7D%3D%5Csum)![\widehat{P}(X,Y)](https://private.codecogs.com/gif.latex?%5Cwidehat%7BP%7D%28X%2CY%29)![f(x,y)](https://private.codecogs.com/gif.latex?f%28x%2Cy%29)

    特征函数f(x,y)关于条件分布P(Y|X)和经验分布![\widehat{P}(X)](https://private.codecogs.com/gif.latex?%5Cwidehat%7BP%7D%28X%29)的期望值，用![E_{P(f)}](https://private.codecogs.com/gif.latex?E_%7BP%28f%29%7D)表示为:

![E_{P(f)}=\sum \widehat{P}(x)P(y|x)f(x,y)](https://private.codecogs.com/gif.latex?E_%7BP%28f%29%7D%3D%5Csum%20%5Cwidehat%7BP%7D%28x%29P%28y%7Cx%29f%28x%2Cy%29)

   如果模型可以从训练集中学习，我们就可以假设这两个期望相等。

![H(P)=-\sum \widehat{P}(x)P(y|x)logP(y|x)](https://private.codecogs.com/gif.latex?H%28P%29%3D-%5Csum%20%5Cwidehat%7BP%7D%28x%29P%28y%7Cx%29logP%28y%7Cx%29)

    我们的目标是得到使H(P)最大的时候对应的P(y|x),这里可以对H(P)加了个负号求极小值，这样做的目的是为了使−H(P)−H(P)为凸函数，方便使用凸优化的方法来求极值。

    对于损失函数求解，过程略去，结果变成：

![P_{w}(y|x)=\frac{1}{Z_{w}(x)}exp(\sum_{i=1}^{M}w_{i}f_{i}(x,y))](https://private.codecogs.com/gif.latex?P_%7Bw%7D%28y%7Cx%29%3D%5Cfrac%7B1%7D%7BZ_%7Bw%7D%28x%29%7Dexp%28%5Csum_%7Bi%3D1%7D%5E%7BM%7Dw_%7Bi%7Df_%7Bi%7D%28x%2Cy%29%29)

    其中Zw(x)为规范因子，定义为：

![Z_{w}(x)=\sum _yexp(\sum_{i=1}^{M}w_{i}f_{i}(x,y))](https://private.codecogs.com/gif.latex?Z_%7Bw%7D%28x%29%3D%5Csum%20_yexp%28%5Csum_%7Bi%3D1%7D%5E%7BM%7Dw_%7Bi%7Df_%7Bi%7D%28x%2Cy%29%29)

    放到MEMM中x就是O，y就是I，MEMM最大熵模型为：

![P(i|o',o)=\frac{exp(\sum _{a}\lambda _{a}f_{a}(o,s))}{Z(o,s')}](https://private.codecogs.com/gif.latex?P%28i%7Co%27%2Co%29%3D%5Cfrac%7Bexp%28%5Csum%20_%7Ba%7D%5Clambda%20_%7Ba%7Df_%7Ba%7D%28o%2Cs%29%29%7D%7BZ%28o%2Cs%27%29%7D)

    递推式变为：

![\delta _{t+1}(s)=max_{s'}\delta _{t}(s')P(s|s',o_{t+1})](https://private.codecogs.com/gif.latex?%5Cdelta%20_%7Bt&plus;1%7D%28s%29%3Dmax_%7Bs%27%7D%5Cdelta%20_%7Bt%7D%28s%27%29P%28s%7Cs%27%2Co_%7Bt&plus;1%7D%29)

    缺点：MEMM存在着标注偏置问题，MEMM所做的是本地归一化，导致有更少转移的状态拥有的转移概率普遍偏高，概率最大路径更容易出现转移少的状态。





