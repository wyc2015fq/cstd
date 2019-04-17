# 逻辑回归（logistic regression）原理详解 - guoziqing506的博客 - CSDN博客





2018年08月01日 12:34:18[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：900标签：[逻辑回归																[logistic regression																[最大似然估计																[分类](https://so.csdn.net/so/search/s.do?q=分类&t=blog)
个人分类：[数据挖掘																[机器学习](https://blog.csdn.net/guoziqing506/article/category/6776278)](https://blog.csdn.net/guoziqing506/article/category/6289196)

所属专栏：[机器学习经典算法研究](https://blog.csdn.net/column/details/25189.html)





机器学习解决的问题，大体上就是两种：数值预测和分类。前者一般采用的是回归模型，比如最常用的线性回归；后者的方法则五花八门，决策树，kNN，支持向量机，朴素贝叶斯等等模型都是用来解决分类问题的。其实，两种问题从本质上讲是一样的：都是通过对已有数据的学习，构建模型，然后对未知的数据进行预测，若是连续的数值预测就是回归问题，若是离散的类标号预测，就是分类问题。

这里面有一类比较特殊的算法，就是逻辑回归（logistic regression）。它叫“回归”，可见基本思路还是回归的那一套，同时，逻辑回归又是标准的解决分类问题的模型。换句话说，逻辑回归是用与回归类似的思路解决了分类问题。

## 1. 阶跃函数

现在有$n$个数据元组$\{X_1, X_2, \dots, X_n\}$，每个数据元组对应了一个类标号$y_i$，同时每个数据元组$X_i$有$m$个属性$\{x_{i1}, x_{i2}, \dots, x_{im}\}$。假设现在面临的是一个简单的二分类问题，类标号有0，1两种。如果用简单的回归方法对已知数据进行曲线拟合的话，我们会得到如下的曲线方程（曲线拟合的方法后面会说到）：



$\begin{equation}z = f(X) = w_0 + w_1x_1 + w_2x_2 + \dots + w_mx_m\end{equation}$

注：并不是说逻辑回归只能解决二分类问题，但是用到多分类时，算法并没有发生变化，只是用的次数更多了而已。

实际上，逻辑回归分类的办法与SVM是一致的，都是在空间中找到曲线，将数据点按相对曲线的位置，分成上下两类。也就是说，对于任意测试元组$X^*$，$f(X^*)$可以根据其正负性而得到类标号。那换句话说，直接依靠拟合曲线的函数值是不能得到类标号的，还需要一种理想的“阶跃函数”，将函数值按照正负性分别映射为0，1类标号。这样的阶跃函数$\phi(z)$如下表示：



$\begin{equation}\phi(z) = \left\{\begin{aligned}    0, ~~~z < 0\\    0.5, ~~~z = 0\\    1, ~~~z > 0    \end{aligned}\right.\end{equation}$

然而，直接这样设计阶跃函数不方便后面的优化计算，因为函数值不连续，无法进行一些相关求导。所以，逻辑回归中，大家选了一个统一的函数，也就是Sigmoid函数，如公式(1)所示：



$\begin{equation}\phi(z) = \frac{1}{1 + e^{-z}}\end{equation}\tag{1}$

Sigmoid函数的图像如下图所示，当$z > 0$时，Sigmoid函数大于0.5；当$z < 0$时，Sigmoid函数小于0.5。所以，我们可以将拟合曲线的函数值带入Sigmoid函数，观察$\phi(z)$与0.5的大小确定其类标号。



![](https://img-blog.csdn.net/20180801123103503?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


**注1：上图摘抄于 [逻辑回归(logistic regression)的本质——极大似然估计](https://blog.csdn.net/zjuPeco/article/details/77165974)**

Sigmoid函数还有一个好处，那就是因为其取值在0，1之间。所以可以看做是测试元组属于类1的后验概率，即$p(y = 1|X)$。其实这一点从图像也可以看出来：$z$的值越大，表明元组的空间位置距离分类面越远，他就越可能属于类1，所以图中$z$越大，函数值也就越接近1；同理，$z$越小，表明元组越不可能属于类1.

## 2. 代价函数

阶跃函数告诉我们，当得到拟合曲线的函数值时，如何计算最终的类标号。但是核心问题仍然是这个曲线如何拟合。既然是回归函数，我们就模仿线性回归，用误差的平方和当做代价函数。代价函数如公式(2)所示：



$\begin{equation}J(w) = \sum_{i = 1}^{n} \frac{1}{2} (\phi(z_i) - y_i)^2\end{equation}\tag{2}$

其中，$z_i = W^TX_i + w_0$，$y_i$为$X_i$真实的类标号。按说此时可以对代价函数求解最小值了，但是如果你将$\phi(z) = \frac{1}{1 + e^{-z}}$带入公式(2)的话，那么当前代价函数的图像是一个非凸函数，非凸函数有不止一个极值点，导致不容易做最优化计算。也就是说，公式(2)的这个代价函数不能用。

**注2：有关于凸函数的相关知识可以参考我的上一篇博客： [最大化期望算法（EM）详解](https://blog.csdn.net/guoziqing506/article/details/81274276)**

那自然要想办法设计新的代价函数。我们在上面说了，$\phi(z)$的取值可以看做是测试元组属于类1的后验概率，所以可以得到下面的结论：



$\begin{equation}\begin{aligned}&p(y = 1|X; W) = \phi(z)\\&p(y = 0|X; W) = 1 - \phi(z)\end{aligned}\end{equation}$

更进一步，上式也可以这样表达：



$\begin{equation}p(y|X; W) = \phi(z)^y (1 - \phi(z))^{1 - y}\end{equation}\tag{3}$

公式(3)表达的含义是在参数$W$下，元组类标号为$y$的后验概率。假设现在已经得到了一个抽样样本，那么联合概率$\prod_{i = 1}^{n}p(y_i|X_i; W)$的大小就可以反映模型的代价：联合概率越大，说明模型的学习结果与真实情况越接近；联合概率越小，说明模型的学习结果与真实情况越背离。而对于这个联合概率，我们可以通过计算参数的最大似然估计的那一套方法来确定使得联合概率最大的参数$W$，此时的$W$就是我们要选的最佳参数，它使得联合概率最大（即代价函数最小）。下面看具体的运算步骤。

**注3：有关参数的最大似然估计可以参考我之前的博客： [最大化期望算法（EM）详解](https://blog.csdn.net/guoziqing506/article/details/81274276)**

## 3. 逻辑回归的运算步骤

### 3.1 写出最大似然函数，并进行对数化处理



$\begin{equation}\begin{aligned}L(W) &= \sum_{i = 1}^n \ln p(y_i|X_i; W)\\&= \sum_{i = 1}^n \ln (\phi(z_i)^{y_i} (1 - \phi(z_i))^{1 - y_i})\\&= \sum_{i = 1}^n y_i \ln \phi(z_i) + (1 - y_i) \ln(1 - \phi(z_i)\end{aligned}\end{equation}\tag{3}$

通过上面的分析，显然$-L(W)$就是代价函数$J(W)$。为方便后面的推导，我把$J(W)$也写出来：



$\begin{equation}J(W) = -\sum_{i = 1}^n y_i \ln \phi(z_i) + (1 - y_i) \ln(1 - \phi(z_i))\end{equation}\tag{4}$

博客 [逻辑回归(logistic regression)的本质——极大似然估计](https://blog.csdn.net/zjuPeco/article/details/77165974) 中给出了$J(W)$的图像，我把它抄在下面，方便大家理解代价函数起到的效果：



![](https://img-blog.csdn.net/20180801121944146?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


可以看出，如果类标号为1，那么$\phi(z)$的取值在$[0, 1]$范围内增大时，代价函数减小，说明越接近真实情况，代价就越小；如果类标号为0，也是一样的道理。

### 3.2 用梯度下降法求代价函数的最小值

如果能求出代价函数的最小值，也就是最大似然函数的最大值。那么得到的权重向量$W$就是逻辑回归的最终解。但是通过上面的图像，你也能发现，$J(W)$是一种非线性的S型函数，不能直接利用偏导数为0求解。于是我们采用梯度下降法。

首先，根据梯度的相关理论，我们知道梯度的负方向就是代价函数下降最快的方向。因此，我们应该沿着梯度负方向逐渐调整权重分量$w_j$，直到得到最小值，所以每个权重分量的变化应该是这样的：



$\begin{equation}\Delta w_j = -\eta \frac{\partial J(W)}{\partial w_j}\end{equation}$

其中$\eta$为学习率，控制步长。而$\frac{\partial J(W)}{\partial w_j}$可以如下计算：



$\begin{equation}\begin{aligned}\frac{\partial J(w)}{\partial w_j} &= - \sum_{i = 1}^n (y_i \frac{1}{\phi(z_i)} - (1 - y_i)\frac{1}{1 - \phi(z_i)}) \cdot \frac{\partial \phi(z_i)}{\partial w_j}\\&= - \sum_{i = 1}^n (y_i(1 - \phi(z_i)) - (1 - y_i) \frac{1}{1 - \phi(z_i)}) \cdot x_{ij}\\&= - \sum_{i = 1}^n (y_i - \phi(z_i)) \cdot x_{ij}\end{aligned}\end{equation}$

上式的推导中用到了Sigmoid函数$\phi(z)$的一个特殊的性质：



$\begin{equation}\phi(z)' = \phi(z)(1 - \phi(z))\end{equation}\tag{4}$

这样，我们就得到了梯度下降法更新权重的变量：



$\begin{equation}w_j := w_j + \eta \sum_{i = 1}^n (y_i - \phi(z_i)) \cdot x_{ij}\end{equation}$

最后，说一下权重向量的初始化问题。一般用接近于0的随机值初始化$w_j$，比如在区间$[-0.01, 0.01]$内均匀选取。这样做的理由是如果$w_i$很大，则加权和可能也很大，根据Sigmoid函数的图像（即本文的第一个图像）可知，大的加权和会使得$\phi(z_i)$的导数接近0，则变化速率变缓使得权重的更新变缓。](https://so.csdn.net/so/search/s.do?q=最大似然估计&t=blog)](https://so.csdn.net/so/search/s.do?q=logistic regression&t=blog)](https://so.csdn.net/so/search/s.do?q=逻辑回归&t=blog)




