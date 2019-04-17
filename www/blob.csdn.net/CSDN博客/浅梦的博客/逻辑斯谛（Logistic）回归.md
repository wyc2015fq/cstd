# 逻辑斯谛（Logistic）回归 - 浅梦的博客 - CSDN博客





2017年08月27日 12:51:40[浅梦s](https://me.csdn.net/u012151283)阅读数：558








# Logistic回归模型

## logistic分布

逻辑斯谛分布

> 
设X是连续随机变量，X具有下列分布函数和密度函数 
$F(x)=P(X\le x)=\frac{1}{1+e^{-(x-\mu)/\gamma}}$
$f(x)=F'(x)=\frac{e^{-(x-\mu)/\gamma}}{\gamma(1+e^{-(x-\mu)/\gamma})^2}$

  密度函数和分布函数如图所示 
![这里写图片描述](https://img-blog.csdn.net/20170827115753710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

  分布函数图形是一条S形曲线。该曲线以点$(\mu,\frac{1}{2})$位中心对称，即满足 
$F(-x+\mu)-\frac{1}{2}=F(x+\mu)+\frac{1}{2}$

  曲线在中心附近增长速度较快，两端较慢。 

  形状参数$\gamma$的值越小，曲线在中心附近增长越快。
## 二项逻辑回归模型

二项逻辑回归模型是如下的条件概率分布  


$\begin{align*}P(Y=1|x)&=\frac{exp(w \cdot x)+b}{1+exp(w\cdot x+b)}\text{(6.5)}\\P(Y=0|x)&=\frac{1}{1+exp(w\cdot x+b)}\text{(6.6)}\end{align*}$

一个事件的几率(odds)是指该事件发生的概率与不发生的概率的比值。如果事件发生的概率是p，那么该事件的几率就是$\frac{p}{1-p}$，该事件发生的对数几率(log odds)或logit函数是 
$logit(p)=log\frac{p}{1-p}$

对逻辑斯谛回归而言，由式(6.5)与式(6.6)得 
$\log\frac{P(Y=1|x)}{1-P(Y=1|x)}=w\cdot x$

逻辑回归模型中，输出Y=1的对数几率是输入x的线性函数。
## 模型参数估计

极大似然估计 

设$P(Y=1|x)=\pi(x),P(Y=0|x)=1-\pi(x)$

似然函数为 
$\prod\limits_{i=1}^N[\pi(x_i)]^{y_i}[1-\pi(x_i)]^{1-y_i}$

对数似然函数为 


$\begin{align*}L(w)&=\sum\limits_{i=1}^N[y_i\log\pi(x_i)+(1-y_i)\log(1-\pi(x_i))]\\&=\sum\limits_{i=1}^N[y_i\log\frac{\pi(x_i)}{1-\pi(x_i)}+\log(1-\pi(x_i))]\\&=\sum\limits_{i=1}^N[y_i(w\cdot x_i)-\log(1+exp(w\cdot x_i))]\end{align*}$

对L(w)求极大值，得到w的估计值。 

这样，问题就变成了以对数似然函数为目标函数的最优化问题。 

通常采用**梯度下降（这里是上升，最大化对数似然函数）法及拟牛顿法**。
## 梯度下降法

$\frac{\partial L(w)}{\partial w}=\sum\limits_{i=1}^Nx_i(y_i-\pi(x))  $，其中$\pi(x)=\frac{exp(w \cdot x)}{1+exp(w\cdot x)}$

设学习率为$\alpha$，则**梯度上升法**的更新公式为 
$w_j = w_j + \alpha\sum\limits_{i=1}^Nx_i(y_i-\pi(x))  $
# 多项逻辑斯谛回归

假设离散型随机变量Y的取值集合是${1,2,...,K}$，那么多项逻辑斯谛回归模型是 


$\begin{align*}P(Y=k|x)=\frac{exp(w_k\cdot x)}{1+\sum\limits_{k=1}^{K-1}exp(w_k\cdot x)},k=1,...,K-1\\P(Y=K|x)=\frac{1}{1+\sum\limits_{k=1}^{K-1}exp(w_k\cdot x)}\end{align*}$

# sigmoid函数的推导

## 根据对数几率回归推导

![这里写图片描述](https://img-blog.csdn.net/20170828230808000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 根据最大熵模型推导

[http://blog.csdn.net/u012151283/article/details/77619799#t2](http://blog.csdn.net/u012151283/article/details/77619799#t2)

最大熵原理告诉我们，当在某些约束条件下选择统计模型时，需要尽可能选择满足这些条件的模型中不确定性最大的那个。 

采用熵作为统计不确定性的度量，这个问题变为一个条件约束的问题。 

在最大熵准则下， 


$\begin{align}p^*(x)=\arg_{p(x)} \max H(x)\\s.t. E_p[f_d(x)]=E_{ \widetilde p}[f_d(x)],d=1,...,D\end{align}$
$f_i(x)$为一组特征函数，而优化中约束的意义是这一组特征函数在某型$p(x)$下的均值等于其数上的均值。
使用拉格朗日方法可以得出一项重要结论，求其最大熵解等价于求一个对应指数形式分布的最大似解。

令$\pi(x)_u=P(Y=u|X)$

根据最大熵模型，有 


$\begin{align*}\pi(x)_v\ge0\\\sum\limits_{v=1}^k\pi(x)_v = 1\\\sum\limits_{i=1}^nx(i)_j\pi(x(i))_u=\sum\limits_{i=1}^nf(u,y(i))x(i)_j\text{(for all u,j)}\end{align*}$
## 指数族分布

指数分布族是指可以表示为指数形式的分布。 
$p(y;\eta)=b(y)exp(\eta^TT(y)-\alpha(\eta))$

其中，$\eta$为自然参数，$T(y)$为充分统计量。$\alpha(\eta)$为归一化系数。当参数$b,\alpha,T$都固定时，就定义了一个以$\eta$为参数的函数族。
逻辑回归假设$y$服从伯努利分布， 


$\begin{align}p(y;\phi)&=\phi^y(1-\phi)^{1-y}\\&=exp(y\log{\phi}+(1-y)\log(1-\phi))\\&=exp(\log{(\frac{\phi}{1-\phi})}y+\log(1-\phi))\end{align}$

令$\eta=\log(\frac{\phi}{1-\phi})$，得到$\phi=\frac{1}{1+e^{-\eta}}$。这就是逻辑回归的激活函数。
逻辑回归建模预测$y|x$，并假设$y|x$服从伯努利分布，所以只需知道$p(y|x)$。 

其次需要一个线性模型，即$p(y|x)=f(wx)$。 

然后通过最大熵原则推出f，就是sigmoid函数。
# 为什么用对数损失函数

如果用平方损失函数，平方损失函数关于参数是非凸的。 

对数损失函数是高阶连续可导的凸函数，由凸优化理论可以根据梯度下降法、牛顿法等求最优解。

# 逻辑回归优点

1、它是直接对分类可能性建模，无需事先假设数据分布，这样就避免了假设分布不准确问题。

2、它不仅预测类别，而且可以得到近似概率预测，这对许多概率辅助决策的任务很有用。

3、对率函数是任意阶可导凸函数，有很好的数学性质，现有许多的数值优化算法都可以直接用于求解。

参考资料

> 
《统计学习方法》第6章 

  《计算广告》第10章 
[指数分布族（The Exponential Family)与广义线性回归（Generalized Linear Model GLM）](http://blog.csdn.net/bitcarmanlee/article/details/51292380)
[逻辑回归的目标函数(损失函数)是凸函数吗？有没有最优解？](https://www.zhihu.com/question/64744700)














