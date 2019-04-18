# Multivariate Linear Regression的参数估计 - 小平子的专栏 - CSDN博客





2016年07月17日 12:27:39[阿拉丁吃米粉](https://me.csdn.net/jinping_shi)阅读数：1068








多元线性回归的表现形式是$$(1)h_\theta(x)=y=\theta_0 + \theta_1x_1 + \theta_2x_2+...+\theta_nx_n \tag{1}$$

其中$x_1...x_n$是$n$个变量，$\theta_0...\theta_n$是$n+1$个参数。$h$是hypothesis的简称。如果定义$x_0 = 1$，(1)式可以变为(2)式：
$$(2)h_\theta(x)=y=\theta_0x_0 + \theta_1x_1 + \theta_2x_2+...+\theta_nx_n  \tag{2}$$

其中$x_0 = 1$.
令： $$X = \begin{bmatrix}   x_0 \\   x_1 \\   \vdots \\   x_n  \end{bmatrix}, \Theta = \begin{bmatrix}   \theta_0 \\   \theta_1 \\   \vdots \\   \theta_n  \end{bmatrix}  $$

(2)式可简记为(3)式：
$$(3)h_\theta(x) = \Theta^{\rm T}X \tag{3}$$
例如下表，我们要根据房屋属性预测房屋价格。那么在该例中，一共有4个属性，分别是Size, Number of bedrooms, Number of floors, Age of home，表示为$x_1, x_2, x_3, x_4$. Price是要预测的值，即$y$，这里表示为$h_\theta(x)$.
![表格](https://img-blog.csdn.net/20160722210635005)
上表中有很多行，每一行称为一个训练样本(training example/sample). 为方便描述，定义以下符号：
$n$: 属性数量
$m$: 训练样本数量
$x^{(i)}$: 第$i$个训练样本的属性
$x^{(i)}_j$: 第$i$个训练样本的第$j$个属性
从(3)式可知，如果我们知道了参数矩阵$\Theta^{\rm T}$的每一个值，就可以用方程(3)来预测未知样本的值。如何求得$\Theta^{\rm T}$？机器学习中的一个重要任务就是估计$\Theta^{\rm T}$. 足够好的$\Theta^{\rm T}$必定会使得预测值无限接近于真实值，如果设计一个关于$\Theta^{\rm T}$的函数用来表示预测值与真实值之间的差异，那么求得一个足够小的差异，就可以解出$\Theta^{\rm T}$，这个函数通常成为代价函数（cost function），简记为$J(\boldsymbol{\theta})$.

## 代价函数及其参数的估计

多元线性回归方程的代价函数定义如下：
$$(4)J(\theta_0, \theta_1, ..., \theta_n) = \frac{1}{2m}\sum_{i=1}^{m}(h_\theta(x^{(i)} ) - y^{(i)})^2  \tag{4}$$

其中$h_\theta(x^{(i)})$是预测值，$y^{(i)}$是真实值。特意在右项乘以$\frac{1}{2m}$是为了之后计算方便（求极值需求导，可约去）。代价函数的直观理解是若$J(\boldsymbol{\theta})$越小，表示预测值与真实值之间的差异越小，一定程度上说明我们的假设$h_\theta(x)$比较好；反之差异越大，$h_\theta(x)$不够好。
### 梯度下降法估计参数

估计$\theta$ (本文中$\theta = \Theta^{\rm T}$)的本质是在代价函数$J(\boldsymbol{\theta})$取最小值（局部最小值）的情况下$\theta$的取值。梯度下降（Gradient descent）法估计参数就是不断对每一个$\theta_i$求偏导，得到新的参数$\theta^*_i$，迭代一定次数或新的参数$\theta^*_i$与上一个参数相比不再变化或变化很小时，即求得$\theta$.

例如，对$\theta_j$求偏导，

$$(5)\frac{\partial J(\boldsymbol{\theta})}{\partial \theta_j} = -\frac{1}{m} \sum_{i=1}^{m} (h_\theta(x^{(i)}) - y^{(i)})x_j^{(i)} \tag{5}$$

梯度是函数上某一点增长最快的方向。在参数估计中，我们需要降低cost，因此选择梯度的反方向作为更新的方向。故更新$\theta_j$时，用上一步的$\theta_j$加上梯度的负数，即

$$\theta_j := \theta_j - \frac{\partial J(\boldsymbol{\theta})}{\partial \theta_j}$$

$$(6)\Rightarrow \theta_j :=\theta_j + \frac{1}{m} \sum_{i=1}^{m} (h_\theta(x^{(i)}) - y^{(i)})x_j^{(i)} \tag{6}$$

更新$\theta_j$时，可以使用所有样本来更新（(6)式中对样本的数量$m$求和），这就是所谓的BGD（批量梯度下降，Batch Gradient Descent）。

实际应用时会在更新$\theta_j$时乘以一个系数，如下：

**Repeat (until converged or reach the maximum iteration times):**
$$\theta_j := \theta_j - \alpha \frac{\partial}{\partial\theta_j}J(\theta_0, \theta_1, ..., \theta_n)$$
$$(7)\Rightarrow \theta_j :=\theta_j + \alpha \frac{1}{m} \sum_{i=1}^{m} (h_\theta(x^{(i)}) - y^{(i)})x_j^{(i)} \tag{7}$$
**以上过程要同时求出所有的$\theta_0, ..., \theta_n$并同时更新所有的$\theta_0, ..., \theta_n$**. 上式中的$\alpha$称为**learning rate**，它控制了学习的速率。当$\alpha$赋值很大时，上述迭代过程有可能不收敛；但当$\alpha$赋值太小时，对大数据量而言，收敛过程可能会很慢。理想的$\alpha$赋值会使的$\theta_j$会随着迭代次数的增大而逐步减小（每一个新的迭代$\theta_j$的值都会比上一次迭代周期中的值小。

迭代终止的条件是：
**达到指定的迭代的次数**，或者
**新的$\theta_j$与上次的$\theta_j$差值在某个阈值之内**
### 正规方程求解参数

正规方程（Normal Equation）求解参数是解析法求解，不需要迭代过程。

过程參考[正规方程推导](http://www.cnblogs.com/hust-ghtao/p/3580188.html)（我不想輸公式了）

### 代价函数的概率解释与推导

上面给出了代价函数的定义与参数估计的过程。代价函数的定义如(4)式，直觉上很好理解，但为何代价函数要定义成这个样子？下面给出一个概率上的解释。

文章最开始给出了线性回归模型的表现形式，如(1)式。更准确的写法应该是：
$$(1’)h_\theta(x)=y=\theta_0 + \theta_1x_1 + \theta_2x_2+...+\theta_nx_n+\varepsilon \tag{1'}$$

多出来的$\varepsilon$是误差项，表示线性回归的假设方程包含未知的误差和随机噪声。通常假设误差项是独立同分布（iid, Independent Identically Distribution），且服从均值为0，方差为$\sigma^2$的正态分布。假设误差项服从正态分布的原因是：- 数学上处理的便利
- 根据中心极限定理，独立的随机变量的和，即多种随机误差的累积，其总的影响是接近正态分布的。实际应用中，诸多独立随机变量的分布是服从正态分布的。

因此，对于每一个训练样本，其误差$\varepsilon^{(i)}$的密度函数为：
$$(7)p(\varepsilon^{(i)}) = \frac{1}{\sqrt{2\pi}\sigma}exp({-\frac{(\varepsilon^{(i)})^2}{2\sigma^2})} \tag{7}$$

再由(1’)式和(3)式，改写成机器学习中常见的条件概率形式，有：
$$(8)p(y^{(i)}|x^{(i)};\theta) = \frac{1}{\sqrt{2\pi}\sigma}exp({-\frac{(y^{(i)} -  \Theta^{\rm T}x^{(i)})^2}{2\sigma^2})} \tag{8}$$

简记为：
$$(9)p(y^{(i)}|x^{(i)};\theta) \sim N( \Theta^{\rm T}x^{(i)}, \sigma^2) \tag{9}$$

上式表示，若给定参数$\theta$，在已知随机变量（训练样本）为$x^{(i)}$的情况下，$y^{(i)}$的概率密度分布。
注意到$x^{(i)}$是iid，对所有随机变量（训练样本）$x^{(i)}$的联合概率密度可以写成连乘的形式：
$$L(\theta) = p(y|X;\Theta) = \prod_{i=1}^{m}p(y^{(i)}|x^{(i)};\theta)$$
$$(10)\Rightarrow L(\theta) = \prod_{i=1}^{m}{\frac{1}{\sqrt{2\pi}\sigma}exp({-\frac{(y^{(i)} -  \Theta^{\rm T}x^{(i)})^2}{2\sigma^2})}} \tag{10}$$
$L(\theta)$称为似然（likelihood）函数。注意到似然函数是关于参数$\theta$的函数，求解参数$\theta$的过程称为参数的极大似然估计。含义是找到一个$\theta$，使得$L(\theta)$的值最大。显然，对$L(\theta)$求$\theta$的导数并令该导数等于0，此时求得的$\theta$即为最小值或极小值（对于单变量的线性回归方程，可以求得最小值，但是对于多变量线性回归，一般求得极小值）。

对(9)式两边取对数，将连乘化为求和：
$$(11)l(\theta) = \log{L(\theta)} = m \log(\frac{1}{\sqrt{2\pi}\sigma} - \frac{1}{\sigma^2} \frac{1}{2} \sum_{i=1}^{m}(y^{(i)} - \Theta^{\rm T}x^{(i)})^2 ) \tag{11}$$

显然，要使得$l(\theta)$最小，则上式中的$\sum_{i=1}^{m}(y^{(i)} - \Theta^{\rm T}x^{(i)})^2$要最大。将该项中的$y^{(i)}$替换为$h_\theta(x^{(i)})$， $\Theta^{\rm T}x^{(i)}$替换为$y^{(i)}$即得(4)式中的形式。
## 线性回归的规范化

拿到training data，提取属性后，构造(4)式作为代价函数，通过梯度下降或正规方程可以得到回归方程的参数，将参数代入(2)式即可预测未知数据。但是如果数据的属性很多，例如一个语料库（corpus）用了2000个不同的单词，将这些单词全部作为线性回归方程的输入，那么属性数量$n=2000$，最后需要求解的参数$\theta$数量为2001个。若以单词是否出现作为属性值，即某篇文章中，一个单词出现记为1，反之记为0，那么属性矩阵$X$将是一个十分稀疏的矩阵。

通常规范化（regularization）为了解决以下问题：
- 相对于训练样本数量，属性数量太多，容易造成过度拟合（over-fitting）
- 属性数量太多且属性矩阵是个稀疏矩阵

线性回归的主要任务是计算代价函数的最小值得到参数。线性方程的规范化（Regularized Linear Regression）就从改造代价函数开始。

如下图，若要根据房屋面积预测房价，根据图中的训练样本分布（那些叉叉），一个二次多项式足够了。
![二次多項式擬合|center](https://img-blog.csdn.net/20160722205849014)
但是如果用更高次的多项式呢？如下图，可以将训练样本拟合得更加好；然而此时用下圖预测，结果肯定惨不忍睹。
![多次項擬合](https://img-blog.csdn.net/20160722210317250)
注意到两个方程之间只差了$\theta_3x^3$和$\theta_4x^4$两项。若令$\theta_3$和$\theta_4$足够小，这两项会趋近于0，进而相当与二次多项式。规范化即以此为理念，如果参数太多，就将多余的参数的缩小。但实际上不知道哪些参数是多余的，因此将所有参数都缩小，改造一下代价函数，得到：
$$(12)J(\theta) = \frac{1}{2m}[\sum_{i=1}^{m}(h_\theta(x^{(i)}) - y^{(i)})^2 + \lambda \sum_{j=1}^{n}\theta_j^2] \tag{12}$$

其中$\lambda$称为规范化系数（regularization parameter）。在迭代过程中，最后的$\theta_1, ..., \theta_n$会变小。
### 规范化后的梯度下降过程

略加推导，规范化后的线性回归梯度下降估计参数过程变为：

**Repeat:**
$$\theta_j := \theta_j - \alpha \frac{\partial}{\partial\theta_j}J(\theta_0, \theta_1, ..., \theta_n)$$
$$\Rightarrow \theta_j :=\theta_j - \alpha [\frac{1}{m} \sum_{i=1}^{m} (h_\theta(x^{(i)}) - y^{(i)})x_j^{(i)} + \frac{\lambda}{m}\theta_j]$$
$$(13)\Rightarrow \theta_j :=\theta_j (1- \alpha \frac{\lambda}{m}) - \alpha \frac{1}{m} \sum_{i=1}^{m} (h_\theta(x^{(i)}) - y^{(i)})x_j^{(i)} \tag{13} $$
### 规范化后的正规方程

规范化后的正规方程形式如下（我也不知道怎么推导）：

$$(14)\theta = (X^TX + \lambda A)^{-1}X^Ty \tag{14}$$

其中A是对角线上除了第一个元素为0的对角方阵：
$$A = \left[\begin{matrix} 0      &       &    &       \\         & 1    &    &       \\         &       &1  &       \\         &       &    & \ddots  \\         &       &     &  &1      \\\end{matrix}\right]$$
# Reference

以上例子、公式、圖片來源於[Coursera Machine Learning在線課程](https://www.coursera.org/learn/machine-learning/)。

回归模型的代价函数的推导过程参考：[回归模型的概率解释](http://nanshu.wang/post/2015-02-11/)。



















