# 信息熵(entropy) - Keith - CSDN博客





2017年01月09日 11:33:22[ke1th](https://me.csdn.net/u012436149)阅读数：1414








信息熵用来描述信息的不确定性，如果不确定性越高，那么信息熵越大，否则则越低。

## 自信息（信息量）

I(X)称为自信息，$I(x)=-logP(x)$。通过公式可以看出，$P(x)$越大，自信息就越小。当然，如果一件事情发生的概率为1，那么他的自信息就是0.

## 信息熵

假设X的分布为P(X),那么其信息熵为： 


$\begin{aligned}H(X)&=E[I(X)]\\&= \sum_ip(x_i)I(x_i)\\&=-\sum_ip(x_i)logp((x_i))\end{aligned}$

## 联合熵

假设$X,Y$的联合分布为$P(X,Y)$,那么其信息熵为: 


$\begin{aligned}H(X,Y)&=E[I(X,Y)]\\&=\sum_{x,y} p(x,y)I(x,y)\\&=-\sum_{x,y} p(x,y)log(p(x,y))\end{aligned}$

## 条件熵

在信息论中，条件熵描述了在已知第二个随机变量$X$的值的前提下，随机变量$Y$的信息熵还有多少。 

如果$H(Y|X=x)$表示已知$X=x$的情况下，$Y$的信息熵，那么我们有： 


$\begin{aligned}H(Y|X)&=\sum_xp(x)H(Y|X=x) \\&=-\sum_xp(x)\sum_yp(y|x)log(p(y|x)) \\&=-\sum_{x,y}p(x,y)log(p(y|x)) \\&=\sum_{x,y}p(x,y)log(\frac{p(x)}{p(x,y)})\end{aligned}$

条件熵与联合熵的关系 


$H(Y|X)=H(X,Y)-H(X)$
## 互信息

在信息论中，两个随机变量的互信息是变量间相互依赖的量度。 

一般的，连个离散随机变量$X$和$Y$的互信息可以定义为： 


$I(X;Y)=\sum_{x,y}p(x,y)log(\frac{p(x,y)}{p(x)p(y)})$
$I(X;Y)=0$当且仅当$X,Y$互为独立随机变量 

互信息又可以表示为： 


$\begin{aligned}I(X;Y)&=H(X)-H(X|Y) \\&=H(Y)-H(Y|X) \\&=H(X)+H(Y)-H(X,Y) \\&=H(X,Y)-H(X|Y)-H(Y|X)\end{aligned}$
## 交叉熵

上面是对于两个随机变量的，下面介绍对于两个分布的。 
$H,Q$是两个分布函数,则他们的交叉熵为： 


$H(P,Q) = -\int P(x)logQ(x) dx$

交叉熵越小，说明这两个分布越相似
## KL散度

KL散度，也称`相对熵`。用于度量两个概率分布之间的差异，给定两个概率分布$P,Q$，两者之间的KL散度为： 


$\begin{aligned}KL(P||Q)&=\int p(x)log(\frac{p(x)}{q(x)}) dx\\&=-H(P)+H(P,Q)\end{aligned}$
**参考资料**
[https://zh.wikipedia.org/wiki/%E8%81%94%E5%90%88%E7%86%B5](https://zh.wikipedia.org/wiki/%E8%81%94%E5%90%88%E7%86%B5)
[https://zh.wikipedia.org/wiki/%E4%BA%92%E4%BF%A1%E6%81%AF](https://zh.wikipedia.org/wiki/%E4%BA%92%E4%BF%A1%E6%81%AF)
[https://zh.wikipedia.org/wiki/%E7%86%B5_(%E4%BF%A1%E6%81%AF%E8%AE%BA)](https://zh.wikipedia.org/wiki/%E7%86%B5_%28%E4%BF%A1%E6%81%AF%E8%AE%BA%29)






