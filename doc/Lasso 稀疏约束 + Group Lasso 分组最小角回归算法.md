# Lasso 稀疏约束 + Group Lasso 分组最小角回归算法

 																				2017年10月31日 12:01:53 			[小鹅鹅](https://me.csdn.net/asd136912) 

### 背景

> Lasso（least absolute shrinkage and selection operator，又译最小绝对值收敛和选择算子、套索算法）是一种同时进行特征选择和正则化（数学）的回归分析方法，旨在增强统计模型的预测准确性和可解释性，最初由[斯坦福大学](https://www.baidu.com/s?wd=%E6%96%AF%E5%9D%A6%E7%A6%8F%E5%A4%A7%E5%AD%A6&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)统计学教授Robert  Tibshirani于1996年基于Leo Breiman的非负参数推断(Nonnegative Garrote,  NNG)提出。Lasso算法最初用于计算最小二乘法模型，这个简单的算法揭示了很多估计量的重要性质，如估计量与岭回归（Ridge  regression，也叫Tikhonov  regularization）和最佳子集选择的关系，Lasso系数估计值(estimate)和软阈值（soft  thresholding）之间的联系。它也揭示了当协变量共线时，Lasso系数估计值不一定唯一（类似标准线性回归）。

### 范数(norm)定义

首先需要了解一下几种向量范式的定义： 
 0-范数：即向量[中非](https://www.baidu.com/s?wd=%E4%B8%AD%E9%9D%9E&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)零元素的个数 $\|w\|_0=\sum_i 1(w_i \neq 0)$

 1-范数：即向量元素绝对值之和，matlab中可以调用函数norm(x, 1)  $\|w\|_1=\sum_{i}|w_i|$

 2-范数：Euclid范数（欧几里得范数，常用计算向量长度），即向量元素绝对值的平方和再开方，matlab中可以调用函数norm(x, 2)  $\|w\|_2=(\sum_{i}{w_i}^{2})^{1/2}$

 p-范数：即向量元素绝对值的p次方和的1/p次幂，matlab中可以调用函数norm(x, p)  $\|w\|_p=(\sum_{i}{w_i}^{p})^{1/p}$

### Ridge Regression 岭回归

在这里先简单地引入一些Ridge Regression岭回归 
 在线性回归中，我们需要对代价函数Cost Function J 最小化拟合训练集: 
$$
J=\frac{1}{m}\sum_{i=1}^{m}(y_i - f(x_i))^2=\frac{1}{m}\|y-Xw\|^2
$$
 岭回归，就是在线性回归的基础上加上l2-norm的约束。为了之后推导方便改成了1/2，因为是求代价函数最小值w所以并不改变结果。 
$$
J_R=\frac{1}{2}\|y-Xw\|^2+\frac{\lambda}{2}\|w\|^2
$$
 其中λ是正则项(惩罚系数)，对w的模做约束，使得它的数值会比较小，很大程度上减轻了overfitting过拟合的问题。通过求解可以得出w 
$$
\hat{w}_R = (X^{T}X+\lambda I)^{-1}X^{T}y
$$
 我们也可以通过下面的优化目标形式表达: 
$$
\min_{w} \frac{1}{2}\|y-Xw\|^2, \quad s.t. \|w\|_2<\theta
$$
 上面两种优化形式是等价的，我们可以找到相对应的λ和θ。



### The Least Absolute Shrinkage and Selection Operator(Lasso) 稀疏约束

在前面的ridge  regression中，对w做2范数约束，就是把解约束在一个l2-ball里面，放缩是对球的半径放缩，因此w的每一个维度都在以同一个系数放缩（如下图），通过放缩不会产生稀疏的解——即某些w的维度是0。在实际应用中，数据的维度中是存在噪音和冗余的，稀疏的解可以找到有用的维度并且减少冗余，提高回归预测的准确性和鲁棒性（减少了overfitting）。在压缩感知、稀疏编码等非常多的机器学习模型中都需要用到LASSO稀疏约束。

稀疏约束最直观的形式应该是范数0，如上面的范数介绍，w的0范数是求w中非零元素的个数。如果约束∥w∥0≤k，就是约束非零元素个数不大于k。不过很明显，0范式是不连续的且非凸的，如果在线性回归中加上0范式的约束，就变成了一个组合优化问题：挑出≤k个系数然后做回归，找到目标函数的最小值对应的系数组合，是一个NP问题。

不过l1-norm（1范数）也可以达到稀疏的效果，是0范数的最优凸近似，更重要的是1范式容易求解，并且是凸的，所以几乎看得到稀疏约束的地方都是用的1范式，从而可以引出The  Least Absolute Shrinkage and Selection Operator(Lasso) 稀疏约束。

LASSO是另一种缩减方法，将回归系数收缩在一定的区域内。LASSO的主要思想是构造一个一阶惩罚函数获得一个精炼的模型, 通过最终确定一些变量的系数为0进行特征筛选。 
$$
\min_{w} \frac{1}{2}\|y-Xw\|^2, \quad s.t. \|w\|_1<\theta \\
J_L=\frac{1}{2}\|y-Xw\|^2+\lambda\sum_{i}|w_i|
$$
![img](https://img-blog.csdn.net/20171031133051692?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYXNkMTM2OTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

椭圆和蓝色的区域（惩罚函数）的切点就是目标函数的最优解，我们可以看到，如果蓝色区域是圆，则很容易切到圆周的任意一点，但是很难切到坐标轴上，这样就得不出稀疏的借，冗余数据就会相对较多；但是如果蓝色区域是菱形或者多边形，则很容易切到坐标轴上，因此很容易产生稀疏的结果。这也说明了为什么1范式会是稀疏的。

虽然惩罚函数只是做了细微的变化，但是相比岭回归可以直接通过矩阵运算得到回归系数相比，LASSO的计算变得相对复杂。由于惩罚项中含有绝对值，此函数的导数是连续不光滑的，所以无法进行求导并使用梯度下降优化。这个时候需要使用subgradient次梯度….[论证过程](http://freemind.pluskid.org/machine-learning/sparsity-and-some-basics-of-l1-regularization/)….最终可以得出 
$$
\bar{w}^j=\text{sgn}(w^{*j})(|w^{*j}|-\lambda)_+\\
$$


![img](https://img-blog.csdn.net/20150319195042160)

从图上可以看出岭回归实际上就是做了一个放缩，而lasso实际是做了一个soft thresholding，把很多权重项置0了，所以就得到了稀疏的结果。

### Group Lasso 分组最小角回归算法

Yuan在2006年将lasso方法推广到group上面，诞生了group  lasso。我们可以将所有变量分组，然后在目标函数中惩罚每一组的L2范数，这样达到的效果就是可以将一整组的系数同时消成零，即抹掉一整组的变量，这种手法叫做Group  Lasso 分组最小角回归算法。其目标函数为： 
$$
\min_\beta(||Y-X\beta||_2^2+\lambda{\sum_{g = 1}^G}||\sqrt {ql}\beta_{I_{g}}||_2)
$$
在group lasso中，将p个特征分成G组，其中i的取值为1，2..g.. G。$I_g$是g组的特征下标， $\sqrt {ql}$是每一组的加权，可以按需调节。不同于Lasso 方法将每个特征的系数项的绝对值加总， 这里所加总的是每个组系数的 L2 范数，在优化的过程中，该结构尽量选出更少的组（组间稀疏），而组内是L2范数，稀疏约束没那么强。

容易看出，group lasso是对lasso的一种推广，即将特征分组后的lasso。显然，如果每个组的特征个数都是1，则group  lasso就回归到原始的lasso。为了求解group lasso，  可以首先假设组内特征是正交的，针对这种情形可以利用分块坐标下降法求解，对于非正交的情形，可以首先对组内特征施加正交化。

### 参考文献

1. <http://statweb.stanford.edu/~tibs/ftp/sparse-grlasso.pdf>
2. <ftp://ess.r-project.org/Teaching/buhlmann/advanced-comput-statist/slides1.pdf>
3. <http://blog.csdn.net/xbinworld/article/details/44276389>
4. <http://freemind.pluskid.org/machine-learning/sparsity-and-some-basics-of-l1-regularization/>
5. <http://blog.csdn.net/infskyline/article/details/47296701>
6. <https://www.zhihu.com/question/38121173/answer/166238142#showWechatShareTip>
7. <https://ask.hellobi.com/blog/mlanddlanddm/7198>
8. 周志华,机器学习, Chapter 11 P252