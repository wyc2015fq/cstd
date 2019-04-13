
# 特征工程PCA降维方法的最大方差理论详解 - BrightLamp的博客 - CSDN博客


2018年12月25日 23:49:43[BrightLampCsdn](https://me.csdn.net/oBrightLamp)阅读数：76



## 摘要
本文给出 PCA最大方差理论求解方法.
## 相关
*配套代码, 请参考文章 :*
纯Python和scikit-learn对比实现PCA特征降维
*系列文章索引 :*
[https://blog.csdn.net/oBrightLamp/article/details/85067981](https://blog.csdn.net/oBrightLamp/article/details/85067981)
## 正文
## 1. 降维方法
对于给定的一组数据点, 以矩阵$X_{m \times n}$表示 :
$$
X = (X_1, X_2,\cdots,X_m)^T
$$
$X_i$为行向量, 中心化后的表示为 :
$$
M_{m \times n}= (X_1-\mu_1,X_2-\mu_2,X_3-\mu_3,\cdots,X_m-\mu_m)^T
$$
其中$\mu_i$是$X_i$的平均值
$$
\mu_i=mean(X_i) = \frac{1}{n}\sum_{j = 1}^{j = n}x_{ij}
$$
行向量$M_i$的均值
$$
\mu&\#x27;_i = mean(M_i) = \frac{1}{n}\sum_{j = 1}^{j = n}m_{ij}= \frac{1}{n}\sum_{j = 1}^{j = n}(x_{ij} - \mu_i) = 0
$$
行向量$M_i$在单位行向量$w$方向上的长度
$$
l_i = M_i\cdot w
$$
$l_i$是标量, 符号$\cdot$表示点积, 投影后的方差 (标量) 可表示为
$$
s = \frac{1}{n}\sum_{i=1}^{n}{(l_i-\mu&\#x27;_i) ^2}= \frac{1}{n}\sum_{i=1}^{n}{l_i^2}= \frac{1}{n}\sum_{i=1}^{n}{(M_i\cdot w)^2}
$$
按照 PCA 的最大方差理论, 我们需要求得一个行向量$w$使得$s$的值最大.
使用向量乘法表示$s$:
$$
s = \frac{1}{n}\sum_{i=1}^{n}{(M_i\cdot w)(M_i\cdot w)}=\frac{1}{n}\sum_{i=1}^{n}{(w M_i^T)(M_iw^T)}
$$
向量乘法满足结合律 :
$$
s =\frac{1}{n}\sum_{i=1}^{n}{w(M_i^T M_i)w^T}
$$
向量乘法满足分配率 :
$$
s =w\sum_{i=1}^{n}(\frac{1}{n}M_i^T M_i)w^T
$$
令 :
$$
A =\sum_{i=1}^{n}(\frac{1}{n}M_i^TM_i)
$$
则 :
$$
s =w A w^T
$$
A 是样本的协方差矩阵. 若存在特征值$\lambda$(标量), 使得:
$$
Aw = \lambda w
$$
则 :
$$
s =\lambda w^T w = \lambda \cdot1 = \lambda
$$
问题简化为如何求 A 的最大特征值.
若矩阵 A 有多个特征值, 将这些特征值从大到小排序, 取其中的前 d 个特征值所对应的行向量$V_i$组成变换矩阵 :
$$
V_{d\times n}=(V_1,V_2,V_3,\cdots,V_d)^T
$$
则矩阵$X_{m \times n}$经 PCA 降维后的矩阵为$Z_{m \times d}$
$$
Z_{m\times d} = M_{m\times n}V_{d\times n}^T
$$
本文仅给出半推导半描述的过程, 用于理解PCA降维的理论和计算机程序编写已经足够.
推导过程参考自《百面机器学习》一书, 更严格的数学证明请参考其他资料.
## 2. 降维损失
PCA 降维是一种有损压缩的方法, 《百面机器学习》中使用特征值$\lambda$定义降维后的信息占比:
$$
\eta = \sqrt{\frac{\sum_{i=1}^{d}\lambda_i^2}{\sum_{i=1}^{n}\lambda_i^2}}
$$
本文作者使用纯Python 和 scikit-learn 对比实现 PCA 降维方法后, 发现 scikit-learn 中 PCA 类的参数 n_components 使用下式定义降维后的信息占比 :
$$
\gamma = \eta^2=\frac{\sum_{i=1}^{d}\lambda_i^2}{\sum_{i=1}^{n}\lambda_i^2}
$$
相关的数值保存在 explained_variance_ratio_ 属性中.
## 3. 编程提示
### 3.1 特征向量方向
从特征值的定义可知 :
$$
Aw = \lambda w=-\lambda(-w)
$$
不同的函数库对特征值的求解方法不同, 最后得到的特征向量, 可能出现长度一致但方向相反的情况.
根据 PCA 降维方法的理论和机制, 我们的目的是求解一个降维的拟合平面, 对组成该降维平面向量方向并无要求, 只要在同一直线上即可.
因此, 降维向量出现数值一致, 正负符号相反的情况并不影响 PCA 方法的正常使用.
### 3.2 numpy 特征向量的求解函数
numpy 的 svd 方法会自动对输入的矩阵$M$进行$M^TM$变换后再求特征值, 并自动将特征值从大到小排序.
numpy 的 eig 方法返回的特征值不排序, 需要自行处理.
推荐使用 svd 方法.

