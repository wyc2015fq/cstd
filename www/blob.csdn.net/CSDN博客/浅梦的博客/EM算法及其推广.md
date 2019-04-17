# EM算法及其推广 - 浅梦的博客 - CSDN博客





2017年08月27日 17:44:49[浅梦s](https://me.csdn.net/u012151283)阅读数：321
个人分类：[机器学习																[聚类和EM算法](https://blog.csdn.net/u012151283/article/category/7132202)](https://blog.csdn.net/u012151283/article/category/6653295)








EM算法是一种迭代算法，用于含有隐变量的概率模型参数的极大似然估计，或极大后验概率估计。 

EM算法的每次迭代由两步组成：E步，求期望；M步，求极大。所以这一算法称为期望极大算法，简称EM算法。

# EM算法的引入

## EM算法

EM算法与初值的选择有关，选择不同的初值可能得到不同的参数估计值。 

一般地，用Y表示观测随机变量的数据，Z表示隐随机变量的数据。Y和Z连在一起称为完全数据，观测数据Y又称为不完全数据。 

EM算法
> 
输入：观测变量数据Y，隐变量数据Z，联合分布$P(Y,Z|\theta)$，条件分布$P(Z|Y,\theta)$； 

  输出：模型参数$\theta$

  （1）选择参数的初始值$\theta^{(0)}$，开始迭代 

  （2）E步：记$\theta^{(i)}$为第i此迭代参数$\theta$的估计值，在第i+1此迭代的E步，计算 


$\begin{align*}Q(\theta,\theta^{(i)})&=E_z[\log P(Y,Z|\theta)|Y,\theta^{(i)}]\\&=\sum\limits_{Z}\log P(Y,Z|\theta)P(Z|Y,\theta^{(i)}) \text{(9.9)} \end{align*}$

   这里，$P(Z|Y,\theta^{(i)})$是在给定观测数据Y和当前的参数估计$\theta^{(i)}$下隐变量数据Z的条件概率分布； 

   （3）M步：求使得$Q(\theta,\theta^{(i)})$极大化的$\theta$，确定第i+1次迭代的参数的估计值$\theta^{(i+1)}$
$\theta^{(i+1)}=\arg\max\limits_{\theta}Q(\theta,\theta^{(i)})$

   （4）重复第(2)步和第(3)步，直到收敛。
式(9.9)的函数$Q(\theta,\theta^{(i)})$是EM算法的核心，称为Q函数(Q function)。 
![这里写图片描述](https://img-blog.csdn.net/20170827173705607?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## EM算法（二）

### 极大似然估计

我们可将对数据建模的方法分为两大类，概率模型和非概率模型 

概率模型： 

1. 贝叶斯分类器 

2. 逻辑回归 

3. 最小二乘法回归和岭回归（使用ML和MAP解释） 

4. 贝叶斯线性回归 

非概率模型： 

1. 感知机 

2. 支持向量机 

3. 决策树 

4. K-means 

在上述每一种方法中，我们都有一个想要优化的目标函数（贪婪或非贪婪，局部或全局）
一种概率的目标函数是极大化（对数）似然函数。对于一些模型，可以找到参数$\theta_{ML}$的极大似然估计值得解析解，然后代入数据求解。 

但是对于更加复杂的模型，可能将参数分为两组$\theta_1,\theta_2$，然后求解关于两个组参数的极大似然估计 
$\theta_{1,ML},\theta_{2,ML}=\arg max_{\theta_1,\theta_2}\sum_{i=1}^{n}\ln{p(x_i|\theta_1,\theta_2)}$

尽管可以在**给定**一个参数的条件下求得另一个参数，但是不能**同时**求解两者。
### 坐标上升

K-means使用的就是一种坐标上升方法。 
![这里写图片描述](https://img-blog.csdn.net/20171204103349003?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 第三种情况

我们想要获得 
$\theta_{1,ML}=\arg max_{\theta_1}\sum_{i=1}^{n}{\ln p(x_i|\theta_1)}$

但是这个函数很难直接进行优化。但是，我们发现我们可以添加第二个变量$\theta_2$使得 
$\sum_{i=1}^{n}\ln p(x_i,\theta_2|\theta_1)  \text{(Function 2)}$

容易处理。 
**注意**- 第二个函数中，$\theta_2$在条件符的左侧，这意味着$\theta_2$上存在着先验。
- 接下来使用EM算法通过**Function 2**求解$\theta_{1,ML}$

### EM算法的目标函数

首先需要定义一个较通用的目标函数使得能够： 

1. 易于优化给定$\theta_1$的关于x的边缘分布$p(x|\theta_1)$

2. 仅出于计算方便的考虑，使用$p(x,\theta_2|\theta_1)$来计算 
**EM目标函数**
$\ln p(x|\theta_1)=\int{q(\theta_2)}\ln{\frac{p(x,\theta_2|\theta_1)}{q(\theta_2)}d\theta_2}+\int{q(\theta_2)}\ln{\frac{q(\theta_2)}{p(\theta_2|x,\theta_1)}d\theta_2}$
**注意**

1. $q(\theta_2)$可以是任意概率分布 

2. 假设我们知道$p(\theta_2|x,\theta_1)$。即，给定数据x和固定的$\theta_1$，我们可以解出关于$\theta_2$的条件后验分布(存在解析解)。
### EM算法的推导

![这里写图片描述](https://img-blog.csdn.net/20171204105545709?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171204105655938?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**注意**- 对于第一项，希望关于完全数据的似然函数的期望是可以计算的（存在解析形式）
- 对于第二项，希望关于辅助变量（隐变量）的条件后验存在解析形式。 
![这里写图片描述](https://img-blog.csdn.net/20171204115303724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

总结- E步相当于利用隐变量的条件后验更新隐变量分布，然后计算完全数据关于隐变量的的期望。
- M步相当于求上述期望关于模型变量的极大。

## EM算法（三）

### 琴声不等式和KL散度

对于凹函数（如log x）有 
$f(\mathbb{E}_{p(t)}t)\ge\mathbb{E}_{p(t)}f(t)$

KL散度用于衡量两个分布之间的差异，不具有对称性，不是一种距离度量。 
$KL(q||p)=\int q(x)\log \frac{q(x)}{p(x)}dx$

具有一下性质 

1. $KL(q||p)\ne KL(p||q)$

2. $KL(q||q)=0$

3. $KL(q||p) \ge 0$
### 推导

设观测变量X受到隐变量T和参数$\theta$影响。 
![这里写图片描述](https://img-blog.csdn.net/20171204132244724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

想要极大化观测变量X关于参数的对数似然函数 
![这里写图片描述](https://img-blog.csdn.net/20171204132405108?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

由于完全变量关于参数的对数似然函数不易直接优化，考虑利用琴声不等式找到一个下届函数进行优化。 
![这里写图片描述](https://img-blog.csdn.net/20171204132549590?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

1. 给定参数$\theta^k$情况下，找到使得$L(\theta^k,q)$最大的$q^{k+1}$
![这里写图片描述](https://img-blog.csdn.net/20171204133405831?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2.给定参数$q^{k+1}$情况下，找到使得$L(\theta,q^{k+1})$最大的$\theta^{k+1}$
![这里写图片描述](https://img-blog.csdn.net/20171204133624385?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### E步，求期望细节

![这里写图片描述](https://img-blog.csdn.net/20171204133931753?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### M步，求极大细节

![这里写图片描述](https://img-blog.csdn.net/20171204134114090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171204134131041?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## EM算法的导出

EM算法是通过不断求解下界的极大化逼近对数似然函数极大化的算法。 

EM算法不能保证找到全局最优值。

## EM算法在非监督学习中的应用

待补充

# EM算法的收敛性

EM算法提供一种近似计算含有隐变量概率模型的极大似然估计方法。 
**问题**

1. EM算法得到的估计序列是否收敛 

2. 若收敛，收敛的全局最大还是局部极大值？
> 
定理9.1 

  设$P(Y|\theta)$是观测数据的似然函数，$\theta^{(i)}(i=1,2,...)$为EM算法得到的参数估计序列，$P(Y|\theta^{(i)})(i=1,2,...)$为对应的似然函数序列，则$P(Y|\theta^{(i)})$是单调递增的，即 
$P(Y|\theta^{(i+1)})\ge P(Y|\theta^{(i)})$
![这里写图片描述](https://img-blog.csdn.net/20171204115342025?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171204134150323?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**单调有界定理**

> 
定理9.2 
![这里写图片描述](https://img-blog.csdn.net/20171206003851727?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# EM算法在高斯混合模型中的应用

请参阅[高斯混合模型GMM](http://blog.csdn.net/u012151283/article/details/77649924)

# EM算法的推广

待补充

# 总结

![这里写图片描述](https://img-blog.csdn.net/20171206004243419?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

参考资料
> 
《统计学习方法》第9章 

   《Machine Learning》ColumbiaX: CSMM.102x Lecture 15 

   《Bayesian Methods for Machine Learning》

























