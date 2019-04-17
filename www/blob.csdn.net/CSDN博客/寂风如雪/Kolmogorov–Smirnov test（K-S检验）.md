# Kolmogorov–Smirnov test（K-S检验） - 寂风如雪 - CSDN博客





2018年07月10日 20:22:45[寂风如雪](https://me.csdn.net/qq_41679006)阅读数：5692








**主要参考资料**： 

(1)[https://en.wikipedia.org/wiki/Kolmogorov%E2%80%93Smirnov_test](https://en.wikipedia.org/wiki/Kolmogorov%E2%80%93Smirnov_test)

(2)[https://wenku.baidu.com/view/ccfa573a3968011ca30091d6.html](https://wenku.baidu.com/view/ccfa573a3968011ca30091d6.html)
![概述](https://img-blog.csdn.net/20180709201802993?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## Kolmogorov–Smirnov statistic
- **累计分布函数：**
![累积分布函数](https://img-blog.csdn.net/20180709202226108?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中 $I_{[-\inf,x]}$ 为indicator function（指示函数）， 


$I_{[-\inf,x]}(X_i)=\left\{\begin{matrix}1,X_i\leq x;\\0,X_i> x;\end{matrix}\right.$- **Kolmogorov–Smirnov statistic:**

对于一个样本集的累计分布函数$F_n(x)$和一个假设的理论分布$F(x)$,Kolmogorov–Smirnov statistic定义为： 
![Kolmogorov–Smirnov statistic](https://img-blog.csdn.net/20180709203317419?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
$sup_x$是距离的上确界(supremum)， 基于Glivenko–Cantelli theorem，若$X_i$服从理论分布$F(x)$，则当n趋于无穷时$D_n$趋于0。
## Kolmogorov distribution
- **准备知识：**

(1)独立增量过程 

顾名思义，就是指其增量是相互独立的。严格定义如下： 
![独立增量过程](https://img-blog.csdn.net/20180710191026365?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

(2)维纳过程（英语：Wiener process） 

大概可以理解为一种数学化的布朗运动，严格定义如下： 
![维纳过程](https://img-blog.csdn.net/20180710191104817?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

(3)布朗桥（英文：Brownian bridge） 

一种特殊的维纳过程，严格定义如下： 
![布朗桥](https://img-blog.csdn.net/20180710191254527?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

就是说一个在$[0,T]$区间上，且$W_T=0$的维纳过程。 

如图： 
![布朗桥](https://img-blog.csdn.net/20180710191511471?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

红色和绿色的都是“布朗桥”。- **Kolmogorov distribution**

(1)Kolmogorov distribution 

Kolmogorov distribution定义为： 
![Kolmogorov distribution](https://img-blog.csdn.net/20180710191726476?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

即是通过求布朗运动上确界得到的随机变量的分布。 

它的累积分布函数可以写为： 
![累计分布函数](https://img-blog.csdn.net/20180710192059442?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

(2)单样本K-S检验 

单样本K-S检验即是检验样本数据点是否满足某种理论分布。 
注意！若该理论分布的参数是由样本点估计的，该方法无效！

我们从零假设出发。（即假设样本点不满足理论分布） 

此时，若理论分布是一种连续分布，则有： 
![趋于Kolmogorov distribution](https://img-blog.csdn.net/20180710192545811?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

也就是说在有无限多的样本点的时候，不论F的具体形式，$\sqrt{n}D_n$将趋向于一个Kolmogorov distribution。（好像也叫做“依分布收敛”） 

然而事实上，我们既不可能有无穷多样本点，也不是为了证明样本点和完全不满足理论分部。 

K-S检验给出了零假设被拒绝的可能性的一种衡量方法（即样本点满足理论分布的可能性）$\alpha$: 


$\alpha =min([\alpha|\sqrt{n}D_n>K_\alpha])$

其中，$K_\alpha$由以下方式给出： 
![1](https://img-blog.csdn.net/20180710195532187?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以这样定性的理解，样本点越偏离理论分布，它的Kolmogorov–Smirnov statistic就会越大，那么我们找到的$K_\alpha$就越大，$\alpha$就越小，反之亦然。 
**PS：**

wiki上给出的并不是这样，而是： 
![wiki](https://img-blog.csdn.net/20180710195723845?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

但按照我的理解这种提法有些问题。因为我们知道$K_1=0$;而$\sqrt{n}D_n>0$几乎总是成立的。那岂不是对于任何样本点，总有$\alpha=1$？ 


也可能是我的理解有问题，欢迎留言指出。 
- 当理论分布函数非连续时 

这里直接引用wiki上的内容

> 
![wiki](https://img-blog.csdn.net/2018071020214914?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


## 双样本集K-S检验

双样本K-S检验即是检验两个样本集是否满足同样的潜在分布。 

其零假设被否定的可能性仍然以$\alpha$给出： 


$\alpha =min([\alpha|D_{n,m}>c(\alpha)\sqrt{\frac{n+m}{nm}}])，$

其中： 
![1](https://img-blog.csdn.net/20180710201824663?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![2](https://img-blog.csdn.net/20180710201835450?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

PS： 

wiki上的提法与此不同，此处采用此种提法的原因与单样本K-S检验相同。








