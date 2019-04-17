# SVM笔记（五） 非线性支持向量机 - xmdxcsj的专栏 - CSDN博客





2016年05月26日 22:57:30[xmucas](https://me.csdn.net/xmdxcsj)阅读数：3142








# SVM笔记（五） 非线性支持向量机

@(svm)

## 预备

### 适合场景

如果训练输入线性不可分，可以使用非线性支持向量机，利用核技巧将输入空间非线性问题转化到特征空间线性可分问题。 
![这里写图片描述](https://img-blog.csdn.net/20160526225655674)
### 核技巧

对于输入空间的所有$x,z$，函数$K(x,z)$满足条件 


$K(x,z)=\phi(x)\cdot \phi(z)$

则称$K(x,z)$为核函数，$\phi(x)$为输入空间到特征空间的映射函数。 

核函数的基本思想是进行输入空间到特征空间的映射，但是并没有显式地定义映射函数，一个核函数可能对应多种映射函数形式。 

核函数导致特征维度增加：增加计算；降低泛化能力（容易过拟合）。
### 核函数的条件

设$\chi\subset R^n$，$K(x,z)$是定义在$\chi \times \chi$上的对称函数，如果对任意$x_i\in \chi, i=1,2,...,m$，$K(x,z)$对应的Gram矩阵$K=[K(x_i,x_j)]_{m\times m}$是半正定矩阵，则称$K(x,z)$是正定核。

### 常用核函数
- 多项式核函数 


$K(x,z)=(x\cdot z+1)^p$
- 高斯核函数 


$K(x,z)=exp(-\frac{||x-z||^2}{2\delta^2})$

## 构建目标函数

考虑到SVM的对偶问题 


$\min_{\alpha}\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_{i}\alpha_{j}y_{i}y_{j}(x_i\cdot x_j)-\sum_{i=1}^{N}\alpha_i$

只是涉及到实例和实例之间的内积$x_i\cdot x_j$，可以直接使用核函数进行替换，无需知道映射函数的具体形式。目标函数可替换为 


$\min_{\alpha}\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_{i}\alpha_{j}y_{i}y_{j}K(x_i, x_j)-\sum_{i=1}^{N}\alpha_i$


$\sum_{i=1}^{N}\alpha_iy_i=0$


$0\le\alpha_i\le C,i=1,2,...,N$

假设$\alpha^*=(\alpha_1^*,\alpha_2^*,...,\alpha_N^*)^T$是上面问题的最优解，那么: 


$w^*=\sum_{i=1}^{N}\alpha_i^*y_ix_i$

选择一个下标j，使得$0\lt\alpha_j^*\lt C$，可得： 


$b^*=y_j-\sum_{i=1}^{N}\alpha_i^*y_iK(x_i,x_j)$

构造决策函数： 


$f(x)=sign(\sum_{i=1}^{N}\alpha_i^*y_iK(x,x_i)+b^*)$
## 求最优解

要求解的最优化问题如下： 


$\min_{\alpha}\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_{i}\alpha_{j}y_{i}y_{j}K(x_i, x_j)-\sum_{i=1}^{N}\alpha_i$


$\sum_{i=1}^{N}\alpha_iy_i=0$


$0\le\alpha_i\le C,i=1,2,...,N$

考虑使用序列最小最优化算法（SMO，sequential minimal optimization），该算法1998年由Platt提出。
### 基本思想

对于有多个变量的最优化问题，使用迭代求解，每次迭代选择两个变量，其余变量固定，求解两个变量的二次规划问题（可以使用解析方法求解），依次迭代下去，知道所有的变量都满足KKT条件。选择两个变量的标准是其中一个变量是违反KKT条件最严重，另一个变量可以由约束条件自动确定。 

直观上面理解：每次找出最离谱的变量，使他满足靠谱的条件，直到所有的变量都靠谱了。

### 解析方法
- 目标函数 

假设选择$\alpha_1$和$\alpha_2$，其他变量固定，最优化问题转化为下面的形式： 


$\min_{\alpha_1,\alpha_2}\frac{1}{2}K_{11}\alpha_{1}^{2}+\frac{1}{2}K_{22}\alpha_{2}^{2}+y_{1}y_{2}K_{12}\alpha_1\alpha_2-(\alpha_1+\alpha_2)+y_{1}\alpha_{1}\sum_{i=3}^{N}y_{i}\alpha_{i}K_{i1}+y_{2}\alpha_{2}\sum_{i=3}^{N}y_{i}\alpha_{i}K_{i2}$


$\alpha_{1}y_{1}+\alpha_{2}y_{2}=-\sum_{i=3}^{N}y_i\alpha_i=\zeta$


$0\le\alpha_i\le C,i=1,2$

ps:第一个式子忽略了一个常数项。- 上下限剪裁 

初始可行解为$\alpha_1^{old}$和$\alpha_2^{old}$，最优解为$\alpha_1^{new}$和$\alpha_2^{new}$. 

假设$y_1\ne y_2, 0\lt\zeta\lt C$，有约束条件可知： 


$\alpha_2^{old}-\alpha_1^{old}=\alpha_2^{new}-\alpha_1^{new}$

考虑到$\alpha_1^{new}$满足$0\le\alpha_1^{new}\le C$，所以$\alpha_2^{new}$满足$\zeta\le\alpha_2^{new}\le C$
**一般来讲**，最优值$\alpha_2^{old}$的取值范围有了进一步的限制，表示如下： 


$L\le\alpha_2^{new}\le H$

当$y_1\ne y_2$，H和L取值如下： 


$L=max(0,\alpha_2^{old}-\alpha_1^{old}),H=min(C,C+\alpha_2^{old}-\alpha_1^{old})$

当$y_1= y_2$，H和L取值如下： 


$L=max(0,\alpha_2^{old}+\alpha_1^{old}-C),H=min(C,\alpha_2^{old}+\alpha_1^{old})$- 最优解 

沿约束方向未剪裁的解 


$\alpha_2^{new,unc}=\alpha_2^{old}+\frac{y_2(E_{1}-E_{2})}{\eta}$

其中 


$\eta=K_{11}+K_{22}-2K_{12}$


$E_i=(\sum_{j=1}^{N}\alpha_jy_jK(x_j,x_i)+b)-y_i$

剪裁以后的解变为 


$\alpha_2^{new} = \begin{cases}  H   &  {\alpha_2^{new,unc}\gt H} \\  \alpha_2^{new,unc} &  {L\le\alpha_2^{new,unc}\le H} \\  L   &  {\alpha_2^{new,unc}\le L} \\ \end{cases} $

根据$\alpha_2^{old}-\alpha_1^{old}=\alpha_2^{new}-\alpha_1^{new}$可以求得 


$\alpha_1^{new}=\alpha_1^{old}+y_1y_2(\alpha_2^{old}-\alpha_1^{old})$
### 选择变量
- 第一个变量 

SMO称选择第一个变量为外层循环，找出违反KKT条件最严重的样本点，对应KKT条件如下： 


$\alpha_i=0\Leftrightarrow y_ig(x_i)\ge1 \text {     间隔线以外正确分类情况}$


$0\lt\alpha_i\lt C\Leftrightarrow y_ig(x_i)=1 \text {     间隔线上的样本点}$


$\alpha_i=C\Leftrightarrow y_ig(x_i)\le1 \text {     两条间隔点之间的点}$

其中$g(x_i)=\sum_{j=1}^{N}\alpha_{j}y_{j}K(x_{i}, x_{j})+b$表示对于输入$x_i$的预测值。 

首先遍历间隔线上面的支持向量点$0\lt\alpha_i\lt C$，如果都满足，遍历整个训练集。- 第二个变量的选择 

称为内层循环。选择标准是能够使$\alpha_2$有足够大的变化（即加快收敛速度）。考虑到$\alpha_2^{new}$正比于$|E_1-E_2|$，可以根据该标准选择$\alpha_2$。 

如果选择$\alpha_2$不能使目标函数有足够的下降，那么遍历间隔边界的支持向量点，如果仍不合适，重新选择$\alpha_1$。- 最后的停机条件 


$\sum_{i=1}^{N}\alpha_iy_i=0$


$0\le\alpha_i\le C,i=1,2,...,N$


$\alpha_i=0\Leftrightarrow y_ig(x_i)\ge1$


$0\lt\alpha_i\lt C\Leftrightarrow y_ig(x_i)=1$


$\alpha_i=C\Leftrightarrow y_ig(x_i)\le1$












