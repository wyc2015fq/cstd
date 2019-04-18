# Logistic Regression逻辑回归的损失函数与梯度下降训练 - 小平子的专栏 - CSDN博客





2019年02月24日 23:14:30[阿拉丁吃米粉](https://me.csdn.net/jinping_shi)阅读数：180








有一篇博文提到logistic regression的简单理解（[Logistic Regression逻辑回归的简单解释](https://blog.csdn.net/jinping_shi/article/details/52326980)）。逻辑回归实际上是odds取对数后的反函数，其函数形式也称为sigmoid function，sigmoid的原义为『像S的形状』。文中最后给出了逻辑回归的表达式：

$$h(\alpha) = \frac{1}{1+e^{-\alpha}}$$

因为$h(\alpha)$的定义域是全体实数$\boldsymbol{R}$，$\alpha$可以用任意函数来代替，一般使用线性函数：$\alpha(\boldsymbol{\theta}, \boldsymbol{x}) = \theta_0 x_0 + \theta_1 x_1 + ... + \theta_n x_n$。

为了方便叙述，改写一下符号：

$$(1)h_{\boldsymbol{\theta}}(\boldsymbol{x}) = g(\boldsymbol{\theta}^T \boldsymbol{x}) = \frac{1}{1+e^{-\boldsymbol{\theta}^T \boldsymbol{x}}} \tag{1}$$

其中粗体$\boldsymbol{x}$是已知的数据（样本，输入），粗体$\boldsymbol{\theta}^T$是参数。机器学习即通过已知样本$\boldsymbol{x}$和对应的label $\boldsymbol{y}$来求解（估计）参数$\boldsymbol{\theta}^T$。

# 损失函数

逻辑回归使用对数损失函数：

$$L(Y, P(Y|X)) = -\log(P(Y|X))$$

对数损失函数直接看函数形式不是很直观，毕竟其它损失函数都可以直接从形式上看出『预测时与真实值之间的差异』这样的含义（绝对值损失，平方差损失等），对数损失函数第一眼难以看到这样的含义。

实际上对数损失函数来源于极大似然估计。$P(Y|X)$的意思是在样本已知（即$X$）的情况下，分类正确（类别为$Y$）的概率。$P(Y|X)$越大表示被正确分类的概率越大，取对数再取反那就是最小，符合损失函数的定义。通俗来讲，一堆参数在一堆数据下的似然值，就是每一条数据（每一条样本）在这一组参数下的条件概率之积，取个对数变成条件概率之和，再取个负号求反就得到了对数损失函数。

逻辑回归的损失函数形式也完全可以从极大似然估计的过程中得到，二者是等价的。

逻辑回归本身用于处理二分类问题。对数损失函数中的$h(Y|X)$就是sigmoid函数$h_\theta(x)$。为了综合$Y=1$和$Y=0$两个类别的损失，基于对数损失函数，逻辑回归的损失函数可以写成如下形式：

$$L(h_{\theta}(x), y)= \left\{\begin{aligned}& -\log(h_\theta(x))   & {y = 1}\\& -\log(1-h_\theta(x)) & {y = 0}\\\end{aligned}\right.$$

为了方便，上式可以改写成一个式子：

$$(2)L(h_{\theta}(x), y) = -y \log(h_\theta (x)) - (1-y)\log(1-h_\theta (x)) \tag{2}$$

(2)式就是逻辑回归的损失函数的形式。如果$y=1$，loss会计算上式中的前半部分，如果$y = 0$，loss计算上式的后半部分。无论0或1，只要分类错误，loss都会变得很大。

下面推导逻辑回归的极大似然估计，最终可以看到，极大似然估计与(2)式是等价的。

前面提到，$P(Y|X) = h_\theta (x)$。逻辑回归的假设是伯努利分布，label有$y=0$和$y=1$两种情况。伯努利分布的概率密度函数$f(x) = p^x(1-p)^{(1-x)}$，套用至逻辑回归，就得到逻辑回归的概率密度函数：

$$f(x) = P(y=1|x)^y \left(1-P(y=1|x)\right)^{(1-y)}$$

其中$P(y=1|x) = h_\theta (x)$

那么似然函数可以写成连乘的形式：

$$L(\boldsymbol\theta) = \prod_{i=1}^m P(y=1|x_i)^{y_i} \left(1-P(y=1|x_i)\right)^{(1-y_i)}$$

两边取以e为底的对数，得到对数似然函数：

$$(3)\begin{aligned}\ln L(\boldsymbol{\theta}) &= \sum_{i=1}^m y_i \ln P(y=1|x_i) + (1-y_i) \ln \left(1-P(y=1|x_i)\right) \\&= \sum_{i=1}^m y_i \ln h_{\boldsymbol{\theta}}(x_i) + (1-y_i) \ln \left(1-h_{\boldsymbol{\theta}}(x_i)\right)\end{aligned} \tag{3}$$

（3)式的形式和(2)式是等价的，只不过对数上一个取log一个取ln。因此才会有逻辑回归的损失函数是从极大似然估计推导而来的说法。

# 梯度下降法训练

梯度下降法训练需要先将损失函数对参数求导，得到参数的迭代公式。

## 损失函数求导

逻辑回归的sigmoid函数有很多特点，这些特点在数学上给我们处理数据带来许多方便。下面给出sigmoid函数的求导过程，在对损失函数求导时会用到以下结论。

令$g(z) = \frac{1}{1+e^{-z}}$，

$$(4)\begin{aligned}g'(z) &= \frac{\partial}{\partial z} \frac{1}{1+e^{-z}} \\&= -\frac{1}{(1+e^{-z})^2} \frac{\partial}{\partial z} e^{-z} \\&= \frac{e^{-z}}{(1+e^{-z})^2} \\&= \left(1-\frac{1}{1+e^{-z}}\right) \frac{1}{1+e^{-z}} \\& = g(z)(1-g(z))\end{aligned} \tag{4}$$

(2)式的逻辑回归损失函数对参数$\theta$求导。求导时，我们先对单个样本的第$j$个参数$\theta_j$求导（即第j维特征），并且将(2)式中的对数取以e为底的对数。

$$(5)\begin{aligned}\frac{\partial}{\partial \theta_j} L(\boldsymbol{\theta}) &= -y \frac{1}{g(\boldsymbol{\theta^T x})} \frac{\partial}{\partial \theta_j} g(\boldsymbol{\theta^T x}) + (1-y) \frac{1}{1-g(\boldsymbol{\theta^T x})} \frac{\partial}{\partial \theta_j} g(\boldsymbol{\theta^T x}) \\&= \left[(1-y) \frac{1}{1-g(\boldsymbol{\theta^T x})} - y \frac{1}{g(\boldsymbol{\theta^T x})} \right] \frac{\partial}{\partial \theta_j} g(\boldsymbol{\theta^T x}) \\&= \left[(1-y) \frac{1}{1-g(\boldsymbol{\theta^T x})} - y \frac{1}{g(\boldsymbol{\theta^T x})} \right] g(\boldsymbol{\theta^T x}) (1-g(\boldsymbol{\theta^T x})) \frac{\partial}{\partial \theta_j} \boldsymbol{\theta^T x} \\&= \left[(1-y) \frac{1}{1-g(\boldsymbol{\theta^T x})} - y \frac{1}{g(\boldsymbol{\theta^T x})} \right] g(\boldsymbol{\theta^T x}) (1-g(\boldsymbol{\theta^T x})) x_j \\&= (g(\boldsymbol{\theta^T x}) - y) x_j\end{aligned} \tag{5}$$

上式即为第j个参数的偏导数。求出所有n个参数的偏导写成向量形式便得到了梯度的表达式。

在更新第j个参数时，我们是沿梯度下降最快的方向，所以(5)式会添加负号，同时为了控制更新的幅度（步长），会在前面乘以一个系数$\alpha$，即学习率，此时得到**单个样本**的参数$\theta_j$的更新迭代表达式：

$$(6)\theta_j := \theta_j - \alpha (g(\boldsymbol{\theta^T x}) - y) x_j \tag{6}$$

当使用所有样本时，如果样本数量为m，就将所有样本的更新求和，更新迭代表达式如下：

$$(7)\theta_j := \theta_j - \alpha \sum_{i=1}^m (g(\boldsymbol{\theta^T x}^{(i)}) - y^{(i)}) x_j^{(i)}\tag{7}$$

## 参数迭代表达式的向量化表示

(7)式就是某个参数的迭代表达式，如果完全按照(7)式的形式写程序，更新一个参数需要循环m次，但是如果写成矩阵的形式，写程序时用矩阵的数据结构，计算就会方便很多。

先对样本空间矩阵化，假设有$m$个样本，$n+1$维特征，用大写的粗体$\boldsymbol{X}$来表示样本。上标表示第几个样本，下标表示第几维特征。

$$(8)\boldsymbol{X} =  \left[ \begin{matrix}   \boldsymbol{x}^{(1)}  \\   \boldsymbol{x}^{(2)}  \\   \vdots \\   \boldsymbol{x}^{(m)}  \end{matrix}  \right] =   \left[ \begin{matrix}   x_0^{(1)} & x_1^{(1)} & \cdots &  x_n^{(1)}  \\   x_0^{(2)} & x_1^{(2)} & \cdots &  x_n^{(2)}  \\   \vdots \\   x_0^{(m)} & x_1^{(m)} & \cdots &  x_n^{(m)}  \end{matrix}  \right]  \tag{8}$$

$$(9)\boldsymbol{y} = \left[\begin{matrix}y^{(1)} \\\vdots \\y^{(m)}\end{matrix}\right] \tag{9}$$

参数$\boldsymbol{\theta}$的矩阵化表示如下：

$$(10)\boldsymbol{\theta}^T = \left[\begin{matrix}\theta_0 \\\vdots \\\theta_n\end{matrix}\right] \tag{10}$$

令$\boldsymbol{A} = \boldsymbol{\theta}^T \boldsymbol{X}$：

$$(11)\begin{aligned}\boldsymbol{A} &= \boldsymbol{\theta}^T \boldsymbol{X} \\&= \left[\begin{matrix}\theta_0 \\\vdots \\\theta_n\end{matrix}\right] \cdot \left[ \begin{matrix}   x_0^{(1)} & x_1^{(1)} & \cdots &  x_n^{(1)}  \\   x_0^{(2)} & x_1^{(2)} & \cdots &  x_n^{(2)}  \\   \vdots \\   x_0^{(m)} & x_1^{(m)} & \cdots &  x_n^{(m)}  \end{matrix}  \right] \\  &= \left[ \begin{matrix}   \theta_0 x_0^{(1)} & \theta_1 x_1^{(1)} & \cdots &  \theta_n x_n^{(1)}  \\   \theta_0 x_0^{(2)} & \theta_1 x_1^{(2)} & \cdots & \theta_n x_n^{(2)}  \\   \vdots \\   \theta_0 x_0^{(m)} & \theta_1 x_1^{(m)} & \cdots &  \theta_n x_n^{(m)}  \end{matrix}  \right]  \end{aligned} \tag{11}$$

令$\boldsymbol{E} = g(\boldsymbol{\theta^T \boldsymbol{X}}) - \boldsymbol{y}$：

$$(12)\boldsymbol{E} = g(\boldsymbol{\theta^T \boldsymbol{X}}) - \boldsymbol{y}=\left[\begin{matrix}g(\boldsymbol{A}^{(1)})-y^{(1)} \\g(\boldsymbol{A}^{(2)})-y^{(2)} \\\vdots \\g(\boldsymbol{A}^{(m)})-y^{(m)}\end{matrix}\right]=\left[\begin{matrix}e^{(1)} \\e^{(2)} \\\vdots \\e^{(m)}\end{matrix}\right] \tag{12}$$

将(11)~(12)式带入(7)式：

$$\begin{aligned}\theta_j &:= \theta_j - \alpha \sum_{i=1}^m (g(\boldsymbol{\theta^T x}^{(i)}) - y^{(i)}) x_j^{(i)} \\&:= \theta_j - \alpha \sum_{i=1}^m e^{(i)} x_j^{(i)} \\&:= \theta_j - \alpha \left(x_j^{(0)}, x_j^{(1)}, \cdots, x_j^{(m)}\right) \cdot \left(e^{(0)}, e^{(1)}, \cdots, e^{(m)}\right)^T \\&:= \theta_j - \alpha \left(x_j^{(0)}, x_j^{(1)}, \cdots, x_j^{(m)}\right) \boldsymbol{E}\end{aligned}$$

对所有参数$\boldsymbol{\theta}$，按照上式的形式，结合(8)~(12)式，可以写成矩阵的形式：

$$(13)\boldsymbol{\theta} := \boldsymbol{\theta} - \alpha \boldsymbol{X}^T \boldsymbol{E} \tag{13}$$

(13)式中，$\boldsymbol{X}^T$是$n+1$行$m$列，$\boldsymbol{E}$是$m$行1列，相乘得到的结果是$n+1$行1列，与$\boldsymbol{\theta}$的维度相同，perfect!

## 批量梯度下降（BGD，Batch Gradient Descent）

批量梯度下降指的是更新参数时，计算所有样本，步骤如下：
- 
求出当前迭代位置参数的梯度

(5)式求的是第j个参数的偏导，参数的梯度就是：
$$\left(\frac{\partial}{\partial \theta_0} J(\boldsymbol{\theta}), \frac{\partial}{\partial \theta_1} J(\boldsymbol{\theta}), \cdots, \frac{\partial}{\partial \theta_n} J(\boldsymbol{\theta})\right)$$- 
确定对所有的$\theta_j$，沿梯度下降的距离是否都小于预先确定的阈值$\epsilon$，如果小于$\epsilon$，算法停止，否则，进入第3步

- 
根据(7)式，使用所有样本（$m$个样本），更新所有参数，需循环m次。如果使用(13)式，就不需要循环，效率会高一点。所有参数更新完成后，返回第1步，开始下一轮迭代


伪代码如下：

```python
epsilon = 0.001 // 新旧梯度的距离，迭代停止条件之一
max_iter = 1000 // 最大迭代次数，迭代停止条件之一
distance = 100
iter_idx = 0
alpha = 0.001 // 学习率
theta = zeros() // 初始化参数
theta_old = theta
m, n = init(X_train, y_train, theta_old) // 初始化各参数,并返回样本个数m和参数个数n
while distance > epsilon or iter_idx < max_iter:
	'''
	根据(7)式循环所有样本更新参数
	'''
	for i in range(m):
		theta += update(theta_old)
	distance = g_distance(theta, theta_old)
	iter_idx += 1
	theta_old = theta
```

## 随机梯度下降（SGD，Stochastic Gradient Descent）

上述BGD中，使用了所有样本计算更新参数。实际应用中，样本数量往往非常大，如点击率预测，样本可以达到千万数量级。这种情况下BGD有个明显问题就是计算速度比较慢。相对于BGD使用所有样本参与计算，SGD每次只用一个样本来计算。每次迭代使用一个样本，可能会出现样本还没有使用完就已经收敛的情况，因此速度会比较快。但因为并没有用到整个样本空间，这时候收敛是不是达到全局最优解了呢？不一定……

## 小批量梯度下降（MBGD，Mini-Batch Gradient Descent）

BGD存在效率问题，SGD存在全局最优解问题，因此有一个结合两种方法的训练方法MBGD。MBGD的训练步骤如下：
- 选择$k$个训练样本（一共有$m$个训练样本）
- 在这$k$个样本中进行$k$次迭代，每次迭代只使用一个样本（即SGD）
- $k$次迭代得到$k$个梯度，对这$k$个梯度加权求和（比如取平均）的结果来更新参数
- 重复以上步骤，直到收敛

通常第1步是随机选取$k$个样本，一般程序中会按顺序从训练数据中取batch，此时随机的过程需要在制作训练数据时保证，只要打乱训练数据的顺序就可以。

# Reference
- [梯度下降小结](https://www.cnblogs.com/pinard/p/5970503.html)
- [logistic回归详解（三）](https://blog.csdn.net/bitcarmanlee/article/details/51473567)
- [Logistic Regression逻辑回归的简单解释](https://blog.csdn.net/jinping_shi/article/details/52326980)
- [逻辑回归](https://blog.csdn.net/mzpmzk/article/details/51425692)





