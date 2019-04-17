# 感知机Perceptron - 浅梦的博客 - CSDN博客





2017年08月25日 16:18:01[浅梦s](https://me.csdn.net/u012151283)阅读数：353








# 感知机学习策略

## 数据假设和学习目标

假设训练数据是线性可分的，感知机学习的目标是**求得一个能够将训练集正实例点和负实例点完全正确分开的分离超平面**。  

## 损失函数

损失函数的一个自然选择是误分类点的总和。但这样的损失函数不是参数w,b的连续可导函数，不易优化。 

另一个选择是误分类点到超平面S的总距离。 

假设超平面S的误分类点集合为M，那么所有误分类点到超平面的总距离为 
$-\frac{1}{||w||}\sum\limits_{x_i\in M}y_i(w\cdot x_i+b)$

不考虑$\frac{1}{||w||}$，就得到感知机学习的损失函数。
# 感知机学习算法

## 原始形式primal

$\min\limits_{w,b}L(w,b)=-\sum\limits_{x_i\in M}y_i(w\cdot x_i+b)\text{(2.5)}$,其中M为误分类点的集合。 

感知机学习算法是误分类驱动的，具体采用随机梯度下降法(stochastic gradient descent)。首先，任意选取一个超平面$w_0,b_0$，然后用梯度下降法不断地极小化目标函数(2.5)。极小化过程中不是一次使M中所有误分类点的梯度下降，而是一次随机选取一个误分类点使其下降。 

原始算法描述
> 
给定感知机模型$f(x)=sign(w\cdot x+b)$

  1. 选取初值$w_0,b_0$

  2. 在训练集中选取数据$(x_i,y_i)$

  3. 如果$y_i(w\cdot x_i+b)\le0$


$\begin{align*}w&\leftarrow w+\eta y_ix_i\\b&\leftarrow b+\eta y_i\end{align*}$

  4. 转至2，直到训练集中没有误分类点。
直观解释： 

当一个实例点被误分类，即位于分离超平面的错误一侧时，则调整$w,b$的值，使分离超平面向该误分类点的一侧移动，以减少该误分类点与超平面间的距离，直至超平面越过该误分类点使其被正确分类。

感知机学习算法由于采用不同的初值或选取不同的误分类点，解可以不同。

## 算法的收敛性

## 对偶形式dual

对偶形式的**基本想法**是，将w和b表示为实例$x_i$和而标记$y_i$的线性组合的形式，通过求解其系数而求得w和b。 

对误分类点$(x_i,y_i)$通过 


$\begin{align*}w&\leftarrow w+\eta y_ix_i\\b&\leftarrow b+\eta y_i\end{align*}$

逐步修改w,b。设修改n次，则w,b关于$(x_i,y_i)$的增量分别是$a_iy_ix_i$和$a_iy_i$，这里$a_i=n_i\eta$。这样，从学习过程不难看出，最后学习到的$w,b$可以分别而表示为 


$\begin{align*}w&=\sum\limits_{i=1}^Na_iy_ix_i\\b&=\sum\limits_{i=1}^Na_iy_i\end{align*}$

这里，$a_i\ge0,i=1,...,N$，**当$\eta=1$，表示第i个实例点由于误分类而进行更新的次数**。实例点更新次数越多，意味着它距离分离超平面越近，也就越难正确分类。这样的实例对学习结果影响最大。
## 对偶算法描述

> 
输出$a,b$,给定感知机模型$f(x)=sign(\sum\limits_{j=1}^Na_jy_jx_j\cdot x+b)$

  其中$a=(a_1,a_2,...,a_N)^T$

  1. $a\leftarrow0,b\leftarrow0$

  2. 在训练集中选取数据$(x_i,y_i)$

  3. 如果$y_i(\sum\limits_{j=1}^Na_jy_jx_j\cdot x+b)\le0$


$\begin{align*}a_i&\leftarrow a_i+\eta \\b&\leftarrow b+\eta y_i\end{align*}$

  4. 转至2，直到训练集中没有误分类点。
对偶形式中训练实例仅以内积的形式出现，为了方便，可以预先将训练集中实例间的内积计算出来并以矩阵的形式存储，这个矩阵就是Gram矩阵。 
$G=[x_i\cdot x_j]_{N\times N}$

参考文献

> 
《统计学习方法》第2章











