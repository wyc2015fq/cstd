# 主成分分析（PCA）推导过程及简单实例 - 技术分享 - CSDN博客

置顶2018年11月14日 17:40:13[丿一叶秋丶](https://me.csdn.net/zhuqiang9607)阅读数：748


## 介绍

主成分分析（Principal Component Analysis，PCA）是一种常用的数据降维算法，可以将高维度的数据降到低维度，并且保留原始数据中最重要的一些特征，同时去除噪声和部分关联特征，从而提高数据的处理效率，降低时间成本。此外，在机器学习中，训练样本固定的情况下，特征维数增加到某个临界点后，再增加维数，反而会降低模型的预测能力（休斯现象，Hughes Phenomenon）。

**降维的必要性：**
- 机器学习是建立在统计学习上的，而数据稀疏（维度过高的情况下）对于统计学而言是一个问题
- 有大量模型需要依靠样本之间的相似度才能被判定，而多数时候样本的相似度由其在特征空间的相互距离决定，这使得样本密度影响了样本的属性
- 维度过多导致计算量增大，影响机器学习效率

**数据降维优点：**
- 增大了样本的采样密度
- 低维数据更容易使用
- 数据之间相互独立
- 减少了变量的个数
- 提高了算法的效率
- 去除了噪声
- 结果的解释性更强

在PCA中，数据从原来的坐标系转换到新的坐标系，在新的坐标系中要求数据之间的协方差为0（即不同维度之间的数据线性不相关），而方差尽可能的大。因此，第一坐标轴是原始数据中方差最大的方向，第二个坐标轴是与第一个新坐标轴正交且方差次大的方向，重复该过程，直到需要的降维数。在详细讲解PCA之前，有必要先介绍协方差。

## 协方差

统计学里最基本的概念有均值、方差、标准差，假设有n个样本：$X_{1},X_{2},...,X_{n}$，那么

均值：$\overline{X}=\frac{\sum_{i=1}^{N}X_{i}}{N}$

方差：$S^{2}=\frac{\sum_{i=1}^{N}(X_{i}-\overline{X})^2}{N-1}$

标准差：$S=\sqrt{\frac{\sum_{i=1}^{N}(X_{i}-\overline{X})^2}{N-1}}$

其中，方差和标准差是描述一维数据的离散程度，但现实中的数据往往是多维的，比如说学生的成绩，不一定只和学习时间有关，还涉及到学习效率，学习状态等等，这时便引入了协方差。协方差定义：

$Cov(X,Y)=\frac{\sum_{i=1}^{N}(X_{i}-\overline{X})(Y_{i}-\overline{Y})}{N-1}$
- $Cov(X,Y)>0$，X和Y是正相关关系
- $Cov(X,Y)<0$，X和Y是负相关关系
- $Cov(X,Y)>0$，X和Y是线性不相关，但是不一定相互独立哦（$sinx$和$cosx$线性不相关，但是它们的平方是有关系的，$sin^{2}x+cos^{2}x=1）$

**不过有的协方差用的是：**

$Cov(X,Y)=\frac{\sum_{i=1}^{N}(X_{i}-\overline{X})(Y_{i}-\overline{Y})}{N}$（除以$N$结果会偏小，得除以$N-1$才能得到**无偏估计**，可以参考“[为什么样本方差（sample variance）的分母是 n-1？](https://www.zhihu.com/question/20099757?sort=created)”）

数据如果有三维，协方差矩阵为：

$C=\begin{bmatrix} cov(x,x)& cov(x,y) &  cov(x,z)\\  cov(y,x) &cov(y,y)  & cov(y,z)\\  cov(z,x)&  cov(z,y)&cov(z,z) \end{bmatrix}$

## PCA步骤

假设有m个样本数据，每个数据是n维的，按列组成矩阵$X_{nm}$，则PCA步骤如下：
- 均值化矩阵$X_{nm}$，得到$X=X_{nm}-\overline{X}_{nm}$
- 求出协方差矩阵$C=\frac{1}{n-1}XX^T$
- 求出协方差矩阵$C$的特征值$\lambda_{i}$和特征向量$w_{i}$
- 选取$k$个最大的特征值对应的特征向量$w_{1},w_{2},...,w_{k}$，组成矩阵$W_{kn}$（特征值选择方法：$\frac{\sum_{i=1}^{k}\lambda_{i}}{\sum_{i=1}^{n}\lambda_{i}}\ge t$，$t$越大保留的特征值越多，按实际要求）
- 降维矩阵$Y_{km} = W_{kn}X_{nm}$

## PCA实例

假设：
$X_{2*5}=\begin{bmatrix} 2& 2 &  3& 5&  3\\ -1&1 & 1& 2&  2\end{bmatrix}$
- 均值化：$X=X_{2*5}-\overline{X}_{2*5}=\begin{bmatrix}-1& -1 &  0& 2&  0\\ -2&0& 0& 1&  1\end{bmatrix}$
- 协方差矩阵：$C=\frac{1}{5-1}XX^T=\begin{bmatrix}\frac{3}{2}& 1 \\ 1& \frac{3}{2}\end{bmatrix}$
- 特征值为：$\lambda_{1}=\frac{5}{2},\lambda_{2}=\frac{1}{2}$，对应的特征向量$w_{1}=\begin{bmatrix}1 \\ 1\end{bmatrix},w_{2}=\begin{bmatrix}1 \\ -1\end{bmatrix}$，特征向量单位化$w_{1}'=\begin{bmatrix}\frac{1}{\sqrt{2}} \\ \frac{1}{\sqrt{2}} \end{bmatrix},w_{2}'=\begin{bmatrix}\frac{1}{\sqrt{2}}  \\ -\frac{1}{\sqrt{2}} \end{bmatrix}$
- 按照特征值大小排序，选择最大的部分特征，这里选取$\lambda_{1}$，此时矩阵$W_{1*2}=\begin{bmatrix}\frac{1}{\sqrt{2}} &\frac{1}{\sqrt{2}} \end{bmatrix}$
- 降维矩阵$Y_{1*5}= W_{1*2}X_{2*5}=\begin{bmatrix}\frac{1}{\sqrt{2}} &\frac{1}{\sqrt{2}} \end{bmatrix}\begin{bmatrix}-1& -1 &  0& 2&  0\\ -2&0& 0& 1&  1\end{bmatrix}=\begin{bmatrix}-\frac{3}{\sqrt{2}} &-\frac{1}{\sqrt{2}} &0&\frac{3}{\sqrt{2}} &\frac{1}{\sqrt{2}}\end{bmatrix}$

