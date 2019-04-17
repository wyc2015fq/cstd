# KL散度（相对熵，信息增益）学习笔记 - 蓁蓁尔的博客 - CSDN博客





2016年06月28日 16:11:19[蓁蓁尔](https://me.csdn.net/u013527419)阅读数：6320








## 1.定义

根据查阅相关资料，知相对熵（relative entropy）又称为KL散度（Kullback–Leibler divergence，简称KLD），信息散度（information divergence），信息增益（information gain）。 
**KL散度是两个概率分布P和Q差别的非对称性的度量。**

对一个离散随机变量的两个概率分布P和Q来说，他们的KL散度定义为： 
![这里写图片描述](https://upload.wikimedia.org/math/d/d/a/dda48a164986bf40fc1c74b4bbf3083e.png)
对于连续的随机变量，定义为：  
![这里写图片描述](https://upload.wikimedia.org/math/b/9/4/b9494fe37085e28fb04643bce8600f90.png)。
注： 

 （1）p(x) 和q(x)分别是pq两个随机变量的PDF,Dkl(P||Q)是一个数值，而不是一个函数 

 （2）KL散度并不满足距离的概念，应为：1）KL散度不是对称的；2）KL散度不满足三角不等式。 

 （3）P通常指数据集，我们已有的数据集，Q表示理论结果
## 2.性质

（1）尽管KL散度从直观上是个度量或距离函数，但它并不是一个真正的度量或者距离，因为它不具有对称性，即D(P||Q) ≠ D(Q||P)

（2）相对熵的值为非负值，即Dkl(P||Q)≥0，只有 P(x)=Q(x) 几乎处处成立的时候，Dkl(P||Q)=0. 

证明：利用 Jensen’s Inequality（直接用，证明略），可以得到。 
![吉布斯不等式内容如下：](https://img-blog.csdn.net/20160630091641602)

所以，移项即可得到KL散度的证明。
## 3.理解

从信息论的角度，KL散度是用来度量使用基于Q的编码来编码来自P的样本平均**所需的额外的比特个数**。 典型情况下，P表示数据的真实分布，Q表示数据的理论分布，模型分布，或P的近似分布。

根据shannon的信息论，给定一个字符集的概率分布，我们可以设计一种编码，使得表示该字符集组成的字符串平均需要的比特数最少。假设这个字符集是X，对x∈X，其出现概率为P(x)，那么其最优编码平均需要的比特数等于这个字符集的熵：

　　H(X)=∑x∈XP(x)log[1/P(x)]

在同样的字符集上，假设存在另一个概率分布Q(X)。如果用概率分布P(X)的最优编码（即字符x的编码长度等于log[1/P(x)]），来为符合分布Q(X)的字符编码，那么表示这些字符就会比理想情况多用一些比特数。**KL-divergence就是用来衡量这种情况下平均每个字符多用的比特数，因此可以用来衡量两个分布的距离。**即： 

Dkl(Q||P) 

=∑x∈XQ(x)[log(1/P(x))]-∑x∈XQ(x[log[1/Q(x)]] 

=∑x∈XQ(x)log[Q(x)/P(x)]
## 4.扩展

[浅谈KL散度（相对熵）在用户画像中的应用](http://mp.weixin.qq.com/s?src=3&timestamp=1467103553&ver=1&signature=shSaGeZCGbmwLetTeH27R4633gyhBqFnxbkiws1qLPTyArzX23NDZa78vDjjVPWAAh2kINjgSMebrunBrpMdbK%2aFjWVShfOJDUY4QXgjV2Ui-NCohf45KymEA-e4TIxn87RXEt5PnSgUjg%2am3PdKo1kxHcZaY5pGKZIXCiaBjoQ=)
[KL 散度（从动力系统到推荐系统）](http://chuansong.me/n/2759305)








