# Graph embedding and extensions:降维框架~ - 家家的专栏 - CSDN博客





2012年06月07日 18:27:17[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3413








**Reference: Graph embedding  and extensions: a general framework for dimensionality reducation.**

在图嵌入的框架下，提出一种有监督的降维方法：**Marginal Fisher Analysis（MFA）.** 使得类内紧致，类间分类~

这种MFA方法，能有效地克服，由于对**数据分布假设**，导致的传统LDA的限制~



无论在有监督学习或无监督学习中，降维技术应用广泛~

 如线性的方法：PCA、LDA、LPP(Locality Preserving projections保存数据集中的局部信息，利用固有的流行结构~): 

    非线性的方法：ISOMAP、LLE、Laplacian Eigenmap~

    kernel trick+线性降维方法~~



**两个创新点：**

**1、一个框架： graph embedding~** (+ 线性的、非线性的、tensorization)

** 2、在这个框架下，提出新的降维方法~**





