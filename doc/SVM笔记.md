# SVM笔记



# 一、概况



## 用途

SVM可以用于二类分类

## 目标函数

在特征空间上间隔最大的线性分类器，通过使用核技巧，将输入空间的非线性分类映射到特征空间的线性分类。根据分类问题的困难程度，可以分为以下三种类型的支持向量机： 
\- 线性可分支持向量机 
\- 线性支持向量机 
\- 非线性支持向量机

## 求解算法

间隔最大化可以形式为一个求解凸二次规划问题，进而利用拉格拉日对偶性转化为求解对偶问题的最优值。 
求解对偶问题使用序列最小最优化算法（SMO，sequential minimal optimization），该算法1998年由Platt提出。

## 参考

后续文章参考《统计机器学习》李航







# 二、拉格朗日对偶性





------

## 用途

对于约束最优化问题，有时原始问题的最优解不好求解，可以借助拉格朗日对偶性将原始问题转化为对偶问题，通过求解对偶问题的解来获得原始问题的最优解。在最大熵模型和支持向量机中用到了该类方法。

## 描述

### 原始问题

假设$f(x),c_I(x),h_j(x)$为定义在$R^n$上面的连续可微函数，原始问题如下： 
$$
\min_{x\in R^n}f(x)\\
s.t. c_i(x)\le0,i=1,2...k\\
h_j(x)=0,j=1,2...l
$$
引入广义拉格朗日函数：
$$
L(x,\alpha,\beta)=f(x)+\sum_{i=1}^{k}\alpha_ic_i(x)+\sum_{j=1}^{l}\beta_jh_j(x)\\
s.t. \alpha_i\ge0,i=1,2...k
$$


可以将求解原始问题转化为求解以下极小极大问题：
$$
\min_{x}\max_{\alpha,\beta:\alpha_i\ge0}L(x,\alpha,\beta)
$$
原始问题和极小极大问题是等价的，证明见参考。



### 对偶问题（极大极小问题）

广义拉格朗日函数的极大极小问题可以表示成以下形式： 
$$
\max_{\alpha,\beta}\min_{x}L(x,\alpha,\beta)\\
s.t. \alpha_i\ge0
$$

### 两种问题的关系

如果原始问题和对偶问题都有最优值，那么满足 
$$
\max_{\alpha,\beta:\alpha_i\ge0}\min_{x}L(x,\alpha,\beta)\le\min_{x}\max_{\alpha,\beta:\alpha_i\ge0}L(x,\alpha,\beta)
$$
对于求解来讲，我们更需要关注的是上式中的等号情况，即原始问题和对偶问题的最优值相等的情况，此时可以通过求解对偶问题的最优解来作为原始问题的最优解。

 

假设$f(x)$和$c_i(x)$是凸函数，$h_j(x)$是仿射函数，并且不等式约束$c_i(x)$是严格可行的，则$x^∗,α^∗,β^∗$分别是原始问题和对偶问题的解的充分必要条件是满足下面的KKT（Karush-Kuhn-Tucker）条件：
$$
\nabla_xL(x^*,\alpha^*,\beta^*)=0\\
\nabla_\alpha L(x^*,\alpha^*,\beta^*)=0\\
\nabla_\beta L(x^*,\alpha^*,\beta^*)=0\\
\alpha_i^*c_i(x^*)=0, i=1,2...k\\
c_i(x^*)\le0,i=1,2...k\\
\alpha_i^*\ge0,i=1,2...k\\
h_j(x^*)=0,j=1,2...l
$$






# 三、线性可分支持向量机



------

## 预备

### 适合场景

如果训练输入线性可分，通过应间隔最大化学习得到的线性分类器称为线性可分支持向量机。 
假设特征空间上的训练数据集： 
$$
T=\{(x_1,y_1),(x_2,y_2),...,(x_N,y_N)\}
$$
其中$x_i$表示第i个特征向量，$y_i∈{+1,−1}$为$x_i$的类标记。

学习目标是在特征空间找到一个分离超平面：
$$
w⋅x+b=0
$$


可以将正负样例分开，即正负样例分布在超平面的两侧。



### 函数间隔和几何间隔

定义超平面关于训练数据集T的函数间隔为超平面(w,b)关于T中所有样本点的函数间隔最小值 
$$
\gamma'=\min_{1,2,...,N}\gamma'_i
$$
其中$\gamma'_i=y_i(w\cdot x_i+b)$ 

定义超平面关于训练数据集T的几何间隔为超平面(w,b)

关于T中所有样本点的几何间隔最小值
$$
\gamma=\min_{1,2,...,N}\gamma_i
$$
其中$\gamma_i=y_i(\frac{w}{||w||}\cdot x_i+\frac{b}{||w||})$

几何间隔的物理意义是指点到超平面的距离，函数间隔则会随着w和b成比例的缩放而改变。



## 构建目标函数

目标是找到能够正确划分训练数据集并且几何间隔最大的分离超平面。直观上面理解就是距离分界面最近的距离最大化。可以表示成如下形式： 
$$
\max_{w,b}\gamma\\
s.t. y_i(\frac{w}{||w||}\cdot x_i+\frac{b}{||w||})\ge\gamma , i=1,2,...,N
$$
基于几何间隔和函数间隔的关系，上式等价于
$$
\max_{w,b}\frac{\gamma'}{||w||}\\
s.t. y_i(w\cdot x_i+b)\ge\gamma' , i=1,2,...,N
$$


考虑到以$λ$等比例改变w和b，对于上式没有影响，这里选择固定一个λ使得$γ′=1$，那么上式等价于：
$$
\min_{w,b}\frac{1}{2}||w||^2\\
s.t. y_i(w\cdot x_i+b)-1\ge 0, i=1,2,...,N
$$
根据上式的最优解$w^∗$和$b^∗$可以构建出分离超平面和分类决策函数如下：
$$
w^*\cdot x+b^*=0\\
f(x)=sign(w^*\cdot x+b^*)
$$
训练数据集中的样本点中与分离超平面距离最近的样本点称为支持向量，对应于约束条件中的等号，即
$$
y_i(w\cdot x_i+b)-1= 0
$$
对应$y={−1,+1}$，支持向量分布在两条超平面上面： 
$$
(w\cdot x_i+b)-1= 0\\
(w\cdot x_i+b)+1= 0
$$
![Alt text](https://img-blog.csdn.net/20160526225351454) 

所以SVM分类中只有支持向量对应的实例在分类中起作用，其他实例点并没有作用。



## 求解的对偶算法

使用拉格朗日对偶性对原始问题求解。首先引入拉格朗日乘子$\alpha_i\ge0,i=1,2,...N$,构建拉格朗日函数: 
$$
L(w,b,\alpha)=\frac{1}{2}||w||^2-\sum_{i=1}^{N}\alpha_iy_i(w\cdot x_i+b)+\sum_{i=1}^{N}\alpha_i
$$
其中，$\alpha=(\alpha_1,\alpha_2,...,\alpha_N)^T$称为拉格朗日乘子向量。

原始问题对应的对偶问题如下：
$$
\max_{\alpha}\min_{w,b}L(w,b,\alpha)
$$
根据$L(w,b,\alpha)$对于w和b的偏导为0，可将原始问题转化为：
$$
\min_{\alpha}\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_{i}\alpha_{j}y_{i}y_{j}(x_i\cdot x_j)-\sum_{i=1}^{N}\alpha_i\\
\sum_{i=1}^{N}\alpha_iy_i=0\\
\alpha_i\ge0,i=1,2,...,N
$$
假设$\alpha^*=(\alpha_1^*,\alpha_2^*,...,\alpha_N^*)^T$是上面问题的最优解，那么:
$$
w^*=\sum_{i=1}^{N}\alpha_i^*y_ix_i
$$
选择一个下标j，使得$\alpha_j^*\gt0$，可得：
$$
b^*=y_j-\sum_{i=1}^{N}\alpha_i^*y_i(x_i\cdot x_j)
$$
根据KKT互补条件可知，$\alpha_i^*\gt0$对应的实例为支持向量。







# 四、线性支持向量机



------

## 预备

### 适合场景

如果训练输入线性不可分，存在一些特异点，通过软间隔最大化学习得到的线性分类器称为线性支持向量机。

## 构建目标函数

和线性可分支持向量机不同在于有些实例点不满足函数间隔大于等于1的条件。考虑引入松弛变量$\xi_i\ge0$和惩罚参数$C$： 
$$
\min_{w,b,\xi}\frac{1}{2}||w||^2+C\sum_{i=1}^{N}\xi_i\\
s.t. y_i(w\cdot x_i+b)\ge 1-\xi_i, i=1,2,...,N\\
\xi_i\ge0, i=1,2,...,N
$$
目标函数有两部分组成：第一部分是间隔，第二部分是误分类点的个数。C是惩罚参数，用于调和两部分的关系。



## 求解的对偶算法

原始问题的对偶问题是： 
$$
\min_{\alpha}\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_{i}\alpha_{j}y_{i}y_{j}(x_i\cdot x_j)-\sum_{i=1}^{N}\alpha_i\\
\sum_{i=1}^{N}\alpha_iy_i=0\\
0\le\alpha_i\le C,i=1,2,...,N
$$
假设$\alpha^*=(\alpha_1^*,\alpha_2^*,...,\alpha_N^*)^T$是上面问题的最优解，那么:
$$
w^*=\sum_{i=1}^{N}\alpha_i^*y_ix_i
$$
选择一个下标j，使得$0\lt\alpha_j^*\lt C$，可得：
$$
b^*=y_j-\sum_{i=1}^{N}\alpha_i^*y_i(x_i\cdot x_j)
$$
根据KKT互补条件可知，$α_i^∗>0$对应的实例为支持向量。

此时的支持向量可能分布在边界线或者边界线和分离超平面之间或者分离超平面误分一侧。

 ![这里写图片描述](https://img-blog.csdn.net/20160526225525030)











# 五、非线性支持向量机



## 预备

### 适合场景

如果训练输入线性不可分，可以使用非线性支持向量机，利用核技巧将输入空间非线性问题转化到特征空间线性可分问题。 
![这里写图片描述](https://img-blog.csdn.net/20160526225655674)

### 核技巧

对于输入空间的所有x,zx,z，函数K(x,z)满足条件 
$$
K(x,z)=\phi(x)\cdot \phi(z)
$$
则称K(x,z)为核函数，ϕ(x)为输入空间到特征空间的映射函数。

核函数的基本思想是进行输入空间到特征空间的映射，但是并没有显式地定义映射函数，一个核函数可能对应多种映射函数形式。

 

核函数导致特征维度增加：增加计算；降低泛化能力（容易过拟合）。



### 核函数的条件

设$\chi\subset R^n​$，K(x,z)是定义在$χ×χ​$上的对称函数，如果对任意$x_i∈χ,i=1,2,...,m​$，K(x,z)对应的Gram矩阵$K=[K(x_i,x_j)]_{m×m}​$是半正定矩阵，则称K(x,z)K(x,z)是正定核。

### 常用核函数

1. 多项式核函数
   $$
   K(x,z)=(x\cdot z+1)^p
   $$

2. 高斯核函数
   $$
   K(x,z)=exp(-\frac{||x-z||^2}{2\delta^2})
   $$

## 构建目标函数

考虑到SVM的对偶问题 
$$
\min_{\alpha}\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_{i}\alpha_{j}y_{i}y_{j}(x_i\cdot x_j)-\sum_{i=1}^{N}\alpha_i
$$


只是涉及到实例和实例之间的内积$x_i⋅x_j$，可以直接使用核函数进行替换，无需知道映射函数的具体形式。目标函数可替换为
$$
\min_{\alpha}\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_{i}\alpha_{j}y_{i}y_{j}K(x_i, x_j)-\sum_{i=1}^{N}\alpha_i\\
\sum_{i=1}^{N}\alpha_iy_i=0\\
0\le\alpha_i\le C,i=1,2,...,N
$$
假设$\alpha^*=(\alpha_1^*,\alpha_2^*,...,\alpha_N^*)^T$是上面问题的最优解，那么:
$$
w^*=\sum_{i=1}^{N}\alpha_i^*y_ix_i
$$
选择一个下标j，使得$0\lt\alpha_j^*\lt C$，可得：
$$
b^*=y_j-\sum_{i=1}^{N}\alpha_i^*y_iK(x_i,x_j)
$$
构造决策函数：
$$
f(x)=sign(\sum_{i=1}^{N}\alpha_i^*y_iK(x,x_i)+b^*)
$$

## 求最优解

要求解的最优化问题如下： 
$$
\min_{\alpha}\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_{i}\alpha_{j}y_{i}y_{j}K(x_i, x_j)-\sum_{i=1}^{N}\alpha_i\\
\sum_{i=1}^{N}\alpha_iy_i=0\\
0\le\alpha_i\le C,i=1,2,...,N
$$
考虑使用序列最小最优化算法（SMO，sequential minimal optimization），该算法1998年由Platt提出。



### 基本思想

对于有多个变量的最优化问题，使用迭代求解，每次迭代选择两个变量，其余变量固定，求解两个变量的二次规划问题（可以使用解析方法求解），依次迭代下去，知道所有的变量都满足KKT条件。选择两个变量的标准是其中一个变量是违反KKT条件最严重，另一个变量可以由约束条件自动确定。 
直观上面理解：每次找出最离谱的变量，使他满足靠谱的条件，直到所有的变量都靠谱了。

### 解析方法

1. 目标函数

    

   假设选择$α_1$和$α_2$，其他变量固定，最优化问题转化为下面的形式：

$$
\min_{\alpha_1,\alpha_2}\frac{1}{2}K_{11}\alpha_{1}^{2}+\frac{1}{2}K_{22}\alpha_{2}^{2}+y_{1}y_{2}K_{12}\alpha_1\alpha_2-(\alpha_1+\alpha_2)+y_{1}\alpha_{1}\sum_{i=3}^{N}y_{i}\alpha_{i}K_{i1}+y_{2}\alpha_{2}\sum_{i=3}^{N}y_{i}\alpha_{i}K_{i2}\\
\alpha_{1}y_{1}+\alpha_{2}y_{2}=-\sum_{i=3}^{N}y_i\alpha_i=\zeta\\
0\le\alpha_i\le C,i=1,2
$$

   ps:第一个式子忽略了一个常数项。

2. 上下限剪裁

    初始可行解为$\alpha_1^{old}$和$\alpha_2^{old}$，最优解为$\alpha_1^{new}$和$\alpha_2^{new}$.

   假设$y_1\ne y_2, 0\lt\zeta\lt C$，有约束条件可知：

   $$
   \alpha_2^{old}-\alpha_1^{old}=\alpha_2^{new}-\alpha_1^{new}
   $$
   考虑到$\alpha_1^{new}$满足$0\le\alpha_1^{new}\le C$，所以$\alpha_2^{new}$满足$\zeta\le\alpha_2^{new}\le C$

   一般来讲，最优值$α_2^{old}$的取值范围有了进一步的限制，表示如下：

   $$
   L\le\alpha_2^{new}\le H
   $$
    当$y_1≠y_2$，H和L取值如下：
   $$
   L=max(0,\alpha_2^{old}-\alpha_1^{old}),H=min(C,C+\alpha_2^{old}-\alpha_1^{old})
   $$
   当$y1=y2$，H和L取值如下： 

   $$
   L=max(0,\alpha_2^{old}+\alpha_1^{old}-C),H=min(C,\alpha_2^{old}+\alpha_1^{old})
   $$

3. 最优解

   沿约束方向未剪裁的解
   $$
   \alpha_2^{new,unc}=\alpha_2^{old}+\frac{y_2(E_{1}-E_{2})}{\eta}
   $$
   其中

   $$
   \eta=K_{11}+K_{22}-2K_{12}\\
   E_i=(\sum_{j=1}^{N}\alpha_jy_jK(x_j,x_i)+b)-y_i
   $$
   剪裁以后的解变为

   $$
   \alpha_2^{new} =
    \begin{cases}
     H   &  {\alpha_2^{new,unc}\gt H} \\
     \alpha_2^{new,unc} &  {L\le\alpha_2^{new,unc}\le H} \\
     L   &  {\alpha_2^{new,unc}\le L} \\
    \end{cases}
   $$
   根据$\alpha_2^{old}-\alpha_1^{old}=\alpha_2^{new}-\alpha_1^{new}$可以求得
   $$
   \alpha_1^{new}=\alpha_1^{old}+y_1y_2(\alpha_2^{old}-\alpha_1^{old})
   $$
   

### 选择变量

1. 第一个变量

   SMO称选择第一个变量为外层循环，找出违反KKT条件最严重的样本点，对应KKT条件如下：

   $$
   \alpha_i=0\Leftrightarrow y_ig(x_i)\ge1 \text {     间隔线以外正确分类情况}\\
   0\lt\alpha_i\lt C\Leftrightarrow y_ig(x_i)=1 \text {     间隔线上的样本点}\\
   \alpha_i=C\Leftrightarrow y_ig(x_i)\le1 \text {     两条间隔点之间的点}
   $$
   其中$g(x_i)=\sum_{j=1}^{N}\alpha_{j}y_{j}K(x_{i}, x_{j})+b$表示对于输入$x_i$的预测值。

   首先遍历间隔线上面的支持向量点$0<α_i<C$，如果都满足，遍历整个训练集。

2. 第二个变量的选择 
   称为内层循环。选择标准是能够使$α_2$有足够大的变化（即加快收敛速度）。考虑到$α_2^{new}$正比于$|E_1−E_2|$，可以根据该标准选择$α_2$。 
   如果选择$α_2$不能使目标函数有足够的下降，那么遍历间隔边界的支持向量点，如果仍不合适，重新选择$α_1$。

3. 最后的停机条件

   $$
   \sum_{i=1}^{N}\alpha_iy_i=0\\
   0\le\alpha_i\le C,i=1,2,...,N\\
   \alpha_i=0\Leftrightarrow y_ig(x_i)\ge1\\
   0\lt\alpha_i\lt C\Leftrightarrow y_ig(x_i)=1\\
   \alpha_i=C\Leftrightarrow y_ig(x_i)\le1
   $$



