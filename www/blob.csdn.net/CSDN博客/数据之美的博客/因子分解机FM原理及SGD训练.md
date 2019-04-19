# 因子分解机FM原理及SGD训练 - 数据之美的博客 - CSDN博客
2017年09月05日 00:31:57[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：519

版权声明：如需转载，请注明出处http://blog.csdn.net/a819825294
目录[(?)](http://blog.csdn.net/a819825294/article/details/51218296#)[[-]](http://blog.csdn.net/a819825294/article/details/51218296#)
- [背景](http://blog.csdn.net/a819825294/article/details/51218296#t0)
- [模型](http://blog.csdn.net/a819825294/article/details/51218296#t1)
- [用途](http://blog.csdn.net/a819825294/article/details/51218296#t2)
- [交叉项系数](http://blog.csdn.net/a819825294/article/details/51218296#t3)
- [SGD求解参数](http://blog.csdn.net/a819825294/article/details/51218296#t4)
## 1.背景
Steffen Rendle于2010年提出Factorization Machines（下面简称FM），并发布开源工具libFM。FM的提出主要对比对象是SVM，与SVM相比，有如下几个优势 
（1）对于输入数据是非常稀疏（比如自动推荐系统），FM可以，而SVM会效果很差，因为训出的SVM模型会面临较高的bias。 
（2）FMs拥有线性的复杂度, 可以通过 primal 来优化而不依赖于像SVM的支持向量机。
## 2.模型
2-way FM(degree = 2)是FM中具有代表性，且比较简单的一种。就以其为例展开介绍。其对输出值是如下建模
![这里写图片描述](https://img-blog.csdn.net/20160422121939486)
其中，![](https://img-blog.csdn.net/20160422120242558)，![](https://img-blog.csdn.net/20160422120354075)，![](https://img-blog.csdn.net/20160422120428935)，n表示特征维度 
![](https://img-blog.csdn.net/20160422120504787)表示两个大小为k的向量![](https://img-blog.csdn.net/20160422120641969)和![](https://img-blog.csdn.net/20160422120654131)的点积![](https://img-blog.csdn.net/20160422120817444)
k是定义factorization维度的超参数，是正整数
因子分解机FM也可以推广到高阶的形式，即将更多互异特征分量之间的相互关系考虑进来。
## 3.用途
（1）回归问题(Regression)：可以采用最小均方误差作为优化的标准（深入理解可以从高斯分布、极大似然估计入手） 
（2）二分类问题(Binary Classification)：利用sigmoid函数。详细原因见 [地址](http://blog.csdn.net/a819825294/article/details/51172466)
（3）排序(Ranking)
## 4.交叉项系数![](https://img-blog.csdn.net/20160422120504787)
（1）示例
样本数据 
![](https://img-blog.csdn.net/20160422125143948)
FM交叉项系数 
![](https://img-blog.csdn.net/20160422125158355)
（2）求解
表面上看FM模型的第3项的计算复杂度为O(kn^2)，但其实可以经过简单的数学处理，计算复杂度降为O(kn)。
数学原理：主要是采用了如公式((a+b+c)2−a2−b2−c2求出交叉项
![](https://img-blog.csdn.net/20160808145837754)
## 5.SGD求解参数
本文利用随机梯度下降SGD进行参数学习，也是一种简单的在线学习方法。
随机梯度下降与梯度下降主要差别在于batch size不一样
注：大家可以根据自己需要定义Loss Function，通过梯度下降得到参数更新的公式
最初的V通过正态分布的形式给出 
所示代码是通过简单的似然估计进行二分类从而进行参数更新 
![](https://img-blog.csdn.net/20160422130332515)
后续会更新利用FTRL训练FM
参考文献 
（1）Steffen Rendle.Factorization Machines 
（2）Steffen Rendle.Factorization Machines with libFM 
（3）[http://tech.meituan.com/deep-understanding-of-ffm-principles-and-practices.html](http://tech.meituan.com/deep-understanding-of-ffm-principles-and-practices.html)
